
#include "TimeDomainSimulation.h"
#include "../CommonFunction/ErrorInfo.h"
#include "../SpareMatrixLib/ComplexLDUSolver.h"
#include <conio.h>

TDSIMULATION::TDSIMULATION()
{
	pDyn = NULL;
	OutputValue = NULL;
	OutputTotal = 0;
	OutputSpace = 0;
	OutputCount = 0;
}

void TDSIMULATION::FreeSpace()
{
	if (OutputSpace > 0)
		FreeArray(OutputValue);
}
void TDSIMULATION::SetDynModel(DYNAMICMODELINFO*tDyn)
{
	pDyn = tDyn;
	m_Solver = makeLDUSolver();
	m_Solver->InitMatrix(&pDyn->m_DynMatrix);
	BusTotal = pDyn->iGetBusTotal();
	pVx = pDyn->Vx;
	pVy = pDyn->Vy;
	pIx = pDyn->Ix;
	pIy = pDyn->Iy;
	int i;
	for (i = 0; i < pDyn->DynamicModelTotal; i++)
	{
		OutputTotal += pDyn->DynamicModel[i]->GetOutputCount();
	}
	MallocOutputSpace((pDyn->TotalTime / pDyn->Step + 20)*OutputTotal);
}

int TDSIMULATION::CheckInitalCalcul()
{
	if (pDyn->bDynInitial == 0)
		pDyn->DynInitial();
	if (pDyn->bDynInitial != 1)
	{
		sprintf(ErrorMessage[0], "ERROR: 模型初始化失败");
		cpGetErrorInfo()->PrintError(1);
		return 0;
	}
	int i;
	real *tIx0 = NULL, *tIy0 = NULL;
	MallocNew(tIx0, real, BusTotal);
	MallocNew(tIy0, real, BusTotal);
	memcpy(tIx0, pVy, BusTotal*sizeof(real));
	memcpy(tIy0, pVx, BusTotal*sizeof(real));
	m_Solver->MatrixVectorMultiply(tIx0, tIy0);

	memset(pIx, 0, BusTotal*sizeof(real));
	memset(pIy, 0, BusTotal*sizeof(real));
	for (i = 0; i < pDyn->iGetEquipTotal(); i++)
	{
		if (pDyn->cpGetEquip(i)->GetEquipModel() != NULL)
		{
			pDyn->cpGetEquip(i)->GetEquipModel()->DynCalculateCurrent();
		}
	}
#ifdef _DEBUG
	int DebugInitInputCurrent = 0;
	if (DebugInitInputCurrent == 1)
	{
		FILE *fp = NULL;
		OpenFile(fp, "DynInitInputCurrent.dbg", "w");
		int i;
		for (i = 0; i<BusTotal; i++)
		{
			fprintf(fp, "%7d: %22.16f%22.16f%22.16f%22.16f   %s\n",
				i + 1, pIx[i], pIy[i], tIx0[i], tIy0[i],
				pDyn->cpGetBus(i)->GetBusName());
		}
		CloseFile(fp);
	}
#endif
	real err;
	for (i = 0; i < BusTotal; i++)
	{
		err = fabs(pIx[i] - tIx0[i]);
		if (err > 0.01)
		{
			sprintf(ErrorMessage[0], "节点'%s'注入电流不平衡，计算值Ix=%f，Iy=%f，实际应为Ix=%f，Iy=%f",
				pDyn->cpGetBus(i)->GetBusName(),
				pIx[i], pIy[i], tIx0[i], tIy0[i]);
			cpGetErrorInfo()->PrintWarning(11, 1);
		}
	}
	cpGetErrorInfo()->CheckMessageType(11);
	return 1;
}

int TDSIMULATION::Calculate(DYFAULTINFO*pFault)
{
	int i;
	for (i = 0; i < pFault->iFaultListNo; i++)
	{
		pFault->pFaultList[i]->SetDynInfo(pDyn);
	}
	int flag_conv_diff = 0, flag_conv_alg = 0, iter;
	int flag;
	flag = CheckInitalCalcul();
	if (flag < 0)
	{
		return 0;
	}

	if(pDyn->Step<0.0001)
		pDyn->Step= 0.01f;
	pDyn->Tnow = 0;
	pDyn->NextEvent = 999.;
	SaveOutputValue(pDyn->Tnow);

	float tNextTime;
	real err;
	while (1)
	{
		if (pFault->GetNextTime() < pDyn->Tnow + 0.00001)
		{
			flag = pFault->ProcessFault(pDyn->Tnow);
			m_Solver->ReSetMatrixElement();
			pDyn->Step = 0.f;
		}
		else{
			pDyn->Step = 0.01f;
			tNextTime = pFault->GetNextTime();
			if (tNextTime < pDyn->NextEvent)tNextTime = pDyn->NextEvent;
			if (tNextTime < pDyn->TotalTime)tNextTime = pDyn->TotalTime;
			if (tNextTime < pDyn->Tnow + pDyn->Step + 0.00001)
			{//重新设置步长
				pDyn->Step = tNextTime - pDyn->Tnow;
			}
		}
		pDyn->Tnow += pDyn->Step;
		for (i = 0; i < pDyn->DynamicModelTotal; i++)
		{
			pDyn->DynamicModel[i]->DynBeforeStep();
		}
		iter = 0;
		while (1)
		{
			flag_conv_diff = 0;
			for (i = 0; i < pDyn->DynamicModelTotal; i++)
			{
				err = pDyn->DynamicModel[i]->DynProcessStep();
				if (err>0.01)
				{
					flag_conv_diff++;
				}
			}
			flag_conv_alg = 0;
			memset(pIx, 0, BusTotal*sizeof(real));
			memset(pIy, 0, BusTotal*sizeof(real));
			for (i = 0; i < pDyn->iGetEquipTotal(); i++)
			{
				if (pDyn->cpGetEquip(i)->GetEquipModel() != NULL)
				{
					err = pDyn->cpGetEquip(i)->GetEquipModel()->DynCalculateCurrent();
					if (err>0.01)
					{
						flag_conv_alg++;
					}
				}
			}
#ifdef _DEBUG
			int DebugInputCurrent = 0;
			if (DebugInputCurrent == 1)
			{
				FILE *fp = NULL;
				OpenFile(fp, "DynInputCurrent.dbg", "w");
				int i;
				for (i = 0; i<BusTotal; i++)
				{
					fprintf(fp, "%7d: %22.16f%22.16f   %s\n",
						i + 1, pIx[i], pIy[i],
						pDyn->cpGetBus(i)->GetBusName());
				}
				CloseFile(fp);
			}
#endif
			memcpy(pVx, pIy, BusTotal*sizeof(real));
			memcpy(pVy, pIx, BusTotal*sizeof(real));
			m_Solver->LinearEquationCal(pVy, pVx);
#ifdef _DEBUG
			int DebugVoltage = 0;
			if (DebugVoltage == 1)
			{
				FILE *fp = NULL;
				OpenFile(fp, "DynVoltage.dbg", "w");
				int i;
				for (i = 0; i<BusTotal; i++)
				{
					fprintf(fp, "%7d: %22.16f%22.16f   %s\n",
						i + 1, pVx[i], pVy[i],
						pDyn->cpGetBus(i)->GetBusName());
				}
				CloseFile(fp);
			}
#endif
			iter++;
			if (flag_conv_diff==0 && flag_conv_alg==0 && iter > 2)
			{
				break;
			}
			if (iter >= pDyn->MaxIterNumber)
			{
				break;
			}
		}
		for (i = 0; i < pDyn->DynamicModelTotal; i++)
		{
			pDyn->DynamicModel[i]->DynAfterStep();
		}
		SaveOutputValue(pDyn->Tnow);
		if (iter < pDyn->MaxIterNumber)
		{
			sprintf(ErrorMessage[0], " Iteration succeeded at t=%12.6f, h=%6.5f iter=%d\n",
				pDyn->Tnow, pDyn->Step, iter);
		}
		else
		{
			sprintf(ErrorMessage[0], " Iteration failed at t=%12.6f, h=%6.5f iter=%d\n",
				pDyn->Tnow, pDyn->Step, iter);
		}
		cpGetErrorInfo()->PrintWarning(-1, 1);
		if (pDyn->Tnow > pDyn->TotalTime - 0.00001)
		{
			break;
		}
		if (_kbhit())
		{
			char c;
			c = _getch();
			if (c != 13)
			{
				printf("按一次空格键暂停计算；按两次空格键退出计算；按其它键继续计算。\n");
				printf("T=%8.4f H=%10.7f\n", pDyn->Tnow, pDyn->Step);
				if (c == 32)
				{
					c = _getch();
					if (c == 32){ printf("未到仿真总时间,停止计算\n"); break; }
				}
			}
		}
		if (pDyn->FlagExit == 'Y')  break;
	}
	return 1;
}

int TDSIMULATION::MallocOutputSpace(int space)
{
	if (OutputSpace > 0)
	{
		real *OldValue;
		OldValue = OutputValue;
		OutputValue = NULL;
		MallocNew(OutputValue, real, OutputSpace + space);
		memcpy(OutputValue, OldValue, OutputSpace*sizeof(real));
		FreeArray(OldValue);
		OutputSpace = OutputSpace + space;
	}
	else{
		MallocNew(OutputValue, real, space);
		OutputSpace = space;
	}
}

void TDSIMULATION::SaveOutputValue(real time)
{
	if (OutputCount + OutputTotal + 1 >= OutputSpace)
	{
		MallocOutputSpace(((pDyn->TotalTime - time) / pDyn->Step + 20)* OutputTotal);
	}
	OutputValue[OutputCount++] = time;
	int i, j, count;
	for (i = 0; i < pDyn->DynamicModelTotal; i++)
	{
		count = pDyn->DynamicModel[i]->GetOutputCount();
		for (j = 0; j < count; j++)
		{
			OutputValue[OutputCount++] = pDyn->DynamicModel[i]->GetOutputValue(j);
		}
	}
}

void TDSIMULATION::WriteCurve(char*file)
{
	FILE*fp;
	fp = NULL;
	OpenFile(fp, file, "w+");
	fprintf(fp, "时间");
	char Name[_MaxNameLen];
	int i, j, count;
	for (i = 0; i < pDyn->DynamicModelTotal; i++)
	{
		count = pDyn->DynamicModel[i]->GetOutputCount();
		for (j = 0; j < count; j++)
		{
			pDyn->DynamicModel[i]->GetOutputName(j, Name);
			fprintf(fp, ",%s", Name);
		}
	}
	for (i = 0; i < OutputCount; i++)
	{
		if (i % (OutputTotal + 1) == 0)
		{
			fprintf(fp, "\n%f", OutputValue[i]);
			continue;
		}
		fprintf(fp, ",%f", OutputValue[i]);
	}
}


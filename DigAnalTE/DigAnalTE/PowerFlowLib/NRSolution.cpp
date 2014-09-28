
#include "NRSolution.h"
#include "../NetWorkInfo/NetWorkInfo.h"
#include "../CommonFunction/ErrorInfo.h"

PFSOLUTION_BASE* makeNRSolver(NETWORKINFO* temp)
{
	PFSOLUTION_BASE* tEngine;
	tEngine = new NRSOLUTION(temp);
	return tEngine;
}

int PowerFlowCal(NETWORKINFO* tNetWorkInfo, PFCalPara tCalPara)
{
	PFSOLUTION_BASE* tPFCal = makeNRSolver(tNetWorkInfo);
	if (tPFCal->Calculate(tCalPara) != 1)
	{
		return 0;
	}
	return 1;
}

int PowerFlowCal(NETWORKINFO* tNetWorkInfo)
{
	PFCalPara tCalPara;
	tCalPara.MaxIter = 50;
	tCalPara.BusPMaxErr = 0.005f;
	tCalPara.BusQMaxErr = 0.005f;
	PFSOLUTION_BASE* tPFCal = makeNRSolver(tNetWorkInfo);
	if (tPFCal->Calculate(tCalPara) != 1)
	{
		return 0;
	}
	return 1;
}

NRSOLUTION::NRSOLUTION(NETWORKINFO* temp) :PFSOLUTION_BASE(temp)
{
	RightP = NULL; RightQ = NULL;
	IsInit = 0;
	m_Solver = makeLDUSolver();
}

void NRSOLUTION::InitPFlow()
{
	FreeArray(RightP); FreeArray(RightQ);

	MAXITER = 100; MAXPERR = MAXQERR = 0.005f;
	IsInit = pNet->m_ChangeCode;

	NRSBusTotal = pNet->iGetBusTotal();
	Volt = pNet->BusVolt; Sita = pNet->BusSita;
	MallocNew(RightP, real, NRSBusTotal);
	MallocNew(RightQ, real, NRSBusTotal);
	m_Solver->InitMatrix(pNet->m_Matrix);
}

int NRSOLUTION::subCalculate(PFCalPara para)
{
	if (IsInit == 0 || IsInit != pNet->m_ChangeCode)
	{
		InitPFlow();
	}
	int i, flag;
	MAXITER = para.MaxIter;
	MAXPERR = para.BusPMaxErr;
	MAXQERR = para.BusQMaxErr;
	sprintf(ErrorMessage[0], "    ��������  �й����     �޹����  �������ڵ���");//��ӡ��Ϣ
	sprintf(ErrorMessage[1], "      ITER      DPMAX        DQMAX    NoConverge");//��ӡ��Ϣ
	cpGetErrorInfo()->PrintWarning(-1, 2);

	//��������
	int tIter, tNConverge;
	for (tIter = 1; tIter <= MAXITER; tIter++)
	{
		flag = 1;
		flag = pNet->BeforeIterCal(tIter);
		if (flag != 1)
		{
			break;
		}

		FormJacobi();

		tNConverge = 0;// !�ڵ��ѹ���>Tolerance �Ľڵ����
		double tMaxPerr, tMaxQerr, tPerr, tQerr;
		tMaxPerr = 0.0;
		tMaxQerr = 0.0;
		for (i = 0; i<NRSBusTotal; i++)
		{
			tPerr = fabs(RightP[i]);
			tQerr = fabs(RightQ[i]);
			tMaxPerr = maxv(tMaxPerr, tPerr);
			tMaxQerr = maxv(tMaxQerr, tQerr);
			if (tPerr>MAXPERR || tQerr > MAXQERR)
			{
				tNConverge++;
			}
		}
		sprintf(ErrorMessage[0], "      %3d%13.5f%13.5f%10d", tIter, tMaxPerr, tMaxQerr, tNConverge);//��ӡ��Ϣ
		cpGetErrorInfo()->PrintWarning(-1, 1);
		flag = m_Solver->LinearEquationCal(RightP, RightQ);
		if (flag != 1)break;
		int i;
		for (i = 0; i < NRSBusTotal; i++)//Nodal voltage and angle adjustments
		{
			Volt[i] = (1 + RightQ[i])*Volt[i];
			Sita[i] += RightP[i];
		}
		//�ж��Ƿ�����
		if (tIter>2 && tNConverge == 0)//���ȸ��ݸ�ĸ�߷����Ƿ�����
		{
			sprintf(ErrorMessage[0], "    =================��������=================");//��ӡ��Ϣ
			cpGetErrorInfo()->PrintWarning(-1, 1);
			return 1;
		}
	}
	sprintf(ErrorMessage[0], "    XXXXXXXXXXXXXXXXX����������XXXXXXXXXXXXXXXXX");//��ӡ��Ϣ
	cpGetErrorInfo()->PrintWarning(-1, 1);
	return 0;
}

void NRSOLUTION::FormJacobi()
{
	double dSita, deltaP, deltaQ, GIJ, BIJ;
	int iI, iJ, II, tJacType, iStart, iSize;
	double ElementH, ElementN;
	double ElementJ, ElementL;

	for (iI = 0; iI < NRSBusTotal; iI++)
	{
		iStart = pNet->m_Matrix->IA[iI];
		iSize = pNet->m_Matrix->NA[iI];
		tJacType = pNet->JacType[iI];
		II = 0;
		deltaP = 0;
		deltaQ = 0;
		int j;
		for (j = iStart; j < iStart + iSize; j++)
		{
			iJ = pNet->m_Matrix->JA[j];
			if (iJ == iI)//���Խ�Ԫ����λ��
			{
				II = j;
				continue;
			}
			GIJ = pNet->m_Matrix->VG[j];
			BIJ = pNet->m_Matrix->VB[j];
			dSita = Sita[iI] - Sita[iJ];
			deltaP += Volt[iI] * Volt[iJ] * (GIJ*cos(dSita) + BIJ*sin(dSita));
			deltaQ += Volt[iI] * Volt[iJ] * (GIJ*sin(dSita) - BIJ*cos(dSita));
			ElementH = Volt[iI] * Volt[iJ] * (GIJ*sin(dSita) - BIJ*cos(dSita));
			ElementN = Volt[iI] * Volt[iJ] * (GIJ*cos(dSita) + BIJ*sin(dSita));
			ElementJ = -Volt[iI] * Volt[iJ] * (GIJ*cos(dSita) + BIJ*sin(dSita));
			ElementL = Volt[iI] * Volt[iJ] * (GIJ*sin(dSita) - BIJ*cos(dSita));
			if (tJacType == 2)//���ƽڵ�
			{
				ElementJ = 0; ElementL = 0;
			}
			if (tJacType == 3 || tJacType < 0)//���ƽڵ�
			{
				ElementH = 0; ElementN = 0; ElementJ = 0; ElementL = 0;
			}
			if (pNet->JacType[iJ] != 1)
			{
				ElementN = 0; ElementL = 0;
			}
			if (pNet->JacType[iJ] == 3)
			{
				ElementH = 0; ElementJ = 0;
			}
			m_Solver->SetElement(j, ElementH, ElementJ, ElementN, ElementL);
		}
		GIJ = pNet->m_Matrix->VG[II] + pNet->PZnet[iI];
		BIJ = pNet->m_Matrix->VB[II] + pNet->QZnet[iI];
		ElementH = -deltaQ;
		ElementJ = deltaP;
		deltaP += Volt[iI] * Volt[iI] * GIJ - pNet->PInet[iI] * Volt[iI];
		deltaQ += -Volt[iI] * Volt[iI] * BIJ - pNet->QInet[iI] * Volt[iI];
		ElementN = (deltaP + Volt[iI] * Volt[iI] * GIJ);
		ElementL = (deltaQ - Volt[iI] * Volt[iI] * BIJ);
		RightP[iI] = pNet->PPGen[iI] + pNet->PPnet[iI] + pNet->PInst[iI] - deltaP + pNet->PLcut[iI];//deltaP ���ʲ�
		RightQ[iI] = pNet->QPGen[iI] + pNet->QPnet[iI] + pNet->QInst[iI] - deltaQ + pNet->QLcut[iI];
		if (tJacType == 2)//���ƽڵ�
		{
			ElementN = 0; ElementJ = 0; ElementL = 1;
			RightQ[iI] = 0;
			pNet->QPGen[iI] = (real)(deltaQ - pNet->QPnet[iI] - pNet->QInst[iI] - pNet->QLcut[iI]);
		}
		if (tJacType == 3 || tJacType < 0)//���ƽڵ�
		{
			ElementH = 1; ElementN = 0; ElementJ = 0; ElementL = 1;
			RightP[iI] = 0;
			RightQ[iI] = 0;
			pNet->PPGen[iI] = (real)(deltaP - pNet->PPnet[iI] - pNet->PInst[iI] - pNet->PLcut[iI]);
			pNet->QPGen[iI] = (real)(deltaQ - pNet->QPnet[iI] - pNet->QInst[iI] - pNet->QLcut[iI]);
		}
		m_Solver->SetElement(II, ElementH, ElementJ, ElementN, ElementL);
	}
}


#include "DyFaultLine.h"
#include "../CommonFunction/ErrorInfo.h"

float DYFAULTLINE::GetNextTime()
{
	if (mStatus == 0)
		return T1;
	if (mStatus == 1)
		return T2;
	return 999.;
}

int DYFAULTLINE::GetIsProcess()
{
	if (mStatus == 2)
		return 1;
	return 0;
}

int DYFAULTLINE::Initial()
{
	BranchIndex = pDyn->BranchSearch(BranchName);
	if (BranchIndex < 0)
	{
		return 0;
	}
	pBranch = pDyn->cpGetBranch(BranchIndex);
	if (pBranch == NULL)
		return 0;
	mStatus = 0;
	if (mFaultSide == 1)
	{
		FaultBusNo = pBranch->iGetFromBusNo();
	}
	else
		FaultBusNo = pBranch->iGetToBusNo();
	real Z;
	Z = Fault_R*Fault_R + Fault_X*Fault_X;
	Yr = Fault_R / Z;
	Yi = -Fault_X / Z;
	return 1;
}

int DYFAULTLINE::ReadLine(char*line)
{
	int flag, flagerr;
	flag = sscanf(line, "%*[^,],%[^,],%d,%f,%f,%f,%f",
		BranchName, &mFaultSide, &T1, &T2, &Fault_R, &Fault_X);
	ReplaceName(BranchName, _MaxNameLen);
	if (flag != 6)return 0;
	flagerr = 0;
	if (mFaultSide != 2)mFaultSide = 1;
	if (fabs(T1) < 0.001)T1 = 0.;
	if (T2 < 0)
	{
		sprintf(ErrorMessage[0], "ERROR: �����·������ʱ����д���󣬶Ͽ�ʱ�����Ϊ��");
		cpGetErrorInfo()->PrintWarning(-1, 1);
		flagerr = 1;
	}
	if (T2 < T1 + 0.001)
	{
		sprintf(ErrorMessage[0], "ERROR: �����·������ǰ��Ͽ�ʱ�������ڶ�·ʱ��");
		cpGetErrorInfo()->PrintWarning(-1, 1);
		flagerr = 1;
	}
	if (fabs(Fault_R) < 0.0001)Fault_R = 0.;
	if (fabs(Fault_X) < 0.0001)Fault_X = 0.000001f;
	if (flagerr == 1)return 0;
	return 1;
}

void DYFAULTLINE::PrintInfo(char*Line)
{
	sprintf(Line, "��·'%s'�����·��������", BranchName);
}

void DYFAULTLINE::ProcessFault()
{
	if (mStatus == 0)
	{
		if (pDyn != NULL)
		{
			pDyn->ModifyNetMatrix(FaultBusNo, FaultBusNo, Yr, Yi);
		}
		mStatus = 1;
	}
	else if (mStatus == 1)
	{
		if (pDyn != NULL)
		{
			pDyn->ModifyNetMatrix(FaultBusNo, FaultBusNo, -Yr, -Yi);
			pDyn->DeleteBranch(BranchIndex);
		}
		mStatus = 2;
	}
}

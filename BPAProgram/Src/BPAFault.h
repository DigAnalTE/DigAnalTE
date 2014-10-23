
#ifndef __BPA_FAULT_H__
#define __BPA_FAULT_H__

#include "../../DigAnalTE/DigAnalTE/StabilityAnalysis/DyFaultInfo.h"

class BPA_FAULTINFO :public DYFAULTINFO
{
public:
	virtual int ReadFile(char*file);
};

class BPAFAULTLINE :public DYFAULTBASE
{
public:
	BPAFAULTLINE(){ pBranch = NULL; mStatus = 0; }
public:
	char BranchName[_MaxNameLen];
	char BusName1[_MaxNameLen], BusName2[_MaxNameLen],ID;
	float BaseKV1, BaseKV2;
	int BranchIndex;
	BRANCHBASE *pBranch;

	int mStatus;//״̬ 0=δ���� 1=������

	int type, flagCut;
	float Fault_R;//�ӵ��迹,����ֵ.
	float Fault_X;
	float Time;//�ܲ�

	int FaultBusNo;
	real Yr, Yi;
public:
	virtual float GetNextTime();
	virtual int GetIsProcess();
	virtual int Initial();
	virtual int ReadLine(char*);
	virtual void PrintInfo(char*Line);
	virtual void ProcessFault();
};

#endif

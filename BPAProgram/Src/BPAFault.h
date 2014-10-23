
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

	int mStatus;//状态 0=未处理 1=处理完

	int type, flagCut;
	float Fault_R;//接地阻抗,标幺值.
	float Fault_X;
	float Time;//周波

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

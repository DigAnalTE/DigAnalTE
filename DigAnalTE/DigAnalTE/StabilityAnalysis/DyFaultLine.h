
#ifndef __DigAnalTE_Fault_Line_h__
#define __DigAnalTE_Fault_Line_h__

#include "DyFaultInfo.h"
#include "../NetWorkInfo/BranchBase.h"

class DYFAULTLINE :public DYFAULTBASE
{
public:
	DYFAULTLINE(){ pBranch = NULL; }
public:
	char BranchName[_MaxNameLen];
	int BranchIndex;
	BRANCHBASE *pBranch;

	int mFaultSide;//短路故障端
	int mStatus;//状态 0=未处理 1=线路短路 2=断开=完成

	float Fault_R;//接地阻抗,标幺值.
	float Fault_X;

	float T1, T2;//短路时间和断开时间

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

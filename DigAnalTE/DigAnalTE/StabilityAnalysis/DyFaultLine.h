
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

	int mFaultSide;//��·���϶�
	int mStatus;//״̬ 0=δ���� 1=��·��· 2=�Ͽ�=���

	float Fault_R;//�ӵ��迹,����ֵ.
	float Fault_X;

	float T1, T2;//��·ʱ��ͶϿ�ʱ��

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

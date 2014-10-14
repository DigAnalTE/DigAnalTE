
#ifndef __DigAnalTE_Fault_Info_h__
#define __DigAnalTE_Fault_Info_h__

#include "../DynamicModel/DynamicModelInfo.h"

#define _MaxFaultNum 1000//考虑故障重数

class DYFAULTBASE
{
public:
	DYNAMICMODELINFO *pDyn;
public:
	DYFAULTBASE()
	{
		pDyn = NULL;
	}
public:
	float GetNextTime(){ return 99999.; }
	int GetIsProcess(){ return 1; }
	void SetDynInfo(DYNAMICMODELINFO*tDyn)
	{
		pDyn = tDyn;
		Initial();
	}
	virtual void Initial(){}
	virtual int ReadLine(char*) = 0;
	virtual void WriteLine(char*) = 0;
	virtual void ProcessFault() = 0;
};

class DYFAULTINFO
{
public:
	DYFAULTINFO();
	~DYFAULTINFO(){ RemoveAllFault(); }
public:
	int iFaultListNo;
	DYFAULTBASE *pFaultList[_MaxFaultNum];
	DYNAMICMODELINFO *pDyn;
	float NextTime;
	int NextFault;//指向下一个事件，-1表示未初始化，-2表示事件都处理过了
public://添加故障的操作
	void RemoveAllFault();
public:
	void PrintInfo(FILE *fp);
	int AddNewFault(char*Line);
	int AddNewFault(DYFAULTBASE*tFault);
public:
	float GetNextTime();
	int ProcessFault(float time);
};

#endif

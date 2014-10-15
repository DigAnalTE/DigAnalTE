
#ifndef __DigAnalTE_Fault_Info_h__
#define __DigAnalTE_Fault_Info_h__

#include "../DynamicModel/DynamicModelInfo.h"

#define _MaxFaultNum 1000//考虑故障数

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
	virtual float GetNextTime(){ return 99999.; }
	virtual int GetIsProcess(){ return 1; }
	int SetDynInfo(DYNAMICMODELINFO*tDyn)
	{
		pDyn = tDyn;
		return Initial();
	}
	virtual int Initial(){ return 1; }
	virtual int ReadLine(char*) = 0;
	virtual void PrintInfo(char*Line) = 0;
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
	int ReadFile(char*file);
	void PrintInfo(FILE *fp);
	int AddNewFault(char*Line);
	int AddNewFault(DYFAULTBASE*tFault);
public:
	float GetNextTime();
	int ProcessFault(float time);
};

#endif

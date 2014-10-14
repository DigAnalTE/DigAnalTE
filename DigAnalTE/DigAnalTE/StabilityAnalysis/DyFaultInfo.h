
#ifndef __DigAnalTE_Fault_Info_h__
#define __DigAnalTE_Fault_Info_h__

#include "../DynamicModel/DynamicModelInfo.h"

#define _MaxFaultNum 1000//���ǹ�������

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
	int NextFault;//ָ����һ���¼���-1��ʾδ��ʼ����-2��ʾ�¼����������
public://��ӹ��ϵĲ���
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

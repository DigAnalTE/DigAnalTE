
#ifndef __DigAnalTE_NETWORKINFO_NETWORK_BASE_H__
#define __DigAnalTE_NETWORKINFO_NETWORK_BASE_H__

// NETWORKBASE类是程序中最基础的类，所有的数据都使用该类进行管理
// 该类包含网络数据的数据接口、查找、排序、孤岛分析等最基本功能
//
// 为了方便用户使用，用户需要理解最基础的三个函数：
// 1.	读写数据函数：接口函数
// 2.	索引函数：NetLink：建立所有的连接、索引和空间。在不增加元件的基础上，该函数只需要调用一次即可。多次调用时允许的，只是会影响速度
// 3.	分析函数：NetAnalysis：根据元件的当前状态，重新分析数据。在元件状态发生变化后，需要重新调用该函数
//
// 派生：派生后，需要重载关键函数：NetLink和NetAnalysis

#include "AreaInfo.h"
#include "BusInfo.h"
#include "BranchInfo.h"
#include "EquipmentInfo.h"

class NETWORK_BASE
{//数据部分，包含三部分：数据部分、数据接口函数、数据索引函数
public:
	NETWORK_BASE();
	~NETWORK_BASE();
	virtual void FreeSpace();
public:
	int m_ChangeCode;		//非常重要的一个标志符，如果基础数据发生变化后，请改变该变量
public:
	int NetError;
	int bIsLink;			//==1表示成功建立索引关系
	virtual void NetLink();	//建立索引关系
public://分析孤岛情况
	virtual int NetAnalysis();

	/*-------------------------PART-1--数据部分-------------------------*/
	//
	//	总体信息
	//
protected:
	char  Description[_MaxNameLen];
	float BMVA;
	float FreqSys;
public:
	float GetBMVA(){ return BMVA; }
	float *pGetBMVA(){ return &BMVA; }
	void SetBMVA(float tBase){ BMVA = tBase; }
	float GetFreqSys(){ return FreqSys; }
	void SetFreqSys(float tFreq){ FreqSys = tFreq; }
	//
	//	区域信息
	//
protected:
	AREAINFO m_AreaInfo;
public:
	int iGetAreaTotal(){ return m_AreaInfo.iGetAreaTotal(); }
	int iGetZoneTotal(){ return m_AreaInfo.iGetZoneTotal(); }
	AREAINFO* cpGetAreaInfo(){ return &m_AreaInfo; }
	AREABASE* cpGetArea(int i){ return m_AreaInfo.cpGetArea(i); }
	int iGetZoneNo(char* tZone){ return m_AreaInfo.iGetZoneNo(tZone); }
	int iGetAreaNo(char* tArea){ return m_AreaInfo.iGetAreaNo(tArea); }

protected://母线信息
	BUSINFO m_BusInfo;
public:
	int iGetBusTotal(){ return m_BusInfo.iGetBusTotal(); }
	BUSINFO* cpGetBusInfo(){ return &m_BusInfo; }
	BUSBASE* cpGetBus(int i){ return m_BusInfo.cpGetBus(i); }
	int BusSearch(char *lpszString){ return m_BusInfo.BusSearch(lpszString); }

protected://线路信息
	BRANCHINFO m_BranchInfo;
public:
	int iGetBranchTotal(){ return m_BranchInfo.iGetBranchTotal(); }
	BRANCHINFO* cpGetBranchInfo(){ return &m_BranchInfo; }
	BRANCHBASE* cpGetBranch(int i){ return m_BranchInfo.cpGetBranch(i); }
	int BranchSearch(char *lpszString){ return m_BranchInfo.BranchSearch(lpszString); }

protected://设备信息
	//该设备必须有对应的母线，如果没有该母线，则在BusInfo中建立新的母线
	EQUIPMENTINFO m_EquipmentInfo;//包含各种各样的设备，单端口，两端口，多端口
public:
	int iGetEquipTotal(){ return m_EquipmentInfo.iGetEquipTotal(); }
	EQUIPMENTINFO* cpGetEquipInfo(){ return &m_EquipmentInfo; }
	EQUIPMENTBASE* cpGetEquip(int i){ return m_EquipmentInfo.GetEquipment()[i]; }
	ONEEQUIPMENTBASE* cpGetOneEquip(int i)
	{
		if (!m_EquipmentInfo.GetEquipment()[i]->IsOneEquipment())
			return NULL;
		return (ONEEQUIPMENTBASE*)(m_EquipmentInfo.GetEquipment()[i]);
	}

	/*-------------------------PART-2--数据接口-------------------------*/
public://普通接口
	int ReadFile(char*file);
	void WriteFile(char*file);
	void WriteFile(FILE*fp);
	void OutputPFOFile(FILE*fp);//输出潮流信息
};

#endif

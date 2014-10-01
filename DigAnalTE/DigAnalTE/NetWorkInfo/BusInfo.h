
#ifndef __DigAnalTE_BUSINFO_H__
#define __DigAnalTE_BUSINFO_H__

//母线信息类的管理
//主要是负责查找

#include "BusBase.h"
#include "../CommonFunction/HashTable.h"

class BUSINFO
{
	friend class NetTempSortBus;
public:
	BUSINFO();
	virtual ~BUSINFO(){ FreeSpace(); }
	virtual void FreeSpace();
	//--------------------基本信息--------------------
protected://数据部分
	int BusTotal;
	BUSBASE *Bus[_MaxBusNo];
public:
	int iGetBusTotal(){ return BusTotal; }
	BUSBASE* cpGetBus(int i){ return Bus[i]; }
	void SetBus(int i, BUSBASE*tBus){ Bus[i] = tBus; }//非常规用法
	int AddNewBus(BUSBASE*tBus);
	//--------------------索引信息--------------------
protected://索引//不需要Build索引，在添加的过程中自动添加
	HASHTABLE m_BusHash;//使用Hash表法管理Bus
public:
	void ResetHashTable();
	int BusSearch(char *lpszString);
};

#endif

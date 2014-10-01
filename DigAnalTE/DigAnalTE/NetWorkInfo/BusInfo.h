
#ifndef __DigAnalTE_BUSINFO_H__
#define __DigAnalTE_BUSINFO_H__

//ĸ����Ϣ��Ĺ���
//��Ҫ�Ǹ������

#include "BusBase.h"
#include "../CommonFunction/HashTable.h"

class BUSINFO
{
	friend class NetTempSortBus;
public:
	BUSINFO();
	virtual ~BUSINFO(){ FreeSpace(); }
	virtual void FreeSpace();
	//--------------------������Ϣ--------------------
protected://���ݲ���
	int BusTotal;
	BUSBASE *Bus[_MaxBusNo];
public:
	int iGetBusTotal(){ return BusTotal; }
	BUSBASE* cpGetBus(int i){ return Bus[i]; }
	void SetBus(int i, BUSBASE*tBus){ Bus[i] = tBus; }//�ǳ����÷�
	int AddNewBus(BUSBASE*tBus);
	//--------------------������Ϣ--------------------
protected://����//����ҪBuild����������ӵĹ������Զ����
	HASHTABLE m_BusHash;//ʹ��Hash������Bus
public:
	void ResetHashTable();
	int BusSearch(char *lpszString);
};

#endif

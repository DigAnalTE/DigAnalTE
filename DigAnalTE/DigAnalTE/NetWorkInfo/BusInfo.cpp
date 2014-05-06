
#include "BusInfo.h"
#include "NetWorkInfo.h"
#include "../CommonFunction/ErrorInfo.h"
#include "../CommonFunction/QikSort.h"

BUSINFO::BUSINFO()
{
	BusTotal=0;
	memset(Bus,0,_MaxBusNo*sizeof(BUSBASE*));
	m_BusHash.InitHashTable(_MaxBusNo);
}

void BUSINFO::FreeSpace()
{
	int i;
	for (i=0;i<_MaxBusNo;i++)
	{
		FreeArray(Bus[i]);
	}
	BusTotal=0;
	m_BusHash.InitHashTable(_MaxBusNo);
}

int BUSINFO::AddNewBus(BUSBASE*tBus)
{
	int flag;
	if (BusTotal>=_MaxBusNo)
	{
		sprintf(ErrorMessage[0],"母线数目超过设定值%d",_MaxBusNo);
		cpGetErrorInfo()->PrintError(1);
		return 0;
	}
	flag = m_BusHash.InsertElementToHashTable(BusTotal,tBus->Name);
	if (flag>=0)
	{
		sprintf(ErrorMessage[0]," 重复的节点数据，忽略: ");
		tBus->PrintInfo(ErrorMessage[1]);
		cpGetErrorInfo()->PrintWarning(7,2);
		return -1;
	}
	Bus[BusTotal]=tBus;
	tBus->BusNo=BusTotal;
	BusTotal++;
	return BusTotal-1;
}

int BUSINFO::BusSearch( char *lpszString )
{
	return m_BusHash.ElementSearch(lpszString);
}
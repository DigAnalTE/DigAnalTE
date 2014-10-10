
#include "EquipmentInfo.h"
#include "NetWorkInfo.h"
#include "Generator.h"
#include "Compensation.h"
#include "NetLoad.h"
#include "../CommonFunction/ErrorInfo.h"
#include "../CommonFunction/QikSort.h"

EQUIPMENTINFO::EQUIPMENTINFO()
{
	EquipmentTotal = 0;
	memset(Equipment, 0, _MaxEquipNo*sizeof(EQUIPMENTBASE*));
	EquipLinkNo = 0;
	EquipLink = NULL; EQINDX = NULL; EQCONT = NULL;
	m_EquipHash.InitHashTable(_MaxEquipNo);
}

void EQUIPMENTINFO::FreeSpace()
{
	int i;
	for (i = 0; i < _MaxBusNo; i++)
	{
		FreeArray(Equipment[i]);
	}
	EquipmentTotal = 0;
	FreeArray(EquipLink); FreeArray(EQINDX); FreeArray(EQCONT);
	m_EquipHash.InitHashTable(_MaxEquipNo);
}

int EQUIPMENTINFO::AddNewEquip(EQUIPMENTBASE *tEquipment)
{
	if (EquipmentTotal >= _MaxEquipNo)
	{
		sprintf(ErrorMessage[0], "设备数目超过设定值%d，忽略该数据：", _MaxEquipNo);
		cpGetErrorInfo()->PrintWarning(6, 1);
		return -1;
	}
	int flag;
	flag = m_EquipHash.InsertElementToHashTable(EquipmentTotal, tEquipment->Name);
	if (flag >= 0)
	{
		sprintf(ErrorMessage[0], " 重复的线路数据数据，忽略: ");
		tEquipment->PrintInfo(ErrorMessage[1]);
		cpGetErrorInfo()->PrintWarning(7, 2);
		return -1;
	}
	Equipment[EquipmentTotal] = tEquipment;
	EquipmentTotal++;
	return EquipmentTotal - 1;
}

GENERATOR* EQUIPMENTINFO::AddNewGeneratorEquip()
{
	if (EquipmentTotal >= _MaxEquipNo)
	{
		sprintf(ErrorMessage[0], "设备数目超过设定值%d，忽略该数据：", _MaxEquipNo);
		cpGetErrorInfo()->PrintWarning(6, 1);
		return NULL;
	}
	GENERATOR *tempGen;
	tempGen = new GENERATOR;
	Equipment[EquipmentTotal] = tempGen;
	EquipmentTotal++;
	return tempGen;
}

class NetTempSortEquipLink : public QikSortInterface
{//排序
public:
	NetTempSortEquipLink(EQUIPMENTINFO::EQUIPLINK*tLink){ pLink = tLink; }
private:
	int flag;
	EQUIPMENTINFO::EQUIPLINK sorttemp;
	EQUIPMENTINFO::EQUIPLINK*pLink;
protected:
	virtual int kompar(int m, int n);
	virtual void swap(int m, int n);
};
int NetTempSortEquipLink::kompar(int m, int n)
{
	if (pLink[m].IBusNo == pLink[n].IBusNo)
	{
		return pLink[m].EquipNo - pLink[n].EquipNo;
	}
	return (pLink[m].IBusNo - pLink[n].IBusNo);
}
void NetTempSortEquipLink::swap(int m, int n)
{
	sorttemp = pLink[m];
	pLink[m] = pLink[n];
	pLink[n] = sorttemp;
}
void EQUIPMENTINFO::BuildLink(int tBusTotal)
{
	int i, j, k, tTerNo;
	BusTotal = tBusTotal;
	EquipLinkNo = 0;
	for (i = 0; i < EquipmentTotal; i++)
	{
		if (Equipment[i]->IsTwoEquipment())
		{
			EquipLinkNo += 2;
		}
		else if (Equipment[i]->IsMltEquipment())
		{
			tTerNo = ((MLTEQUIPMENTBASE*)(Equipment[i]))->PortNumber;
			EquipLinkNo += tTerNo*(tTerNo - 1);
		}
		else{
			EquipLinkNo++;
		}
	}
	if (EquipLink != NULL)FreeArray(EquipLink);
	if (EquipLinkNo <= 0)
	{
		if (EQINDX != NULL)FreeArray(EQINDX);
		if (EQCONT != NULL)FreeArray(EQCONT);
		MallocNew(EQINDX, int, BusTotal);
		MallocNew(EQCONT, int, BusTotal);
		for (i = 0; i < BusTotal; i++)
		{
			EQINDX[i] = 0;
			EQCONT[i] = 0;
		}
		return;
	}
	MallocNew(EquipLink, EQUIPLINK, EquipLinkNo);
	ONEEQUIPMENTBASE* tempOne;
	TWOEQUIPMENTBASE* tempTwo;
	MLTEQUIPMENTBASE* tempMulty;
	for (i = 0, j = 0; i < EquipmentTotal; i++)
	{
		if (Equipment[i]->IsOneEquipment())
		{
			tempOne = (ONEEQUIPMENTBASE*)(Equipment[i]);
			EquipLink[j].IBusNo = tempOne->BusNo;
			EquipLink[j].Type = 1;
			EquipLink[j].EquipNo = i;
			EquipLink[j].nDirtn = 1;
			j++;
		}
		else if (Equipment[i]->IsTwoEquipment())
		{
			tempTwo = (TWOEQUIPMENTBASE*)(Equipment[i]);
			EquipLink[j].IBusNo = tempTwo->BusNo1;
			EquipLink[j].Type = 2;
			EquipLink[j].EquipNo = i;
			EquipLink[j].nDirtn = 1;
			j++;
			EquipLink[j].IBusNo = tempTwo->BusNo2;
			EquipLink[j].Type = 2;
			EquipLink[j].EquipNo = i;
			EquipLink[j].nDirtn = -1;
			j++;
		}
		else if (Equipment[i]->IsMltEquipment())
		{
			tempMulty = (MLTEQUIPMENTBASE*)(Equipment[i]);
			tTerNo = tempMulty->PortNumber;
			for (j = 0; j < tTerNo - 1; j++)
			{
				for (k = j + 1; k < tTerNo; k++)
				{
					EquipLink[j].IBusNo = tempMulty->BusNo[j];
					EquipLink[j].Type = 3;
					EquipLink[j].EquipNo = i;
					EquipLink[j].nDirtn = j*tTerNo + k;
					j++;
				}
			}
		}
	}

	NetTempSortEquipLink tempSort(EquipLink);
	tempSort.QikSort(0, EquipLinkNo - 1);

	if (EQINDX != NULL)FreeArray(EQINDX);
	if (EQCONT != NULL)FreeArray(EQCONT);
	MallocNew(EQINDX, int, BusTotal);
	MallocNew(EQCONT, int, BusTotal);
	int tBn = 0;
	while (tBn < EquipLinkNo && EquipLink[tBn].IBusNo < 0)tBn++;
	for (i = 0, tBn; i < BusTotal&&tBn < EquipLinkNo; i++)
	{
		EQINDX[i] = tBn;
		EQCONT[i] = 0;
		while (EquipLink[tBn].IBusNo == i)
		{
			tBn++;
			EQCONT[i]++;
		}
	}
	for (; i < BusTotal; i++)
	{
		EQINDX[i] = tBn;
		EQCONT[i] = 0;
	}
}

int EQUIPMENTINFO::EquipSearch(char *lpszString)
{
	return m_EquipHash.ElementSearch(lpszString);
}

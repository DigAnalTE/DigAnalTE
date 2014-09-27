
#include "AreaInfo.h"
#include "NetWorkBase.h"
#include "../CommonFunction/constdef.h"
#include "../CommonFunction/ErrorInfo.h"

AREAINFO::AREAINFO()
{
	ZoneTotal = 0; AreaTotal = 0;
	m_ZoneHash.InitHashTable(_MaxZoneNo);
	memset(Area, 0, _MaxAreaNo*sizeof(AREABASE*));
	memset(ZoneName, 0, _MaxZoneNo*_MaxNameLen*sizeof(char));
}

void AREAINFO::FreeSpace()
{
	ZoneTotal = 0; AreaTotal = 0;
	int i;
	for (i = 0; i < _MaxAreaNo; i++)
	{
		FreeArray(Area[i]);
	}
}

int AREAINFO::AddNewArea(AREABASE*tArea)
{
	if (AreaTotal >= _MaxAreaNo)
	{
		return -1;
	}
	Area[AreaTotal] = tArea;
	AreaTotal++;
	return AreaTotal - 1;
}

void AREAINFO::BuildLink(NETWORK_BASE* pNet)
{
	int i, j, BusTotal, BusNo, ZoneNo, ZoneTotal = 0;
	//第一步，重新建立Zone表，根据Bus上的Zone信息
	char *Zone;
	ZoneTotal = 0;
	m_ZoneHash.InitHashTable(_MaxZoneNo);
	BusTotal = pNet->iGetBusTotal();
	for (i = 0; i < BusTotal; i++)
	{
		Zone = pNet->cpGetBus(i)->Zone;
		ZoneNo = m_ZoneHash.InsertElementToHashTable(ZoneTotal, Zone);
		if (ZoneNo < 0)
		{
			ZoneNo = ZoneTotal;
			if (ZoneTotal > _MaxZoneNo)
			{
				sprintf(ErrorMessage[0], "Zone数目超过设定值%d，忽略：%s", _MaxZoneNo, Zone);
				cpGetErrorInfo()->PrintWarning(0, 1);
			}
			else{
				strcpy(ZoneName[ZoneTotal], Zone);
			}
			ZoneTotal++;
		}
		pNet->cpGetBus(i)->ZoneNo = ZoneNo;
	}
	//第二步，初始化area信息
	for (i = 0; i < AreaTotal; i++)
	{
		//初始化Area中的ZoneNo
		for (j = 0; j < Area[i]->ZoneTotal; j++)
		{
			Zone = Area[i]->ZoneName[j];
			ZoneNo = m_ZoneHash.InsertElementToHashTable(ZoneTotal, Zone);
			if (ZoneNo < 0)
			{
				ZoneNo = ZoneTotal;
				sprintf(ErrorMessage[0], "Zone'%s'中没有母线", Zone);
				cpGetErrorInfo()->PrintWarning(0, 1);
				if (ZoneTotal > _MaxZoneNo)
				{
					sprintf(ErrorMessage[0], "Zone数目超过设定值%d，忽略：%s", _MaxZoneNo, Zone);
					cpGetErrorInfo()->PrintWarning(0, 1);
				}
				else{
					strcpy(ZoneName[ZoneTotal], Zone);
				}
				ZoneTotal++;
			}
			Area[i]->ZoneNo[j] = ZoneNo;
			ZoneAreaNo[ZoneNo] = i;
		}
		//初始化Area中的SlackBusNo
		BusNo = pNet->BusSearch(Area[i]->SlackBusName);
		Area[i]->AreaSlackBusNo = BusNo;
	}
	ZoneTotal = ZoneTotal;
	for (i = 0; i < BusTotal; i++)
	{
		pNet->cpGetBus(i)->AreaNo = ZoneAreaNo[pNet->cpGetBus(i)->ZoneNo];
	}
}

int AREAINFO::ReadLine(char*Line)
{//7,Name,  No,  Zone...
	AREABASE *tArea;
	tArea = new AREABASE;
	int i, flag, No;
	No = 0;
	flag = sscanf(Line, "%*[^,],%[^,],%d",
		tArea->AreaName, &No);
	if (flag < 1)
	{
		return 0;
	}
	ReplaceName(tArea->AreaName, _MaxNameLen);
	char formate[100], formate1[100];
	strcpy(formate, "%*[^,],%*[^,],%*[^,],");
	for (i = 0; i < No&&i < _MaxAreaZoneNo; i++)
	{
		strcpy(formate1, formate);
		strcat(formate1, "%[^,\r\n]");
		flag = sscanf(Line, formate1, tArea->ZoneName[i]);
		if (flag < 1)
			return 0;
		ReplaceName(tArea->ZoneName[i], _MaxNameLen);
		strcat(formate, "%*[^,],");
	}
	tArea->ZoneTotal = i;
	AddNewArea(tArea);
	return 1;
}

void AREAINFO::WriteLine(int i, char*Line)
{
	sprintf(Line, "7,%s,	%d",
		Area[i]->AreaName, Area[i]->ZoneTotal);
	char tempLine[_MaxLineLen];
	int iA;
	for (iA = 0; iA < Area[i]->ZoneTotal&&iA < _MaxAreaZoneNo; iA++)
	{
		sprintf(tempLine, ",	%s", Area[i]->ZoneName[iA]);
		strcat(Line, tempLine);
	}
	strcat(Line, "\n");
}

int AREAINFO::iGetZoneNo(char* tZone)
{
	return m_ZoneHash.ElementSearch(tZone);
}

int AREAINFO::iGetAreaNo(char* tArea)
{
	int i;
	for (i = 0; i < AreaTotal; i++)
	{
		if (strncmp(tArea, Area[i]->AreaName, _MaxNameLen) == 0)
		{
			return i;
		}
	}
	return -1;
}
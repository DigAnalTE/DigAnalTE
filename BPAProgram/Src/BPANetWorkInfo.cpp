
#include "BPANetWorkInfo.h"
#include "../../DigAnalTE/DigAnalTE/CommonFunction/ErrorInfo.h"
#include "BPABus.h"
#include "BPABranch.h"
#include "BPAReadWrite.h"

#define _MaxBPALineLen_ 120

int BPA_NETWORKINFO::ReadFile(char*tFileName)
{
	char Line[_MaxBPALineLen_];
	char(*LineST)[_MaxBPALineLen_]; int *IsRead = NULL; int *IsChange = NULL;
	int i, j, flag, LineTotal = 0;//���Ӹù��ܣ�֧��һ�ж���β��������

	//��һ�������ļ����뵽LineST������
	FILE*	fpPFfile;
	fpPFfile = fopen(tFileName, "rb");
	if (fpPFfile == NULL)
		return 0;
	while (fgets(Line, _MaxBPALineLen_, fpPFfile))
		LineTotal++;
	rewind(fpPFfile);
	MallocCharArray(LineST, (LineTotal + 4), _MaxBPALineLen_);
	i = 0;
	int flagAddLine = 0;
	while (fgets(Line, _MaxBPALineLen_, fpPFfile))
	{
		if (flagAddLine == 1)
		{
			if (strlen(Line) < _MaxBPALineLen_ - 1)
				flagAddLine = 0;
			continue;
		}
		if (strlen(Line) >= _MaxBPALineLen_ - 1)
		{
			if (Line[_MaxBPALineLen_ - 2] != 10 && Line[_MaxBPALineLen_ - 2] != 13)
			{//���￼�ǵ���Щ�պ�_MaxBPALineLen_-2���ַ�����
				flagAddLine = 1;//��ʾ���л�δ����
			}
		}
		if (strlen(Line) > 3 && Line[0] == '(' && Line[1] == 'E' && Line[2] == 'N' && Line[3] == 'D')
			break;//��ֹ��(END)
		if (strlen(Line) > 3 && Line[0] == '/' && (Line[1] == 'M' && Line[2] == 'V' && Line[3] == 'A'
			|| Line[1] == ' ' && Line[2] == 'M' && Line[3] == 'V' && Line[4] == 'A'))
		{//����"/MVA_BASE = 1000\"��//problem�˴�����������
			char *ex;
			if ((ex = strrchr(Line, '=')) != NULL)
			{
				SetBMVA((float)atof(++ex));
			}
		}
		if (strlen(Line) == 0
			|| Line[0] == 'C' || Line[0] == 'H' || Line[0] == '.' || Line[0] == ' '//ע�� ����
			|| Line[0] == '(' || Line[0] == '[' || Line[0] == '/' || Line[0] == '\\' || Line[0] == '>'//��������
			|| Line[0] == 10 || Line[0] == 13)//���з� �س���
			continue;
		strncpy(LineST[i], Line, _MaxBPALineLen_);
		i++;
	}
	fclose(fpPFfile);
	LineTotal = i;
	MallocNew(IsRead, int, LineTotal); memset(IsRead, 0, LineTotal*sizeof(int));
	MallocNew(IsChange, int, LineTotal); memset(IsChange, 0, LineTotal*sizeof(int));
	for (i = 0; i < LineTotal; i++)
	{
		if (LineST[i][2] == 'D' || LineST[i][2] == 'A' || LineST[i][2] == 'R')
		{
			IsChange[i] = 1;
		}
		if (LineST[i][0] == 'P' || LineST[i][0] == 'Z' || LineST[i][0] == 'D')
		{
			IsChange[i] = 1;
		}
	}
	//��������
	BUSBASE *tempBus;
	BRANCHBASE *tempBranchBase;
	for (i = 0; i < LineTotal; i++)
	{
		if (IsRead[i] != 0)continue;
		if (IsChange[i] != 0)continue;//����������ǳ������ݣ��������޸Ŀ�
		flag = 0;
		switch (LineST[i][0])
		{
		case 'A':
			flag = Area_ReadBPALine(LineST[i]);
			break;
		case 'B':
			tempBus = new BPABUS();
			flag = tempBus->ReadLine(LineST[i]);
			if (flag != 1)
			{
				delete tempBus;
				flag = 0;
			}
			else{
				flag = cpGetBusInfo()->AddNewBus(tempBus);
				if (flag < 0)
				{
					delete tempBus;
					flag = 0;
				}
				else
					flag = 1;
			}
			break;
		case 'L':
			if (LineST[i][1] == '+')
				break;//����L+��
			if (LineST[i][1] == ' ')
			{
				tempBranchBase = new LBRANCH();
				flag = tempBranchBase->ReadLine(LineST[i]);
				if (flag != 1)
				{
					delete tempBranchBase;
					flag = 0;
				}
				else{
					flag = cpGetBranchInfo()->AddNewBranch(tempBranchBase);
					if (flag < 0)
					{
						delete tempBranchBase;
						flag = 0;
					}
					else
						flag = 1;
				}
			}
			break;
		case 'E':
			tempBranchBase = new EBRANCH();
			flag = tempBranchBase->ReadLine(LineST[i]);
			if (flag != 1)
			{
				delete tempBranchBase;
				flag = 0;
			}
			else{
				flag = cpGetBranchInfo()->AddNewBranch(tempBranchBase);
				if (flag < 0)
				{
					delete tempBranchBase;
					flag = 0;
				}
				else
					flag = 1;
			}
			break;
		case 'T':
			tempBranchBase = new TBRANCH();
			flag = tempBranchBase->ReadLine(LineST[i]);
			if (flag != 1)
			{
				delete tempBranchBase;
				flag = 0;
			}
			else{
				flag = cpGetBranchInfo()->AddNewBranch(tempBranchBase);
				if (flag < 0)
				{
					delete tempBranchBase;
					flag = 0;
				}
				else
					flag = 1;
			}
			break;
		}
		if (flag == 1)IsRead[i] = 1;
	}

	NetLink();

	char Name1[_MaxNameLen], Name2[_MaxNameLen], BranchName[_MaxNameLen], ID;
	float BaseKv1, BaseKv2;
	char CID[2];
	int BusIndex, BranchIndex;
	char Zone[_MaxNameLen]; int tZoneNo; float LoadPper, LoadQper, GenPper, GenQper;
	for (i = 0; i < LineTotal; i++)
	{
		if (IsRead[i] != 0)continue;
		//if(IsChange[i]!=1)continue;
		flag = 0;
		switch (LineST[i][0])
		{
		case '+':
			GetItemFromLine(LineST[i], Name1, PLUS_Para[1], PLUS_Loca[1]); ReplaceName(Name1, _MaxNameLen);
			GetItemFromLine(LineST[i], (void*)(&BaseKv1), PLUS_Para[2], PLUS_Loca[2]);
			ResetBPAName(Name1, BaseKv1);
			BusIndex = BusSearch(Name1);
			if (BusIndex < 0)
			{
				sprintf(ErrorMessage[0], "�Ҳ���ĸ�ߣ�%s", LineST[i]);
				cpGetErrorInfo()->PrintWarning(1, 1);
				break;
			}
			flag = cpGetBus(BusIndex)->ReadLine(LineST[i]);
			break;
		case 'L':
			if (LineST[i][1] == '+')
			{
				GetItemFromLine(LineST[i], (void*)(Name1), LPLUS_Para[1], LPLUS_Loca[1]);
				GetItemFromLine(LineST[i], (void*)(&BaseKv1), LPLUS_Para[2], LPLUS_Loca[2]);
				GetItemFromLine(LineST[i], (void*)(Name2), LPLUS_Para[4], LPLUS_Loca[4]);
				GetItemFromLine(LineST[i], (void*)(&BaseKv2), LPLUS_Para[5], LPLUS_Loca[5]);
				GetItemFromLine(LineST[i], (void*)(&CID), LPLUS_Para[6], LPLUS_Loca[6]);
				ID = CID[0];
				GenerateBPABranchName(BranchName, Name1, BaseKv1, Name2, BaseKv2, ID);
				BranchIndex = BranchSearch(BranchName);
				if (BranchIndex < 0)
				{
					sprintf(ErrorMessage[0], "�Ҳ�����·��%s", LineST[i]);
					cpGetErrorInfo()->PrintWarning(1, 1);
					break;
				}
				flag = cpGetBranch(BranchIndex)->ReadLine(LineST[i]);
			}
			break;
		case 'P':
			if (LineST[i][1] == 'Z')
			{
				GetItemFromLine(LineST[i], Zone, PZ_Para[0], PZ_Loca[0]); ReplaceName(Zone, _MaxNameLen);
				GetItemFromLine(LineST[i], &LoadPper, PZ_Para[1], PZ_Loca[1]);
				GetItemFromLine(LineST[i], &LoadQper, PZ_Para[2], PZ_Loca[2]);
				GetItemFromLine(LineST[i], &GenPper, PZ_Para[3], PZ_Loca[3]);
				GetItemFromLine(LineST[i], &GenQper, PZ_Para[4], PZ_Loca[4]);
				if (fabs(GenPper - 1.0) > 0.0001 || fabs(GenQper - 1.0) > 0.0001
					|| LoadPper<-0.0001 || LoadPper>1.0001
					|| LoadQper<-0.0001 || LoadQper>1.0001)
				{
					flag = 0;
					break;
				}
				if (fabs(GenPper) < 0.0001)GenPper = 1.f;
				if (fabs(GenQper) < 0.0001)GenQper = 1.f;
				if (fabs(GenPper - 1.0) > 0.0001 || fabs(GenQper - 1.0) > 0.0001
					|| LoadPper < -0.0001 || LoadQper < -0.0001)
				{
					sprintf(ErrorMessage[0], "PZ��Ŀǰ�޷�����������޸ģ����Կ�Ƭ��%s", LineST[i]);
					cpGetErrorInfo()->PrintWarning(1, 1);
					flag = 0;
					break;
				}
				tZoneNo = cpGetAreaInfo()->iGetZoneNo(Zone);
				if (tZoneNo < 0)
				{
					sprintf(ErrorMessage[0], "�Ҳ���������%s, ���Կ�Ƭ: %s", Zone, LineST[i]);
					cpGetErrorInfo()->PrintWarning(-1, 1);
					flag = 1;
					break;
				}
				for (j = 0; j < iGetBusTotal(); j++)
				{
					if (cpGetBus(j)->ZoneNo == tZoneNo)
					{
						if (cpGetBus(j)->cBusType == 'B')
						{
							((BPABUS*)cpGetBus(j))->m_fBusPLoadPer = LoadPper;
							((BPABUS*)cpGetBus(j))->m_fBusQLoadPer = LoadQper;
						}
					}
				}
				flag = 1;
			}
			break;
		}
		if (flag == 1)IsRead[i] = 1;
	}
#ifdef _DEBUG
	for (i = 0; i < LineTotal; i++)
	{
		if (IsRead[i]>0)continue;
		sprintf(ErrorMessage[0], "  ����ʶ������ݿ���%s", LineST[i]);
		cpGetErrorInfo()->PrintWarning(8, 1);
	}
#endif
	free(LineST); FreeArray(IsRead); FreeArray(IsChange);
	//cpGetErrorInfo()->CheckMessageType(8);//�鿴δʶ��Ƭ��
	cpGetErrorInfo()->CheckMessageType(1);//�鿴����������
	cpGetErrorInfo()->CheckMessageType(6);//���Խ�޴���

	m_ChangeCode++;

	NetLink();
	return 1;
}

int BPA_NETWORKINFO::Area_ReadBPALine(char* PFLine)
{
	int i, k, AreaNo;
	float BaseKv;
	char Name[_MaxNameLen], Zone[_MaxNameLen], tArea[_MaxNameLen];
	GetItemFromLine(PFLine, tArea, AREA_Para[0], AREA_Loca[0]);
	ReplaceName(tArea, _MaxNameLen);
	tArea[_MaxNameLen - 1] = '\0';
	if (strlen(PFLine) > 3 && PFLine[2] == '+')
	{//����
		k = cpGetAreaInfo()->iGetAreaNo(tArea);
		if (k >= cpGetAreaInfo()->iGetAreaTotal())
		{
			sprintf(ErrorMessage[0], "A+��û�ж�Ӧ��A�� '%s' ������, ���Ըÿ���", tArea);
			strcpy(ErrorMessage[1], PFLine);
			cpGetErrorInfo()->PrintWarning(0, 2);
			return 0;
		}
		AreaNo = k;
	}
	else{
		AREABASE *tempArea;
		tempArea = new AREABASE;
		tempArea->ZoneTotal = 0;
		GetItemFromLine(PFLine, Name, AREA_Para[1], AREA_Loca[1]);
		GetItemFromLine(PFLine, &BaseKv, AREA_Para[2], AREA_Loca[2]);
		if (BaseKv<0 || BaseKv>99999.)
		{
			sprintf(ErrorMessage[0], " �ڵ� %s�Ļ�׼��ѹΪ0������ȱʡ������Ϊ100.0KV", Name);
			cpGetErrorInfo()->PrintWarning(0, 1);
			BaseKv = 100.0f;
		}
		ReplaceName(Name, _MaxNameLen);
		ResetBPAName(Name, BaseKv);
		strcpy(tempArea->SlackBusName, Name);
		strcpy(tempArea->AreaName, tArea);
		GetItemFromLine(PFLine, &(tempArea->AreaPower), AREA_Para[3], AREA_Loca[3]);
		AreaNo = cpGetAreaInfo()->AddNewArea(tempArea);
		if (AreaNo < 0)
		{
			sprintf(ErrorMessage[0], "������Ŀ�����趨ֵ%d�����Ըÿ���", _MaxAreaNo);
			strcpy(ErrorMessage[1], PFLine);
			cpGetErrorInfo()->PrintWarning(6, 2);
			return 0;
		}
	}
	i = 0;
	while ((int)(strlen(PFLine)) > (AREA_Loca[4] + 3 * i + 2))
	{//����ط��ܲ����Ƴ��ȣ�ֻҪ�оͿ��Լ�������
		GetItemFromLine(PFLine, Zone, AREA_Para[4], AREA_Loca[4] + 3 * i);
		i++;
		if (IsBlank(Zone, _MaxNameLen))continue;
		if (cpGetAreaInfo()->cpGetArea(AreaNo)->ZoneTotal >= _MaxAreaZoneNo)
		{
			sprintf(ErrorMessage[0], "����%s��Ŀ�����趨ֵ%d�����Զ����Zone��%s", cpGetAreaInfo()->cpGetArea(AreaNo)->ZoneName, _MaxAreaZoneNo, Zone);
			cpGetErrorInfo()->PrintWarning(0, 1);
			continue;
		}
		strcpy(cpGetAreaInfo()->cpGetArea(AreaNo)->ZoneName[cpGetAreaInfo()->cpGetArea(AreaNo)->ZoneTotal], Zone);
		cpGetAreaInfo()->cpGetArea(AreaNo)->ZoneTotal++;
	}
	return 1;
}

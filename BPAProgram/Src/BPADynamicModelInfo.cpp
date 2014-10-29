
#include "BPADynamicModelInfo.h"
#include "BPAReadWrite.h"
#include "BPAGenerator.h"
#include "BPAMCModel.h"
#include "BPABusDyn.h"
#include "../../DigAnalTE/DigAnalTE/CommonFunction/ErrorInfo.h"
#include "../../DigAnalTE/DigAnalTE/CommonFunction/QikSort.h"

#define _MaxBPALineLen_ 120

BPA_DYNAMICMODELINFO::BPA_DYNAMICMODELINFO()
{
	Step0 = 0.01;
	TotalTime = 6;
}

void BPA_DYNAMICMODELINFO::AddNewBusDyn(int busno)
{
	if (m_BusDyn[busno] != NULL)
		return;
	m_BusDyn[busno] = new BPABUSDYNMODEL;
	m_BusDyn[busno]->BusNo = busno;
	m_BusDyn[busno]->SetSolInfo(this);
}

class SortBusByName : public QikSortInterface
{
public:
	SortBusByName(NETWORKINFO*tNet)
	{
		pNet = tNet;
	}
private:
	NETWORKINFO*pNet;
	BUSBASE*tBus;
protected:
	virtual int kompar(int m, int n);
	virtual void swap(int m, int n);
};

int SortBusByName::kompar(int m, int n)
{
	return strncmp(pNet->cpGetBus(m)->GetBusName(), pNet->cpGetBus(n)->GetBusName(), _MaxNameLen);
}

void SortBusByName::swap(int m, int n)
{
	tBus = pNet->cpGetBus(m);
	pNet->cpGetBusInfo()->SetBus(m, pNet->cpGetBus(n));
	pNet->cpGetBusInfo()->SetBus(n, tBus);
}

int BPA_DYNAMICMODELINFO::ReadFile(char*tFileName)
{
	sprintf(ErrorMessage[0], "BPA读入需要两个文件，该函数ReadFile禁用，请使用函数ReadBPAFile");
	cpGetErrorInfo()->PrintError(1);
	return 0;
}

int BPA_DYNAMICMODELINFO::ReadBPAFile(char*datfile, char*swifile)
{
	int flag;
	flag = Net_ReadBPALine(datfile);
	if (flag != 1)
		return flag;

	char Line[_MaxBPALineLen_];
	char(*LineST)[_MaxBPALineLen_]; int *IsRead = NULL;
	int i, LineTotal = 0, Line90No = -1;//增加该功能，支持一行读到尾，并舍弃

	//第一步，将文件读入到LineST数组中
	FILE*	fpPFfile;
	fpPFfile = fopen(swifile, "rb");
	if (fpPFfile == NULL)
		return 0;
	while (fgets(Line, _MaxBPALineLen_, fpPFfile))
		LineTotal++;
	rewind(fpPFfile);
	MallocCharArray(LineST, (LineTotal + 4), _MaxBPALineLen_);
	i = 0;
	while (fgets(Line, _MaxBPALineLen_, fpPFfile))
	{
		if (strlen(Line) > 1 && Line[0] == '9' && Line[1] == '9')
			break;//终止符99
		if (strlen(Line) > 1 && Line[0] == '9' && Line[1] == '0')
		{
			if (Line90No >= 0)
			{
				sprintf(ErrorMessage[0], "有多张90卡，数据出错");
				cpGetErrorInfo()->PrintError(1);
				return 0;
			}
			Line90No = i;
			continue;
		}
		if (strlen(Line) == 0
			|| Line[0] == '.' || Line[0] == ' '//注释 空行
			|| Line[0] == 10 || Line[0] == 13)//换行符 回车符
			continue;
		strncpy(LineST[i], Line, _MaxBPALineLen_);
		i++;
	}
	fclose(fpPFfile);
	LineTotal = i;
	if (Line90No < 0)Line90No = LineTotal;
	MallocNew(IsRead, int, LineTotal); memset(IsRead, 0, LineTotal*sizeof(int));

	//读入模型数据
	for (i = 0; i < Line90No; i++)
	{
		if (IsRead[i] != 0)continue;
		flag = 0;
		switch (LineST[i][0])
		{
		case 'C'://注释卡
			flag = 1;
			break;
		case 'L':
			if (LineST[i][1] == 'S')
			{//LS卡
				flag = 1;
			}
			else if (LineST[i][1] == 'N')
			{//LN卡
				flag = LN_ReadBPALine(LineST[i]);
			}
			break;
		case 'M':
			if (LineST[i][1] == 'C')
			{
				flag = MC_ReadBPALine(LineST[i]);
			}
			break;
		case 'F':
			if (LineST[i][1] == 'F')
			{
				flag = FF_ReadBPALine(LineST[i]);
			}
			break;
		}
		if (flag == 1)IsRead[i] = 1;
	}
	cpGetErrorInfo()->CheckMessageType(15);
	//索引连接
	int equipno;
	EQUIPMENT_DYN_MODEL*tEqipModel;
	for (i = 0; i < DynamicModelTotal; i++)
	{
		equipno = EquipSearch(DynamicModel[i]->EquipmentName);
		if (equipno < 0)
		{
			sprintf(ErrorMessage[0], "ERROR: 没有找到模型对应的设备");
			DynamicModel[i]->PrintInfo(ErrorMessage[1]);
			cpGetErrorInfo()->PrintWarning(13, 2);
			continue;
		}
		DynamicModel[i]->EquipIndex = equipno;
		DynamicModel[i]->pEquip = cpGetEquip(equipno);
		tEqipModel = DynamicModel[i]->IsEquipModel();
		if (tEqipModel != NULL)
		{
			cpGetEquip(equipno)->SetEquipModel(tEqipModel);
		}
	}
	cpGetErrorInfo()->CheckMessageType(13);
	//读入输出卡
	char tBusName[_MaxNameLen], tID;
	float tBaseKV;
	for (i = Line90No; i < LineTotal; i++)
	{
		if (IsRead[i] != 0)continue;
		flag = 0;
		switch (LineST[i][0])
		{
		case 'M'://输出设置卡
			if (LineST[i][1] == 'H')
				flag = 1;
			break;
		case 'G':
			if (LineST[i][1] == 'H')
			{//GH卡
				flag = GH_ReadBPALine(LineST[i]);
			}
			else
			{//发电机输出卡
				GetItemFromLine(LineST[i], tBusName, OUT_GEN_Para[0], OUT_GEN_Loca[0]);
				GetItemFromLine(LineST[i], &tBaseKV, OUT_GEN_Para[1], OUT_GEN_Loca[1]);
				GetItemFromLine(LineST[i], &tID, OUT_GEN_Para[2], OUT_GEN_Loca[2]);
				equipno = BPAGenSearch(tBusName, tBaseKV, tID);
				tEqipModel = cpGetEquip(equipno)->GetEquipModel();
				if (tEqipModel == NULL)
				{
					sprintf(ErrorMessage[0], "输出数据没有对应的模型：%s", Line);
					cpGetErrorInfo()->PrintWarning(-1, 1);
					continue;
				}
				flag = tEqipModel->ReadOutLine(LineST[i]);
				if (flag != 1)
				{
					sprintf(ErrorMessage[0], "输出数据读取失败：%s", Line);
					cpGetErrorInfo()->PrintWarning(11, 1);
				}
			}
			break;
		case 'B':
			if (LineST[i][1] == 'H')
			{
			}
			else{
				GetItemFromLine(LineST[i], tBusName, OUT_GEN_Para[0], OUT_GEN_Loca[0]);
				GetItemFromLine(LineST[i], &tBaseKV, OUT_GEN_Para[1], OUT_GEN_Loca[1]);
				equipno = BPABusSearch(tBusName, tBaseKV);
				AddNewBusDyn(equipno);
				flag = m_BusDyn[equipno]->ReadOutLine(LineST[i]);
				if (flag != 1)
				{
					sprintf(ErrorMessage[0], "输出数据读取失败：%s", Line);
					cpGetErrorInfo()->PrintWarning(11, 1);
				}
			}
		}
		if (flag == 1)IsRead[i] = 1;
	}
#ifdef _DEBUG
	for (i = 0; i < LineTotal; i++)
	{
		if (IsRead[i]>0)continue;
		sprintf(ErrorMessage[0], "  不能识别的数据卡：%s", LineST[i]);
		cpGetErrorInfo()->PrintWarning(8, 1);
	}
#endif
	free(LineST); FreeArray(IsRead);
	cpGetErrorInfo()->CheckMessageType(8);
	return 1;
}

int BPA_DYNAMICMODELINFO::BPABusSearch(char *name, float baseKV)
{
	char lpszString[_MaxNameLen];
	strcpy(lpszString, name);
	ResetBPAName(lpszString, baseKV);
	return BusSearch(lpszString);
}

int BPA_DYNAMICMODELINFO::BPAGenSearch(char *name, float baseKV, char id)
{
	char lpszString[_MaxNameLen];
	char lpszString2[_MaxNameLen];
	strcpy(lpszString2, name);
	ResetBPAName(lpszString2, baseKV);
	sprintf(lpszString, "G-%s-ID='%c'", lpszString2, id);
	return EquipSearch(lpszString);
}

int BPA_DYNAMICMODELINFO::Net_ReadBPALine(char*tFileName)
{
	char Line[_MaxBPALineLen_];
	char(*LineST)[_MaxBPALineLen_]; int *IsRead = NULL; int *IsChange = NULL;
	int i, j, flag, LineTotal = 0;//增加该功能，支持一行读到尾，并舍弃

	//第一步，将文件读入到LineST数组中
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
			{//这里考虑到有些刚好_MaxBPALineLen_-2个字符的行
				flagAddLine = 1;//表示该行还未读完
			}
		}
		if (strlen(Line) > 3 && Line[0] == '(' && Line[1] == 'E' && Line[2] == 'N' && Line[3] == 'D')
			break;//终止符(END)
		if (strlen(Line) > 3 && Line[0] == '/' && (Line[1] == 'M' && Line[2] == 'V' && Line[3] == 'A'
			|| Line[1] == ' ' && Line[2] == 'M' && Line[3] == 'V' && Line[4] == 'A'))
		{//读入"/MVA_BASE = 1000\"卡//problem此处还存在问题
			char *ex;
			if ((ex = strrchr(Line, '=')) != NULL)
			{
				SetBMVA((float)atof(++ex));
			}
		}
		if (strlen(Line) == 0
			|| Line[0] == '-'
			|| Line[0] == 'C' || Line[0] == 'H' || Line[0] == '.' || Line[0] == ' '//注释 空行
			|| Line[0] == '(' || Line[0] == '[' || Line[0] == '/' || Line[0] == '\\' || Line[0] == '>'//控制命令
			|| Line[0] == 10 || Line[0] == 13)//换行符 回车符
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
	//读入数据
	BPABUS *tempBus;
	BPABRANCH *tempBranchBase;
	for (i = 0; i < LineTotal; i++)
		//for (i = LineTotal-1; i >=0; i--)
	{
		if (IsRead[i] != 0)continue;
		if (IsChange[i] != 0)continue;//这里输入的是常规数据，不包含修改卡
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
				else{
					BPABus[flag] = tempBus;
					flag = 1;
				}
			}
			break;
		case 'L':
			if (LineST[i][1] == '+')
				break;//跳过L+卡
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
					else{
						BPABranch[flag] = tempBranchBase;
						flag = 1;
					}
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
			if (LineST[i][1] == 'P')
				tempBranchBase = new TPBRANCH();
			else
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

	SortBusByName tSort(this);
	tSort.QikSort(iGetBusTotal());
	cpGetBusInfo()->ResetHashTable();

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
			BusIndex = BPABusSearch(Name1, BaseKv1);
			if (BusIndex < 0)
			{
				sprintf(ErrorMessage[0], "找不到母线：%s", LineST[i]);
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
					sprintf(ErrorMessage[0], "找不到线路：%s", LineST[i]);
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
					sprintf(ErrorMessage[0], "PZ卡目前无法处理发电机的修改，忽略卡片：%s", LineST[i]);
					cpGetErrorInfo()->PrintWarning(1, 1);
					flag = 0;
					break;
				}
				tZoneNo = cpGetAreaInfo()->iGetZoneNo(Zone);
				if (tZoneNo < 0)
				{
					sprintf(ErrorMessage[0], "找不到区域名%s, 忽略卡片: %s", Zone, LineST[i]);
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
		sprintf(ErrorMessage[0], "  不能识别的数据卡：%s", LineST[i]);
		cpGetErrorInfo()->PrintWarning(8, 1);
	}
#endif
	free(LineST); FreeArray(IsRead); FreeArray(IsChange);
	//cpGetErrorInfo()->CheckMessageType(8);//查看未识别卡片数
	cpGetErrorInfo()->CheckMessageType(1);//查看错误数据数
	cpGetErrorInfo()->CheckMessageType(6);//检查越限错误

	m_ChangeCode++;

	NetLink();
	return 1;
}

void BPA_DYNAMICMODELINFO::Update()
{
	int i;
	float tBMVA = GetBMVA();
	for (i = 0; i<iGetBusTotal(); i++)
	{
		cpGetBus(i)->m_fBusV = BusVolt[i];
		cpGetBus(i)->m_fBusSita = BusSita[i] * 180 / 3.1415926535897932384626433832795f;
		if (PPGen[i]>PGmax[i])
		{
			cpGetBus(i)->m_fPerr = (PPGen[i] - PGmax[i])*GetBMVA();
			PPGen[i] = PGmax[i];
		}
		//else if (PPGen[i]<PGmin[i])
		//{//BPA没有Pmin这项数据
		//	cpGetBus(i)->m_fPerr = (PPGen[i] - PGmin[i])*GetBMVA();
		//	PPGen[i] = PGmin[i];
		//}
		else
			cpGetBus(i)->m_fPerr = 0;
		if (QPGen[i]>QGmax[i])
		{
			cpGetBus(i)->m_fQerr = (QPGen[i] - QGmax[i])*GetBMVA();
			QPGen[i] = QGmax[i];
		}
		else if (QPGen[i] < QGmin[i])
		{
			cpGetBus(i)->m_fQerr = (QPGen[i] - QGmin[i])*GetBMVA();
			QPGen[i] = QGmin[i];
		}
		else
			cpGetBus(i)->m_fQerr = 0;
	}
	SumInfo();
	for (i = 0; i < iGetBranchTotal(); i++)
	{//分析线路潮流
		if (cpGetBranch(i)->iGetState() != 1)
			continue;
		cpGetBranch(i)->UpdateValue(this);
	}
	for (i = 0; i < iGetBusTotal(); i++)
	{
		cpGetBus(i)->UpdateValue(this);
	}
	for (i = 0; i < iGetEquipTotal(); i++)
	{
		if (cpGetEquip(i)->iGetState() != 1)
			continue;
		cpGetEquip(i)->UpdateValue(this);
	}
}

int BPA_DYNAMICMODELINFO::Area_ReadBPALine(char* PFLine)
{
	int i, k, AreaNo;
	float BaseKv;
	char Name[_MaxNameLen], Zone[_MaxNameLen], tArea[_MaxNameLen];
	GetItemFromLine(PFLine, tArea, AREA_Para[0], AREA_Loca[0]);
	ReplaceName(tArea, _MaxNameLen);
	tArea[_MaxNameLen - 1] = '\0';
	if (strlen(PFLine) > 3 && PFLine[2] == '+')
	{//续卡
		k = cpGetAreaInfo()->iGetAreaNo(tArea);
		if (k >= cpGetAreaInfo()->iGetAreaTotal())
		{
			sprintf(ErrorMessage[0], "A+卡没有对应的A卡 '%s' 不存在, 忽略该卡：", tArea);
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
			sprintf(ErrorMessage[0], " 节点 %s的基准电压为0，程序缺省将其设为100.0KV", Name);
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
			sprintf(ErrorMessage[0], "区域数目超过设定值%d，忽略该卡：", _MaxAreaNo);
			strcpy(ErrorMessage[1], PFLine);
			cpGetErrorInfo()->PrintWarning(6, 2);
			return 0;
		}
	}
	i = 0;
	while ((int)(strlen(PFLine)) > (AREA_Loca[4] + 3 * i + 2))
	{//这个地方能不限制长度，只要有就可以继续读入
		GetItemFromLine(PFLine, Zone, AREA_Para[4], AREA_Loca[4] + 3 * i);
		i++;
		if (IsBlank(Zone, _MaxNameLen))continue;
		if (cpGetAreaInfo()->cpGetArea(AreaNo)->ZoneTotal >= _MaxAreaZoneNo)
		{
			sprintf(ErrorMessage[0], "区域%s数目超过设定值%d，忽略多余的Zone：%s", cpGetAreaInfo()->cpGetArea(AreaNo)->ZoneName, _MaxAreaZoneNo, Zone);
			cpGetErrorInfo()->PrintWarning(0, 1);
			continue;
		}
		strcpy(cpGetAreaInfo()->cpGetArea(AreaNo)->ZoneName[cpGetAreaInfo()->cpGetArea(AreaNo)->ZoneTotal], Zone);
		cpGetAreaInfo()->cpGetArea(AreaNo)->ZoneTotal++;
	}
	return 1;
}

int BPA_DYNAMICMODELINFO::LN_ReadBPALine(char* Line)
{
	int busno;
	char Name[_MaxNameLen];
	float BaseKv;
	int i;
	for (i = 0; i < 5; i++)
	{
		GetItemFromLine(Line, Name, SWI_LN_Para[2 * i + 0], SWI_LN_Loca[2 * i + 0]);
		GetItemFromLine(Line, &BaseKv, SWI_LN_Para[2 * i + 1], SWI_LN_Loca[2 * i + 1]);
		ReplaceName(Name, _MaxNameLen);
		if (Name[0] != '\0')
		{
			busno = BPABusSearch(Name, BaseKv);
			if (busno < 0)
			{
				sprintf(ErrorMessage[0], "LN卡找不到母线:'%s-%7.1fkv': %s", Name, BaseKv, Line);
				cpGetErrorInfo()->PrintWarning(-1, 1);
				continue;
			}
			if (BPABus[busno] == NULL)
			{
				sprintf(ErrorMessage[0], "母线:'%s-%7.1fkv'使用非BPA卡读入，无法使用LN卡: %s", Name, BaseKv, Line);
				cpGetErrorInfo()->PrintWarning(-1, 1);
				continue;
			}
			BPABus[busno]->SetLNCard();
		}
	}
	return 1;
}

int BPA_DYNAMICMODELINFO::MC_ReadBPALine(char* Line)
{
	int busno;
	char Name[_MaxNameLen], cID;
	float BaseKv, pper, qper;
	GetItemFromLine(Line, Name, SWI_MC_Para[0], SWI_MC_Loca[0]);
	GetItemFromLine(Line, &BaseKv, SWI_MC_Para[1], SWI_MC_Loca[1]);
	GetItemFromLine(Line, &cID, SWI_MC_Para[2], SWI_MC_Loca[2]);
	ReplaceName(Name, _MaxNameLen);
	busno = BPABusSearch(Name, BaseKv);
	if (busno < 0)
	{
		sprintf(ErrorMessage[0], "MC卡找不到母线:'%s-%7.1fkv': %s", Name, BaseKv, Line);
		cpGetErrorInfo()->PrintWarning(-1, 1);
		return 0;
	}
	if (BPABus[busno] == NULL)
	{
		sprintf(ErrorMessage[0], "母线:'%s-%7.1fkv'使用非BPA卡读入，无法使用MC卡: %s", Name, BaseKv, Line);
		cpGetErrorInfo()->PrintWarning(-1, 1);
		return 0;
	}
	GetItemFromLine(Line, &pper, SWI_MC_Para[4], SWI_MC_Loca[4]);
	GetItemFromLine(Line, &qper, SWI_MC_Para[5], SWI_MC_Loca[5]);
	if (fabs(pper) < 0.001)pper = 1;
	if (fabs(qper) < 0.001)qper = 1;
	if (fabs(pper - 1) > 0.001 || fabs(qper - 1) > 0.001)
	{
		sprintf(ErrorMessage[0], "MC卡中P(pu)和Q(pu)必须为1: %s", Line);
		cpGetErrorInfo()->PrintWarning(-1, 1);
		return 0;
	}
	int flag;
	BPAGENERATOR *tGen;
	tGen = new BPAGENERATOR;
	sprintf(tGen->Name, "G-%s-ID='%c'", BPABus[busno]->Name, cID);
	strcpy(tGen->BusName, BPABus[busno]->Name);
	tGen->m_fGenP = BPABus[busno]->m_fGenP;
	tGen->m_fGenQ = BPABus[busno]->m_fGenQ;
	tGen->m_fPper = pper;
	tGen->m_fQper = qper;
	tGen->pBus = BPABus[busno];
	flag = cpGetEquipInfo()->AddNewEquip(tGen);
	if (flag < 0)
	{
		delete tGen;
		return 0;
	}
	DYNAMIC_MODEL_BASE*tModel;
	tModel = new BPAMCMODEL;
	flag = tModel->ReadLine(Line);
	if (flag != 1)
	{
		delete tModel;
		sprintf(ErrorMessage[0], "动态模型数据读取失败：%s", Line);
		cpGetErrorInfo()->PrintWarning(15, 1);
		return 0;
	}
	strcpy(tModel->EquipmentName, tGen->Name);
	flag = InsertNewDynamicModel(tModel);
	if (flag < 0)
	{
		delete tModel;
		sprintf(ErrorMessage[0], "添加动态模型失败：%s", Line);
		cpGetErrorInfo()->PrintWarning(15, 1);
		return 0;
	}
	BPABus[busno]->m_fGenPper += pper;
	BPABus[busno]->m_fGenQper += qper;

	return 1;
}

int BPA_DYNAMICMODELINFO::GH_ReadBPALine(char* Line)
{
	GetItemFromLine(Line, GHName, OUT_GH_Para[0], OUT_GH_Loca[0]);
	GetItemFromLine(Line, &GHBaseKV, OUT_GH_Para[1], OUT_GH_Loca[1]);
	GetItemFromLine(Line, &GHID, OUT_GH_Para[2], OUT_GH_Loca[2]);
	ReplaceName(GHName, _MaxNameLen);
	return 1;
}

int BPA_DYNAMICMODELINFO::FF_ReadBPALine(char* Line)
{
	float DT, ENDT;
	GetItemFromLine(Line, &DT, SWI_FF_Para[0], SWI_FF_Loca[0]);
	GetItemFromLine(Line, &ENDT, SWI_FF_Para[1], SWI_FF_Loca[1]);
	if (fabs(DT) > 0.001)
		Step0 = DT / 50;
	if (fabs(ENDT) > 0.001)
		TotalTime = ENDT / 50;
	return 1;
}


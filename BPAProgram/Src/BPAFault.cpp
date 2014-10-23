
#include "BPAFault.h"
#include "BPAReadWrite.h"
#include "../../DigAnalTE/DigAnalTE/CommonFunction/ErrorInfo.h"

int BPA_FAULTINFO::ReadFile(char*file)
{
	FILE* fpfile = NULL;
	OpenFile(fpfile, file, "rb");
	if (fpfile == NULL)
		return 0;
	int flag, type, flagNoSupport;
	char Line[_MaxLineLen];
	DYFAULTBASE*tFault;
	while (fgets(Line, _MaxLineLen, fpfile))
	{
		if (strlen(Line) > 1 && Line[0] == '9' && Line[1] == '0')
			break;//终止符90
		flagNoSupport = 0;
		if (Line[0] == 'L'&&Line[1] == 'S')
		{
			GetItemFromLine(Line, &type, "I2", 35);
			if (type == 1 || type == -1)
			{
				tFault = new BPAFAULTLINE;
				flag = tFault->ReadLine(Line);
				if (flag != 1)
				{
					delete tFault;
					sprintf(ErrorMessage[0], "故障数据读取失败：%s", Line);
					cpGetErrorInfo()->PrintWarning(11, 1);
					continue;
				}
				flag = AddNewFault(tFault);
				if (flag < 0)
				{
					delete tFault;
					sprintf(ErrorMessage[0], "添加故障数据失败：%s", Line);
					cpGetErrorInfo()->PrintWarning(12, 1);
					continue;
				}
			}
			else{
				flagNoSupport = 1;
			}
		}
		if (Line[0] == 'F'&&Line[1] == 'L'&&Line[2] == 'T')
		{
			GetItemFromLine(Line, &type, "I2", 31);
			flagNoSupport = 1;
		}
		if (flagNoSupport == 1)
		{
			sprintf(ErrorMessage[0], "不支持的故障卡：%s", Line);
			cpGetErrorInfo()->PrintWarning(13, 1);
		}
	}
	cpGetErrorInfo()->CheckMessageType(11);
	cpGetErrorInfo()->CheckMessageType(12);
	cpGetErrorInfo()->CheckMessageType(13);
	return 1;
}

float BPAFAULTLINE::GetNextTime()
{
	if (mStatus == 0)
		return Time / 50;
	return 999.;
}

int BPAFAULTLINE::GetIsProcess()
{
	if (mStatus != 0)
		return 1;
	return 0;
}

int BPAFAULTLINE::Initial()
{
	BranchIndex = pDyn->BranchSearch(BranchName);
	if (BranchIndex < 0)
	{
		return 0;
	}
	pBranch = pDyn->cpGetBranch(BranchIndex);
	if (pBranch == NULL)
		return 0;
	mStatus = 0;
	FaultBusNo = pBranch->iGetFromBusNo();
	real Z;
	Z = Fault_R*Fault_R + Fault_X*Fault_X;
	Yr = Fault_R / Z;
	Yi = -Fault_X / Z;
	return 1;
}

int BPAFAULTLINE::ReadLine(char*line)
{
	GetItemFromLine(line, BusName1, SWI_LS_LINE_Para[0], SWI_LS_LINE_Loca[0]);
	GetItemFromLine(line, &BaseKV1, SWI_LS_LINE_Para[1], SWI_LS_LINE_Loca[1]);
	GetItemFromLine(line, BusName2, SWI_LS_LINE_Para[2], SWI_LS_LINE_Loca[2]);
	GetItemFromLine(line, &BaseKV2, SWI_LS_LINE_Para[3], SWI_LS_LINE_Loca[3]);
	GetItemFromLine(line, &ID, SWI_LS_LINE_Para[4], SWI_LS_LINE_Loca[4]);
	GetItemFromLine(line, &type, SWI_LS_LINE_Para[5], SWI_LS_LINE_Loca[5]);
	GetItemFromLine(line, &Time, SWI_LS_LINE_Para[6], SWI_LS_LINE_Loca[6]);
	GetItemFromLine(line, &Fault_R, SWI_LS_LINE_Para[7], SWI_LS_LINE_Loca[7]);
	GetItemFromLine(line, &Fault_X, SWI_LS_LINE_Para[8], SWI_LS_LINE_Loca[8]);
	ReplaceName(BusName1, _MaxNameLen);
	ReplaceName(BusName2, _MaxNameLen);
	GenerateBPABranchName(BranchName, BusName1, BaseKV1, BusName2, BaseKV2, ID);
	int flagerr = 0;
	if (fabs(Time) < 0.001)Time = 0.;
	if (Time < -0.001)
	{
		sprintf(ErrorMessage[0], "ERROR: 三相短路数据中时间填写错误，断开时间必须为正");
		cpGetErrorInfo()->PrintWarning(-1, 1);
		flagerr = 1;
	}
	if (fabs(Fault_R) < 0.0001)Fault_R = 0.;
	if (fabs(Fault_X) < 0.0001)Fault_X = 0.0001f;
	if (type != 1 && type != -1)
	{
		sprintf(ErrorMessage[0], "ERROR: 三相短路数据目前只支持MDE=1和-1的情况");
		cpGetErrorInfo()->PrintWarning(-1, 1);
		flagerr = 1;
	}
	flagCut = 0;
	if (line[3] == '-' || line[17] == '-')
	{
		flagCut = 1;
		if (line[3] != '-' || line[17] != '-')
		{
			sprintf(ErrorMessage[0], "ERROR: 三相短路数据目前只支持两端同时断开的情况");
			cpGetErrorInfo()->PrintWarning(-1, 1);
			flagerr = 1;
		}
	}
	if (flagerr == 1)
	{
		sprintf(ErrorMessage[0], "ERROR: 读卡失败: %s", line);
		cpGetErrorInfo()->PrintError(1);
		flagerr = 1;
	}
	return 1;
}

void BPAFAULTLINE::PrintInfo(char*Line)
{
	sprintf(Line, "线路'%s'三项短路故障数据", BranchName);
}

void BPAFAULTLINE::ProcessFault()
{
	if (type == 1)
	{
		pDyn->ModifyNetMatrix(FaultBusNo, FaultBusNo, Yr, Yi);
	}
	else if (type == -1)
	{
		pDyn->ModifyNetMatrix(FaultBusNo, FaultBusNo, -Yr, -Yi);
	}
	if (flagCut == 1)
		pDyn->DeleteBranch(BranchIndex);
	mStatus = 1;
}

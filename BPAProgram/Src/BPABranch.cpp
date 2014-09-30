
#include "BPABranch.h"
#include "BPAReadWrite.h"
#include "../../DigAnalTE/DigAnalTE/CommonFunction/ErrorInfo.h"
#include "../../DigAnalTE/DigAnalTE/NetWorkInfo/NetWorkInfo.h"

int BPABRANCH::ReadLine(char*line)
{//读入BPA文件
	int flag;
	if (line[1] == '+')
	{
		GetItemFromLine(line, &Bk1, LPLUS_Para[7], LPLUS_Loca[7]);
		GetItemFromLine(line, &Bk2, LPLUS_Para[8], LPLUS_Loca[8]);
		return 1;
	}
	flag = subReadBPALine(line);
	GenerateBPABranchName(Name, BusName1, BaseKv1, BusName2, BaseKv2, ID);
	ResetBPAName(BusName1, BaseKv1);
	ResetBPAName(BusName2, BaseKv2);
	return flag;
}

void BPABRANCH::subJacElement(NETWORKINFO*pNet)
{
	real Z, sbase;
	Z = R*R + X*X;
	sbase = pNet->GetBMVA();
	Y12r = -R / (Z*Ratio1*Ratio2); Y12i = X / (Z*Ratio1*Ratio2); Y21r = Y12r; Y21i = Y12i;
	Y11r = R / (Z*Ratio1*Ratio1) + G1 / (Ratio1*Ratio2);
	Y11i = -X / (Z*Ratio1*Ratio1) + B1 / (Ratio1*Ratio2) - Bk1 / sbase / (Ratio1*Ratio2);
	Y22r = R / (Z*Ratio2*Ratio2) + G2 / (Ratio1*Ratio2);
	Y22i = -X / (Z*Ratio2*Ratio2) + B2 / (Ratio1*Ratio2) - Bk2 / sbase / (Ratio1*Ratio2);
}

int LBRANCH::subReadBPALine(char*Line)
{
	char CID[2];
	GetItemFromLine(Line, (void*)(Owner), LBRANCH_Para[0], LBRANCH_Loca[0]);
	GetItemFromLine(Line, (void*)(BusName1), LBRANCH_Para[1], LBRANCH_Loca[1]);
	GetItemFromLine(Line, (void*)(&BaseKv1), LBRANCH_Para[2], LBRANCH_Loca[2]);
	GetItemFromLine(Line, (void*)(&m_nMeter), LBRANCH_Para[3], LBRANCH_Loca[3]);
	GetItemFromLine(Line, (void*)(BusName2), LBRANCH_Para[4], LBRANCH_Loca[4]);
	GetItemFromLine(Line, (void*)(&BaseKv2), LBRANCH_Para[5], LBRANCH_Loca[5]);
	GetItemFromLine(Line, (void*)(&CID), LBRANCH_Para[6], LBRANCH_Loca[6]);
	ReplaceName(BusName1, _MaxNameLen);
	ReplaceName(BusName2, _MaxNameLen);
	if (m_nMeter != 2)m_nMeter = 1;
	ID = CID[0];
#ifdef _DEBUG
	if (fabs(BaseKv1 - BaseKv2) > 0.1)
	{
		sprintf(ErrorMessage[0], "L卡两端节点电压等级有差别，这样会造成计算错误, 暂时不做改变：");
		strcpy(ErrorMessage[1], Line);
		cpGetErrorInfo()->PrintWarning(0, 2);
	}
#endif
	GetItemFromLine(Line, (void*)(&Imax), LBRANCH_Para[7], LBRANCH_Loca[7]);
	GetItemFromLine(Line, (void*)(&Ncnt), LBRANCH_Para[8], LBRANCH_Loca[8]);
	GetItemFromLine(Line, (void*)(&R), LBRANCH_Para[9], LBRANCH_Loca[9]);
	GetItemFromLine(Line, (void*)(&X), LBRANCH_Para[10], LBRANCH_Loca[10]);
	GetItemFromLine(Line, (void*)(&G1), LBRANCH_Para[11], LBRANCH_Loca[11]);
	GetItemFromLine(Line, (void*)(&B1), LBRANCH_Para[12], LBRANCH_Loca[12]);
	GetItemFromLine(Line, (void*)(&Length), LBRANCH_Para[13], LBRANCH_Loca[13]);
	GetItemFromLine(Line, (void*)(&Description), LBRANCH_Para[14], LBRANCH_Loca[14]);
	GetItemFromLine(Line, (void*)(&m_iMonth), LBRANCH_Para[15], LBRANCH_Loca[15]);
	GetItemFromLine(Line, (void*)(&m_iYear), LBRANCH_Para[16], LBRANCH_Loca[16]);
	GetItemFromLine(Line, (void*)(&m_jMonth), LBRANCH_Para[17], LBRANCH_Loca[17]);
	GetItemFromLine(Line, (void*)(&m_jYear), LBRANCH_Para[18], LBRANCH_Loca[18]);
	G2 = G1; B2 = B1;
	if (fabs(X) < 0.000099)
	{
		strcpy(ErrorMessage[0], Line);
		sprintf(ErrorMessage[1], "该支路的电抗太小。X = %8.5f", X);
		cpGetErrorInfo()->PrintWarning(0, 2);
		if (fabs(X) < 0.00005) X = 0.00005f;
	}
	if (fabs(X) - 0.0000001 < fabs(R))
	{
		strcpy(ErrorMessage[0], Line);
		sprintf(ErrorMessage[1], "该支路的电阻大于等于电抗。R = %8.5f X = %8.5f", R, X);
		cpGetErrorInfo()->PrintWarning(0, 2);
	}
	if (fabs(R*R + X*X) < 0.0000000099)
	{
		sprintf(ErrorMessage[0], "线路阻抗过小，修改阻抗X=0.0001：%s", Line);
		cpGetErrorInfo()->PrintWarning(0, 1);
		X = 0.0001f;
	}
	Smax = (float)(Imax*sqrtf(3)*BaseKv1 / 1000); if (Smax < 0.00001){ Smax = 999999.0; }
	Ratio1 = 1; Ratio2 = 1;
	return 1;
}

int EBRANCH::subReadBPALine(char*Line)
{
	char CID[2];
	GetItemFromLine(Line, (void*)(Owner), EBRANCH_Para[0], EBRANCH_Loca[0]);
	GetItemFromLine(Line, (void*)(BusName1), EBRANCH_Para[1], EBRANCH_Loca[1]);
	GetItemFromLine(Line, (void*)(&BaseKv1), EBRANCH_Para[2], EBRANCH_Loca[2]);
	GetItemFromLine(Line, (void*)(&m_nMeter), EBRANCH_Para[3], EBRANCH_Loca[3]);
	GetItemFromLine(Line, (void*)(BusName2), EBRANCH_Para[4], EBRANCH_Loca[4]);
	GetItemFromLine(Line, (void*)(&BaseKv2), EBRANCH_Para[5], EBRANCH_Loca[5]);
	GetItemFromLine(Line, (void*)(&CID), EBRANCH_Para[6], EBRANCH_Loca[6]);
	ReplaceName(BusName1, _MaxNameLen);
	ReplaceName(BusName2, _MaxNameLen);
	if (m_nMeter != 2)m_nMeter = 1;
	ID = CID[0];
#ifdef _DEBUG
	if (fabs(BaseKv1 - BaseKv2) > 0.1)
	{
		sprintf(ErrorMessage[0], "L卡两端节点电压等级有差别，这样会造成计算错误, 暂时不做改变：");
		strcpy(ErrorMessage[1], Line);
		cpGetErrorInfo()->PrintWarning(0, 2);
	}
#endif
	GetItemFromLine(Line, (void*)(&Imax), EBRANCH_Para[7], EBRANCH_Loca[7]);
	GetItemFromLine(Line, (void*)(&Ncnt), EBRANCH_Para[8], EBRANCH_Loca[8]);
	GetItemFromLine(Line, (void*)(&R), EBRANCH_Para[9], EBRANCH_Loca[9]);
	GetItemFromLine(Line, (void*)(&X), EBRANCH_Para[10], EBRANCH_Loca[10]);
	GetItemFromLine(Line, (void*)(&G1), EBRANCH_Para[11], EBRANCH_Loca[11]);
	GetItemFromLine(Line, (void*)(&B1), EBRANCH_Para[12], EBRANCH_Loca[12]);
	GetItemFromLine(Line, (void*)(&G2), EBRANCH_Para[13], EBRANCH_Loca[13]);
	GetItemFromLine(Line, (void*)(&B2), EBRANCH_Para[14], EBRANCH_Loca[14]);
	GetItemFromLine(Line, (void*)(&m_iMonth), EBRANCH_Para[15], EBRANCH_Loca[15]);
	GetItemFromLine(Line, (void*)(&m_iYear), EBRANCH_Para[16], EBRANCH_Loca[16]);
	GetItemFromLine(Line, (void*)(&m_jMonth), EBRANCH_Para[17], EBRANCH_Loca[17]);
	GetItemFromLine(Line, (void*)(&m_jYear), EBRANCH_Para[18], EBRANCH_Loca[18]);
	if (fabs(X) < 0.000099)
	{
		strcpy(ErrorMessage[0], Line);
		sprintf(ErrorMessage[1], "该支路的电抗太小。X = %8.5f", X);
		cpGetErrorInfo()->PrintWarning(0, 2);
	}
	if (fabs(R*R + X*X) < 0.0000000099)
	{
		sprintf(ErrorMessage[0], "线路阻抗过小，修改阻抗X=0.0001：%s", Line);
		cpGetErrorInfo()->PrintWarning(0, 1);
		X = 0.0001f;
	}
	Smax = (float)(Imax*sqrtf(3)*BaseKv1 / 1000); if (Smax < 0.00001){ Smax = 999999.0; }
	Ratio1 = 1; Ratio2 = 1;
	return 1;
}

int TBRANCH::subReadBPALine(char*Line)
{
	char CID[2];
	GetItemFromLine(Line, (void*)(Owner), TBRANCH_Para[0], TBRANCH_Loca[0]);
	GetItemFromLine(Line, (void*)(BusName1), TBRANCH_Para[1], TBRANCH_Loca[1]);
	GetItemFromLine(Line, (void*)(&BaseKv1), TBRANCH_Para[2], TBRANCH_Loca[2]);
	GetItemFromLine(Line, (void*)(&m_nMeter), TBRANCH_Para[3], TBRANCH_Loca[3]);
	GetItemFromLine(Line, (void*)(BusName2), TBRANCH_Para[4], TBRANCH_Loca[4]);
	GetItemFromLine(Line, (void*)(&BaseKv2), TBRANCH_Para[5], TBRANCH_Loca[5]);
	GetItemFromLine(Line, (void*)(&CID), TBRANCH_Para[6], TBRANCH_Loca[6]);
	ReplaceName(BusName1, _MaxNameLen);
	ReplaceName(BusName2, _MaxNameLen);
	if (m_nMeter != 2)m_nMeter = 1;
	ID = CID[0];
	GetItemFromLine(Line, (void*)(&Imax), TBRANCH_Para[7], TBRANCH_Loca[7]);
	GetItemFromLine(Line, (void*)(&Ncnt), TBRANCH_Para[8], TBRANCH_Loca[8]);
	GetItemFromLine(Line, (void*)(&R), TBRANCH_Para[9], TBRANCH_Loca[9]);
	GetItemFromLine(Line, (void*)(&X), TBRANCH_Para[10], TBRANCH_Loca[10]);
	GetItemFromLine(Line, (void*)(&G1), TBRANCH_Para[11], TBRANCH_Loca[11]);
	GetItemFromLine(Line, (void*)(&B1), TBRANCH_Para[12], TBRANCH_Loca[12]);
	GetItemFromLine(Line, (void*)(&TK1), TBRANCH_Para[13], TBRANCH_Loca[13]);
	GetItemFromLine(Line, (void*)(&TK2), TBRANCH_Para[14], TBRANCH_Loca[14]);
	GetItemFromLine(Line, (void*)(&m_iMonth), TBRANCH_Para[15], TBRANCH_Loca[15]);
	GetItemFromLine(Line, (void*)(&m_iYear), TBRANCH_Para[16], TBRANCH_Loca[16]);
	GetItemFromLine(Line, (void*)(&m_jMonth), TBRANCH_Para[17], TBRANCH_Loca[17]);
	GetItemFromLine(Line, (void*)(&m_jYear), TBRANCH_Para[18], TBRANCH_Loca[18]);
	if (fabs(X) < 0.000099)
	{
		strcpy(ErrorMessage[0], Line);
		sprintf(ErrorMessage[1], "该支路的电抗太小。X = %8.5f", X);
		cpGetErrorInfo()->PrintWarning(0, 2);
	}
	if (fabs(TK1) < 0.000099) TK1 = BaseKv1;
	if (fabs(TK2) < 0.000099) TK2 = BaseKv2;
	Ratio1 = TK1 / BaseKv1; Ratio2 = TK2 / BaseKv2;
	if (Ratio1 > 1.11 || Ratio1<0.89 || Ratio2>1.11 || Ratio2 < 0.89)
	{
		strcpy(ErrorMessage[0], Line);
		sprintf(ErrorMessage[1], "该卡中变压器分接头偏离正常值过大。");
		cpGetErrorInfo()->PrintWarning(0, 2);
	}
	G1 = G1 / 2; B1 = B1 / 2;
	G2 = G1; B2 = B1;
	if (fabs(R*R + X*X) < 0.0000000099)
	{
		strcpy(ErrorMessage[0], Line);
		sprintf(ErrorMessage[1], "线路阻抗过小，修改阻抗X=0.0001。", Line);
		cpGetErrorInfo()->PrintWarning(0, 2);
		X = 0.0001f;
	}
	Smax = Imax; if (Smax < 0.00001){ Smax = 999999.0; }
	//	Smax=999999.0;
	return 1;
}

void BPABRANCH::OutputPFOFile(FILE*fp, int nDirtn)
{
	float Pin, Qin, Pout, Qout, Ploss, Qloss;
	Pin = IP;
	Qin = IQ;
	Pout = JP;
	Qout = JQ;
	Ploss = Pin + Pout;
	Qloss = Qin + Qout;
	if (nDirtn == 2)
	{
		Pin = Pout; Qin = Qout;
	}
	if (fabs(Pin) < 0.001)Pin = 0.;
	if (fabs(Qin) < 0.001)Qin = 0.;
	if (fabs(Ploss) < 0.001)Ploss = 0.;
	if (fabs(Qloss) < 0.001)Qloss = 0.;

	char OutLine[_MaxLineLen];
	char BusName[9];
	float baseKV;
	if (nDirtn == 1)
	{
		strncpy(BusName, BusName2, 8); ReplaceName(BusName, 9);
		baseKV = BaseKv2;
	}
	else{
		strncpy(BusName, BusName1, 8); ReplaceName(BusName, 9);
		baseKV = BaseKv1;
	}
	sprintf(OutLine, "       %s%6.1f   %c           %8.1f线路有功%8.1f线路无功%7.3f有功损耗%8.2f无功损耗 %s\n",// %8.3f充电功率\n",
		BusName, baseKV,
		ID,
		Pin, Qin, Ploss, Qloss,
		Owner);
	fprintf(fp, OutLine);
}

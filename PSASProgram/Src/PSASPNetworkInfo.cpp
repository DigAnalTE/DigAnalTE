
#include "PSASPNetWorkInfo.h"
#include "../../DigAnalTE/DigAnalTE/CommonFunction/ErrorInfo.h"
#include <io.h>

PSASP_NETWORKINFO::PSASP_NETWORKINFO()
{
	PSASPBusTotal = 0;
	memset(PSASPBus, 0, _MaxBusNo*sizeof(PSASPBUS*));
	PSASPACLineTotal = 0;
	memset(PSASPACLine, 0, _MaxBranchNo*sizeof(PSASPACLINE*));
	PSASPTransTotal = 0;
	memset(PSASPTrans, 0, _MaxBranchNo*sizeof(PSASPTRANS*));
	PSASPDCLineTotal = 0;
	memset(PSASPDCLine, 0, _MaxBranchNo*sizeof(PSASPDCLINE*));
	PSASPGenTotal = 0;
	memset(PSASPGen, 0, _MaxEquipNo*sizeof(PSASPGENERATOR*));
	PSASPLoadTotal = 0;
	memset(PSASPLoad, 0, _MaxEquipNo*sizeof(PSASPLOAD*));
	PSASPAreaTotal = 0;
	memset(PSASPArea, 0, _MaxAreaNo*sizeof(PSASPAREA*));
}

int PSASP_NETWORKINFO::ReadFile(char*dir)
{
	char CtrlFileL0[_MAX_PATH];
	char BusFileL1[_MAX_PATH];
	char ACLineFileL2[_MAX_PATH];
	char TranFileL3[_MAX_PATH];
	char DCLineFileL4[_MAX_PATH];
	char GenFileL5[_MAX_PATH];
	char LoadFileL6[_MAX_PATH];
	char AreaFileL7[_MAX_PATH];
	strcpy(m_DataDir, dir);
	char *ex;
	ex = strrchr(m_DataDir, '.');
	if (ex != NULL)
	{
		*ex = '\0';
	}
	strcpy(CtrlFileL0, m_DataDir);     strcat(CtrlFileL0, ".L0");
	strcpy(BusFileL1, m_DataDir);      strcat(BusFileL1, ".L1");
	strcpy(ACLineFileL2, m_DataDir);   strcat(ACLineFileL2, ".L2");
	strcpy(TranFileL3, m_DataDir);     strcat(TranFileL3, ".L3");
	strcpy(DCLineFileL4, m_DataDir);   strcat(DCLineFileL4, ".L4");
	strcpy(GenFileL5, m_DataDir);      strcat(GenFileL5, ".L5");
	strcpy(LoadFileL6, m_DataDir);     strcat(LoadFileL6, ".L6");
	strcpy(AreaFileL7, m_DataDir);     strcat(AreaFileL7, ".L7");
	if (ReadL0File(CtrlFileL0) != 1) return 0;
	if (ReadL1File(BusFileL1) != 1) return 0;
	if (PSASPBusTotal != m_iNBB)
		return 0;
	if (ReadL2File(ACLineFileL2) != 1) return 0;
	if (PSASPACLineTotal != m_iNLL)
		return 0;
	if (ReadL3File(TranFileL3) != 1) return 0;
	if (PSASPTransTotal != m_iNTT)
		return 0;
	if (ReadL4File(DCLineFileL4) != 1) return 0;
	if (PSASPDCLineTotal != m_iNDC)
		return 0;
	if (ReadL5File(GenFileL5) != 1) return 0;
	if (PSASPGenTotal != m_iNGG)
		return 0;
	if (ReadL6File(LoadFileL6) != 1) return 0;
	if (PSASPLoadTotal != m_iNLoad)
		return 0;
	if (ReadL7File(AreaFileL7) != 1) return 0;
	return 1;
}

int PSASP_NETWORKINFO::ReadL0File(char*file)
{
	if (_access(file, 0) == -1)
	{
		return 0;
	}

	FILE*	fpPFfile;
	char Line[_MaxLineLen];
	fpPFfile = fopen(file, "rb");
	if (fpPFfile == NULL)
		return 0;

	int flag;
	if (fgets(Line, _MaxLineLen, fpPFfile) == NULL)	return 0;
	flag = sscanf(Line, "%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d,",
		&m_iNBB, &m_iNLL, &m_iNTT, &m_iNDC,
		&m_iNGG, &m_iNLoad, &m_iNArea,
		&m_iNUD, &m_iNEQ, &m_iNSS, &m_iNFACT);
	if (flag != 11) return 0;
	if (fgets(Line, _MaxLineLen, fpPFfile) == NULL)	return 0;
	flag = sscanf(Line, "%f, %f, %f, %f, %d, %d, %d, %d, %d, %d, %d,",
		&m_SB, &m_Vmax, &m_Vmin, &m_Eps,
		&m_iMeth, &m_iIter, &m_iArea,
		&m_iUD, &m_iMeq, &m_iISS, &m_iNUP);
	if (flag != 11) return 0;
	BMVA = m_SB;

	fclose(fpPFfile);

	if (m_iNBB > _MaxBusNo)
	{
		sprintf(ErrorMessage[0], "母线数目%d超过设定值%d", m_iNBB, _MaxBusNo);
		cpGetErrorInfo()->PrintError(1);
		return 0;
	}
	if (m_iNLL + m_iNTT > _MaxBranchNo)
	{
		sprintf(ErrorMessage[0], "线路和变压器总数目%d超过设定值%d", m_iNLL + m_iNTT, _MaxBranchNo);
		cpGetErrorInfo()->PrintError(1);
		return 0;
	}
	if (m_iNDC + m_iNGG + m_iNLoad > _MaxEquipNo)
	{
		sprintf(ErrorMessage[0], "直流、发电机、负荷总数目%d超过设定值%d", m_iNDC + m_iNGG + m_iNLoad, _MaxEquipNo);
		cpGetErrorInfo()->PrintError(1);
		return 0;
	}
	if (m_iNArea > _MaxAreaNo-1)
	{
		sprintf(ErrorMessage[0], "分区数目%d超过设定值%d", m_iNArea, _MaxAreaNo-1);
		cpGetErrorInfo()->PrintError(1);
		return 0;
	}

	return 1;
}

int PSASP_NETWORKINFO::ReadL1File(char*file)
{
	int i, flag, busno;
	busno = m_iNBB;
	if (busno <= 0) return 1;

	if (_access(file, 0) == -1)
	{
		return 0;
	}

	FILE*	fpPFfile;
	char Line[_MaxLineLen];
	fpPFfile = fopen(file, "rb");
	if (fpPFfile == NULL)
		return 0;

	PSASPBUS *tBus;
	for (i = 0; i < busno; i++)
	{
		if (fgets(Line, _MaxLineLen, fpPFfile) == NULL)	return 0;

		tBus = new PSASPBUS;
		flag = tBus->ReadLine(Line);
		if (flag != 1)
		{
			delete tBus;
			continue;
		}
		flag = cpGetBusInfo()->AddNewBus(tBus);
		if (flag < 0)
		{//返回添加成功后的序号值//负数表示添加失败
			delete tBus;
			continue;
		}
		if (tBus->m_fBusVmax < 0.001)
			tBus->m_fBusVmax = m_Vmax;
		if (tBus->m_fBusVmin < 0.001)
			tBus->m_fBusVmin = m_Vmin;

		tBus->BusNo = PSASPBusTotal;
		PSASPBus[PSASPBusTotal] = tBus;
		PSASPBusTotal++;
	}

	fclose(fpPFfile);
	return 1;
}

int PSASP_NETWORKINFO::ReadL2File(char*file)
{
	int i, flag, lineno;
	lineno = m_iNLL;
	if (lineno <= 0) return 1;

	if (_access(file, 0) == -1)
	{
		return 0;
	}

	FILE*	fpPFfile;
	char Line[_MaxLineLen];
	fpPFfile = fopen(file, "rb");
	if (fpPFfile == NULL)
		return 0;

	PSASPACLINE *tLine;

	for (i = 0; i < lineno; i++)
	{
		if (fgets(Line, _MaxLineLen, fpPFfile) == NULL)	return 0;

		tLine = new PSASPACLINE;
		flag = tLine->ReadLine(Line);
		if (flag == 1)
		{
			if (tLine->m_iIname <= 0 || tLine->m_iIname > m_iNBB)
				flag = 0;
			if (tLine->m_iJname <= 0 || tLine->m_iJname > m_iNBB)
				flag = 0;
		}
		if (flag != 1)
		{
			delete tLine;
			continue;
		}
		flag = cpGetBranchInfo()->AddNewBranch(tLine);
		if (flag < 0)
		{//返回添加成功后的序号值//负数表示添加失败
			delete tLine;
			continue;
		}
		strcpy(tLine->BusName1, PSASPBus[tLine->m_iIname - 1]->Name);
		strcpy(tLine->BusName2, PSASPBus[tLine->m_iJname - 1]->Name);

		PSASPACLine[PSASPACLineTotal] = tLine;
		PSASPACLineTotal++;
	}

	fclose(fpPFfile);
	return 1;
}

int PSASP_NETWORKINFO::ReadL3File(char*file)
{
	int i, flag, tranno;
	tranno = m_iNTT;
	if (tranno <= 0) return 1;

	if (_access(file, 0) == -1)
	{
		return 0;
	}

	FILE*	fpPFfile;
	char Line[_MaxLineLen];
	fpPFfile = fopen(file, "rb");
	if (fpPFfile == NULL)
		return 0;

	PSASPTRANS* tTrans;

	for (i = 0; i < tranno; i++)
	{
		if (fgets(Line, _MaxLineLen, fpPFfile) == NULL)	return 0;

		tTrans = new PSASPTRANS;
		flag = tTrans->ReadLine(Line);
		if (flag == 1)
		{
			if (tTrans->m_iIname <= 0 || tTrans->m_iIname > m_iNBB)
				flag = 0;
			if (tTrans->m_iJname <= 0 || tTrans->m_iJname > m_iNBB)
				flag = 0;
		}
		if (flag != 1)
		{
			delete tTrans;
			continue;
		}
		flag = cpGetBranchInfo()->AddNewBranch(tTrans);
		if (flag < 0)
		{//返回添加成功后的序号值//负数表示添加失败
			delete tTrans;
			continue;
		}
		strcpy(tTrans->BusName1, PSASPBus[tTrans->m_iIname - 1]->Name);
		strcpy(tTrans->BusName2, PSASPBus[tTrans->m_iJname - 1]->Name);

		PSASPTrans[PSASPTransTotal] = tTrans;
		PSASPTransTotal++;
	}

	fclose(fpPFfile);
	return 1;
}

int PSASP_NETWORKINFO::ReadL4File(char*file)
{
	int i, flag, dcno;
	dcno = m_iNDC;
	if (dcno <= 0) return 1;

	if (_access(file, 0) == -1)
	{
		return 0;
	}

	FILE*	fpPFfile;
	char Line1[_MaxLineLen];
	char Line2[_MaxLineLen];
	char Line3[_MaxLineLen];
	char Line4[_MaxLineLen];
	char Line5[_MaxLineLen];
	fpPFfile = fopen(file, "rb");
	if (fpPFfile == NULL)
		return 0;

	PSASPDCLINE *tDCLine;

	for (i = 0; i < dcno; i++)
	{
		tDCLine = new PSASPDCLINE;
		if (fgets(Line1, _MaxLineLen, fpPFfile) == NULL)	return 0;
		if (fgets(Line2, _MaxLineLen, fpPFfile) == NULL)	return 0;
		if (fgets(Line3, _MaxLineLen, fpPFfile) == NULL)	return 0;
		if (fgets(Line4, _MaxLineLen, fpPFfile) == NULL)	return 0;
		if (fgets(Line5, _MaxLineLen, fpPFfile) == NULL)	return 0;
		flag = tDCLine->ReadLine(Line1);
		if (flag != 1)
		{
			delete tDCLine;
			continue;
		}
		flag = tDCLine->ReadLine(Line2);
		if (flag != 1)
		{
			delete tDCLine;
			continue;
		}
		flag = tDCLine->ReadLine(Line3);
		if (flag != 1)
		{
			delete tDCLine;
			continue;
		}
		flag = tDCLine->ReadLine(Line4);
		if (flag != 1)
		{
			delete tDCLine;
			continue;
		}
		flag = tDCLine->ReadLine(Line5);
		if (flag != 1)
		{
			delete tDCLine;
			continue;
		}
		if (flag == 1)
		{
			if (tDCLine->m_iIname <= 0 || tDCLine->m_iIname > m_iNBB)
				flag = 0;
			if (tDCLine->m_iJname <= 0 || tDCLine->m_iJname > m_iNBB)
				flag = 0;
			if (flag != 1)
			{
				delete tDCLine;
				continue;
			}
		}
		flag = cpGetEquipInfo()->AddNewEquip(tDCLine);
		if (flag < 0)
		{//返回添加成功后的序号值//负数表示添加失败
			delete tDCLine;
			continue;
		}
		strcpy(tDCLine->BusName1, PSASPBus[tDCLine->m_iIname - 1]->Name);
		strcpy(tDCLine->BusName2, PSASPBus[tDCLine->m_iJname - 1]->Name);

		PSASPDCLine[PSASPDCLineTotal] = tDCLine;
		PSASPDCLineTotal++;
	}

	fclose(fpPFfile);
	return 1;
}

int PSASP_NETWORKINFO::ReadL5File(char*file)
{
	int i, flag, genno;
	genno = m_iNGG;
	if (genno <= 0) return 1;

	if (_access(file, 0) == -1)
	{
		return 0;
	}

	FILE*	fpPFfile;
	char Line[_MaxLineLen];
	fpPFfile = fopen(file, "rb");
	if (fpPFfile == NULL)
		return 0;

	PSASPGENERATOR *tGen;

	for (i = 0; i < genno; i++)
	{
		if (fgets(Line, _MaxLineLen, fpPFfile) == NULL)	return 0;

		tGen = new PSASPGENERATOR;
		flag = tGen->ReadLine(Line);
		if (flag == 1)
		{
			if (tGen->m_iBusNo <= 0 || tGen->m_iBusNo > m_iNBB)
				flag = 0;
		}
		if (flag != 1)
		{
			delete tGen;
			continue;
		}
		tGen->m_fGenP = tGen->m_Pg * m_SB;
		tGen->m_fGenQ = tGen->m_Qg * m_SB;
		tGen->m_fGenQmax = tGen->m_Qmax * m_SB;
		tGen->m_fGenQmin = tGen->m_Qmin * m_SB;
		tGen->m_fGenPmax = tGen->m_Pmax * m_SB;
		tGen->m_fGenPmin = tGen->m_Pmin * m_SB;
		flag = cpGetEquipInfo()->AddNewEquip(tGen);
		if (flag < 0)
		{//返回添加成功后的序号值//负数表示添加失败
			delete tGen;
			continue;
		}
		strcpy(tGen->BusName, PSASPBus[tGen->m_iBusNo - 1]->Name);

		PSASPGen[PSASPGenTotal] = tGen;
		PSASPGenTotal++;
	}

	fclose(fpPFfile);
	return 1;
}

int PSASP_NETWORKINFO::ReadL6File(char*file)
{
	int i, flag, loadno;
	loadno = m_iNLoad;
	if (loadno <= 0) return 1;

	if (_access(file, 0) == -1)
	{
		return 0;
	}

	FILE*	fpPFfile;
	char Line[_MaxLineLen];
	fpPFfile = fopen(file, "rb");
	if (fpPFfile == NULL)
		return 0;

	PSASPLOAD *tLoad;

	for (i = 0; i < loadno; i++)
	{
		if (fgets(Line, _MaxLineLen, fpPFfile) == NULL)	return 0;

		tLoad = new PSASPLOAD;
		flag = tLoad->ReadLine(Line);
		if (flag == 1)
		{
			if (tLoad->m_iBusNo <= 0 || tLoad->m_iBusNo > m_iNBB)
				flag = 0;
		}
		if (flag != 1)
		{
			delete tLoad;
			continue;
		}
		tLoad->m_fLoadP = tLoad->m_Pl*m_SB;
		tLoad->m_fLoadQ = tLoad->m_Ql*m_SB;
		tLoad->m_Qmax *= m_SB;
		tLoad->m_Qmin *= m_SB;
		tLoad->m_Pmax *= m_SB;
		tLoad->m_Pmin *= m_SB;
		flag = cpGetEquipInfo()->AddNewEquip(tLoad);
		if (flag < 0)
		{//返回添加成功后的序号值//负数表示添加失败
			delete tLoad;
			continue;
		}
		strcpy(tLoad->BusName, PSASPBus[tLoad->m_iBusNo - 1]->Name);

		PSASPLoad[PSASPLoadTotal] = tLoad;
		PSASPLoadTotal++;
	}

	fclose(fpPFfile);
	return 1;
}

int PSASP_NETWORKINFO::ReadL7File(char*file)
{
	int i, flag, areano;
	areano = m_iNArea;
	AREABASE *tArea;
	int tZoneNo, tMaxZoneNo = 0, tMinZoneNo = 1;
	if (areano <= 0)
	{
		for (i = 0; i < PSASPBusTotal; i++)
		{
			tZoneNo = PSASPBus[i]->m_iAreaNo;
			if (tZoneNo >= _MaxAreaZoneNo)
			{
				sprintf(ErrorMessage[0], "母线区域号(%d)太大，必须有对应的L7文件", tZoneNo);
				PSASPBus[i]->PrintInfo(ErrorMessage[1]);
				cpGetErrorInfo()->PrintError(2);
				return 0;
			}
			if (tMaxZoneNo < tZoneNo)
				tMaxZoneNo = tZoneNo;
			if (tMinZoneNo > tZoneNo)
				tMinZoneNo = tZoneNo;
		}
		tArea = new AREABASE;
		sprintf(tArea->AreaName, "WholeSystem");
		tArea->ZoneTotal = tMaxZoneNo - tMinZoneNo + 1;
		int j;
		for (i = 0, j = tMinZoneNo; i<_MaxAreaZoneNo && j <= tMaxZoneNo; i++, j++)
		{
			sprintf(tArea->ZoneName[i], "%4d", j);
		}
		return 1;
	}

	if (_access(file, 0) == -1)
	{//如果没有L7文件
		return 0;
	}

	FILE*	fpPFfile;
	char Line[_MaxLineLen];
	fpPFfile = fopen(file, "rb");
	if (fpPFfile == NULL)
		return 0;

	for (i = 0; i < areano; i++)
	{
		if (fgets(Line, _MaxLineLen, fpPFfile) == NULL)	return 0;

		PSASPArea[PSASPAreaTotal] = new PSASPAREA;
		flag = sscanf(Line, "%*[^']'%[^']", PSASPArea[PSASPAreaTotal]->AreaName);
		if (flag != 1)
		{
			delete PSASPArea[PSASPAreaTotal];
			PSASPArea[PSASPAreaTotal] = NULL;
			continue;
		}
		ReplaceName(PSASPArea[PSASPAreaTotal]->AreaName, _MaxNameLen);
		tArea = new AREABASE;
		strcpy(tArea->AreaName, PSASPArea[PSASPAreaTotal]->AreaName);
		tArea->ZoneTotal = 1;
		strcpy(tArea->ZoneName[0], PSASPArea[PSASPAreaTotal]->AreaName);
		flag = cpGetAreaInfo()->AddNewArea(tArea);
		if (flag < 0)
		{//返回添加成功后的序号值//负数表示添加失败
			delete tArea;
			continue;
		}
		PSASPAreaTotal++;
	}

	if (PSASPAreaTotal != m_iNArea)
		return 0;

	for (i = 0; i < PSASPBusTotal; i++)
	{
		tZoneNo = PSASPBus[i]->m_iAreaNo;
		if (tZoneNo > m_iNArea)
		{
			sprintf(ErrorMessage[0], "母线区域号(%d)在L7文件中没有定义", tZoneNo);
			PSASPBus[i]->PrintInfo(ErrorMessage[1]);
			cpGetErrorInfo()->PrintError(2);
			return 0;
		}
		if (tZoneNo <= 0)
		{
			sprintf(PSASPBus[i]->Zone, "NULLZONE");
			tMinZoneNo = tZoneNo;
			continue;
		}
		strcpy(PSASPBus[i]->Zone, PSASPArea[tZoneNo - 1]->AreaName);
	}
	if (tMinZoneNo <= 0)
	{
		tArea = new AREABASE;
		sprintf(tArea->AreaName, "NULLZONE");
		tArea->ZoneTotal = 1;
		sprintf(tArea->ZoneName[0], "NULLZONE");
		flag = cpGetAreaInfo()->AddNewArea(tArea);
		if (flag < 0)
		{//返回添加成功后的序号值//负数表示添加失败
			delete tArea;
			return 0;
		}
	}
	fclose(fpPFfile);
	return 1;
}

void PSASP_NETWORKINFO::OutputPSASPPFOFile()
{
	char BusFileLP1[_MAX_PATH];
	char ACLineFileLP2[_MAX_PATH];
	char TranFileLP3[_MAX_PATH];
	char DCLineFileLP4[_MAX_PATH];
	char GenFileLP5[_MAX_PATH];
	char LoadFileLP6[_MAX_PATH];
	strcpy(BusFileLP1, m_DataDir);      strcat(BusFileLP1, ".LP1");
	strcpy(ACLineFileLP2, m_DataDir);   strcat(ACLineFileLP2, ".LP2");
	strcpy(TranFileLP3, m_DataDir);     strcat(TranFileLP3, ".LP3");
	strcpy(DCLineFileLP4, m_DataDir);   strcat(DCLineFileLP4, ".LP4");
	strcpy(GenFileLP5, m_DataDir);      strcat(GenFileLP5, ".LP5");
	strcpy(LoadFileLP6, m_DataDir);     strcat(LoadFileLP6, ".LP6");

	int i;
	FILE*	fpPFOfile;
	fpPFOfile = fopen(BusFileLP1, "w+");
	if (fpPFOfile == NULL)
		return;
	for(i = 0; i < PSASPBusTotal; i++)
	{
		PSASPBus[i]->OutputPFOFile(fpPFOfile);
	}
	fclose(fpPFOfile);

	fpPFOfile = fopen(ACLineFileLP2, "w+");
	if (fpPFOfile == NULL)
		return;
	for (i = 0; i < PSASPACLineTotal; i++)
	{
		PSASPACLine[i]->OutputPFOFile(fpPFOfile, 0);
	}
	fclose(fpPFOfile);

	fpPFOfile = fopen(TranFileLP3, "w+");
	if (fpPFOfile == NULL)
		return;
	for (i = 0; i < PSASPTransTotal; i++)
	{
		PSASPTrans[i]->OutputPFOFile(fpPFOfile, 0);
	}
	fclose(fpPFOfile);

	fpPFOfile = fopen(DCLineFileLP4, "w+");
	if (fpPFOfile == NULL)
		return;
	for (i = 0; i < PSASPDCLineTotal; i++)
	{
		PSASPDCLine[i]->OutputPFOFile(fpPFOfile, 0);
	}
	fclose(fpPFOfile);

	fpPFOfile = fopen(GenFileLP5, "w+");
	if (fpPFOfile == NULL)
		return;
	for (i = 0; i < PSASPGenTotal; i++)
	{
		PSASPGen[i]->OutputPFOFile(fpPFOfile, 0);
	}
	fclose(fpPFOfile);

	fpPFOfile = fopen(LoadFileLP6, "w+");
	if (fpPFOfile == NULL)
		return;
	for (i = 0; i < PSASPLoadTotal; i++)
	{
		PSASPLoad[i]->OutputPFOFile(fpPFOfile, 0);
	}
	fclose(fpPFOfile);
}

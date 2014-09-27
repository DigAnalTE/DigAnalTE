
#include "EquipmentBase.h"
#include "NetWorkInfo.h"
#include "../CommonFunction/ErrorInfo.h"

void ONEEQUIPMENTBASE::JacElement(NETWORKINFO*pNet)
{
	JacType = 1;
	PGNET = 0; QGNET = 0; PGMAX = 0; PGMIN = 0; QGMAX = 0; QGMIN = 0;
	PPNET = 0; QPNET = 0; PINET = 0; QINET = 0; PZNET = 0; QZNET = 0;
	PInst = 0; QInst = 0;
	subJacElement(pNet);
	pNet->NodeTypeModify(BusNo, JacType);
	pNet->NodeGenModify(BusNo, PGNET, QGNET, PGMAX, PGMIN, QGMAX, QGMIN);
	pNet->NodeLoadModify(BusNo, PPNET, QPNET, PINET, QINET, PZNET, QZNET);
	pNet->NodeInsertPower(BusNo, PInst, QInst);
}

void ONEEQUIPMENTBASE::subJacElement(NETWORKINFO*pNet)
{
}
void ONEEQUIPMENTBASE::PrintInfo(char*Line)
{
	sprintf(Line, "%s: %s\n", Name, BusName);
}

void ONEEQUIPMENTBASE::VarientLink(NETWORK_BASE*pNet)
{
	int K1;
	K1 = pNet->BusSearch(BusName);
	if (K1 < 0)
	{//设备中的母线不存在，是因为该设备停运
		iSetState(0);//停运
		BusNo = -1;
		pBus = NULL;
		return;
	}
	BusNo = K1;
	pBus = pNet->cpGetBus(K1);
}

void TWOEQUIPMENTBASE::JacElement(NETWORKINFO*pNet)
{
	JacType1 = 1;
	PGNET1 = 0; QGNET1 = 0; PGMAX1 = 0; PGMIN1 = 0; QGMAX1 = 0; QGMIN1 = 0;
	PPNET1 = 0; QPNET1 = 0; PINET1 = 0; QINET1 = 0; PZNET1 = 0; QZNET1 = 0;
	PInst1 = 0; QInst1 = 0;
	JacType2 = 1;
	PGNET2 = 0; QGNET2 = 0; PGMAX2 = 0; PGMIN2 = 0; QGMAX2 = 0; QGMIN2 = 0;
	PPNET2 = 0; QPNET2 = 0; PINET2 = 0; QINET2 = 0; PZNET2 = 0; QZNET2 = 0;
	PInst2 = 0; QInst2 = 0;
	subJacElement(pNet);
	pNet->NodeTypeModify(BusNo1, JacType1);
	pNet->NodeGenModify(BusNo1, PGNET1, QGNET1, PGMAX1, PGMIN1, QGMAX1, QGMIN1);
	pNet->NodeLoadModify(BusNo1, PPNET1, QPNET1, PINET1, QINET1, PZNET1, QZNET1);
	pNet->NodeInsertPower(BusNo1, PInst1, QInst1);
	pNet->NodeTypeModify(BusNo2, JacType2);
	pNet->NodeGenModify(BusNo2, PGNET2, QGNET2, PGMAX2, PGMIN2, QGMAX2, QGMIN2);
	pNet->NodeLoadModify(BusNo2, PPNET2, QPNET2, PINET2, QINET2, PZNET2, QZNET2);
	pNet->NodeInsertPower(BusNo2, PInst2, QInst2);
}

void TWOEQUIPMENTBASE::subJacElement(NETWORKINFO*pNet)
{
}

void TWOEQUIPMENTBASE::VarientLink(NETWORK_BASE*pNet)
{
	int K1, K2;
	K1 = pNet->BusSearch(BusName1);
	K2 = pNet->BusSearch(BusName2);
	if (K1 < 0)
	{
		sprintf(ErrorMessage[0], "数据中的节点 '%s' 不存在：", BusName1);
		PrintInfo(ErrorMessage[1]);
		cpGetErrorInfo()->PrintError(2);
		return;
	}
	if (K2 < 0)
	{
		sprintf(ErrorMessage[0], "数据中的节点 '%s' 不存在：", BusName2);
		PrintInfo(ErrorMessage[1]);
		cpGetErrorInfo()->PrintError(2);
		return;
	}
	BusNo1 = K1;
	BusNo2 = K2;
	pBus1 = pNet->cpGetBus(K1);
	pBus2 = pNet->cpGetBus(K2);
}

void TWOEQUIPMENTBASE::PrintInfo(char*Line)
{
	sprintf(Line, "%s: %s-%s\n", Name, BusName1, BusName2);
}

void MLTEQUIPMENTBASE::VarientLink(NETWORK_BASE*pNet)
{
	int j, K1;
	for (j = 0; j < PortNumber; j++)
	{
		K1 = pNet->BusSearch(BusName[j]);
		if (K1 < 0)
		{
			sprintf(ErrorMessage[0], "设备中的节点 '%s' 不存在", BusName[j]);
			cpGetErrorInfo()->PrintError(1);
			return;
		}
		BusNo[j] = K1;
	}
}

void MLTEQUIPMENTBASE::PrintInfo(char*Line)
{
	sprintf(Line, "%s\n", Name);
}



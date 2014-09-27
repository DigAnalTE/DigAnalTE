
#include "NetWorkInfo.h"
#include "../CommonFunction/ErrorInfo.h"
#include "../SpareMatrixLib/ComplexSpareMatrix.h"

NETWORKINFO::NETWORKINFO()
{
	JacType = NULL;
	BusVolt = NULL; BusSita = NULL;
	PPGen = NULL; QPGen = NULL; PGmax = NULL; PGmin = NULL; QGmax = NULL; QGmin = NULL; VCtrl = NULL; Vmax = NULL; Vmin = NULL;
	PPnet = NULL; QPnet = NULL; PInet = NULL; QInet = NULL; PZnet = NULL; QZnet = NULL; PLcut = NULL; QLcut = NULL;
	PInst = NULL; QInst = NULL;
	m_Matrix = new COMPLEXSPAREMATRIX;
	bNetInfoCode = -1;
}

NETWORKINFO::~NETWORKINFO()
{
	FreeSpace();
	delete m_Matrix;
	m_Matrix = NULL;
}

void NETWORKINFO::FreeSpace()
{
	NETWORK_BASE::FreeSpace();
	FreeArray(JacType);
	FreeArray(BusVolt); FreeArray(BusSita);
	FreeArray(PPGen); FreeArray(QPGen);
	FreeArray(PGmax); FreeArray(PGmin); FreeArray(QGmax); FreeArray(QGmin);
	FreeArray(VCtrl); FreeArray(Vmax); FreeArray(Vmin);
	FreeArray(PPnet); FreeArray(QPnet); FreeArray(PInet); FreeArray(QInet); FreeArray(PZnet); FreeArray(QZnet);
	FreeArray(PLcut); FreeArray(QLcut);
	FreeArray(PInst); FreeArray(QInst);
	m_Matrix->FreeSpace();
}

void NETWORKINFO::NetLink()
{//该函数若不调用，可能发生未分配空间的问题
	NETWORK_BASE::NetLink();
	cpGetErrorInfo()->CheckMessageType(7);//检查重复错误
}

int NETWORKINFO::NetAnalysis()
{//该函数中的内容必须依次调用
	if (bIsLink == 0)NetLink();
	NETWORK_BASE::NetAnalysis();
	GenerateNetInfo();
	SumInfo();
	return 1;
}

void NETWORKINFO::BuildMatrix()
{
	int i, j, tLn, OLDJ;
	int BusTotal, BranchLinkNo;
	BRANCHINFO::BRANCHLINK* tBranchLink = cpGetBranchInfo()->BranchLink;
	BusTotal = iGetBusTotal();
	BranchLinkNo = cpGetBranchInfo()->BranchLinkNo;

	//统计元素个数
	int TotalRow, TotalElement;
	TotalElement = 0;
	TotalRow = BusTotal;
	j = 0;
	while (j < BranchLinkNo && tBranchLink[j].IBusOldNo < 0)j++;
	for (i = 0; i < BusTotal&&j < BranchLinkNo; i++)
	{
		OLDJ = -1;
		TotalElement++;//一个对角元
		while (j < BranchLinkNo && tBranchLink[j].IBusOldNo == i)
		{
			if (tBranchLink[j].JBusOldNo != OLDJ
				&&tBranchLink[j].JBusOldNo != i)//不考虑单端口元件
			{
				TotalElement++;
				OLDJ = tBranchLink[j].JBusOldNo;
			}
			j++;
		}
	}
	TotalElement = TotalElement + BusTotal - i;
	//开空间
	m_Matrix->FreeSpace();
	m_Matrix->MallocSpace(TotalRow, TotalElement);

	//生成相应的矩阵
	j = 0;
	while (j < BranchLinkNo && tBranchLink[j].IBusOldNo < 0)j++;
	for (i = 0, tLn = -1; i < BusTotal&&j < BranchLinkNo; i++)
	{
		m_Matrix->IA[i] = tLn + 1;
		m_Matrix->NA[i] = 0;
		OLDJ = -1;
		while (j < BranchLinkNo && tBranchLink[j].IBusOldNo == i)
		{
			if (tBranchLink[j].JBusOldNo != OLDJ
				&&tBranchLink[j].JBusOldNo != i)//不考虑单端口元件
			{
				tLn++;
				m_Matrix->NA[i]++;
				OLDJ = tBranchLink[j].JBusOldNo;
				m_Matrix->JA[tLn] = OLDJ;
			}
			j++;
		}
		tLn++;
		m_Matrix->NA[i]++;//增加了一个对角元
		m_Matrix->JA[tLn] = i;
		m_Matrix->II[i] = tLn;
	}
	for (; i < BusTotal; i++)
	{
		m_Matrix->IA[i] = tLn + 1;
		m_Matrix->NA[i] = 0;
		tLn++;
		m_Matrix->NA[i]++;//增加了一个对角元
		m_Matrix->JA[tLn] = i;
		m_Matrix->II[i] = tLn;
	}

	m_Matrix->SpareMatrixReorder();
}

void NETWORKINFO::GenerateNetInfo()
{
	if (bNetInfoCode != bIsLink)
	{
		bNetInfoCode = bIsLink;
		FreeArray(JacType);
		FreeArray(BusVolt); FreeArray(BusSita);
		FreeArray(PPGen); FreeArray(QPGen);
		FreeArray(PGmax); FreeArray(PGmin); FreeArray(QGmax); FreeArray(QGmin);
		FreeArray(VCtrl); FreeArray(Vmax); FreeArray(Vmin);
		FreeArray(PInst); FreeArray(QInst);
		FreeArray(PPnet); FreeArray(QPnet); FreeArray(PInet); FreeArray(QInet); FreeArray(PZnet); FreeArray(QZnet);
		FreeArray(PLcut); FreeArray(QLcut);

		int BusTotal = iGetBusTotal();
		MallocNew(JacType, int, BusTotal);
		MallocNew(BusVolt, real, BusTotal);
		MallocNew(BusSita, real, BusTotal);
		MallocNew(PPGen, real, BusTotal);
		MallocNew(QPGen, real, BusTotal);
		MallocNew(PGmax, real, BusTotal);
		MallocNew(PGmin, real, BusTotal);
		MallocNew(QGmax, real, BusTotal);
		MallocNew(QGmin, real, BusTotal);
		MallocNew(VCtrl, real, BusTotal);
		MallocNew(Vmax, real, BusTotal);
		MallocNew(Vmin, real, BusTotal);
		MallocNew(PPnet, real, BusTotal);
		MallocNew(QPnet, real, BusTotal);
		MallocNew(PInet, real, BusTotal);
		MallocNew(QInet, real, BusTotal);
		MallocNew(PZnet, real, BusTotal);
		MallocNew(QZnet, real, BusTotal);
		MallocNew(PLcut, real, BusTotal);
		MallocNew(QLcut, real, BusTotal);
		MallocNew(PInst, real, BusTotal);
		MallocNew(QInst, real, BusTotal);
		BuildMatrix();
	}
	int i, tRowTotal;//,tIIJacPosition,tJJJacPosition,tIJJacPosition,tJIJacPosition;
	m_Matrix->Clear();
	tRowTotal = iGetBusTotal();
	memset(JacType, 0, tRowTotal*sizeof(int));
	for (i = 0; i < tRowTotal; i++)
	{
		JacType[i] = 1;
	}
	memset(BusVolt, 0, tRowTotal*sizeof(real));
	memset(BusSita, 0, tRowTotal*sizeof(real));
	memset(PPGen, 0, tRowTotal*sizeof(real));
	memset(QPGen, 0, tRowTotal*sizeof(real));
	memset(PGmax, 0, tRowTotal*sizeof(real));
	memset(PGmin, 0, tRowTotal*sizeof(real));
	memset(QGmax, 0, tRowTotal*sizeof(real));
	memset(QGmin, 0, tRowTotal*sizeof(real));
	memset(VCtrl, 0, tRowTotal*sizeof(real));
	memset(Vmax, 0, tRowTotal*sizeof(real));
	memset(Vmin, 0, tRowTotal*sizeof(real));
	memset(PPnet, 0, tRowTotal*sizeof(real));
	memset(QPnet, 0, tRowTotal*sizeof(real));
	memset(PInet, 0, tRowTotal*sizeof(real));
	memset(QInet, 0, tRowTotal*sizeof(real));
	memset(PZnet, 0, tRowTotal*sizeof(real));
	memset(QZnet, 0, tRowTotal*sizeof(real));
	memset(PLcut, 0, tRowTotal*sizeof(real));
	memset(QLcut, 0, tRowTotal*sizeof(real));
	memset(PInst, 0, tRowTotal*sizeof(real));
	memset(QInst, 0, tRowTotal*sizeof(real));
	//关于jacobi矩阵的元素计算
	for (i = 0; i < iGetBusTotal(); i++)
	{
		BusVolt[i] = cpGetBus(i)->m_fBusV;
		BusSita[i] = 0.0174532925f*cpGetBus(i)->m_fBusSita;
		if (cpGetBus(i)->Removed == 1)
		{
			NodeTypeModify(cpGetBus(i)->BusNo, 3);
			continue;
		}
		cpGetBus(i)->JacElement(this);
	}
	for (i = 0; i < iGetBranchTotal(); i++)
	{
		if (cpGetBranch(i)->iGetState() != 1)continue;
		cpGetBranch(i)->JacElement(this);
	}
	for (i = 0; i < iGetEquipTotal(); i++)
	{
		if (cpGetEquip(i)->iGetState() != 1)continue;
		cpGetEquip(i)->JacElement(this);
	}
	for (i = 0; i < iGetBusTotal(); i++)
	{
		if (JacType[i]>3 || JacType[i] < 1)
		{
			JacType[i] = 3;//可能是移除的母线
		}
	}
}

void NETWORKINFO::SumInfo(int isProcess/* =0 */)
{
	int i, BusTotal;
	BusTotal = iGetBusTotal();
	float tVoltage;
	PMaxTotal = 0;
	PGenTotal = 0;
	PLoadTotal = 0;
	PLossTotal = 0;
	for (i = 0; i < BusTotal; i++)
	{
		if (cpGetBus(i)->Removed != 0)continue;
		tVoltage = BusVolt[i];
		PMaxTotal += PGmax[i];
		PGenTotal += PPGen[i] + PInst[i];
		PLoadTotal -= PPnet[i] + PInet[i] * tVoltage + PZnet[i] * tVoltage*tVoltage + PLcut[i];
	}
	for (i = 0; i < iGetBranchTotal(); i++)
	{
		if (cpGetBranch(i)->iGetState() != 1)continue;
		PLossTotal += cpGetBranch(i)->fGetLineLoss();
	}
	PMaxTotal *= GetBMVA();
	PGenTotal *= GetBMVA();
	PLoadTotal *= GetBMVA();
	PLossTotal *= GetBMVA();
}

void NETWORKINFO::NodeTypeModify(int BusNo, int tJacType)
{
	if (tJacType < 1 || tJacType>3 || JacType[BusNo] >= tJacType)
	{
		return;
	}
	JacType[BusNo] = tJacType;
}
void NETWORKINFO::NodeGenModify(int BusNo, real Pgen, real Qgen, real Pmax, real Pmin, real Qmax, real Qmin)
{
	PPGen[BusNo] += Pgen / BMVA;
	QPGen[BusNo] += Qgen / BMVA;
	PGmax[BusNo] += Pmax / BMVA;
	PGmin[BusNo] += Pmin / BMVA;
	QGmax[BusNo] += Qmax / BMVA;
	QGmin[BusNo] += Qmin / BMVA;
}
void NETWORKINFO::NodeVmaxModify(int BusNo, real tVmax, real tVmin)
{
	Vmax[BusNo] = minv(Vmax[BusNo], tVmax);
	Vmin[BusNo] = maxv(Vmin[BusNo], tVmin);
}
void NETWORKINFO::NodeVCtrlModify(int busNo, real tVctrl)
{
	VCtrl[busNo] = tVctrl;
}
void NETWORKINFO::NodeLoadModify(int BusNo, real tPPNET, real tQPNET, real tPINET, real tQINET, real tPZNET, real tQZNET)
{
	PPnet[BusNo] += tPPNET / BMVA;
	QPnet[BusNo] += tQPNET / BMVA;
	PInet[BusNo] += tPINET / BMVA;
	QInet[BusNo] += tQINET / BMVA;
	PZnet[BusNo] += tPZNET / BMVA;
	QZnet[BusNo] += tQZNET / BMVA;
}
void NETWORKINFO::NodeInsertPower(int busNo, real pinsert, real qinsert)
{
	PInst[busNo] += pinsert / BMVA;
	QInst[busNo] += qinsert / BMVA;
}
void NETWORKINFO::NodeLoadCut(int busNo, real plcut, real qlcut)
{
	PLcut[busNo] += plcut / BMVA;
	QLcut[busNo] += qlcut / BMVA;
}
void NETWORKINFO::BranchGBModify(int IBusNo, int JBusNo, real GII, real BII, real GJJ, real BJJ, real GIJ, real BIJ, real GJI, real BJI)
{
	m_Matrix->GBElementModify(IBusNo, IBusNo, GII, BII);
	m_Matrix->GBElementModify(JBusNo, JBusNo, GJJ, BJJ);
	m_Matrix->GBElementModify(IBusNo, JBusNo, GIJ, BIJ);
	m_Matrix->GBElementModify(JBusNo, IBusNo, GJI, BJI);
}

void NETWORKINFO::DampPQTable(FILE*fp)
{
	int i;
	for (i = 0; i < iGetBusTotal(); i++)
	{
		fprintf(fp, " %7d %-s%7.1f", i, cpGetBus(i)->Name, cpGetBus(i)->BaseKv);
		fprintf(fp, "%12.5lf%12.5lf%12.5lf%12.5lf%12.5lf%12.5lf%12.5lf%12.5lf%12.5lf%12.5lf%12.5lf%12.5lf%12.5lf%12.5lf%12.5lf%12.5lf%12.5lf%12.5lf\n",
			PPGen[i], QPGen[i],
			PGmax[i], PGmin[i],
			QGmax[i], QGmin[i],
			Vmax[i], Vmin[i],
			PPnet[i], QPnet[i],
			PInet[i], QInet[i],
			PZnet[i], QZnet[i],
			PLcut[i], QLcut[i],
			PInst[i], QInst[i]);
	}
}

int NETWORKINFO::BeforeIterCal(int iter)
{
	int i, flag;
	for (i = 0; i < iGetBusTotal(); i++)
	{
		if (cpGetBus(i)->Removed == 1)continue;
		flag = cpGetBus(i)->BeforeIterCal(iter);
		if (flag != 1)return -1;
	}
	for (i = 0; i < iGetBranchTotal(); i++)
	{
		if (cpGetBranch(i)->iGetState() != 1)continue;
		flag = cpGetBranch(i)->BeforeIterCal(iter);
		if (flag != 1)return -1;
	}
	for (i = 0; i < iGetEquipTotal(); i++)
	{
		if (cpGetEquip(i)->iGetState() != 1)continue;
		flag = cpGetEquip(i)->BeforeIterCal(iter);
		if (flag != 1)return -1;
	}
	return 1;
}

void NETWORKINFO::Update()
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
		else if (PPGen[i]<PGmin[i])
		{
			cpGetBus(i)->m_fPerr = (PPGen[i] - PGmin[i])*GetBMVA();
			PPGen[i] = PGmin[i];
		}
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
	for (i = 0; i < iGetEquipTotal(); i++)
	{
		if (cpGetEquip(i)->iGetState() != 1)
			continue;
		cpGetEquip(i)->UpdateValue(this);
	}
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
}

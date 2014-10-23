
#include "GenEq1Constant.h"
#include "DynamicModelInfo.h"

GenEq1Constant::GenEq1Constant()
{
	strcpy(ModelName, "GC");
	strcpy(mVarient[0].IndexName, "W");
	strcpy(mVarient[0].OutName, "���ٶ�");
	mVarient[0].pValu = &W;
	mVarient[0].outflag = 0;
	strcpy(mVarient[1].IndexName, "Sita");
	strcpy(mVarient[1].OutName, "�Ƕ�");
	mVarient[1].pValu = &Sita;
	mVarient[1].outflag = 0;
	strcpy(mVarient[2].IndexName, "PE");
	strcpy(mVarient[2].OutName, "��Ź���");
	mVarient[2].pValu = &PE;
	mVarient[2].outflag = 0;
	strcpy(mVarient[3].IndexName, "PM");
	strcpy(mVarient[3].OutName, "��е����");
	mVarient[3].pValu = &PM;
	mVarient[3].outflag = 0;
	Varient = mVarient;
}

int GenEq1Constant::ReadLine(char*line)
{
	int flag;
	flag = sscanf(line, "%*[^,],%[^,],%f,%f,%f,%f",
		EquipmentName, &EMWS, &MVABase, &R, &Xd1);
	ReplaceName(EquipmentName, _MaxNameLen);
	if (flag != 5)return 0;
	return 1;
}

void GenEq1Constant::PrintInfo(char*line)
{
	sprintf(line, "Eq1�㶨ģ��'%s'", EquipmentName);
}

int GenEq1Constant::CheckInputData()
{
	if (pEquip == NULL)
		return 0;
	m_BusNo = pEquip->GetBusNo(0);
	if (MVABase < 0.01)
		MVABase = BMVA;
	R = R*BMVA / MVABase;
	Xd1 = Xd1*BMVA / MVABase;
	Tj = EMWS*2.0 / BMVA;
	return 1;
}
void GenEq1Constant::ModifyMatrix()
{
	real gii, bii;
	gii = R / (R*R + Xd1*Xd1); bii = -Xd1 / (R*R + Xd1*Xd1);
	pSolInfo->ModifyNetMatrix(m_BusNo, m_BusNo, gii, bii);
	//real V2;
	//pSolInfo->GetVxVy(m_BusNo, Vx, Vy);
	//V2 = Vx*Vx + Vy*Vy;
	//pSolInfo->ModifyNetMatrix(m_BusNo, m_BusNo, -GenP / V2, GenQ / V2);
}

int GenEq1Constant::DynInitial()
{
	real V2;
	pSolInfo->GetVxVy(m_BusNo, Vx, Vy);
	V2 = Vx*Vx + Vy*Vy;
	pEquip->GetInsertPQ(GenP, GenQ, 0);
	GenP /= BMVA;
	GenQ /= BMVA;

	Ix = (GenP*Vx + GenQ*Vy) / V2;
	Iy = (GenP*Vy - GenQ*Vx) / V2;
	OldIx = Ix;
	OldIy = Iy;

	real EQR, EQI;
	EQR = Vx + R*Ix - Xd1*Iy;
	EQI = Vy + R*Iy + Xd1*Ix;

	Sita = (real)atan2(EQI, EQR);

	real Id, Iq, Ud, Uq;
	Id = Ix*sin(Sita) - Iy*cos(Sita);
	Iq = Ix*cos(Sita) + Iy*sin(Sita);
		Ud = Vx*sin(Sita) - Vy*cos(Sita);
	Uq = Vx*cos(Sita) + Vy*sin(Sita);

	Eq1 = Uq + R*Iq + Xd1*Id;

	W = 1.0; OldW = 1;

	PE = GenP + R*(Id*Id + Iq*Iq);
	PM = PE;
	Pdelta = 0;

	DynAfterStep();
	return 1;
}

void GenEq1Constant::DynBeforeStep()
{
	real step;
	step = pSolInfo->Step;
	A_W = step / Tj;
	A_Sita = step * 2 * 50 * 3.1415;
	W = W0 + A_W*Pdelta0;
	Sita = Sita0 + A_Sita*(W - 1);
}

real GenEq1Constant::DynProcessStep()
{
	real err;
	pSolInfo->GetVxVy(m_BusNo, Vx, Vy);
	PE = Vx*Ix + Vy*Iy;
	Pdelta = PM - PE;
	W = W0 + A_W / 2 * (Pdelta + Pdelta0);
	Sita = Sita0 + A_Sita / 2 * (W + W0 - 2);
	err = OldW - W;
	OldW = W;
	return err;
}

real GenEq1Constant::DynCalculateCurrent()
{
	real err;
	Ix = Eq1 / Xd1*sin(Sita) - Vy / Xd1;
	Iy = -Eq1 / Xd1*cos(Sita) + Vx / Xd1;
	err = fabs(Ix - OldIx) + fabs(Iy - OldIy);
	OldIx = Ix;
	OldIy = Iy;
	pIx[m_BusNo] -= Eq1 / Xd1*sin(Sita);
	pIy[m_BusNo] += Eq1 / Xd1*cos(Sita);
	return err;
}

void GenEq1Constant::DynAfterStep()
{
	W0 = W;
	Sita0 = Sita;
	Pdelta0 = Pdelta;
}

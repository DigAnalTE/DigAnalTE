
#include "BPAMCModel.h"
#include "BPAReadWrite.h"
#include "BPADynamicModelInfo.h"

int BPAMCMODEL::ReadLine(char*line)
{
	GetItemFromLine(line, &EMWS, SWI_MC_Para[3], SWI_MC_Loca[3]);
	GetItemFromLine(line, &MVABase, SWI_MC_Para[6], SWI_MC_Loca[6]);
	GetItemFromLine(line, &R, SWI_MC_Para[7], SWI_MC_Loca[7]);
	GetItemFromLine(line, &Xd1, SWI_MC_Para[8], SWI_MC_Loca[8]);
	return 1;
}

int BPAMCMODEL::ReadOutLine(char*Line)
{
	if (Line[19] >= '1' && Line[19] <= '9')
	{
		mVarient[0].outflag = 1;
	}
	if (Line[22] >= '1' && Line[22] <= '9')
	{
		mVarient[1].outflag = 1;
	}
	if (Line[34] >= '1' && Line[34] <= '9')
	{
		mVarient[2].outflag = 1;
	}
	if (Line[37] >= '1' && Line[37] <= '9')
	{
		mVarient[3].outflag = 1;
	}
	if (Line[49] >= '1' && Line[49] <= '9')
	{
		mVarient[4].outflag = 1;
	}
	return 1;
}

BPAMCMODEL::BPAMCMODEL()
{
	strcpy(ModelName, "MC");
	strcpy(mVarient[0].IndexName, "dSita");
	strcpy(mVarient[0].OutName, "发电机相对角度");
	mVarient[0].pValu = &dSita;
	mVarient[0].outflag = 0;
	strcpy(mVarient[1].IndexName, "dW");
	strcpy(mVarient[1].OutName, "发电机速度偏差");
	mVarient[1].pValu = &dF;
	mVarient[1].outflag = 0;
	strcpy(mVarient[2].IndexName, "PM");
	strcpy(mVarient[2].OutName, "发电机机械功率");
	mVarient[2].pValu = &PM;
	mVarient[2].outflag = 0;
	strcpy(mVarient[3].IndexName, "PE");
	strcpy(mVarient[3].OutName, "发电机电磁功率");
	mVarient[3].pValu = &PE;
	mVarient[3].outflag = 0;
	strcpy(mVarient[4].IndexName, "Q");
	strcpy(mVarient[4].OutName, "发电机无功功率");
	mVarient[4].pValu = &GenQ;
	mVarient[4].outflag = 0;
	strcpy(mVarient[5].IndexName, "Sita");
	strcpy(mVarient[5].OutName, "发电机角度");
	mVarient[5].pValu = &Sita;
	mVarient[5].outflag = 0;
	Varient = mVarient;
}

void BPAMCMODEL::PrintInfo(char*line)
{
	sprintf(line, "发电机MC模型'%s'", EquipmentName);
}

int BPAMCMODEL::CheckInputData()
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
void BPAMCMODEL::ModifyMatrix()
{
	real gii, bii;
	gii = R / (R*R + Xd1*Xd1); bii = -Xd1 / (R*R + Xd1*Xd1);
	pSolInfo->ModifyNetMatrix(m_BusNo, m_BusNo, gii, bii);
	//real V2;
	//pSolInfo->GetVxVy(m_BusNo, Vx, Vy);
	//V2 = Vx*Vx + Vy*Vy;
	//pSolInfo->ModifyNetMatrix(m_BusNo, m_BusNo, -GenP / V2, GenQ / V2);
}

int BPAMCMODEL::DynInitial()
{
	real V2;
	pSolInfo->GetVxVy(m_BusNo, Vx, Vy);
	V2 = Vx*Vx + Vy*Vy;
	pEquip->GetInsertPQ(GenP, GenQ, 0);
	GenP /= BMVA;
	GenQ /= BMVA;

	Ix = (GenP*Vx + GenQ*Vy) / V2;
	Iy = (GenP*Vy - GenQ*Vx) / V2;

	real EQR, EQI;
	EQR = Vx + R*Ix - Xd1*Iy;
	EQI = Vy + R*Iy + Xd1*Ix;

	Sita = (real)atan2(EQI, EQR);
	dSita = Sita*57.29578;

	real Id, Iq, Ud, Uq;
	Id = Ix*sin(Sita) - Iy*cos(Sita);
	Iq = Ix*cos(Sita) + Iy*sin(Sita);
	Ud = Vx*sin(Sita) - Vy*cos(Sita);
	Uq = Vx*cos(Sita) + Vy*sin(Sita);

	Eq1 = Uq + R*Iq + Xd1*Id;

	W = 1.0; OldW = 0.;
	dF = (W - 1) * 50;

	PE = GenP + R*(Id*Id + Iq*Iq);
	PM = PE;
	Pdelta = 0;

	DynAfterStep();
	return 1;
}

void BPAMCMODEL::DynBeforeStep()
{
	real step;
	step = pSolInfo->Step;
	A_W = step / Tj;
	A_Sita = step * 2 * 50 * 3.1415;
	W = W0 + A_W*Pdelta0;
	Sita = Sita0 + A_Sita*(W - 1);
}

real BPAMCMODEL::DynProcessStep()
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

real BPAMCMODEL::DynCalculateCurrent()
{
	Ix = Eq1 / Xd1*sin(Sita);
	Iy = -Eq1 / Xd1*cos(Sita);
	pIx[m_BusNo] -= Ix;
	pIy[m_BusNo] -= Iy;
	return 0;
}

void BPAMCMODEL::DynAfterStep()
{
	W0 = W;
	Sita0 = Sita;
	Pdelta0 = Pdelta;
	GenQ = Vy*Ix - Vx*Iy;
	dSita = Sita*57.29578;
	while (dSita > 360)dSita -= 360;
	while (dSita < -360)dSita += 360;
	dF = (W - 1) * 50;
}


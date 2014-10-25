
#include "BusDynModel.h"
#include "DynamicModelInfo.h"

BUSDYNMODEL::BUSDYNMODEL()
{
	strcpy(ModelName, "BUS");
	strcpy(mVarient[0].IndexName, "Vx");
	strcpy(mVarient[0].OutName, "Vx");
	mVarient[0].pValu = &Vx;
	mVarient[0].outflag = 0;
	strcpy(mVarient[1].IndexName, "Vy");
	strcpy(mVarient[1].OutName, "Vy");
	mVarient[1].pValu = &Vy;
	mVarient[1].outflag = 0;
	strcpy(mVarient[2].IndexName, "V");
	strcpy(mVarient[2].OutName, "电压幅值");
	mVarient[2].pValu = &V;
	mVarient[2].outflag = 0;
	strcpy(mVarient[3].IndexName, "Sita");
	strcpy(mVarient[3].OutName, "电压相角");
	mVarient[3].pValu = &Sita;
	mVarient[3].outflag = 0;
	strcpy(mVarient[4].IndexName, "Frequency");
	strcpy(mVarient[4].OutName, "频率");
	mVarient[4].pValu = &Frequency;
	mVarient[4].outflag = 0;
	Varient = mVarient;
}

int BUSDYNMODEL::CheckInputData()
{
	if (pEquip == NULL)
		return 0;
	strcpy(EquipmentName, pSolInfo->cpGetBus(BusNo)->GetBusName());
	return 1;
}

int BUSDYNMODEL::DynInitial()
{
	pSolInfo->GetVxVy(BusNo, Vx, Vy);
	V = sqrt(Vx*Vx + Vy*Vy);

	Sita = (real)atan2(Vx, Vy);

	Frequency = 50;

	DynAfterStep();
	return 1;
}

void BUSDYNMODEL::DynBeforeStep()
{
}

real BUSDYNMODEL::DynProcessStep()
{
	pSolInfo->GetVxVy(BusNo, Vx, Vy);
	return 0;
}

void BUSDYNMODEL::DynAfterStep()
{
	pSolInfo->GetVxVy(BusNo, Vx, Vy);
	Sita = (real)atan2(Vx, Vy);
}

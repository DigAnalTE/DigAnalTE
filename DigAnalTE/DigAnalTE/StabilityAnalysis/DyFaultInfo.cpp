
#include "DyFaultInfo.h"

DYFAULTINFO::DYFAULTINFO()
{
	iFaultListNo = 0;
	memset(pFaultList, 0, _MaxFaultNum*sizeof(DYFAULTBASE*));
	pDyn = NULL;
	NextTime = 99999.;
	NextFault = -1;
}

void DYFAULTINFO::RemoveAllFault()
{
	int i;
	for (i = 0; i < iFaultListNo; i++)
	{
		FreeArray(pFaultList[i]);
	}
	iFaultListNo = 0;
}

float DYFAULTINFO::GetNextTime()
{
	if (NextFault < -1)
		return 99999.;
	int i;
	NextTime = 99999.;
	NextFault = -2;
	float tTime;
	for (i = 0; i < iFaultListNo; i++)
	{
		if (pFaultList[i]->GetIsProcess())
			continue;
		tTime = pFaultList[i]->GetNextTime();
		if (tTime < NextTime)
		{
			NextFault = i;
			NextTime = tTime;
		}
	}
	return NextTime;
}

int DYFAULTINFO::ProcessFault(float time)
{
	return 1;
}

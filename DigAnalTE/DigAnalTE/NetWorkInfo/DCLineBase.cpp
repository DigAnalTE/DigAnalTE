
#include "DCLineBase.h"
#include "BusInfo.h"
#include "NetWorkInfo.h"

void DCLINEBASE::subJacElement(NETWORKINFO*pNet)
{
	JacType1 = 0;
	JacType2 = 0;
	PPNET1 = -PlanPower;
	PPNET2 = PlanPower;
}

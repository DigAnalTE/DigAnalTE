
#include "DCLineBase.h"
#include "BusInfo.h"
#include "NetWorkInfo.h"

void DCLINEBASE::subJacElement(NETWORKINFO*pNet)
{
	JacType1 = 0;
	JacType2 = 0;
	PInst1 = -Pref;
	QInst1 = -Pref*tan(Alpha);
	PInst2 = Pref;
	QInst1 = -Pref*tan(Beta);
}

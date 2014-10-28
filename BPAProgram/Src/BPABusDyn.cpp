
#include "BPABusDyn.h"

int BPABUSDYNMODEL::ReadOutLine(char*Line)
{
	if (Line[17] >= '1' && Line[17] <= '9')
	{
		mVarient[2].outflag = 1;
	}
	if (Line[18] >= '1' && Line[18] <= '9')
	{
		mVarient[3].outflag = 1;
	}
	if (Line[20] >= '1' && Line[20] <= '9')
	{
		mVarient[4].outflag = 1;
	}
	return 1;
}

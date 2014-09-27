
#include "QikSort.h"

void QikSortInterface::QikSort(int mm, int nn)//,CompFun kompar,SwapFun swap)
{//Ã°ÅÝ·¨
	int i, j;
	for (i = mm; i < nn; i++)
	{
		for (j = i + 1; j <= nn; j++)
		{
			if (kompar(i, j) > 0)
				swap(i, j);
		}
	}
}

int TempFloatSort::kompar(int m, int n)
{
	if (FloatValue[n] > FloatValue[m] - error)
	{
		return -1;
	}
	return 1;
}
void TempFloatSort::swap(int m, int n)
{
	tempf = FloatValue[m];
	FloatValue[m] = FloatValue[n];
	FloatValue[n] = tempf;
	tempn = Index[m];
	Index[m] = Index[n];
	Index[n] = tempn;
}
int TempRealSort::kompar(int m, int n)
{
	if (RealValue[n] > RealValue[m] - error)
	{
		return -1;
	}
	return 1;
}
void TempRealSort::swap(int m, int n)
{
	tempf = RealValue[m];
	RealValue[m] = RealValue[n];
	RealValue[n] = tempf;
	tempn = Index[m];
	Index[m] = Index[n];
	Index[n] = tempn;
}
int TempRealGreatSort::kompar(int m, int n)
{
	if (RealValue[n] < RealValue[m] - error)
	{
		return -1;
	}
	return 1;
}
void TempRealGreatSort::swap(int m, int n)
{
	tempf = RealValue[m];
	RealValue[m] = RealValue[n];
	RealValue[n] = tempf;
	tempn = Index[m];
	Index[m] = Index[n];
	Index[n] = tempn;
}
int TempIntSort::kompar(int m, int n)
{
	return IntValue[m] - IntValue[n];
}
void TempIntSort::swap(int m, int n)
{
	tempf = IntValue[m];
	IntValue[m] = IntValue[n];
	IntValue[n] = tempf;
	tempn = Index[m];
	Index[m] = Index[n];
	Index[n] = tempn;
}


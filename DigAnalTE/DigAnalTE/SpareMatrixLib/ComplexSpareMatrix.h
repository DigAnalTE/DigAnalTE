
#ifndef __COMPLEX_SPARE_MATRIX_H__
#define __COMPLEX_SPARE_MATRIX_H__

#include "SpareMatrix.h"

class COMPLEXSPAREMATRIX : public SPAREMATRIXBASE
{
public:
	COMPLEXSPAREMATRIX()
	{
		VG=NULL;VB=NULL;//VX=NULL;
	}
	~COMPLEXSPAREMATRIX(){}
public://元素
	real *VG,*VB;
public:
	virtual void subMallocSpace(int tRow,int tElement);
	virtual void FreeSpace();
	virtual void Clear();//将所有值清零
public:
	void PrintMatrix(class NETWORKINFO*pNet,FILE*fp);
public://设备进行修改
	void GBElementModify(int IBusNo,int JBusNo,real GII,real BII);
	int GetGBElementValue(int IBusNo,int JBusNo,real& GII,real& BII);
};

#endif

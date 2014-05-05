
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
public://Ԫ��
	real *VG,*VB;
public:
	virtual void subMallocSpace(int tRow,int tElement);
	virtual void FreeSpace();
	virtual void Clear();//������ֵ����
public:
	void PrintMatrix(class NETWORKINFO*pNet,FILE*fp);
public://�豸�����޸�
	void GBElementModify(int IBusNo,int JBusNo,real GII,real BII);
	int GetGBElementValue(int IBusNo,int JBusNo,real& GII,real& BII);
};

#endif

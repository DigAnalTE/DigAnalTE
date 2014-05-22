
#ifndef __COMPLEX_LDU_SOLVER_H__
#define __COMPLEX_LDU_SOLVER_H__

#include "ComplexSpareMatrixSolver.h"

class LDUSOLVER : public COMPLEXSPAREMATRIXSOLVER
{
public:
	LDUSOLVER()
	{
		VA=NULL;
		
		LDUnVASpace=0;
		LDUnVANumb=0;
		LDUIA=NULL;
		L_VA=NULL;
		U_VA=NULL;
		LDUJA=NULL;
		LDULINK=NULL;
	}
private://LDU�ֽ��ľ���
	int LDUnVASpace;//���ռ����Ϣ =0ʱ��ʾδ����LDU�ֽ�
	int LDUnVANumb;//ʹ�õĿռ���
	//ϡ�����洢//�����ǽṹ�ԳƵ�//������ֵ���Գ�
	int *LDUIA;
	Composite *L_VA;
	Composite *U_VA;
	int *LDUJA;
	int *LDULINK;
 	void ReMallocSpace(int tAddSpace);//���ռ䲻����ʱ�����������ռ�
public:
	int LDUFactorization();
	virtual int Calculate();
	virtual void subInitMatrix()
	{
		MallocNew(LDUIA,int,RowTotal);
		ReMallocSpace((int)(ElementTotal*1.2));
	}
	virtual void subFreeSpace()
	{
		FreeArray(VA);
		if (LDUnVASpace!=0)
		{
			LDUnVASpace=0;
			LDUnVANumb=0;
			FreeArray(LDUIA);
			FreeArray(L_VA);
			FreeArray(U_VA);
			FreeArray(LDUJA);
			FreeArray(LDULINK);
		}
	}
};

#endif
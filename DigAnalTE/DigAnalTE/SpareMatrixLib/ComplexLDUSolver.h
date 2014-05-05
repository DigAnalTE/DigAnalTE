
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
private://LDU分解后的矩阵
	int LDUnVASpace;//开空间的信息 =0时表示未进行LDU分解
	int LDUnVANumb;//使用的空间数
	//稀疏矩阵存储//必须是结构对称的//可以数值不对称
	int *LDUIA;
	Composite *L_VA;
	Composite *U_VA;
	int *LDUJA;
	int *LDULINK;
 	void ReMallocSpace(int tAddSpace);//当空间不够的时候重新申请大空间
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
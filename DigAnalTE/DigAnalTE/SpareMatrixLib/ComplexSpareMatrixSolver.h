
#ifndef __COMPLEX_SPARE_MATRIX_SOLVER_H__
#define __COMPLEX_SPARE_MATRIX_SOLVER_H__

#include "ComplexSpareMatrix.h"

typedef double cstype;

class COMPLEXSPAREMATRIXSOLVER
{
public:
	COMPLEXSPAREMATRIXSOLVER()
	{
		VA=NULL;flagChange=1;pMatrix=NULL;
		X=NULL;Y=NULL;
	}
	~COMPLEXSPAREMATRIXSOLVER(){FreeSpace();}
public:
	int flagChange;//是否有元素发生改变，LU算法可以在没有变化的时候不必多次分解
	COMPLEXSPAREMATRIX *pMatrix;
protected:
	int RowTotal;
	int ElementTotal;
	struct Composite
	{
		cstype RowH, RowN;
		cstype RowJ, RowL;
	};
	Composite *VA;
	
	int *NewNo;//新的节点号  NewBusNo=NewNo[OldBusNo]
	int *OldNo;//旧的节点号  OldBusNo=OldNo[NewBusNo]
	int *NIA;
	int *NNA;
	int *NJA;
	int *VALink;//指向原来的VA   新的值[i]=VA[VALink[i]]
public:
	void InitMatrix(COMPLEXSPAREMATRIX *tMatrix);
	void ReSetMatrixElement();//从pMatrix重新读取值到VA中
	virtual void subInitMatrix(){}
protected:
	real *X,*Y;
protected:
	virtual int Calculate()=0;
public:
	int LinearEquationCal(real *tX,real *tY)//计算
	{
		X=tX;Y=tY;
		return Calculate();
	}
	void FreeSpace()
	{
		FreeArray(VA);
		subFreeSpace();
	}
	void subFreeSpace(){}
	void PrintMatrix(NETWORKINFO*pNet,FILE*fp);
public:
	void SetChangeFlag(int flag){flagChange=flag;}
	void SetElement(int L,cstype RH,cstype RJ,cstype RN,cstype RL)
	{
		flagChange=1;
		VA[L].RowH=RH;
		VA[L].RowJ=RJ;
		VA[L].RowN=RN;
		VA[L].RowL=RL;
	}
	void ModifyElement(int busno,cstype RH,cstype RJ,cstype RN,cstype RL)
	{
		flagChange=1;
		int L=pMatrix->II[busno];
		VA[L].RowH+=RH;
		VA[L].RowJ+=RJ;
		VA[L].RowN+=RN;
		VA[L].RowL+=RL;
	}
	void ModifyElement(int busno,int busno2,cstype RH,cstype RJ,cstype RN,cstype RL)
	{
		flagChange=1;
		int i,L=0;
		for (i=0,L=pMatrix->IA[busno];i<pMatrix->NA[busno];i++,L++)
		{
			if (pMatrix->JA[L]==busno2)
			{
				VA[L].RowH+=RH;
				VA[L].RowJ+=RJ;
				VA[L].RowN+=RN;
				VA[L].RowL+=RL;
				return;
			}
		}
	}
};

COMPLEXSPAREMATRIXSOLVER* makeLDUSolver();

#endif

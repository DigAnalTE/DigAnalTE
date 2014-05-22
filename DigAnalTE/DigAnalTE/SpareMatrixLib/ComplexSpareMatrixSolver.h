
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
	int flagChange;//�Ƿ���Ԫ�ط����ı䣬LU�㷨������û�б仯��ʱ�򲻱ض�ηֽ�
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
	
	int *NewNo;//�µĽڵ��  NewBusNo=NewNo[OldBusNo]
	int *OldNo;//�ɵĽڵ��  OldBusNo=OldNo[NewBusNo]
	int *NIA;
	int *NNA;
	int *NJA;
	int *VALink;//ָ��ԭ����VA   �µ�ֵ[i]=VA[VALink[i]]
public:
	void InitMatrix(COMPLEXSPAREMATRIX *tMatrix);
	void ReSetMatrixElement();//��pMatrix���¶�ȡֵ��VA��
	virtual void subInitMatrix(){}
protected:
	real *X,*Y;
protected:
	virtual int Calculate()=0;
public:
	int LinearEquationCal(real *tX,real *tY)//����
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

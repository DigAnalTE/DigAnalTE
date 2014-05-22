
#ifndef __NRSOLUTION_H__
#define __NRSOLUTION_H__

#include "PowerFlowFunction.h"
#include "../SpareMatrixLib/ComplexSpareMatrixSolver.h"

class NRSOLUTION : public PFSOLUTION_BASE
{
public:
	NRSOLUTION(NETWORKINFO* temp);
	virtual ~NRSOLUTION()
	{
		FreeSpace();
	}
private:
	real *RightP,*RightQ;//�Ҷ˺���
	real *Volt,*Sita;
	COMPLEXSPAREMATRIXSOLVER *m_Solver;
	int NRSBusTotal,NRSJacElTotal;
private:
	int MAXITER;//ţ������ѷ��ȱʡ����������
	float MAXPERR,MAXQERR;//�ж������ڵ���������
public:
	virtual int subCalculate(PFCalPara);//�ڶ�����ʼ����
protected:
	int IsInit;//�Ƿ��ʼ����
	void InitPFlow();//��һ����ʼ��//��ʼ������û������subInit,��Ϊ�����ʼ����������Ķ����϶࣬�ڱ�Ҫ��ʱ���ٵ���

	virtual void FreeSpace()//�������ͷſռ�
	{
		pNet=NULL;
		m_Solver->FreeSpace();
		NRSBusTotal=0;NRSJacElTotal=0;
		FreeArray(RightP);FreeArray(RightQ);
	}
private:
	void FormJacobi();//����jacobi����
};

#endif


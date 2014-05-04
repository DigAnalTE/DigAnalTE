
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
	real *RightP,*RightQ;//右端函数
	real *Volt,*Sita;
	COMPLEXSPAREMATRIXSOLVER *m_Solver;
	int NRSBusTotal,NRSJacElTotal;
private:
	int MAXITER;//牛顿拉夫逊法缺省最大迭代次数
	float MAXPERR,MAXQERR;//判断收敛节点的误差限制
public:
	virtual int subCalculate(PFCalPara);//第二步开始计算
protected:
	int IsInit;//是否初始化过
	void InitPFlow();//第一步初始化//初始化函数没有派生subInit,因为这个初始化函数处理的东西较多，在必要的时候再调用

	virtual void FreeSpace()//第三步释放空间
	{
		pNet=NULL;
		m_Solver->FreeSpace();
		NRSBusTotal=0;NRSJacElTotal=0;
		FreeArray(RightP);FreeArray(RightQ);
	}
private:
	void FormJacobi();//生成jacobi矩阵
};

#endif


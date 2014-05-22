
#ifndef __DigAnalTE_PowerFlowFunction_H__
#define __DigAnalTE_PowerFlowFunction_H__

#include "../NetWorkInfo/NetWorkInfo.h"
#include "../CommonFunction/ErrorInfo.h"

struct PFCalPara
{//潮流计算控制参数
	int MaxIter;//牛顿拉夫逊法最大迭代次数
	float BusPMaxErr;//判断收敛节点的误差限制
	float BusQMaxErr;
	//MaxIter=100;BusPMaxErr=0.005f;BusQMaxErr=0.005f;
};

class PFSOLUTION_BASE
{
public:
	PFSOLUTION_BASE(NETWORKINFO* temp)
	{
		mPara.MaxIter=50;
		mPara.BusPMaxErr=0.005f;
		mPara.BusQMaxErr=0.005f;
		pNet=temp;
		autoUpdate=1;
	}
	~PFSOLUTION_BASE()
	{
		FreeSpace();
	}
    virtual void FreeSpace(){}
protected:
	PFCalPara       mPara;
	NETWORKINFO*    pNet;
public:
	int autoUpdate;//是否在计算完潮流后在这个函数中进行Update
public:
	int Calculate(PFCalPara tPara)
	{
		mPara=tPara;
        return Calculate();
	}
	int Calculate()
	{
		if (pNet->NetError!=1)
		{
			sprintf(ErrorMessage[0],"数据存在错误，不能进行潮流计算");
			cpGetErrorInfo()->PrintError(1);
			return 0;
		}
		if (subCalculate(mPara)==1)
		{
			if(autoUpdate)pNet->Update();
			return 1;
		}
		return 0;
	}
	virtual int subCalculate(PFCalPara)=0;//纯虚函数必须派生，纯虚函数的目的是为了检查派生时派生函数名的正确性
};

PFSOLUTION_BASE* makeNRSolver(NETWORKINFO*);

#endif
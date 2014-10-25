
#ifndef __BUS_DYNAMIC_MODEL_H__
#define __BUS_DYNAMIC_MODEL_H__

#include "DynamicModelBase.h"

class BUSDYNMODEL :public EQUIPMENT_DYN_MODEL
{
public:
	BUSDYNMODEL();
public:
	int BusNo;
	real Vx, Vy, V, Sita, Frequency;
public://输出变量
	DynVarient mVarient[5];
	virtual int GetVarientTotal(){ return 5; }
public:
	virtual int ReadLine(char*Line){ return 1; }
	virtual void PrintInfo(char*Line){}
	virtual int CheckInputData();
public:
	//virtual void ModifyMatrix();
	virtual int DynInitial();//微分方程初值计算
	virtual void DynBeforeStep();//积分前处理//可以使用显式算法进行预测
	virtual real DynProcessStep();//积分//隐式梯形积分法
	//virtual real DynCalculateCurrent();
	virtual void DynAfterStep();//积分后处理
};

#endif

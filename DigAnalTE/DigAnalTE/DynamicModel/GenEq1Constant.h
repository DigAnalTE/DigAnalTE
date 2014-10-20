
#ifndef __DigAnalTE_Generator_Eq1Constant_Model_H__
#define __DigAnalTE_Generator_Eq1Constant_Model_H__

#include "DynamicModelBase.h"

class GenEq1Constant : public EQUIPMENT_DYN_MODEL
{
public:
	GenEq1Constant();
public:
	int m_BusNo;
	float EMWS, Tj;
	float MVABase;
	float R;
	float Xd1;
public:
	real Eq1;
	real W, Sita, PE, PM, Pdelta;
	real W0, Sita0, Pdelta0;
	real A_W, A_Sita;
	real GenP, GenQ;
	real Vx, Vy, Ix, Iy;
	real OldW, OldIx, OldIy;//用于判断是否收敛
public://输出变量
	DynVarient mVarient[4];
	virtual int GetVarientTotal(){ return 4; }
public:
	virtual int ReadLine(char*Line);
	virtual void PrintInfo(char*Line);
	virtual int CheckInputData();//初值检查
public:
	virtual void ModifyMatrix();
	virtual int DynInitial();//微分方程初值计算
	virtual void DynBeforeStep();//积分前处理//可以使用显式算法进行预测
	virtual real DynProcessStep();//积分//隐式梯形积分法
	virtual real DynCalculateCurrent();
	virtual void DynAfterStep();//积分后处理
};

#endif

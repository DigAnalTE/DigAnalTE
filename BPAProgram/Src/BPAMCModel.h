
#ifndef __BPA_MC_Model_H__
#define __BPA_MC_Model_H__

#include "../../DigAnalTE/DigAnalTE/DynamicModel/DynamicModelBase.h"

class BPAMCMODEL : public EQUIPMENT_DYN_MODEL
{
public:
	BPAMCMODEL();
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
	real OldW;//用于判断是否收敛
	real dSita, dF, PMout, PEout, Qout;
public://输出变量
	DynVarient mVarient[6];
	virtual int GetVarientTotal(){ return 6; }
public:
	virtual int ReadLine(char*Line);
	virtual int ReadOutLine(char*Line);
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

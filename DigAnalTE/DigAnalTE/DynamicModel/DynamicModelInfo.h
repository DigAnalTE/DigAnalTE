
#ifndef __DigAnalTE_DYNAMIC_INFO_H__
#define __DigAnalTE_DYNAMIC_INFO_H__

#include "../NetWorkInfo/NetWorkInfo.h"
#include "DynamicModelBase.h"
#include "../SpareMatrixLib/ComplexSpareMatrix.h"

class DYNAMICMODELINFO : public NETWORKINFO
{
public:
	DYNAMICMODELINFO();
	virtual ~DYNAMICMODELINFO();
	virtual void FreeSpace();

public://动态模型信息
	int DynamicModelTotal;
	DYNAMIC_MODEL_BASE *DynamicModel[_MaxDynamicModelNo];
	int InsertNewDynamicModel(DYNAMIC_MODEL_BASE*);
	/*-------------------------函数部分-------------------------*/
public://两个关键函数
	//virtual void NetLink();
	virtual int NetAnalysis();
	int bDynInitial;
	int DynInitial();//计算完潮流之后，需要进行初始化

	/*-------------------------暂态数据-------------------------*/
public:
	float TotalTime;
	float Step, Tnow;
	int MaxIterNumber;
	int FlagExit;//退出计算的符号
	float NextEvent;//下一个事件的事件
public://导纳矩阵——用于暂态计算
	float *Vx, *Vy;//求解迭代过程中的电压
	float *Ix, *Iy;//求解迭代过程中的电流
	int bDynMatrixForm;
	class COMPLEXSPAREMATRIX m_DynMatrix;
	int GetVxVy(int busno, real &vx, real &vy);
public:
	void FormJacobi();//形成Jacobi矩阵，使用注入电流法进行计算
	void DeleteBranch(int BranchNo);
	void DeleteOneEquipment(int EquipNo);
	void ModifyNetMatrix(int busno, real P, real Q);//用于有名值
	void ModifyNetMatrix(int ibusno, int jbusno, real g, real b);//用于标幺值

public://读写文件
	virtual int ReadFile(char*file);
};

#endif

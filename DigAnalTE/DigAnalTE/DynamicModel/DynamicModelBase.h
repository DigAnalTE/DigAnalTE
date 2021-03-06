
#ifndef __DigAnalTE_DYNAMIC_MODEL_BASE_H__
#define __DigAnalTE_DYNAMIC_MODEL_BASE_H__

#include "../CommonFunction/constdef.h"
#include "../NetWorkInfo/EquipmentBase.h"

class EQUIPMENT_DYN_MODEL;
class DYNAMIC_MODEL_BASE
{//动态模型的基类
public:
	DYNAMIC_MODEL_BASE()
	{
		ModelName[0] = '\0';
		EquipmentName[0] = '\0';
		pEquip = NULL;
		pModel = NULL;
		mPrio = 999;//最差
		m_State = 1;
		BMVA = 100.;
		pSolInfo = NULL;
	}
	~DYNAMIC_MODEL_BASE(){}
public:
	char ModelName[_MaxNameLen];
	char* GetModelName(){ return ModelName; }
	virtual EQUIPMENT_DYN_MODEL* IsEquipModel(){ return NULL; }
public://母线信息
	int EquipIndex;//设备号
	char EquipmentName[_MaxNameLen];//无论什么动态模型，都要与一个元件相关联
	EQUIPMENTBASE* pEquip;
	char* GetEquipmentName(){ return EquipmentName; }
	class EQUIPMENT_DYN_MODEL *pModel;//如果只是控制模型的话，需要与元件模型相连
public:
	float mPrio;//初始化的优先度，比较麻烦的一个量，由于初始化，计算需要按照顺序进行，所以必须设置优先度
	int m_State;//本模型的状态
	real BMVA;
	class DYNAMICMODELINFO* pSolInfo;
	void SetSolInfo(DYNAMICMODELINFO* tSol){ pSolInfo = tSol; }
public://读写部分
	virtual int ReadLine(char*Line) = 0;
	virtual void PrintInfo(char*Line) = 0;
	virtual int CheckInputData(){ return 1; }//数据检查//在潮流计算前调用
public://计算部分//使用pSolInfo进行控制
	virtual int DynInitial(){ return 1; }//微分方程初值计算
	virtual void DynBeforeStep() = 0;//积分前处理//可以使用显式算法进行预测
	virtual real DynProcessStep() = 0;//积分//隐式梯形积分法
	virtual void DynAfterStep() = 0;//积分后处理
	virtual void DynFailedStep(){}//积分失败处理
	virtual void DynEventProcess(){}//事件处理
};

struct DynVarient
{
	char IndexName[_MaxNameLen];//模型间索引名
	char OutName[_MaxNameLen];//输出曲线名
	real *pValu;
	int outflag;//非零表示输出到文件
};

class EQUIPMENT_DYN_MODEL :public DYNAMIC_MODEL_BASE
{//有注入电流的模型
public:
	EQUIPMENT_DYN_MODEL()
	{
		mPrio = 0;//最好
		pIx = NULL;
		pIy = NULL;
		Varient = NULL;
	}
public:
	virtual EQUIPMENT_DYN_MODEL* IsEquipModel(){ return this; }
public://为其他模型做接口
	DynVarient *Varient;//必须指向派生类
	virtual int GetVarientTotal() = 0;//获得输出总数
	real* GetVarient(char *name);
	DynVarient* GetDynVarient(char *name, int flag = 0);//flag==0不退出，flag==1退出
	real nVarient[10];//预留10个空间供控制模型进行交互
	real* GetnVarient(int no);//对预留空间进行访问
	virtual int ReadOutLine(char*Line);
public://为设备做接口
	real *pIx, *pIy;
	virtual void ModifyMatrix(){}
	virtual real DynCalculateCurrent(){ return 0; }
};

#endif

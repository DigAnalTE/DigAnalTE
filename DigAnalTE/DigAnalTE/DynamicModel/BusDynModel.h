
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
public://�������
	DynVarient mVarient[5];
	virtual int GetVarientTotal(){ return 5; }
public:
	virtual int ReadLine(char*Line){ return 1; }
	virtual void PrintInfo(char*Line){}
	virtual int CheckInputData();
public:
	//virtual void ModifyMatrix();
	virtual int DynInitial();//΢�ַ��̳�ֵ����
	virtual void DynBeforeStep();//����ǰ����//����ʹ����ʽ�㷨����Ԥ��
	virtual real DynProcessStep();//����//��ʽ���λ��ַ�
	//virtual real DynCalculateCurrent();
	virtual void DynAfterStep();//���ֺ���
};

#endif

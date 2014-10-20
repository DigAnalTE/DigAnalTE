
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
	real OldW, OldIx, OldIy;//�����ж��Ƿ�����
public://�������
	DynVarient mVarient[4];
	virtual int GetVarientTotal(){ return 4; }
public:
	virtual int ReadLine(char*Line);
	virtual void PrintInfo(char*Line);
	virtual int CheckInputData();//��ֵ���
public:
	virtual void ModifyMatrix();
	virtual int DynInitial();//΢�ַ��̳�ֵ����
	virtual void DynBeforeStep();//����ǰ����//����ʹ����ʽ�㷨����Ԥ��
	virtual real DynProcessStep();//����//��ʽ���λ��ַ�
	virtual real DynCalculateCurrent();
	virtual void DynAfterStep();//���ֺ���
};

#endif


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
	real OldW;//�����ж��Ƿ�����
	real dSita, dF, PMout, PEout, Qout;
public://�������
	DynVarient mVarient[6];
	virtual int GetVarientTotal(){ return 6; }
public:
	virtual int ReadLine(char*Line);
	virtual int ReadOutLine(char*Line);
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

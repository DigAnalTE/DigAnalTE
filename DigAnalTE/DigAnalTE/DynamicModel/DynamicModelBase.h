
#ifndef __DigAnalTE_DYNAMIC_MODEL_BASE_H__
#define __DigAnalTE_DYNAMIC_MODEL_BASE_H__

#include "../CommonFunction/constdef.h"
#include "../NetWorkInfo/EquipmentBase.h"

class EQUIPMENT_DYN_MODEL;
class DYNAMIC_MODEL_BASE
{//��̬ģ�͵Ļ���
public:
	DYNAMIC_MODEL_BASE()
	{
		ModelName[0] = '\0';
		EquipmentName[0] = '\0';
		pEquip = NULL;
		pModel = NULL;
		mPrio = 999;//���
		m_State = 1;
		BMVA = 100.;
		pSolInfo = NULL;
	}
	~DYNAMIC_MODEL_BASE(){}
public:
	char ModelName[_MaxNameLen];
	char* GetModelName(){ return ModelName; }
	virtual EQUIPMENT_DYN_MODEL* IsEquipModel(){ return NULL; }
public://ĸ����Ϣ
	int EquipIndex;//�豸��
	char EquipmentName[_MaxNameLen];//����ʲô��̬ģ�ͣ���Ҫ��һ��Ԫ�������
	EQUIPMENTBASE* pEquip;
	char* GetEquipmentName(){ return EquipmentName; }
	class EQUIPMENT_DYN_MODEL *pModel;//���ֻ�ǿ���ģ�͵Ļ�����Ҫ��Ԫ��ģ������
public:
	float mPrio;//��ʼ�������ȶȣ��Ƚ��鷳��һ���������ڳ�ʼ����������Ҫ����˳����У����Ա����������ȶ�
	int m_State;//��ģ�͵�״̬
	real BMVA;
	class DYNAMICMODELINFO* pSolInfo;
	void SetSolInfo(DYNAMICMODELINFO* tSol){ pSolInfo = tSol; }
public://��д����
	virtual int ReadLine(char*Line) = 0;
	virtual void PrintInfo(char*Line) = 0;
	virtual int CheckInputData(){ return 1; }//���ݼ��//�ڳ�������ǰ����
public://���㲿��//ʹ��pSolInfo���п���
	virtual int DynInitial(){ return 1; }//΢�ַ��̳�ֵ����
	virtual void DynBeforeStep() = 0;//����ǰ����//����ʹ����ʽ�㷨����Ԥ��
	virtual real DynProcessStep() = 0;//����//��ʽ���λ��ַ�
	virtual void DynAfterStep() = 0;//���ֺ���
	virtual void DynFailedStep(){}//����ʧ�ܴ���
	virtual void DynEventProcess(){}//�¼�����
};

struct DynVarient
{
	char IndexName[_MaxNameLen];//ģ�ͼ�������
	char OutName[_MaxNameLen];//���������
	real *pValu;
	int outflag;//�����ʾ������ļ�
};

class EQUIPMENT_DYN_MODEL :public DYNAMIC_MODEL_BASE
{//��ע�������ģ��
public:
	EQUIPMENT_DYN_MODEL()
	{
		mPrio = 0;//���
		pIx = NULL;
		pIy = NULL;
		Varient = NULL;
	}
public:
	virtual EQUIPMENT_DYN_MODEL* IsEquipModel(){ return this; }
public://Ϊ����ģ�����ӿ�
	DynVarient *Varient;//����ָ��������
	virtual int GetVarientTotal() = 0;//����������
	real* GetVarient(char *name);
	DynVarient* GetDynVarient(char *name, int flag = 0);//flag==0���˳���flag==1�˳�
	real nVarient[10];//Ԥ��10���ռ乩����ģ�ͽ��н���
	real* GetnVarient(int no);//��Ԥ���ռ���з���
	virtual int ReadOutLine(char*Line);
public://Ϊ�豸���ӿ�
	real *pIx, *pIy;
	virtual void ModifyMatrix(){}
	virtual real DynCalculateCurrent(){ return 0; }
};

#endif

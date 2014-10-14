
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

public://��̬ģ����Ϣ
	int DynamicModelTotal;
	DYNAMIC_MODEL_BASE *DynamicModel[_MaxDynamicModelNo];
	int InsertNewDynamicModel(DYNAMIC_MODEL_BASE*);
	/*-------------------------��������-------------------------*/
public://�����ؼ�����
	//virtual void NetLink();
	virtual int NetAnalysis();
	int bDynInitial;
	int DynInitial();//�����곱��֮����Ҫ���г�ʼ��

	/*-------------------------��̬����-------------------------*/
public:
	float TotalTime;
	float Step, Tnow;
	int MaxIterNumber;
	int FlagExit;//�˳�����ķ���
	float NextEvent;//��һ���¼����¼�
public://���ɾ��󡪡�������̬����
	float *Vx, *Vy;//�����������еĵ�ѹ
	float *Ix, *Iy;//�����������еĵ���
	int bDynMatrixForm;
	class COMPLEXSPAREMATRIX m_DynMatrix;
	int GetVxVy(int busno, real &vx, real &vy);
public:
	void FormJacobi();//�γ�Jacobi����ʹ��ע����������м���
	void DeleteBranch(int BranchNo);
	void DeleteOneEquipment(int EquipNo);
	void ModifyNetMatrix(int busno, real P, real Q);//��������ֵ
	void ModifyNetMatrix(int ibusno, int jbusno, real g, real b);//���ڱ���ֵ

public://��д�ļ�
	virtual int ReadFile(char*file);
};

#endif

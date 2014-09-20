
#ifndef __DigAnalTE_BRANCH_BASE_H__
#define __DigAnalTE_BRANCH_BASE_H__

//��·�࣬��Ҫ�ǽ����豸�������ߺͱ�ѹ��
//������û�������ѹ���������ЧΪ������ѹ��
//ֱ���Ȳ�����������·���������豸�����豸���й���

#include "../CommonFunction/constdef.h"

class BRANCHBASE
{
public:
	BRANCHBASE()
	{
		c_BranchType=' ';State=1;
		FromBusNo=-1;ToBusNo=-1;
		TK=1;Smax=0;
		m_pFBusV=NULL;m_pFBusSita=NULL;m_pTBusV=NULL;m_pTBusSita=NULL;
		IP=IQ=JP=JQ=0;
		Ploss=Qloss=0;
	}
public:
	char c_BranchType;//�����������ʶ��
public://��·��Ϣ
	char Name[_MaxNameLen];
	int State;//�Ƿ�Ͷ������. =1:Ͷ�� ����:ͣ��
	char BusName1[_MaxNameLen],BusName2[_MaxNameLen];
	int FromBusNo,ToBusNo;
public://��·�Ļ�������//ע��ȫ��Ϊ��ôֵ
	float R,X,G,B,TK;//�����Ǳ�ѹ��
	float Smax;//�������//����ֵ
public:
	int iGetState(){return State;}
	void iSetState(int tState){State=tState;}
	int iGetFromBusNo(){return FromBusNo;}
	int iGetToBusNo(){return ToBusNo;}
	float fGetLineLoss(){return Ploss;}
public://��д����
	virtual int ReadLine(char*Line);
	virtual void WriteLine(char*Line);
	void PrintInfo(char*Line)
	{//�������������Ϣ
		sprintf(Line,"%s--%s",BusName1,BusName2);
	}
	virtual void OutputPFOFile(FILE*fp,int nDirtn);//���������Ϣ//nDirtn: 1���� 2����
public:
	virtual void VarientLink(class NETWORK_BASE*);

//--------------------��������--------------------
public://�������
	float IP,IQ,JP,JQ;//������������˳���//ʵ��ֵ
	float Ploss,Qloss;
	float *m_pFBusV,*m_pFBusSita;//��ʼ��//��ĸ�ߵ�ѹ//Update��ʹ���ˣ��ڳ������������û��ʹ��
	float *m_pTBusV,*m_pTBusSita;//��ֹ��
public://�����������
	//��һ�����޸����е����緽�̲���
	real Y11r,Y12r,Y21r,Y22r;//������ʱ����//subJacElement���㣬Ȼ��JacElement�ӵ�NETWORKINFO��
	real Y11i,Y12i,Y21i,Y22i;
	virtual void subJacElement();
	virtual void JacElement(class NETWORKINFO*pNet);
	//�ڶ������ڼ����������ÿ�����������еĵ���
	virtual int BeforeIterCal(int iter){return 1;}//��ÿһ�ε���ǰ����//����ֵ��Ϊ1����ʾ����
	virtual int AfterIterCal(){return 0;}//��ÿһ�ε�����ɺ���ã��������ļ���״̬��//����ֵ�����ʾ״̬�޸�
	//�����������������ļ���
	virtual void UpdateValue(class NETWORKINFO*pNet);
	virtual void SubUpdateValue(class NETWORKINFO*pNet){}//����֮����Զ�����������ֵ
};

#endif

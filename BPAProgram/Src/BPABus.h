
#ifndef __BPA_BUS_BASE_H__
#define __BPA_BUS_BASE_H__

#include "../../DigAnalTE/DigAnalTE/NetWorkInfo/BusBase.h"

class BPABUS : public BUSBASE
{
public:
	BPABUS()
	{
		cBusType='B';
		c_BPAType=' ';
		Owner[0]='\0';
		m_fGenP=0;m_fGenQ=0;m_fGenPmax=0;m_fGenQmax=0;m_fGenQmin=0;
		m_fBusPPLoad=0;m_fBusQPLoad=0;m_fBusPLoadPer=1.f;m_fBusQLoadPer=1.f;
		m_fBusPalPLoad=0;m_fBusPalQLoad=0;
		m_fGenPper=0;m_fGenQper=0;IsLNCard=0;
	}
public://BPA����������//������
	char c_BPAType;
public://����
	char Owner[_MaxNameLen];
public://��д����
	virtual int ReadLine(char*Line);
	virtual void OutputPFOFile(FILE*fp);//���������Ϣ
public://�����������
	//��һ�����޸����е����緽�̲���
	virtual void JacElement(class NETWORKINFO*Topo);
	//�ڶ������ڼ����������ÿ�����������еĵ���
	virtual int BeforeIterCal(int iter){return 1;}//��ÿһ�ε���ǰ����//����ֵ��Ϊ1����ʾ����
	virtual int AfterIterCal(int TypeChange=0){return 1;}//��ÿһ�ε�����ɺ���ã��������ļ���״̬��//����ֵ��Ϊ1����ʾ��δ����
	//�����������������ļ���
	virtual void UpdateValue(class NETWORKINFO*Topo);
public://ĸ�������ӵ��豸����ȫ��Ϊ�˼���BPA����ƣ�������Ҫ�����е��豸���е�����ģ
	//���粿��
	float m_fGenP,m_fGenQ;
	float m_fGenPmax;
	float m_fGenQmax,m_fGenQmin;
	//���ɲ���
	float m_fBusPPLoad,m_fBusQPLoad;
	float m_fBusPLoadPer,m_fBusQLoadPer;
	//���ݵ翹����
	float m_fBusPalPLoad,m_fBusPalQLoad;
	//��ϸ��ģ֮��ĸ���ϵ�Чʣ�ಿ��//����ϸ��ģ֮�󣬴˲���Ӧ��ȫ��Ϊ0
	float m_fGenPper,m_fGenQper;int IsLNCard;//IsLNCard=1ʱʣ�෢�����ЧΪ����
	void SetLNCard(){IsLNCard=1;}
};

#endif


#ifndef __DigAnalTE_BUS_BASE_H__
#define __DigAnalTE_BUS_BASE_H__

//ĸ��ʵ����
//������ʵ�ִ󲿷ֲ�����ԭ���ϲ������������еĿ���Ӧ�÷ŵ��豸��

#include "../CommonFunction/constdef.h"

class BUSBASE
{
	friend class BUSINFO;
public:
	BUSBASE()
	{
		Removed=0;
		cBusType=' ';
		Name[0]='\0';BaseKv=100.f;
		Zone[0]='\0';ZoneNo=-1;AreaNo=-1;
		Owner[0]='\0';
		m_fBusVmax=1.5f;m_fBusVmin=0.f;
		m_fBusV=1;m_fBusSita=0;m_fPerr=0;m_fQerr=0;
	}
public:
	int Removed;//������
	char cBusType;//�����������ʶ��
public:
	char Name[_MaxNameLen];
	float BaseKv;
	int BusNo;
	char Zone[_MaxNameLen];
	int ZoneNo,AreaNo;
	float m_fBusVmax,m_fBusVmin;//ĸ�ߵ�ѹ�����ޣ�ֻ���ڱ���
public:
	char Owner[_MaxNameLen];
public:
	char GetBusType(){return cBusType;}
	char* GetBusName(){return Name;}
	char* GetZoneName(){return Zone;}
	char* GetOwnerName(){return Owner;}
	float fGetBaseKv(){return BaseKv;}
public://��д����
	virtual int ReadLine(char*Line);
	virtual void WriteLine(char*Line);
	virtual void PrintInfo(char*Line);//���ھ������Ϣ
	virtual void OutputPFOFile(FILE*fp);//���������Ϣ
public://�������ӹ�ϵ
	virtual void VarientLink(class NETWORK_BASE*){}//ר��Ϊ����ڵ�����������ṩ

//--------------------��������--------------------
public://�������
	float m_fBusV,m_fBusSita;//��ѹ�����
	float m_fPerr,m_fQerr;//�й��޹����

public://�����������
	//��һ�����޸����е����緽�̲���
	virtual void JacElement(class NETWORKINFO*pNet){}
	//�ڶ������ڼ����������ÿ�����������еĵ���
	virtual int BeforeIterCal(int iter){return 1;}//��ÿһ�ε���ǰ����//����ֵ��Ϊ1����ʾ����
	virtual int AfterIterCal(){return 0;}//��ÿһ�ε�����ɺ���ã��������ļ���״̬��//����ֵ�����ʾ״̬�޸�
	//�����������������ļ���
	virtual void UpdateValue(class NETWORKINFO*pNet){}
};

#endif

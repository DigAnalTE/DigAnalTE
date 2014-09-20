
#ifndef __BPA_BRANCH_H__
#define __BPA_BRANCH_H__

#include "../../DigAnalTE/DigAnalTE/NetWorkInfo/BranchBase.h"

class BPABRANCH : public BRANCHBASE
{
public:
	BPABRANCH()
	{
		c_BranchType='B';
		c_BPAType=' ';
		Owner[0]='\0';
		Ratio1=Ratio2=1;
		G1=B1=G2=B2=0;
		Gk1=Bk1=Gk2=Bk2=0;
	}
public://BPA����������//������
	char c_BPAType;
	float BaseKv1,BaseKv2;
	char ID;
	char Owner[_MaxNameLen];
	float G1,B1,G2,B2; //���Գ���·
	float Ratio1,Ratio2;//��ֵΪ������
	float Gk1,Bk1,Gk2,Bk2; //�߿�
	int m_nMeter;	//���ʲ��
	virtual void subJacElement();
public://��д����
	virtual int ReadLine(char*Line);
	virtual int subReadBPALine(char*)=0;
	virtual void OutputPFOFile(FILE*fp,int nDirtn);//���������Ϣ
};

class LBRANCH:public BPABRANCH
{
public:
	LBRANCH(){c_BPAType='L';}
protected:
	float Imax;//��·�����
	int Ncnt;//������·��Ŀ��������Ϣ��
	float Length;//����
	char Description[9];//����
	char m_iMonth[2];	//Ͷ���·�
	int  m_iYear;		//Ͷ�����
	char m_jMonth[2];	//ͣ���·�
	int  m_jYear;		//ͣ�����
public:
	virtual int subReadBPALine(char*);
};

class EBRANCH:public BPABRANCH
{
public:
	EBRANCH(){c_BPAType='E';}
protected:
	float Imax;//��·�����
	int Ncnt;//������·��Ŀ��������Ϣ��
	char m_iMonth[2];	//Ͷ���·�
	int  m_iYear;		//Ͷ�����
	char m_jMonth[2];	//ͣ���·�
	int  m_jYear;		//ͣ�����
public:
	virtual int subReadBPALine(char*);
};

class TBRANCH:public BPABRANCH
{
	friend class BRANCHINFO;
public:
	TBRANCH(){c_BPAType='T';}
	~TBRANCH(){}
protected:
	float Imax;//����
	int Ncnt;//������·��Ŀ��������Ϣ��
	float TK1;
	float TK2;
	char m_iMonth[2];	//Ͷ���·�
	int  m_iYear;		//Ͷ�����
	char m_jMonth[2];	//ͣ���·�
	int  m_jYear;		//ͣ�����
public:
	virtual int subReadBPALine(char*);
};


#endif


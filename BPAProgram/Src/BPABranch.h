
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
public://BPA派生的类型//子类型
	char c_BPAType;
	float BaseKv1,BaseKv2;
	char ID;
	char Owner[_MaxNameLen];
	float G1,B1,G2,B2; //不对称线路
	float Ratio1,Ratio2;//该值为计算结果
	float Gk1,Bk1,Gk2,Bk2; //高抗
	int m_nMeter;	//功率测点
	virtual void subJacElement();
public://读写部分
	virtual int ReadLine(char*Line);
	virtual int subReadBPALine(char*)=0;
	virtual void OutputPFOFile(FILE*fp,int nDirtn);//输出潮流信息
};

class LBRANCH:public BPABRANCH
{
public:
	LBRANCH(){c_BPAType='L';}
protected:
	float Imax;//线路额定电流
	int Ncnt;//并联线路数目，仅作信息用
	float Length;//长度
	char Description[9];//描述
	char m_iMonth[2];	//投运月份
	int  m_iYear;		//投运年份
	char m_jMonth[2];	//停运月份
	int  m_jYear;		//停运年份
public:
	virtual int subReadBPALine(char*);
};

class EBRANCH:public BPABRANCH
{
public:
	EBRANCH(){c_BPAType='E';}
protected:
	float Imax;//线路额定电流
	int Ncnt;//并联线路数目，仅作信息用
	char m_iMonth[2];	//投运月份
	int  m_iYear;		//投运年份
	char m_jMonth[2];	//停运月份
	int  m_jYear;		//停运年份
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
	float Imax;//容量
	int Ncnt;//并联线路数目，仅作信息用
	float TK1;
	float TK2;
	char m_iMonth[2];	//投运月份
	int  m_iYear;		//投运年份
	char m_jMonth[2];	//停运月份
	int  m_jYear;		//停运年份
public:
	virtual int subReadBPALine(char*);
};


#endif


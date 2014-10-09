
#ifndef __PSASP_DCLINE_H__
#define __PSASP_DCLINE_H__

#include "../../DigAnalTE/DigAnalTE/NetWorkInfo/DCLineBase.h"

class PSASPDCLINE : public DCLINEBASE
{
public:
	PSASPDCLINE()
	{
		iLine = 0;
	}
private:
	int iLine;//需要读入5行数据
public://数据
	//int m_iMark;
	int m_iIname;	//I_name在L1中行号
	int m_iJname;	//J_Name在L1中行号
	int m_iLineNo;
	float m_R1;
	float m_L1;
	float m_R2;
	float m_L2;
	float m_RL;
	float m_XL;
	float m_Re;
	float m_Le;
	float m_Rs;
	float m_Ls;
	float m_Vhi;
	float m_Vli;
	int m_Bi;
	float m_Qci;
	float m_Sti;
	float m_Rki;
	float m_Xki;
	float m_Vmaxi;
	float m_Vmini;
	int m_Ni;
	float m_Vhj;
	float m_Vlj;
	int m_Bj;
	float m_Qcj;
	float m_Stj;
	float m_Rkj;
	float m_Xkj;
	float m_Vmaxj;
	float m_Vminj;
	int m_Nj;
	int m_OP;
	float m_Id;
	float m_Vd;
	float m_Amin;
	float m_A0;
	float m_Bmin;
	float m_B0;
	int m_iAreaNo;
public://读写部分
	virtual int ReadLine(char*Line);
	//virtual void OutputPFOFile(FILE*fp);//输出潮流信息
};

#endif

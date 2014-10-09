
#ifndef __PSASP_BRANCH_H__
#define __PSASP_BRANCH_H__

#include "../../DigAnalTE/DigAnalTE/NetWorkInfo/BranchBase.h"

class PSASPACLINE : public BRANCHBASE
{
public:
	PSASPACLINE()
	{
		c_BranchType = 'P';
	}
public://����
	int m_iMark;	//1��I_break=.t. and J_break=.t.//I J��·���պ�
					//2��I_break=.f. and J_break=.t.
					//3��I_break=.t. and J_break=.f.
					//0��I_break=.f. and J_break=.f.
	int m_iIname;	//I_name��L1���к�
	int m_iJname;	//J_Name��L1���к�
	int m_iLineNo;	//ID_No,��·���
	//float m_R;	//R1, ���ݵ�λΪ��ôֵ
	//float m_X;	//X1
	int m_iFT;		//���������߹�����־From/To�� ��������������
					//0������·����I��ĸ��������
					//n������·��������n
	int m_iType;	//Mode_Ctrl ��·���ƹ�������
	int m_iPL;		//Par_No �Զ���ģ�Ͳ������
	int m_iCB;		//Bus_Ctrl��L1���кţ�����ĸ����
	int m_iCL;		//Line_ctrl, ������·���
	float m_VQP;	//Value_Ctr �������ĸ���ֵ
	float m_LC;		//Rate_kA ��·�����(kA)
	float m_LIM;	//Up_limit ����·�������޵İٷ���������������=LC*LIM/100.
public://�������
	float m_Pi, m_Qi, m_Pj, m_Qj, m_Qci, m_Qcj;
	virtual void JacElement(class NETWORKINFO*pNet);
public://��д����
	virtual int ReadLine(char*Line);
	virtual void OutputPFOFile(FILE*fp, int nDirtn);//���������Ϣ
	virtual void subUpdateValue(class NETWORKINFO*pNet);
};

#endif

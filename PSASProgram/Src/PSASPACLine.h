
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
public://数据
	int m_iMark;	//1：I_break=.t. and J_break=.t.//I J断路器闭合
					//2：I_break=.f. and J_break=.t.
					//3：I_break=.t. and J_break=.f.
					//0：I_break=.f. and J_break=.f.
	int m_iIname;	//I_name在L1中行号
	int m_iJname;	//J_Name在L1中行号
	int m_iLineNo;	//ID_No,线路编号
	//float m_R;	//R1, 数据单位为标么值
	//float m_X;	//X1
	int m_iFT;		//区域联络线归属标志From/To， 其损耗算入归属区
					//0：该线路属于I侧母线所在区
					//n：该线路属于区域n
	int m_iType;	//Mode_Ctrl 线路控制功能类型
	int m_iPL;		//Par_No 自定义模型参数组号
	int m_iCB;		//Bus_Ctrl在L1中行号，被控母线名
	int m_iCL;		//Line_ctrl, 被控线路编号
	float m_VQP;	//Value_Ctr 被控量的给定值
	float m_LC;		//Rate_kA 线路额定容量(kA)
	float m_LIM;	//Up_limit 该线路容量上限的百分数，即容量上限=LC*LIM/100.
public://潮流结果
	float m_Pi, m_Qi, m_Pj, m_Qj, m_Qci, m_Qcj;
	virtual void JacElement(class NETWORKINFO*pNet);
public://读写部分
	virtual int ReadLine(char*Line);
	virtual void OutputPFOFile(FILE*fp, int nDirtn);//输出潮流信息
	virtual void subUpdateValue(class NETWORKINFO*pNet);
};

#endif

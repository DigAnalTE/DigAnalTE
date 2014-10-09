
#ifndef __PSASP_GENERATOR_H__
#define __PSASP_GENERATOR_H__

#include "../../DigAnalTE/DigAnalTE/NetWorkInfo/Generator.h"

class PSASPGENERATOR : public GENERATOR
{
public:
	PSASPGENERATOR()
	{
	}
public://数据
	//int m_iMark;	//发电机数据标记 1 为有效 
					//1：valid=.t.，0：valid=.f.

	int m_iBusNo;	//bus_name在L1中行号

	int m_iType;	//MODE_CTRL 该母线类型
					//      0: 平衡节点
					//      1:  PQ节点
					//     -1:  PV节点
					//     -2:  PV节点，控制该点无功Qmin<=Q<=Qmax，越限后变为PQ点
					//     -3:  PQ节点，控制该点电压Vmin<=V<=Vmax，越限后变为PV点
	float m_Pg;	//发电机有功，标幺值
	float m_Qg;	//无功
	//float m_V;	//母线电压幅值，标幺值
	float m_sita;	//母线电压幅角(度)
	float m_Qmax;	//发电机无功上限，标幺值
	float m_Qmin;	//发电机无功下限，标幺值
	float m_Pmax;	//发电机有功上限，标幺值
	float m_Pmin;	//发电机有功下限，标幺值

	int m_iPL;		//PAR_GROUP
	int m_iCB;		//Bus_ctrl 被控母线名
	int m_iCL;		//LINE_CTRL 被控线路号
	float m_VQP;	//VALUE_CTRL 被控量给定值，标幺值
	int m_iKcd;		//0//厂用电百分数 K%
public://读写部分
	virtual int ReadLine(char*Line);
	virtual void UpdateValue(NETWORKINFO*pNet);
	virtual void OutputPFOFile(FILE*fp, int nDirtn);//输出潮流信息
};

#endif

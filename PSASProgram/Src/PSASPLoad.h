
#ifndef __PSASP_LOAD_H__
#define __PSASP_LOAD_H__

#include "../../DigAnalTE/DigAnalTE/NetWorkInfo/NetLoad.h"

class PSASPLOAD : public NETLOAD
{
public:
	PSASPLOAD()
	{
	}
public://数据
	//int m_iMark;	//负荷数据标记
					//1：valid=.t.，0：valid=.f.
	int m_iBusNo;	//bus_name在L1中行号
	int m_iLoadNo;	//No_Load
	int m_iType;	//MODE_CTRL 该母线类型
					//      0: 平衡节点
					//      1:  PQ节点
					//     -1:  PV节点
					//     -2:  PV节点，控制该点无功Qmin<=Q<=Qmax，越限后变为PQ点
					//     -3:  PQ节点，控制该点电压Vmin<=V<=Vmax，越限后变为PV点
	float m_Pl;		//负荷有功功率，标幺值
	float m_Ql;		//负荷无功功率，标幺值
	float m_V;		//母线电压幅值，pu
	float m_sita;	//母线电压幅角，度
	float m_Qmax;	//负荷无功上限
	float m_Qmin;	//无功下限
	float m_Pmax;	//负荷有功上限
	float m_Pmin;	//有功下限
	int m_iPL;		//PAR_GROUP
	int m_iCB;		//Bus_ctrl 被控母线名
	int m_iCL;		//LINE_CTRL 被控线路号
	float m_VQP;	//VALUE_CTRL 被控量给定值，标幺值
public://读写部分
	virtual int ReadLine(char*Line);
	virtual void UpdateValue(NETWORKINFO*pNet);
	virtual void OutputPFOFile(FILE*fp, int nDirtn);//输出潮流信息
};

#endif

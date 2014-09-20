
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
public://BPA派生的类型//子类型
	char c_BPAType;
public://数据
	char Owner[_MaxNameLen];
public://读写部分
	virtual int ReadLine(char*Line);
	virtual void OutputPFOFile(FILE*fp);//输出潮流信息
public://潮流计算过程
	//第一步：修改其中的网络方程部分
	virtual void JacElement(class NETWORKINFO*Topo);
	//第二步：在计算过程中在每步迭代过程中的调整
	virtual int BeforeIterCal(int iter){return 1;}//在每一次迭代前调用//返回值不为1，表示出错
	virtual int AfterIterCal(int TypeChange=0){return 1;}//在每一次迭代完成后调用，用来更改计算状态的//返回值不为1，表示尚未收敛
	//第三步：计算结束后的计算
	virtual void UpdateValue(class NETWORKINFO*Topo);
public://母线上连接的设备。完全是为了兼容BPA而设计，将来需要对所有的设备进行单独建模
	//发电部分
	float m_fGenP,m_fGenQ;
	float m_fGenPmax;
	float m_fGenQmax,m_fGenQmin;
	//负荷部分
	float m_fBusPPLoad,m_fBusQPLoad;
	float m_fBusPLoadPer,m_fBusQLoadPer;
	//电容电抗部分
	float m_fBusPalPLoad,m_fBusPalQLoad;
	//详细建模之后母线上等效剩余部分//在详细建模之后，此部分应该全部为0
	float m_fGenPper,m_fGenQper;int IsLNCard;//IsLNCard=1时剩余发电机等效为负荷
	void SetLNCard(){IsLNCard=1;}
};

#endif

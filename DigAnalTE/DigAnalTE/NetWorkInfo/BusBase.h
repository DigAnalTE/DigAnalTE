
#ifndef __DigAnalTE_BUS_BASE_H__
#define __DigAnalTE_BUS_BASE_H__

//母线实现类
//该类已实现大部分操作，原则上不必派生，所有的控制应该放到设备上

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
	int Removed;//不计算
	char cBusType;//用于派生类的识别
public:
	char Name[_MaxNameLen];
	float BaseKv;
	int BusNo;
	char Zone[_MaxNameLen];
	int ZoneNo,AreaNo;
	float m_fBusVmax,m_fBusVmin;//母线电压上下限，只用于报警
public:
	char Owner[_MaxNameLen];
public:
	char GetBusType(){return cBusType;}
	char* GetBusName(){return Name;}
	char* GetZoneName(){return Zone;}
	char* GetOwnerName(){return Owner;}
	float fGetBaseKv(){return BaseKv;}
public://读写部分
	virtual int ReadLine(char*Line);
	virtual void WriteLine(char*Line);
	virtual void PrintInfo(char*Line);//用于警告等信息
	virtual void OutputPFOFile(FILE*fp);//输出潮流信息
public://建立连接关系
	virtual void VarientLink(class NETWORK_BASE*){}//专门为特殊节点的特殊需求提供

//--------------------潮流计算--------------------
public://潮流结果
	float m_fBusV,m_fBusSita;//电压、相角
	float m_fPerr,m_fQerr;//有功无功误差

public://潮流计算过程
	//第一步：修改其中的网络方程部分
	virtual void JacElement(class NETWORKINFO*pNet){}
	//第二步：在计算过程中在每步迭代过程中的调整
	virtual int BeforeIterCal(int iter){return 1;}//在每一次迭代前调用//返回值不为1，表示出错
	virtual int AfterIterCal(){return 0;}//在每一次迭代完成后调用，用来更改计算状态的//返回值非零表示状态修改
	//第三步：计算结束后的计算
	virtual void UpdateValue(class NETWORKINFO*pNet){}
};

#endif

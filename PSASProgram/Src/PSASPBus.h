
#ifndef __PSASP_BUS_H__
#define __PSASP_BUS_H__

#include "../../DigAnalTE/DigAnalTE/NetWorkInfo/BusBase.h"

class PSASPBUS : public BUSBASE
{
public:
	PSASPBUS()
	{
		cBusType = 'P';
	}
public://数据
	//char m_cBusName[21];	//Bus_Name。如果Bus_Name长度小于等于10，在前十位输出，并向第10位右对齐输出；否则右对齐输出。
	//float m_Vbase;		//Base_kV
	int m_iAreaNo;	//Area_No
	float m_Vmax;	//Vmax_kV
	float m_Vmin;	//Vmin_kV
	float m_CB1;	//SC1_MVA 母线断路器额定单相短路容量MVA，用于短路电流分析
	float m_CB3;	//SC3_MVA 三相
public://读写部分
	virtual int ReadLine(char*Line);
	virtual void OutputPFOFile(FILE*fp);//输出潮流信息
};

#endif


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
public://����
	//char m_cBusName[21];	//Bus_Name�����Bus_Name����С�ڵ���10����ǰʮλ����������10λ�Ҷ�������������Ҷ��������
	//float m_Vbase;		//Base_kV
	int m_iAreaNo;	//Area_No
	float m_Vmax;	//Vmax_kV
	float m_Vmin;	//Vmin_kV
	float m_CB1;	//SC1_MVA ĸ�߶�·��������·����MVA�����ڶ�·��������
	float m_CB3;	//SC3_MVA ����
public://��д����
	virtual int ReadLine(char*Line);
	virtual void OutputPFOFile(FILE*fp);//���������Ϣ
};

#endif

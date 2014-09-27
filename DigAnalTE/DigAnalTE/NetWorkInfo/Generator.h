
#ifndef __DigAnalTE_GENERATOR_H__
#define __DigAnalTE_GENERATOR_H__

#include "EquipmentBase.h"

class GENERATOR : public ONEEQUIPMENTBASE
{
public:
	GENERATOR()
	{
		c_EquipType = 'G'; GenType = 1;
		m_fGenP = 0; m_fGenQ = 0; m_fGenPmax = 0; m_fGenPmin = 0; m_fGenQmax = 0; m_fGenQmin = 0;
		m_fGenVCtrl = 1.05f;
	}
public://数据部分
	int GenType;//雅克比类型。3:BS类型 2:PV类型 1:PQ类型
	float m_fGenP, m_fGenQ;
	float m_fGenPmax, m_fGenPmin;
	float m_fGenQmax, m_fGenQmin;
	float m_fBaseMVA;
	float m_fGenVCtrl;//设定电压，该电压是控制电压，不是实际电压//当PQ节点或无功越界时无效
public:
	virtual void subJacElement(class NETWORKINFO*pNet);
	virtual void UpdateValue(class NETWORKINFO*pNet);
public://读写部分
	virtual int ReadLine(char*Line);
	virtual void WriteLine(char*Line);
	virtual void OutputPFOFile(FILE*fp, int nDirtn);
};

#endif

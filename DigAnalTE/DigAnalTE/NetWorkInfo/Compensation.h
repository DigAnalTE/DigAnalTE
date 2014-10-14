
#ifndef __DigAnalTE_COMPENSATION_H__
#define __DigAnalTE_COMPENSATION_H__

#include "EquipmentBase.h"

class COMPENSATION : public ONEEQUIPMENTBASE
{
public:
	COMPENSATION()
	{
		c_EquipType = 'C';
	}
public://数据部分
	float m_fPP, m_fQP;//恒功率
	float m_fPZ, m_fQZ;//恒阻抗
public:
	virtual void subJacElement(class NETWORKINFO*pNet);
public://读写部分
	virtual int ReadLine(char*Line);
	virtual void WriteLine(char*Line);
	virtual void OutputPFOFile(FILE*fp, int nDirtn);
public://暂态计算部分
	virtual void FormDynMatrix(class DYNAMICMODELINFO*);
};

#endif


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
public://���ݲ���
	float m_fPP, m_fQP;//�㹦��
	float m_fPZ, m_fQZ;//���迹
public:
	virtual void subJacElement(class NETWORKINFO*pNet);
public://��д����
	virtual int ReadLine(char*Line);
	virtual void WriteLine(char*Line);
	virtual void OutputPFOFile(FILE*fp, int nDirtn);
public://��̬���㲿��
	virtual void FormDynMatrix(class DYNAMICMODELINFO*);
};

#endif

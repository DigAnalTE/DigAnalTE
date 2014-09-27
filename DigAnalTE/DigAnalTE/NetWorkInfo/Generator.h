
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
public://���ݲ���
	int GenType;//�ſ˱����͡�3:BS���� 2:PV���� 1:PQ����
	float m_fGenP, m_fGenQ;
	float m_fGenPmax, m_fGenPmin;
	float m_fGenQmax, m_fGenQmin;
	float m_fBaseMVA;
	float m_fGenVCtrl;//�趨��ѹ���õ�ѹ�ǿ��Ƶ�ѹ������ʵ�ʵ�ѹ//��PQ�ڵ���޹�Խ��ʱ��Ч
public:
	virtual void subJacElement(class NETWORKINFO*pNet);
	virtual void UpdateValue(class NETWORKINFO*pNet);
public://��д����
	virtual int ReadLine(char*Line);
	virtual void WriteLine(char*Line);
	virtual void OutputPFOFile(FILE*fp, int nDirtn);
};

#endif

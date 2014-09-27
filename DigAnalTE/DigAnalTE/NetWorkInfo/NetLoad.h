
#ifndef __DigAnalTE_NET_LOAD_H__
#define __DigAnalTE_NET_LOAD_H__

#include "EquipmentBase.h"

class NETLOAD : public ONEEQUIPMENTBASE
{
public:
	NETLOAD()
	{
		c_EquipType = 'L';
		m_fLoadP = 0; m_fLoadQ = 0;
		m_LoadType = 0;
		m_fLoadPPer = 1.f; m_fLoadQPer = 1.f;
	}
public://���ݲ���
	//���ɲ���
	float m_fLoadP, m_fLoadQ;//m_fLoadP��ʾ��󸺺� m_fLoadPPer��ʾ��ʽ�µĸ���ϵ��
	int m_LoadType;//�������͡�0�㹦�� 1����� 2���迹 3�Զ��壨�Զ��廹δ������
	float m_fLoadPPer, m_fLoadQPer;
public:
	virtual void subJacElement(class NETWORKINFO*pNet);
	virtual void SetLoadPQper(float tPper, float tQper);
public://��д����
	virtual int ReadLine(char*Line);
	virtual void WriteLine(char*Line);
	virtual void OutputPFOFile(FILE*fp, int nDirtn);
};

#endif

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
public://数据部分
	//负荷部分
	float m_fLoadP, m_fLoadQ;//m_fLoadP表示最大负荷 m_fLoadPPer表示方式下的负荷系数
	int m_LoadType;//负荷类型。0恒功率 1恒电流 2恒阻抗 3自定义（自定义还未制作）
	float m_fLoadPPer, m_fLoadQPer;
public:
	virtual void subJacElement(class NETWORKINFO*pNet);
	virtual void SetLoadPQper(float tPper, float tQper);
public://读写部分
	virtual int ReadLine(char*Line);
	virtual void WriteLine(char*Line);
	virtual void OutputPFOFile(FILE*fp, int nDirtn);
};

#endif
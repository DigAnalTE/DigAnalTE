
#ifndef __DigAnalTE_DCLine_Base_H__
#define __DigAnalTE_DCLine_Base_H__

#include "EquipmentBase.h"

class DCLINEBASE : public TWOEQUIPMENTBASE
{//集成一个等效为负荷的直流线路基类
public:
	DCLINEBASE()
	{
		c_EquipType = 'D';
	}
public://数据部分
	float Pref;//安排直流功率MW
	float Alpha;//整流侧正常触发角
	float Beta;//逆变侧正常关断角
public:
	virtual void subJacElement(class NETWORKINFO*pNet);
	//virtual void UpdateValue(class NETWORKINFO*pNet);
public://读写部分
	virtual int ReadLine(char*Line);
	virtual void WriteLine(char*Line){}
	//virtual void OutputPFOFile(FILE*fp, int nDirtn);
};

#endif


#ifndef __DigAnalTE_DCLine_Base_H__
#define __DigAnalTE_DCLine_Base_H__

#include "EquipmentBase.h"

class DCLINEBASE : public TWOEQUIPMENTBASE
{//����һ����ЧΪ���ɵ�ֱ����·����
public:
	DCLINEBASE()
	{
		c_EquipType = 'D';
	}
public://���ݲ���
	float Pref;//����ֱ������MW
	float Alpha;//����������������
	float Beta;//���������ضϽ�
public:
	virtual void subJacElement(class NETWORKINFO*pNet);
	//virtual void UpdateValue(class NETWORKINFO*pNet);
public://��д����
	virtual int ReadLine(char*Line);
	virtual void WriteLine(char*Line){}
	//virtual void OutputPFOFile(FILE*fp, int nDirtn);
};

#endif

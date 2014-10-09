
#ifndef __PSASP_GENERATOR_H__
#define __PSASP_GENERATOR_H__

#include "../../DigAnalTE/DigAnalTE/NetWorkInfo/Generator.h"

class PSASPGENERATOR : public GENERATOR
{
public:
	PSASPGENERATOR()
	{
	}
public://����
	//int m_iMark;	//��������ݱ�� 1 Ϊ��Ч 
					//1��valid=.t.��0��valid=.f.

	int m_iBusNo;	//bus_name��L1���к�

	int m_iType;	//MODE_CTRL ��ĸ������
					//      0: ƽ��ڵ�
					//      1:  PQ�ڵ�
					//     -1:  PV�ڵ�
					//     -2:  PV�ڵ㣬���Ƹõ��޹�Qmin<=Q<=Qmax��Խ�޺��ΪPQ��
					//     -3:  PQ�ڵ㣬���Ƹõ��ѹVmin<=V<=Vmax��Խ�޺��ΪPV��
	float m_Pg;	//������й�������ֵ
	float m_Qg;	//�޹�
	//float m_V;	//ĸ�ߵ�ѹ��ֵ������ֵ
	float m_sita;	//ĸ�ߵ�ѹ����(��)
	float m_Qmax;	//������޹����ޣ�����ֵ
	float m_Qmin;	//������޹����ޣ�����ֵ
	float m_Pmax;	//������й����ޣ�����ֵ
	float m_Pmin;	//������й����ޣ�����ֵ

	int m_iPL;		//PAR_GROUP
	int m_iCB;		//Bus_ctrl ����ĸ����
	int m_iCL;		//LINE_CTRL ������·��
	float m_VQP;	//VALUE_CTRL ����������ֵ������ֵ
	int m_iKcd;		//0//���õ�ٷ��� K%
public://��д����
	virtual int ReadLine(char*Line);
	virtual void UpdateValue(NETWORKINFO*pNet);
	virtual void OutputPFOFile(FILE*fp, int nDirtn);//���������Ϣ
};

#endif

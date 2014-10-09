
#ifndef __PSASP_LOAD_H__
#define __PSASP_LOAD_H__

#include "../../DigAnalTE/DigAnalTE/NetWorkInfo/NetLoad.h"

class PSASPLOAD : public NETLOAD
{
public:
	PSASPLOAD()
	{
	}
public://����
	//int m_iMark;	//�������ݱ��
					//1��valid=.t.��0��valid=.f.
	int m_iBusNo;	//bus_name��L1���к�
	int m_iLoadNo;	//No_Load
	int m_iType;	//MODE_CTRL ��ĸ������
					//      0: ƽ��ڵ�
					//      1:  PQ�ڵ�
					//     -1:  PV�ڵ�
					//     -2:  PV�ڵ㣬���Ƹõ��޹�Qmin<=Q<=Qmax��Խ�޺��ΪPQ��
					//     -3:  PQ�ڵ㣬���Ƹõ��ѹVmin<=V<=Vmax��Խ�޺��ΪPV��
	float m_Pl;		//�����й����ʣ�����ֵ
	float m_Ql;		//�����޹����ʣ�����ֵ
	float m_V;		//ĸ�ߵ�ѹ��ֵ��pu
	float m_sita;	//ĸ�ߵ�ѹ���ǣ���
	float m_Qmax;	//�����޹�����
	float m_Qmin;	//�޹�����
	float m_Pmax;	//�����й�����
	float m_Pmin;	//�й�����
	int m_iPL;		//PAR_GROUP
	int m_iCB;		//Bus_ctrl ����ĸ����
	int m_iCL;		//LINE_CTRL ������·��
	float m_VQP;	//VALUE_CTRL ����������ֵ������ֵ
public://��д����
	virtual int ReadLine(char*Line);
	virtual void UpdateValue(NETWORKINFO*pNet);
	virtual void OutputPFOFile(FILE*fp, int nDirtn);//���������Ϣ
};

#endif

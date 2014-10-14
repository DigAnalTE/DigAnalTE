
#ifndef __PSASP_NETWORK_INFO_H__
#define __PSASP_NETWORK_INFO_H__

#include "../../DigAnalTE/DigAnalTE/NetWorkInfo/NetWorkInfo.h"
#include "PSASPBus.h"
#include "PSASPACLine.h"
#include "PSASPTrans.h"
#include "PSASPDCLine.h"
#include "PSASPGenerator.h"
#include "PSASPLoad.h"
#include "PSASPArea.h"

class PSASP_NETWORKINFO : public NETWORKINFO
{
public:
	PSASP_NETWORKINFO();
private:
	int m_iNBB;		//ĸ����Ŀ
	int m_iNLL;		//��������Ŀ
	int m_iNTT;		//��ѹ��������Ŀ
	int m_iNDC;		//ֱ������Ŀ
	int m_iNGG;		//�������Ŀ
	int m_iNLoad;	//������Ŀ
	int m_iNArea;	//������Ŀ
	int m_iNUD;		//CTRL_UD��1��t.��0��.f.��

	int m_iNEQ;		//1
	int m_iNSS;		//1
	int m_iNFACT;	//Ctrl_Fact
	float m_SB;		//��Ŀ��Ϣ�л�׼����
	float m_Vmax;	//����ֵ����ĸ����Ϣ��Vmax����ʱΪ��ֵ
	float m_Vmin;
	float m_Eps;	//TOLERANCE ���������������
	int m_iMeth;	//Method ���㷽��
	int m_iIter;	//Iteration �������ƴ���
	int m_iArea;	//Ctrl_Area 0-�������� 1-���������ʽ�������
	int m_iUD;		//Ctrl_ud �û��Զ��幦��
	int m_iMeq;		//EQ_Method 
	int m_iISS;		//Ctrl_sub
	int m_iNUP;		//UP_CALL
private://����
	int PSASPBusTotal;
	PSASPBUS *PSASPBus[_MaxBusNo];
	int PSASPACLineTotal;
	PSASPACLINE *PSASPACLine[_MaxBranchNo];
	int PSASPTransTotal;
	PSASPTRANS *PSASPTrans[_MaxBranchNo];
	int PSASPDCLineTotal;
	PSASPDCLINE *PSASPDCLine[_MaxBranchNo];
	int PSASPGenTotal;
	PSASPGENERATOR *PSASPGen[_MaxEquipNo];
	int PSASPLoadTotal;
	PSASPLOAD *PSASPLoad[_MaxEquipNo];
	int PSASPAreaTotal;
	PSASPAREA *PSASPArea[_MaxAreaNo];
public:
	char m_DataDir[_MAX_PATH];//�����ļ�Ŀ¼·��
	virtual int ReadFile(char*file);
	void OutputPSASPPFOFile();
private:
	int ReadL0File(char*file); //��LF.L0�ļ�
	int ReadL1File(char*file); //��LF.L1�ļ�
	int ReadL2File(char*file); //��LF.L2�ļ�
	int ReadL3File(char*file); //��LF.L3�ļ�
	int ReadL4File(char*file); //��LF.L4�ļ�
	int ReadL5File(char*file); //��LF.L5�ļ�
	int ReadL6File(char*file); //��LF.L6�ļ�
	int ReadL7File(char*file); //��LF.L7�ļ�
};

#endif

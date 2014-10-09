
#ifndef __PSASP_TRANS_H__
#define __PSASP_TRANS_H__

#include "../../DigAnalTE/DigAnalTE/NetWorkInfo/BranchBase.h"

class PSASPTRANS : public BRANCHBASE
{
public:
	PSASPTRANS()
	{
		c_BranchType = 'P';
		flag3W = -1;
	}
public://����
	int flag3W;//�������ѹ����ЧΪ�����������ѹ�������Ե�Ľڵ���ø�����ʾ
	//LF.L3�ļ���Ӧ���������ѹ����-Trans_2W(���Ϊ2W)��
	//�������ѹ����-Trans_3W(���Ϊ3W)��
	//�����ѹ����-SHIFT(���ΪSF)������
	int m_iMark;	//2W, SF��1��valid=.t.��0��valid=.f.
					//3W��{ Break_1, Break_2, Break_3}, 1��.t.��0��.f.
	int m_iIname;	//I_name��L1���к�
					//2W, SF��I_name
					//3W��Name_N
	int m_iJname;	//J_Name��L1���к�
					//2W, SF��J_name
					//3W��{Name_1, Name_2, Name_3}
	int m_iTranNo;	//J_Name��L1���к�
					//2W, SF��J_name
					//3W��{Name_1, Name_2, Name_3}
	//float m_R;	//2W, SF��R1
					//3W��{ R1, R2, R3}
	//float m_X;	//2W, SF��X1
					//3W��{ X1, X2, X3}
	float m_Tk;		//2W, SF��Tk ��ѹ�����
					//3W��{ Tk1, Tk2, Tk3}
	float m_Rm;		// Gm/(Gm* Gm+ Bm* Bm), ���Gm=Bm=0, Rm=0
					// 2W, SF��Gm
					// 3W��{ Gm, 0, 0}
	float m_Xm;		//���ŵ翹
					// Bm/(Gm* Gm+ Bm* Bm) , ���Gm=Bm=0, Xm=0
					// 2W, SF��Bm
					// 3W��{ Bm, 0, 0}
	int m_iFT;		//��ѹ��������־
					//0: �ñ�ѹ����Ĺ�I��ĸ������
					//n���ñ�ѹ����Ĺ�����n
					//-1:I��ĸ��Ϊ��������ĵ� (�ƺ����Ϊ��������)
					//-2: J��ĸ��Ϊ��������ĵ�
	int m_iType;	//��ѹ�����ƹ�������
					//2W, SF ��Mode_Ctrl
					//3W��ֻ��һ������ΪMode_Ctrl����Side_Ctrl��������������Ϊ0������PL, CB, CL, VQP����ˡ�
	int m_iPL;		// Par_No
	float m_TP;		//0
	int m_iCB;		//����ĸ���� 
					//2W��3W��Bus_Ctrl
					//SF��0
	int m_iCL;		//������·���
					//Line_Ctrl
	float m_VQP;	//����������ֵ
					//2W��3W��Value_Ctrl
					//SF��Pij_Ctrl
	float m_sita;	//���������
					//SF�� Ang_Shift
					//2W, 3W��0
	float m_TC;		//�ñ�ѹ������� (MVA)
					//2W, SF ��Rate_MVA
					//3W��{Rate_MVA1, Rate_MVA2, Rate_MVA3}
	float m_LIM;	//�ñ�ѹ���������޵İٷ����� �� ����=TC* LIM/100 
					//Up_Limit
	int m_iID;		//��1��ʼ��˳����
	int m_iJstar;	// J* =0
	int m_iTranType;//TrsType ��ѹ������
					//2W��1
					//SF��2
					//3W��3
public://�������
	float m_Pi, m_Qi, m_Pj, m_Qj, m_Qci, m_Qcj;
public://��д����
	virtual int ReadLine(char*Line);
	virtual void OutputPFOFile(FILE*fp, int nDirtn);//���������Ϣ
	virtual void subUpdateValue(class NETWORKINFO*pNet);
};

#endif

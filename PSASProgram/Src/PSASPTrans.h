
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
public://数据
	int flag3W;//三绕组变压器等效为三个两绕组变压器，中性点的节点号用负数表示
	//LF.L3文件对应与两绕组变压器表-Trans_2W(简记为2W)、
	//三绕组变压器表-Trans_3W(简记为3W)和
	//移相变压器表-SHIFT(简记为SF)三个表。
	int m_iMark;	//2W, SF：1：valid=.t.，0：valid=.f.
					//3W：{ Break_1, Break_2, Break_3}, 1：.t.，0：.f.
	int m_iIname;	//I_name在L1中行号
					//2W, SF：I_name
					//3W：Name_N
	int m_iJname;	//J_Name在L1中行号
					//2W, SF：J_name
					//3W：{Name_1, Name_2, Name_3}
	int m_iTranNo;	//J_Name在L1中行号
					//2W, SF：J_name
					//3W：{Name_1, Name_2, Name_3}
	//float m_R;	//2W, SF：R1
					//3W：{ R1, R2, R3}
	//float m_X;	//2W, SF：X1
					//3W：{ X1, X2, X3}
	float m_Tk;		//2W, SF：Tk 变压器变比
					//3W：{ Tk1, Tk2, Tk3}
	float m_Rm;		// Gm/(Gm* Gm+ Bm* Bm), 如果Gm=Bm=0, Rm=0
					// 2W, SF：Gm
					// 3W：{ Gm, 0, 0}
	float m_Xm;		//激磁电抗
					// Bm/(Gm* Gm+ Bm* Bm) , 如果Gm=Bm=0, Xm=0
					// 2W, SF：Bm
					// 3W：{ Bm, 0, 0}
	int m_iFT;		//变压器归属标志
					//0: 该变压器损耗归I侧母线区域
					//n：该变压器损耗归区域n
					//-1:I侧母线为三卷变中心点 (似乎编号为负数？？)
					//-2: J侧母线为三卷变中心点
	int m_iType;	//变压器控制功能类型
					//2W, SF ：Mode_Ctrl
					//3W：只能一个绕组为Mode_Ctrl，由Side_Ctrl决定，其他绕组为0。以下PL, CB, CL, VQP均如此。
	int m_iPL;		// Par_No
	float m_TP;		//0
	int m_iCB;		//被控母线名 
					//2W，3W：Bus_Ctrl
					//SF：0
	int m_iCL;		//被控线路编号
					//Line_Ctrl
	float m_VQP;	//被控量给定值
					//2W，3W：Value_Ctrl
					//SF：Pij_Ctrl
	float m_sita;	//移相器相角
					//SF： Ang_Shift
					//2W, 3W：0
	float m_TC;		//该变压器额定容量 (MVA)
					//2W, SF ：Rate_MVA
					//3W：{Rate_MVA1, Rate_MVA2, Rate_MVA3}
	float m_LIM;	//该变压器容量上限的百分数， 即 上限=TC* LIM/100 
					//Up_Limit
	int m_iID;		//从1开始，顺序编号
	int m_iJstar;	// J* =0
	int m_iTranType;//TrsType 变压器类型
					//2W：1
					//SF：2
					//3W：3
public://潮流结果
	float m_Pi, m_Qi, m_Pj, m_Qj, m_Qci, m_Qcj;
public://读写部分
	virtual int ReadLine(char*Line);
	virtual void OutputPFOFile(FILE*fp, int nDirtn);//输出潮流信息
	virtual void subUpdateValue(class NETWORKINFO*pNet);
};

#endif

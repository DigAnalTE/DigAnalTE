
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
	int m_iNBB;		//母线数目
	int m_iNLL;		//交流线数目
	int m_iNTT;		//变压器绕组数目
	int m_iNDC;		//直流线数目
	int m_iNGG;		//发电机数目
	int m_iNLoad;	//负荷数目
	int m_iNArea;	//区域数目
	int m_iNUD;		//CTRL_UD。1：t.；0：.f.。

	int m_iNEQ;		//1
	int m_iNSS;		//1
	int m_iNFACT;	//Ctrl_Fact
	float m_SB;		//项目信息中基准容量
	float m_Vmax;	//标幺值，当母线信息的Vmax不填时为此值
	float m_Vmin;
	float m_Eps;	//TOLERANCE 迭代计算允许误差
	int m_iMeth;	//Method 计算方法
	int m_iIter;	//Iteration 迭代限制次数
	int m_iArea;	//Ctrl_Area 0-正常潮流 1-进行区域功率交换控制
	int m_iUD;		//Ctrl_ud 用户自定义功能
	int m_iMeq;		//EQ_Method 
	int m_iISS;		//Ctrl_sub
	int m_iNUP;		//UP_CALL
private://数据
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
	char m_DataDir[_MAX_PATH];//数据文件目录路径
	virtual int ReadFile(char*file);
	void OutputPSASPPFOFile();
private:
	int ReadL0File(char*file); //读LF.L0文件
	int ReadL1File(char*file); //读LF.L1文件
	int ReadL2File(char*file); //读LF.L2文件
	int ReadL3File(char*file); //读LF.L3文件
	int ReadL4File(char*file); //读LF.L4文件
	int ReadL5File(char*file); //读LF.L5文件
	int ReadL6File(char*file); //读LF.L6文件
	int ReadL7File(char*file); //读LF.L7文件
};

#endif

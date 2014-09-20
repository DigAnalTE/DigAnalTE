
#ifndef __DigAnalTE_BRANCH_BASE_H__
#define __DigAnalTE_BRANCH_BASE_H__

//线路类，主要是交流设备：交流线和变压器
//程序中没有三卷变压器，必须等效为三个变压器
//直流等不被看做是线路，看做是设备，在设备类中管理

#include "../CommonFunction/constdef.h"

class BRANCHBASE
{
public:
	BRANCHBASE()
	{
		c_BranchType=' ';State=1;
		FromBusNo=-1;ToBusNo=-1;
		TK=1;Smax=0;
		m_pFBusV=NULL;m_pFBusSita=NULL;m_pTBusV=NULL;m_pTBusSita=NULL;
		IP=IQ=JP=JQ=0;
		Ploss=Qloss=0;
	}
public:
	char c_BranchType;//用于派生类的识别
public://线路信息
	char Name[_MaxNameLen];
	int State;//是否投入运行. =1:投入 其他:停运
	char BusName1[_MaxNameLen],BusName2[_MaxNameLen];
	int FromBusNo,ToBusNo;
public://线路的基本参数//注：全部为标么值
	float R,X,G,B,TK;//可以是变压器
	float Smax;//最大容量//标幺值
public:
	int iGetState(){return State;}
	void iSetState(int tState){State=tState;}
	int iGetFromBusNo(){return FromBusNo;}
	int iGetToBusNo(){return ToBusNo;}
	float fGetLineLoss(){return Ploss;}
public://读写部分
	virtual int ReadLine(char*Line);
	virtual void WriteLine(char*Line);
	void PrintInfo(char*Line)
	{//用于输出错误信息
		sprintf(Line,"%s--%s",BusName1,BusName2);
	}
	virtual void OutputPFOFile(FILE*fp,int nDirtn);//输出潮流信息//nDirtn: 1正向 2反向
public:
	virtual void VarientLink(class NETWORK_BASE*);

//--------------------潮流计算--------------------
public://潮流结果
	float IP,IQ,JP,JQ;//计算出来的两端潮流//实际值
	float Ploss,Qloss;
	float *m_pFBusV,*m_pFBusSita;//起始端//绑定母线电压//Update中使用了，在潮流计算过程中没有使用
	float *m_pTBusV,*m_pTBusSita;//终止端
public://潮流计算过程
	//第一步：修改其中的网络方程部分
	real Y11r,Y12r,Y21r,Y22r;//部分临时变量//subJacElement计算，然后JacElement加到NETWORKINFO中
	real Y11i,Y12i,Y21i,Y22i;
	virtual void subJacElement();
	virtual void JacElement(class NETWORKINFO*pNet);
	//第二步：在计算过程中在每步迭代过程中的调整
	virtual int BeforeIterCal(int iter){return 1;}//在每一次迭代前调用//返回值不为1，表示出错
	virtual int AfterIterCal(){return 0;}//在每一次迭代完成后调用，用来更改计算状态的//返回值非零表示状态修改
	//第三步：计算结束后的计算
	virtual void UpdateValue(class NETWORKINFO*pNet);
	virtual void SubUpdateValue(class NETWORKINFO*pNet){}//派生之后可以对其他的量赋值
};

#endif

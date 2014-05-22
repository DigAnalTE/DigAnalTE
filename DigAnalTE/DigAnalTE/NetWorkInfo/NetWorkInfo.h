
#ifndef __DigAnalTE_NETWORKINFO_NETWORKINFO_H__
#define __DigAnalTE_NETWORKINFO_NETWORKINFO_H__

// NETWORKINFO类是程序中最基础的类，所有的数据都使用该类进行管理，因此该类相当复杂
// 从NETWORK_BASE派生后，可以提供分析数据：导纳矩阵（可以做多种分析）、注入量（用于潮流计算）、孤岛信息
//
// 在使用该类时需要进行如下步骤：
// 1.定义对象:		NETWORKINFO gNetInfo;
// 2.读入数据
// 3.NetLink:		gNetInfo.NetLink
// 4.NetAnalysis:	gNetInfo.NetAnalysis
// 用户在逐步理解该类中的内容后可以进行更复杂的操作

#include "NetWorkBase.h"

class NETWORKINFO : public NETWORK_BASE
{
public:
	NETWORKINFO();
	virtual ~NETWORKINFO();
	virtual void FreeSpace();
	
/*-------------------------函数部分-------------------------*/
public://两个关键函数
	virtual void NetLink();
	virtual int NetAnalysis();

/*-------------------------基本分析-------------------------*/
public://矩阵部分——用于潮流计算
	//----计算类型-----
	//计算过程中类型不会随着VCtrl/Vmax/Vmin发生变化
	int *JacType;//雅克比类型。3:BS类型 2:PV类型 1:PQ类型 负数:删除
	//计算过程中的电压幅值
	real *BusVolt, *BusSita;
	//----注入部分----
	//注入部分被分为三部分：发电、负荷、等效//注：以母线注入方向为正方向，负荷一般为负数
	//注入有功：PPGen[i]+PPnet[i]+PInet[i]*V+PZnet[i]*V*V+PLcut[i]+PInst[i]
	//注入无功：QPGen[i]+QPnet[i]+QInet[i]*V+QZnet[i]*V*V+QLcut[i]+QInst[i]
	//1 发电部分//发电部分为发电机的注入，有最大最小值，可以进行调节
	real *PPGen,*QPGen;//标么值
	real *PGmax,*PGmin;
	real *QGmax,*QGmin;//涉及到控制，超过无功范围，则转换为PQ节点
		//Qmax Qmin是在0的基础上，最大最小值
		//实际提供给的无功为QPnet，无功范围为Qmax~Qmin
	real *VCtrl;
	real *Vmax,*Vmin;//涉及到控制，超过电压范围，则转换为PV节点，强制提供无功
	//2 负荷部分//负荷分为：恒功率、恒电流、恒阻抗三部分
	real *PPnet,*QPnet;
	real *PInet,*QInet;
	real *PZnet,*QZnet;
	real *PLcut,*QLcut;//切负荷部分//注意正负号
	float fGetPLoad(int i){return -(PPnet[i]+PInet[i]+PZnet[i]+PLcut[i]);}
	float fGetQLoad(int i){return -(QPnet[i]+QInet[i]+QZnet[i]+QLcut[i]);}
	//3 等效部分//其他元件等效为直流的注入功率部分
	real *PInst,*QInst;

	//----导纳矩阵----
	class COMPLEXSPAREMATRIX *m_Matrix;
	void BuildMatrix();
public://函数部分
	int bNetInfoCode;//是否建立了矩阵结构
	void GenerateNetInfo();//生成矩阵的结构，可以反复进行
	void Update();//相当于反方向的GenerateNetInfo()//在计算完潮流的电压和幅值后调用该函数可以更新所有信息
	void UpdateVxVy(real*Vx,real*Vy)
	{
		int i;
		real EK,FK;
		for (i=0;i<iGetBusTotal();i++)
		{
			EK=Vx[i];
			FK=Vy[i];
			BusVolt[i]=(float)(sqrt(EK*EK+FK*FK));
			BusSita[i]=(float)(57.2957795*atan2(FK,EK));
		}
		Update();
	}
public://函数修改部分//在未了解一下函数功能的情况下，请勿使用
	void NodeTypeModify(int busNo,int JacType);
	void NodeGenModify(int busNo,real Pgen,real Qgen,real Pmax,real Pmin,real Qmax,real Qmin);//当有多组限制值时：Qmax Qmin取和
	void NodeVmaxModify(int busNo,real Vmax,real Vmin);//当有多组限制值时：Vmax取最小 Vmin取最大
	void NodeVCtrlModify(int busNo,real Vctrl);
	void NodeLoadModify(int busNo,real PPNET,real QPNET,real PINET,real QINET,real PZNET,real QZNET);
	void NodeInsertPower(int busNo,real pinsert,real qinsert);
	void NodeLoadCut(int busNo,real plcut,real qlcut);
	void BranchGBModify(int IBusNo,int JBusNo,real GII,real BII,real GJJ,real BJJ,real GIJ,real BIJ,real GJI,real BJI);
	void DampPQTable(FILE*fp);
	int BeforeIterCal(int iter);

/*-------------------------PART3--总结信息-------------------------*/
public:
	float PMaxTotal,PGenTotal,PLoadTotal;//有名值
	float PLossTotal;
public://最后利用JacType进行平衡节点分析：!=1: 若平衡节点不正确
	void SumInfo(int isProcess=0);//进行分析，分析结果在NetError中//isProcess==1时对其进行处理，将不能计算的孤岛跳过
};

#endif

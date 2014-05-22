
#ifndef __DigAnalTE_NETWORKINFO_NETWORKINFO_H__
#define __DigAnalTE_NETWORKINFO_NETWORKINFO_H__

// NETWORKINFO���ǳ�������������࣬���е����ݶ�ʹ�ø�����й�����˸����൱����
// ��NETWORK_BASE�����󣬿����ṩ�������ݣ����ɾ��󣨿��������ַ�������ע���������ڳ������㣩���µ���Ϣ
//
// ��ʹ�ø���ʱ��Ҫ�������²��裺
// 1.�������:		NETWORKINFO gNetInfo;
// 2.��������
// 3.NetLink:		gNetInfo.NetLink
// 4.NetAnalysis:	gNetInfo.NetAnalysis
// �û������������е����ݺ���Խ��и����ӵĲ���

#include "NetWorkBase.h"

class NETWORKINFO : public NETWORK_BASE
{
public:
	NETWORKINFO();
	virtual ~NETWORKINFO();
	virtual void FreeSpace();
	
/*-------------------------��������-------------------------*/
public://�����ؼ�����
	virtual void NetLink();
	virtual int NetAnalysis();

/*-------------------------��������-------------------------*/
public://���󲿷֡������ڳ�������
	//----��������-----
	//������������Ͳ�������VCtrl/Vmax/Vmin�����仯
	int *JacType;//�ſ˱����͡�3:BS���� 2:PV���� 1:PQ���� ����:ɾ��
	//��������еĵ�ѹ��ֵ
	real *BusVolt, *BusSita;
	//----ע�벿��----
	//ע�벿�ֱ���Ϊ�����֣����硢���ɡ���Ч//ע����ĸ��ע�뷽��Ϊ�����򣬸���һ��Ϊ����
	//ע���й���PPGen[i]+PPnet[i]+PInet[i]*V+PZnet[i]*V*V+PLcut[i]+PInst[i]
	//ע���޹���QPGen[i]+QPnet[i]+QInet[i]*V+QZnet[i]*V*V+QLcut[i]+QInst[i]
	//1 ���粿��//���粿��Ϊ�������ע�룬�������Сֵ�����Խ��е���
	real *PPGen,*QPGen;//��ôֵ
	real *PGmax,*PGmin;
	real *QGmax,*QGmin;//�漰�����ƣ������޹���Χ����ת��ΪPQ�ڵ�
		//Qmax Qmin����0�Ļ����ϣ������Сֵ
		//ʵ���ṩ�����޹�ΪQPnet���޹���ΧΪQmax~Qmin
	real *VCtrl;
	real *Vmax,*Vmin;//�漰�����ƣ�������ѹ��Χ����ת��ΪPV�ڵ㣬ǿ���ṩ�޹�
	//2 ���ɲ���//���ɷ�Ϊ���㹦�ʡ�����������迹������
	real *PPnet,*QPnet;
	real *PInet,*QInet;
	real *PZnet,*QZnet;
	real *PLcut,*QLcut;//�и��ɲ���//ע��������
	float fGetPLoad(int i){return -(PPnet[i]+PInet[i]+PZnet[i]+PLcut[i]);}
	float fGetQLoad(int i){return -(QPnet[i]+QInet[i]+QZnet[i]+QLcut[i]);}
	//3 ��Ч����//����Ԫ����ЧΪֱ����ע�빦�ʲ���
	real *PInst,*QInst;

	//----���ɾ���----
	class COMPLEXSPAREMATRIX *m_Matrix;
	void BuildMatrix();
public://��������
	int bNetInfoCode;//�Ƿ����˾���ṹ
	void GenerateNetInfo();//���ɾ���Ľṹ�����Է�������
	void Update();//�൱�ڷ������GenerateNetInfo()//�ڼ����곱���ĵ�ѹ�ͷ�ֵ����øú������Ը���������Ϣ
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
public://�����޸Ĳ���//��δ�˽�һ�º������ܵ�����£�����ʹ��
	void NodeTypeModify(int busNo,int JacType);
	void NodeGenModify(int busNo,real Pgen,real Qgen,real Pmax,real Pmin,real Qmax,real Qmin);//���ж�������ֵʱ��Qmax Qminȡ��
	void NodeVmaxModify(int busNo,real Vmax,real Vmin);//���ж�������ֵʱ��Vmaxȡ��С Vminȡ���
	void NodeVCtrlModify(int busNo,real Vctrl);
	void NodeLoadModify(int busNo,real PPNET,real QPNET,real PINET,real QINET,real PZNET,real QZNET);
	void NodeInsertPower(int busNo,real pinsert,real qinsert);
	void NodeLoadCut(int busNo,real plcut,real qlcut);
	void BranchGBModify(int IBusNo,int JBusNo,real GII,real BII,real GJJ,real BJJ,real GIJ,real BIJ,real GJI,real BJI);
	void DampPQTable(FILE*fp);
	int BeforeIterCal(int iter);

/*-------------------------PART3--�ܽ���Ϣ-------------------------*/
public:
	float PMaxTotal,PGenTotal,PLoadTotal;//����ֵ
	float PLossTotal;
public://�������JacType����ƽ��ڵ������!=1: ��ƽ��ڵ㲻��ȷ
	void SumInfo(int isProcess=0);//���з��������������NetError��//isProcess==1ʱ������д��������ܼ���Ĺµ�����
};

#endif

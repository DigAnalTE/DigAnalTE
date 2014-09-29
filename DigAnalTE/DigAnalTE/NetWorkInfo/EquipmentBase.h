
#ifndef __DigAnalTE_EQUIPMENT_BASE_H__
#define __DigAnalTE_EQUIPMENT_BASE_H__

//EQUIPMENT�����豸�࣬��ӵ�һ����
//����ཫ�����豸ȫ��������һ�𣬰����˵��˿ڡ�˫�˿ڡ���˿�
//
//��Ȼ����ʹ�������ǵ��˿�Ԫ�������ǣ�Ϊ���Ժ����չ����������Ҳȫ�����������
//����ֱ����˫�˿��豸
//ע��Ŀǰֻ�е��˿�˫�˿��豸���й���ϸ���ԣ���˿��豸Ŀǰû��ʹ��

#include "../CommonFunction/constdef.h"

class EQUIPMENTBASE
{
public:
	EQUIPMENTBASE(){ c_EquipType = ' '; }
protected:
	int EquipPortType;//���ͣ��˿�����������ǽ�ֹ�ⲿ�����޸ĵ�//�����û���Ҫʹ��
	//1 ONEEQUIPMENTBASE
	//2 TWOEQUIPMENTBASE
	//3 MLTEQUIPMENTBASE
public:
	bool IsOneEquipment(){ return EquipPortType == 1; }
	bool IsTwoEquipment(){ return EquipPortType == 2; }
	bool IsMltEquipment(){ return EquipPortType == 3; }
public:
	char c_EquipType;//�����������ʶ��
public:
	char Name[_MaxNameLen];//Ϊ�豸����ʶ�������ΪԪ��ʶ���Ψһ��ʾ
	int State;//�Ƿ�Ͷ������. =1:Ͷ�� =0:ͣ��
	int iGetState(){ return State; }
	void iSetState(int tState){ State = tState; }
public://��д����
	virtual int ReadLine(char*Line) = 0;
	virtual void WriteLine(char*Line) = 0;
	virtual void PrintInfo(char*Line) = 0;
	virtual void OutputPFOFile(FILE*fp, int nDirtn){}//���������Ϣ
public:
	virtual void VarientLink(class NETWORK_BASE*) = 0;

public://�������㲿��
	virtual void JacElement(class NETWORKINFO*pNet){}
	virtual int BeforeIterCal(int iter){ return 1; }//��ÿһ�ε���ǰ����
	virtual int AfterIterCal(){ return 0; }//��ÿһ�ε�����ɺ���ã��������ļ���״̬��//����ֵ�����ʾ״̬�޸�
	virtual void UpdateValue(class NETWORKINFO*pNet){}
};

class ONEEQUIPMENTBASE : public EQUIPMENTBASE
{//���˿��豸�Ļ���
public:
	ONEEQUIPMENTBASE()
	{
		State = 1; EquipPortType = 1; c_EquipType = '1';
		BusNo = -1; pBus = NULL;
		PPNET = QPNET = PINET = QINET = PZNET = QZNET = 0;
		PGNET = 0; QGNET = 0; PGMAX = 0; PGMIN = 0; QGMAX = 0; QGMIN = 0;
	}
	~ONEEQUIPMENTBASE(){}
public:
	char BusName[_MaxNameLen];
	int BusNo;
	char Owner[_MaxNameLen];
	class BUSBASE* pBus;
	virtual void VarientLink(class NETWORK_BASE*);
public:
	int JacType;
	real PGNET, QGNET, PGMAX, PGMIN, QGMAX, QGMIN;
	real PPNET, QPNET;//�㹦��
	real PINET, QINET;//�����
	real PZNET, QZNET;//���迹
	real PInst, QInst;
	virtual void subJacElement(class NETWORKINFO*pNet);
	virtual void JacElement(class NETWORKINFO*pNet);
public://��д����
	virtual void PrintInfo(char*Line);
};

class TWOEQUIPMENTBASE : public EQUIPMENTBASE
{//���˿���ϸ��ģԪ���Ļ���
public:
	TWOEQUIPMENTBASE()
	{
		State = 1; EquipPortType = 2; c_EquipType = '2';
		BusNo1 = -1; BusNo2 = -1;
		pBus1 = NULL; pBus2 = NULL;
		PGNET1 = 0; QGNET1 = 0; PGMAX1 = 0; PGMIN1 = 0; QGMAX1 = 0; QGMIN1 = 0;
		PPNET1 = QPNET1 = PINET1 = QINET1 = PZNET1 = QZNET1 = 0;
		PGNET2 = 0; QGNET2 = 0; PGMAX2 = 0; PGMIN2 = 0; QGMAX2 = 0; QGMIN2 = 0;
		PPNET2 = QPNET2 = PINET2 = QINET2 = PZNET2 = QZNET2 = 0;
	}
	~TWOEQUIPMENTBASE(){}
public:
	char BusName1[_MaxNameLen];
	int BusNo1;
	char BusName2[_MaxNameLen];
	int BusNo2;
	BUSBASE* pBus1;
	BUSBASE* pBus2;
public:
	int JacType1;
	real PGNET1, QGNET1, PGMAX1, PGMIN1, QGMAX1, QGMIN1;
	real PPNET1, QPNET1;//�㹦��
	real PINET1, QINET1;//�����
	real PZNET1, QZNET1;//���迹
	real PInst1, QInst1;
	int JacType2;
	real PGNET2, QGNET2, PGMAX2, PGMIN2, QGMAX2, QGMIN2;
	real PPNET2, QPNET2;//�㹦��
	real PINET2, QINET2;//�����
	real PZNET2, QZNET2;//���迹
	real PInst2, QInst2;
public:
	virtual void VarientLink(class NETWORK_BASE*);
	virtual void subJacElement(class NETWORKINFO*pNet);
	virtual void JacElement(class NETWORKINFO*pNet);
public://��д����
	virtual void PrintInfo(char*Line);
};

class MLTEQUIPMENTBASE : public EQUIPMENTBASE
{//��˿���ϸ��ģԪ���Ļ���
public:
	MLTEQUIPMENTBASE(){ State = 1; EquipPortType = 3; c_EquipType = '3'; BusName = NULL; PortNumber = 0; }
	~MLTEQUIPMENTBASE(){ FreeSpace(); }
public:
	int PortNumber;//�˿���
	char(*BusName)[_MaxNameLen];
	int *BusNo;
public:
	virtual void VarientLink(class NETWORK_BASE*);
	void Malloc()
	{
		MallocNew2D(BusName, char, PortNumber, _MaxNameLen);
	}
	void Malloc(int tPort)
	{
		PortNumber = tPort;
		MallocNew2D(BusName, char, PortNumber, _MaxNameLen);
	}
	void FreeSpace(){ FreeArray2D(BusName); }
	virtual void JacElement(class NETWORKINFO*pNet);
public://��д����
	virtual void PrintInfo(char*Line);
};

#endif
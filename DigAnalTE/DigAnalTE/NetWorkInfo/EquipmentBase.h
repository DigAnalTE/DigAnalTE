
#ifndef __DigAnalTE_EQUIPMENT_BASE_H__
#define __DigAnalTE_EQUIPMENT_BASE_H__

//EQUIPMENT类是设备类，最复杂的一个类
//这个类将所有设备全部囊括在一起，包括了单端口、双端口、多端口
//
//虽然里面使用最多的是单端口元件，但是，为了以后的扩展，将其他的也全部进行了设计
//两端直流是双端口设备
//注：目前只有单端口双端口设备进行过详细测试；多端口设备目前没有使用

#include "../CommonFunction/constdef.h"

class EQUIPMENTBASE
{
public:
	EQUIPMENTBASE(){ c_EquipType = ' '; }
protected:
	int EquipPortType;//类型：端口数量。这个是禁止外部函数修改的//建议用户不要使用
	//1 ONEEQUIPMENTBASE
	//2 TWOEQUIPMENTBASE
	//3 MLTEQUIPMENTBASE
public:
	bool IsOneEquipment(){ return EquipPortType == 1; }
	bool IsTwoEquipment(){ return EquipPortType == 2; }
	bool IsMltEquipment(){ return EquipPortType == 3; }
public:
	char c_EquipType;//用于派生类的识别
public:
	char Name[_MaxNameLen];//为设备增加识别符，作为元件识别的唯一标示
	int State;//是否投入运行. =1:投入 =0:停运
	int iGetState(){ return State; }
	void iSetState(int tState){ State = tState; }
public://读写部分
	virtual int ReadLine(char*Line) = 0;
	virtual void WriteLine(char*Line) = 0;
	virtual void PrintInfo(char*Line) = 0;
	virtual void OutputPFOFile(FILE*fp, int nDirtn){}//输出潮流信息
public:
	virtual void VarientLink(class NETWORK_BASE*) = 0;

public://潮流计算部分
	virtual void JacElement(class NETWORKINFO*pNet){}
	virtual int BeforeIterCal(int iter){ return 1; }//在每一次迭代前调用
	virtual int AfterIterCal(){ return 0; }//在每一次迭代完成后调用，用来更改计算状态的//返回值非零表示状态修改
	virtual void UpdateValue(class NETWORKINFO*pNet){}
};

class ONEEQUIPMENTBASE : public EQUIPMENTBASE
{//单端口设备的基类
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
	real PPNET, QPNET;//恒功率
	real PINET, QINET;//恒电流
	real PZNET, QZNET;//恒阻抗
	real PInst, QInst;
	virtual void subJacElement(class NETWORKINFO*pNet);
	virtual void JacElement(class NETWORKINFO*pNet);
public://读写部分
	virtual void PrintInfo(char*Line);
};

class TWOEQUIPMENTBASE : public EQUIPMENTBASE
{//两端口详细建模元件的基类
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
	real PPNET1, QPNET1;//恒功率
	real PINET1, QINET1;//恒电流
	real PZNET1, QZNET1;//恒阻抗
	real PInst1, QInst1;
	int JacType2;
	real PGNET2, QGNET2, PGMAX2, PGMIN2, QGMAX2, QGMIN2;
	real PPNET2, QPNET2;//恒功率
	real PINET2, QINET2;//恒电流
	real PZNET2, QZNET2;//恒阻抗
	real PInst2, QInst2;
public:
	virtual void VarientLink(class NETWORK_BASE*);
	virtual void subJacElement(class NETWORKINFO*pNet);
	virtual void JacElement(class NETWORKINFO*pNet);
public://读写部分
	virtual void PrintInfo(char*Line);
};

class MLTEQUIPMENTBASE : public EQUIPMENTBASE
{//多端口详细建模元件的基类
public:
	MLTEQUIPMENTBASE(){ State = 1; EquipPortType = 3; c_EquipType = '3'; BusName = NULL; PortNumber = 0; }
	~MLTEQUIPMENTBASE(){ FreeSpace(); }
public:
	int PortNumber;//端口数
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
public://读写部分
	virtual void PrintInfo(char*Line);
};

#endif

#ifndef __DigAnalTE_AREAINFO_H__
#define __DigAnalTE_AREAINFO_H__

//区域类，管理Area和Zone
//Zone属于Area
//Area类中可以包含控制

#include "../CommonFunction/constdef.h"
#include "../CommonFunction/HashTable.h"

struct AREABASE
{//Area
	char AreaName[_MaxNameLen];

	int ZoneTotal;
	char ZoneName[_MaxAreaZoneNo][_MaxNameLen];
	int ZoneNo[_MaxAreaZoneNo];

	//控制部分//目前该功能还未启用
	char SlackBusName[_MaxNameLen];//内部平衡节点
	int AreaSlackBusNo;

	int ControlFlag;//控制标识位，=0时控制取消
	float AreaPower;//外送功率//当ControlFlag=1时，对其进行控制//当ControlFlag=0时，表示信息
};

class AREAINFO
{
public:
	AREAINFO();
	virtual ~AREAINFO(){ FreeSpace(); }
	virtual void FreeSpace();
	//--------------------基本信息--------------------
protected://Zone信息
	int ZoneTotal;
	char ZoneName[_MaxZoneNo][_MaxNameLen];
	int ZoneAreaNo[_MaxZoneNo];
protected://Area的数目比较少，不必使用检索算法
	int AreaTotal;
	AREABASE *Area[_MaxAreaNo];
public:
	int iGetZoneTotal(){ return ZoneTotal; }
	int iGetAreaTotal(){ return AreaTotal; }
	AREABASE *cpGetArea(int i){ return Area[i]; }
	int AddNewArea(AREABASE*tArea);
public://读写部分
	virtual int ReadLine(char*Line);
	virtual void WriteLine(int, char*Line);
	//--------------------索引部分--------------------
protected:
	HASHTABLE m_ZoneHash;//使用Hash表法管理Zone
public:
	virtual void BuildLink(class NETWORK_BASE* tNet);//建立索引
	int iGetZoneNo(char* tZone);
	int iGetAreaNo(char* tArea);
};


#endif

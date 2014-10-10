
#ifndef __DigAnalTE_EQUIPMENT_INFO_H__
#define __DigAnalTE_EQUIPMENT_INFO_H__

//设备信息类的管理

#include "EquipmentBase.h"
#include "../CommonFunction/HashTable.h"

class EQUIPMENTINFO
{
public:
	EQUIPMENTINFO();
	virtual ~EQUIPMENTINFO(){ FreeSpace(); }
	virtual void FreeSpace();
	//--------------------基本信息--------------------
protected:
	int EquipmentTotal;
	EQUIPMENTBASE *Equipment[_MaxEquipNo];
public:
	int iGetEquipTotal(){ return EquipmentTotal; }
	EQUIPMENTBASE**GetEquipment(){ return Equipment; }
	int AddNewEquip(EQUIPMENTBASE *tEquipment);
	class GENERATOR* AddNewGeneratorEquip();
	//--------------------索引信息--------------------
public:
	struct EQUIPLINK
	{
		int IBusNo;
		int Type;//端口数
		int EquipNo;
		int nDirtn;
	};
public://索引
	int BusTotal;
	int EquipLinkNo;
	EQUIPLINK *EquipLink;
	int *EQINDX, *EQCONT;
public:
	void BuildLink(int BusTotal);

protected://索引
	HASHTABLE m_EquipHash;//使用Hash表法管理设备
public:
	int EquipSearch(char *lpszString);
};

#endif
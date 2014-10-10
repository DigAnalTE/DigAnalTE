
#ifndef __DigAnalTE_EQUIPMENT_INFO_H__
#define __DigAnalTE_EQUIPMENT_INFO_H__

//�豸��Ϣ��Ĺ���

#include "EquipmentBase.h"
#include "../CommonFunction/HashTable.h"

class EQUIPMENTINFO
{
public:
	EQUIPMENTINFO();
	virtual ~EQUIPMENTINFO(){ FreeSpace(); }
	virtual void FreeSpace();
	//--------------------������Ϣ--------------------
protected:
	int EquipmentTotal;
	EQUIPMENTBASE *Equipment[_MaxEquipNo];
public:
	int iGetEquipTotal(){ return EquipmentTotal; }
	EQUIPMENTBASE**GetEquipment(){ return Equipment; }
	int AddNewEquip(EQUIPMENTBASE *tEquipment);
	class GENERATOR* AddNewGeneratorEquip();
	//--------------------������Ϣ--------------------
public:
	struct EQUIPLINK
	{
		int IBusNo;
		int Type;//�˿���
		int EquipNo;
		int nDirtn;
	};
public://����
	int BusTotal;
	int EquipLinkNo;
	EQUIPLINK *EquipLink;
	int *EQINDX, *EQCONT;
public:
	void BuildLink(int BusTotal);

protected://����
	HASHTABLE m_EquipHash;//ʹ��Hash�������豸
public:
	int EquipSearch(char *lpszString);
};

#endif
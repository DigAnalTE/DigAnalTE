
#ifndef __DigAnalTE_AREAINFO_H__
#define __DigAnalTE_AREAINFO_H__

//�����࣬����Area��Zone
//Zone����Area
//Area���п��԰�������

#include "../CommonFunction/constdef.h"
#include "../CommonFunction/HashTable.h"

struct AREABASE
{//Area
	char AreaName[_MaxNameLen];

	int ZoneTotal;
	char ZoneName[_MaxAreaZoneNo][_MaxNameLen];
	int ZoneNo[_MaxAreaZoneNo];

	//���Ʋ���//Ŀǰ�ù��ܻ�δ����
	char SlackBusName[_MaxNameLen];//�ڲ�ƽ��ڵ�
	int AreaSlackBusNo;

	int ControlFlag;//���Ʊ�ʶλ��=0ʱ����ȡ��
	float AreaPower;//���͹���//��ControlFlag=1ʱ��������п���//��ControlFlag=0ʱ����ʾ��Ϣ
};

class AREAINFO
{
public:
	AREAINFO();
	virtual ~AREAINFO(){ FreeSpace(); }
	virtual void FreeSpace();
	//--------------------������Ϣ--------------------
protected://Zone��Ϣ
	int ZoneTotal;
	char ZoneName[_MaxZoneNo][_MaxNameLen];
	int ZoneAreaNo[_MaxZoneNo];
protected://Area����Ŀ�Ƚ��٣�����ʹ�ü����㷨
	int AreaTotal;
	AREABASE *Area[_MaxAreaNo];
public:
	int iGetZoneTotal(){ return ZoneTotal; }
	int iGetAreaTotal(){ return AreaTotal; }
	AREABASE *cpGetArea(int i){ return Area[i]; }
	int AddNewArea(AREABASE*tArea);
public://��д����
	virtual int ReadLine(char*Line);
	virtual void WriteLine(int, char*Line);
	//--------------------��������--------------------
protected:
	HASHTABLE m_ZoneHash;//ʹ��Hash������Zone
public:
	virtual void BuildLink(class NETWORK_BASE* tNet);//��������
	int iGetZoneNo(char* tZone);
	int iGetAreaNo(char* tArea);
};


#endif

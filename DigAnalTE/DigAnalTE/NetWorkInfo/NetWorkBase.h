
#ifndef __DigAnalTE_NETWORKINFO_NETWORK_BASE_H__
#define __DigAnalTE_NETWORKINFO_NETWORK_BASE_H__

// NETWORKBASE���ǳ�������������࣬���е����ݶ�ʹ�ø�����й���
// ��������������ݵ����ݽӿڡ����ҡ����򡢹µ����������������
//
// Ϊ�˷����û�ʹ�ã��û���Ҫ��������������������
// 1.	��д���ݺ������ӿں���
// 2.	����������NetLink���������е����ӡ������Ϳռ䡣�ڲ�����Ԫ���Ļ����ϣ��ú���ֻ��Ҫ����һ�μ��ɡ���ε���ʱ����ģ�ֻ�ǻ�Ӱ���ٶ�
// 3.	����������NetAnalysis������Ԫ���ĵ�ǰ״̬�����·������ݡ���Ԫ��״̬�����仯����Ҫ���µ��øú���
//
// ��������������Ҫ���عؼ�������NetLink��NetAnalysis

#include "AreaInfo.h"
#include "BusInfo.h"
#include "BranchInfo.h"
#include "EquipmentInfo.h"

class NETWORK_BASE
{//���ݲ��֣����������֣����ݲ��֡����ݽӿں�����������������
public:
	NETWORK_BASE();
	~NETWORK_BASE();
	virtual void FreeSpace();
public:
	int m_ChangeCode;		//�ǳ���Ҫ��һ����־��������������ݷ����仯����ı�ñ���
public:
	int NetError;
	int bIsLink;			//==1��ʾ�ɹ�����������ϵ
	virtual void NetLink();	//����������ϵ
public://�����µ����
	virtual int NetAnalysis();

	/*-------------------------PART-1--���ݲ���-------------------------*/
	//
	//	������Ϣ
	//
protected:
	char  Description[_MaxNameLen];
	float BMVA;
	float FreqSys;
public:
	float GetBMVA(){ return BMVA; }
	float *pGetBMVA(){ return &BMVA; }
	void SetBMVA(float tBase){ BMVA = tBase; }
	float GetFreqSys(){ return FreqSys; }
	void SetFreqSys(float tFreq){ FreqSys = tFreq; }
	//
	//	������Ϣ
	//
protected:
	AREAINFO m_AreaInfo;
public:
	int iGetAreaTotal(){ return m_AreaInfo.iGetAreaTotal(); }
	int iGetZoneTotal(){ return m_AreaInfo.iGetZoneTotal(); }
	AREAINFO* cpGetAreaInfo(){ return &m_AreaInfo; }
	AREABASE* cpGetArea(int i){ return m_AreaInfo.cpGetArea(i); }
	int iGetZoneNo(char* tZone){ return m_AreaInfo.iGetZoneNo(tZone); }
	int iGetAreaNo(char* tArea){ return m_AreaInfo.iGetAreaNo(tArea); }

protected://ĸ����Ϣ
	BUSINFO m_BusInfo;
public:
	int iGetBusTotal(){ return m_BusInfo.iGetBusTotal(); }
	BUSINFO* cpGetBusInfo(){ return &m_BusInfo; }
	BUSBASE* cpGetBus(int i){ return m_BusInfo.cpGetBus(i); }
	int BusSearch(char *lpszString){ return m_BusInfo.BusSearch(lpszString); }

protected://��·��Ϣ
	BRANCHINFO m_BranchInfo;
public:
	int iGetBranchTotal(){ return m_BranchInfo.iGetBranchTotal(); }
	BRANCHINFO* cpGetBranchInfo(){ return &m_BranchInfo; }
	BRANCHBASE* cpGetBranch(int i){ return m_BranchInfo.cpGetBranch(i); }
	int BranchSearch(char *lpszString){ return m_BranchInfo.BranchSearch(lpszString); }

protected://�豸��Ϣ
	//���豸�����ж�Ӧ��ĸ�ߣ����û�и�ĸ�ߣ�����BusInfo�н����µ�ĸ��
	EQUIPMENTINFO m_EquipmentInfo;//�������ָ������豸�����˿ڣ����˿ڣ���˿�
public:
	int iGetEquipTotal(){ return m_EquipmentInfo.iGetEquipTotal(); }
	EQUIPMENTINFO* cpGetEquipInfo(){ return &m_EquipmentInfo; }
	EQUIPMENTBASE* cpGetEquip(int i){ return m_EquipmentInfo.GetEquipment()[i]; }
	ONEEQUIPMENTBASE* cpGetOneEquip(int i)
	{
		if (!m_EquipmentInfo.GetEquipment()[i]->IsOneEquipment())
			return NULL;
		return (ONEEQUIPMENTBASE*)(m_EquipmentInfo.GetEquipment()[i]);
	}

	/*-------------------------PART-2--���ݽӿ�-------------------------*/
public://��ͨ�ӿ�
	int ReadFile(char*file);
	void WriteFile(char*file);
	void WriteFile(FILE*fp);
	void OutputPFOFile(FILE*fp);//���������Ϣ
};

#endif

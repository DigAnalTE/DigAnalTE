
#ifndef __DigAnalTE_BRANCHINFO_H__
#define __DigAnalTE_BRANCHINFO_H__

//��·��Ϣ��Ĺ���
//��Ҫ�Ǹ������

#include "BranchBase.h"
#include "../CommonFunction/HashTable.h"

class BRANCHINFO
{
public:
	BRANCHINFO();
	virtual ~BRANCHINFO(){FreeSpace();}
	virtual void FreeSpace();
//--------------------������Ϣ--------------------
private:
	int BranchTotal;
	BRANCHBASE *Branch[_MaxBranchNo];
public:
	int iGetBranchTotal(){return BranchTotal;}
	BRANCHBASE *cpGetBranch(int i){return Branch[i];}
	int AddNewBranch(BRANCHBASE*tBranch);
//--------------------HASH����--------------------
protected://����//����ҪBuild����������ӵĹ������Զ����
	HASHTABLE m_BranchHash;//ʹ��Hash������Branch
public:
	int BranchSearch( char *lpszString );

//--------------------��������--------------------
public:
	struct BRANCHLINK
	{
		int IBusOldNo;
		int JBusOldNo;
		int BrnhNo;
		int nDirtn;//1���� 2����
	};
public://��ĸ�ߵ���·������//�ⲿ����Ϣ����Ҫ�������ط�����Ҳ���õ�
	int IsBuild;
	int BusTotal;
	int BranchLinkNo;
	BRANCHLINK *BranchLink;
	int *BRINDX,*BRCONT;
	void BuildLink(int BusTotal);
};

#endif

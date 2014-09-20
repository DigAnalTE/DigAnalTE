
#ifndef __DigAnalTE_BRANCHINFO_H__
#define __DigAnalTE_BRANCHINFO_H__

//线路信息类的管理
//主要是负责查找

#include "BranchBase.h"
#include "../CommonFunction/HashTable.h"

class BRANCHINFO
{
public:
	BRANCHINFO();
	virtual ~BRANCHINFO(){FreeSpace();}
	virtual void FreeSpace();
//--------------------基本信息--------------------
private:
	int BranchTotal;
	BRANCHBASE *Branch[_MaxBranchNo];
public:
	int iGetBranchTotal(){return BranchTotal;}
	BRANCHBASE *cpGetBranch(int i){return Branch[i];}
	int AddNewBranch(BRANCHBASE*tBranch);
//--------------------HASH索引--------------------
protected://索引//不需要Build索引，在添加的过程中自动添加
	HASHTABLE m_BranchHash;//使用Hash表法管理Branch
public:
	int BranchSearch( char *lpszString );

//--------------------两端索引--------------------
public:
	struct BRANCHLINK
	{
		int IBusOldNo;
		int JBusOldNo;
		int BrnhNo;
		int nDirtn;//1正向 2反向
	};
public://从母线到线路的索引//这部分信息很重要，其他地方可能也会用到
	int IsBuild;
	int BusTotal;
	int BranchLinkNo;
	BRANCHLINK *BranchLink;
	int *BRINDX,*BRCONT;
	void BuildLink(int BusTotal);
};

#endif

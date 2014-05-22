
#ifndef __DigAnalTE_BRANCHINFO_H__
#define __DigAnalTE_BRANCHINFO_H__

//��·��Ϣ��Ĺ���
//��Ҫ�Ǹ������

#include "BranchBase.h"

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
//--------------------������Ϣ--------------------
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

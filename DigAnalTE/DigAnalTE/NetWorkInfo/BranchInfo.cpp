
#include "BranchInfo.h"
#include "../CommonFunction/ErrorInfo.h"
#include "../CommonFunction/QikSort.h"
#include "NetWorkInfo.h"

BRANCHINFO::BRANCHINFO()
{
	BranchTotal=0;
	memset(Branch,0,_MaxBranchNo*sizeof(BRANCHBASE*));
	IsBuild=0;
	BranchLinkNo=0;
	BranchLink=NULL;BRINDX=NULL;BRCONT=NULL;
	m_BranchHash.InitHashTable(_MaxBranchNo);
}

void BRANCHINFO::FreeSpace()
{
	int i;
	for (i=0;i<_MaxBranchNo;i++)
	{
		FreeArray(Branch[i]);
	}
	BranchTotal=0;
	BranchLinkNo=0;FreeArray(BranchLink);FreeArray(BRINDX);FreeArray(BRCONT);
	m_BranchHash.InitHashTable(_MaxBranchNo);
}

int BRANCHINFO::AddNewBranch(BRANCHBASE*tBranch)
{
	if (BranchTotal>=_MaxBranchNo)
	{
		sprintf(ErrorMessage[0],"线路数目超过设定值%d",_MaxBranchNo);
		cpGetErrorInfo()->PrintWarning(6,1);
		return -1;
	}
	int flag;
	flag = m_BranchHash.InsertElementToHashTable(BranchTotal,tBranch->Name);
	if (flag>=0)
	{
		sprintf(ErrorMessage[0]," 重复的线路数据数据，忽略: ");
		tBranch->PrintInfo(ErrorMessage[1]);
		cpGetErrorInfo()->PrintWarning(7,2);
		return -1;
	}
	Branch[BranchTotal] = tBranch;
	BranchTotal++;
	return BranchTotal-1;
}

class NetTempSortBranchLink : public QikSortInterface
{//排序
public:
	NetTempSortBranchLink(BRANCHINFO::BRANCHLINK*tLink){pLink=tLink;}
private:
	int flag;
	BRANCHINFO::BRANCHLINK sorttemp;
	BRANCHINFO::BRANCHLINK*pLink;
protected:
	virtual int kompar(int m,int n);
	virtual void swap(int m,int n);
};
int NetTempSortBranchLink::kompar(int m,int n)
{
	if (pLink[m].IBusOldNo==pLink[n].IBusOldNo)
	{
		return pLink[m].JBusOldNo-pLink[n].JBusOldNo;
	}
	return (pLink[m].IBusOldNo-pLink[n].IBusOldNo);
}
void NetTempSortBranchLink::swap(int m,int n)
{
	sorttemp=pLink[m];
	pLink[m]=pLink[n];
	pLink[n]=sorttemp;
}
void BRANCHINFO::BuildLink(int tBusTotal)
{
	int i,j;
	BusTotal=tBusTotal;
	BranchLinkNo=2*BranchTotal;
	if(BranchLink!=NULL)FreeArray(BranchLink);
	if (BranchLinkNo>0)
		MallocNew(BranchLink,BRANCHLINK,BranchLinkNo);
	for (i=0,j=0;i<BranchTotal;i++)
	{
		BranchLink[2*j].IBusOldNo=Branch[i]->FromBusNo;
		BranchLink[2*j].JBusOldNo=Branch[i]->ToBusNo;
		BranchLink[2*j].BrnhNo=i;
		BranchLink[2*j].nDirtn=1;
		BranchLink[2*j+1].IBusOldNo=Branch[i]->ToBusNo;
		BranchLink[2*j+1].JBusOldNo=Branch[i]->FromBusNo;
		BranchLink[2*j+1].BrnhNo=i;
		BranchLink[2*j+1].nDirtn=2;
		j++;
	}
	BranchLinkNo=2*j;
	
	NetTempSortBranchLink tempSort(BranchLink);
	tempSort.QikSort(0,BranchLinkNo-1);
	
	if(BRINDX!=NULL)FreeArray(BRINDX);
	if(BRCONT!=NULL)FreeArray(BRCONT);
	if(BusTotal>0){MallocNew(BRINDX,int,BusTotal);memset(BRINDX,0,BusTotal*sizeof(int));}
	if(BusTotal>0){MallocNew(BRCONT,int,BusTotal);memset(BRCONT,0,BusTotal*sizeof(int));}
	int tBn;
	for (i=0,tBn=0;i<BusTotal&&tBn<BranchLinkNo;i++)
	{
		BRINDX[i]=tBn;
		BRCONT[i]=0;
		while (tBn<BranchLinkNo && BranchLink[tBn].IBusOldNo==i)
		{
			tBn++;
			BRCONT[i]++;
		}
	}
}

int BRANCHINFO::BranchSearch( char *lpszString )
{
	return m_BranchHash.ElementSearch(lpszString);
}


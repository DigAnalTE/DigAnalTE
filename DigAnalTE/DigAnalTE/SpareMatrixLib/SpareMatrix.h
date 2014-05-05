
#ifndef __SPAREMATRIX_H__
#define __SPAREMATRIX_H__

//稀疏矩阵基类
//提供矩阵结构功能
//由于没有元素项，必须使用派生类

#include "../CommonFunction/QikSort.h"

class SPAREMATRIXBASE : public QikSortInterface
{
public:
	SPAREMATRIXBASE()
	{
		RowTotal=0;
		ElementTotal=0;
		IA=NULL;NA=NULL;JA=NULL;II=NULL;
		IsNewNo=0;
		NewNo=NULL;OldNo=NULL;NIA=NULL;NNA=NULL;NJA=NULL;VALink=NULL;
		JacSpaceMax=0;
	}
	~SPAREMATRIXBASE(){FreeSpace();}
public:
	int RowTotal;
	int ElementTotal;
	int *IA,*NA,*JA,*II;
public://重新编号
	int IsNewNo;//是否编号了，没有编号不能进行计算，即下面的空间是否存在//矩阵计算时需要自动编号
	int *NewNo;//新的节点号  NewBusNo=NewNo[OldBusNo]
	int *OldNo;//旧的节点号  OldBusNo=OldNo[NewBusNo]
	int *NIA;
	int *NNA;
	int *NJA;
	int *VALink;//指向原来的VA   新的值[i]=VA[VALink[i]]
	int JacSpaceMax;//考虑注入元后矩阵大小//排序后尽量给出一个较准确的值//过大的值影响内存//过小的值影响速度
public:
	void SpareMatrixReorder();//稀疏矩阵重新编号
protected:
	virtual int kompar(int m,int n);
	virtual void swap(int m,int n);
public:
	void MallocSpace(int tRow,int tElement)
	{
		if(tRow==RowTotal && tElement==ElementTotal)
			return;
		if(RowTotal!=0 && ElementTotal!=0)
			FreeSpace();
		RowTotal=tRow;ElementTotal=tElement;
		MallocNew(IA,int,RowTotal);
		MallocNew(NA,int,RowTotal);
		MallocNew(JA,int,ElementTotal);
		MallocNew(II,int,RowTotal);
		subMallocSpace(tRow,tElement);
	}
	virtual void subMallocSpace(int tRow,int tElement)=0;
	void MallocSpace(SPAREMATRIXBASE*pOld)
	{
		MallocSpace(pOld->RowTotal,pOld->ElementTotal);
		memcpy(IA,pOld->IA,pOld->RowTotal*sizeof(int));
		memcpy(NA,pOld->NA,pOld->RowTotal*sizeof(int));
		memcpy(JA,pOld->JA,pOld->ElementTotal*sizeof(int));
		memcpy(II,pOld->II,pOld->RowTotal*sizeof(int));
	}
	virtual void FreeSpace()//将所有空间清空
	{
		if(RowTotal!=0 && ElementTotal!=0)
		{
			RowTotal=0;ElementTotal=0;
			FreeArray(IA);FreeArray(NA);FreeArray(JA);FreeArray(II);
		}
		if (IsNewNo!=0)
		{
			IsNewNo=0;
			FreeArray(NewNo);FreeArray(OldNo);FreeArray(NIA);FreeArray(NNA);FreeArray(NJA);FreeArray(VALink);
		}
		JacSpaceMax=0;
	}
	virtual void Clear()=0;//将所有值清零
};

#endif

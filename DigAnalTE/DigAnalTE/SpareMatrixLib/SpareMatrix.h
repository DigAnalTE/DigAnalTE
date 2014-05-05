
#ifndef __SPAREMATRIX_H__
#define __SPAREMATRIX_H__

//ϡ��������
//�ṩ����ṹ����
//����û��Ԫ�������ʹ��������

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
public://���±��
	int IsNewNo;//�Ƿ����ˣ�û�б�Ų��ܽ��м��㣬������Ŀռ��Ƿ����//�������ʱ��Ҫ�Զ����
	int *NewNo;//�µĽڵ��  NewBusNo=NewNo[OldBusNo]
	int *OldNo;//�ɵĽڵ��  OldBusNo=OldNo[NewBusNo]
	int *NIA;
	int *NNA;
	int *NJA;
	int *VALink;//ָ��ԭ����VA   �µ�ֵ[i]=VA[VALink[i]]
	int JacSpaceMax;//����ע��Ԫ������С//�����������һ����׼ȷ��ֵ//�����ֵӰ���ڴ�//��С��ֵӰ���ٶ�
public:
	void SpareMatrixReorder();//ϡ��������±��
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
	virtual void FreeSpace()//�����пռ����
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
	virtual void Clear()=0;//������ֵ����
};

#endif

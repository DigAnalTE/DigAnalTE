
#include "ComplexSpareMatrix.h"

void COMPLEXSPAREMATRIX::subMallocSpace(int tRow,int tElement)
{
	MallocNew(VG,real,tElement);
	MallocNew(VB,real,tElement);
}

void COMPLEXSPAREMATRIX::FreeSpace()
{
	SPAREMATRIXBASE::FreeSpace();
	FreeArray(VG);
	FreeArray(VB);
}

void COMPLEXSPAREMATRIX::Clear()
{
	memset(VG,0,ElementTotal*sizeof(real));
	memset(VB,0,ElementTotal*sizeof(real));
}

void COMPLEXSPAREMATRIX::GBElementModify(int IBusNo,int JBusNo,real GII,real BII)
{
	int i,j;
	for (i=0,j=IA[IBusNo];i<NA[IBusNo];i++,j++)
	{
		if (JA[j]==JBusNo)
		{
			VG[j]+=GII;
			VB[j]+=BII;
			return;
		}
	}
}
int COMPLEXSPAREMATRIX::GetGBElementValue(int IBusNo,int JBusNo,real& GII,real& BII)
{
	int i,j;
	for (i=0,j=IA[IBusNo];i<NA[IBusNo];i++,j++)
	{
		if (JA[j]==JBusNo)
		{
			GII=VG[j];
			BII=VB[j];
			return 1;
		}
	}
	GII=0;
	BII=0;
	return -1;
}

#include "../NetWorkInfo/NetWorkInfo.h"
void COMPLEXSPAREMATRIX::PrintMatrix(NETWORKINFO*pNet,FILE*fp)
{
	int i,j,k,II,inew,jnew,jold;
	char*nameI;
	float basekvI;
	char*nameJ;
	float basekvJ;
	fprintf(fp,"\n No.   I  --  J             G             B \n");
	k=0;
	for(i=0;i<RowTotal;i++)
	{
		inew=NewNo[i];
		II=0;
		nameI=pNet->cpGetBus(i)->GetBusName();
		basekvI=pNet->cpGetBus(i)->fGetBaseKv();
		fprintf(fp,"+ROW= %7d  :  Num in this Row : %7d\n",i+1,NA[i]);
		k=NIA[inew];
		for(j=0;j<NNA[inew];j++,k++)
		{
			jnew=NJA[k];
			jold=OldNo[jnew];
			if(i==jold){II=VALink[k];continue;}
			if(jnew>inew)continue;
			nameJ=pNet->cpGetBus(jold)->GetBusName();
			basekvJ=pNet->cpGetBus(jold)->fGetBaseKv();
			fprintf(fp,"%7d -- %7d -- %7d: %14.4f%14.4f   ",i+1,jold+1,jnew+1,VG[VALink[k]],VB[VALink[k]]);
			fprintf(fp,"%s%6.1fkV-- %s%6.1fkV\n",nameI,basekvI,nameJ,basekvJ);
		}
		fprintf(fp,"%7d -- %7d -- %7d: %14.4f%14.4f   ",i+1,i+1,inew+1,VG[II],VB[II]);
		fprintf(fp,"%s%6.1fkV-- %s%6.1fkV\n",nameI,basekvI,nameI,basekvI);
	}
	fprintf(fp,"\n Check:  k= %-3d, Y_Total=%-3d\n",k,ElementTotal);
}
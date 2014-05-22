
#include "BranchBase.h"
#include "NetWorkInfo.h"
#include "../CommonFunction/ErrorInfo.h"

int BRANCHBASE::ReadLine(char*Line)
{//5,Name,Status,IBus,JBus,ID,R,X,G,B,Smax,Rate
	int flag;
	flag=sscanf(Line,"%*[^,],%[^,],%d,%[^,],%[^,],%f,%f,%f,%f,%f,%f",
		Name,&State,BusName1,BusName2,
		&R,&X,&G,&B,&Smax,&TK);
	if(flag<10)
	{
		return 0;
	}
	if(TK<0.1)
		TK=1;
	ReplaceName(Name,_MaxNameLen);
	ReplaceName(BusName1,_MaxNameLen);
	ReplaceName(BusName2,_MaxNameLen);
	return 1;
}

void BRANCHBASE::WriteLine(char*Line)
{
	if(fabs(TK-1.0)<0.001)
	sprintf(Line,"5,%10s,%5d,%10s,%10s,%10g,%10g,%10g,%10g,%10g,%10g\n",
		Name,State,
		BusName1,BusName2,
		R,X,G,B,Smax,TK);
	else
	sprintf(Line,"6,%10s,%5d,%10s,%10s,%10g,%10g,%10g,%10g,%10g,%10g\n",
		Name,State,
		BusName1,BusName2,
		R,X,G,B,Smax,TK);
}

void BRANCHBASE::OutputPFOFile(FILE*fp,int dirn)
{
	if(dirn==2)
	{
		fprintf(fp,"  --��·'%s' to '%s'  %8.1f��·�й�%8.1f��·�޹�%7.3f�й����%8.2f�޹����\n",
			Name,BusName1,
			JP*BMVA,JQ*BMVA,Ploss*BMVA,Qloss*BMVA);
	}else{
		fprintf(fp,"  --��·'%s' to '%s'  %8.1f��·�й�%8.1f��·�޹�%7.3f�й����%8.2f�޹����\n",
			Name,BusName2,
			IP*BMVA,IQ*BMVA,Ploss*BMVA,Qloss*BMVA);
	}
}

void BRANCHBASE::JacElement(NETWORKINFO*pNet)
{
	subJacElement();
	pNet->BranchGBModify(FromBusNo,ToBusNo,Y11r,Y11i,Y22r,Y22i,Y12r,Y12i,Y21r,Y21i);
}

void BRANCHBASE::VarientLink(NETWORK_BASE*pNet)
{
	int K1,K2;
	K1=pNet->BusSearch(BusName1);
	K2=pNet->BusSearch(BusName2);
	if (K1<0)
	{
		sprintf(ErrorMessage[0],"��·�еĽڵ� '%s' �����ڣ�",BusName1);
		PrintInfo(ErrorMessage[1]);
		cpGetErrorInfo()->PrintError(2);
		return;
	}
	if (K2<0)
	{
		sprintf(ErrorMessage[0],"��·�еĽڵ� '%s' �����ڣ�",BusName2);
		PrintInfo(ErrorMessage[1]);
		cpGetErrorInfo()->PrintError(2);
		return;
	}
	FromBusNo=K1;
	ToBusNo=K2;
	m_pFBusV=&pNet->cpGetBus(K1)->m_fBusV;
	m_pFBusSita=&pNet->cpGetBus(K1)->m_fBusSita;
	m_pTBusV=&pNet->cpGetBus(K2)->m_fBusV;
	m_pTBusSita=&pNet->cpGetBus(K2)->m_fBusSita;
	BMVA=pNet->GetBMVA();
}

void BRANCHBASE::subJacElement()
{
	real Z;
	Z=R*R+X*X;
	Y12r=-R/(Z*TK);Y12i= X/(Z*TK);Y21r=Y12r;Y21i=Y12i;
	Y11r= R/Z+G/TK;
	Y11i=-X/Z+B/TK;
	Y22r= R/(Z*TK*TK)+G/TK;
	Y22i=-X/(Z*TK*TK)+B/TK;
}

void BRANCHBASE::UpdateValue(NETWORKINFO*pNet)
{//���㳱��
	subJacElement();
	double dSita, Vi, Vj;
	Vi = *m_pFBusV;
	Vj = *m_pTBusV;
	dSita = 0.0174532925f*(*m_pFBusSita-*m_pTBusSita);
	IP = (float)( Vi*Vi*Y11r+Vi*Vj*cos(dSita)*Y12r+Vi*Vj*sin(dSita)*Y12i);
	IQ = (float)(-Vi*Vi*Y11i+Vi*Vj*sin(dSita)*Y12r-Vi*Vj*cos(dSita)*Y12i);
	JP = (float)( Vj*Vj*Y22r+Vi*Vj*cos(dSita)*Y21r-Vi*Vj*sin(dSita)*Y21i);
	JQ = (float)(-Vj*Vj*Y22i-Vi*Vj*sin(dSita)*Y21r-Vi*Vj*cos(dSita)*Y21i);
	Ploss = IP+JP;// if(Ploss<0)Ploss=-Ploss;
	Qloss = IQ+JQ;// if(Qloss<0)Qloss=-Qloss;
}

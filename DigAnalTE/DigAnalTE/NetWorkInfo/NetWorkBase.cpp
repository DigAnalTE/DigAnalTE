
#include "NetWorkInfo.h"
#include "Generator.h"
#include "NetLoad.h"
#include "Compensation.h"
#include "../CommonFunction/ErrorInfo.h"

NETWORK_BASE::NETWORK_BASE()
{
	BMVA=100.0f;FreqSys=50.f;
	m_ChangeCode=1;
	bIsLink=0;
	NetError=1;
}

NETWORK_BASE::~NETWORK_BASE()
{
	FreeSpace();
}

void NETWORK_BASE::FreeSpace()
{
	m_BusInfo.FreeSpace();
	m_BranchInfo.FreeSpace();
	m_AreaInfo.FreeSpace();
	m_EquipmentInfo.FreeSpace();
	m_ChangeCode++;
}

void NETWORK_BASE::NetLink()
{
	int i,BusTotal,BranchTotal,EquipTotal;

	BusTotal=iGetBusTotal();

	BranchTotal=iGetBranchTotal();
	for (i=0;i<BranchTotal;i++)
	{//将Branch中BusName等信息转换成BusNo
		cpGetBranch(i)->VarientLink(this);
	}
	cpGetBranchInfo()->BuildLink(iGetBusTotal());

	EquipTotal=iGetEquipTotal();
	for (i=0;i<EquipTotal;i++)
	{//转换BusName
		cpGetEquip(i)->VarientLink(this);
	}
	cpGetEquipInfo()->BuildLink(iGetBusTotal());

	cpGetAreaInfo()->BuildLink(this);//根据Bus中的信息生成Zone，将Area中ZoneName等信息转换成ZoneNo
	
	cpGetErrorInfo()->CheckMessageType(7);//检查重复错误
	bIsLink=m_ChangeCode;
}

int NETWORK_BASE::NetAnalysis()
{
	if(bIsLink!=m_ChangeCode)NetLink();
	return 1;
}

void NETWORK_BASE::WriteFile(char*file)
{
	FILE* fpPFfile=NULL;
	OpenFile(fpPFfile,file,"w");
	WriteFile(fpPFfile);
	CloseFile(fpPFfile);
}

void NETWORK_BASE::WriteFile(FILE*fpPFfile)
{
	int i;
	char Line[_MaxLineLen];
	int genNo,loadNo,comNo;
	genNo=loadNo=comNo=0;
	for (i=0;i<iGetEquipTotal();i++)
	{
		if(cpGetEquip(i)->c_EquipType=='G')
			genNo++;
		if(cpGetEquip(i)->c_EquipType=='L')
			loadNo++;
		if(cpGetEquip(i)->c_EquipType=='C')
			comNo++;
	}
	int lineNo,tranNo;
	lineNo=tranNo=0;
	for (i=0;i<iGetBranchTotal();i++)
	{
		if(fabs(cpGetBranch(i)->TK-1.0)>0.001)
			tranNo++;
		else
			lineNo++;
	}
	sprintf(Line,"0,%10s,%5d,%5d,%5d,%5d,%5d,%5d,%10g\n",
		Description,iGetBusTotal(),genNo,loadNo,comNo,lineNo,tranNo,BMVA);
	fprintf(fpPFfile,Line);
	for (i=0;i<iGetBusTotal();i++)
	{
		cpGetBus(i)->WriteLine(Line);
		fprintf(fpPFfile,Line);
	}
	for (i=0;i<iGetEquipTotal();i++)
	{
		cpGetEquip(i)->WriteLine(Line);
		fprintf(fpPFfile,Line);
	}
	for (i=0;i<iGetBranchTotal();i++)
	{
		cpGetBranch(i)->WriteLine(Line);
		fprintf(fpPFfile,Line);
	}
	for (i=0;i<iGetAreaTotal();i++)
	{
		cpGetAreaInfo()->WriteLine(i,Line);
		fprintf(fpPFfile,Line);
	}
}

int NETWORK_BASE::ReadFile(char*file)
{
	FILE* fpPFfile=NULL;
	OpenFile(fpPFfile,file,"rb");
	if (fpPFfile == NULL)
		return 0;
	int flag, type;
	int tBusNo, tBranchNo, tTransNo, tGenNo, tLoadNo, tComNo, tAreaNo;
	int iBusNo, iBranchNo, iTransNo, iGenNo, iLoadNo, iComNo, iAreaNo;
	char Line[_MaxLineLen];
	tBusNo=0;
	while(fgets(Line, _MaxLineLen, fpPFfile))
	{//0, Description, Bus, Gen, Load, Shunt, ACLine, Trans, Area,	SB
		flag=sscanf(Line,"%d",&type);
		if(flag<1 || type!=0)
			continue;
		flag=sscanf(Line,"%*d,%[^,],%d,%d,%d,%d,%d,%d,%d,%f",
			Description,
			&tBusNo, &tGenNo, &tLoadNo, &tComNo, &tBranchNo, &tTransNo, &tAreaNo,
			&BMVA);
		if(flag<9)
		{
			fclose(fpPFfile);
			return 0;
		}
		break;
	}
	if(tBusNo<=0)
	{
		fclose(fpPFfile);
		return 0;
	}
	rewind(fpPFfile);
	BUSBASE* tBus;
	iBusNo=0;
	while(fgets(Line, _MaxLineLen, fpPFfile))
	{
		flag=sscanf(Line,"%d",&type);
		if(flag<1 || type!=1)
			continue;
		tBus=new BUSBASE;
		flag=tBus->ReadLine(Line);
		if(flag!=1)
		{
			delete tBus;
			fclose(fpPFfile);
			return 0;
		}
		flag=m_BusInfo.AddNewBus(tBus);
		if(flag<0)
		{
			delete tBus;
			fclose(fpPFfile);
			return 0;
		}
		iBusNo++;
	}
	if(iBusNo!=tBusNo)
	{
		fclose(fpPFfile);
		return 0;
	}
	rewind(fpPFfile);
	GENERATOR* tGen;
	iGenNo=0;
	while(fgets(Line, _MaxLineLen, fpPFfile))
	{
		flag=sscanf(Line,"%d",&type);
		if(flag<1 || type!=2)
			continue;
		tGen=new GENERATOR;
		flag=tGen->ReadLine(Line);
		if(flag!=1)
		{
			delete tGen;
			fclose(fpPFfile);
			return 0;
		}
		flag=m_EquipmentInfo.AddNewEquip(tGen);
		if(flag<0)
		{
			delete tGen;
			fclose(fpPFfile);
			return 0;
		}
		iGenNo++;
	}
	if(iGenNo!=tGenNo)
	{
		fclose(fpPFfile);
		return 0;
	}
	rewind(fpPFfile);
	NETLOAD* tLoad;
	iLoadNo=0;
	while(fgets(Line, _MaxLineLen, fpPFfile))
	{
		flag=sscanf(Line,"%d",&type);
		if(flag<1 || type!=3)
			continue;
		tLoad=new NETLOAD;
		flag=tLoad->ReadLine(Line);
		if(flag!=1)
		{
			delete tLoad;
			fclose(fpPFfile);
			return 0;
		}
		flag=m_EquipmentInfo.AddNewEquip(tLoad);
		if(flag<0)
		{
			delete tLoad;
			fclose(fpPFfile);
			return 0;
		}
		iLoadNo++;
	}
	if(iLoadNo!=tLoadNo)
	{
		fclose(fpPFfile);
		return 0;
	}
	rewind(fpPFfile);
	COMPENSATION* tCom;
	iComNo=0;
	while(fgets(Line, _MaxLineLen, fpPFfile))
	{
		flag=sscanf(Line,"%d",&type);
		if(flag<1 || type!=4)
			continue;
		tCom=new COMPENSATION;
		flag=tCom->ReadLine(Line);
		if(flag!=1)
		{
			delete tCom;
			fclose(fpPFfile);
			return 0;
		}
		flag=m_EquipmentInfo.AddNewEquip(tCom);
		if(flag<0)
		{
			delete tCom;
			fclose(fpPFfile);
			return 0;
		}
		iComNo++;
	}
	if(iComNo!=tComNo)
	{
		fclose(fpPFfile);
		return 0;
	}
	rewind(fpPFfile);
	BRANCHBASE* tBranch;
	iBranchNo=0;
	iTransNo=0;
	while(fgets(Line, _MaxLineLen, fpPFfile))
	{
		flag=sscanf(Line,"%d",&type);
		if(flag<1 || (type!=5&&type!=6))
			continue;
		tBranch=new BRANCHBASE;
		flag=tBranch->ReadLine(Line);
		if(flag!=1)
		{
			delete tBranch;
			fclose(fpPFfile);
			return 0;
		}
		flag=m_BranchInfo.AddNewBranch(tBranch);
		if(flag<0)
		{
			delete tBranch;
			fclose(fpPFfile);
			return 0;
		}
		if(type==5)
			iBranchNo++;
		if(type==6)
			iTransNo++;
	}
	if(iBranchNo!=tBranchNo)
	{
		fclose(fpPFfile);
		return 0;
	}
	if(iTransNo!=tTransNo)
	{
		fclose(fpPFfile);
		return 0;
	}
	rewind(fpPFfile);
	iAreaNo=0;
	while(fgets(Line, _MaxLineLen, fpPFfile))
	{
		flag=sscanf(Line,"%d",&type);
		if(flag<1 || type!=7)
			continue;
		flag=m_AreaInfo.ReadLine(Line);
		if(flag!=1)
		{
			fclose(fpPFfile);
			return 0;
		}
		iAreaNo++;
	}
	if(iAreaNo!=tAreaNo)
	{
		fclose(fpPFfile);
		return 0;
	}
	CloseFile(fpPFfile);
	return 1;
}

void NETWORK_BASE::OutputPFOFile(FILE*fpfile)
{
	int i,j,BusTotal;
	int BrnStart,BrnCount,BranchNo,Directtn;
	BusTotal=iGetBusTotal();
	for (i=0;i<BusTotal;i++)
	{
		cpGetBus(i)->OutputPFOFile(fpfile);
		BrnStart=cpGetBranchInfo()->BRINDX[i];
		BrnCount=cpGetBranchInfo()->BRCONT[i];
		for (j=0;j<BrnCount;j++)
		{
			BranchNo=cpGetBranchInfo()->BranchLink[BrnStart+j].BrnhNo;
			Directtn=cpGetBranchInfo()->BranchLink[BrnStart+j].nDirtn;
			cpGetBranch(BranchNo)->OutputPFOFile(fpfile,Directtn);
		}
		BrnStart=cpGetEquipInfo()->EQINDX[i];
		BrnCount=cpGetEquipInfo()->EQCONT[i];
		for (j=0;j<BrnCount;j++)
		{
			BranchNo=cpGetEquipInfo()->EquipLink[BrnStart+j].EquipNo;
			Directtn=cpGetEquipInfo()->EquipLink[BrnStart+j].nDirtn;
			cpGetEquip(BranchNo)->OutputPFOFile(fpfile,Directtn);
		}
	}
}
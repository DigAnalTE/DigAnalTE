
#include "BPABus.h"
#include "../../DigAnalTE/DigAnalTE/NetWorkInfo/NetWorkInfo.h"
#include "BPAReadWrite.h"
#include "../../DigAnalTE/DigAnalTE/CommonFunction/ErrorInfo.h"

void BPABUS::JacElement(NETWORKINFO*Topo)
{
	BUSBASE::JacElement(Topo);
	if (Removed == 1)
	{
		Topo->NodeTypeModify(BusNo, 3);
	}
	else{
		switch (c_BPAType)
		{
		case 'E':
		case 'Q':
			Topo->NodeTypeModify(BusNo, 2);
			break;
		case 'S':
			Topo->NodeTypeModify(BusNo, 3);
			break;
		case ' ':
		default:
			Topo->NodeTypeModify(BusNo, 1);
			break;
		}
	}
	Topo->NodeGenModify(BusNo, m_fGenP, m_fGenQ, m_fGenPmax, 0, m_fGenQmax, m_fGenQmin);
	//	Topo->NodeGenModify(BusNo,m_fGenP*(1-m_fGenPper),m_fGenQ*(1-m_fGenQper),m_fGenPmax*(1-m_fGenPper),0,m_fGenQmax*(1-m_fGenQper),m_fGenQmin*(1-m_fGenQper));
	Topo->NodeLoadModify(BusNo, -m_fBusPPLoad*m_fBusPLoadPer, -m_fBusQPLoad*m_fBusQLoadPer, 0, 0, m_fBusPalPLoad, m_fBusPalQLoad);
}

void BPABUS::UpdateValue(NETWORKINFO*pNet)
{//problem 此处有问题
	//	float pgen,qgen,bmva;
	//	float plcut,qlcut;
	//	bmva=pNet->GetBMVA();
	//// 	pmax=pNet->PGmax[BusNo]*bmva;
	//// 	pmin=pNet->PGmin[BusNo]*bmva;
	//	pgen=pNet->PPGen[BusNo]*bmva;
	//// 	qmax=pNet->QGmax[BusNo]*bmva;
	//// 	qmin=pNet->QGmin[BusNo]*bmva;
	//	qgen=pNet->QPGen[BusNo]*bmva;
	//	plcut=pNet->PLcut[BusNo]*bmva;
	//	qlcut=pNet->QLcut[BusNo]*bmva;
	//	if ((1-m_fGenPper)>0)
	//	{
	//		m_fGenP=pgen;///(1-m_fGenPper);
	//	}
	//	if ((1-m_fGenQper)>0)
	//	{
	//		m_fGenQ=qgen;///(1-m_fGenQper);
	//	}
	//	m_fPLoadCut=plcut;
	//	m_fQLoadCut=qlcut;
	//
	//	SubUpdateValue(pNet);
}

int BPABUS::ReadLine(char* PFLine)
{
	float V1, V2;
	c_BPAType = PFLine[1];
	GetItemFromLine(PFLine, Owner, BUS_Para[0], BUS_Loca[0]); ReplaceName(Owner, _MaxNameLen);
	GetItemFromLine(PFLine, Name, BUS_Para[1], BUS_Loca[1]); ReplaceName(Name, _MaxNameLen);
	GetItemFromLine(PFLine, (void*)(&BaseKv), BUS_Para[2], BUS_Loca[2]);
	ResetBPAName(Name, BaseKv);
	GetItemFromLine(PFLine, Zone, BUS_Para[3], BUS_Loca[3]); ReplaceName(Zone, _MaxNameLen);
	if (BaseKv<0 || BaseKv>99999.)
	{
		sprintf(ErrorMessage[0], " 节点 %s的基准电压为0，程序缺省将其设为100.0KV", Name);
		cpGetErrorInfo()->PrintWarning(0, 1);
		BaseKv = 100.0f;
	}
	GetItemFromLine(PFLine, (void*)(&m_fBusPPLoad), BUS_Para[4], BUS_Loca[4]);
	GetItemFromLine(PFLine, (void*)(&m_fBusQPLoad), BUS_Para[5], BUS_Loca[5]);
	GetItemFromLine(PFLine, (void*)(&m_fBusPalPLoad), BUS_Para[6], BUS_Loca[6]);
	GetItemFromLine(PFLine, (void*)(&m_fBusPalQLoad), BUS_Para[7], BUS_Loca[7]);
	GetItemFromLine(PFLine, (void*)(&m_fGenPmax), BUS_Para[8], BUS_Loca[8]);
	GetItemFromLine(PFLine, (void*)(&m_fGenP), BUS_Para[9], BUS_Loca[9]);
	GetItemFromLine(PFLine, (void*)(&m_fGenQmax), BUS_Para[10], BUS_Loca[10]);
	GetItemFromLine(PFLine, (void*)(&m_fGenQmin), BUS_Para[11], BUS_Loca[11]);
	GetItemFromLine(PFLine, (void*)(&V1), BUS_Para[12], BUS_Loca[12]);
	GetItemFromLine(PFLine, (void*)(&V2), BUS_Para[13], BUS_Loca[13]);
	//m_fGenQ=m_fGenQmax;
	if (fabs(m_fGenPmax) < 0 || m_fGenPmax < m_fGenP) m_fGenPmax = 999999.f;
	m_fGenPmax = fabs(m_fGenPmax);

	switch (c_BPAType)
	{
	case ' ':
		m_fGenQ = m_fGenQmax;
		m_fBusVmax = V1; m_fBusVmin = V2;
		break;
	case 'E':
	case 'Q':
		m_fGenQ = 0;
		m_fBusV = V1;
		m_fBusVmin = V2;
		break;
	case 'S':
		m_fGenQ = 0;
		m_fBusV = V1;
		m_fBusSita = V2;
		break;
	default:
		sprintf(ErrorMessage[0], " 目前不支持该卡%s", PFLine);
		cpGetErrorInfo()->PrintError(1);
		return 0;
		break;
	}
	return 1;
}

void BPABUS::OutputPFOFile(FILE*fp)
{
	char OutLine[_MaxLineLen];
	char BusName[9], ZoneName[3], OwnerName[4];

	strncpy(BusName, Name, 8); ReplaceName(BusName, 9);
	strncpy(ZoneName, Zone, 2); ReplaceName(ZoneName, 3);
	strncpy(OwnerName, Owner, 3); ReplaceName(OwnerName, 4);

	float pLoad, qLoad;
	pLoad = m_fBusPPLoad*m_fBusPLoadPer;
	qLoad = m_fBusQPLoad*m_fBusQLoadPer;

	sprintf(OutLine, "%s%6.1f %7.2fkV/%5.1f度  %s%8.1f有功出力%8.1f无功出力%7.1f有功负荷%8.1f无功负荷 %s  %7.3f电压pu  B%c\n",
		BusName, BaseKv,
		m_fBusV*BaseKv, m_fBusSita,
		ZoneName,
		m_fGenP, m_fGenQ,
		pLoad, qLoad,
		OwnerName,
		m_fBusV,
		c_BPAType);
	if (OutLine[121] == ' '){ OutLine[121] = '\n'; OutLine[122] = '\0'; }

	int k;
	if (fabs(m_fGenP) <= 0.04)
	{
		for (k = 36; k < 52; k++)
		{
			OutLine[k] = ' ';
		}
		if (fabs(m_fGenQ) <= 0.04)
		{
			for (k = 52; k < 68; k++)
			{
				OutLine[k] = ' ';
			}
		}
	}
	if (fabs(pLoad) + fabs(qLoad) <= 0.001)
	{
		for (k = 68; k < 100; k++)
		{
			OutLine[k] = ' ';
		}
	}
	fprintf(fp, OutLine);
}
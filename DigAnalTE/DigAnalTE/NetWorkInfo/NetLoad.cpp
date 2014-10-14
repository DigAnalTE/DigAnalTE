
#include "NetLoad.h"
#include "../DynamicModel/DynamicModelInfo.h"

void NETLOAD::subJacElement(NETWORKINFO*pNet)
{
	if (m_LoadType == 0)
	{
		PPNET -= m_fLoadP*m_fLoadPPer;
		QPNET -= m_fLoadQ*m_fLoadQPer;
	}
	else if (m_LoadType == 1)
	{
		PINET -= m_fLoadP*m_fLoadPPer;
		QINET -= m_fLoadQ*m_fLoadQPer;
	}
	else if (m_LoadType == 2)
	{
		PZNET -= m_fLoadP*m_fLoadPPer;
		QZNET -= m_fLoadQ*m_fLoadQPer;
	}
}

void NETLOAD::SetLoadPQper(float tPper, float tQper)
{
	m_fLoadPPer = tPper;
	m_fLoadQPer = tQper;
}

int NETLOAD::ReadLine(char*Line)
{//3,Name,Status,BusName,Type,Pload,Qload,Pper,Qper
	int flag;
	flag = sscanf(Line, "%*[^,],%[^,],%d,%[^,],%d,%f,%f,%f,%f",
		Name, &State, BusName, &m_LoadType,
		&m_fLoadP, &m_fLoadQ,
		&m_fLoadPPer, &m_fLoadQPer);
	if (flag < 8)
	{
		return 0;
	}
	ReplaceName(Name, _MaxNameLen);
	ReplaceName(BusName, _MaxNameLen);
	return 1;
}

void NETLOAD::WriteLine(char*Line)
{
	sprintf(Line, "N,%10s,%5d,%10s,%5d,%10g,%10g,%10g,%10g\n",
		Name, State, BusName, m_LoadType,
		m_fLoadP, m_fLoadQ,
		m_fLoadPPer, m_fLoadQPer);
}

void NETLOAD::OutputPFOFile(FILE*fp, int nDirtn)
{
	fprintf(fp, "  --����'%s' at '%s'  %8.1f�й�����%8.1f�޹�����\n",
		Name, BusName,
		m_fLoadP*m_fLoadPPer, m_fLoadQ*m_fLoadQPer);
}

void NETLOAD::FormDynMatrix(DYNAMICMODELINFO*pDyn)
{
	real V;
	V = pBus->m_fBusV;
	real P, Q;
	if (m_LoadType == 0)
	{
		P = m_fLoadP*m_fLoadPPer / V / V;
		Q = -m_fLoadQ*m_fLoadQPer / V / V;
	}
	else if (m_LoadType == 1)
	{
		P = m_fLoadP*m_fLoadPPer / V;
		Q = -m_fLoadQ*m_fLoadQPer / V;
	}
	else if (m_LoadType == 2)
	{
		P = m_fLoadP*m_fLoadPPer;
		Q = -m_fLoadQ*m_fLoadQPer;
	}
	pDyn->ModifyNetMatrix(BusNo, BusNo, P / pDyn->GetBMVA(), Q / pDyn->GetBMVA());
}

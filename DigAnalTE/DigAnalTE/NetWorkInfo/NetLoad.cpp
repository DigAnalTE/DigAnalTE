
#include "NetLoad.h"

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
	fprintf(fp, "  --负荷'%s' at '%s'  %8.1f有功负荷%8.1f无功负荷\n",
		Name, BusName,
		m_fLoadP*m_fLoadPPer, m_fLoadQ*m_fLoadQPer);
}

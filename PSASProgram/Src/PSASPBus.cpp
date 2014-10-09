
#include "PSASPBus.h"

int PSASPBUS::ReadLine(char*line)
{
	int flag;
	flag = sscanf(line, "%*[^']'%[^']',%f,%d,%f,%f,%f,%f",
		Name,
		&BaseKv,	&m_iAreaNo,
		&m_Vmax,	&m_Vmin, 
		&m_CB1,		&m_CB3 );
	if (flag != 7) return 0;
	if (m_iAreaNo < 0)
		return 0;
	ReplaceName(Name, _MaxNameLen);
	m_fBusVmax = m_Vmax / BaseKv;
	m_fBusVmin = m_Vmin / BaseKv;
	sprintf(Zone, "%4d", m_iAreaNo);
	return 1;
}

void PSASPBUS::OutputPFOFile(FILE*fp)
{
	fprintf(fp, "%4d,%15.10f,%15.10f,\n", BusNo + 1, m_fBusV, m_fBusSita);
}

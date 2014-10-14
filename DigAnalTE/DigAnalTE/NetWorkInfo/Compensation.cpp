
#include "Compensation.h"
#include "../DynamicModel/DynamicModelInfo.h"

void COMPENSATION::subJacElement(NETWORKINFO*pNet)
{
	PGNET = 0; QGNET = 0; PGMAX = 0; PGMIN = 0; QGMAX = 0; QGMIN = 0;
	PGNET = m_fPP;
	QGNET = m_fQP;
	PGMAX = m_fPP;
	PGMIN = m_fPP;
	QGMAX = m_fQP;
	QGMIN = m_fQP;
	PZNET = m_fPZ; QZNET = m_fQZ;
}

int COMPENSATION::ReadLine(char*Line)
{//4,Name,Status,BusName,PP,QP,PZ,QZ
	int flag;
	flag = sscanf(Line, "%*[^,],%[^,],%d,%[^,],%f,%f,%f,%f",
		Name, &State, BusName,
		&m_fPP, &m_fQP, &m_fPZ, &m_fQZ);
	if (flag < 7)
	{
		return 0;
	}
	ReplaceName(Name, _MaxNameLen);
	ReplaceName(BusName, _MaxNameLen);
	return 1;
}

void COMPENSATION::WriteLine(char*Line)
{
	sprintf(Line, "S,%10s,%5d,%10s,%10g,%10g,%10g,%10g\n",
		Name, State, BusName,
		m_fPP, m_fQP,
		m_fPZ, m_fQZ);
}

void COMPENSATION::OutputPFOFile(FILE*fp, int nDirtn)
{
	fprintf(fp, "  --补偿'%s' at '%s'  %8.1f有功补偿%8.1f无功补偿\n",
		Name, BusName,
		m_fPP + m_fPZ, m_fQP + m_fQZ);
}

void COMPENSATION::FormDynMatrix(DYNAMICMODELINFO*pDyn)
{
	real V;
	V = pBus->m_fBusV;
	real P, Q;
	P = m_fPP / V / V + m_fPZ;
	Q = m_fQP / V / V + m_fQZ;
	pDyn->ModifyNetMatrix(BusNo, BusNo, -P / pDyn->GetBMVA(), Q / pDyn->GetBMVA());
}

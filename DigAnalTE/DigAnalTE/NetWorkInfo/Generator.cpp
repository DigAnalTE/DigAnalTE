
#include "Generator.h"
#include "BusInfo.h"
#include "NetWorkInfo.h"

void GENERATOR::subJacElement(NETWORKINFO*pNet)
{
	JacType = GenType;
	PGNET = 0; QGNET = 0; PGMAX = 0; PGMIN = 0; QGMAX = 0; QGMIN = 0;
	PGMAX = m_fGenPmax;
	QGMAX = m_fGenQmax;
	if (State == 1)
	{
		PGMIN = m_fGenPmin;
		QGMIN = m_fGenQmin;
		if (m_fGenP > m_fGenPmax)
		{
			PGNET = m_fGenPmax;
		}
		else if (m_fGenP < m_fGenPmin)
		{
			PGNET = m_fGenPmin;
		}
		else
			PGNET = m_fGenP;
		if (m_fGenQ > m_fGenQmax)
		{
			QGNET = m_fGenQmax;
		}
		else if (m_fGenQ < m_fGenQmin)
		{
			QGNET = m_fGenQmin;
		}
		else
			QGNET = m_fGenQ;
	}
	if (State == 1 && GenType > 1)
	{
		pNet->NodeVCtrlModify(BusNo, m_fGenVCtrl);
	}
}

void GENERATOR::UpdateValue(NETWORKINFO*pNet)
{
	float pgen, pmax, qgen, qmax, bmva;
	bmva = pNet->GetBMVA();
	pmax = pNet->PGmax[BusNo] * bmva;
	pgen = pNet->PPGen[BusNo] * bmva;
	qmax = pNet->QGmax[BusNo] * bmva;
	qgen = pNet->QPGen[BusNo] * bmva;
	if (pmax > 0.0001)
		m_fGenP = pgen*m_fGenPmax / pmax;
	else
		m_fGenP = 0;
	if (qmax > 0.0001)
		m_fGenQ = qgen*m_fGenQmax / qmax;
	else
		m_fGenQ = 0;
}

int GENERATOR::ReadLine(char*Line)
{//2,Name,Status,BusName,ID,Type,Pgen,Qgen,Pmax,Pmin,Qmax,Qmin
	int flag;
	flag = sscanf(Line, "%*[^,],%[^,],%d,%[^,],%d,%f,%f,%f,%f,%f,%f",
		Name, &State,
		BusName, &GenType,
		&m_fGenP, &m_fGenQ, &m_fGenPmax, &m_fGenPmin, &m_fGenQmax, &m_fGenQmin);
	if (flag < 10)
	{
		return 0;
	}
	ReplaceName(Name, _MaxNameLen);
	ReplaceName(BusName, _MaxNameLen);
	return 1;
}

void GENERATOR::WriteLine(char*Line)
{
	sprintf(Line, "2,%10s,%5d,%10s,%5d,%10g,%10g,%10g,%10g,%10g,%10g\n",
		Name, State, BusName, GenType,
		m_fGenP, m_fGenQ,
		m_fGenPmax, m_fGenPmin,
		m_fGenQmax, m_fGenQmin);
}

void GENERATOR::OutputPFOFile(FILE*fp, int nDirtn)
{
	fprintf(fp, "  --发电'%s' at '%s'  %8.1f有功出力%8.1f无功出力\n",
		Name, BusName,
		m_fGenP, m_fGenQ);
}

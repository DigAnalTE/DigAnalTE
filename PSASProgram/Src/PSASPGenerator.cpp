
#include "PSASPGenerator.h"
#include "../../DigAnalTE/DigAnalTE/NetWorkInfo/NetWorkInfo.h"

int PSASPGENERATOR::ReadLine(char*Line)
{
	int flag;
	flag = sscanf(Line, "%d, %d, %d, %f, %f, %f, %f, %f, %f, %f, %f, %d, %d, %d, %f, %d,",
		&State, &m_iBusNo, &m_iType,
		&m_Pg, &m_Qg, &m_fGenVCtrl, &m_sita,
		&m_Qmax, &m_Qmin, &m_Pmax, &m_Pmin,
		&m_iPL, &m_iCB, &m_iCL, &m_VQP, &m_iKcd);
	if (flag != 16) return 0;
	sprintf(Name, "G%4d", m_iBusNo);
	switch (m_iType)
	{
	case 0:
		if (fabs(m_Pmax) < 0.001) m_Pmax = 999.f;
		if (fabs(m_Pmin) < 0.001) m_Pmin = -999.f;
		if (fabs(m_Qmax) < 0.001) m_Qmax = 999.f;
		if (fabs(m_Qmin) < 0.001) m_Qmin = -999.f;
		GenType = 3;
		break;
	case -1:
		if (fabs(m_Qmax) < 0.001) m_Qmax = 999.f;
		if (fabs(m_Qmin) < 0.001) m_Qmin = -999.f;
	case -2:
		if (fabs(m_Pmax) < 0.001) m_Pmax = m_Pg;
		if (fabs(m_Pmin) < 0.001) m_Pmin = m_Pg;
		GenType = 2;
		break;
	case 1:
		if (fabs(m_Qmax) < 0.001) m_Qmax = m_Qg;
		if (fabs(m_Qmin) < 0.001) m_Qmin = m_Qg;
	case -3:
	default:
		if (fabs(m_Pmax) < 0.001) m_Pmax = m_Pg;
		if (fabs(m_Pmin) < 0.001) m_Pmin = m_Pg;
		GenType = 1;
		break;
	}
	return 1;
}

void PSASPGENERATOR::UpdateValue(NETWORKINFO*pNet)
{
	GENERATOR::UpdateValue(pNet);
	float bmva;
	bmva = pNet->GetBMVA();
	m_Pg = m_fGenP / bmva;
	m_Qg = m_fGenQ / bmva;
}

void PSASPGENERATOR::OutputPFOFile(FILE*fp, int nDirtn)
{
	fprintf(fp, "%5d,%20.10f,%20.10f,\n",
		m_iBusNo, m_Pg, m_Qg);
}
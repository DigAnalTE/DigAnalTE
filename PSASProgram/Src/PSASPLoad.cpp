
#include "PSASPLoad.h"
#include "../../DigAnalTE/DigAnalTE/NetWorkInfo/NetWorkInfo.h"

int PSASPLOAD::ReadLine(char*Line)
{
	int flag;
	flag = sscanf(Line, "%d, %d, %d, %d, %f, %f, %f, %f, %f, %f, %f, %f, %d, %d, %d, %f,",
		&State, &m_iBusNo, &m_iLoadNo,
		&m_iType, &m_Pl, &m_Ql,
		&m_V, &m_sita, &m_Qmax,
		&m_Qmin, &m_Pmax, &m_Pmin,
		&m_iPL, &m_iCB, &m_iCL,
		&m_VQP);
	if (flag != 16) return 0;
	sprintf(Name, "L%4d", m_iBusNo);
	return 1;
}

void PSASPLOAD::UpdateValue(NETWORKINFO*pNet)
{
	NETLOAD::UpdateValue(pNet);
	float bmva;
	bmva = pNet->GetBMVA();
	m_Pl = m_fLoadP / bmva;
	m_Ql = m_fLoadQ / bmva;
}

void PSASPLOAD::OutputPFOFile(FILE*fp, int nDirtn)
{
	fprintf(fp, "%5d,%5d,%20.10f,%20.10f,\n",
		m_iBusNo, m_iLoadNo, m_Pl, m_Ql);
}

#include "PSASPTrans.h"
#include "../../DigAnalTE/DigAnalTE/NetWorkInfo/NetWorkInfo.h"

int PSASPTRANS::ReadLine(char*Line)
{
	int flag;
	flag = sscanf(Line, "%d, %d, %d, %d, %f, %f, %f, %f, %f, %d, %d, %d, %f, %d, %d, %f, %f, %f, %f, %d, %d, %d,",
		&m_iMark,	&m_iIname,	&m_iJname,
		&m_iTranNo,	&R,			&X,
		&m_Tk,		&m_Rm,		&m_Xm,
		&m_iFT,		&m_iType,	&m_iPL,
		&m_TP,		&m_iCB,		&m_iCL,
		&m_VQP,		&m_sita,		&m_TC,
		&m_LIM,		&m_iID,		&m_iJstar,
		&m_iTranType);
	if (flag != 22) return 0;
	sprintf(Name, "T%4d", m_iTranNo);
	if (m_iIname < 0)
	{
		m_iIname = -m_iIname;
		flag3W = m_iIname;
	}
	if (m_iJname < 0)
	{
		m_iJname = -m_iJname;
		flag3W = m_iJname;
	}
	if (fabs(m_Rm) > 0.0001 && fabs(m_Xm) > 0.0001)
	{
		G = m_Rm / (m_Rm*m_Rm + m_Xm*m_Xm);
		B = m_Xm / (m_Rm*m_Rm + m_Xm*m_Xm);
	}
	else{
		G = 0;
		B = 0;
	}
	TK = m_Tk;
	State = 0;
	if (m_iMark == 1)State = 1;
	FromBusNo = m_iIname - 1;
	ToBusNo = m_iJname - 1;
	return 1;
}

void PSASPTRANS::OutputPFOFile(FILE*fp, int nDirtn)
{
	fprintf(fp, "%6d,%6d,%6d,%10.5f,%10.5f,%10.5f,%10.5f,%10.5f,%10.5f,\n",
		m_iIname, m_iJname, m_iTranNo,
		m_Pi, m_Qi, m_Pj, m_Qj, m_Qci, m_Qcj);
}

void PSASPTRANS::subUpdateValue(NETWORKINFO*pNet)
{//¼ÆËã³±Á÷
	m_Pi = IP / pNet->GetBMVA();
	m_Qi = IQ / pNet->GetBMVA();
	m_Pj = -JP / pNet->GetBMVA();
	m_Qj = -JQ / pNet->GetBMVA();
	float Vi, Vj;
	Vi = *m_pFBusV;
	Vj = *m_pTBusV;
	m_Qci = Vi*Vi*B / TK;
	m_Qcj = Vj*Vj*B / TK;
}
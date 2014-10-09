
#include "PSASPACLine.h"
#include "../../DigAnalTE/DigAnalTE/NetWorkInfo/NetWorkInfo.h"

int PSASPACLINE::ReadLine(char*Line)
{
	int flag;
	flag = sscanf(Line, "%d, %d, %d, %d, %f, %f, %f, %d, %d, %d, %d, %d, %f, %f, %f,",
		&m_iMark,
		&m_iIname,	&m_iJname,	&m_iLineNo,
		&R,			&X,			&B,
		&m_iFT,		&m_iType,
		&m_iPL,		&m_iCB,		&m_iCL,
		&m_VQP,		&m_LC,		&m_LIM );
	if (flag != 15) return 0;
	G = 0;
	TK = 1;
	sprintf(Name, "L%4d", m_iLineNo);
	State = 0;
	if (m_iMark == 1)State = 1;
	FromBusNo = m_iIname - 1;
	ToBusNo = m_iJname - 1;
	return 1;
}

void PSASPACLINE::OutputPFOFile(FILE*fp, int nDirtn)
{
	fprintf(fp, "%6d,%6d,%6d,%10.5f,%10.5f,%10.5f,%10.5f,%10.5f,%10.5f,\n",
		m_iIname, m_iJname, m_iLineNo,
		m_Pi, m_Qi, m_Pj, m_Qj, m_Qci, m_Qcj);
}

void PSASPACLINE::JacElement(NETWORKINFO*pNet)
{
	if (m_iIname == m_iJname)
	{
		real Z;
		Z = R*R + X*X;
		pNet->PZnet[FromBusNo] += -R / Z;
		pNet->QZnet[FromBusNo] += -X / Z;
	}
	else{
		BRANCHBASE::JacElement(pNet);
	}
}

void PSASPACLINE::subUpdateValue(NETWORKINFO*pNet)
{//¼ÆËã³±Á÷
	if (m_iIname == m_iJname)
	{
		real Z;
		Z = R*R + X*X;
		float Vi, Vj;
		Vi = *m_pFBusV;
		Vj = *m_pTBusV;
		m_Pi = R / Z*Vi*Vi;
		m_Qi = X / Z*Vi*Vi;
		m_Pj = 0;
		m_Qj = 0;
		m_Qci = 0;
		m_Qcj = 0;
	}
	else{
		m_Pi = IP / pNet->GetBMVA();
		m_Qi = IQ / pNet->GetBMVA();
		m_Pj = -JP / pNet->GetBMVA();
		m_Qj = -JQ / pNet->GetBMVA();
		float Vi, Vj;
		Vi = *m_pFBusV;
		Vj = *m_pTBusV;
		m_Qci = Vi*Vi*B;
		m_Qcj = Vj*Vj*B;
	}
}
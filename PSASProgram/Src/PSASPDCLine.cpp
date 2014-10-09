
#include "PSASPDCLine.h"

int PSASPDCLINE::ReadLine(char*Line)
{
	int flag;
	if (iLine < 0 || iLine>=5)return 0;
	switch (iLine)
	{
	case 0:
		flag = sscanf(Line, "%d, %d, %d, %d, %f, %f, %f, %f, %f, %f,",
			&State, &m_iIname, &m_iJname,
			&m_iLineNo, &m_R1, &m_L1,
			&m_R2, &m_L2, &m_RL,
			&m_XL);
		if (flag != 10) return 0;
		break;
	case 1:
		flag = sscanf(Line, " %f, %f, %f, %f,",
			&m_Re, &m_Le,
			&m_Rs, &m_Ls);
		if (flag != 4)  return 0;
		break;
	case 2:
		flag = sscanf(Line, " %f, %f, %d, %f, %f, %f, %f, %f, %f, %d, ",
			&m_Vhi, &m_Vli, &m_Bi,
			&m_Qci, &m_Sti, &m_Rki,
			&m_Xki, &m_Vmaxi, &m_Vmini,
			&m_Ni);
		if (flag != 10)  return 0;
		break;
	case 3:
		flag = sscanf(Line, " %f, %f, %d, %f, %f, %f, %f, %f, %f, %d, ",
			&m_Vhj, &m_Vlj, &m_Bj,
			&m_Qcj, &m_Stj, &m_Rkj,
			&m_Xkj, &m_Vmaxj, &m_Vminj,
			&m_Nj);
		if (flag != 10)  return 0;
		break;
	case 4:
		flag = sscanf(Line, " %d, %f, %f, %f, %f, %f, %f, %d, ",
			&m_OP, &m_Id, &m_Vd,
			&m_Amin, &m_A0, &m_Bmin,
			&m_B0, &m_iAreaNo);
		if (flag != 8)  return 0;
		break;
	}
	sprintf(Name, "DC%4d", m_iLineNo);
	BusNo1 = m_iIname - 1;
	BusNo2 = m_iJname - 1;
	return 1;
}

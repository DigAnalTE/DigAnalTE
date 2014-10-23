
#include "BPAGenerator.h"
#include "../../DigAnalTE/DigAnalTE/NetWorkInfo/BusInfo.h"
#include "../../DigAnalTE/DigAnalTE/NetWorkInfo/NetWorkInfo.h"
#include "../../DigAnalTE/DigAnalTE/DynamicModel/DynamicModelInfo.h"

void BPAGENERATOR::subJacElement(NETWORKINFO*pNet)
{
}

void BPAGENERATOR::UpdateValue(NETWORKINFO*pNet)
{
	m_fGenP = pBus->m_fGenP*m_fPper;
	m_fGenQ = pBus->m_fGenQ*m_fQper;
}

int BPAGENERATOR::ReadLine(char*Line)
{
	return 0;
}

void BPAGENERATOR::FormDynMatrix(DYNAMICMODELINFO*pDyn)
{
	real V;
	V = pBus->m_fBusV;
	pDyn->ModifyNetMatrix(BusNo, BusNo, -m_fGenP / V / V / pDyn->GetBMVA(), m_fGenQ / V / V / pDyn->GetBMVA());
}

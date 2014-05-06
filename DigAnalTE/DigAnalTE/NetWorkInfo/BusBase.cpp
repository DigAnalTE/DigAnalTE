
#include "BusBase.h"
#include "../CommonFunction/ErrorInfo.h"
#include "NetWorkInfo.h"

int BUSBASE::ReadLine(char*Line)
{//1,Name,Base,Zone,VM,VA,Vmax,Vmin
	int flag;
	flag=sscanf(Line,"%*[^,],%[^,],%f,%[^,],%f,%f,%f,%f",
		Name,&BaseKv,Zone,
		&m_fBusV,&m_fBusSita,&m_fBusVmax,&m_fBusVmin);
	if(flag<7)
	{
		return 0;
	}
	ReplaceName(Name,_MaxNameLen);
	ReplaceName(Zone,_MaxNameLen);
	return 1;
}

void BUSBASE::WriteLine(char*Line)
{
	sprintf(Line,"1,%10s,%8g,%5s,%10g,%10g,%10g,%10g\n",
		Name,BaseKv,Zone,m_fBusV,m_fBusSita,m_fBusVmax,m_fBusVmin);
}

void BUSBASE::PrintInfo(char*Line)
{
	sprintf(Line,"%s-%7.1fkv",Name,BaseKv);
}

void BUSBASE::OutputPFOFile(FILE*fp)
{
	fprintf(fp,"母线'%s'  %7.2fkV %5.1f度   %8.1f有功误差%8.1f无功误差\n",
		Name,
		m_fBusV*BaseKv,m_fBusSita,
		m_fPerr,m_fQerr);
}


#include "DynamicModelBase.h"
#include "../CommonFunction/ErrorInfo.h"

real* EQUIPMENT_DYN_MODEL::GetVarient(char *name)
{
	DynVarient *tVar;
	tVar = GetDynVarient(name);
	if (tVar == NULL)
		return NULL;
	return tVar->pValu;
}

DynVarient* EQUIPMENT_DYN_MODEL::GetDynVarient(char *name, int flag)
{
	int i;
	if (GetVarientTotal()<=0)
	{
		pModel->PrintInfo(ErrorMessage[1]);
		sprintf(ErrorMessage[0], "设备'%s'含有0个输出变量", ErrorMessage[1]);
		cpGetErrorInfo()->PrintError(1);
		return NULL;
	}
	for (i = 0; i < GetVarientTotal(); i++)
	{
		if (strcmp(Varient[i].IndexName, name) == 0)
			return &Varient[i];
	}
	if (flag == 0)
		return NULL;
	pModel->PrintInfo(ErrorMessage[1]);
	sprintf(ErrorMessage[0], "设备'%s'找不到变量'%s',该变量含有的变量名如下：", ErrorMessage[1], name);
	for (i = 0; i < GetVarientTotal(); i++)
	{
		if (i % 5 == 0)
		{
			cpGetErrorInfo()->PrintWarning(-1, 1);
			ErrorMessage[0][0] = '\0';
		}
		sprintf(ErrorMessage[1], "     %s", Varient[i].IndexName);
		strcat(ErrorMessage[0], ErrorMessage[1]);
	}
	cpGetErrorInfo()->PrintError(1);
	return NULL;
}

real* EQUIPMENT_DYN_MODEL::GetnVarient(int no)
{
	if (no < 0 || no>9)
		return NULL;
	return &nVarient[no];
}

int EQUIPMENT_DYN_MODEL::ReadOutLine(char*Line)
{
	int i, flag;
	char outname[10][_MaxNameLen];
	flag = sscanf(Line, "%*[^,],%*[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,]",
		outname[0], outname[1], outname[2], outname[3], outname[4],
		outname[5], outname[6], outname[7], outname[8], outname[9]);
	DynVarient*tVar;
	for (i = 0; i < flag; i++)
	{
		ReplaceName(outname[i], _MaxNameLen);
		if (strcmp(outname[i], "ALL") == 0)
		{
			int j;
			for (j = 0; j < GetVarientTotal(); j++)
			{
				Varient[j].outflag = 1;
			}
			return 1;
		}
		tVar = GetDynVarient(outname[i]);
		if (tVar != NULL)
			tVar->outflag = 1;
	}
	return 1;
}


#include "DyFaultInfo.h"
#include "DyFaultLine.h"
#include "../CommonFunction/ErrorInfo.h"

DYFAULTINFO::DYFAULTINFO()
{
	iFaultListNo = 0;
	memset(pFaultList, 0, _MaxFaultNum*sizeof(DYFAULTBASE*));
	pDyn = NULL;
	NextTime = 99999.;
	NextFault = -1;
}

void DYFAULTINFO::RemoveAllFault()
{
	int i;
	for (i = 0; i < iFaultListNo; i++)
	{
		FreeArray(pFaultList[i]);
	}
	iFaultListNo = 0;
}

int DYFAULTINFO::AddNewFault(DYFAULTBASE*tFault)
{
	if (iFaultListNo >= _MaxDynamicModelNo)
	{
		return -1;
	}
	pFaultList[iFaultListNo] = tFault;
	iFaultListNo++;
	return iFaultListNo - 1;
}

float DYFAULTINFO::GetNextTime()
{
	if (NextFault < -1)
		return 99999.;
	int i;
	NextTime = 99999.;
	NextFault = -2;
	float tTime;
	for (i = 0; i < iFaultListNo; i++)
	{
		if (pFaultList[i]->GetIsProcess())
			continue;
		tTime = pFaultList[i]->GetNextTime();
		if (tTime < NextTime)
		{
			NextFault = i;
			NextTime = tTime;
		}
	}
	return NextTime;
}

int DYFAULTINFO::ProcessFault(float time)
{
	if (NextFault >= 0)
		pFaultList[NextFault]->ProcessFault();
	return 1;
}

int DYFAULTINFO::ReadFile(char*file)
{
	FILE* fpfile = NULL;
	OpenFile(fpfile, file, "rb");
	if (fpfile == NULL)
		return 0;
	int flag;
	char Line[_MaxLineLen], type[10];
	while (fgets(Line, _MaxLineLen, fpfile))
	{
		flag = sscanf(Line, "%9s", type);
		if (flag < 1)
			continue;
		if (strncmp(type, "-999", 4) == 0)
			break;
	}
	while (fgets(Line, _MaxLineLen, fpfile))
	{
		flag = sscanf(Line, "%9s", type);
		if (flag < 1)
			continue;
		if (strncmp(type, "-999", 4) == 0)
			break;
	}
	DYFAULTBASE*tFault;
	while (fgets(Line, _MaxLineLen, fpfile))
	{
		flag = sscanf(Line, "%9s", type);
		if (flag < 1)
			continue;
		if (type[0] == '/')
			continue;
		if (strncmp(type, "-999", 4) == 0)
			break;
		if (type[0] == 'F')
		{
			if (type[1] == 'L')
			{
				tFault = new DYFAULTLINE;
				flag = tFault->ReadLine(Line);
				if (flag != 1)
				{
					delete tFault;
					sprintf(ErrorMessage[0], "¹ÊÕÏÊý¾Ý¶ÁÈ¡Ê§°Ü£º%s", Line);
					cpGetErrorInfo()->PrintWarning(11, 1);
					continue;
				}
				flag = AddNewFault(tFault);
				if (flag < 0)
				{
					delete tFault;
					sprintf(ErrorMessage[0], "Ìí¼Ó¹ÊÕÏÊý¾ÝÊ§°Ü£º%s", Line);
					cpGetErrorInfo()->PrintWarning(12, 1);
					continue;
				}
			}
		}
	}
	cpGetErrorInfo()->CheckMessageType(11);
	cpGetErrorInfo()->CheckMessageType(12);
	return 1;
}

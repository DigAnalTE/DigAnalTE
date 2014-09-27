
#include "constdef.h"
#define _STR(x) #x
#define STR(x) _STR(x)
static char version_id[] =
"DigAnalTE Copyright (C) Ver "STR(ProgramVersion)"";
char *GetVersionStr(){ return version_id; }

bool ReplaceName(char* tName, int Length)
{
	char *ex;
	int i, j;
	//trimleft
	ex = tName; i = 0;
	while ((*ex == ' ' || *ex == 9) && i < Length - 1){ ex++; i++; }//找到开始的非空字符
	for (j = 0; j < Length - 1 && i < Length - 1; j++, ex++, i++)
	{
		if (*ex == '\0')break;
		tName[j] = *ex;
	}
	tName[j] = '\0';
	//trimright
	j--;
	for (; j >= 0; j--)
	{
		if (tName[j] == ' ' || tName[j] == '\r' || tName[j] == '\n' || tName[j] == 9)
		{
			tName[j] = '\0';
			continue;
		}
		break;
	}
	return true;
}

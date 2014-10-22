
#include "BPAReadWrite.h"
#include "../../DigAnalTE/DigAnalTE/CommonFunction/ErrorInfo.h"

void GetItemFromLine(char* Line, void *value, const char f_para[10], int f_loca)
{
	int i, j, flag_zero;
	char WORD[20];
	char F_MVARating[10];
	strncpy(F_MVARating, &f_para[1], 9); F_MVARating[9] = '\0';
	int length = (int)atof(F_MVARating);
#ifdef _DEBUG
	if (length > 19 || length <= 0)
	{
		sprintf(ErrorMessage[0], "ERROR: ��ʽ���ȳ����������Ҫ���,%s\n", f_para);
		cpGetErrorInfo()->PrintError(1);
	}
#endif
	if (strlen(Line) > (unsigned)f_loca)//20120331����һ���жϣ����ݳ����Ƿ�����
	{
		flag_zero = 0;//�����ǿո�֮��Ŀո���Ҫ����
		for (i = 0, j = 0; i < length; i++)
		{
			if (Line[f_loca + i] == '\0')break;
			if (Line[f_loca + i] == '\n')break;
			if (Line[f_loca + i] == '\r')break;
			if (f_para[0] != 'A' && Line[f_loca + i] == ' ')
			{//�����ַ�ʱ�������м���ֿո�
				if (flag_zero == 1)flag_zero = 2;
				continue;
			}
			if (flag_zero == 2)
			{
				sprintf(ErrorMessage[0], "���ݿ����ܴ�λ������ո�λ�ã�%s", Line);
				cpGetErrorInfo()->PrintWarning(1, 1);
			}
			WORD[j] = Line[f_loca + i];
			j++;
			flag_zero = 1;
		}
		WORD[j] = '\0';
	}
	else
		WORD[0] = '\0';
	if (f_para[0] == 'A')
	{
		char* cvalue = (char*)value;
		if (length == 1)
		{
			*cvalue = WORD[0];
		}
		else{
			strncpy(cvalue, WORD, length);
			for (i = strlen(cvalue); i < length; i++)
				cvalue[i] = ' ';
			cvalue[length] = '\0';
		}
	}
	else if (f_para[0] == 'I')
	{
		int* ivalue = (int*)value;
		if (WORD[0] == '\0')
		{
			WORD[0] = '0';
			WORD[1] = '\0';
		}
		*ivalue = atoi(WORD);
	}
	else if (f_para[0] == 'F')
	{
		float* fvalue = (float*)value;
		if (WORD[0] == '\0')
		{
			WORD[0] = '0';
			WORD[1] = '\0';
		}
		*fvalue = (float)atof(WORD);
		char *ex;
		if ((ex = strrchr(WORD, '.')) == NULL)
		{
			int nPoint;
			if ((ex = strrchr(F_MVARating, '.')) != NULL)
				nPoint = (-1)*atoi(++ex);
			else
				nPoint = 0;
			*fvalue = (float)((*fvalue)*pow(10.f, nPoint));
		}
	}
	else{
		sprintf(ErrorMessage[0], "ERROR: ��ʽ����F�ͻ�A�ͣ��������Ҫ���,%s\n", f_para);
		cpGetErrorInfo()->PrintError(1);
	}
}

void PutItemToLine(char* Line, void *value, const char f_para[10], int f_loca)
{
	int i, j, flagMinus = 0;
	char WORD[_MaxLineLen]; WORD[0] = '\0';
	char F_MVARating[10];
	strncpy(F_MVARating, &f_para[1], 9); F_MVARating[9] = '\0';
	int length, WORDlength;
	length = (int)atof(F_MVARating);
	if (length > 9 || length <= 0)
	{
		sprintf(ErrorMessage[0], "ERROR: ��ʽ���ȳ����������Ҫ���,%s\n", f_para);
		cpGetErrorInfo()->PrintError(1);
	}
	if (f_para[0] == 'A')
	{
		char* cvalue = (char*)value;
		if (IsBlank(cvalue, length))return;
		WORDlength = strlen(cvalue);
		if (WORDlength > length)
		{
			for (i = WORDlength - 1; i >= 0; i--)
			{
				if (cvalue[i] != ' ')
				{
					break;
				}
				WORDlength--;
			}
		}
		if (WORDlength > length)
		{
			sprintf(ErrorMessage[0], "ERROR: �������ݳ���Խ��, ��ʽ%s, ֵ%s\n", f_para, cvalue);
			cpGetErrorInfo()->PrintError(1);
		}
		strncpy(WORD, cvalue, WORDlength);
	}
	else if (f_para[0] == 'I')
	{
		int* ivalue = (int*)value;
		if (*ivalue == 0)return;
		sprintf(WORD, "%d", *ivalue);
		WORDlength = strlen(WORD);
		if (WORDlength > length)
		{
			sprintf(ErrorMessage[0], "ERROR: �������ݳ���Խ��, ��ʽ%s, ֵ%s\n", f_para, WORD);
			cpGetErrorInfo()->PrintError(1);
		}
	}
	else if (f_para[0] == 'F')
	{
		float fvalue = *(float*)value;
		if (fvalue < 0)
		{//��������ȡ������
			fvalue = -fvalue;
			flagMinus = 1;
			length--;
		}
		if (IsZero(fvalue))return;
		fvalue += (float)pow(.1f, length + 1);
		//(*fvalue)+=0.000000001;
		int nPoint, iLength;
		char *ex;
		sprintf(WORD, "%f", fvalue);
		WORDlength = strlen(WORD);
		if ((ex = strrchr(WORD, '.')) == NULL)
		{//׷��С����
			WORD[WORDlength] = '.';
			WORD[WORDlength + 1] = '\0';
		}
		else if (ex - WORD == 1 && WORD[0] == '0'){//���Ϊ0.���򽫵�һλɾ��
			for (i = 0; i<WORDlength; i++)
			{
				WORD[i] = WORD[i + 1];
			}
		}
		WORDlength = strlen(WORD);
		if (WORDlength>length + 2)
		{
			WORDlength = length + 2;
		}
		for (; WORDlength > 0; WORDlength--)
		{
			if (WORD[WORDlength - 1] != '0')
			{
				break;
			}
		}
		WORD[WORDlength] = '\0';
		if (WORDlength > length)
		{
			if ((ex = strrchr(WORD, '.')) != NULL)
			{
				iLength = ex - WORD;
			}
			else
				iLength = WORDlength;
			int tempi;
			if ((ex = strrchr(F_MVARating, '.')) != NULL)
				nPoint = atoi(++ex);
			else
				nPoint = 0;
			if (iLength > length || (iLength == length && nPoint > 0))//�������ֲ���Խ��
			{//����������Խ�磬��հ�
				return;
				//sprintf(ErrorMessage[0],"ERROR: �������ݳ���Խ��, ��ʽ%s, ֵ%s\n",f_para,WORD);
				//cpGetErrorInfo()->PrintError(1);
				for (iLength = 0; iLength < length; iLength++)
				{
					WORD[iLength] = '9';
				}
				WORD[iLength - 1] = '.';
				WORD[iLength] = '\0';
			}
			else{
				if (flagMinus == 0 && nPoint != 0 && iLength + nPoint == length)
				{//����д���൱�ڶ�дһλ �������ڸ��������
					tempi = (int)((fvalue)*pow(10.f, nPoint));
					sprintf(WORD, "%d", tempi);
				}
				WORDlength = length;
			}
		}
	}
	else{
		sprintf(ErrorMessage[0], "ERROR: ��ʽ����F�ͻ�A�ͣ��������Ҫ���,%s\n", f_para);
		cpGetErrorInfo()->PrintError(1);
	}
	if (flagMinus == 1)
	{
		Line[f_loca] = '-';
		f_loca++;
	}
	for (i = 0, j = 0; i < WORDlength; i++)
	{//д��ָ��λ��
		Line[f_loca + i] = WORD[i];
	}
}

bool IsBlank(char* Line, int len)
{
	int i, strl;
	strl = strlen(Line);
	if (len < strl)strl = len;
	for (i = 0; i < strl; i++)
	{
		if (Line[i] != ' ')
		{
			return false;
		}
	}
	return true;
}

void ResetBPAName(char*Name, float baseKV)
{
	int i;
	i = strlen(Name);
	for (; i < 8; i++)
	{
		Name[i] = ' ';
	}
	sprintf(&Name[8], "%-7.2f", baseKV);
	i = strlen(Name);
	for (; i < 15; i++)
	{
		Name[i] = ' ';
	}
	Name[15] = '\0';
}

void GenerateBPABranchName(char*Name, char*Name1, float baseKV1, char*Name2, float baseKV2, char ID)
{
	int i;
	sprintf(Name, "%-8s%-7.2f%-8s%-7.2f%c", Name1, baseKV1, Name2, baseKV2, ID);
}

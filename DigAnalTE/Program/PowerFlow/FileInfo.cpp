
#include "FileInfo.h"
#include "../../DigAnalTE/CommonFunction/ErrorInfo.h"
#include <io.h>
#include <direct.h>

FILEINFO::FILEINFO()
{
	if (_getcwd(CurrentDirection, _MAX_PATH_) == NULL)
		perror("_getcwd error");
	strcat(CurrentDirection, "\\");
	strcpy(ProgramDirection, CurrentDirection);
	strcpy(ProjectDirection, CurrentDirection);
	InputFileName[0] = '\0';
	OutputFileName[0] = '\0';
	//mfInputFile=NULL;
	mfOutputFile = NULL;
	LineTotal = 0;
	LineST = NULL;
}

FILEINFO::~FILEINFO()
{
	//CloseFile(mfInputFile);
	CloseFile(mfOutputFile);
	if (LineST != NULL)free(LineST);
}

void FILEINFO::ReadAllLines(char* endkey)
{
	LineTotal = 0;
	if (LineST != NULL)free(LineST);

	FILE*	fpPFfile;
	fpPFfile = fopen(InputFileName, "rb");
	if (fpPFfile == NULL)
		return;

	char Line[_MaxLineLen];
	while (fgets(Line, _MaxLineLen, fpPFfile))
		LineTotal++;
	rewind(fpPFfile);

	if (LineTotal == 0)
	{
		return;
	}

	MallocCharArray(LineST, (LineTotal + 4), _MaxLineLen);

	int i, flagAddLine, endkeylen;
	i = 0;
	flagAddLine = 0;
	endkeylen = 0; if (endkey != NULL)endkeylen = strlen(endkey);
	while (fgets(Line, _MaxLineLen, fpPFfile))
	{
		if (flagAddLine == 1)
		{
			if (strlen(Line) < _MaxLineLen - 1)
				flagAddLine = 0;
			continue;
		}
		if (strlen(Line) >= _MaxLineLen - 1)
		{
			if (Line[_MaxLineLen - 2] != 10 && Line[_MaxLineLen - 2] != 13)
			{//这里考虑到有些刚好_MaxLineLen-2个字符的行
				flagAddLine = 1;//表示该行还未读完
			}
		}
		if (endkeylen > 0 && strncmp(Line, endkey, endkeylen) == 0)
		{//终止符
			break;
		}
		strncpy(LineST[i], Line, _MaxLineLen);
		i++;
	}
	fclose(fpPFfile);

	LineTotal = i;
}

void FILEINFO::SetInputFileName(char* tempFile)
{
	strcpy(InputFileName, tempFile);
}

void FILEINFO::SetProgram(char* file)
{
	char*str;
	if (strchr(file, ':') == NULL)
	{
		strcpy(ProgramDirection, CurrentDirection);
		strcat(ProgramDirection, file);
	}
	else
		strcpy(ProgramDirection, file);
	if (_access(ProgramDirection, 0) == -1)
	{
		sprintf(ErrorMessage[0], "可执行程序*.exe不存在：%s\n", ProgramDirection);
		cpGetErrorInfo()->PrintError(1);
		return;
	}
	if ((str = strrchr(ProgramDirection, '\\')) != NULL || (str = strrchr(ProgramDirection, '/')) != NULL)
		str[1] = '\0';
}
void FILEINFO::InitInputFileName(char* temp)
{
	SetDefaultFileType(temp);
	if (InputFileName[0] != '\0')
	{
		GetInputFileNameFromCommandLine();
	}
	else
		GetInputFileNameFromScreen();
	int i, slen1, slen2, flag = 0;
	slen1 = strlen(temp);
	slen2 = strlen(InputFileName);
	if (slen1 < slen2)
	{
		for (i = 0; i < slen1; i++)
		{
			if (InputFileName[slen2 - slen1 + i] != temp[i] && abs(InputFileName[slen2 - slen1 + i] - temp[i]) != 32)
				break;
		}
		if (i >= slen1)
			flag = 1;
	}
	if (flag == 0)
	{
		sprintf(ErrorMessage[0], "文件%s的后缀名不符合要求%s，可能是错误的文件", InputFileName, InputFileType);
		cpGetErrorInfo()->PrintWarning(0, 1);
	}
	char*str;
	if (strchr(InputFileName, ':') == NULL)
	{
		strcpy(ProjectDirection, CurrentDirection);
		strcat(ProjectDirection, InputFileName);
	}
	else
		strcpy(ProjectDirection, InputFileName);
	if ((str = strrchr(ProjectDirection, '\\')) != NULL || (str = strrchr(ProjectDirection, '/')) != NULL)
		str[1] = '\0';
	return;
}

void FILEINFO::SetDefaultFileType(char* temp)
{
	InputFileType[0] = '\0';
	if (temp[0] != '.')
	{
		InputFileType[0] = '.';
		InputFileType[1] = '\0';
	}
	strcat(InputFileType, temp);
}

void FILEINFO::GetInputFileNameFromCommandLine()
{
	if (_access(InputFileName, 0) == -1)
	{
		sprintf(ErrorMessage[0], "File: %s doesn't exist.\n", InputFileName);
		strcat(InputFileName, InputFileType);
		if (_access(InputFileName, 0) == -1)
		{
			cpGetErrorInfo()->PrintError(1);
		}
	}
}

void FILEINFO::GetInputFileNameFromScreen()
{
	int iii, i, flag_pfl = 0;

	while (!flag_pfl)
	{
		for (i = 0; i < _MAX_PATH_; i++) InputFileName[i] = '\0';
		iii = 0;
		printf("Please input file name (*%s) >\n", InputFileType);
		for (;;)
		{
			if (iii >= _MAX_PATH_) break;
			scanf("%c", &InputFileName[iii]);
			if (InputFileName[iii] == 10) break;
			iii++;
		}
		InputFileName[iii] = '\0';
		if (_access(InputFileName, 0) == -1)
		{
			sprintf(ErrorMessage[0], "File: %s doesn't exist.\n", InputFileName);
			strcat(InputFileName, InputFileType);
			if (_access(InputFileName, 0) == -1)
			{
				printf(ErrorMessage[0]);
			}
		}
		else
			flag_pfl = 1;
	}
}

void FILEINFO::IntOutputFileName(char* temp)
{
	char *ex;
	strcpy(OutputFileName, InputFileName);
	ex = strrchr(OutputFileName, '.');
	if (ex != NULL)
	{
		*ex = '\0';
	}
	if (temp[0] != '.')
	{
		strcat(OutputFileName, ".");
	}
	strcat(OutputFileName, temp);
	OpenFile(mfOutputFile, OutputFileName, "w+");
}

int FILEINFO::SetProjectDirection(char* file)
{
	char tempfile[_MaxLineLen];
	if (strchr(file, ':') == NULL)
	{
		strcpy(tempfile, ProjectDirection);
	}
	else
		tempfile[0] = '\0';
	strcat(tempfile, file);
	if (_access(tempfile, 0) == -1)
	{
		return 0;
	}
	strcpy(file, tempfile);
	return 1;
}

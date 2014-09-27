
#include "ErrorInfo.h"
#include "constdef.h"

char ErrorMessage[10][_ERROR_MESSAGE_LENGTH];
ERROR_MESG_INFO gErrorInfo;
ERROR_MESG_INFO* cpGetErrorInfo(){ return &gErrorInfo; }

void ERROR_MESG_INFO::PrintWarning(int Type, int NBL)
{
	int i, slen;
	if (Type >= 0 && IsStoreMessage && MessageCount < MaxMessage)
		ErrorType[Type]++;
	for (i = 0; i < NBL; i++)
	{
		//首先处理一下，将消息的后缀都加上回车
		slen = strlen(ErrorMessage[i]);
		if (ErrorMessage[i][slen - 1] != '\n')
		{
			ErrorMessage[i][slen] = '\n';
			ErrorMessage[i][slen + 1] = '\0';
		}
		else if (slen >= 2 && ErrorMessage[i][slen - 2] == '\r'){
			ErrorMessage[i][slen - 2] = '\n';//windows版本不能包含'\r'
			ErrorMessage[i][slen - 1] = '\0';
		}
		PrintOut(ErrorMessage[i]);
		if (Type >= 0 && IsStoreMessage && MessageCount < MaxMessage)
		{
			strcpy(StoredMessage[MessageCount], ErrorMessage[i]);
			MessageCount++;
		}
	}
	if (MessageCount >= MaxMessage)
	{
		sprintf(ErrorMessage[0], "More Than %d Warnings. Program Aborted.", MaxMessage);
		PrintOut(ErrorMessage[0]);
		StopProgram();
		ClearWarning();
	}
}

void ERROR_MESG_INFO::CheckMessageType(int Type)
{
	if (ErrorType[Type] > 0)
	{
		sprintf(ErrorMessage[0], "There are %d Errors. Program Aborted:\n", ErrorType[Type]);
		printf(ErrorMessage[0]);
		if (fpFile != NULL)
		{
			fprintf(fpFile, ErrorMessage[0]);
		}
		PrintAllWarningToScreen();
		StopProgram();
	}
}

void ERROR_MESG_INFO::PrintError(int NBL)
{
	int i, slen;
	for (i = 0; i < NBL; i++)
	{
		slen = strlen(ErrorMessage[i]);
		if (ErrorMessage[i][slen - 1] != '\n')
		{
			ErrorMessage[i][slen] = '\n';
			ErrorMessage[i][slen + 1] = '\0';
		}
		printf(ErrorMessage[i]);
		if (fpFile != NULL)
		{
			fprintf(fpFile, ErrorMessage[i]);
		}
		if (IsStoreMessage && MessageCount < MaxMessage)
		{
			strcpy(StoredMessage[MessageCount], ErrorMessage[i]);
			MessageCount++;
		}
	}
	StopProgram();
}

void ERROR_MESG_INFO::PrintOut(char* massage)
{
	if (IsScreenMessage>0)
	{
		printf(massage);
	}
	if (fpFile != NULL)
	{
		fprintf(fpFile, massage);
	}
}

void ERROR_MESG_INFO::PrintAllWarning(FILE*fp)
{
	if (fp == NULL)return;
	int i;
	fprintf(fp, "\n  <TABLE: 警告信息>\n");
	for (i = 0; i < MessageCount; i++)
	{
		fprintf(fp, StoredMessage[i]);
	}
	fprintf(fp, "  </TABLE: 警告信息>\n");
}

void ERROR_MESG_INFO::PrintAllWarningToScreen()
{
	int i;
	for (i = 0; i < MessageCount; i++)
	{
		printf(StoredMessage[i]);
	}
}

void ERROR_MESG_INFO::PassProgram()
{
}

void ERROR_MESG_INFO::StopProgram()
{
	exit(0);
}
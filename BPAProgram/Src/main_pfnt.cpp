
#include "BPANetWorkInfo.h"
#include "../../DigAnalTE/DigAnalTE/PowerFlowLib/PowerFlowFunction.h"

#include <io.h>
#define _MAX_PATH_ 256

#ifdef _DEBUG
#	pragma comment(lib,"../DigAnalTE/DigAnalTE/Lib/DigAnalTED.lib")
#else
#	pragma comment(lib,"../DigAnalTE/DigAnalTE/Lib/DigAnalTE.lib")
#endif

BPA_NETWORKINFO gNetWorkInfo;

int main(int argc, char **argv)
{
	char InputFileName[_MAX_PATH_];
	char OutputFileName[_MAX_PATH_];
	FILE* mfOutputFile = NULL;
	if (argc > 1)
	{
		strcpy(InputFileName, argv[1]);
	}
	else{
		printf("�������볱��DAT�ļ�");
		printf("press Enter to exit\n");
		getchar();
		return 0;
	}
	if (_access(InputFileName, 0) == -1)
	{
		printf("File: %s doesn't exist.\n", InputFileName);
		printf("press Enter to exit\n");
		getchar();
		return 0;
	}
	strcpy(OutputFileName, InputFileName);
	char *ex;
	ex = strrchr(OutputFileName, '.');
	if (ex != NULL)
	{
		*ex = '\0';
	}
	strcat(OutputFileName, ".pfo");
	OpenFile(mfOutputFile, OutputFileName, "w+");

	cpGetErrorInfo()->SetMessageFile(mfOutputFile);//����FILE�����д�����Ϣ�Ϳ��Լ�¼������ļ�����

	sprintf(ErrorMessage[0], "��ӭʹ�ó����������");
	sprintf(ErrorMessage[1], GetVersionStr());
	sprintf(ErrorMessage[2], "\n");
	cpGetErrorInfo()->PrintWarning(-1, 3);

	int flag;
	flag = gNetWorkInfo.ReadFile(InputFileName);
	if (flag != 1)
		return 0;

	gNetWorkInfo.NetLink();
	gNetWorkInfo.NetAnalysis();

	if (gNetWorkInfo.NetError != 1)
	{
		sprintf(ErrorMessage[0], "���ݴ��ڴ��󣬲��ܽ��г�������");
		cpGetErrorInfo()->PrintError(1);
		return 0;
	}

	//��������
	if (PowerFlowCal(&gNetWorkInfo) != 1)
	{
		return 0;
	}

	gNetWorkInfo.OutputPFOFile(mfOutputFile);

	return 0;
}

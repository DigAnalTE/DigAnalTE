
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
		printf("必须输入潮流DAT文件");
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

	cpGetErrorInfo()->SetMessageFile(mfOutputFile);//设置FILE后，所有错误信息就可以记录在这个文件中了

	sprintf(ErrorMessage[0], "欢迎使用潮流计算程序");
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
		sprintf(ErrorMessage[0], "数据存在错误，不能进行潮流计算");
		cpGetErrorInfo()->PrintError(1);
		return 0;
	}

	//潮流计算
	if (PowerFlowCal(&gNetWorkInfo) != 1)
	{
		return 0;
	}

	gNetWorkInfo.OutputPFOFile(mfOutputFile);

	return 0;
}

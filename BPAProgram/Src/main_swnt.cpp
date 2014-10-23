
#include "BPADynamicModelInfo.h"
#include "BPAFault.h"
#include "../../DigAnalTE/DigAnalTE/PowerFlowLib/PowerFlowFunction.h"
#include "../../DigAnalTE/DigAnalTE/StabilityAnalysis/TimeDomainSimulation.h"

#include <io.h>
#define _MAX_PATH_ 256

#ifdef _DEBUG
#	pragma comment(lib,"../DigAnalTE/DigAnalTE/Lib/DigAnalTED.lib")
#else
#	pragma comment(lib,"../DigAnalTE/DigAnalTE/Lib/DigAnalTE.lib")
#endif

BPA_DYNAMICMODELINFO gDynWorkInfo;
BPA_FAULTINFO gFault;

int main(int argc, char **argv)
{
	char DATFileName[_MAX_PATH_];
	char SWIFileName[_MAX_PATH_];
	char OutFileName[_MAX_PATH_];
	char CurFileName[_MAX_PATH_];
	FILE* mfOutputFile = NULL;
	if (argc > 2)
	{
		strcpy(DATFileName, argv[1]);
		strcpy(SWIFileName, argv[2]);
	}
	else{
		printf("必须输入潮流DAT文件和暂态SWI文件");
		printf("press Enter to exit\n");
		getchar();
		return 0;
	}
	if (_access(DATFileName, 0) == -1)
	{
		printf("File: %s doesn't exist.\n", DATFileName);
		printf("press Enter to exit\n");
		getchar();
		return 0;
	}
	if (_access(SWIFileName, 0) == -1)
	{
		printf("File: %s doesn't exist.\n", SWIFileName);
		printf("press Enter to exit\n");
		getchar();
		return 0;
	}
	strcpy(OutFileName, SWIFileName);
	char *ex;
	ex = strrchr(OutFileName, '.');
	if (ex != NULL)
	{
		*ex = '\0';
	}
	strcpy(CurFileName, OutFileName);
	strcat(OutFileName, "-DigAnalTE.out");
	strcat(CurFileName, "-DigAnalTE.txt");//BPA的cur文件有特殊格式
	OpenFile(mfOutputFile, OutFileName, "w+");

	cpGetErrorInfo()->SetMessageFile(mfOutputFile);//设置FILE后，所有错误信息就可以记录在这个文件中了

	sprintf(ErrorMessage[0], "欢迎使用暂态计算程序");
	sprintf(ErrorMessage[1], GetVersionStr());
	sprintf(ErrorMessage[2], "\n");
	cpGetErrorInfo()->PrintWarning(-1, 3);

	int flag;
	flag = gDynWorkInfo.ReadBPAFile(DATFileName, SWIFileName);
	if (flag != 1)
		return 0;

	gDynWorkInfo.NetLink();
	gDynWorkInfo.NetAnalysis();

	if (gDynWorkInfo.NetError != 1)
	{
		sprintf(ErrorMessage[0], "数据存在错误，不能进行潮流计算");
		cpGetErrorInfo()->PrintError(1);
		return 0;
	}

	flag = gFault.ReadFile(SWIFileName);
	if (flag != 1)
	{
		sprintf(ErrorMessage[0], "故障数据读取失败，请检查数据文件");
		cpGetErrorInfo()->PrintError(1);
		return 0;
	}

	//潮流计算
	if (PowerFlowCal(&gDynWorkInfo) != 1)
	{
		return 0;
	}

	flag = gDynWorkInfo.DynInitial();
	if (flag != 1)
	{
		sprintf(ErrorMessage[0], "动态模型初始化失败，退出");
		cpGetErrorInfo()->PrintError(1);
		return 0;
	}

	TDSIMULATION tSimulator;
	tSimulator.SetDynModel(&gDynWorkInfo);
	flag = tSimulator.CheckInitalCalcul();
	if (flag != 1)
	{
		sprintf(ErrorMessage[0], "初始化计算失败，退出");
		cpGetErrorInfo()->PrintError(1);
		return 0;
	}
	flag = tSimulator.Calculate(&gFault);
	if (flag != 1)
	{
		sprintf(ErrorMessage[0], "暂态计算失败，退出");
		cpGetErrorInfo()->PrintError(1);
		return 0;
	}
	tSimulator.WriteCurve(CurFileName);

	return 0;
}

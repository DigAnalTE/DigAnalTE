
#include "../../DigAnalTE/CommonFunction/constdef.h"
#include "../../DigAnalTE/CommonFunction/ErrorInfo.h"
#include "../../DigAnalTE/StabilityAnalysis/DyFaultInfo.h"
#include "../../DigAnalTE/DynamicModel/DynamicModelInfo.h"
#include "../../DigAnalTE/PowerFlowLib/PowerFlowFunction.h"
#include "../../DigAnalTE/StabilityAnalysis/TimeDomainSimulation.h"
#include <io.h>
#include <direct.h>
#define _MAX_PATH_ 256

#ifdef _DEBUG
#	pragma comment(lib,"../../DigAnalTE/Lib/DigAnalTED.lib")
#else
#	pragma comment(lib,"../../DigAnalTE/Lib/DigAnalTE.lib")
#endif

DYNAMICMODELINFO gDynWorkInfo;
DYFAULTINFO gFault;

int main(int argc, char **argv)
{
	char InputFileName[_MAX_PATH_];
	char OutputFileName[_MAX_PATH_];
	char CurveFileName[_MAX_PATH_];
	FILE* mfOutputFile = NULL;
	if (argc > 1)
	{
		strcpy(InputFileName, argv[1]);
	}
	else{
		printf("必须输入数据DAT文件");
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
	strcpy(CurveFileName, OutputFileName);
	strcat(OutputFileName, "-DigAnalTE.out");
	strcat(CurveFileName, "-DigAnalTE.cur");
	OpenFile(mfOutputFile, OutputFileName, "w+");
	cpGetErrorInfo()->SetMessageFile(mfOutputFile);

	sprintf(ErrorMessage[0], "欢迎使用稳定计算程序");
	sprintf(ErrorMessage[1], GetVersionStr());
	sprintf(ErrorMessage[2], "\n");
	cpGetErrorInfo()->PrintWarning(-1, 3);

	int flag;
	flag = gDynWorkInfo.ReadFile(InputFileName);
	if (flag != 1)
	{
		sprintf(ErrorMessage[0], "数据读取失败，请检查数据文件");
		cpGetErrorInfo()->PrintError(1);
		return 0;
	}

	gDynWorkInfo.NetLink();
	gDynWorkInfo.NetAnalysis();

	if (gDynWorkInfo.NetError != 1)
	{
		sprintf(ErrorMessage[0], "数据存在错误，不能进行潮流计算");
		cpGetErrorInfo()->PrintError(1);
		return 0;
	}

	flag = gFault.ReadFile(InputFileName);
	if (flag != 1)
	{
		sprintf(ErrorMessage[0], "故障数据读取失败，请检查数据文件");
		cpGetErrorInfo()->PrintError(1);
		return 0;
	}

	//潮流计算
	if (PowerFlowCal(&gDynWorkInfo) != 1)
	{
		sprintf(ErrorMessage[0], "潮流计算失败，退出");
		cpGetErrorInfo()->PrintError(1);
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
	tSimulator.WriteCurve(CurveFileName);

	return 0;
}

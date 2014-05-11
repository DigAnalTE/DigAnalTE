
#include "FileInfo.h"
#include "../../DigAnalTE/NetWorkInfo/NetWorkInfo.h"
#include "../../DigAnalTE/PowerFlowLib/PowerFlowFunction.h"

#ifdef _DEBUG
#	pragma comment(lib,"../../DigAnalTE/Lib/DigAnalTED.lib")
#else
#	pragma comment(lib,"../../DigAnalTE/Lib/DigAnalTE.lib")
#endif

NETWORKINFO gNetWorkInfo;

int main(int argc,char **argv)
{
	FILEINFO gFileInfo;
	if(argc>1)
	{
		gFileInfo.SetInputFileName(argv[1]);
	}
	gFileInfo.InitInputFileName("raw");
	gFileInfo.IntOutputFileName("pfos");
	cpGetErrorInfo()->SetMessageFile(gFileInfo.GetOutFile());

	sprintf(ErrorMessage[0],"欢迎使用潮流计算程序");
	sprintf(ErrorMessage[1],GetVersionStr());
	sprintf(ErrorMessage[2],"\n");
	cpGetErrorInfo()->PrintWarning(-1,3);
	
	int flag;
	flag = gNetWorkInfo.ReadFile(gFileInfo.GetInputFileName());
	if(flag!=1)
		return 0;

	gNetWorkInfo.NetLink();
	gNetWorkInfo.NetAnalysis();
	
	if (gNetWorkInfo.NetError!=1)
	{
		sprintf(ErrorMessage[0],"数据存在错误，不能进行潮流计算");
		cpGetErrorInfo()->PrintError(1);
		return 0;
	}

	PFCalPara tCalPara;
	tCalPara.MaxIter=50;
	tCalPara.BusPMaxErr=0.005f;
	tCalPara.BusQMaxErr=0.005f;
	
	PFSOLUTION_BASE* tPFCal = makeNRSolver(&gNetWorkInfo);
	if (tPFCal->Calculate(tCalPara)!=1)
	{
		return 0;
	}

	gNetWorkInfo.OutputPFOFile(gFileInfo.GetOutFile());

	return 0;
}

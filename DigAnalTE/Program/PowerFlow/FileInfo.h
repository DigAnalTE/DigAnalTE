
#ifndef __FILEINFO_H__
#define __FILEINFO_H__

//文件操作
//为了方便使用，提供一个功能较全的FILE类
//在熟悉该类使用方法后，用户可以自行选择是否使用

#include "../../DigAnalTE/CommonFunction/constdef.h"

#define _MAX_PATH_ 256

class FILEINFO
{
public:
	FILEINFO();
	virtual ~FILEINFO();

protected://目录，将所有地址都转换成全地址
	char CurrentDirection[_MAX_PATH_];
	char ProgramDirection[_MAX_PATH_];
	char ProjectDirection[_MAX_PATH_];
protected:
	char InputFileType[_MAX_PATH_];
	char InputFileName[_MAX_PATH_];
	char OutputFileName[_MAX_PATH_];
	void SetDefaultFileType(char*);
	void GetInputFileNameFromCommandLine();
	void GetInputFileNameFromScreen();
public:
	//FILE* mfInputFile;
	FILE* mfOutputFile;
	FILE* GetOutFile(){ return mfOutputFile; }
	FILE** ppGetOutFile(){ return &mfOutputFile; }
public:
	void SetProgram(char*);
	void SetInputFileName(char*);
	void InitInputFileName(char*);
	char* GetInputFileName(){ return InputFileName; }
	void IntOutputFileName(char*);
	char* GetOutputFileName(){ return OutputFileName; }
	int SetProjectDirection(char*);
public://读入
	int LineTotal;
	char(*LineST)[_MaxLineLen];
	void ReadAllLines(char* endkey = NULL);//将文件读入进来，endkey为结束符
};

#endif

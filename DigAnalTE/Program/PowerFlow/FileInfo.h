
#ifndef __FILEINFO_H__
#define __FILEINFO_H__

//�ļ�����
//Ϊ�˷���ʹ�ã��ṩһ�����ܽ�ȫ��FILE��
//����Ϥ����ʹ�÷������û���������ѡ���Ƿ�ʹ��

#include "../../DigAnalTE/CommonFunction/constdef.h"

#define _MAX_PATH_ 256

class FILEINFO
{
public:
	FILEINFO();
	virtual ~FILEINFO();

protected://Ŀ¼�������е�ַ��ת����ȫ��ַ
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
public://����
	int LineTotal;
	char(*LineST)[_MaxLineLen];
	void ReadAllLines(char* endkey = NULL);//���ļ����������endkeyΪ������
};

#endif


#ifndef __ERROR_MESG_INFO_H__
#define __ERROR_MESG_INFO_H__

//�����Ǵ�������
//�ں��˴�����Ĵ�������
//Ŀǰ�����ж�ʹ���˸���Դ�����д���
//�û����Ը����Լ����������¶�cpp�ļ��е�ʵ�ַ��������޸�

//����ʹ�÷�����
//���ȶ�ErrorMessage���鸳ֵ��Ȼ����ö�Ӧ��PrintError��PrintWarning���
//1 ���治�˳���PrintWarning(0,*)���ྯ��ᱣ��������������������MaxMessage֮���˳�
//2 �Ⱦ��棬�����඼������֮���ٸ��ݾ�����ѡ���˳���Ŀ�ģ��������������������Ȼ�����˳�
//  �ڷ��ִ����ʱ��PrintWarning(k,*)  kΪ���ͣ��������0
//  һ�δ������֮��CheckMessageType(k)
//3 ���ش��󣬱��������˳���PrintError(*)
//4 ֻ�Ǵ�ӡ��ʾ�����㾯�棬���浽�����б��У�PrintWarning(-1,*)
//  �����PrintOut(char*)
//*��ʾ��ǰʹ�õ�ErrorMessage�ĸ���

#include "constdef.h"

const int MaxMessage=1000;//MESSAGE��Ҫ�ۻ�һ������
const int MaxMessageType=20;//20�ִ���
#define _ERROR_MESSAGE_LENGTH 250

extern char ErrorMessage[10][_ERROR_MESSAGE_LENGTH];

class ERROR_MESG_INFO
{
public:
	ERROR_MESG_INFO()
	{
		MessageCount=0;
		memset(StoredMessage,0,MaxMessage*_ERROR_MESSAGE_LENGTH);
		IsStoreMessage=1;
		IsScreenMessage=1;
		memset(ErrorType,0,MaxMessageType*sizeof(int));
	}
	virtual ~ERROR_MESG_INFO(){}
private:
	int ErrorType[MaxMessageType];
	int MessageCount;
	char StoredMessage[MaxMessage][_ERROR_MESSAGE_LENGTH];
private:
	FILE *fpFile;       //==NULLʱ��������ļ�
	int IsScreenMessage;//������Ϣ�Ƿ��ӡ����Ļ//������Ϣǿ�ƴ�ӡ
	int IsStoreMessage; //������Ϣ�Ƿ�洢����  //������Ϣǿ�ƴ�ӡ
public://�����״̬����Щ״̬������ʵʱ��ӡ
	void SetMessageFile(FILE*fp){fpFile=fp;}
	void SetScreenMessage(int YesNo){IsScreenMessage=YesNo;}
	void SetStoreMessage(int YesNo){IsStoreMessage=YesNo;}
	FILE* GetErrorFile(){return fpFile;}
	int GetScreenMessage(){return IsScreenMessage;}
	int GetStoreMessage(){return IsStoreMessage;}
public:
	int GetMessageTotal(){return MessageCount;}
	void GetMessage(int Index,char* massage){strcpy(massage,StoredMessage[Index]);}
	void PrintWarning(int,int);//��һ������Ϊ�������ͣ��ڶ�������Ϊ��ϢERRBUF�ĸ���
	void PrintError(int);//����Ϊ��ϢERRBUF�ĸ���//��������Ϊ0
	void PrintOut(char*);
	void CheckMessageType(int);//�����Ϣ���ͣ��������ʹ��ڴ���ʱ���˳�
	void PrintAllWarning(FILE*fp);//ͳһ��ӡ���д���ľ�����Ϣ
	void PrintAllWarningToScreen();
	void ClearWarning(){MessageCount=0;}
public:
	void PassProgram();
	void StopProgram();
};
ERROR_MESG_INFO* cpGetErrorInfo();

#endif

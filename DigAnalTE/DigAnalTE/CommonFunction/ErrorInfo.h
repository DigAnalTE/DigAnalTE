
#ifndef __ERROR_MESG_INFO_H__
#define __ERROR_MESG_INFO_H__

//该类是错误处理类
//融合了大多数的错误处理方法
//目前代码中都使用了该类对错误进行处理
//用户可以根据自己的需求，重新对cpp文件中的实现方法进行修改

//该类使用方法：
//首先对ErrorMessage数组赋值，然后调用对应的PrintError或PrintWarning输出
//1 警告不退出：PrintWarning(0,*)此类警告会保存下来，当警告数超过MaxMessage之后退出
//2 先警告，将此类都分析完之后，再根据警告数选择退出，目的：将此类错误都搜索出来，然后再退出
//  在发现错误的时候：PrintWarning(k,*)  k为类型，必须大于0
//  一段代码结束之后：CheckMessageType(k)
//3 严重错误，必须马上退出：PrintError(*)
//4 只是打印提示，不算警告，不存到警告列表中：PrintWarning(-1,*)
//  或调用PrintOut(char*)
//*表示当前使用的ErrorMessage的个数

#include "constdef.h"

const int MaxMessage=1000;//MESSAGE需要累积一定数量
const int MaxMessageType=20;//20种错误
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
	FILE *fpFile;       //==NULL时不输出到文件
	int IsScreenMessage;//警告信息是否打印到屏幕//错误信息强制打印
	int IsStoreMessage; //警告信息是否存储起来  //错误信息强制打印
public://警告的状态。这些状态控制了实时打印
	void SetMessageFile(FILE*fp){fpFile=fp;}
	void SetScreenMessage(int YesNo){IsScreenMessage=YesNo;}
	void SetStoreMessage(int YesNo){IsStoreMessage=YesNo;}
	FILE* GetErrorFile(){return fpFile;}
	int GetScreenMessage(){return IsScreenMessage;}
	int GetStoreMessage(){return IsStoreMessage;}
public:
	int GetMessageTotal(){return MessageCount;}
	void GetMessage(int Index,char* massage){strcpy(massage,StoredMessage[Index]);}
	void PrintWarning(int,int);//第一个参数为警告类型，第二个参数为消息ERRBUF的个数
	void PrintError(int);//参数为消息ERRBUF的个数//参数可以为0
	void PrintOut(char*);
	void CheckMessageType(int);//检查信息类型，当该类型存在错误时，退出
	void PrintAllWarning(FILE*fp);//统一打印所有储存的警告信息
	void PrintAllWarningToScreen();
	void ClearWarning(){MessageCount=0;}
public:
	void PassProgram();
	void StopProgram();
};
ERROR_MESG_INFO* cpGetErrorInfo();

#endif


#ifndef __DigAnalTE_CONSTDEF_H__
#define __DigAnalTE_CONSTDEF_H__

#define ProgramVersion 1.0 //本工具的版本号
//表头
#define _CRT_SECURE_NO_WARNINGS

#include <string.h>
#include <stddef.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
char *GetVersionStr();

//网络大小
#define _MaxBusNo      300
#define _MaxBranchNo   (5*_MaxBusNo)
#define _MaxEquipNo    (_MaxBusNo)
#define _MaxZoneNo     1200
#define _MaxAreaNo     100
#define _MaxAreaZoneNo 40 //每个Area中Zone的个数
#define _MaxIslandNo   ((int)(_MaxBusNo/100)+10)

//所有的元件都有一个标示符，就是名称
#define _MaxNameLen 256

//定义精度
//#define _DOUBLE_PRECISION
#ifdef _DOUBLE_PRECISION
typedef double real;
#	ifndef pi
#		define pi (3.1415926)
#	endif
#else
typedef float real;
#	ifndef pi
#		define pi (3.1415926f)
#	endif
#endif

#define _MaxLineLen 1024 //输出字符串的长度//规范长度，派生类的输出长度不能长于该值//若字符串长度过小，可能出现错误

//文件操作
#define OpenFile(fp,filename,type) \
{\
if(filename==NULL){printf("打开文件时文件名不能为空");printf("press Enter to exit\n");getchar();exit(0);}\
if(fp!=NULL){printf("文件指针在打开新文件前必须赋为NULL，或者该指针已经指向了某文件");printf("press Enter to exit\n");getchar();exit(0);}\
if((fp=fopen (filename,type))==NULL)\
{\
	printf("cannot create file: %s",filename);\
	exit(1);\
}\
}
#define CloseFile(fp) {if(fp!=NULL)fclose(fp);fp=NULL;}

//空间操作
#define MallocNew(a,type,no) {if(a==NULL){ a=new type[(no)]; if(a==NULL) { printf("a out of memory\n");exit(1);}}}
#define	MallocNew2D(a,type,no1,no2){if(a==NULL){ a=(type (*)[no2]) malloc((no1)*no2*sizeof(type));  if(a==NULL){printf("a out of memory\n");exit(1);}}}

#define FreeArray(a) {if(a!=NULL){delete[] a;}a=NULL;}
#define FreeArray2D(a) {if(a!=NULL){delete[] a;}a=NULL;}

//分配2维字符数组，Name[n][MaxNameLen];
#define	MallocCharArray(Name,n,MaxNameLen)  {Name=(char (*)[MaxNameLen]) malloc((n)*MaxNameLen*sizeof(char));  if(Name==NULL){printf("Name2D:out of memory.");_press_enter_exit;}}

//常用函数
#define minv(x,y) ( (x>y)?y:x)
#define maxv(x,y) ( (x>y)?x:y)
extern "C"{
	//使用同一个函数避免转换带来的问题
	bool ReplaceName(char* tName,int Length);//处理BusName中的空格和长度等问题
}

#endif


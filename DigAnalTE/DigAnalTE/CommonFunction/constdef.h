
#ifndef __DigAnalTE_CONSTDEF_H__
#define __DigAnalTE_CONSTDEF_H__

#define ProgramVersion 1.0 //�����ߵİ汾��
//��ͷ
#define _CRT_SECURE_NO_WARNINGS

#include <string.h>
#include <stddef.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
char *GetVersionStr();

//�����С
#define _MaxBusNo      300
#define _MaxBranchNo   (5*_MaxBusNo)
#define _MaxEquipNo    (_MaxBusNo)
#define _MaxZoneNo     1200
#define _MaxAreaNo     100
#define _MaxAreaZoneNo 40 //ÿ��Area��Zone�ĸ���
#define _MaxIslandNo   ((int)(_MaxBusNo/100)+10)

//���е�Ԫ������һ����ʾ������������
#define _MaxNameLen 256

//���徫��
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

#define _MaxLineLen 1024 //����ַ����ĳ���//�淶���ȣ��������������Ȳ��ܳ��ڸ�ֵ//���ַ������ȹ�С�����ܳ��ִ���

//�ļ�����
#define OpenFile(fp,filename,type) \
{\
if(filename==NULL){printf("���ļ�ʱ�ļ�������Ϊ��");printf("press Enter to exit\n");getchar();exit(0);}\
if(fp!=NULL){printf("�ļ�ָ���ڴ����ļ�ǰ���븳ΪNULL�����߸�ָ���Ѿ�ָ����ĳ�ļ�");printf("press Enter to exit\n");getchar();exit(0);}\
if((fp=fopen (filename,type))==NULL)\
{\
	printf("cannot create file: %s",filename);\
	exit(1);\
}\
}
#define CloseFile(fp) {if(fp!=NULL)fclose(fp);fp=NULL;}

//�ռ����
#define MallocNew(a,type,no) {if(a==NULL){ a=new type[(no)]; if(a==NULL) { printf("a out of memory\n");exit(1);}}}
#define	MallocNew2D(a,type,no1,no2){if(a==NULL){ a=(type (*)[no2]) malloc((no1)*no2*sizeof(type));  if(a==NULL){printf("a out of memory\n");exit(1);}}}

#define FreeArray(a) {if(a!=NULL){delete[] a;}a=NULL;}
#define FreeArray2D(a) {if(a!=NULL){delete[] a;}a=NULL;}

//����2ά�ַ����飬Name[n][MaxNameLen];
#define	MallocCharArray(Name,n,MaxNameLen)  {Name=(char (*)[MaxNameLen]) malloc((n)*MaxNameLen*sizeof(char));  if(Name==NULL){printf("Name2D:out of memory.");_press_enter_exit;}}

//���ú���
#define minv(x,y) ( (x>y)?y:x)
#define maxv(x,y) ( (x>y)?x:y)
extern "C"{
	//ʹ��ͬһ����������ת������������
	bool ReplaceName(char* tName,int Length);//����BusName�еĿո�ͳ��ȵ�����
}

#endif


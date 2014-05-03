
#ifndef __DigAnalTE_HASHTABLE_H__
#define __DigAnalTE_HASHTABLE_H__

//使用方法：
//1、定义HASHTABLE对象
//2、调用InitHashTable函数
//3、使用InsertElementToHashTable插入元素
//4、使用ElementSearch查找元素
#include "constdef.h"

struct HASHTABLENODE
{
	char* sKey;
	int Index;
};

class HASHTABLE
{
public:
	HASHTABLE()
	{
		IsInit=0;
		HashTableSize=0;iHashTotal=0;
		gHashTable=NULL;
	}
	~HASHTABLE()
	{
		FreeTable();
	}
public:
	int InitHashTable(int ElementTotal);//默认HASH表的空间为10*ElementTotal
	void FreeTable();//释放空间
	int InsertElementToHashTable( int index, const char *lpszString );//插入元素//返回值：-1=成功 非负数=失败(返回相同元素的index)
	int ElementSearch( const char *lpszString );//查找元素//返回值：非负数=找到 -1=没找到 -2=多于一个符合条件
private:
	int IsInit;//必须先使用InitHashTable初始化，然后才能使用函数InsertElementToHashTable和ElementSearch
	int HashTableSize;//HashTable大小
	int iHashTotal;//HashTable使用了多少//这两个值会对HASHTABLE利用率进行检查，如果利用率超过一半，将发生错误
	HASHTABLENODE *gHashTable;
	unsigned long HashString( const char *lpszFileName );
};

#endif

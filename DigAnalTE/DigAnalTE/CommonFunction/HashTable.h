
#ifndef __DigAnalTE_HASHTABLE_H__
#define __DigAnalTE_HASHTABLE_H__

//ʹ�÷�����
//1������HASHTABLE����
//2������InitHashTable����
//3��ʹ��InsertElementToHashTable����Ԫ��
//4��ʹ��ElementSearch����Ԫ��
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
	int InitHashTable(int ElementTotal);//Ĭ��HASH��Ŀռ�Ϊ10*ElementTotal
	void FreeTable();//�ͷſռ�
	int InsertElementToHashTable( int index, const char *lpszString );//����Ԫ��//����ֵ��-1=�ɹ� �Ǹ���=ʧ��(������ͬԪ�ص�index)
	int ElementSearch( const char *lpszString );//����Ԫ��//����ֵ���Ǹ���=�ҵ� -1=û�ҵ� -2=����һ����������
private:
	int IsInit;//������ʹ��InitHashTable��ʼ����Ȼ�����ʹ�ú���InsertElementToHashTable��ElementSearch
	int HashTableSize;//HashTable��С
	int iHashTotal;//HashTableʹ���˶���//������ֵ���HASHTABLE�����ʽ��м�飬��������ʳ���һ�룬����������
	HASHTABLENODE *gHashTable;
	unsigned long HashString( const char *lpszFileName );
};

#endif

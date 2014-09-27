
#include "constdef.h"
#include "HashTable.h"
#include "ErrorInfo.h"

//以下函数计算lpszFileName 字符串的hash值
unsigned long HASHTABLE::HashString(const char *lpszFileName)
{
	const char *key = lpszFileName;
	unsigned long h = 0;
	while (*key != 0)
	{
		h = (h << 5) - h + *key;
		key++;
	}
	return h;
}

int HASHTABLE::InitHashTable(int ElementTotal)
{
	if (ElementTotal < 0)
	{
		sprintf(ErrorMessage[0], "HASHTABLE开辟空间不能为负数，请重新检查InitHashTable函数");
		cpGetErrorInfo()->PrintError(1);
		return -2;
	}
	if (ElementTotal == 0)
	{
		ElementTotal = 1000;
	}
	if (IsInit == 0 || 5 * ElementTotal > HashTableSize)
	{
		FreeTable();
		HashTableSize = 10 * ElementTotal;
		MallocNew(gHashTable, HASHTABLENODE, HashTableSize);
	}
	memset(gHashTable, 0, HashTableSize*sizeof(HASHTABLENODE));

	IsInit = 1;
	return 1;
}

int HASHTABLE::InsertElementToHashTable(int index, const char *lpszString)
{
	if (IsInit == 0)
	{
		sprintf(ErrorMessage[0], "必须先使用InitHashTable初始化，然后才能使用函数InsertElementToHashTable和ElementSearch");
		cpGetErrorInfo()->PrintError(1);
		return -2;
	}
	if (iHashTotal >= HashTableSize / 2)
	{
		sprintf(ErrorMessage[0], "HASHTABLE开辟空间太小，请重新检查");
		cpGetErrorInfo()->PrintError(1);
		return -2;
	}
	int  nPos = HashString(lpszString) % HashTableSize;

	while (gHashTable[nPos].sKey)
	{
		if (strcmp(gHashTable[nPos].sKey, lpszString) == 0)
		{
			return gHashTable[nPos].Index;
		}
		nPos = (nPos + 1) % HashTableSize;
	}
	int strLength;
	strLength = strlen(lpszString);
	gHashTable[nPos].sKey = new char[strLength + 1];
	strncpy(gHashTable[nPos].sKey, lpszString, strLength);
	gHashTable[nPos].sKey[strLength] = '\0';
	gHashTable[nPos].Index = index;

	return -1;
}

int HASHTABLE::ElementSearch(const char *lpszString)
{
	if (IsInit == 0)
	{
		sprintf(ErrorMessage[0], "必须先使用InitHashTable初始化，然后才能使用函数InsertElementToHashTable和ElementSearch");
		cpGetErrorInfo()->PrintError(1);
	}
	int  nPos = HashString(lpszString) % HashTableSize;

	while (gHashTable[nPos].sKey)
	{
		if (strcmp(gHashTable[nPos].sKey, lpszString) == 0)
		{
			return gHashTable[nPos].Index;
		}
		nPos = (nPos + 1) % HashTableSize;
	}
	return -1;
}

void HASHTABLE::FreeTable()
{
	if (IsInit == 0)return;
	int i;
	for (i = 0; i < HashTableSize; i++)
	{
		if (gHashTable[i].sKey)
			delete gHashTable[i].sKey;
	}
	FreeArray(gHashTable);
	HashTableSize = 0; iHashTotal = 0;
	IsInit = 0;
}
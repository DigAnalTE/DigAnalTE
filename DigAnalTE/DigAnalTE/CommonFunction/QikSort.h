
#ifndef __QIKSORT_INTERFACE_H__
#define __QIKSORT_INTERFACE_H__

//该类是排序算法
//目前代码中都使用了该类进行排序
//该排序方法适合电力系统计算的应用
//要使用排序法，必须派生自QikSortInterface，派生kompar和swap函数，这两个函数分别用于值的比较和交换
//kompar--判断排序在前或在后的函数
//swap--调换位置的函数

#include "constdef.h"

class QikSortInterface
{
protected:
	virtual int kompar(int m, int n){ return m > n; }
	virtual void swap(int m, int n){}
public:
	void QikSort(int mm, int nn);
	void QikSort(int size){ QikSort(0, size - 1); }
};

class TempFloatSort : public QikSortInterface
{
public:
	TempFloatSort(float *tFloatValue, int *tIndex)
	{
		FloatValue = tFloatValue;
		Index = tIndex;
		error = 0.0001f;
	}
private:
	float *FloatValue;
	int *Index;
	float error;
	int tempn; float tempf;
protected:
	void SetError(float terr){ error = terr; }
	virtual int kompar(int m, int n);
	virtual void swap(int m, int n);
};
class TempRealSort : public QikSortInterface
{
public:
	TempRealSort(real *tRealValue, int *tIndex)
	{
		RealValue = tRealValue;
		Index = tIndex;
		error = 0.0001f;
	}
private:
	real *RealValue;
	int *Index;
	real error;
	int tempn; real tempf;
protected:
	void SetError(real terr){ error = terr; }
	virtual int kompar(int m, int n);
	virtual void swap(int m, int n);
};
class TempRealGreatSort : public QikSortInterface
{
public:
	TempRealGreatSort(real *tRealValue, int *tIndex)
	{
		RealValue = tRealValue;
		Index = tIndex;
		error = 0.0001f;
	}
private:
	real *RealValue;
	int *Index;
	real error;
	int tempn; real tempf;
protected:
	void SetError(real terr){ error = terr; }
	virtual int kompar(int m, int n);
	virtual void swap(int m, int n);
};
class TempIntSort : public QikSortInterface
{
public:
	TempIntSort(int *tIntValue, int *tIndex)
	{
		IntValue = tIntValue;
		Index = tIndex;
	}
private:
	int *IntValue;
	int *Index;
	int tempn; int tempf;
protected:
	virtual int kompar(int m, int n);
	virtual void swap(int m, int n);
};

#endif

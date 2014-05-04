
#ifndef __DigAnalTE_PowerFlowFunction_H__
#define __DigAnalTE_PowerFlowFunction_H__

#include "../NetWorkInfo/NetWorkInfo.h"
#include "../CommonFunction/ErrorInfo.h"

struct PFCalPara
{//����������Ʋ���
	int MaxIter;//ţ������ѷ������������
	float BusPMaxErr;//�ж������ڵ���������
	float BusQMaxErr;
	//MaxIter=100;BusPMaxErr=0.005f;BusQMaxErr=0.005f;
};

class PFSOLUTION_BASE
{
public:
	PFSOLUTION_BASE(NETWORKINFO* temp)
	{
		mPara.MaxIter=50;
		mPara.BusPMaxErr=0.005f;
		mPara.BusQMaxErr=0.005f;
		pNet=temp;
		autoUpdate=1;
	}
	~PFSOLUTION_BASE()
	{
		FreeSpace();
	}
    virtual void FreeSpace(){}
protected:
	PFCalPara       mPara;
	NETWORKINFO*    pNet;
public:
	int autoUpdate;//�Ƿ��ڼ����곱��������������н���Update
public:
	int Calculate(PFCalPara tPara)
	{
		mPara=tPara;
        return Calculate();
	}
	int Calculate()
	{
		if (pNet->NetError!=1)
		{
			sprintf(ErrorMessage[0],"���ݴ��ڴ��󣬲��ܽ��г�������");
			cpGetErrorInfo()->PrintError(1);
			return 0;
		}
		if (subCalculate(mPara)==1)
		{
			if(autoUpdate)pNet->Update();
			return 1;
		}
		return 0;
	}
	virtual int subCalculate(PFCalPara)=0;//���麯���������������麯����Ŀ����Ϊ�˼������ʱ��������������ȷ��
};

PFSOLUTION_BASE* makeNRSolver(NETWORKINFO*);

#endif
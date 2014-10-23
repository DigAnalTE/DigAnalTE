
#ifndef __BPA_GENERATOR_H__
#define __BPA_GENERATOR_H__

#include "BPABus.h"
#include "../../DigAnalTE/DigAnalTE/NetWorkInfo/Generator.h"

class BPAGENERATOR :public GENERATOR
{//BPA的发电机不同于DigAnalTE中的，不参与潮流计算
public:
	float m_fPper, m_fQper;
	BPABUS *pBus;
public:
	virtual void subJacElement(class NETWORKINFO*pNet);
	virtual void UpdateValue(class NETWORKINFO*pNet);
public://读写部分
	virtual int ReadLine(char*Line);
public://暂态计算部分
	virtual void FormDynMatrix(class DYNAMICMODELINFO*);
};

#endif

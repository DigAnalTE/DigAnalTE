
#ifndef __BPA_GENERATOR_H__
#define __BPA_GENERATOR_H__

#include "BPABus.h"
#include "../../DigAnalTE/DigAnalTE/NetWorkInfo/Generator.h"

class BPAGENERATOR :public GENERATOR
{//BPA�ķ������ͬ��DigAnalTE�еģ������볱������
public:
	float m_fPper, m_fQper;
	BPABUS *pBus;
public:
	virtual void subJacElement(class NETWORKINFO*pNet);
	virtual void UpdateValue(class NETWORKINFO*pNet);
public://��д����
	virtual int ReadLine(char*Line);
public://��̬���㲿��
	virtual void FormDynMatrix(class DYNAMICMODELINFO*);
};

#endif

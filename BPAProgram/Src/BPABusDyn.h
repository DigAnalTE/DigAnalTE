
#ifndef __BPA_BUS_DYN_MODEL_H__
#define __BPA_BUS_DYN_MODEL_H__

#include "../../DigAnalTE/DigAnalTE/DynamicModel/BusDynModel.h"

class BPABUSDYNMODEL :public BUSDYNMODEL
{
public:
	virtual int ReadOutLine(char*Line);
};

#endif

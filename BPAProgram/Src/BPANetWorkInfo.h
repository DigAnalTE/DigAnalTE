
#ifndef __BPA_NETWORK_INFO_H__
#define __BPA_NETWORK_INFO_H__

#include "../../DigAnalTE/DigAnalTE/NetWorkInfo/NetWorkInfo.h"

class BPA_NETWORKINFO : public NETWORKINFO
{
public:
	virtual int ReadFile(char*file);
	virtual void OutputPFOFile(FILE*fp);//���������Ϣ
private:
	int Area_ReadBPALine(char* PFLine);
};

#endif

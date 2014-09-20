
#ifndef __BPA_NETWORK_INFO_H__
#define __BPA_NETWORK_INFO_H__

#include "../../DigAnalTE/DigAnalTE/NetWorkInfo/NetWorkInfo.h"

class BPA_NETWORKINFO : public NETWORKINFO
{
public:
	int ReadFile(char*file);
private:
	int Area_ReadBPALine(char* PFLine);
};

#endif

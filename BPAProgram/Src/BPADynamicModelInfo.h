
#ifndef __BPA_DYNAMIC_MODEL_INFO_H__
#define __BPA_DYNAMIC_MODEL_INFO_H__

#include "BPABus.h"
#include "BPABranch.h"
#include "../../DigAnalTE/DigAnalTE/DynamicModel/DynamicModelInfo.h"

class BPA_DYNAMICMODELINFO : public DYNAMICMODELINFO
{
public:
	BPA_DYNAMICMODELINFO();
public:
	virtual int ReadFile(char*file);
	int ReadBPAFile(char*datfile, char*swifile);
	virtual void Update();//由于BPA的设备都是在母线中处理，所以母线需要先进行Update
public:
	BPABUS *BPABus[_MaxBusNo];
	BPABRANCH *BPABranch[_MaxBranchNo];
	int BPABusSearch(char *name, float baseKV);
	int BPAGenSearch(char*name, float baseKV, char ID);
	virtual void AddNewBusDyn(int BusNo);
public:
	char GHName[_MaxNameLen];
	float GHBaseKV;
	char GHID;
private:
	int Net_ReadBPALine(char* PFLine);
	int Area_ReadBPALine(char* PFLine);
	int LN_ReadBPALine(char* Line);
	int MC_ReadBPALine(char* Line);
	int FF_ReadBPALine(char* Line);
	int GH_ReadBPALine(char* Line);
};

#endif

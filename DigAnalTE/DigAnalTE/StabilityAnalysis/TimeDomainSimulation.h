
#ifndef __DigAnalTE_TimeDomainSimulation_H

#include "../DynamicModel/DynamicModelInfo.h"
#include "DyFaultInfo.h"
#include "../SpareMatrixLib/ComplexSpareMatrixSolver.h"

class TDSIMULATION
{
public:
	TDSIMULATION();
	virtual ~TDSIMULATION(){ FreeSpace(); }
	void FreeSpace();
	void SetDynModel(DYNAMICMODELINFO*tDyn);
protected://计算信息
	DYNAMICMODELINFO*pDyn;
public://数据
	int BusTotal;
	float *pVx, *pVy;//求解迭代过程中的电压
	float *pIx, *pIy;//求解迭代过程中的电流
	COMPLEXSPAREMATRIXSOLVER *m_Solver;
public:
	int OutputTotal;
	int OutputSpace, OutputCount;
	real *OutputValue;
	int MallocOutputSpace(int space);
	void SaveOutputValue(float time);
public:
	int CheckInitalCalcul();
	int Calculate(DYFAULTINFO*pFault);
	void WriteCurve(char*file);
};

#endif

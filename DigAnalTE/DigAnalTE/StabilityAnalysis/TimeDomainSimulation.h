
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
protected://������Ϣ
	DYNAMICMODELINFO*pDyn;
public://����
	int BusTotal;
	float *pVx, *pVy;//�����������еĵ�ѹ
	float *pIx, *pIy;//�����������еĵ���
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

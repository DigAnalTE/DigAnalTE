
#include "ComplexSpareMatrixSolver.h"

void COMPLEXSPAREMATRIXSOLVER::InitMatrix(COMPLEXSPAREMATRIX *tMatrix)
{
	pMatrix = tMatrix;
	RowTotal = pMatrix->RowTotal;
	ElementTotal = pMatrix->ElementTotal;
	MallocNew(VA, Composite, ElementTotal);
	if (pMatrix->IsNewNo == 0)
	{
		pMatrix->SpareMatrixReorder();
	}
	//pMatrix->ReSetOldOrder();
	NewNo = pMatrix->NewNo;
	OldNo = pMatrix->OldNo;
	NIA = pMatrix->NIA;
	NNA = pMatrix->NNA;
	NJA = pMatrix->NJA;
	VALink = pMatrix->VALink;
	subInitMatrix();
	ReSetMatrixElement();
}

void COMPLEXSPAREMATRIXSOLVER::ReSetMatrixElement()
{
	int i;
	for (i = 0; i < pMatrix->ElementTotal; i++)
	{
		SetElement(i,
			pMatrix->VB[i], -pMatrix->VG[i],
			-pMatrix->VG[i], -pMatrix->VB[i]);
	}
}

int COMPLEXSPAREMATRIXSOLVER::MatrixVectorMultiply(real *tX, real *tY)
{
	real *tempX = NULL, *tempY = NULL;
	MallocNew(tempX, real, RowTotal); memcpy(tempX, tX, RowTotal*sizeof(real));
	MallocNew(tempY, real, RowTotal); memcpy(tempY, tY, RowTotal*sizeof(real));
	memset(tX, 0, RowTotal*sizeof(real));
	memset(tY, 0, RowTotal*sizeof(real));
	int i, j, k, jCol;
	for (i = 0; i<RowTotal; i++)
	{
		for (j = 0, k = pMatrix->IA[i]; j<pMatrix->NA[i]; j++, k++)
		{
			jCol = pMatrix->JA[k];
			tX[i] += VA[k].RowH*tempX[jCol] + VA[k].RowN*tempY[jCol];
			tY[i] += VA[k].RowJ*tempX[jCol] + VA[k].RowL*tempY[jCol];
		}
	}
	return 1;
}

#include "../NetWorkInfo/NetWorkInfo.h"
void COMPLEXSPAREMATRIXSOLVER::PrintMatrix(NETWORKINFO*pNet, FILE*fp)
{
	int k, l, m, n, p;
	char Line[_MaxLineLen];
	for (k = 0; k < RowTotal; k++)
	{
		l = OldNo[k];
		sprintf(Line, " %7d%7d ", k + 1, l + 1);
		fprintf(fp, Line);
		pNet->cpGetBus(l)->PrintInfo(Line);
		fprintf(fp, Line);
		fprintf(fp, "\n");
		for (m = 0, n = NIA[k]; m < NNA[k]; m++, n++)
		{
			p = VALink[n];
			sprintf(Line, "     %7d%7d%11.4f%11.4f%11.4f%11.4f\n",
				NJA[n] + 1, OldNo[NJA[n]] + 1,
				VA[p].RowH, VA[p].RowN, VA[p].RowJ, VA[p].RowL);
			fprintf(fp, Line);
		}
	}
}

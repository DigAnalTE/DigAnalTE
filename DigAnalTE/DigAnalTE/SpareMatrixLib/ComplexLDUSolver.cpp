
#include "ComplexLDUSolver.h"
#include "../CommonFunction/ErrorInfo.h"

COMPLEXSPAREMATRIXSOLVER* makeLDUSolver(){ return new LDUSOLVER; }

void LDUSOLVER::ReMallocSpace(int tAddSpace)
{//当空间不够的时候重新申请大空间
	//if (LDUnVASpace + tAddSpace < pMatrix->JacSpaceMax)tAddSpace = pMatrix->JacSpaceMax - LDUnVASpace;
	if (tAddSpace < 1)return;
	Composite *O_LVA, *O_UVA;
	int *OldJA, *OldLINK;
	int NewSpace = LDUnVASpace + tAddSpace;
	O_LVA = L_VA;
	O_UVA = U_VA;
	OldJA = LDUJA;
	OldLINK = LDULINK;
	L_VA = NULL;		MallocNew(L_VA, Composite, NewSpace);	memset((char*)L_VA, 0, NewSpace*sizeof(Composite));	memcpy(L_VA, O_LVA, LDUnVASpace*sizeof(real));
	U_VA = NULL;		MallocNew(U_VA, Composite, NewSpace);	memset((char*)U_VA, 0, NewSpace*sizeof(Composite));	memcpy(U_VA, O_UVA, LDUnVASpace*sizeof(real));
	LDUJA = NULL;		MallocNew(LDUJA, int, NewSpace);		memset((char*)LDUJA, 0, NewSpace*sizeof(int));		memcpy(LDUJA, OldJA, LDUnVASpace*sizeof(int));
	LDULINK = NULL;	MallocNew(LDULINK, int, NewSpace);	memset((char*)LDULINK, 0, NewSpace*sizeof(int));		memcpy(LDULINK, OldLINK, LDUnVASpace*sizeof(int));
	FreeArray(O_LVA);
	FreeArray(O_UVA);
	FreeArray(OldJA);
	FreeArray(OldLINK);
	LDUnVASpace = NewSpace;
}

int LDUSOLVER::LDUFactorization()
{
	int i, j, k, iVA, l;
	//第一步，生成L矩阵和U矩阵的初始部分
	for (i = 0, iVA = 0; i < RowTotal; i++)
	{
		LDUIA[i] = iVA;
		for (j = 0, k = NIA[i]; j < NNA[i]; j++, k++)
		{
			if (NJA[k] < i)
			{//i行 NJA[k]列的元素//下半部分
				l = LDUIA[NJA[k]];
				l = LDULINK[l];
				while (1)//查找LDUJA[k]列
				{
					if (l<0 || LDUJA[l]>i)
					{//找不到
						sprintf(ErrorMessage[0], "LDU分解只能用于对称结构的稀疏矩阵");
						cpGetErrorInfo()->PrintError(1);
						return 0;
						break;
					}
					else if (LDUJA[l] == i)
					{//找到了
						L_VA[l] = VA[VALink[k]];
						break;
					}
					l = LDULINK[l];
				}
				continue;
			}
			LDUJA[iVA] = NJA[k];
			U_VA[iVA] = VA[VALink[k]];//上半部分
			if (NJA[k] == i)
			{
				L_VA[iVA] = VA[VALink[k]];
				U_VA[iVA].RowJ = 0;
				L_VA[iVA].RowN = 0;
			}
			LDULINK[iVA] = iVA + 1;
			iVA++;
		}
		LDULINK[iVA - 1] = -1;
	}
	LDUnVANumb = iVA;
	//进行LDU分解
	int m, tLastLink;
	real IIRowH, IIRowL;//对角元
	for (i = 0; i < RowTotal; i++)
	{//处理第i行//由于是complex元素，需要处理两遍，第一遍处理第一行，第二遍处理第二行
		//第一遍//根据RowH进行处理
		IIRowH = U_VA[LDUIA[i]].RowH;
		if (fabs(IIRowH) < 0.000001)
		{
			sprintf(ErrorMessage[0], "规格化运算发生错误");
			cpGetErrorInfo()->PrintError(1);
			return 0;
		}
		//处理该元素
		L_VA[LDUIA[i]].RowH = 1;
		L_VA[LDUIA[i]].RowJ /= IIRowH;
		L_VA[LDUIA[i]].RowL -= U_VA[LDUIA[i]].RowN*L_VA[LDUIA[i]].RowJ;
		U_VA[LDUIA[i]].RowL -= U_VA[LDUIA[i]].RowN*L_VA[LDUIA[i]].RowJ;//=L_VA[LDUIA[i]].RowL
		j = LDUIA[i];
		j = LDULINK[j];
		while (j >= 0)
		{
			//处理这一行的元素//L_VA[j]=L_VA[j]/IIRowH;//规格化运算
			L_VA[j].RowH /= IIRowH;
			L_VA[j].RowJ /= IIRowH;
			j = LDULINK[j];
		}
		//开始处理各行数据
		j = LDUIA[i];
		j = LDULINK[j];
		while (j >= 0)
		{
			L_VA[j].RowN -= U_VA[LDUIA[i]].RowN*L_VA[j].RowH;
			L_VA[j].RowL -= U_VA[LDUIA[i]].RowN*L_VA[j].RowJ;
			U_VA[j].RowJ -= U_VA[j].RowH*L_VA[LDUIA[i]].RowJ;
			U_VA[j].RowL -= U_VA[j].RowN*L_VA[LDUIA[i]].RowJ;
			//下面开始消去运算
			m = LDUIA[LDUJA[j]];//第一个点
			//U_VA[m]-=L_VA[j]*U_VA[j];//LDUJA[j]行LDUJA[k]列进行消去，第一个消去点(j==k)有点不同
			L_VA[m].RowH -= U_VA[j].RowH*L_VA[j].RowH;
			L_VA[m].RowJ -= U_VA[j].RowH*L_VA[j].RowJ;
			L_VA[m].RowL -= U_VA[j].RowN*L_VA[j].RowJ;
			U_VA[m].RowH -= U_VA[j].RowH*L_VA[j].RowH;
			U_VA[m].RowN -= U_VA[j].RowN*L_VA[j].RowH;
			U_VA[m].RowL -= U_VA[j].RowN*L_VA[j].RowJ;
			k = LDULINK[j];
			while (k >= 0)
			{//LDUJA[j]行LDUJA[k]列进行消去
				m = LDUIA[LDUJA[j]];
				tLastLink = m;
				m = LDULINK[m];
				while (1)//查找LDUJA[k]列
				{
					if (m<0 || LDUJA[m]>LDUJA[k])
					{//找不到
						if (LDUnVANumb >= LDUnVASpace)ReMallocSpace(LDUnVASpace / 2);
						LDUJA[LDUnVANumb] = LDUJA[k];
						LDULINK[LDUnVANumb] = m;
						LDULINK[tLastLink] = LDUnVANumb;
						m = LDUnVANumb;
						L_VA[m] = { 0, 0, 0, 0 };
						U_VA[m] = { 0, 0, 0, 0 };
						LDUnVANumb++;
					}
					if (LDUJA[m] == LDUJA[k])
					{//找到了
						//U_VA[m]-=L_VA[j]*U_VA[k];
						L_VA[m].RowH -= U_VA[j].RowH*L_VA[k].RowH;
						L_VA[m].RowJ -= U_VA[j].RowH*L_VA[k].RowJ;
						L_VA[m].RowN -= U_VA[j].RowN*L_VA[k].RowH;
						L_VA[m].RowL -= U_VA[j].RowN*L_VA[k].RowJ;
						U_VA[m].RowH -= U_VA[k].RowH*L_VA[j].RowH;
						U_VA[m].RowJ -= U_VA[k].RowH*L_VA[j].RowJ;
						U_VA[m].RowN -= U_VA[k].RowN*L_VA[j].RowH;
						U_VA[m].RowL -= U_VA[k].RowN*L_VA[j].RowJ;
						break;
					}
					tLastLink = m;
					m = LDULINK[m];
				}
				k = LDULINK[k];
			}
			j = LDULINK[j];
		}
		//处理第二遍//根据RowL进行处理
		//处理该元素
		IIRowL = U_VA[LDUIA[i]].RowL;
		L_VA[LDUIA[i]].RowL = 1;
		if (fabs(IIRowL) < 0.000001)
		{
			sprintf(ErrorMessage[0], "规格化运算发生错误");
			cpGetErrorInfo()->PrintError(1);
			return 0;
		}
		//处理这一行的元素
		j = LDUIA[i];
		j = LDULINK[j];
		while (j >= 0)
		{//U_VA[j]=U_VA[j]/U_VA[LDUIA[i]];//规格化运算
			L_VA[j].RowN /= IIRowL;
			L_VA[j].RowL /= IIRowL;
			j = LDULINK[j];
		}
		j = LDUIA[i];
		j = LDULINK[j];
		while (j >= 0)
		{
			//下面开始消去运算
			m = LDUIA[LDUJA[j]];//第一个点
			//U_VA[m]-=L_VA[j]*U_VA[j];//LDUJA[j]行LDUJA[k]列进行消去，第一个消去点(j==k)有点不同
			L_VA[m].RowH -= U_VA[j].RowJ*L_VA[j].RowN;
			L_VA[m].RowJ -= U_VA[j].RowJ*L_VA[j].RowL;
			L_VA[m].RowL -= U_VA[j].RowL*L_VA[j].RowL;
			U_VA[m].RowH -= U_VA[j].RowJ*L_VA[j].RowN;
			U_VA[m].RowN -= U_VA[j].RowL*L_VA[j].RowN;
			U_VA[m].RowL -= U_VA[j].RowL*L_VA[j].RowL;
			k = LDULINK[j];
			while (k >= 0)
			{//LDUJA[j]行LDUJA[k]列进行消去
				m = LDUIA[LDUJA[j]];
				tLastLink = m;
				m = LDULINK[m];
				while (1)//查找LDUJA[k]列
				{
					if (m<0 || LDUJA[m]>LDUJA[k])
					{//找不到
						sprintf(ErrorMessage[0], "严重错误");
						cpGetErrorInfo()->PrintError(1);
						return 0;
					}
					if (LDUJA[m] == LDUJA[k])
					{//找到了
						//U_VA[m]-=L_VA[j]*U_VA[k];
						L_VA[m].RowH -= U_VA[j].RowJ*L_VA[k].RowN;
						L_VA[m].RowJ -= U_VA[j].RowJ*L_VA[k].RowL;
						L_VA[m].RowN -= U_VA[j].RowL*L_VA[k].RowN;
						L_VA[m].RowL -= U_VA[j].RowL*L_VA[k].RowL;
						U_VA[m].RowH -= U_VA[k].RowJ*L_VA[j].RowN;
						U_VA[m].RowJ -= U_VA[k].RowJ*L_VA[j].RowL;
						U_VA[m].RowN -= U_VA[k].RowL*L_VA[j].RowN;
						U_VA[m].RowL -= U_VA[k].RowL*L_VA[j].RowL;
						break;
					}
					tLastLink = m;
					m = LDULINK[m];
				}
				k = LDULINK[k];
			}
			j = LDULINK[j];
		}
	}
	for (i = 0; i < RowTotal; i++)
	{
		IIRowH = U_VA[LDUIA[i]].RowH;
		U_VA[LDUIA[i]].RowN /= IIRowH;
		//处理这一行的元素
		j = LDUIA[i];
		j = LDULINK[j];
		while (j >= 0)
		{//U_VA[j]=U_VA[j]/U_VA[LDUIA[i]];//规格化运算
			U_VA[j].RowH /= IIRowH;
			U_VA[j].RowN /= IIRowH;
			j = LDULINK[j];
		}
		IIRowL = U_VA[LDUIA[i]].RowL;
		//处理这一行的元素
		j = LDUIA[i];
		j = LDULINK[j];
		while (j >= 0)
		{
			U_VA[j].RowJ /= IIRowL;
			U_VA[j].RowL /= IIRowL;
			j = LDULINK[j];
		}
	}
	flagChange = 0;
	return 1;
}

int LDUSOLVER::Calculate()
{
	if (flagChange == 1)
	{
		LDUFactorization();
	}
	int i, j, k, old;
	//前代
	for (i = 0; i < RowTotal; i++)
	{
		old = OldNo[i];
		if (X[old] == 0 && Y[old] == 0)
			continue;
		k = LDUIA[i];
		Y[old] -= L_VA[k].RowJ*X[old];
		k = LDULINK[k];
		while (k >= 0)
		{
			j = LDUJA[k];
			X[OldNo[j]] -= L_VA[k].RowH*X[old] + L_VA[k].RowN*Y[old];
			Y[OldNo[j]] -= L_VA[k].RowJ*X[old] + L_VA[k].RowL*Y[old];
			k = LDULINK[k];
		}
	}
	//除法
	for (i = 0; i < RowTotal; i++)
	{
		old = OldNo[i];
		X[old] /= U_VA[LDUIA[i]].RowH;
		Y[old] /= U_VA[LDUIA[i]].RowL;
	}
	//回代
	for (i = RowTotal - 1; i >= 0; i--)
	{
		old = OldNo[i];
		k = LDUIA[i];
		k = LDULINK[k];
		while (k >= 0)
		{
			j = LDUJA[k];
			Y[old] -= U_VA[k].RowL*Y[OldNo[j]];
			Y[old] -= U_VA[k].RowJ*X[OldNo[j]];
			X[old] -= U_VA[k].RowN*Y[OldNo[j]];
			X[old] -= U_VA[k].RowH*X[OldNo[j]];
			k = LDULINK[k];
		}
		k = LDUIA[i];
		X[old] -= U_VA[k].RowN*Y[old];
	}
	return 1;
}
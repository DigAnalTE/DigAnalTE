
#include "../CommonFunction/constdef.h"
#include "../CommonFunction/ErrorInfo.h"
#include "SpareMatrix.h"

int SPAREMATRIXBASE::kompar(int m, int n)
{
	return NJA[m] - NJA[n];
}

void SPAREMATRIXBASE::swap(int m, int n)
{
	int tempi;
	tempi = NJA[n];
	NJA[n] = NJA[m];
	NJA[m] = tempi;

	tempi = VALink[n];
	VALink[n] = VALink[m];
	VALink[m] = tempi;
}

void SPAREMATRIXBASE::SpareMatrixReorder()
{
	if (IsNewNo != 0)
	{
		IsNewNo = 0;
		FreeArray(NewNo); FreeArray(OldNo); FreeArray(NIA); FreeArray(NNA); FreeArray(NJA); FreeArray(VALink);
	}
	int i;
	MallocNew(NewNo, int, RowTotal);
	MallocNew(OldNo, int, RowTotal);
	for (i = 0; i < RowTotal; i++)
	{
		NewNo[i] = i;
		OldNo[i] = i;
	}
	JacSpaceMax = ElementTotal * 5;

	//�ڶ����������µĽڵ��
	MallocNew(NIA, int, RowTotal);
	MallocNew(NNA, int, RowTotal);
	MallocNew(NJA, int, ElementTotal);
	MallocNew(VALink, int, ElementTotal);

	//�����������±��
	for (i = 0; i < ElementTotal; i++)
	{
		NJA[i] = NewNo[JA[i]];
		VALink[i] = i;
	}
	for (i = 0; i < RowTotal; i++)
	{
		QikSort(IA[i], IA[i] + NA[i] - 1);
		NIA[NewNo[i]] = IA[i];
		NNA[NewNo[i]] = NA[i];
	}

	IsNewNo = 1;
}
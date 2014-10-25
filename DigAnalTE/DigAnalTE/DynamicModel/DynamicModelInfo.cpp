
#include "DynamicModelInfo.h"
#include "../CommonFunction/ErrorInfo.h"
#include "GenEq1Constant.h"

DYNAMICMODELINFO::DYNAMICMODELINFO()
{
	DynamicModelTotal = 0;
	memset(DynamicModel, 0, _MaxDynamicModelNo*sizeof(DYNAMIC_MODEL_BASE*));
	memset(m_BusDyn, 0, _MaxBusNo*sizeof(BUSDYNMODEL*));
	Vx = NULL; Vy = NULL; Ix = NULL; Iy = NULL;
	bDynMatrixForm = 0; bDynInitial = 0;
	TotalTime = 6.;
	Tnow = 0; Step = 0.01f;
	MaxIterNumber = 10;
	FlagExit = 0;
	NextEvent = 9999.;
}

DYNAMICMODELINFO::~DYNAMICMODELINFO()
{
	FreeSpace();
}

void DYNAMICMODELINFO::FreeSpace()
{
	int i;
	for (i = 0; i<DynamicModelTotal; i++)
	{
		if (DynamicModel[i] == NULL)
			continue;
		delete DynamicModel[i];
		DynamicModel[i] = NULL;
	}
	DynamicModelTotal = 0;
	m_DynMatrix.FreeSpace();
	bDynMatrixForm = 0;
	bDynInitial = 0;
	FreeArray(Vx);
	FreeArray(Vy);
	FreeArray(Ix);
	FreeArray(Iy);
}

int DYNAMICMODELINFO::InsertNewDynamicModel(DYNAMIC_MODEL_BASE*tempObj)
{
	if (DynamicModelTotal >= _MaxDynamicModelNo)
	{
		return -1;
	}
	DynamicModel[DynamicModelTotal] = tempObj;
	DynamicModelTotal++;
	return DynamicModelTotal - 1;
}

void DYNAMICMODELINFO::AddNewBusDyn(int busno)
{
	if (m_BusDyn[busno] != NULL)
		return;
	m_BusDyn[busno] = new BUSDYNMODEL;
	m_BusDyn[busno]->BusNo = busno;
}

#include "../CommonFunction/QikSort.h"
class DynTempSortModel : public QikSortInterface
{
public:
	DynTempSortModel(DYNAMICMODELINFO*tInfo){ pInfo = tInfo; }
private:
	int flag;
	DYNAMIC_MODEL_BASE *tempObj;
	DYNAMICMODELINFO*pInfo;
protected:
	virtual int kompar(int m, int n);
	virtual void swap(int m, int n);
};
int DynTempSortModel::kompar(int m, int n)
{
	flag = pInfo->DynamicModel[m]->EquipIndex - pInfo->DynamicModel[n]->EquipIndex;
	if (flag == 0)
	{
		if (pInfo->DynamicModel[n]->mPrio>pInfo->DynamicModel[m]->mPrio + 0.0001)
		{
			flag = -1;
		}
		else if (pInfo->DynamicModel[n]->mPrio<pInfo->DynamicModel[m]->mPrio - 0.0001)
			flag = 1;
		else
			flag = 0;
	}
	return flag;
}
void DynTempSortModel::swap(int m, int n)
{
	tempObj = pInfo->DynamicModel[m];
	pInfo->DynamicModel[m] = pInfo->DynamicModel[n];
	pInfo->DynamicModel[n] = tempObj;
}

int DYNAMICMODELINFO::NetAnalysis()
{
	int i, flag, equipno;
	flag = NETWORKINFO::NetAnalysis();
	if (flag != 1)return flag;
	EQUIPMENT_DYN_MODEL*tModel;
	for (i = 0; i<DynamicModelTotal; i++)
	{
		DynamicModel[i]->SetSolInfo(this);
		DynamicModel[i]->BMVA = GetBMVA();
		equipno = EquipSearch(DynamicModel[i]->EquipmentName);
		if (equipno < 0)
		{
			sprintf(ErrorMessage[0], "ERROR: 没有找到模型对应的设备");
			DynamicModel[i]->PrintInfo(ErrorMessage[1]);
			cpGetErrorInfo()->PrintWarning(11, 2);
			continue;
		}
		DynamicModel[i]->EquipIndex = equipno;
		DynamicModel[i]->pEquip = cpGetEquip(equipno);
		if (cpGetEquip(equipno)->iGetState() == 0)
			DynamicModel[i]->m_State = 0;
		tModel = DynamicModel[i]->IsEquipModel();
		if (tModel!=NULL)
		{
			cpGetEquip(equipno)->SetEquipModel(tModel);
		}
	}
	for (i = 0; i<DynamicModelTotal; i++)
	{
		tModel = DynamicModel[i]->IsEquipModel();
		if (tModel == NULL)
		{
			DynamicModel[i]->pModel = DynamicModel[i]->pEquip->GetEquipModel();
			if (DynamicModel[i]->pModel==NULL)
			{
				sprintf(ErrorMessage[0], "ERROR: 没有找到模型对应的注入电流模型");
				DynamicModel[i]->PrintInfo(ErrorMessage[1]);
				cpGetErrorInfo()->PrintWarning(12, 2);
			}
		}
	}
	cpGetErrorInfo()->CheckMessageType(11);
	cpGetErrorInfo()->CheckMessageType(12);
	for (i = 0; i < iGetEquipTotal(); i++)
	{
		if (cpGetEquip(i)->GetEquipModel()==NULL)
		{
			sprintf(ErrorMessage[0], "WARNING: 设备没有对应的暂态模型，将作为等效负荷进行处理");
			cpGetEquip(i)->PrintInfo(ErrorMessage[1]);
			cpGetErrorInfo()->PrintWarning(-1, 2);
		}
	}
	DynTempSortModel tSort(this);
	tSort.QikSort(DynamicModelTotal);
	int tBusTotal = iGetBusTotal();
	FreeArray(Vx); MallocNew(Vx, real, tBusTotal);
	FreeArray(Vy); MallocNew(Vy, real, tBusTotal);
	FreeArray(Ix); MallocNew(Ix, real, tBusTotal);
	FreeArray(Iy); MallocNew(Iy, real, tBusTotal);
	for (i = 0; i<DynamicModelTotal; i++)
	{
		if (DynamicModel[i]->m_State == 0)
			continue;
		flag = DynamicModel[i]->CheckInputData();
		if (flag<0)
		{
			sprintf(ErrorMessage[0], "ERROR: 模型数据发生错误");
			DynamicModel[i]->PrintInfo(ErrorMessage[1]);
			cpGetErrorInfo()->PrintWarning(13, 2);
		}
		tModel = DynamicModel[i]->IsEquipModel();
		if (tModel != NULL)
		{
			tModel->pIx = Ix;
			tModel->pIy = Iy;
		}
		else{
			if (DynamicModel[i]->pModel->m_State == 0)
				DynamicModel[i]->m_State = 0;
		}
	}
	cpGetErrorInfo()->CheckMessageType(13);
	for (i = 0; i < iGetBusTotal(); i++)
	{
		if (m_BusDyn[i] == NULL)
			continue;
		m_BusDyn[i]->CheckInputData();
	}
	return 1;
}

int DYNAMICMODELINFO::ReadFile(char*file)
{
	int flag;
	flag = NETWORKINFO::ReadFile(file);
	if (flag != 1)
		return flag;

	FILE* fpfile = NULL;
	OpenFile(fpfile, file, "rb");
	if (fpfile == NULL)
		return 0;
	char Line[_MaxLineLen], type[10];
	while (fgets(Line, _MaxLineLen, fpfile))
	{
		flag = sscanf(Line, "%9s", type);
		if (flag < 1)
			continue;
		if (strncmp(type, "-999", 4) == 0)
			break;
	}
	DYNAMIC_MODEL_BASE*tModel;
	while (fgets(Line, _MaxLineLen, fpfile))
	{//读入动态模型
		flag = sscanf(Line, "%9s", type);
		if (flag < 1)
			continue;
		if (type[0] == '/')
			continue;
		if (strncmp(type, "-999", 4) == 0)
			break;
		if (strncmp(type, "GC", 2) == 0)
		{
			tModel = new GenEq1Constant;
			flag = tModel->ReadLine(Line);
			if (flag != 1)
			{
				delete tModel;
				sprintf(ErrorMessage[0], "动态模型数据读取失败：%s", Line);
				cpGetErrorInfo()->PrintWarning(11, 1);
				continue;
			}
			flag = InsertNewDynamicModel(tModel);
			if (flag < 0)
			{
				delete tModel;
				sprintf(ErrorMessage[0], "添加动态模型失败：%s", Line);
				cpGetErrorInfo()->PrintWarning(11, 1);
				continue;
			}
			continue;
		}
		sprintf(ErrorMessage[0], "没有对应的数据：%s", Line);
		cpGetErrorInfo()->PrintWarning(12, 1);
	}
	cpGetErrorInfo()->CheckMessageType(11);
	cpGetErrorInfo()->CheckMessageType(12);

	int i, equipno;
	EQUIPMENT_DYN_MODEL*tEqipModel;
	for (i = 0; i < DynamicModelTotal; i++)
	{//对动态模型进行连接，为读输出数据做准备
		equipno = EquipSearch(DynamicModel[i]->EquipmentName);
		if (equipno < 0)
		{
			sprintf(ErrorMessage[0], "ERROR: 没有找到模型对应的设备");
			DynamicModel[i]->PrintInfo(ErrorMessage[1]);
			cpGetErrorInfo()->PrintWarning(13, 2);
			continue;
		}
		DynamicModel[i]->EquipIndex = equipno;
		DynamicModel[i]->pEquip = cpGetEquip(equipno);
		tEqipModel = DynamicModel[i]->IsEquipModel();
		if (tEqipModel != NULL)
		{
			cpGetEquip(equipno)->SetEquipModel(tEqipModel);
		}
	}
	cpGetErrorInfo()->CheckMessageType(13);

	//读入输出数据和计算控制数据
	char tEquipName[_MaxNameLen], tBusName[_MaxNameLen];
	while (fgets(Line, _MaxLineLen, fpfile))
	{
		flag = sscanf(Line, "%9s", type);
		if (flag < 1)
			continue;
		if (type[0] == '/')
			continue;
		if (strncmp(type, "-999", 4) == 0)
			break;
		if (type[0] == 'O')
		{//模型输出数据
			flag = sscanf(Line, "%*[^,],%[^,]", tEquipName);
			if (flag != 1)
			{
				sprintf(ErrorMessage[0], "输出数据读取失败：%s", Line);
				cpGetErrorInfo()->PrintWarning(-1, 1);
				continue;
			}
			ReplaceName(tEquipName, _MaxNameLen);
			equipno = EquipSearch(tEquipName);
			tEqipModel = cpGetEquip(equipno)->GetEquipModel();
			if (tEqipModel == NULL)
			{
				sprintf(ErrorMessage[0], "输出数据没有对应的模型：%s", Line);
				cpGetErrorInfo()->PrintWarning(-1, 1);
				continue;
			}
			flag = tEqipModel->ReadOutLine(Line);
			if (flag != 1)
			{
				sprintf(ErrorMessage[0], "输出数据读取失败：%s", Line);
				cpGetErrorInfo()->PrintWarning(11, 1);
			}
			continue;
		}
		if (type[0] == 'B')
		{//母线输出数据
			flag = sscanf(Line, "%*[^,],%[^,]", tBusName);
			if (flag != 1)
			{
				sprintf(ErrorMessage[0], "输出数据读取失败：%s", Line);
				cpGetErrorInfo()->PrintWarning(-1, 1);
				continue;
			}
			ReplaceName(tBusName, _MaxNameLen);
			equipno = BusSearch(tBusName);
			AddNewBusDyn(equipno);
			flag = m_BusDyn[equipno]->ReadOutLine(Line);
			if (flag != 1)
			{
				sprintf(ErrorMessage[0], "输出数据读取失败：%s", Line);
				cpGetErrorInfo()->PrintWarning(11, 1);
			}
			continue;
		}
		if (strncmp(type, "CAL", 3) == 0)
		{
			flag = ReadCalculateInfo(Line);
			if (flag != 1)
			{
				sprintf(ErrorMessage[0], "仿真控制数据读取失败：%s", Line);
				cpGetErrorInfo()->PrintWarning(11, 1);
			}
			continue;
		}
	}
	return 1;
}

int DYNAMICMODELINFO::ReadCalculateInfo(char*line)
{
	int flag;
	flag = sscanf(line, "%*[^,],%f,%f", &TotalTime, &Step);
	if (flag != 2)
		return 0;
	return 1;
}

DYNAMIC_MODEL_BASE *DYNAMICMODELINFO::DynModelSearch(char*tEquipName, char*tModelName)
{
	int equipno;
	equipno = EquipSearch(tEquipName);
	if (equipno < 0)return NULL;
	int i;
	for (i = 0; i < DynamicModelTotal; i++)
	{
		if (DynamicModel[i]->EquipIndex != equipno)
			continue;
		if (strcmp(DynamicModel[i]->GetModelName(),tModelName) == 0)
			return DynamicModel[i];
	}
	return NULL;
}

int DYNAMICMODELINFO::DynInitial()
{
	if (bDynInitial == 1)
		return 1;
	int i;
	memset(Vx, 0, iGetBusTotal()*sizeof(real));
	memset(Vy, 0, iGetBusTotal()*sizeof(real));
	float v, sita;
	for (i = 0; i < iGetBusTotal(); i++)
	{
		v = cpGetBus(i)->m_fBusV;
		sita = cpGetBus(i)->m_fBusSita*0.0174532925f;
		Vx[i] = v*cos(sita);
		Vy[i] = v*sin(sita);
	}
	FormJacobi();
	for (i = 0; i < iGetBusTotal(); i++)
	{
		if (m_BusDyn[i] == NULL)
			continue;
		m_BusDyn[i]->DynInitial();
	}
	for (i = 0; i < DynamicModelTotal; i++)
	{
		DynamicModel[i]->DynInitial();
	}
	bDynInitial = 1;
	return 1;
}

void DYNAMICMODELINFO::FormJacobi()
{
	int i;
	m_DynMatrix.MallocSpace(m_Matrix);
	m_DynMatrix.Clear();
	m_DynMatrix.SpareMatrixReorder();
	for (i = 0; i < iGetBusTotal(); i++)
	{
		cpGetBus(i)->FormDynMatrix(this);
	}
	for (i = 0; i<iGetBranchTotal(); i++)
	{
		if (cpGetBranch(i)->iGetState() == 0)continue;
		cpGetBranch(i)->FormDynMatrix(this);
	}
	for (i = 0; i<iGetEquipTotal(); i++)
	{
		if (cpGetEquip(i)->iGetState() == 0)continue;
		if (cpGetEquip(i)->GetEquipModel() != NULL)
			cpGetEquip(i)->GetEquipModel()->ModifyMatrix();
		else
			cpGetEquip(i)->FormDynMatrix(this);
	}
	bDynMatrixForm = 1;
}

void DYNAMICMODELINFO::DeleteBranch(int BranchNo)
{
	if (cpGetBranch(BranchNo)->iGetState() == 0)
		return;
	cpGetBranch(BranchNo)->iSetState(0);
	if (bDynMatrixForm == 0)return;
	cpGetBranch(BranchNo)->DeFormDynMatrix(this);
}

void DYNAMICMODELINFO::DeleteOneEquipment(int EquipNo)
{
	if (cpGetEquip(EquipNo)->iGetState() == 0)
		return;
	cpGetEquip(EquipNo)->iSetState(0);
	if (bDynMatrixForm == 0)return;
	cpGetEquip(EquipNo)->DeFormDynMatrix(this);
}
void DYNAMICMODELINFO::ModifyNetMatrix(int IBusNo, real P, real Q)
{
	m_DynMatrix.GBElementModify(IBusNo, IBusNo, P / BMVA, Q / BMVA);
}

void DYNAMICMODELINFO::ModifyNetMatrix(int ibusno, int jbusno, real g, real b)
{
	m_DynMatrix.GBElementModify(ibusno, jbusno, g, b);
}

int DYNAMICMODELINFO::GetVxVy(int busno, real &vx, real &vy)
{
	if (busno >= 0 && busno < iGetBusTotal())
	{
		vx = Vx[busno];
		vy = Vy[busno];
		return 1;
	}
	vx = Vx[0];
	vy = Vy[0];
	return 0;
}

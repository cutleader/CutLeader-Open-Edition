#include "StdAfx.h"
#include "MachineLibHelper.h"

#include "DataBaseManager.h"

#include "ParamConfigLoader.h"
#include "ExpertLibItemLoader.h"
#include "MachineItem.h"
#include "MachineItemLoader.h"
#include "MachineParam.h"

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

CString MachineLibHelper::GetNcPostFix(LONGLONG iParamConfigID)
{
	DBConnectPtr pExpLibDBCon = DataBaseManager::GetInstance()->GetExpLibDBConnect();
	DBConnectPtr pMacLibDBConnect = DataBaseManager::GetInstance()->GetMacLibDBConnect();

	LONGLONG iExpLibItemID = ParamConfigLoader::GetExpLibItemID(pExpLibDBCon, iParamConfigID);
	LONGLONG iMacItemID = ExpertLibItemLoader::GetMacIDByExpLibItem(pExpLibDBCon, iExpLibItemID);
	MachineParamPtr pMacParam = MachineItemLoader::LoadMacParam(pMacLibDBConnect, iMacItemID);
	return pMacParam->GetNCPostFix();
}

END_CUTLEADER_NAMESPACE()

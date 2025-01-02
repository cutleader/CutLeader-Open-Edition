#include "StdAfx.h"
#include "RecentDataWriter.h"

#include "baseConst.h"
#include "IntegerList.h"
#include "DBConnect.h"

#include "RecentData.h"
#include "RecentDataList.h"

BEGIN_CUTLEADER_NAMESPACE()

void RecentDataWriter::SaveRecentData(DBConnectPtr pDBConnect, RecentDataListPtr pRecentDataList)
{
	pDBConnect->BeginTransaction();

	// clear table at first.
	CString strSQL1 = _T("delete from dataMgr_recentDataList;");
	pDBConnect->ExecuteModify(strSQL1, FALSE);

	// insert each RecentData object.
	for (unsigned int i = 0; i < pRecentDataList->size(); i++)
	{
		RecentDataPtr pRecentData = pRecentDataList->at(i);

		// 需要处理一下才能保存
		CString strDataPath_modifed = pRecentData->GetDataPath();
		strDataPath_modifed.Replace(L"'", L"''");

		CString strSQL2;
		strSQL2.Format(_T("insert into dataMgr_recentDataList values (%I64d, '%s', %I64d);"),
					   pRecentData->GetID(), strDataPath_modifed, pRecentData->GetDataID());
		pDBConnect->ExecuteModify(strSQL2, FALSE);
	}

	pDBConnect->CommitTransaction();
}

END_CUTLEADER_NAMESPACE()

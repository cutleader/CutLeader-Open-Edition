#include "StdAfx.h"
#include "RecentDataLoader.h"

#include "IntegerList.h"
#include "baseConst.h"
#include "DBRecord.h"
#include "DBField.h"
#include "DBFieldList.h"
#include "DBConnect.h"

#include "RecentData.h"
#include "RecentDataList.h"

BEGIN_CUTLEADER_NAMESPACE()

RecentDataListPtr RecentDataLoader::LoadRecentData(DBConnectPtr pDBConnect)
{
	RecentDataListPtr pRecentDataList(new RecentDataList);

	CString strSQL2 = _T("select * from dataMgr_recentDataList;");
	int iQueryIndex = pDBConnect->ExecuteQuery(strSQL2);
	while (DBRecordPtr pDBRecord = pDBConnect->GetNextRecord(iQueryIndex))
	{
		DBFieldListPtr pDBFieldList = pDBRecord->GetDBFieldList();

		RecentDataPtr pRecentData(new RecentData);

		// ID.
		LONGLONG iID = pDBFieldList->at(0)->GetFieldVal().llVal;
		pRecentData->SetID(iID);

		// the path of the data.
		CString strPath(pDBFieldList->at(1)->GetFieldVal().bstrVal);
		pRecentData->SetDataPath(strPath);

		// the data ID.
		LONGLONG iDataID = pDBFieldList->at(2)->GetFieldVal().llVal;
		pRecentData->SetDataID(iDataID);

		pRecentDataList->push_back(pRecentData);
	}

	return pRecentDataList;
}

END_CUTLEADER_NAMESPACE()

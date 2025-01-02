#pragma once

#include "clDataManagerCommon.h"

DECLARE_CUTLEADER_CLASS(DBConnect)
DECLARE_CUTLEADER_CLASS(RecentDataList)

BEGIN_CUTLEADER_NAMESPACE()

class ClDataManager_Export RecentDataLoader
{
public:
	// load all recent data from the database.
	// notes:
	//   1) if the table not exist, create it.
	static RecentDataListPtr LoadRecentData(DBConnectPtr pDBConnect);
};

END_CUTLEADER_NAMESPACE()

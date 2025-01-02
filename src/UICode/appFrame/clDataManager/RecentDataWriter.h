#pragma once

#include "clDataManagerCommon.h"

DECLARE_CUTLEADER_CLASS(DBConnect)
DECLARE_CUTLEADER_CLASS(RecentDataList)

BEGIN_CUTLEADER_NAMESPACE()

class ClDataManager_Export RecentDataWriter
{
public:
	static void SaveRecentData(DBConnectPtr pDBConnect, RecentDataListPtr pRecentDataList);
};

END_CUTLEADER_NAMESPACE()

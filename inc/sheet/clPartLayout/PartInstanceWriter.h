#pragma once

#include "clPartLayoutCommon.h"

DECLARE_CUTLEADER_CLASS(DBConnect)
DECLARE_CUTLEADER_CLASS(PartInstanceList)

BEGIN_CUTLEADER_NAMESPACE()

// use this class to write part instance into database.
// notes: 
//   1) we should not save the cut loop instances in part instance, because they will be saved in sheet.
class ClPartLayout_Export PartInstanceWriter
{
public:
	static void SavePartInstList(DBConnectPtr pDBConnect, LONGLONG iSheetId, PartInstanceListPtr pPartInstances);	
};

END_CUTLEADER_NAMESPACE()

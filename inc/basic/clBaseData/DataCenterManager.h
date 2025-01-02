#pragma once

#include "clBaseDataCommon.h"
#include "DataItem.h"

DECLARE_CUTLEADER_CLASS(DataCenterItemList)
DECLARE_CUTLEADER_CLASS(FolderNode)

BEGIN_CUTLEADER_NAMESPACE()

class ClBaseData_Export DataCenterManager
{
public:
	// search data items from the folder item using the searching condition.
	// notes:
	//   1) search recursively.
	//   2) for time comparison, only compare date.
	static DataCenterItemListPtr SearchDataItem(FolderNodePtr pFolderNode, BOOL bDataType, DATANODE_TYPE iDataNodeType, 
		BOOL bName, CString strName, BOOL bCreateTime, long long iCreateTime, BOOL bModifyTime, long long iModifyTime);

	// search data items from the folder item using the searching condition.
	// notes:
	//   1) search recursively.
	static DataCenterItemListPtr SearchDataItem(FolderNodePtr pFolderNode, BOOL bDataType, DATANODE_TYPE iDataNodeType, BOOL bName, CString strName);
};

END_CUTLEADER_NAMESPACE()

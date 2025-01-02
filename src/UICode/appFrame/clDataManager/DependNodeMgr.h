#pragma once

#include "clDataManagerCommon.h"
#include "DataItem.h"
#include "PartWriter.h"
#include "SheetManager.h"
#include "NestJobMgr.h"

DECLARE_CUTLEADER_CLASS(DataCenterItemList)
DECLARE_CUTLEADER_CLASS(DataItem)
DECLARE_CUTLEADER_CLASS(FolderNode)
DECLARE_CUTLEADER_CLASS(DependNode)
DECLARE_CUTLEADER_CLASS(DependNodeList)

BEGIN_CUTLEADER_NAMESPACE()

// this class will manage the dependence relationship.
class ClDataManager_Export DependNodeMgr
{
public:
	// get the dependence tree of "pDataItem".
	static DependNodePtr GetDependTree(const DataCenterItemList* pAllDataItems, DataItemPtr pDataItem);

	// get the dependence tree of all data items under "pFolderNode".
	static DependNodeListPtr GetDependTree(const DataCenterItemList* pAllDataItemList, const FolderNode* pFolderNode);

	// get the dependence tree of all data items in "pDataItemList".
	static DependNodeListPtr GetDependTree(const DataCenterItemList* pAllDataItemList, const DataCenterItemList* pDataItemList);
};

END_CUTLEADER_NAMESPACE()

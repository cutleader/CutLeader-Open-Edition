#pragma once

#include "clBaseDataCommon.h"
#include "DataItem.h"
#include <vector>


DECLARE_CUTLEADER_CLASS(DataCenterItemList)
DECLARE_CUTLEADER_CLASS(IDataCenterItem)


BEGIN_CUTLEADER_NAMESPACE()


class ClBaseData_Export DataCenterItemList : public std::vector<IDataCenterItemPtr>
{
public:
	DataCenterItemList(void);
	~DataCenterItemList(void);

public:
	/************************************************************************/
	// about getting data item.

	// get all DataItem.
	// notes:
	//   1) this function should be a recursive one.
	//   2) caller must allocate "pDataItemList".
	void GetAllDataItem(DataCenterItemListPtr pDataItemList);

	// get the data items by the type.
	// notes:
	//   1) this is not a recursive function.
	DataCenterItemListPtr GetItemByType(DATANODE_TYPE dataNodeType) const;

	// get data item by data ID.
	IDataCenterItemPtr GetItemByDataID(LONGLONG iDataID) const;
	/************************************************************************/


	// get FolderNode.
	// notes:
	//   1) this function is not a recursive one.
	DataCenterItemListPtr GetFolderNodeList();

	// get item by item ID.
	IDataCenterItemPtr GetItemByID(LONGLONG iItemID) const;

	void RemoveItem(LONGLONG iItemID);
};

END_CUTLEADER_NAMESPACE()

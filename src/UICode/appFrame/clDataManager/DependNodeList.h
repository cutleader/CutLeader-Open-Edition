#pragma once

#include "clDataManagerCommon.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(IntegerList)
DECLARE_CUTLEADER_CLASS(DataCenterItemList)
DECLARE_CUTLEADER_CLASS(DependNode)

BEGIN_CUTLEADER_NAMESPACE()

// the list of the DependNode.
class DependNodeList : public std::vector<DependNodePtr>
{
public:
	DependNodeList(void);
	~DependNodeList(void);

public:
	/************************************************************************/
	// about the data items in the dependence tree.

	// get all data items in the dependence trees.
	// notes:
	//   1) filter out the same DataItem.
	DataCenterItemListPtr GetAllDataItem();

	// get all IDs of data item.
	// notes:
	//   1) filter out the same DataItem.
	IntegerListPtr GetAllDataItemID();
	/************************************************************************/

	// check whether there are some data items which depend on "at(i).m_pDataItem".
	BOOL HasDependence();
};

END_CUTLEADER_NAMESPACE()

#pragma once

#include "ClDataManagerCommon.h"


DECLARE_CUTLEADER_CLASS(IntegerList)
DECLARE_CUTLEADER_CLASS(DataItem)
DECLARE_CUTLEADER_CLASS(DataCenterItemList)
DECLARE_CUTLEADER_CLASS(DependNodeList)


BEGIN_CUTLEADER_NAMESPACE()


// the interface for dependence node(a node in the dependence tree).
class DependNode
{
public:
	DependNode(void);
	~DependNode(void);

public: // get/set functions.
	DataItemPtr GetDataItem() const { return m_pDataItem; }
	void SetDataItem(DataItemPtr pDataItem) { m_pDataItem = pDataItem; }

	DependNodeListPtr GetDependNodeList() const { return m_pDependNodeList; }

public: // other functions.
	/************************************************************************/
	// about the data items in the dependence tree.

	// get all IDs of data item.
	IntegerListPtr GetAllDataItemID() const;

	// get all data items in the dependence tree.
	DataCenterItemListPtr GetAllDataItem() const;
	/************************************************************************/


	// check whether there are some data items which depend on "m_pDataItem".
	BOOL HasDependence() const;

private:
	DataItemPtr m_pDataItem;

	// the nodes which depend on this node.
	// notes: allocate the memory inside the class.
	DependNodeListPtr m_pDependNodeList;
};

END_CUTLEADER_NAMESPACE()

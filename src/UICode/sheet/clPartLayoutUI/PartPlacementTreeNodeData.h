#pragma once

#include "clPartLayoutUICommon.h"

BEGIN_CUTLEADER_NAMESPACE()

// the node type of the part pmt tree.
typedef enum tagPmtTreeNodeType
{
	// the root node(first layer). 
	PMT_TREE_NODE_ROOT				= 0,

	// the part node(second layer).
	PMT_TREE_NODE_PART				= 1,

	// the part pmt node(third layer).
	PMT_TREE_NODE_PMT_ITEM			= 2,

} PMT_TREE_NODE_TYPE;

// the data used to describe a pattern tree node.
class PartPlacementTreeNodeData
{
public:
	PartPlacementTreeNodeData(void);
	~PartPlacementTreeNodeData(void);

public: // get/set functions.
	void SetPmtTreeNodeType(PMT_TREE_NODE_TYPE pmtTreeNodeType) { m_pmtTreeNodeType = pmtTreeNodeType; }
	PMT_TREE_NODE_TYPE GetPmtTreeNodeType() const { return m_pmtTreeNodeType; }

	void SetNodeData(LONGLONG iNodeData) { m_iNodeData = iNodeData; }
	LONGLONG GetNodeData()const { return m_iNodeData; }

private:
	// the node type of the part pmt tree.
	PMT_TREE_NODE_TYPE m_pmtTreeNodeType;

	// the node data.
	// notes:
	//  1) for "PMT_TREE_NODE_ROOT", this will be "-1".
	//  2) for "PMT_TREE_NODE_PART", this will be "-1".
	//  3) for "PMT_TREE_NODE_PMT_ITEM", this will be the part pmt ID.
	LONGLONG m_iNodeData;
};

END_CUTLEADER_NAMESPACE()

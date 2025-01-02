#pragma once

#include "clCutSequenceUICommon.h"

BEGIN_CUTLEADER_NAMESPACE()

// the node type.
typedef enum tagSeqTreeNodeType
{
	// the root node(first layer). 
	SEQ_TREE_NODE_ROOT				= 0,

	// the seqUnit type node(second layer).
	SEQ_TREE_NODE_SEQ_TYPE			= 1,

	// the seqUnit node(third layer).
	SEQ_TREE_NODE_SEQ_ITEM			= 2,

} SEQ_TREE_NODE_TYPE;

// the data used to describe a node on sequence unit tree.
class SequenceTreeNodeData
{
public:
	SequenceTreeNodeData(void);
	~SequenceTreeNodeData(void);

public: // get/set functions.
	void SetSeqTreeNodeType(SEQ_TREE_NODE_TYPE seqTreeNodeType) { m_seqTreeNodeType = seqTreeNodeType; }
	SEQ_TREE_NODE_TYPE GetSeqTreeNodeType() const { return m_seqTreeNodeType; }

	void SetNodeData(LONGLONG iNodeData) { m_iNodeData = iNodeData; }
	LONGLONG GetNodeData() const { return m_iNodeData; }

private:
	// the node type.
	SEQ_TREE_NODE_TYPE m_seqTreeNodeType;

	// the node data.
	// notes:
	//  1) for "SEQ_TREE_NODE_ROOT", this will be "-1".
	//  2) for "SEQ_TREE_NODE_SEQ_TYPE", this will be "-1".
	//  3) for "SEQ_TREE_NODE_SEQ_ITEM", this will be the seqUnit ID.
	LONGLONG m_iNodeData;
};

END_CUTLEADER_NAMESPACE()

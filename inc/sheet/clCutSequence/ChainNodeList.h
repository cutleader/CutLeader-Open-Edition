#pragma once

#include "clCutSequenceCommon.h"
#include "Point2D.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(IChainNode)
DECLARE_CUTLEADER_CLASS(ChainNodeList)

BEGIN_CUTLEADER_NAMESPACE()

class ClCutSequence_Export ChainNodeList : public std::vector<IChainNodePtr>
{
public:
	ChainNodeList(void);
	~ChainNodeList(void);

public:
	// 这个节点是否存在。
	BOOL IsNodeExist(LONGLONG iNodeID) const;

	// 得到节点索引。
	int GetNodeIndex(IChainNodePtr pChainNode) const;

	// 删除“连割节点”。
	void DelChainNode(LONGLONG iNodeID);

	// 在“iNextNodeID”前插入一个节点。
	// 注：
	//  1) -1，插到最后。
	void InsertNode(LONGLONG iNextNodeID, IChainNodePtr pChainNode);

	// 在连割节点前面插入一些连割节点
	void InsertChainNode_before_chainNode(IChainNodePtr pChainNode, ChainNodeListPtr pChainNodeList);

	// 得到距离点最近的节点。
	IChainNodePtr GetClosestNode(const Point2D& pt) const;

	// 得到该节点的下一个节点。
	// 注：
	//  1) 如果该节点已经是最后一个，返回-1。
	LONGLONG GetNextNodeID(IChainNodePtr pChainNode) const;
};

END_CUTLEADER_NAMESPACE()

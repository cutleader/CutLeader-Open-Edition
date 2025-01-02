#pragma once

#include "clCutSequenceCommon.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(LoopInstance)
DECLARE_CUTLEADER_CLASS(BridgeNode)

BEGIN_CUTLEADER_NAMESPACE()

// “桥接节点”集合。
class BridgeNodeList : public std::vector<BridgeNodePtr>
{
public:
	BridgeNodeList(void);
	~BridgeNodeList(void);

public:
	// 根据“回路实例”删除“桥接节点”。
	// 注：
	//   1) 这不是递归函数。
	void DelBdgeNodeByLoop(LoopInstancePtr pLoopInstance);

	// 删除指定的“桥接节点”。
	// 注：
	//   1) 这不是递归函数。
	void DelBdgeNodeByID(LONGLONG iBdgeNodeID);
};

END_CUTLEADER_NAMESPACE()

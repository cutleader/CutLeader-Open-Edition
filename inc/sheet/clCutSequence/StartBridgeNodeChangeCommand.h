#pragma once

#include "SequenceCommandBase.h"

DECLARE_CUTLEADER_CLASS(BridgeSequence)
DECLARE_CUTLEADER_CLASS(BridgeNode)

BEGIN_CUTLEADER_NAMESPACE()

// 修改“桥接工序”的起始“回路实例”。
class ClCutSequence_Export StartBridgeNodeChangeCommand : public SequenceCommandBase
{
public:
	StartBridgeNodeChangeCommand(EditorData editorData, CutSequenceListPtr pCutSequences, ConfigDataPtr pConfigData, BridgeSequencePtr pBridgeSequence, BridgeNodePtr pBridgeNode);
	virtual ~StartBridgeNodeChangeCommand(void);

public:
	virtual void Do();
	virtual void UnDo();

private:
	// 要修改的“桥接工序”。
	BridgeSequencePtr m_pBridgeSequence;

	// 把这个“回路实例”设为起始的。
	BridgeNodePtr m_pBridgeNode;

	/************************************************************************/
	// 回退命令时可能会用到的数据。

	// “桥接工序”在原工序集合中的位置。
	int m_iExistSeqIndex;

	// 老的起始节点。
	BridgeNodePtr m_pOldStartNode;
	/************************************************************************/
};

END_CUTLEADER_NAMESPACE()

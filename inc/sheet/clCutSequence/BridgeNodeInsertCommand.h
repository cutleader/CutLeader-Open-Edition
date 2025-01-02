#pragma once

#include "SequenceCommandBase.h"

DECLARE_CUTLEADER_CLASS(LoopInstance)
DECLARE_CUTLEADER_CLASS(PartInstanceList)
DECLARE_CUTLEADER_CLASS(BridgeSequence)
DECLARE_CUTLEADER_CLASS(BridgeNode)
DECLARE_CUTLEADER_CLASS(BridgePt)
DECLARE_CUTLEADER_CLASS(OutLoopAdder)
DECLARE_CUTLEADER_CLASS(HostSeqMover)

BEGIN_CUTLEADER_NAMESPACE()

// 在“桥接节点”前添加一个“回路实例”作为“父亲桥接节点”。
// 注：
//  1) 插入桥接的“回路实例”上可能有其他工序，此时需要删掉那个工序。
//  2) 新插入的“桥接节点”变成了“根节点”。
class ClCutSequence_Export BridgeNodeInsertCommand : public SequenceCommandBase
{
public:
	BridgeNodeInsertCommand(EditorData editorData, CutSequenceListPtr pCutSequences, ConfigDataPtr pConfigData,
					  PartInstanceListPtr pPartInstances, BridgeSequencePtr pBridgeSequence, BridgeNodePtr pBridgeNode, BridgePtPtr pBridgePt, LoopInstancePtr pLoopInstance, BridgePtPtr pParentBdgePt);
	~BridgeNodeInsertCommand(void);

public:
	virtual void Do();
	virtual void UnDo();

private:
	// 当前修改的“桥接工序”。
	BridgeSequencePtr m_pBdgeSeq;

	// 在这个“桥接节点”前插入新的节点。
	BridgeNodePtr m_pBridgeNode;
	BridgePtPtr m_pBridgePt;

	// 基于这个“回路实例”新建“桥接节点”。
	LoopInstancePtr m_pLoopInstance;
	BridgePtPtr m_pParentBdgePt;

	// 已有“桥接工序”的切割方向要不要调整。
	BOOL m_bReverseCutDir;

	// 原有的根节点，回退命令时用到。
	BridgeNodePtr m_pRootBdgeNode;

	// 工序调整器。
	OutLoopAdderPtr m_pOutLoopAdder;
	HostSeqMoverPtr m_pHostSeqMover;
};

END_CUTLEADER_NAMESPACE()

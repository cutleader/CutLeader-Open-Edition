#pragma once

#include "SequenceCommandBase.h"

DECLARE_CUTLEADER_CLASS(PartInstanceList)
DECLARE_CUTLEADER_CLASS(LoopInstance)
DECLARE_CUTLEADER_CLASS(BridgeSequence)
DECLARE_CUTLEADER_CLASS(BridgeNode)
DECLARE_CUTLEADER_CLASS(BridgePt)
DECLARE_CUTLEADER_CLASS(OutLoopAdder)
DECLARE_CUTLEADER_CLASS(HostSeqMover)

BEGIN_CUTLEADER_NAMESPACE()

// 在“桥接节点”下添加一个“回路实例”作为“子桥接节点”。
// 注：
//  1) 加入桥接的“回路实例”上可能有其他工序，此时需要删掉那个工序。
class ClCutSequence_Export BridgeNodeAddCommand : public SequenceCommandBase
{
public:
	BridgeNodeAddCommand(const EditorData& editorData, CutSequenceListPtr pCutSequences, ConfigDataPtr pConfigData,
		PartInstanceListPtr pPartInstances, BridgeSequencePtr pBridgeSequence, BridgeNodePtr pParentBdgeNode, BridgePtPtr pParentBridgePt,
		LoopInstancePtr pLoopInstance, BridgePtPtr pChildBdgePt);
	~BridgeNodeAddCommand(void);

public:
	virtual void Do() override;
	virtual void UnDo() override;

private:
	// 当前修改的“桥接工序”。
	BridgeSequencePtr m_pBdgeSeq;

	// 在这个“桥接节点”上挂新的节点。
	BridgeNodePtr m_pParentBdgeNode;
	BridgePtPtr m_pParentBridgePt;

	// 基于这个“回路实例”新建“桥接节点”。
	LoopInstancePtr m_pLoopInstance;
	BridgePtPtr m_pChildBdgePt;

	// 这个“回路实例”的切割方向要不要调整。
	BOOL m_bReverseCutDir;

	// 工序调整器。
	OutLoopAdderPtr m_pOutLoopAdder;
	HostSeqMoverPtr m_pHostSeqMover;
};

END_CUTLEADER_NAMESPACE()

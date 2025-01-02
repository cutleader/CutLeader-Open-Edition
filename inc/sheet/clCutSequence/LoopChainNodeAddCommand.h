#pragma once

#include "SequenceCommandBase.h"
#include "LoopPt.h"

DECLARE_CUTLEADER_CLASS(LoopPt)
DECLARE_CUTLEADER_CLASS(PartPlacementList)
DECLARE_CUTLEADER_CLASS(PartInstanceList)
DECLARE_CUTLEADER_CLASS(LoopInstanceList)
DECLARE_CUTLEADER_CLASS(LoopChainNode)
DECLARE_CUTLEADER_CLASS(ChainNodeList)
DECLARE_CUTLEADER_CLASS(ChainCutSequence)
DECLARE_CUTLEADER_CLASS(StartPtMover)
DECLARE_CUTLEADER_CLASS(OutLoopAdder)
DECLARE_CUTLEADER_CLASS(HostSeqMover)

BEGIN_CUTLEADER_NAMESPACE()

// 添加一个“连割轮廓”节点。
class ClCutSequence_Export LoopChainNodeAddCommand : public SequenceCommandBase
{
public:
	LoopChainNodeAddCommand(EditorData editorData, CutSequenceListPtr pCutSequences, ConfigDataPtr pConfigData, PartPlacementListPtr pPartPlacements,
					PartInstanceListPtr pPartInstances, LoopInstanceListPtr pOuterLoopInsts, ChainCutSequencePtr pChainSeq, LoopChainNodePtr pChainLoopNode, LONGLONG iNextNodeID, LoopPtPtr pLoopPt);
	virtual ~LoopChainNodeAddCommand(void);

public:
	virtual void Do();
	virtual void UnDo();

private:
	ChainCutSequencePtr m_pChainSeq;

	// 要新加的“连割回路”节点。
	LoopChainNodePtr m_pChainLoopNode;
	ChainNodeListPtr m_pChainNodes_avoid; // 当在一个“连割轮廓”后加一个“连割轮廓”时，需要考虑避让路径。

	// 插到这个节点之前，-1的话插到最后。
	LONGLONG m_iNextNodeID;

	// 这个“回路实例”的切割方向要不要调整。
	BOOL m_bReverseCutDir;

	// 对起切点和工序的调整器。
	StartPtMoverPtr m_pStartPtMover; // 不需调整时这个对象为空。
	OutLoopAdderPtr m_pOutLoopAdder;
	HostSeqMoverPtr m_pHostSeqMover;

	LoopInstanceListPtr m_pOuterLoopInsts;
	PartPlacementListPtr m_pPartPmts;
	PartInstanceListPtr m_pPartInsts;
};

END_CUTLEADER_NAMESPACE()

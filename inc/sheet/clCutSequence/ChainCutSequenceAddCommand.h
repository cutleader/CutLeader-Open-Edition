#pragma once

#include "SequenceCommandBase.h"
#include "LoopPt.h"

DECLARE_CUTLEADER_CLASS(LoopPt)
DECLARE_CUTLEADER_CLASS(PartPlacementList)
DECLARE_CUTLEADER_CLASS(PartInstanceList)
DECLARE_CUTLEADER_CLASS(LoopInstanceList)
DECLARE_CUTLEADER_CLASS(ChainCutSequence)
DECLARE_CUTLEADER_CLASS(StartPtMover)
DECLARE_CUTLEADER_CLASS(OutLoopAdder)

BEGIN_CUTLEADER_NAMESPACE()

// 用“连割点”和“回路实例”创建“连割工序”。
// 注：
//  1) 此时送进来的pChainSeq中最多只有一个“连割轮廓”，“连割点”可能多个
class ClCutSequence_Export ChainCutSequenceAddCommand : public SequenceCommandBase
{
public:
	ChainCutSequenceAddCommand(EditorData editorData, CutSequenceListPtr pCutSequences, ConfigDataPtr pConfigData, PartPlacementListPtr pPartPlacements, PartInstanceListPtr pPartInstances,
		LoopInstanceListPtr pOuterLoopInsts, ChainCutSequencePtr pChainSeq, LoopPtPtr pLoopPt);
	~ChainCutSequenceAddCommand(void);

public:
	virtual void Do();
	virtual void UnDo();

private:
	// “连割工序”。
	ChainCutSequencePtr m_pChainSeq;

	// 连割工序插在这个工序前面，-1就插到最后。
	LONGLONG m_iChainNextSeqID;

	// 对起切点修改和工序调整的帮助类。
	StartPtMoverPtr m_pStartPtMover; // 不需调整时这个对象为空。
	OutLoopAdderPtr m_pOutLoopAdder;

	LoopInstanceListPtr m_pOuterLoopInsts;
	PartPlacementListPtr m_pPartPmts;
	PartInstanceListPtr m_pPartInsts;
};

END_CUTLEADER_NAMESPACE()

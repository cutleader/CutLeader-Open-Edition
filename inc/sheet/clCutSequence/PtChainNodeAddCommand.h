#pragma once

#include "SequenceCommandBase.h"
#include "StartPtMover.h"

DECLARE_CUTLEADER_CLASS(PtChainNode)
DECLARE_CUTLEADER_CLASS(ChainCutSequence)

BEGIN_CUTLEADER_NAMESPACE()

// 添加一个“连割点”节点。
class ClCutSequence_Export PtChainNodeAddCommand : public SequenceCommandBase
{
public:
	PtChainNodeAddCommand(EditorData editorData, CutSequenceListPtr pCutSequences, ConfigDataPtr pConfigData, ChainCutSequencePtr pChainSeq, PtChainNodePtr pChainPtNode, LONGLONG iNextNodeID);
	~PtChainNodeAddCommand(void);

public:
	virtual void Do();
	virtual void UnDo();

private:
	ChainCutSequencePtr m_pChainSeq;

	// 要新加的“连割点”节点。
	PtChainNodePtr m_pChainPtNode;

	// 插到这个节点之前，-1的话插到最后。
	LONGLONG m_iNextNodeID;
};

END_CUTLEADER_NAMESPACE()

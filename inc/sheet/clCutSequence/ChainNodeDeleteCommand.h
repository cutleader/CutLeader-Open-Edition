#pragma once

#include "SequenceCommandBase.h"

DECLARE_CUTLEADER_CLASS(ChainCutSequence)
DECLARE_CUTLEADER_CLASS(IChainNode)

BEGIN_CUTLEADER_NAMESPACE()

// 删除一个“连割节点”。
class ClCutSequence_Export ChainNodeDeleteCommand : public SequenceCommandBase
{
public:
	ChainNodeDeleteCommand(EditorData editorData, CutSequenceListPtr pCutSequences, ConfigDataPtr pConfigData, ChainCutSequencePtr pChainSeq, IChainNodePtr pChainNode);
	~ChainNodeDeleteCommand(void);

public:
	virtual void Do();
	virtual void UnDo();

private:
	ChainCutSequencePtr m_pChainSeq;

	// 删除的节点。
	IChainNodePtr m_pChainNode;

	// 删除前节点的位置。
	int m_iPos;
};

END_CUTLEADER_NAMESPACE()

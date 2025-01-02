#pragma once

#include "SequenceCommandBase.h"

DECLARE_CUTLEADER_CLASS(ChainCutSequence)
DECLARE_CUTLEADER_CLASS(IChainNode)

BEGIN_CUTLEADER_NAMESPACE()

// ɾ��һ��������ڵ㡱��
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

	// ɾ���Ľڵ㡣
	IChainNodePtr m_pChainNode;

	// ɾ��ǰ�ڵ��λ�á�
	int m_iPos;
};

END_CUTLEADER_NAMESPACE()

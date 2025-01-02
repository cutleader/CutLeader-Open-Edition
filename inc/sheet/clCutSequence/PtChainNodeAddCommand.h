#pragma once

#include "SequenceCommandBase.h"
#include "StartPtMover.h"

DECLARE_CUTLEADER_CLASS(PtChainNode)
DECLARE_CUTLEADER_CLASS(ChainCutSequence)

BEGIN_CUTLEADER_NAMESPACE()

// ���һ��������㡱�ڵ㡣
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

	// Ҫ�¼ӵġ�����㡱�ڵ㡣
	PtChainNodePtr m_pChainPtNode;

	// �嵽����ڵ�֮ǰ��-1�Ļ��嵽���
	LONGLONG m_iNextNodeID;
};

END_CUTLEADER_NAMESPACE()

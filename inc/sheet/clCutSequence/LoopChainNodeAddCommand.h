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

// ���һ���������������ڵ㡣
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

	// Ҫ�¼ӵġ������·���ڵ㡣
	LoopChainNodePtr m_pChainLoopNode;
	ChainNodeListPtr m_pChainNodes_avoid; // ����һ�����������������һ��������������ʱ����Ҫ���Ǳ���·����

	// �嵽����ڵ�֮ǰ��-1�Ļ��嵽���
	LONGLONG m_iNextNodeID;

	// �������·ʵ�������и��Ҫ��Ҫ������
	BOOL m_bReverseCutDir;

	// �����е�͹���ĵ�������
	StartPtMoverPtr m_pStartPtMover; // �������ʱ�������Ϊ�ա�
	OutLoopAdderPtr m_pOutLoopAdder;
	HostSeqMoverPtr m_pHostSeqMover;

	LoopInstanceListPtr m_pOuterLoopInsts;
	PartPlacementListPtr m_pPartPmts;
	PartInstanceListPtr m_pPartInsts;
};

END_CUTLEADER_NAMESPACE()

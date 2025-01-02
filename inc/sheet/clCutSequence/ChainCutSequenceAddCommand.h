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

// �á�����㡱�͡���·ʵ��������������򡱡�
// ע��
//  1) ��ʱ�ͽ�����pChainSeq�����ֻ��һ����������������������㡱���ܶ��
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
	// ������򡱡�
	ChainCutSequencePtr m_pChainSeq;

	// ���������������ǰ�棬-1�Ͳ嵽���
	LONGLONG m_iChainNextSeqID;

	// �����е��޸ĺ͹�������İ����ࡣ
	StartPtMoverPtr m_pStartPtMover; // �������ʱ�������Ϊ�ա�
	OutLoopAdderPtr m_pOutLoopAdder;

	LoopInstanceListPtr m_pOuterLoopInsts;
	PartPlacementListPtr m_pPartPmts;
	PartInstanceListPtr m_pPartInsts;
};

END_CUTLEADER_NAMESPACE()

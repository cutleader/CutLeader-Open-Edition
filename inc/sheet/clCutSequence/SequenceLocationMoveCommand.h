#pragma once

#include "SequenceCommandBase.h"

DECLARE_CUTLEADER_CLASS(PartInstanceList)
DECLARE_CUTLEADER_CLASS(ICutSequence)

BEGIN_CUTLEADER_NAMESPACE()

// 把工序调整到“基准工序”后面。
class ClCutSequence_Export SequenceLocationMoveCommand : public SequenceCommandBase
{
public:
	SequenceLocationMoveCommand(EditorData editorData, CutSequenceListPtr pCutSequences, ConfigDataPtr pConfigData, CutSequenceListPtr pMovCutSeqs, ICutSequencePtr pBaseCutSeq, PartInstanceListPtr pAllPartInst);
	virtual ~SequenceLocationMoveCommand(void);

public:
	virtual void Do();
	virtual void UnDo();

private:
	// 要调整位置的工序。
	CutSequenceListPtr m_pMovCutSeqs;

	// 调整到这个工序后面。
	// 注：
	//  1) 为空时调整到开始位置。
	//  2) 这个工序不能在移动的工序中。
	ICutSequencePtr m_pBaseCutSequence;

	// 工序移动之前的位置。
	std::vector<int> m_iOldPosList;

	// 所有的零件实例。
	PartInstanceListPtr m_pAllPartInsts;
};

END_CUTLEADER_NAMESPACE()

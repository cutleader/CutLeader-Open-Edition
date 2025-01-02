#pragma once

#include "SequenceCommandBase.h"

BEGIN_CUTLEADER_NAMESPACE()

// �����¹�������
class ClCutSequence_Export LoopSequenceAddCommand : public SequenceCommandBase
{
public:
	LoopSequenceAddCommand(EditorData editorData, CutSequenceListPtr pCutSequences, ConfigDataPtr pConfigData, CutSequenceListPtr pNewCutSequences);
	virtual ~LoopSequenceAddCommand(void);

public:
	virtual void Do();
	virtual void UnDo();

private:
	// the added sequences.
	CutSequenceListPtr m_pNewCutSequences;
};

END_CUTLEADER_NAMESPACE()

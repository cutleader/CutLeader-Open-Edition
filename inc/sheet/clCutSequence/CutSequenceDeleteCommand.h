#pragma once

#include "SequenceCommandBase.h"

DECLARE_CUTLEADER_CLASS(CutSequenceDeleteCommand)

BEGIN_CUTLEADER_NAMESPACE()

class ClCutSequence_Export CutSequenceDeleteCommand : public SequenceCommandBase
{
public:
	CutSequenceDeleteCommand(EditorData editorData, CutSequenceListPtr pCutSequences, ConfigDataPtr pConfigData, CutSequenceListPtr pDelCutSeqs);
	~CutSequenceDeleteCommand(void);

public:
	virtual void Do();
	virtual void UnDo();

private:
	// the deleted sequence unit.
	CutSequenceListPtr m_pDelCutSeqs;

	// keep their position before deleting them.
	std::vector<int> m_iInsertPosList;
};

END_CUTLEADER_NAMESPACE()

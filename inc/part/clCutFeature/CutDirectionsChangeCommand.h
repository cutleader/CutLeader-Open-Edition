#pragma once

#include "clCutFeatureCommon.h"
#include "CommandBase.h"

DECLARE_CUTLEADER_CLASS(LoopStartCutDataList)

BEGIN_CUTLEADER_NAMESPACE()

// this command can change the cut direction of loops.
class ClCutFeature_Export CutDirectionsChangeCommand : public CommandBase
{
public:
	CutDirectionsChangeCommand(const EditorData& editorData, LoopStartCutDataListPtr pLoopStaEndCutList);
	virtual ~CutDirectionsChangeCommand(void);

public:
	virtual void Do() override;
	virtual void UnDo() override;

private:
	LoopStartCutDataListPtr m_pLoopStaEndCutList;
};

END_CUTLEADER_NAMESPACE()

#pragma once

#include "clCutFeatureCommon.h"
#include "CommandBase.h"

DECLARE_CUTLEADER_CLASS(LoopStartCutData)

BEGIN_CUTLEADER_NAMESPACE()

// this command can change the cut direction of the loop.
class ClCutFeature_Export CutDirectionChangeCommand : public CommandBase
{
public:
	CutDirectionChangeCommand(const EditorData& editorData, LoopStartCutDataPtr pLoopStartCutData);
	~CutDirectionChangeCommand(void);

public:
	virtual void Do() override;
	virtual void UnDo() override;

private:
	LoopStartCutDataPtr m_pLoopStartCutData;
};

END_CUTLEADER_NAMESPACE()

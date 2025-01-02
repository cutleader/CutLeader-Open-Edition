#pragma once

#include "clCutFeatureCommon.h"
#include "PatternPosition.h"
#include "CommandBase.h"

DECLARE_CUTLEADER_CLASS(LoopStartCutData)

BEGIN_CUTLEADER_NAMESPACE()

// 这个命令把起切点从图形上移到图形上。
class ClCutFeature_Export StartCutPtModifyCommand : public CommandBase
{
public:
	StartCutPtModifyCommand(EditorData editorData, LoopStartCutDataPtr pLoopStartCutData, LONGLONG iNewStartPatId, PatternPosition newPatternPosition);
	~StartCutPtModifyCommand(void);

public:
	virtual void Do();
	virtual void UnDo();

private:
	// the loop pierce which will be modified.
	LoopStartCutDataPtr m_pLoopStartCutData;

	// the start cut point.
	LONGLONG m_iStartPatternID;
	PatternPosition m_PatternPosition;
};

END_CUTLEADER_NAMESPACE()

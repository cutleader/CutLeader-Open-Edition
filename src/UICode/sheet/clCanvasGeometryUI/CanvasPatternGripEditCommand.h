#pragma once

#include "clCanvasGeometryUICommon.h"
#include "CanvasPatternEditCommandBase.h"

DECLARE_CUTLEADER_CLASS(IPattern)
DECLARE_CUTLEADER_CLASS(PartCadData)
DECLARE_CUTLEADER_CLASS(PartCamData)

BEGIN_CUTLEADER_NAMESPACE()

// this command can grip-edit the patterns.
class ClCanvasGeometryUI_Export CanvasPatternGripEditCommand : public CanvasPatternEditCommandBase
{
public:
	CanvasPatternGripEditCommand(const EditorData& editorData, PartCadDataPtr pPartCadData, PartCamDataPtr pPartCamData,
		LONGLONG iEditingPatternID, IPatternPtr pTmpPattern);
	~CanvasPatternGripEditCommand(void);

public:
	virtual void Do();
	virtual void UnDo();

private:
	LONGLONG m_iEditingPatternID;
	IPatternPtr m_pTmpPattern;
};

END_CUTLEADER_NAMESPACE()

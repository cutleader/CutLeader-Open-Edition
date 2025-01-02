#pragma once

#include "clCanvasGeometryUICommon.h"
#include "CanvasPatternEditCommandBase.h"

DECLARE_CUTLEADER_CLASS(PartCadData)
DECLARE_CUTLEADER_CLASS(IPattern)
DECLARE_CUTLEADER_CLASS(PartCamData)

BEGIN_CUTLEADER_NAMESPACE()

// the command will change the prop of pattern.
class ClCanvasGeometryUI_Export CanvasPatternPropertyEditCommand : public CanvasPatternEditCommandBase
{
public:
	CanvasPatternPropertyEditCommand(const EditorData& editorData, PartCadDataPtr pPartCadData, PartCamDataPtr pPartCamData,
		LONGLONG iEditingPatternID, const DataPropItem& newPropItem);
	~CanvasPatternPropertyEditCommand(void);

public:
	virtual void Do();
	virtual void UnDo();

private:
	LONGLONG m_iEditingPatternID;
	DataPropItem m_newPropItem;
};

END_CUTLEADER_NAMESPACE()

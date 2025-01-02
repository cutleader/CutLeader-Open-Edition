#pragma once

#include "clPartLayoutCommon.h"
#include "CanvasPatternEditCommandBase.h"
#include "Matrix2D.h"

DECLARE_CUTLEADER_CLASS(PatternLoopList)
DECLARE_CUTLEADER_CLASS(PartCadData)
DECLARE_CUTLEADER_CLASS(PartCamData)
DECLARE_CUTLEADER_CLASS(PolygonPatternLoop)

BEGIN_CUTLEADER_NAMESPACE()

class CanvasLoopSizeChangeCommand : public CanvasPatternEditCommandBase
{
public:
	CanvasLoopSizeChangeCommand(const EditorData& editorData, PartCadDataPtr pPartCadData, PartCamDataPtr pPartCamData,
		PolygonPatternLoopPtr pPolygonPatternLoop, double dNewLoopWidth, double dNewLoopHeight);
	~CanvasLoopSizeChangeCommand(void);

public:
	virtual void Do() override;
	virtual void UnDo() override;

private:
	PolygonPatternLoopPtr m_pPolygonPatternLoop;
	double m_dNewLoopWidth;
	double m_dNewLoopHeight;
};

END_CUTLEADER_NAMESPACE()

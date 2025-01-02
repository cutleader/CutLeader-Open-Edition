#pragma once

#include "clGeometryFeatureCommon.h"

DECLARE_CUTLEADER_CLASS(Point2D)
DECLARE_CUTLEADER_CLASS(GlViewPort)
DECLARE_CUTLEADER_CLASS(IPattern)
DECLARE_CUTLEADER_CLASS(PatternList)

BEGIN_CUTLEADER_NAMESPACE()

// this class will draw the control points of the pattern.
// notes:
//   1) do not draw control points of the control pattern.
//   2) be clear that control point and snap point are different.
//   3) use "DisplayParam::m_drawassistColor" to draw these points, and all draw mode should be maintained in this class.
class ClGeometryFeature_Export SnapPtDrawer
{
public:
	SnapPtDrawer(GlViewPortPtr pViewPort);
	~SnapPtDrawer(void);

public:
	// draw the control points of the pattern.
	// note:
	//   1) use the 10*10 cross lines to indicate the control point.
	void DrawPatListCtrlPt(PatternListPtr pPatList);
	void DrawPatCtrlPt(IPatternPtr pPattern);

	// draw the snap point at "pSnapPt"
	// notes:
	//   1) use a 10*10 filled rect to show the snap point.
	void DrawSnapPt(Point2DPtr pSnapPt, COLORREF color);

private:
	void DrawCtrlPt(IPatternPtr pPattern);

private:
	GlViewPortPtr m_pViewPort;
};

END_CUTLEADER_NAMESPACE()

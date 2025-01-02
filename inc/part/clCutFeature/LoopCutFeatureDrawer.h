#pragma once

#include "clCutFeatureCommon.h"
#include "Matrix2D.h"
#include "BasicGeometryDrawer.h"

DECLARE_CUTLEADER_CLASS(GlViewPort)
DECLARE_CUTLEADER_CLASS(LineArc2DList)
DECLARE_CUTLEADER_CLASS(IPatternLoop)
DECLARE_CUTLEADER_CLASS(LoopCutFeature)
DECLARE_CUTLEADER_CLASS(LoopStartCutData)
DECLARE_CUTLEADER_CLASS(LoopCornerData)
DECLARE_CUTLEADER_CLASS(LoopMicroJoint)

BEGIN_CUTLEADER_NAMESPACE()

// this class will draw loop features.
class ClCutFeature_Export LoopCutFeatureDrawer : public BasicGeometryDrawer
{
public:
	LoopCutFeatureDrawer(GlViewPortPtr pViewPort);
	~LoopCutFeatureDrawer(void);

public:
	// draw cut features of the loop.
	// notes:
	//   1) if the loop rect is within 15*15 pixel, do not draw features.
	void DrawLoopFeature(const LoopCutFeature* pLoopFeature);

	// 绘制切割方向。
	// 注:
	//   1) we should adjust the width of start cut triangle based on the current world coordinate.
	void DrawLoopCutDir(const LoopStartCutData* pLoopStartCutData, const Matrix2D& mat = Matrix2D());

	// 绘制切割结束点
	void DrawLoopEndCutPt(const LoopStartCutData* pLoopStartCutData, const Matrix2D& mat = Matrix2D());

	// 绘制引入引出。
	void DrawLoopLead(const LoopStartCutData* pLoopStartCutData, const Matrix2D& mat = Matrix2D());

	// draw corner features on the loop.
	void DrawLoopCorner(const LoopCornerData* pLoopCorner, BOOL bFollowLoopDirection, const Matrix2D& mat = Matrix2D());

	// draw the MicroJoints on the loop.
	/* params:
	*  bFollowLoopDirection, emPatternLoopCutSide: they come from loop start/end cut info.
	*/
	void DrawMicroJoint(const LoopMicroJoint* pLoopMicroJoint, BOOL bFollowLoopDirection, PatternLoopCutSideType emPatternLoopCutSide, const Matrix2D& mat = Matrix2D());

	// draw the filled rect which stand for a cut feature location.
	// notes:
	//   1) after set draw color to "color", this function will restore the color.
	void DrawFeatureLoc(const Point2D& featureLoc, COLORREF color);

	// draw a ring which stand for a position selection.
	void DrawPosRing(const Point2D& pos);

private:
	void CheckArrayPatternLoop(const IPatternLoop* pPatternLoop, const LineArc2DList* pGeomList, const Matrix2D& mat = Matrix2D());
};

END_CUTLEADER_NAMESPACE()

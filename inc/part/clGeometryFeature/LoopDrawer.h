#pragma once

#include "clGeometryFeatureCommon.h"
#include "BasicGeometryDrawer.h"

DECLARE_CUTLEADER_CLASS(IPatternLoop)
DECLARE_CUTLEADER_CLASS(PatternLoopList)

BEGIN_CUTLEADER_NAMESPACE()

// this class draw the pattern loop.
// notes:
//   1) if the loop rect is within 0*0 ~ 5*5 pixel, only draw points.
//   2) if the loop rect is within 5*5 ~ 10*10 pixel, only draw circles.
class ClGeometryFeature_Export LoopDrawer : public BasicGeometryDrawer
{
public:
	LoopDrawer(GlViewPortPtr pViewPort);
	~LoopDrawer(void);

public:
	// draw pattern loop.
	void DrawPatternLoop(const IPatternLoop* pPatternLoop, Matrix2D mat = Matrix2D());
	void DrawPatternLoopList(const PatternLoopList* pPatternLoopList, Matrix2D mat = Matrix2D());

	// draw the active loop.
	void DrawActiveLoop(const IPatternLoop* pPatternLoop, COLORREF iColor, Matrix2D mat = Matrix2D());

	// draw the selected loop.
	void DrawSelectedLoop(const IPatternLoop* pPatternLoop, COLORREF iColor, Matrix2D mat = Matrix2D());

    // 绘制正常状态的轮廓，所谓正常状态是相对高亮和选中状态而言，使用传入的颜色，用线宽为1的实线进行绘制。
    void DrawNormalStatusLoop(const IPatternLoop* pPatternLoop, COLORREF iColor, Matrix2D mat = Matrix2D());

    // 绘制轮廓数据，调用者负责设置绘制样式。
    void DrawLoopData(const IPatternLoop* pPatternLoop, Matrix2D mat = Matrix2D());

	// 标记开放轮廓。
	void FlagOpenLoops(const PatternLoopList* pPatternLoopList);
};

END_CUTLEADER_NAMESPACE()

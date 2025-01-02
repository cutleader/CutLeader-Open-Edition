#pragma once

#include "clGeometryFeatureCommon.h"
#include "BasicGeometryDrawer.h"

DECLARE_CUTLEADER_CLASS(IPattern)
DECLARE_CUTLEADER_CLASS(PatternList)

BEGIN_CUTLEADER_NAMESPACE()

// this class draw the pattern.
// notes:
//   1) we do not draw the control pattern. 
//   2) we can call GetLineArcs() function of all patterns, and then draw each line and arc.
class ClGeometryFeature_Export PatternDrawer : public BasicGeometryDrawer
{
public:
	PatternDrawer(GlViewPortPtr pViewPort);
	~PatternDrawer(void);

public:
	// draw pattern.
	// 注:
	// 1) color and line width should be set in this function.
	void DrawPat(const IPattern* pPattern);
	void DrawPatList(const PatternList* pPatList);

	// draw active pattern.
	// 注:
	// 1) 用线宽为2的实线显示几何特征，颜色使用系统参数中的。
	void DrawActivePat(const IPattern* pPattern);

	// draw selected pattern.
	// 注:
	// 1) 用线宽为1的虚线显示几何特征，颜色使用系统参数中的。
	void DrawSelPat(const IPattern* pPattern);
	void DrawSelPatList(const PatternList* pPatList);

private:
	// draw pattern.
	void DrawPatData(const IPattern* pPattern);
};

END_CUTLEADER_NAMESPACE()

#pragma once

#include "clPartLayoutCommon.h"
#include "BasicGeometryDrawer.h"

DECLARE_CUTLEADER_CLASS(IPatternLoop)
DECLARE_CUTLEADER_CLASS(PartPlacement)
DECLARE_CUTLEADER_CLASS(PartPlacementList)
DECLARE_CUTLEADER_CLASS(PartInstance)
DECLARE_CUTLEADER_CLASS(PartInstanceList)

BEGIN_CUTLEADER_NAMESPACE()

// this class draw the part placement.
// notes:
//   1) for cam features, we always use the color in the option.
class ClPartLayout_Export PartPlacementDrawer : public BasicGeometryDrawer
{
public:
	PartPlacementDrawer(GlViewPortPtr pViewPort);
	~PartPlacementDrawer(void);

public:
	// draw the part placement.
	// notes: 
	//   1) color and line width should be set in this function.
	void DrawPartPlacement(const PartPlacement* pPartPlacement);
	void DrawPartPlacements(const PartPlacementList* pPartPlacements);

	// draw the active part placement.
	// notes: 
	//   1) set the line width as 2.
	//   2) set the color in this function.
	//   3) only draw geometry feature.
	void DrawActivePartPlacement(const PartPlacement* pPartPlacement);

	// draw selected part placement.
	// notes:
	//   1) set the line width as 1, use stipple mode.
	//   2) set the color in this function.
	//   3) only draw geometry feature.
	void DrawSelectedPartPlacement(const PartPlacement* pPartPlacement);
	void DrawSelectedPartPlacements(const PartPlacementList* pPartPlacements);

	// draw selected part inst list.
	void DrawSelectedPartInstances(const PartInstanceList* pPartInstances);

	// 绘制当前的“零件实例”。
	// 注: 
	//   1) set the line width as 2.
	//   2) set the color in this function.
	//   3) only draw geometry feature.
	void DrawActivePartInstance(const PartInstance* pPartInstance);

private:
	// 标记开放轮廓。
	void FlagOpenLoops(const PartPlacement* pPartPlacement);

	// draw cut feature of the part pmt.
	// notes:
	//   1) if the loop rect is within 15*15 pixel, do not draw features.
	void DrawCutFeature(const PartPlacement* pPartPlacement);

	// whether we need to draw cut feature of the loop.
	BOOL IgnoreCutFeature(const IPatternLoop* pPatternLoop, const Matrix2D& mat);
};

END_CUTLEADER_NAMESPACE()

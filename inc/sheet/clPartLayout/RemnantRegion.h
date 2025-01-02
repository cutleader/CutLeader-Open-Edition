#pragma once

#include "clPartLayoutCommon.h"
#include "clCutFeatureCommon.h"
#include "Rect2D.h"
#include <vector>
#include <tuple>

DECLARE_CUTLEADER_CLASS(Polygon2D)

BEGIN_CUTLEADER_NAMESPACE()

// 余料区域。
// 注：
//  1) 余料区域是一些材料，上面可能有些洞。边框我们叫做“区域边框”，内孔我们叫“区域内孔”。
//  2) 把板材上已排的零件去除，剩下的是一块块“余料区域”，这些“余料区域”之间其实是有拓扑关系的。
class ClPartLayout_Export RemnantRegion
{
public:
	RemnantRegion(void);
	~RemnantRegion(void);

public: // get/set functions.
	void SetOuterPoly(Polygon2DPtr pOuterPoly) { m_pOuterPoly = pOuterPoly; }
	Polygon2DPtr GetOuterPoly() const { return m_pOuterPoly; }

	void SetOuterRect(Rect2D outerRect) { m_outerRect = outerRect; }
	const Rect2D& GetOuterRect() const { return m_outerRect; }

	void SetHoleRegions(std::vector<std::tr1::tuple<Polygon2DPtr, Rect2D, double, PatternLoopCutOffsetType>> holeRegions) { m_holeRegions = holeRegions; }
	const std::vector<std::tr1::tuple<Polygon2DPtr, Rect2D, double, PatternLoopCutOffsetType>>& GetHoleRegions() const { return m_holeRegions; }

	void IsOutRegion(BOOL bOutRegion) { m_bOutRegion = bOutRegion; }
	BOOL IsOutRegion() const { return m_bOutRegion; }

private:
	// “区域边框”。
	Polygon2DPtr m_pOuterPoly;

	// “区域边框”的外接矩形。
	// 注：
	//  1) 缓存外接矩形是为了后面的使用。
	Rect2D m_outerRect;

	// “区域内孔”。<区域内孔多边形，区域内孔外接矩形，形成这个内孔的零件外轮廓上面的刀具尺寸, 形成这个内孔的零件外轮廓使用的阴阳切方式>
	std::vector<std::tr1::tuple<Polygon2DPtr, Rect2D, double, PatternLoopCutOffsetType>> m_holeRegions;

	// 是不是最外面的“余料区域”。
	// 注：
	//  1) 最外面的区域有点特别，它包括了材料的边框。
	BOOL m_bOutRegion;
};

END_CUTLEADER_NAMESPACE()

#pragma once

#include "clCanvasGeometryUICommon.h"
#include "Rect2D.h"
#include <vector>
#include <tuple>

DECLARE_CUTLEADER_CLASS(Polygon2D)
DECLARE_CUTLEADER_CLASS(Polygon2DList)
DECLARE_CUTLEADER_CLASS(CanvasZone)
DECLARE_CUTLEADER_CLASS(IntegerList)

BEGIN_CUTLEADER_NAMESPACE()

// 底图区域。
// 注：
//  1) 底图区域是一些底图上的连通区域，它们被底图上的轮廓隔开，底图区域肯定有外轮廓，不一定有内轮廓。这些“底图区域”之间其实是有拓扑关系的。去看看文档。
class CanvasZone
{
public:
	CanvasZone(void);
	~CanvasZone(void);

public:
	void SetOuterPoly(Polygon2DPtr pOuterPoly) { m_pOuterPoly = pOuterPoly; }
	Polygon2DPtr GetOuterPoly() const { return m_pOuterPoly; }

	void SetPatLoopID4OuterPoly(LONGLONG iPatLoopID4OuterPoly) { m_iPatLoopID4OuterPoly = iPatLoopID4OuterPoly; }
	LONGLONG GetPatLoopID4OuterPoly() const { return m_iPatLoopID4OuterPoly; }

	Polygon2DListPtr GetInnerPolys() const { return m_pInnerPolys; }
	void SetInnerPolys(Polygon2DListPtr pInnerPolys) { m_pInnerPolys = pInnerPolys; }

	IntegerListPtr GetPatLoopIDs4InnerPolys() const { return m_pPatLoopIDs4InnerPolys; }
	void SetPatLoopIDs4InnerPolys(IntegerListPtr pPatLoopIDs4InnerPolys) { m_pPatLoopIDs4InnerPolys = pPatLoopIDs4InnerPolys; }

private:
	// 底图区域的外轮廓。
	// 注：
	// 对于最外层底图区域，由于底图没有材料边框，所以这里放了一个很大很大的多边形，且m_iPatternLoopID为-1。
	Polygon2DPtr m_pOuterPoly;
	LONGLONG m_iPatLoopID4OuterPoly; // 底图区域外轮廓对应的几何轮廓。

	// 底图区域的内轮廓。
	Polygon2DListPtr m_pInnerPolys;
	IntegerListPtr m_pPatLoopIDs4InnerPolys; // 底图区域内轮廓对应的几何轮廓。
};

END_CUTLEADER_NAMESPACE()

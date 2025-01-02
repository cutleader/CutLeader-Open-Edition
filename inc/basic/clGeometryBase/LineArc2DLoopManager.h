#pragma once

#include "clGeometryBaseCommon.h"
#include "baseConst.h"
#include <vector>
#include "Point2DList.h"
#include "Line2D.h"

DECLARE_CUTLEADER_CLASS(LineArc2DList)
DECLARE_CUTLEADER_CLASS(LineArc2DLoop)
DECLARE_CUTLEADER_CLASS(LineArc2DLoopList)

BEGIN_CUTLEADER_NAMESPACE()

class ClGeometryBase_Export LineArc2DLoopManager
{
public:
	// build geom loops using base geoms.
	// notes:
	//   1) "pGeo2DList" do not need to connect head-with-tail.
	//   2) this function is similar with "PatternLoopList::CalculateLoops".
	//   3) by default we use SYSTEM_TOLERANCE as the criteria to check whether geometries are connected.
	static LineArc2DLoopListPtr BuildGeomLoop(LineArc2DListPtr pGeo2DList, double dTol = SYSTEM_TOLERANCE);

	// "pGeomLoopList" can be sorted to several(or one) LineArc2DLoopList objects, all LineArc2DLoop objects in each LineArc2DLoopList object are adjacent.
	static std::vector<LineArc2DLoopListPtr> FindAdjacentLoopsList(const LineArc2DLoopList* pGeomLoopList, double dSpace);

	// 找到最外围的“基本图形”回路。
	// 注：
	//  1) 外接矩形最大的作为外围回路。
	static int GetOuterGeomLoop(const LineArc2DLoopList* pGeomLoopList);

	// 得到LineArc2DLoop和直线的所有交点。
	static Point2DListPtr GetCrossPts_with_line(const Line2D& line, const LineArc2DLoop* pGeomLoop);

	// 把节点在pt处打断，形成新的LineArc2DLoop。新的LineArc2DLoop中的line/arc也是克隆出来的。
	static LineArc2DLoopPtr SplitNode_at_pt(const LineArc2DLoop* pGeomLoop, const Point2D& pt);

	// 在startPt和endPt处把LineArc2DLoop剪断，返回剪断后的两段，这两段的起点都是startPt。第一段的方向是沿着LineArc2DLoop的方向，第二段是逆着LineArc2DLoop的方向。
	static std::vector<LineArc2DListPtr> CutGeomLoop_at_startEndPt(const LineArc2DLoop* pGeomLoop, const Point2D& startPt, const Point2D& endPt);

private:
	// find LineArc2DLoop object "iGeomLoopID" in LineArc2DLoopList objects "adjacentLoopsList".
	// notes:
	//   1) this function return the index of the LineArc2DLoopList object which hold the LineArc2DLoop "iGeomLoopID".
	static int FindGeomLoop(std::vector<LineArc2DLoopListPtr> adjacentLoopsList, LONGLONG iGeomLoopID);
};

END_CUTLEADER_NAMESPACE()

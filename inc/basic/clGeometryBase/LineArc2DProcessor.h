#pragma once

#include "clGeometryBaseCommon.h"
#include <vector>
#include "Point2D.h"

DECLARE_CUTLEADER_CLASS(LineArc2DList)
DECLARE_CUTLEADER_CLASS(ILineArc2D)
DECLARE_CUTLEADER_CLASS(Line2D)
DECLARE_CUTLEADER_CLASS(Line2DList)
DECLARE_CUTLEADER_CLASS(Arc2D)
DECLARE_CUTLEADER_CLASS(Point2DList)

BEGIN_CUTLEADER_NAMESPACE()

// the processor for 2d line/arc, 二维直线/圆弧的处理器类。
class ClGeometryBase_Export LineArc2DProcessor
{
public:
	// expand the line or the "line-like" arc to a similar rectangle if "pGeo2DList" only has one item.
	// notes:
	//   1) after calling this, we can convert this "pGeo2DList" to polygon.
	static void ExpandLineArcToRect(LineArc2DListPtr pGeo2DList);

	// cut lines at the cross points.
	// notes:
	//   1) this function cannot fit all cases, BUT if you input two
	//      crosses lines, this function can get the correct 4 lines.
	static void CutLineByCross(Line2DListPtr pLine2Ds);

	// convert "pArc" to lines.
	// notes:
	//   1) if pArc is very small, return a lineList without items in it.
	static Line2DListPtr DevideArcToLine(const Arc2D* pArc, double dCutArcFactor);

	// adjust the loop to CCW.
	// notes:
	//   1) "pGeo2DList" should connect "head-tail" to a closed loop.
	//   2) if "pGeo2DList" connect "head-tail" as CW, return TRUE.
	static BOOL AdjustLoopToCCW(LineArc2DListPtr pGeom2DList);

	// build points to arc using Least Square Fitting Algorithm.
	/* params:
	*  pPtList: build arc from these points, point count should be greater than 2.
	*  dMaxOffset: the max offset between the "pPtList" to the arc.
	*  bCheckMidPt: 计算拟合误差时，是否考虑“候选点”的中点。
	*/
	// 注:
	//   1) if points are on one line, "dMaxOffset" will be a large value.
	//   2) start/end points of the arc will be adjusted to the first/last point in "pPtList".
    //   3) 待完善：拟合成圆好像有点问题。还有，同样的一组点，把它们移动一下，拟合出的结果可能就不同，
	//      遇到的案例是这样的：点来自样条曲线，很密，几乎连成直线，调用该函数时可能会遇到边界条件，
	//      也就是会先拟合出一个超级大的圆，此时在这个圆上面找点时很可能触碰到边界条件，一旦点集移动了，
	//      触碰的边界条件也不同了，所以可能出现不同的结果，目前暂时没有处理。
	static Arc2DPtr BuildPts2Arc(const Point2DList* pSeedPtList, double& dMaxOffset, BOOL bCheckMidPt = FALSE);

    // 得到落在直线/圆弧上的点，并按照到直线/圆弧起点的距离排序。
    static Point2DListPtr SortPtsOnLine(Line2DPtr pLine2D, Point2DListPtr pPtList);
    static Point2DListPtr SortPtsOnArc(Arc2DPtr pArc2D, Point2DListPtr pPtList);

	// 得到点到圆的两个切点。
	// 注：
	// 1) 调用方需确定pt点在圆外面。
	static bool GetTwoTangencyPtsOnCircle(const Point2D& pt, const Point2D& centerPt, double dRadius, Point2D& firstTangencyPt, Point2D& secondTangencyPt);


	/************************************************************************/
	// 关于基本图形之间的关系。

	// 2个基本图形集合之间是不是有图形相交，本图形集合中的图形相交不算。
	// 注：
	//  1) 端点接触也可认为相交。
	static BOOL TwoGeomListCross(LineArc2DListPtr pGeo2DList1, LineArc2DListPtr pGeo2DList2);

	// 基本图形“pGeom”是否被“pGeoms”截断。
	// 注：
	//  1) 如果“pGeom”的端点接触到“pGeoms”，不认为是截断。
	static BOOL GeomIsCutByGeoms(ILineArc2DPtr pGeom, LineArc2DListPtr pGeoms);

	// 得到两个直线之间的距离。
	// 注：
	//  1) 两个直线相交的话，距离为0。
	static double GetTwoLineDis(Line2DPtr pLine1, Line2DPtr pLine2);

	// 得到直线和圆弧之间的距离。
	// 注：
	//  1) 两个相交的话，距离为0。
	static double GetLineArcDis(Line2DPtr pLine, Arc2DPtr pArc);

	// 得到两个圆弧之间的距离。
	// 注：
	//  1) 两个相交的话，距离为0。
	static double GetTwoArcDis(Arc2DPtr pArc1, Arc2DPtr pArc2);

	// 得到两个基本图形间的距离。
	// 注：
	//  1) 两个基本图形相交的话，距离为0。
	static double GetTwoGeomDis(ILineArc2DPtr pGeom1, ILineArc2DPtr pGeom2);

	// 2个基本图形集合之间的最短距离。
	// 注：
	//  1) 得到pGeoms1中各个基本图形到pGeoms2中各个基本图形的距离，返回最短的距离值。
	//  2) 如果pGeoms1中某些基本图形和pGeoms2中某些基本图形相交，则距离为0。
	static double GetTwoGeomsDis(LineArc2DListPtr pGeoms1, LineArc2DListPtr pGeoms2);
	/************************************************************************/


	// 点到线段的距离
	static double SquareDistance_ofPoint2Line(const Point2D& pt, const Point2D& startPt, const Point2D& endPt);
	static double SquareDistance_ofPoint2Line_2(double dPtX, double dPtY, double dStartPtX, double dStartPtY, double dEndPtX, double dEndPtY);
};

END_CUTLEADER_NAMESPACE()

#pragma once

#include "clGeometryBaseCommon.h"
#include "Vector2D.h"
#include "Path2D.h"
#include "AlgData.h"
#include <list>

DECLARE_CUTLEADER_CLASS(Line2D)
DECLARE_CUTLEADER_CLASS(Line2DList)
DECLARE_CUTLEADER_CLASS(Polygon2D)
DECLARE_CUTLEADER_CLASS(Polygon2DList)

BEGIN_CUTLEADER_NAMESPACE()


// the result of the 2d polygon boolean operation, 二维多边形布尔运算的结果。
enum PolygonBooleanType
{
	PolygonBoolean_Succeed 			= 0, // succeed
	PolygonBoolean_TryAgain 		= 1, // please try again.
	PolygonBoolean_Error 			= 2, // error
};


// this class will process 2d polygon，这个类用来处理二维多边形对象。
class ClGeometryBase_Export Polygon2DProcessor
{
public:
	/************************************************************************/	
	// optimize the polygon.

	// notes:
	//   1) two adjacent lines in the polygon can be merged.
	//   2) make the polygon has less vertexes.
	// params:
	//   1)  bOuter: whether used for outer boundary.
	static void SleekPolygon(Polygon2DPtr pPolygon2D, double dOptPolyFactor, BOOL bOuter = TRUE);

	// notes:
	//   1) this function will make the polygon more simple(have less vertexes),
	//      so in some case, the result polygon may be anamorphic with the 
	//      original one.
	//   2) this function also call SleekPolygon function.
	static void OptimizePolygon(Polygon2DPtr pPolygon2D, double dOptPolyFactor);
	/************************************************************************/


	/************************************************************************/
	// enlarge or compress the polygon

	// enlarge the polygon with "dVal".
	static void EnlargePolygon(Polygon2DPtr pPolygon2D, double dVal, double dCutArcFactor, double dOptPolyFactor);

	// this function is the reverse function of EnlargePolygon.
	// notes: 
	//   1) returns the polygons after compressing "pPolygon2D". if the returned polygons contains no items, fails.
	static Polygon2DListPtr CompressPolygon(Polygon2DPtr pPolygon2D, double dVal);

	// compress "pPolyList" with "dVal".
	static Polygon2DListPtr CompressPolygon(Polygon2DListPtr pPolyList, double dVal, double dOptPolyFactor);
	/************************************************************************/


	/************************************************************************/
	// merge polygons.
	
	// merge the polygons in "pPolyList".
	static Polygon2DPtr MergePolygon(Polygon2DListPtr pPolyList, double dOptPolyFactor, BOOL bNeedSleek = TRUE);

	// merge "pPolygon2D1" with "pPolygon2D2".
	static Polygon2DPtr MergePolygon(Polygon2DPtr pPolygon2D1, Polygon2DPtr pPolygon2D2);

	// 整理这些多边形，把相交的归类放在一起。
	// 注：polygonRects为多边形外接矩形缓存，为空的话就不用这个缓存。
	static std::vector<Polygon2DListPtr> MergePolyEx(Polygon2DListPtr pPolyList, const std::map<LONGLONG, Rect2D>& polygonRects);
	/************************************************************************/


	/************************************************************************/
	// about subtract polygon.

	// subtract "pPolygon2" from "pPolygon1" and return the result polygons.
	// notes:
	//   1) if failed, return empty list.
	static std::vector<Polygon2D> SubtractPolygon(const Polygon2D& polygon1, const Polygon2D& polygon2);

	//
	static PolygonBooleanType SubtractPolygon(std::vector<Polygon2D>& polygonList, const Polygon2D& polygon);

	// 从“pPolygon2D1”中切掉和“pPolygon2D2”相交的部分。
	// 注：
	//  1) 把“pPolygon2D2”想象成一个模具，往“pPolygon2D1”上面一压，“pPolygon2D1”被压掉下来的部分即是需要的。
	//     还要用“pPolygon2D1”压“pPolygon2D2”一次，这样才不会漏掉多边形。
	//  2) 如果切割多边形失败，返回空的集合。
	static Polygon2DListPtr CutOffPolygon(const Polygon2D* pPolygon2D1, const Polygon2D* pPolygon2D2, const Line2DList* pLineList1, Line2DList* pLineList2);
	/************************************************************************/


	/************************************************************************/
	// about NFP.

	// build the NFP of "pFixedPoly" and "pMovingPoly".
	// notes:
	//   1) "pPolygon2D" should be allocated by the caller.
	static void BuildNFP(const Polygon2D* pFixedPoly, const Polygon2D* pMovingPoly, Polygon2DPtr pPolygon2D, BOOL bCheckResult = TRUE);

	// build the internal NFP of the fixed and moving polygon at the reference point.
	//  注：
	//   1) 待完善。考虑一种情况，如果期望得到的一个内临界多边形像一个“凸”子，并且上面的凸台扁的成了一根线，
	//      这时这个凸台无法通过多边形返回，只能返回一个正方形，扁成线的凸台通过向retLines中添加一根线返回。
	static Polygon2DListPtr BuildInnerNFP(const Polygon2D* pFixedPoly, const Line2DList* pFixPolyEdges, const Polygon2D* pMovingPoly, const Line2DList* pMovePolyEdges, const Point2D& refPt);

	// get a point on NFP, at this point, the area composed by "pFixedPoly" and "pMovingPoly" is min.
	static Point2D GetMinPairAreaPos(Polygon2DPtr pFixedPoly, Polygon2DPtr pMovingPoly, Polygon2DPtr pNFPPoly, double dSpacing);
	/************************************************************************/


	/************************************************************************/
	// 关于多边形和直线的相交。

	// go through the polygon "pPoly" with a ray which define with "startPt" and "dCosA/dSinA".
	static void ThroughPolygon(Polygon2DPtr pPoly, Point2D startPt, double dCosA, double dSinA, std::list<Dist>& distList);

	// 得到直线和多边形的交点，没有交点返回空容器。
	/* 参数：
	*  basePt, iDir: 直线的起点和延伸方向。
	*  bSort: 是否把交点沿着射线方向排序。
	*/
	static Point2DListPtr GetCrossPtsWithLine(const Polygon2D* pPoly, const Point2D& basePt, POSITION_INFO iDir, BOOL bSort = TRUE);
	/************************************************************************/


	// load/save polygon.
	static void SavePolygon(Polygon2DPtr pPoly, CString strPath);
	static Polygon2DPtr LoadPolygon(CString strPath);

	// 把多边形中的粘着点分开。
	/* 参数：
	*  dDis: 粘着点被拉开的距离。
	*/
	// 注:
	//   1) 这个函数假设多边形最多只有一个粘着点。 
	static void SeperateStickPt(Polygon2D& poly, double dDis);

	// get the edge indexes in "pPoly1" which is common with "pPoly2".
	static std::vector<int> GetCommonEdges(Polygon2DPtr pPoly1, Polygon2DPtr pPoly2);

	// 初始化这个类需要用到的一些静态变量。
	static void Init(BOOL bSleekCtrl, double dSleekRate);

	// 去掉多边形中的尖刺。所谓尖刺就像一根针一样刺入或者刺出这个多边形。多边形顶点数小于4时不处理。
	static void RemoveSpike_of_polygon(Polygon2D& poly);

private:
	// 这三个点是不是需要做光顺。
	static BOOL CanSleek(Point2D pt1, Point2D pt2, Point2D pt3, double dLongEdge);

private:
	// 控制哪些边不被光顺，参照SheetEditParam中的注释。
	static BOOL m_bSleekCtrl;
	static double m_dSleekRate;
};

END_CUTLEADER_NAMESPACE()

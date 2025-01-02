#pragma once

#include "clGeometryBaseCommon.h"
#include "Vector2D.h"
#include "Path2D.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(Line2D)
DECLARE_CUTLEADER_CLASS(Line2DList)
DECLARE_CUTLEADER_CLASS(Polygon2D)

BEGIN_CUTLEADER_NAMESPACE()


// the relationship between point and polygon.
typedef enum PtPolygonPos
{
	OUTSIDE_POLY		= 0,
	ON_POLY				= 1, 
	INSIDE_POLY			= 2
} PT_POLYGON_POS;


// this class stands for a 2d polygon, of course it's "closed", 二维多边形对象。
// notes:
//   1) be careful to call "push_back" to build the polygon, it's not easy to build a polygon using this method, please use functions in Polygon2DBuilder class.
//   2) do not need to call IsClosedPath() for a polygon, after building a polygon, it's closed. BUT if you call IsClosedPath() for a polygon, 
//      return FALSE, YES, the polygon is not a closed path.
//   3) the polygon is not always CCW.
class ClGeometryBase_Export Polygon2D : public Path2D
{
public:
	Polygon2D(void);
	~Polygon2D(void);

public:
	BOOL operator < (const Polygon2D& poly) const;
	BOOL operator == (const Polygon2D& poly) const;

public:
	/************************************************************************/
	// with other polygon.

	BOOL IncludePolygon(const Polygon2D& poly) const;

	// whether this polygon intersects with "poly".
	// notes:
	//   1) here, "intersect" means inter-intersection, this means if only vertexes intersect, we do not think two polygon intersect.
	BOOL IntersectWith(const Polygon2D& poly) const;
	bool IntersectWith(const Polygon2D& poly, const Line2DList* pLineList1, const Line2DList* pLineList2) const; // for performance.

	// whether point:"iPtIndex" touches with "pPoly->iEdgeIndex"
	BOOL IsTouch(int iPtIndex, const Polygon2D* pPoly, int iEdgeIndex) const;

	// get the edge indexes in "pPoly" which is common with this polygon.
	vector<int> GetCommonEdges(Polygon2DPtr pPoly);

	// 得到两个多边形重合部分的线段。
	Line2DListPtr GetCommonLines(Polygon2DPtr pPoly);
	/************************************************************************/


	/************************************************************************/
	// about the direction

    // whether the 2d polygon is counter clockwise.
	BOOL IsCCW() const;

	// make the polygon CCW.
	void AdjustToCCW();
	/************************************************************************/


	/************************************************************************/
	// 和点的关系。

	// 从点pt打出射线的最短长度。
	double GetRayMinLen(const Point2D& pt, double dCos, double dSin) const;

	// get the relationship between point and polygon.
	PT_POLYGON_POS GetPointPos(const Point2D& pt) const;

	// 得到多边形上到点“pt”最近的点(不一定是顶点)。
	Point2D GetClosestPt(const Point2D& pt) const;
	/************************************************************************/


	LONGLONG GetPolygonID() const { return m_iID; }

	Polygon2DPtr Clone() const;

	// get area of the polygon
	double GetArea() const;

	// get edges of the polygon.
	Line2DListPtr GetLineList() const;
	LineArc2DListPtr GetLineArc2Ds() const;

	// 得到长度
	double GetLength() const;

	// get the gravity point.
	// notes:
	//   1) before call this, caller should make sure the number of vertex points is greater than than 2.
	Point2D GetGravityPt() const;

	// whether line(vertex(iEdgeIndex), vertex(iEdgeIndex+1)) is a chimb.
	// notes:
	//   1) chimb means this edge is on the hull of the polygon.
	BOOL IsChimb(int iEdgeIndex) const;

	// check the self-cross point num.
	int GetSelfCrossNum() const;

	// 是否是个矩形。
	bool IsRect() const;

	// 得到该顶点处的内侧夹角。调用前需确认多边形是CCW的。
	double GetIncludedAngle_inner(int iPtIndex) const;

private:
	LONGLONG m_iID;
};

END_CUTLEADER_NAMESPACE()

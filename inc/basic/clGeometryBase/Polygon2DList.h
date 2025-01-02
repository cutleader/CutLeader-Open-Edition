#pragma once

#include "clGeometryBaseCommon.h"
#include "Vector2D.h"
#include "Rect2D.h"
#include "Path2D.h"
#include <vector>
#include <map>

DECLARE_CUTLEADER_CLASS(Polygon2D)
DECLARE_CUTLEADER_CLASS(Polygon2DList)

BEGIN_CUTLEADER_NAMESPACE()

class ClGeometryBase_Export Polygon2DList : public std::vector<Polygon2DPtr>
{
public:
	Polygon2DList(void);
	~Polygon2DList(void);

public:
	// get the polygon index which has the max area.
	int GetMaxAreaPoly() const;

	// remove the polygon which has no convexes.
	void RemoveNullPolygon();

	// whether "pPoly" intersect with one polygon in the list.
	// 注：polygonRects为多边形外接矩形缓存，为空的话就不用这个缓存。
	bool IntersectWith(Polygon2DPtr pPoly, const std::map<LONGLONG, Rect2D>& polygonRects) const;

	// 得到多边形上到点“pt”最近的点(不一定是顶点)。
	Point2D GetClosestPt(const Point2D& pt) const;
	Point2D GetClosestPt_2(const Point2D& pt, int& iPolygonIndex) const; // 这个函数还返回最近点所在的多边形。

	// get the boundary rect of polygons.
	Rect2D GetRectBox() const;

	// 变换多边形集合。
	void Transfer(const Vector2D& transferVect);
	void Transform(const Matrix2D& transformMatrix);
};

END_CUTLEADER_NAMESPACE()

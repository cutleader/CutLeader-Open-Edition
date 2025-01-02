#pragma once

#include "clGeometryBaseCommon.h"
#include "Rect2D.h"
#include "Point2DList.h"


DECLARE_CUTLEADER_CLASS(ILineArc2D)

BEGIN_CUTLEADER_NAMESPACE()


// two types of 2d line/arc, 二维直线圆弧对象的类型。
enum LineArc2DType
{
	LineArc2D_Line 		= 0,
	LineArc2D_Arc 		= 1,
};


// the interface for 2D line and arc, 二维直线和圆弧的接口。
class ClGeometryBase_Export ILineArc2D
{
public:
	/************************************************************************/
	// about the start point and end point.

	virtual const Point2D& GetStartPoint() const = 0;
	virtual void SetStartPoint(const Point2D& pt) = 0;

	virtual const Point2D& GetEndPoint() const = 0;
	virtual void SetEndPoint(const Point2D& pt) = 0;
	/************************************************************************/


	/************************************************************************/
	// about figure out some point at geom.

	// get the point on the geom, this point should be the closest point to "pt".
	virtual Point2D GetClosestPt(const Point2D& pt) const = 0;

	// get cross points between two geoms.
	virtual Point2DListPtr GetCrossPt(ILineArc2DPtr pGeom2DBase, double dTol = GEOM_TOLERANCE) const = 0;

	// get the point on the geom by the percent.
	virtual Point2D GetPointByPercent(double dPercent) const = 0;

	// move from "fromPt" along geom, the move distance is "dDistance".
	// notes:
	//   1) if the move distance will exceed the geom, return the end point of geom.
	//   2) if "dDistance" is negtive, move backward.
	virtual Point2D MoveForward(const Point2D& fromPt, double dDistance) const = 0;
	/************************************************************************/


	/************************************************************************/
	// about transform the geom.

	// transform the geom using the Matrix.
	virtual void Transform(const Matrix2D& mat) = 0;

	// reverse the geom2D, actually the shape is same, we only change the direction.
	virtual void Reverse() = 0;
	/************************************************************************/


	// get the percent by the point on the GeomBase.
	// 注：
	//   1) 对直线来说，如果"pt"距离起点或终点过近（小于GEOM_TOLERANCE），返回0或者1.
	virtual double GetPercentByPoint(const Point2D& pt) const = 0;

	// get the distance from the geom to "pt".
	virtual double Distance(const Point2D& pt) const = 0;

	// get the type of the geometry.
	virtual LineArc2DType GetType() const = 0;

	// get the rectangle range
	virtual Rect2D GetRectBox() const = 0;

	virtual ILineArc2DPtr Clone() const = 0;

	// 更新缓存。
	virtual void UpdateCache() = 0;
};

END_CUTLEADER_NAMESPACE()

#pragma once

#include "clGeometryBaseCommon.h"
#include "Vector2D.h"
#include "Rect2D.h"
#include <vector>


DECLARE_CUTLEADER_CLASS(ILineArc2D)

BEGIN_CUTLEADER_NAMESPACE()


// 基本图形集合。
class ClGeometryBase_Export LineArc2DList : public std::vector<ILineArc2DPtr>
{
public:
	LineArc2DList(void);
	~LineArc2DList(void);

public:
	// get line/arcs those are "close" to pt.
	// notes:
	//  1) within the capture range of the specified point.
	//  2) we may not find the point, return the vector that do not contain items.
	LineArc2DListPtr GetLineArcsCloseTo(const Point2D& pt, double dCaptureRange) const;

	// get the cross point that is "closest" to pt, if fails, return NULL.
	Point2DPtr GetCrossClosestTo(const Point2D& pt) const;

	// 得到外围矩形。
	Rect2D GetRectBox() const;

	// 做变换。
	void Transform(const Matrix2D& mat);

	// clone this geom list。
	LineArc2DListPtr Clone() const;

	// reverse the line/arcs.
	void Reverse();

	// 得到“图形集合”到点的最近距离。
	double GetClosestDis(const Point2D& pt) const;

	// 得到距离“pt”最近的点。
	Point2D GetClosestPt(const Point2D& pt);

	// 得到基本图形的总长。
	double GetTotalLength() const;
};

END_CUTLEADER_NAMESPACE()

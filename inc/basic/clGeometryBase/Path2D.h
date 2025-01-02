#pragma once

#include "clGeometryBaseCommon.h"
#include "Rect2D.h"
#include "Point2DList.h"

BEGIN_CUTLEADER_NAMESPACE()

// this class stands for a "path", it may be open or closed.
// notes:
//   1) a closed path maybe (0,0), (1,1), (1,2), (0,0). BUT for a polygon, do not need the last point(0,0).
//   2) you can call many "push_back" to build a path, remember! if you want to build a close path, add the first point to the end again.
class ClGeometryBase_Export Path2D : public Point2DList
{
public:
	Path2D(void);
	~Path2D(void);

public:
	/************************************************************************/
	// notes:
	//   1) before call this, make sure the point count is greater than 1.
	//   2) the result is within [0, 2PI].

	// get the vector angle from "point:iPtIndex" to "point:iPtIndex+1"
	double GetAngleToNext(int iPtIndex) const;

	// get the vector angle from "point:iPtIndex-1" to "point:iPtIndex"
	double GetAngleToPre(int iPtIndex) const;
	/************************************************************************/

	// get the normal line of edge.
	void GetNormalLine(int iEdgeIndex, double& dDX, double& dDY) const
	{
		if (iEdgeIndex == (int)size() - 1)
		{
			const Point2D& pt1 = at_with_safe_index(0);
			const Point2D& pt2 = at_with_safe_index(iEdgeIndex);
			dDX = pt1.Y() - pt2.Y();
			dDY = pt2.X() - pt1.X();
		}
		else
		{
			const Point2D& pt1 = at_with_safe_index(iEdgeIndex);
			const Point2D& pt2 = at_with_safe_index(iEdgeIndex+1);
			dDX = pt2.Y() - pt1.Y();
			dDY = pt1.X() - pt2.X();
		}
	}

	/************************************************************************/
	// rotate the path with "dAngle" based point:"pt".

	void Rotate(const Point2D& pt, double dAngle);
	virtual void Rotate(const Point2D& pt, double dCos, double dSin);
	/************************************************************************/

	// reverse the point order.
	void Reverse();

	// get the boundary rect.
	Rect2D GetBoundaryRect() const;

	// use this function to check whether the path is closed.
	// notes:
	//   1) for a closed path, the first and last point are same.
	//   2) return TRUE, if the path is closed.	
	BOOL IsClosedPath();

	// if the path is closed, return true, then remove last item.
	// if the path is open, return false.
	BOOL AmendClosePath();

	// get the max distance between two points.
	double GetPathDiameter() const;

	void Transfer(const Point2D& pt);
	void Transfer(const Vector2D& vect);
	void Transfer(const Matrix2D& mat);
};

END_CUTLEADER_NAMESPACE()

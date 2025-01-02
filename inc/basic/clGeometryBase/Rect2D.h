#pragma once

#include "clGeometryBaseCommon.h"
#include "Point2D.h"

DECLARE_CUTLEADER_CLASS(Point2DList)
DECLARE_CUTLEADER_CLASS(LineArc2DList)
DECLARE_CUTLEADER_CLASS(Rect2D)

BEGIN_CUTLEADER_NAMESPACE()

typedef enum tagRectSide
{
	RECT_BOTTOM			= 1,
	RECT_RIGHT			= 2,
	RECT_TOP			= 3,
	RECT_LEFT			= 4,
} RECT_SIDE;

enum RECT_DIR
{
	LEFT_2_RIGHT		= 1,
	RIGHT_2_LEFT		= 2,
	BOTTOM_2_TOP		= 3,
	TOP_2_BOTTOM		= 4,
};

// 矩形类。
class ClGeometryBase_Export Rect2D
{
public:
	Rect2D(void);
	Rect2D(const Rect2D& rect);
	Rect2D(double dXMin, double dXMax, double dYMin, double dYMax);
	Rect2D(const Point2D& leftBottomPt, const Point2D& rightTopPt);
	~Rect2D(void);

public:
	/************************************************************************/
	// over ride some operators.

	// we define these for the template class which inherit from class SmallObjList.
	bool operator ==(const Rect2D& Pt) const { return TRUE; }
	bool operator <(const Rect2D& Pt) const { return TRUE; }

	// whether this rect intersect with "rect"
	// notes:
	//   1) if one contain another, we think there is an intersection.
	BOOL operator &(const Rect2D& rect) const;
	/************************************************************************/

public: // get/set functions.
	void SetXMin(double dXMin){ m_dXMin = dXMin; }
	double GetXMin() const { return m_dXMin; }

	void SetXMax(double dXMax) { m_dXMax = dXMax; }
	double GetXMax() const { return m_dXMax; }

	void SetYMin(double dYMin) { m_dYMin = dYMin; }
	double GetYMin() const { return m_dYMin; }

	void SetYMax(double dYMax) { m_dYMax = dYMax; }
	double GetYMax() const { return m_dYMax; }

	double GetWidth() const { return m_dXMax - m_dXMin; }
	double GetHeight() const { return m_dYMax - m_dYMin; }

public:
	/************************************************************************/
	// 得到矩形的一些基础点。

	Point2D GetLeftBottomPt() const { return Point2D(m_dXMin, m_dYMin); }
	Point2D GetLeftTopPt() const { return Point2D(m_dXMin, m_dYMax); }
	Point2D GetRightBottomPt() const { return Point2D(m_dXMax, m_dYMin); }
	Point2D GetRightTopPt() const { return Point2D(m_dXMax, m_dYMax); }
	Point2D GetCenterPt() const { return Point2D((m_dXMin+m_dXMax)/2, (m_dYMin+m_dYMax)/2); }

	/*
			3                     2
			+--------------------+  
			|                    |
			|                    |  
			|                    |  
			|                    |  
			|                    |  
			|                    |  
			+--------------------+  
			0                     1
	*/
	Point2DListPtr GetAllCornerPoint();

	// get the specified corner point.
	Point2D GetCornerPt(RECT_CORNER rectCorner) const;
	/************************************************************************/


	/************************************************************************/
	// 和其他矩形的关系。

	// whether this rect includes "rect".
	BOOL IncludeRect(const Rect2D& rect) const;

	// notes:
	//   1) return true if there is an intersection, otherwise, return false.
	//   2) if one contain another, we think there is an intersection.
	BOOL Intersect(const Rect2D& rect, Rect2D& resRect) const;

	// unite this rect with "rect".
	void Unite(const Rect2D& rect);

	// compare two rects.
	// notes: 
	//   1) the rect at outside is smaller.
	BOOL SmallerThan(const Rect2D& rect2D);

	// 得到2个矩形之间的距离。
	// 注：
	//  1) 如果2个矩形相交，我们认为距离是0.
	double DistanceTo(const Rect2D& rect) const;

	// 往iDir方向移动，是不是能撞上rect。
	// 注：
	//  1) 两个矩形相交的话，肯定是能撞上的。
	BOOL CommonAtDir(const Rect2D& rect, POSITION_INFO iDir) const;

	// check whether this rect is left-lower/higher than the other.
	bool LeftLower(const Rect2D& rect) const;
	bool LeftHigher(const Rect2D& rect) const;

	// check whether this rect is right-lower/higher than the other.
	bool RightLower(const Rect2D& rect) const;
	bool RightHigher(const Rect2D& rect) const;

	// check whether this rect is low-lefter/righter than the other.
	bool LowLefter(const Rect2D& rect) const;
	bool LowRighter(const Rect2D& rect) const;

	// check whether this rect is high-lefter/righter than the other.
	bool HighLefter(const Rect2D& rect) const;
	bool HighRighter(const Rect2D& rect) const;
	/************************************************************************/


	/************************************************************************/
	// 矩形和点的关系。

	// 矩形到点的距离。
	// 注：
	//  1) 用四条边判断。
	double DistanceToA(const Point2D& pt) const;

	// get the distance between the rect and the "pt".
	// notes: 
	//   1) we will calculate the distances between the 4 corners and the "pt", then choose the least one.
	double DistanceTo(const Point2D& pt) const;

	// whether the rect include "pt".
	BOOL ContainPt(const Point2D& pt) const;

	// get the corner which is closest to "pt".
	RECT_CORNER GetClosestCorner(Point2D pt);
	/************************************************************************/


	/************************************************************************/
	// 修改矩形。

	// extend the rectangle.
	void Extend(double dX, double dY);

	// expand rect with "dVal".
	void Expand(double dVal);

	// transform the rect.
	void Transform(const Matrix2D& matrix);

	void Transform(double dX, double dY);

	void Mirror(double dXRate, double dYRate);

	// cut the rect.
	void CutRect(RECT_CORNER iCornor, double dX, double dY);
	/************************************************************************/


	// whether the rect in the range presented by "dRangeX dRangeY".
	BOOL InRange(double dRangeX, double dRangeY) const;

	/*
		         posPt *
		+-------------*------+  
		|            *       |  
		|           *        |  
		|          * dAngle  |  
		|         **************
		|                    |  
		|                    |  
		|                    |  
		|                    |  
		|                    |  
		+--------------------+  
	*/
	// notes:
	//   1) dAngle : 0 ~ TWOPI
	//   2) RECT_SIDE : whether is the cross point?
	RECT_SIDE GetPosInfoByAngle(double dAngle, Point2D& posPt);

	// return the 4 lines of the rect.
	// notes: return lines with CCW.
	LineArc2DListPtr GetLineS() const;

	// 得到较长的边。
	double GetLongEdge() const;

private:
	double m_dXMin;
	double m_dXMax;
	double m_dYMin;
	double m_dYMax;
};

END_CUTLEADER_NAMESPACE()

#pragma once

#include "clPartLayoutCommon.h"
#include "Rect2D.h"
#include "ClData.h"

// declare class.
DECLARE_CUTLEADER_CLASS(Polygon2D)
DECLARE_CUTLEADER_CLASS(NestPolygonInfo)
//

BEGIN_CUTLEADER_NAMESPACE()

// 已排多边形的信息。
class ClPartLayout_Export NestPolygonInfo : public ClData
{
public:
	NestPolygonInfo(Polygon2DPtr pNestPoly, Rect2D polyRect);
	~NestPolygonInfo(void);

public: // get/set functions.
	void SetNestPoly(Polygon2DPtr pNestPoly) { m_pNestPoly = pNestPoly; }
	Polygon2DPtr GetNestPoly() { return m_pNestPoly; }

	void SetPolyRect(Rect2D polyRect) { m_polyRect = polyRect; }
	Rect2D GetPolyRect() { return m_polyRect; }

public:
	// 当前对象是不是应该排在“pNestPolygonInfo”前面。
	// 注：
	//  1) 用该多边形的外接矩形来判断位置。
	//  2) 先后顺序要参考光线的照射方向，如果光线往下照，那么位置高的排在前面。
	static bool BeforeTo4Bottom(NestPolygonInfoPtr pNestPolygonInfo1, NestPolygonInfoPtr pNestPolygonInfo2);
	static bool BeforeTo4Top(NestPolygonInfoPtr pNestPolygonInfo1, NestPolygonInfoPtr pNestPolygonInfo2);
	static bool BeforeTo4Left(NestPolygonInfoPtr pNestPolygonInfo1, NestPolygonInfoPtr pNestPolygonInfo2);
	static bool BeforeTo4Right(NestPolygonInfoPtr pNestPolygonInfo1, NestPolygonInfoPtr pNestPolygonInfo2);

private:
	// 在排版位置的多边形。
	Polygon2DPtr m_pNestPoly;

	// 该多边形的外接矩形。
	Rect2D m_polyRect;
};

END_CUTLEADER_NAMESPACE()
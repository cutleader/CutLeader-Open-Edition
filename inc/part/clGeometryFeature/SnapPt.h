#pragma once

#include "clGeometryFeatureCommon.h"
#include "Point2D.h"

BEGIN_CUTLEADER_NAMESPACE()

// 捕获点。
class ClGeometryFeature_Export SnapPt
{
public:
	// 捕获点的类型。
	enum SnapPtType
	{
		EndPtOf2DLineArc				= 0, // 二维线段或圆弧的端点。
		MiddlePtOf2DLineArc				= 1, // 二维线段或圆弧的中点。
		CenterPtOf2DArc					= 2, // 二维圆弧的圆心。
		ClosestPtTo2DLineArc			= 3, // 光标到二维线段或圆弧的最近点。
		FootPtTo2DLine					= 4, // 上一输入点到二维线段的垂足。
		TangencyPtTo2DArc				= 5, // 上一输入点到二维圆弧的切点。
		ControlPtOfPattern				= 6, // 几何特征的控制点。
		CrossPtOfTwo2DLineArc			= 7, // 两个二维线段或圆弧的交点。
		ClosestPtToOneAssistLine		= 8, // 光标到辅助线的最近点。
		CrossPtOfTwoAssistLines			= 9, // 两条辅助线的交点。
	};

	SnapPt(SnapPtType emSnapPtType, const Point2D& snapPtPosition);
	~SnapPt(void);

public:
	SnapPtType GetSnapPtType() const { return m_emSnapPtType; }
	const Point2D& GetSnapPtPosition() const { return m_snapPtPosition; }

public:
	CString GetSnapPtName() const;

private:
	SnapPtType m_emSnapPtType;
	Point2D m_snapPtPosition;
};

END_CUTLEADER_NAMESPACE()

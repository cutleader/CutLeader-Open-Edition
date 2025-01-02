#pragma once

#include "clGeometryFeatureCommon.h"
#include "Point2D.h"

DECLARE_CUTLEADER_CLASS(SnapInput)
DECLARE_CUTLEADER_CLASS(PatternList)

BEGIN_CUTLEADER_NAMESPACE()

// 捕获类型。
enum SnapType
{
	SnapNone = 1, // 不捕捉任何点。
	SnapEndPtOf2DLineArc = 2, // 捕捉二维线段或圆弧的端点。
	SnapMiddlePtOf2DLineArc = 4, // 捕捉二维线段或圆弧的中点。
	SnapCenterPtOf2DArc = 8, // 捕捉二维圆弧的圆心。
	SnapClosestPtTo2DLineArc = 16, // 捕捉光标到二维线段圆弧的最近点。
	SnapFootPtTo2DLine = 32, // 捕捉上一输入点到二维线段的垂足。
	SnapTangencyPtTo2DArc = 64, // 捕捉上一输入点到二维圆弧的切点。
	SnapControlPtOfPattern = 128, // 捕捉几何特征的控制点。
	SnapCrossPtOfTwo2DLineArc = 256, // 捕捉两个二维线段圆弧的交点。
	SnapClosestPtToOneAssistLine = 512, // 捕捉光标到辅助线的最近点。
	SnapCrossPtOfTwoAssistLines = 1024, // 捕捉两条辅助线的交点。
};

// 用于捕捉特征点的输入数据。
// 注：
// 1) 该结构需要在捕捉的时候实时创建，比如在鼠标移动的时候，不同的action在不同的状态下可能会返回不同的该结构。
class ClGeometryFeature_Export SnapInput
{
public:
	SnapInput(DWORD dwSnapStyle, const PatternListPtr pPatterns, const Matrix2D& transformMatrix);
	~SnapInput();

public:
	DWORD GetSnapStyle() const { return m_dwSnapStyle; }
	const PatternListPtr GetPatterns() const { return m_pPatterns; }
	const Matrix2D& GetTransformMatrix() const { return m_transformMatrix; }

public:
	static SnapInputPtr BuildSnapInput4SnapNone();

private:
	DWORD m_dwSnapStyle; // 捕捉类型，由SnapType中一个或多个组成。
	const PatternListPtr m_pPatterns; // 捕捉这些几何特征上的点。
	const Matrix2D m_transformMatrix; // 几何特征要做的矩阵变换。
};

END_CUTLEADER_NAMESPACE()

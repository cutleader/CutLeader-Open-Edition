#pragma once

#include "clGeometryFeatureCommon.h"
#include <vector>
#include "Matrix2D.h"

DECLARE_CUTLEADER_CLASS(GlViewPort)
DECLARE_CUTLEADER_CLASS(Point2D)
DECLARE_CUTLEADER_CLASS(Point2DList)
DECLARE_CUTLEADER_CLASS(Line2D)
DECLARE_CUTLEADER_CLASS(Line2DList)
DECLARE_CUTLEADER_CLASS(PatternList)
DECLARE_CUTLEADER_CLASS(LineArc2DList)
DECLARE_CUTLEADER_CLASS(AssistPtList)
DECLARE_CUTLEADER_CLASS(SnapPt)

BEGIN_CUTLEADER_NAMESPACE()

// “捕获点”管理器。
// 注：
// 1) 这个类参考几何特征来获得一些特征点，如点到直线的垂足，圆弧的圆心、等等，不同的action在不同的状态时可能需要捕捉不同的特征点。
// 2) 目前的特征点都是参考几何特征获得的，所以放在这个模块中，如果以后要参考到工艺甚至工序，到时看看是不是把该功能移走。
class ClGeometryFeature_Export SnapPtManager
{
public:
	SnapPtManager(GlViewPortPtr pViewPort);
	~SnapPtManager(void);

public:
	// 计算捕获点。
	// 注：
	// 1) dwSnapType: 捕捉方式，用枚举类型SnapType中的值相互组合。
	// 2) pPatList, transformMatrix: 基于这些几何特征进行捕捉，由于可能在板材下捕捉，所以transformMatrix是几何特征的变换矩阵。
	// 3) pFirstPt: 参考成员变量m_pFirstPt的注释。
	// 4) dPtX, dPtY: 鼠标当前坐标。
	void CalculateSnapPt(DWORD dwSnapType, PatternListPtr pPatList, const Matrix2D& transformMatrix, Point2DPtr pFirstPt, double dPtX, double dPtY);

	// 绘制辅助点和辅助线，以及捕获点。
	void DrawAssistInfo();

	// 重置捕捉状态。
	// 注:
	//   1) 重置的时机由调用方决定，例如在开始一个action之前。
	void ResetSnapStatus();

	// get the snap point, may be NULL.
	SnapPtPtr GetSnapPt() const { return m_pSnapPt; }

private:
	// 检查是不是捕获了某个几何特征的某个控制点。
	Point2DPtr CheckPatternCtrlPt();

	// 检查是不是捕获了某两个二维线段圆弧的交点。
	Point2DPtr CheckPatternCrossPt(const LineArc2DList* p2DLineArcs_inCaptureRange);

	// 检查光标是不是靠近了某个垂足。
	// 注：
	// 1) 该函数计算m_pFirstPt到p2DLineArcs_inCaptureRange中的二维线段的垂足，和m_pCursorPt距离小于m_dCaptureRange的即为所需。
	Point2DPtr CheckFootPtTo2DLine(const LineArc2DList* p2DLineArcs_inCaptureRange);

	// 检查光标是不是靠近了某个切点。
	// 注：
	// 1) 该函数计算m_pFirstPt到p2DLineArcs_inCaptureRange中的二维圆弧的切点，和m_pCursorPt距离小于m_dCaptureRange的即为所需。
	Point2DPtr CheckTangencyPtTo2DArc(const LineArc2DList* p2DLineArcs_inCaptureRange);

	// 检查光标是不是捕获了某个二维线段圆弧的端点。
	Point2DPtr CheckEndPtOf2DLineArc(const LineArc2DList* p2DLineArcs_inCaptureRange);

	// 检查光标是不是捕获了某个二维线段圆弧的中点。
	Point2DPtr CheckMiddlePtOf2DLineArc(const LineArc2DList* p2DLineArcs_inCaptureRange);

	// 检查光标是不是捕获了某个二维圆弧的圆心。
	Point2DPtr CheckCenterPtOf2DArc();

	// 检查辅助线之间的交点，以及辅助线和几何特征的交点。
	// 注:
	//   1) 如果只有一条辅助线，那么就检查它和几何特征的交点。
	//   2) 如果有两条辅助线，那么就检查它俩之间的交点。
	Point2DPtr CheckAssistLineCrossPt();

	// 计算辅助线。
	void CalcAssistLines();

private:
	GlViewPortPtr m_pViewPort;

	// 输入的数据。
	PatternListPtr m_pPatList; // 基于这些几何特征进行捕捉。
	Matrix2D m_transformMatrix; // 几何特征的变换矩阵。
	Point2DPtr m_pFirstPt; // 上一次的输入点，如画线段时输入的起点，夹点编辑时选中的夹点。如果当前action的当前状态下需要捕获垂足、切点这样的点，就需要传入这个值。
	Point2DPtr m_pCursorPt; // 光标的当前位置。

	// 计算得到的捕获点。
	SnapPtPtr m_pSnapPt;

	// 辅助点数据和辅助线数据。
	// 注：
	// 1) 最多只能有两根辅助线，一个水平一个竖直。
	AssistPtListPtr m_pAssistPtList;
	Line2DPtr m_pHorizontalAssistLine;
	Line2DPtr m_pVerticalAssistLine;

	// 几何特征的缓存数据。
	// 注：
	// 1) 只要有几何特征变化了，就要更新这些数据。
	LONGLONG m_iPreviousPatternListID; // 上次捕捉这些几何特征的特征点。
	LONGLONG m_iLatestModifyTimeStampOfAllPatterns; // 所有几何特征的最后修改时间。
	LineArc2DListPtr m_p2DLineArcsOfAllPatterns; // 所有几何特征的二维直线圆弧数据。
	Point2DListPtr m_pControlPtsOfAllPatterns; // 所有几何特征的控制点。

	// 捕捉距离，世界坐标系下。需要及时更新。
	double m_dCaptureRange;
};

END_CUTLEADER_NAMESPACE()

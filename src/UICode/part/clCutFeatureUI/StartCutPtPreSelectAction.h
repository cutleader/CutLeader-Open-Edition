#pragma once

#include "CutFeatureEditBase.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(LoopStartCutData)

BEGIN_CUTLEADER_NAMESPACE()

// 一些动作的基类，这些动作支持点选和框选“回路起切”特征。
// 注：
//   1) 通过选择“起切点”来选择“回路起切”特征。
class StartCutPtPreSelectAction : public CutFeatureEditBase
{
protected:
	enum PRE_SELECT_STATE
	{
		// 还没有开始选择。
		PRE_SELECT_NO_INPUT			= 0,

		// 鼠标左键按下。
		PRE_SELECT_LEFT_BTN_DOWN	= 1,

		// 鼠标左键抬起。
		PRE_SELECT_LEFT_BTN_UP		= 2,
	};

public:
    StartCutPtPreSelectAction(PatternListPtr pPatList, PatternLoopListPtr pPatternLoopList, LoopTopologyItemListPtr pLoopTopologyItems, 
		LoopCutFeatureListPtr pLoopFeatureList, GlViewPortPtr pViewPort);
	~StartCutPtPreSelectAction(void);

public:  // implement IInteract interface.
	BOOL LButtonDown(double& dPtX, double& dPtY);
	BOOL LButtonUp(double& dPtX, double& dPtY);
	BOOL MovePoint(double& dPtX, double& dPtY);
	BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

public: // implement IAction interface.
	BOOL DrawBeforeFinish();

private:
	// 计算出选中的“回路起切”特征。
	std::vector<std::pair<LoopStartCutDataPtr, Point2D>> CalcSelLoopStaEndCut();

protected:
	// 离光标最近的起切点。
	Point2DPtr m_pActiveStartPt;

	// 选中的起切点及对应的“回路起切特征”。
	std::vector<std::pair<LoopStartCutDataPtr, Point2D>> m_selStaEndPtPairs;

	// 选择状态。
	PRE_SELECT_STATE m_preSelectState;

	// 选择框的数据。
	/*
		LeftTopPointX;
		LeftTopPointY;
		RightBottomPointX;
		RightBottomPointY;
	*/
	double m_data[4];
};

END_CUTLEADER_NAMESPACE()

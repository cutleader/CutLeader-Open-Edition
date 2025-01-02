#pragma once

#include "PartPlacementEditAction.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(LoopStartCutData)
DECLARE_CUTLEADER_CLASS(LoopInstanceList)

BEGIN_CUTLEADER_NAMESPACE()

// 一些动作的基类，这些动作支持在板材上点选和框选“起切点及回路起切特征”。
// 注：
// 1) 通过选择“起切点”来选择“回路起切特征”。
class StartCutPtPreSelectAction2 : public PartPlacementEditAction
{
protected:
	enum PRE_SELECT_STATE
	{
		PRE_SELECT_NO_INPUT			= 0, // 还没有开始选择。
		PRE_SELECT_LEFT_BTN_DOWN	= 1, // 鼠标左键按下。
		PRE_SELECT_LEFT_BTN_UP		= 2, // 鼠标左键抬起。
	};

public:
	StartCutPtPreSelectAction2(IMaterialSizePtr pMaterialSize, PartPlacementListPtr pPartPlacementList, PartInstanceListPtr pPartInstanceList,
		PartGroupListPtr pPartGroups, GlViewPortPtr pViewPort);
	~StartCutPtPreSelectAction2(void);

public:  // implement IInteract interface.
	BOOL LButtonDown(double& dPtX, double& dPtY);
	BOOL LButtonUp(double& dPtX, double& dPtY);
	BOOL MovePoint(double& dPtX, double& dPtY);
	BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

public: // implement IAction interface.
	BOOL DrawBeforeFinish();

protected:
	// 选择状态。
	PRE_SELECT_STATE m_preSelectState;

	// 选择框的数据。
	double m_data[4]; // LeftTopPointX, LeftTopPointY, RightBottomPointX, RightBottomPointY

	// 鼠标移动时的候选起切点，只有一个有效。
	Point2DPtr m_pActiveStartCutPt; // 不框选时的“当前起切点”。
	Point2DListPtr m_pRectSelectStartCutPts; // 框选时的“当前起切点”。

	// 选中的起切点及回路起切特征。
	std::vector<std::pair<LoopStartCutDataPtr, Point2D>> m_selStaEndPtPairs;

	// 缓存所有的回路实例。
	LoopInstanceListPtr m_pAllLoopInstanceList;
};

END_CUTLEADER_NAMESPACE()

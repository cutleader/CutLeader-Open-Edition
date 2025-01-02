#pragma once

#include "CutFeatureEditBase.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(MicroJoint)
DECLARE_CUTLEADER_CLASS(MicroJointList)

BEGIN_CUTLEADER_NAMESPACE()

// 一些动作的基类，这些动作支持点选和框选微连接。
// 注：
//   1) 通过选择“微连接点”来选择“微连接”特征。
class ClCutFeatureUI_Export MicroJointPreSelectAction : public CutFeatureEditBase
{
protected:
	typedef enum tagPreSelectState
	{
		// 还没有开始选择。
		PRE_SELECT_NO_INPUT			= 0,

		// 鼠标左键按下。
		PRE_SELECT_LEFT_BTN_DOWN	= 1,

		// 鼠标左键抬起。
		PRE_SELECT_LEFT_BTN_UP		= 2,
	} PRE_SELECT_STATE;

public:
    MicroJointPreSelectAction(PatternListPtr pPatList, PatternLoopListPtr pPatternLoopList, LoopTopologyItemListPtr pLoopTopologyItems, 
		LoopCutFeatureListPtr pLoopFeatureList, GlViewPortPtr pViewPort);
	virtual ~MicroJointPreSelectAction(void);

public:  // implement IInteract interface.
	BOOL LButtonDown(double& dPtX, double& dPtY);
	BOOL LButtonUp(double& dPtX, double& dPtY);
	BOOL MovePoint(double& dPtX, double& dPtY);
	BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

public: // implement IAction interface.
	BOOL DrawBeforeFinish();

public:
	// 得到选中的微连接特征。
	MicroJointListPtr GetSelectMJList();

private:
	// 计算出选中的“微连接特征”。
	std::vector<std::pair<MicroJointPtr, Point2D>> CalcSelectMJPt();

protected:
	// 离光标最近的微连接点。
	Point2DPtr m_pActiveMJPt;

	// 选中的微连接点及对应的“微连接特征”。
	std::vector<std::pair<MicroJointPtr, Point2D>> m_selMjPtPairs;

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

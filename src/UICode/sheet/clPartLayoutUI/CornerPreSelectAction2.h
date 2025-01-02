#pragma once

#include "PartPlacementEditAction.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(CornerList)
DECLARE_CUTLEADER_CLASS(ICorner)
DECLARE_CUTLEADER_CLASS(LoopInstanceList)

BEGIN_CUTLEADER_NAMESPACE()

// 一些动作的基类，这些动作支持点选和框选角特征。
// 注：
//   1) 通过选择“角点”来选择“角特征”。
class CornerPreSelectAction2 : public PartPlacementEditAction
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
	CornerPreSelectAction2(IMaterialSizePtr pMaterialSize, PartPlacementListPtr pPartPlacementList, PartInstanceListPtr pPartInstanceList, PartGroupListPtr pPartGroups, GlViewPortPtr pViewPort);
	~CornerPreSelectAction2(void);

public:  // implement IInteract interface.
	BOOL LButtonDown(double& dPtX, double& dPtY);
	BOOL LButtonUp(double& dPtX, double& dPtY);
	BOOL MovePoint(double& dPtX, double& dPtY);
	BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

public: // implement IAction interface.
	BOOL DrawBeforeFinish();

public:
	// 得到选中的角特征。
	CornerListPtr GetSelectCornerList();

private:
	// 计算出选中的“角特征”。
	std::vector< std::pair<ICornerPtr, Point2D> > CalcSelectCornerPt();

protected:
	// 离光标最近的角特征点。
	Point2DPtr m_pActiveCornerPt;

	// 选中的“角特征点”及对应的角特征对象。
	std::vector< std::pair<ICornerPtr, Point2D> > m_selCornerPtPairs;

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

	// 缓存所有的回路实例。
	LoopInstanceListPtr m_pAllLoopInstanceList;
};

END_CUTLEADER_NAMESPACE()

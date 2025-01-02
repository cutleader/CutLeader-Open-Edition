#pragma once

#include "PartPlacementEditAction.h"

BEGIN_CUTLEADER_NAMESPACE()

// this action will help select part pmts in some actions.
class PartPlacementPreSelectAction : public PartPlacementEditAction
{
protected:
	enum PARTPMT_PRE_SELECT_STATE
	{
		// 还没有开始选择。
		PRE_SELECT_NO_INPUT			= 0,

		// 鼠标左键按下。
		PRE_SELECT_LEFT_BTN_DOWN	= 1,

		// 鼠标左键抬起。
		PRE_SELECT_LEFT_BTN_UP		= 2,
	};

public:
	PartPlacementPreSelectAction(IMaterialSizePtr pMaterialSize, PartPlacementListPtr pPartPlacementList, PartInstanceListPtr pPartInstanceList,
		PartGroupListPtr pPartGroups, GlViewPortPtr pViewPort);
	~PartPlacementPreSelectAction(void);

public:  // implement IInteract interface.
	BOOL LButtonDown(double& dPtX, double& dPtY);
	BOOL LButtonUp(double& dPtX, double& dPtY);
	BOOL MovePoint(double& dPtX, double& dPtY);
	BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

public: // implement IAction interface.
	BOOL DrawBeforeFinish();

public:
    PartPlacementListPtr GetSelectedPrtPmts() const { return m_pSelectedPartPlacements; }

protected:
	PARTPMT_PRE_SELECT_STATE m_preSelectState;

	// the data for selection.
	/*
		LeftTopPointX;
		LeftTopPointY;
		RightBottomPointX;
		RightBottomPointY;
	*/
	double m_data[4];

    // 鼠标移动的时候高亮的零件放置对象。
    PartPlacementListPtr m_pActivePartPlacements;

    // 选中的零件放置。
    PartPlacementListPtr m_pSelectedPartPlacements;
};

END_CUTLEADER_NAMESPACE()

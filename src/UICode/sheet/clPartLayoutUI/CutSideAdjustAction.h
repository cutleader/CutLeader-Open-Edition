#pragma once

#include <map>
#include <tuple>
#include "cLCutFeatureCommon.h"
#include "PartPlacementEditAction.h"
#include "SpreadSamePartActionBar.h"

DECLARE_CUTLEADER_CLASS(LeadConfigItem)
DECLARE_CUTLEADER_CLASS(ILead)
DECLARE_CUTLEADER_CLASS(LoopInstanceList)
DECLARE_CUTLEADER_CLASS(LoopInstance)

BEGIN_CUTLEADER_NAMESPACE()

// 设置轮廓实例的阴切阳切。
class CutSideAdjustAction : public PartPlacementEditAction
{
	enum CutSideAdjustState
	{
		CutSideAdjust_None				= 0,
		CutSideAdjust_FirstPt			= 1, // 左键按下
		CutSideAdjust_SecondPt			= 2, // 左键抬起
	};

public:
	CutSideAdjustAction(IMaterialSizePtr pMaterialSize, PartPlacementListPtr pPartPlacements, PartInstanceListPtr pPartInstances, PartGroupListPtr pPartGroups,
		PatternLoopCutOffsetType emCutOffsetType, GlViewPortPtr pViewPort, LeadConfigItemPtr pLeadConfigItem, SpreadSamePartActionBar* pSpreadSamePartActionBar);
	~CutSideAdjustAction(void);

public: // implements IInteract interface.
	virtual BOOL LButtonDown(double& dPtX, double& dPtY) override;
	virtual BOOL LButtonUp(double& dPtX, double& dPtY) override;
	virtual BOOL MovePoint(double& dPtX, double& dPtY) override;
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) override;

public: // implement IAction interface.
	virtual BOOL DrawBeforeFinish() override;
	virtual int GetActionType() const override { return CUTSIDE_ADJUST; }
	virtual int GetActionName() const override { return IDS_SWITCH_CUTSIDE; }

private:
	CutSideAdjustState m_emState;

	// the data for selection.
	double m_data[4]; // LeftTopPointX, LeftTopPointY, RightBottomPointX, RightBottomPointY

	// 鼠标移动时的候选loop inst，只有一个有效。
	LoopInstancePtr m_pActiveLoopInstance; // 不框选时的“当前loop inst”。
	LoopInstanceListPtr m_pRectSelectLoopInstances; // 框选时的“当前loop inst”。

	// 所有的轮廓实例。
	LoopInstanceListPtr m_pAllLoopInstances;

	PatternLoopCutOffsetType m_emCutOffsetType;
	LeadConfigItemPtr m_pLeadConfigItem;
    SpreadSamePartActionBar* m_pSpreadSamePartActionBar;
};

END_CUTLEADER_NAMESPACE()

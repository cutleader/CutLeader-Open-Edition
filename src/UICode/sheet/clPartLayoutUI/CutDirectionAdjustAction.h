#pragma once

#include "PartPlacementEditAction.h"
#include "SpreadSamePartActionBar.h"
#include "clCutFeatureCommon.h"

DECLARE_CUTLEADER_CLASS(LoopInstanceList)
DECLARE_CUTLEADER_CLASS(LoopInstance)

BEGIN_CUTLEADER_NAMESPACE()

// 调整回路实例的切割方向。
class CutDirectionAdjustAction : public PartPlacementEditAction
{
	enum CutDirectionAdjustState
	{
		CutDirectionAdjust_None		= 0,
		CutDirectionAdjust_FirstPt	= 1, // 左键按下
		CutDirectionAdjust_SecondPt = 2, // 左键抬起
	};

public:
	CutDirectionAdjustAction(IMaterialSizePtr pMaterialSize, PartPlacementListPtr pPartPlacements, PartInstanceListPtr pPartInstances,
		PartGroupListPtr pPartGroups, CutDirectionChangeType emCutDirectionChangeType, GlViewPortPtr pViewPort, SpreadSamePartActionBar* pSpreadSamePartActionBar);
	virtual ~CutDirectionAdjustAction(void);

public: // implements IInteract interface.
	virtual BOOL LButtonDown(double& dPtX, double& dPtY);
	virtual BOOL LButtonUp(double& dPtX, double& dPtY) override;
	virtual BOOL MovePoint(double& dPtX, double& dPtY);
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

public: // implement IAction interface.
	virtual BOOL DrawBeforeFinish() override;
	virtual int GetActionType() const override { return DIRECTION_ADJUST; }
	virtual int GetActionName() const override { return IDS_CHANGE_CUTDIR; }

private:
	CutDirectionAdjustState m_emState;

	// the data for selection.
	double m_data[4];

	// 鼠标移动时的候选loop inst，只有一个有效。
	LoopInstancePtr m_pActiveLoopInstance; // 不框选时的“当前loop inst”。
	LoopInstanceListPtr m_pRectSelectLoopInstances; // 框选时的“当前loop inst”。

	CutDirectionChangeType m_emCutDirectionChangeType;
	LoopInstanceListPtr m_pAllLoopInstances;
	SpreadSamePartActionBar* m_pSpreadSamePartActionBar;
};

END_CUTLEADER_NAMESPACE()

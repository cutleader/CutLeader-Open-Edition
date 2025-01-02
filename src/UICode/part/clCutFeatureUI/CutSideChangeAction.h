#pragma once

#include "clCutFeatureCommon.h"
#include "Point2D.h"
#include "CutFeatureEditBase.h"

DECLARE_CUTLEADER_CLASS(LeadConfigItem)
DECLARE_CUTLEADER_CLASS(ILead)

BEGIN_CUTLEADER_NAMESPACE()

// 设置几何轮廓的阴切阳切。
class ClCutFeatureUI_Export CutSideChangeAction : public CutFeatureEditBase
{
	enum CutSideChangeState
	{
		CutSideChange_None = 0,
		CutSideChange_FirstPt = 1, // 左键按下
		CutSideChange_SecondPt = 2, // 左键抬起
	};

public:
	CutSideChangeAction(PatternListPtr pPatList, PatternLoopListPtr pPatternLoopList, LoopTopologyItemListPtr pLoopTopologyItems, 
		LoopCutFeatureListPtr pLoopFeatureList, PatternLoopCutOffsetType emCutOffsetType, GlViewPortPtr pViewPort, LeadConfigItemPtr pLeadConfigItem);
	~CutSideChangeAction(void);

public: // implements IInteract interface.
	virtual BOOL LButtonDown(double& dPtX, double& dPtY) override;
	virtual BOOL LButtonUp(double& dPtX, double& dPtY) override;
	virtual BOOL MovePoint(double& dPtX, double& dPtY) override;
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) override;

public: // implement IAction interface.
	virtual BOOL DrawBeforeFinish() override;
	virtual int GetActionType() const override { return CUTSIDE_CHANGE; }
	virtual int GetActionName() const override { return IDS_SWITCH_CUTSIDE; }

private:
	CutSideChangeState m_emState;

	// 框选的框框(左上x，左上y，右下x，右下y)
	double m_boxData[4];

	// 鼠标移动时的候选轮廓，只有一个有效。
	IPatternLoopPtr m_pActivePatternLoop; // 不框选时的“当前轮廓”。
	PatternLoopListPtr m_pActivePatternLoops_inRect; // 框选时的“当前轮廓”。

	PatternLoopCutOffsetType m_emCutOffsetType;
	LeadConfigItemPtr m_pLeadConfigItem;
};

END_CUTLEADER_NAMESPACE()

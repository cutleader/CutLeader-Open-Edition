#pragma once

#include "PartPlacementEditAction.h"
#include "SpreadSamePartActionBar.h"
#include "PatternPosition.h"
#include "Point2D.h"

DECLARE_CUTLEADER_CLASS(CondConfigItem)
DECLARE_CUTLEADER_CLASS(LeadConfigItem)
DECLARE_CUTLEADER_CLASS(ILoopStartCutPt)
DECLARE_CUTLEADER_CLASS(ILead)
DECLARE_CUTLEADER_CLASS(LoopStartCutData)
DECLARE_CUTLEADER_CLASS(LoopInstance)
DECLARE_CUTLEADER_CLASS(LoopInstanceList)

BEGIN_CUTLEADER_NAMESPACE()

// 调整回路起切点。
class StartCutPtAdjustAction : public PartPlacementEditAction
{
public:
	StartCutPtAdjustAction(IMaterialSizePtr pMaterialSize, PartPlacementListPtr pPartPlacements, PartInstanceListPtr pPartInstances, PartGroupListPtr pPartGroups,
		GlViewPortPtr pViewPort, LeadConfigItemPtr pLeadConfigItem, SpreadSamePartActionBar* pSpreadSamePartActionBar);
	~StartCutPtAdjustAction(void);

public: // implements IInteract interface.
	virtual BOOL LButtonDown(double& dPtX, double& dPtY);
	virtual BOOL MovePoint(double& dPtX, double& dPtY);
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

public: // implement IAction interface.
	virtual BOOL DrawBeforeFinish() override;
	virtual int GetActionType() const override { return START_PT_ADJUST; }
	virtual int GetActionName() const override { return IDS_EDIT_LOOP_START_PT; }
	virtual SnapInputPtr GetSnapInput() const override;

private:
	// 把起切点移到角上。
	void StartPt2Corner(LoopStartCutDataPtr pLoopStartCutData, ILoopStartCutPtPtr pLoopStartCutPt, ILeadPtr pLeadIn, ILeadPtr pLeadOut);

	// 把起切点移到图形上。
	void StartPt2Geom(LoopStartCutDataPtr pLoopStartCutData, LONGLONG iNewPatID, const PatternPosition& newPatPos);

private:
	// 当前被捕获的回路实例。
	LoopInstancePtr m_pActiveLoopInstance;

	LoopInstanceListPtr m_pAllLoopInstances;
	LeadConfigItemPtr m_pLeadConfigItem;
	SpreadSamePartActionBar* m_pSpreadSamePartActionBar;
};

END_CUTLEADER_NAMESPACE()

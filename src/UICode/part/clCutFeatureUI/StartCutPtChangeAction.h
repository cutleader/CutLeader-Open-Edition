#pragma once

#include "Point2D.h"
#include "CutFeatureEditBase.h"
#include "PatternPosition.h"

DECLARE_CUTLEADER_CLASS(LeadConfigItem)
DECLARE_CUTLEADER_CLASS(LoopStartCutData)

BEGIN_CUTLEADER_NAMESPACE()

// 调整回路起切点。
class StartCutPtChangeAction : public CutFeatureEditBase
{
public:
	StartCutPtChangeAction(PatternListPtr pPatList, PatternLoopListPtr pPatternLoopList, LoopTopologyItemListPtr pLoopTopologyItems, 
		LoopCutFeatureListPtr pLoopFeatureList, GlViewPortPtr pViewPort, LeadConfigItemPtr pLeadConfigItem);
	~StartCutPtChangeAction(void);

public: // implements IInteract interface.
	virtual BOOL LButtonDown(double& dPtX, double& dPtY);
	virtual BOOL MovePoint(double& dPtX, double& dPtY);
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

public: // implement IAction interface.
	virtual BOOL DrawBeforeFinish() override;
	virtual int GetActionType() const override { return START_PT_CHANGE; }
	virtual int GetActionName() const override { return IDS_EDIT_LOOP_START_PT; }
	virtual SnapInputPtr GetSnapInput() const override;

private:
	// 把起切点移到图形上。
	void StartPt2Geom(LoopStartCutDataPtr pLoopStartCutData, LONGLONG iNewPatternID, const PatternPosition& newPatternPosition);

private:
	// 当前被捕获的轮廓。
	IPatternLoopPtr m_pActivePatternLoop;

	LeadConfigItemPtr m_pLeadConfigItem;
};

END_CUTLEADER_NAMESPACE()

#pragma once

#include "PatternEditActionBase.h"

DECLARE_CUTLEADER_CLASS(LoopDrawer)
DECLARE_CUTLEADER_CLASS(IPatternLoop)

BEGIN_CUTLEADER_NAMESPACE()

// use this class to select loop.
// To be implemented:
//   1) currently we do not have many operations for loop, so this class is not used now.
class ClGeometryFeatureUI_Export PatternLoopSelectAction : public PatternEditActionBase
{
public:
	PatternLoopSelectAction(PartCadDataPtr pPartCadData, GlViewPortPtr pViewPort);
	~PatternLoopSelectAction(void);

public:  // implement IInteract interface.
	BOOL LButtonDown(double& dPtX, double& dPtY);
	BOOL LButtonUp(double& dPtX, double& dPtY);
	BOOL MovePoint(double& dPtX, double& dPtY);

	BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	BOOL OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);

public: // implement IAction interface.
	BOOL DrawBeforeFinish() override;
	virtual int GetActionType() const override { return EDIT_SELECTPATLOOP; }
	virtual int GetActionName() const override { return IDS_SELECT_GEOM; }

public:
	void SelectLoop(IPatternLoopPtr pSelectedLoop) { m_pSelectedLoop = pSelectedLoop; }

private:
	IPatternLoopPtr m_pSelectedLoop;

	LoopDrawerPtr m_loopDrawerPtr;
};

END_CUTLEADER_NAMESPACE()

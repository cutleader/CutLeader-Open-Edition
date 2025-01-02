#pragma once

#include "CanvasPatternPreSelectAction.h"
#include "clGeometryFeatureCommon.h"

DECLARE_CUTLEADER_CLASS(PatternDeleteAction)
DECLARE_CUTLEADER_CLASS(TextStructList)
DECLARE_CUTLEADER_CLASS(CanvasLoopAlignAction)

BEGIN_CUTLEADER_NAMESPACE()

// this action can align the loops.
class CanvasLoopAlignAction : public CanvasPatternPreSelectAction
{
public:
	CanvasLoopAlignAction(PartPlacementPtr pPartPlacement, AlignLoopStyle emAlignLoopStyle, SelectPatternOrLoopActionBar* pSelectPatternOrLoopActionBar, GlViewPortPtr pViewPort);
	~CanvasLoopAlignAction(void);

public: // implement IInteract interface.
	virtual BOOL LButtonDown(double& dPtX, double& dPtY) override;
	virtual BOOL LButtonUp(double& dPtX, double& dPtY) override;
	virtual BOOL MovePoint(double& dPtX, double& dPtY) override;
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) override;

public:  // implement IAction interface.
	virtual BOOL DrawBeforeFinish() override;
	virtual int GetActionType() const override { return AlignLoopsOnCanvas; }
	virtual int GetActionName() const override { return IDS_COMMAND_AlignLoop; }
	virtual void UpdateForUndoRedo() override;
	virtual SnapInputPtr GetSnapInput() const override;

private:
	AlignLoopStyle m_emAlignLoopStyle;
};

END_CUTLEADER_NAMESPACE()

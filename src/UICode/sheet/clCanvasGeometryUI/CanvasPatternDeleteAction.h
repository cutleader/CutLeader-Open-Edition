#pragma once

#include "CanvasPatternPreSelectAction.h"

DECLARE_CUTLEADER_CLASS(PatternDeleteAction)
DECLARE_CUTLEADER_CLASS(TextStructList)

BEGIN_CUTLEADER_NAMESPACE()

// this action can delete the pattern.
class CanvasPatternDeleteAction : public CanvasPatternPreSelectAction
{
public:
	CanvasPatternDeleteAction(PartPlacementPtr pPartPlacement, PatternListPtr pSelectedPatterns, TextStructListPtr pSelectedTextStructs,
		SelectPatternOrLoopActionBar* pSelectPatternOrLoopActionBar, GlViewPortPtr pViewPort);
	~CanvasPatternDeleteAction(void);

public: // implement IInteract interface.
	BOOL LButtonDown(double& dPtX, double& dPtY);
	BOOL LButtonUp(double& dPtX, double& dPtY);
	BOOL RButtonUp(double& dPtX, double& dPtY);
	BOOL MovePoint(double& dPtX, double& dPtY);
	BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

public:  // implement IAction interface.
	virtual BOOL DrawBeforeFinish() override;
	virtual int GetActionType() const override { return DeletePatternOnCanvas; }
	virtual int GetActionName() const override { return IDS_COMMAND_DEL_PAT; }
	virtual void UpdateForUndoRedo() override;
	virtual SnapInputPtr GetSnapInput() const override;

public:
	// 删除选中的几何特征。
	void DeleteSelectedPatterns();
};

END_CUTLEADER_NAMESPACE()

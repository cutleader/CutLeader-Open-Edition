#pragma once

#include "PatternPreSelectAction.h"

DECLARE_CUTLEADER_CLASS(PatternDeleteAction)

BEGIN_CUTLEADER_NAMESPACE()

// this action can delete the pattern.
// notes:
//   1) if patterns have been selected before enter this action, delete selected
//      patterns and set PatternSelectAction as the next action.
class PatternDeleteAction : public PatternPreSelectAction
{
public:
	PatternDeleteAction(PartCadDataPtr pPartCadData, GlViewPortPtr pViewPort, PatternListPtr pSelectedPatterns, TextStructListPtr pSelectedTextStructs);
	~PatternDeleteAction(void);

public: // implement IInteract interface.
	BOOL LButtonDown(double& dPtX, double& dPtY);
	BOOL LButtonUp(double& dPtX, double& dPtY);
	BOOL RButtonUp(double& dPtX, double& dPtY);
	BOOL MovePoint(double& dPtX, double& dPtY);

	BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	BOOL OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);

public:  // implement IAction interface.
	virtual BOOL DrawBeforeFinish() override;
	virtual int GetActionType() const override { return EDIT_DELETE; }
	virtual int GetActionName() const override { return IDS_COMMAND_DEL_PAT; }
	virtual void UpdateForUndoRedo();
	virtual SnapInputPtr GetSnapInput() const override;

public:
	// 删除选中的几何特征。
	// 注:
	// 1) if no pattern selected, do nothing.
	void DeleteSelectedPats();
};

END_CUTLEADER_NAMESPACE()

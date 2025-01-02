#pragma once

#include "PatternEditActionBase.h"

DECLARE_CUTLEADER_CLASS(PatternExplodeAction)

BEGIN_CUTLEADER_NAMESPACE()

// this class will explode the std/array pattern.
// notes:
//   1) do not support multi-select currently.
//   2) for explode array pattern:
//      a) if line array pattern's repeat-num as 3, after exploding the array pattern, we will get 3 new patterns, and delete the array pattern and its control pattern.
//      b) if the control pattern is std pattern, do not explode the control pattern.
class PatternExplodeAction : public PatternEditActionBase
{
	typedef enum tagExplodeGridPatState
	{
		EXPLODE_PAT_NOTSTART		= 0,
		EXPLODE_PAT_SELECTPAT		= 1,
	} EXPLODE_PAT_STATE;

public:
	PatternExplodeAction(PartCadDataPtr pPartCadData, GlViewPortPtr pViewPort);
	~PatternExplodeAction(void);

public:  // implement IInteract interface.
	virtual BOOL LButtonDown(double& dPtX, double& dPtY);
	virtual BOOL RButtonUp(double& dPtX, double& dPtY);
	virtual BOOL MovePoint(double& dPtX, double& dPtY);
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

public: // implement IAction interface.
	virtual BOOL DrawBeforeFinish() override;
	virtual int GetActionType() const override { return EDIT_EXPLODE; }
	virtual int GetActionName() const override { return IDS_EXPLODE; }
	virtual void UpdateForUndoRedo() override;

public:
	// explode the pattern.
	// notes:
	//   1) user should imput a std/array pattern.
	void ExplodePat(IPatternPtr pStdArrayPat);

private:
	// only one pattern is highlighted during this action.
	IPatternPtr m_pActivePat;

	// update this when mouse click.
	IPatternPtr m_pStdArrayPat;

	EXPLODE_PAT_STATE m_state;
};

END_CUTLEADER_NAMESPACE()

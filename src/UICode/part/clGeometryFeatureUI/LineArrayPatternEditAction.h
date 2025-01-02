#pragma once

#include "ArrayPatternEditActionBase.h"

BEGIN_CUTLEADER_NAMESPACE()

// this class will create or edit Pattern Line.
// notes:
//   1) in this action, we do not support keyboard input value.
//   2) by default, we use the base point of the control pattern as base point, of course we can change the base point after create the LineArrayPattern.
class LineArrayPatternEditAction : public ArrayPatternEditActionBase
{
	typedef enum tagEditGridPatternLineState
	{
		EDIT_PATTERNLINE_NOTSTART			= 0,
		EDIT_PATTERNLINE_BASEPOINT			= 1,
		EDIT_PATTERNLINE_SPACEANGLE			= 2,
		EDIT_PATTERNLINE_REPEATCOUNT		= 3,

	} EDIT_PATTERNLINE_STATE;

public:
	LineArrayPatternEditAction(PartCadDataPtr pPartCadData, GlViewPortPtr pViewPort);
	~LineArrayPatternEditAction(void);

public: // implement IInteract interface.
	virtual BOOL LButtonDown(double& dPtX, double& dPtY);
	virtual BOOL MovePoint(double& dPtX, double& dPtY);
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);

public: // implement IAction interface.
	virtual BOOL DrawBeforeFinish() override;
	virtual int GetActionType() const override { return DRAW_PATTERNLINE; }
	virtual int GetActionName() const override { return IDS_GEOMETRY_PATLINE_TIP; }

	// update the action for undo/redo operation.
	// notes:
	//   1) return to PatternSelectAction:
	//      a) if array pattern is lost when grip-edit pattern.
	//   2) reset the status of the action.
	//      a) if control pattern is lost when draw array pattern.
	virtual void UpdateForUndoRedo() override;

public:	// implement IGripEditPatternAction interface.
	virtual void SetGripEditPattern(IPatternPtr pPattern);
	virtual void SetGripEditPt(double dPtX, double dPtY);

private:
	void CalcSpaceAngle(double dPtX, double dPtY);

	// calc the repeat count.
	BOOL CalcRepeatCount(double dPtX, double dPtY);

private:
	/*
		double	StartPointX;
		double	StartPointY;
		double	Spacing;
		double	Angle;
		long	RepeatCount;
	*/
	double m_data[5];

	EDIT_PATTERNLINE_STATE m_editState;
};

END_CUTLEADER_NAMESPACE()

#pragma once

#include "ArrayPatternEditActionBase.h"

BEGIN_CUTLEADER_NAMESPACE()

// this class will create or edit Pattern grid.
// notes:
//   1) in this action, we do not support keyboard input value.
class GridArrayPatternEditAction : public ArrayPatternEditActionBase
{
	typedef enum tagEditGridPatternGridState
	{
		EDIT_PATTERNGRID_NOTSTART			= 0,
		EDIT_PATTERNGRID_BASEPOINT			= 1,
		EDIT_PATTERNGRID_SPACING			= 2,
		EDIT_PATTERNGRID_SPACINTX			= 3,
		EDIT_PATTERNGRID_SPACINTY			= 4,
		EDIT_PATTERNGRID_REPEATCOUNT		= 5,
		EDIT_PATTERNGRID_REPEATCOUNTX		= 6,
		EDIT_PATTERNGRID_REPEATCOUNTY		= 7,
	} EDIT_PATTERNGRID_STATE;

public:
	GridArrayPatternEditAction(PartCadDataPtr pPartCadData, GlViewPortPtr pViewPort);
	~GridArrayPatternEditAction(void);

public: // implement IInteract interface.
	virtual BOOL LButtonDown(double& dPtX, double& dPtY);
	virtual BOOL MovePoint(double& dPtX, double& dPtY);
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);

public: // implement IAction interface.
	virtual int GetActionType() const override { return DRAW_PATTERNGRID; }
	virtual int GetActionName() const override { return IDS_GEOMETRY_PATGRID_TIP; }

	virtual BOOL DrawBeforeFinish() override;

	// update the action for undo/redo operation.
	// notes:
	//   1) return to PatternSelectAction:
	//      a) if array pattern is lost when grip-edit pattern.
	//   2) reset the status of the action.
	//      a) if control pattern is lost when draw array pattern.
	virtual void UpdateForUndoRedo() override;

public:	//for Grip Edit.
	virtual void SetGripEditPattern(IPatternPtr pPattern);
	virtual void SetGripEditPt(double dPtX, double dPtY);

private:
	void CalcXSpacing(double dPtX, double dPtY);
	void CalcYSpacing(double dPtX, double dPtY);
	BOOL CalcXRepeatCount(double dPtX, double dPtY);
	BOOL CalcYRepeatCount(double dPtX, double dPtY);

private:
	EDIT_PATTERNGRID_STATE m_editState;

	/*
		0	StartPointX;
		1	StartPointY;
		2	XSpacing;
		3	YSpacing;
		4	XRepeatCount;
		5	YRepeatCount;
	*/
	double m_data[7];
};

END_CUTLEADER_NAMESPACE()

#pragma once

#include "PatternGripEditActionBase.h"

BEGIN_CUTLEADER_NAMESPACE()

// this class can create/edit the arc pattern.
// notes:
//   1) use keyboard input to create arc:
//      step 1: input center point.
//      step 2: input the start point.
//      step 3: input the sweep angle.
//	 2) use keyboard input to grip edit arc:
//      a) EDIT_ARC_CENTER_POINT: input (a, b) to move center point.
//      b) EDIT_ARC_START_POINT, EDIT_ARC_START_POINT: input start and sweep angle.
//      c) EDIT_ARC_MID_POINT: input (a, b) to change the center point and radius.
class ArcPatternEditAction : public PatternGripEditActionBase
{
	typedef enum tagDrawArcState
	{
		DRAW_ARC_NOTSTART     = 0,
		DRAW_ARC_CENTER_POINT = 1,
		DRAW_ARC_START_POINT  = 2,
		DRAW_ARC_END_POINT    = 3,

	} DRAW_ARC_STATE;

	typedef enum tagEditArcState
	{
		EDIT_ARC_NOTSTART     = 0,
		EDIT_ARC_CENTER_POINT = 1,
		EDIT_ARC_START_POINT  = 2,
		EDIT_ARC_MID_POINT    = 3,
		EDIT_ARC_END_POINT    = 4,

	} EDIT_ARC_STATE;

public:
	ArcPatternEditAction(PartCadDataPtr pPartCadData, GlViewPortPtr pViewPort);
	~ArcPatternEditAction(void);

public: // implement IInteract interface.
	virtual BOOL LButtonDown(double& dPtX, double& dPtY);
	virtual BOOL MovePoint(double& dPtX, double& dPtY);
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);

public: // implement IAction interface.
	virtual BOOL DrawBeforeFinish() override;
	virtual int GetActionType() const override { return DRAW_ARC_STD; }
	virtual int GetActionName() const override { return IDS_GEOMETRY_ARC_TIP; }
	virtual void ConvertKBInput(double& dVal1, double& dVal2) override;
	virtual void UpdateForUndoRedo() override; // if pattern is lost when grip-edit pattern, return to PatternSelectAction.

public:	// implement IGripEditPatternAction interface.
	virtual void SetGripEditPattern(IPatternPtr pPattern);
	virtual void SetGripEditPt(double dPtX, double dPtY);

private:
	void ChangeMidPoint(double midPtX, double midPtY);

private:
	DRAW_ARC_STATE m_iStatus;
	EDIT_ARC_STATE m_editState;

	// the data for creating arc pattern.
	/* 
		CenterPointX 
		CenterPointY 
		Radius 
		StartAngle 
		MoveAngle
	*/
	double m_data[5];
};

END_CUTLEADER_NAMESPACE()

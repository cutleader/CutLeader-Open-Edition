#pragma once

#include "ArrayPatternEditActionBase.h"

BEGIN_CUTLEADER_NAMESPACE()

// this class will create or edit Pattern circle.
// notes:
//   1) in this action, we do not support keyboard input value.
class CircleArrayPatternEditAction : public ArrayPatternEditActionBase
{
	typedef enum tagEditGridPatternCircleState
	{
		EDIT_PATTERNCIRCLE_NOTSTART			= 0,
		EDIT_PATTERNCIRCLE_BASEPOINT			= 1,
		EDIT_PATTERNCIRCLE_STARTANGLE			= 2,
		EDIT_PATTERNCIRCLE_SPACE				= 3,

	} EDIT_PATTERNCIRCLE_STATE;

public:
	CircleArrayPatternEditAction(PartCadDataPtr pPartCadData, GlViewPortPtr pViewPort);
	~CircleArrayPatternEditAction(void);

public: // implement IInteract interface.
	virtual BOOL LButtonDown(double& dPtX, double& dPtY);
	virtual BOOL MovePoint(double& dPtX, double& dPtY);
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);

public: // implement IAction interface.
	virtual BOOL DrawBeforeFinish() override;
	virtual int GetActionType() const override { return DRAW_PATTERNCIRCLE; }
	virtual int GetActionName() const override { return IDS_GEOMETRY_PATCIRCLE_TIP; }

	// update the action for undo/redo operation.
	// notes:
	//   1) return to PatternSelectAction:
	//      a) if array pattern is lost when grip-edit pattern.
	//   2) reset the status of the action.
	//      a) if control pattern is lost when draw array pattern.
	virtual void UpdateForUndoRedo();

public:	//for Grip Edit.
	virtual void SetGripEditPattern(IPatternPtr pPattern);
	virtual void SetGripEditPt(double dPtX, double dPtY);

private:
	void CalcRadiusAngle(double dPtX, double dPtY);
	void CalcSpaceAndRepeatCount(double dPtX, double dPtY);

private:
	EDIT_PATTERNCIRCLE_STATE m_editState;

	/*
		0	CenterPointX;
		1	CenterPointY;
		2   StartAngle;
		3   Radius;
		4	SpaceAngle;
		5	RepeatCount;
	*/
	double m_data[6];
};

END_CUTLEADER_NAMESPACE()

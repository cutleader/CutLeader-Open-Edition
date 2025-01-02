#pragma once

#include "PatternGripEditActionBase.h"

BEGIN_CUTLEADER_NAMESPACE()

// this class can create/edit the double D pattern.
// notes:
//   1) use keyboard input to create DoubleD:
//      step 1: input (a, b) to specify center point.
//      step 2: input the radius.
//      step 3: input (a, b) to specify the width and angle.
//	 2) use keyboard input to grip edit DoubleD:
//      a) EDIT_ARC_CENTER_POINT:
//         input (a, b) to move center point.
//      b) EDIT_ARC_START_POINT, EDIT_ARC_START_POINT: 
//         input start and sweep angle.
//      c) EDIT_ARC_MID_POINT: 
//         input (a, b) to change the center point and radius
class DoubleDPatternEditAction : public PatternGripEditActionBase
{
	typedef enum tagDrawDoubleDState
	{
		DRAW_DOUBLED_NOTSTART         = 0,
		DRAW_DOUBLED_FIRST_POINT	  = 1,
		DRAW_DOUBLED_SECOND_POINT	  = 2,
		DRAW_DOUBLED_THIRD_POINT      = 3
	} DRAW_DOUBLED_STATE;

	typedef enum tagEditDoubleDState
	{
		EDIT_DOUBLED_NOTSTART         = 0,
		EDIT_DOUBLED_CENTER_POINT	  = 1,
		EDIT_DOUBLED_TOP_POINT		  = 2,
		EDIT_DOUBLED_RIGHT_POINT	  = 3,
		EDIT_DOUBLED_LEFT_POINT		  = 4,
		EDIT_DOUBLED_BOTTOM_POINT	  = 5,
	} EDIT_DOUBLED_STATE;

public:
	DoubleDPatternEditAction(PartCadDataPtr pPartCadData, GlViewPortPtr pViewPort);
	~DoubleDPatternEditAction(void);

public: // implement IInteract interface.
	virtual BOOL LButtonDown(double& dPtX, double& dPtY);
	virtual BOOL MovePoint(double& dPtX, double& dPtY);
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);

public: // implement IAction interface.
	virtual BOOL DrawBeforeFinish() override;
	virtual int GetActionType() const override { return DRAW_DOUBLED; }
	virtual int GetActionName() const override { return IDS_GEOMETRY_DOUBLED; }
	virtual void ConvertKBInput(double& dVal1, double& dVal2) override;
	virtual void UpdateForUndoRedo() override; // if pattern is lost when grip-edit pattern, return to PatternSelectAction.

public: // implement IGripEditPatternAction interface.
	virtual void SetGripEditPattern(IPatternPtr pPattern);
	virtual void SetGripEditPt(double dPtX, double dPtY);

private:
	DRAW_DOUBLED_STATE m_drawState;
	EDIT_DOUBLED_STATE m_editState;

	// the data for creating double D pattern.
	/*
	  CenterPointX;
	  CenterPointY;
	  Radius;
	  Chord;
	  Angle;
	*/
	double m_data[5];
};

END_CUTLEADER_NAMESPACE()

#pragma once

#include "PatternGripEditActionBase.h"

BEGIN_CUTLEADER_NAMESPACE()

// this class creates/edits the SingleD pattern.
class SingleDPatternEditAction : public PatternGripEditActionBase
{
	typedef enum tagDrawSingleDState
	{
		DRAW_SINGLED_NOTSTART	       = 0,
		DRAW_SINGLED_FIRST_POINT	   = 1,
		DRAW_SINGLED_SECOND_POINT	   = 2,
		DRAW_SINGLED_THIRD_POINT	   = 3,

	} DRAW_SINGLED_STATE;

	typedef enum tagEditSingleDState
	{
		EDIT_SINGLED_NOTSTART          = 0,
		EDIT_SINGLED_CENTER_POINT	   = 1,
		// default: top point middle point of line
		EDIT_SINGLED_TOP_POINT		   = 2,
		EDIT_SINGLED_LEFT_POINT		   = 3,
		EDIT_SINGLED_RIGHT_POINT       = 4,
		EDIT_SINGLED_BOTTOM_POINT      = 5,

	} EDIT_SINGLED_STATE;

public:
	SingleDPatternEditAction(PartCadDataPtr pPartCadData, GlViewPortPtr pViewPort);
	~SingleDPatternEditAction(void);

public: // implement IInteract interface.
	virtual BOOL LButtonDown(double& dPtX, double& dPtY);
	virtual BOOL MovePoint(double& dPtX, double& dPtY);
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);

public: // implement IAction interface.
	virtual BOOL DrawBeforeFinish() override;

	virtual int GetActionType() const override { return DRAW_SINGLED; }
	virtual int GetActionName() const override { return IDS_GEOMETRY_SINGLED_TIP; }

	virtual void ConvertKBInput(double& dVal1, double& dVal2) override;

	// if pattern is lost when grip-edit pattern, return to PatternSelectAction.
	virtual void UpdateForUndoRedo() override;

public: // implement IGripEditPatternAction interface.
	virtual void SetGripEditPattern(IPatternPtr pPattern);
	virtual void SetGripEditPt(double dPtX, double dPtY);

private:
	/*
		double CenterPointX;
		double CenterPointY;
		double Radius;
		double Chord;
		double Angle;
	*/
	double m_data[5];

	DRAW_SINGLED_STATE m_drawState;
	EDIT_SINGLED_STATE m_editState;
};

END_CUTLEADER_NAMESPACE()

#pragma once

#include "PatternGripEditActionBase.h"

BEGIN_CUTLEADER_NAMESPACE()

// create and edit round corner hole in this class.
class RoundCornerHolePatternEditAction : public PatternGripEditActionBase
{
	typedef enum tagDrawRcRectState
	{
		DRAW_RCRECT_NOTSTART		= 0,
		DRAW_RCRECT_FIRST_POINT		= 1,
		DRAW_RCRECT_SECOND_POINT	= 2,
		DRAW_RCRECT_THIRD_POINT		= 3,

	} DRAW_RCRECT_STATE;

	typedef enum tagEditRcRectState
	{
		EDIT_RCRECT_NOTSTART		= 0,
		EDIT_RCRECT_CENTER_POINT    = 1,
		EDIT_RCRECT_TOP_POINT		= 2,
		EDIT_RCRECT_LEFT_POINT		= 3,
		EDIT_RCRECT_BOTTOM_POINT	= 4,
		EDIT_RCRECT_RIGHT_POINT		= 5,
		EDIT_RCRECT_CORNER_POINT    = 6,

	} EDIT_RCRECT_STATE;

public:
	RoundCornerHolePatternEditAction(PartCadDataPtr pPartCadData, GlViewPortPtr pViewPort);
	~RoundCornerHolePatternEditAction(void);

public: // implement IInteract interface.
	virtual BOOL LButtonDown(double& dPtX, double& dPtY);
	virtual BOOL MovePoint(double& dPtX, double& dPtY);
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);

public: // implement IAction interface.
	virtual BOOL DrawBeforeFinish() override;

	virtual int GetActionType() const override { return DRAW_RC_HOLE; }
	virtual int GetActionName() const override { return IDS_GEOMETRY_RCHOLE; }

	virtual void ConvertKBInput(double& dVal1, double& dVal2) override;

	// if pattern is lost when grip-edit pattern, return to PatternSelectAction.
	virtual void UpdateForUndoRedo();

public: // implement IGripEditPatternAction interface.
	virtual void SetGripEditPattern(IPatternPtr pPattern);
	virtual void SetGripEditPt(double dPtX, double dPtY);

private:
	/*
		double CenterPointX;
		double CenterPointY;
		double Length;
		double Width;
		double Radius;
		double BaseAngle;
	*/
	double m_data[6];

	DRAW_RCRECT_STATE m_drawState;
	EDIT_RCRECT_STATE m_editState;
};

END_CUTLEADER_NAMESPACE()

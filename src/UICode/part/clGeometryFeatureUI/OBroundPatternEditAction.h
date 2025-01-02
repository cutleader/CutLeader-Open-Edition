#pragma once

#include "PatternGripEditActionBase.h"

BEGIN_CUTLEADER_NAMESPACE()

// this class can create/edit the OBround pattern.
class OBroundPatternEditAction : public PatternGripEditActionBase
{
	typedef enum tagDrawObroundState
	{
		DRAW_OBROUND_NOTSTART			= 0,
		DRAW_OBROUND_FIRST_POINT		= 1,
		DRAW_OBROUND_SECOND_POINT		= 2,
		DRAW_OBROUND_THIRD_POINT		= 3,

	} DRAW_OBROUND_STATE;

	typedef enum tagEditObroundState
	{
		EDIT_OBROUND_NOTSTART	        = 0,
		EDIT_OBROUND_CENTER_POINT		= 1,
		EDIT_OBROUND_TOP_POINT			= 2,
		EDIT_OBROUND_LEFT_POINT			= 3,
		EDIT_OBROUND_BOTTOM_POINT		= 4,
		EDIT_OBROUND_RIGHT_POINT		= 5,

	} EDIT_OBROUND_STATE;

public:
	OBroundPatternEditAction(PartCadDataPtr pPartCadData, GlViewPortPtr pViewPort);
	~OBroundPatternEditAction(void);

public: // implement IInteract interface.
	virtual BOOL LButtonDown(double& dPtX, double& dPtY);
	virtual BOOL MovePoint(double& dPtX, double& dPtY);
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);

public: // implement IAction interface.
	virtual BOOL DrawBeforeFinish() override;
	virtual int GetActionType() const override { return DRAW_OBROUND; }
	virtual int GetActionName() const override { return IDS_GEOMETRY_OBROUND; }
	virtual void ConvertKBInput(double& dVal1, double& dVal2) override;
	virtual void UpdateForUndoRedo() override; // if pattern is lost when grip-edit pattern, return to PatternSelectAction.

public: // implement IGripEditPatternAction interface.
	virtual void SetGripEditPattern(IPatternPtr pPattern);
	virtual void SetGripEditPt(double dPtX, double dPtY);

private:
	DRAW_OBROUND_STATE m_drawStatus;
	EDIT_OBROUND_STATE m_editState;

	// the data for creating OBround pattern.
	/*
	  CenterPointX;
	  CenterPointY;
	  Length;
	  Width;
	  Angle;
	*/
	double m_data[5];
};

END_CUTLEADER_NAMESPACE()

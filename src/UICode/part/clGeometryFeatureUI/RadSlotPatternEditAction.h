#pragma once

#include "PatternGripEditActionBase.h"

BEGIN_CUTLEADER_NAMESPACE()

// this class can create/edit the radius slot pattern.
class RadSlotPatternEditAction : public PatternGripEditActionBase
{
	typedef enum tagDrawRadSlotState
	{
		DRAW_RADSLOT_NOTSTART			= 0,
		DRAW_RADSLOT_FIRST_POINT		= 1,
		DRAW_RADSLOT_SECOND_POINT		= 2,
		DRAW_RADSLOT_THIRD_POINT		= 3,
		DRAW_RADSLOT_FOURTH_POINT		= 4,

	} DRAW_RADSLOT_STATE;

	typedef enum tagEditRadSlotState
	{
		EDIT_RADSLOT_NOTSTART           = 0,

		// use this to adjust the position.
		EDIT_RADSLOT_CENTER_POINT		= 1,

		// use this to adjust the start angle.
		EDIT_RADSLOT_START_POINT		= 2,

		// this can change the radius.
		EDIT_RADSLOT_MID_POINT			= 3,

		// use this to adjust the end angle.
		EDIT_RADSLOT_END_POINT			= 4,

		// point in the middle of inner arc, adjust the width.
		EDIT_RADSLOT_WIDTH_POINT		= 5,

	} EDIT_RADSLOT_STATE;

public:
	RadSlotPatternEditAction(PartCadDataPtr pPartCadData, GlViewPortPtr pViewPort);
	~RadSlotPatternEditAction(void);

public: // implement IInteract interface.
	virtual BOOL LButtonDown(double& dPtX, double& dPtY);
	virtual BOOL MovePoint(double& dPtX, double& dPtY);
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);

public: // implement IAction interface.
	virtual BOOL DrawBeforeFinish() override;
	virtual int GetActionType() const override { return DRAW_RADSLOT; }
	virtual int GetActionName() const override { return IDS_GEOMETRY_RADSLOT_TIP; }
	virtual void ConvertKBInput(double& dVal1, double& dVal2) override;
	virtual void UpdateForUndoRedo() override; // if pattern is lost when grip-edit pattern, return to PatternSelectAction.

public: // implement IGripEditPatternAction interface.
	virtual void SetGripEditPattern(IPatternPtr pPattern);
	virtual void SetGripEditPt(double dPtX, double dPtY);

private:
	void CheckArcDir(double& startAng, double& endAng, double& lastAng);

private:
	DRAW_RADSLOT_STATE m_drawStatus;
	EDIT_RADSLOT_STATE m_editState;

	// the data for creating radius slot pattern.
	/*
	  CenterPointX;
	  CenterPointY;
	  Radius;
	  Width;
	  StartAngle;
	  MovingAngle;
	*/
	double m_data[6];

	double m_dLastAng;
};

END_CUTLEADER_NAMESPACE()

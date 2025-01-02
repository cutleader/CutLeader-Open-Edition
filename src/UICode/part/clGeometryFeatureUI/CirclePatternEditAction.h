#pragma once

#include "PatternGripEditActionBase.h"

BEGIN_CUTLEADER_NAMESPACE()

// this class will create/edit circle pattern.
class CirclePatternEditAction : public PatternGripEditActionBase
{
	typedef enum tagDrawCircleState
	{
		DRAW_CIRCLE_NOTSTART	     = 0,
		DRAW_CIRCLE_CENTER_POINT	 = 1,
		DRAW_CIRCLE_END_POINT		 = 2,

	} DRAW_CIRCLE_STATE;

	typedef enum tagEditCircleState
	{
		EDIT_CIRCLE_NOTSTART         = 0,
		EDIT_CIRCLE_CENTER_POINT	 = 1,
		EDIT_CIRCLE_TOP_POINT		 = 2,
		EDIT_CIRCLE_LEFT_POINT		 = 3,
		EDIT_CIRCLE_BOTTOM_POINT     = 4,
		EDIT_CIRCLE_RIGHT_POINT		 = 5,

	} EDIT_CIRCLE_STATE;

public:
	CirclePatternEditAction(PartCadDataPtr pPartCadData, GlViewPortPtr pViewPort);
	~CirclePatternEditAction(void);

public: // implement IInteract interface.
	virtual BOOL LButtonDown(double& dPtX, double& dPtY) override;
	virtual BOOL MovePoint(double& dPtX, double& dPtY) override;
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) override;

public: // implement IAction interface.
	virtual BOOL DrawBeforeFinish() override;
	virtual int GetActionType() const override { return DRAW_ROUNDHOLE; }
	virtual int GetActionName() const override { return IDS_GEOMETRY_ROUNDHOLE_TIP; }
	virtual void UpdateForUndoRedo() override;

public: // implement IGripEditPatternAction interface.
	virtual void SetGripEditPattern(IPatternPtr pPattern) override;
	virtual void SetGripEditPt(double dPtX, double dPtY) override;

private:
	DRAW_CIRCLE_STATE m_drawState;
	EDIT_CIRCLE_STATE m_editState;

	// the data for creating circle.
	/* 
		CenterPointX 
		CenterPointY 
		Radius
	*/
	double m_data[3];
};

END_CUTLEADER_NAMESPACE()

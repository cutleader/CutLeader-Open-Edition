#pragma once

#include "CanvasGeometryEditAction.h"
#include "CanvasPatternGripEditActionBase.h"

DECLARE_CUTLEADER_CLASS(CanvasCirclePatternEditAction)

BEGIN_CUTLEADER_NAMESPACE()

class CanvasCirclePatternEditAction : public CanvasPatternGripEditActionBase
{
	enum DRAW_CIRCLE_STATE
	{
		DRAW_CIRCLE_NOTSTART		= 0,
		DRAW_CIRCLE_CENTER_POINT	= 1,
		DRAW_CIRCLE_END_POINT		= 2,
	};

	enum EDIT_CIRCLE_STATE
	{
		EDIT_CIRCLE_NOTSTART		= 0,
		EDIT_CIRCLE_CENTER_POINT	= 1,
		EDIT_CIRCLE_TOP_POINT		= 2,
		EDIT_CIRCLE_LEFT_POINT		= 3,
		EDIT_CIRCLE_BOTTOM_POINT	= 4,
		EDIT_CIRCLE_RIGHT_POINT		= 5,
	};

public:
	CanvasCirclePatternEditAction(PartPlacementPtr pPartPlacement, GlViewPortPtr pViewPort);
	~CanvasCirclePatternEditAction(void);

public:  // implement IInteract interface.
	virtual BOOL LButtonDown(double& dPtX, double& dPtY);
	virtual BOOL MovePoint(double& dPtX, double& dPtY);
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);

public: // implement IAction interface.
	virtual int GetActionType() const override { return EditCirclePatternOnCanvas; }
	virtual int GetActionName() const override { return IDS_GEOMETRY_ROUNDHOLE_TIP; }
	virtual BOOL DrawBeforeFinish() override;
	virtual void UpdateForUndoRedo() override;

public: // implement IGripEditPatternAction interface.
	virtual void SetGripEditPattern(IPatternPtr pPattern);
	virtual void SetGripEditPt(double dPtX, double dPtY);

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

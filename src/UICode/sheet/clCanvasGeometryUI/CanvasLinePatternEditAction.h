#pragma once

#include "CanvasGeometryEditAction.h"
#include "CanvasPatternGripEditActionBase.h"

DECLARE_CUTLEADER_CLASS(CanvasLinePatternEditAction)

BEGIN_CUTLEADER_NAMESPACE()

class CanvasLinePatternEditAction : public CanvasPatternGripEditActionBase
{
protected:
	enum DRAW_LINE_STATE
	{
		DRAW_LINE_NOTSTART		= 0,
		DRAW_LINE_START_POINT	= 1,
		DRAW_LINE_END_POINT		= 2,

	};
	enum EDIT_LINE_STATE
	{
		EDIT_LINE_NOTSTART		= 0,
		EDIT_LINE_CENTER_POINT	= 1,
		EDIT_LINE_START_POINT	= 2,
		EDIT_LINE_END_POINT		= 3,
	} ;

public:
	CanvasLinePatternEditAction(PartPlacementPtr pPartPlacement, GlViewPortPtr pViewPort);
	~CanvasLinePatternEditAction(void);

public:  // implement IInteract interface.
	virtual BOOL LButtonDown(double& dPtX, double& dPtY);
	virtual BOOL MovePoint(double& dPtX, double& dPtY);
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

public: // implement IAction interface.
	virtual int GetActionType() const override { return EditLinePatternOnCanvas; }
	virtual int GetActionName() const override { return IDS_GEOMETRY_LINE_TIP; }
	virtual BOOL DrawBeforeFinish() override;
	virtual void UpdateForUndoRedo() override;
	virtual SnapInputPtr GetSnapInput() const override;

public: // implement IGripEditPatternAction interface.
	virtual void SetGripEditPattern(IPatternPtr pPattern);
	virtual void SetGripEditPt(double dPtX, double dPtY);

protected:
	double m_data[4]; // the data for creating line.

	DRAW_LINE_STATE m_drawState;
	EDIT_LINE_STATE m_editState;
};

END_CUTLEADER_NAMESPACE()

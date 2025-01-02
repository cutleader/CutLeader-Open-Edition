#pragma once

#include "CanvasGeometryEditAction.h"
#include "CanvasPatternGripEditActionBase.h"

DECLARE_CUTLEADER_CLASS(CanvasArcPatternEditAction)

BEGIN_CUTLEADER_NAMESPACE()

class CanvasArcPatternEditAction : public CanvasPatternGripEditActionBase
{
	enum DRAW_ARC3P_STATE
	{
		DRAW_ARC3P_NOTSTART			= 0,
		DRAW_ARC3P_START_POINT		= 1,
		DRAW_ARC3P_MID_POINT		= 2,
		DRAW_ARC3P_END_POINT		= 3,
	};
	enum EDIT_ARC_STATE
	{
		EDIT_ARC_NOTSTART			= 0,
		EDIT_ARC_CENTER_POINT		= 1,
		EDIT_ARC_START_POINT		= 2,
		EDIT_ARC_MID_POINT			= 3,
		EDIT_ARC_END_POINT			= 4,
	};

public:
	CanvasArcPatternEditAction(PartPlacementPtr pPartPlacement, GlViewPortPtr pViewPort);
	~CanvasArcPatternEditAction(void);

public:  // implement IInteract interface.
	virtual BOOL LButtonDown(double& dPtX, double& dPtY);
	virtual BOOL MovePoint(double& dPtX, double& dPtY);
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

public: // implement IAction interface.
	virtual int GetActionType() const override { return EditArcPatternOnCanvas; }
	virtual int GetActionName() const override { return IDS_GEOMETRY_ARC_TIP; }
	virtual BOOL DrawBeforeFinish() override;
	virtual void UpdateForUndoRedo() override;

public: // implement IGripEditPatternAction interface.
	virtual void SetGripEditPattern(IPatternPtr pPattern);
	virtual void SetGripEditPt(double dPtX, double dPtY);

private:
	void ChangeMidPoint(double midPtX, double midPtY);

protected:
	DRAW_ARC3P_STATE m_iStatus;
	EDIT_ARC_STATE m_editState;

	// the data for creating arc.
	/*
		StartPointX
		StartPointY
		MiddlePointX
		MiddlePointY
		EndPointX
		EndPointY
	*/
	double m_data[6];
};

END_CUTLEADER_NAMESPACE()

#pragma once

#include "CanvasGeometryEditAction.h"
#include "CanvasPatternGripEditActionBase.h"

DECLARE_CUTLEADER_CLASS(CanvasRectPatternEditAction)

BEGIN_CUTLEADER_NAMESPACE()

class CanvasRectPatternEditAction : public CanvasPatternGripEditActionBase
{
	enum DRAW_RECTHOLE_STATE
	{
		DRAW_RECTHOLE_NOTSTART			= 0,
		DRAW_RECTHOLE_START_POINT		= 1,
		DRAW_RECTHOLE_END_POINT			= 2,
	};
	enum EDIT_RECTHOLE_STATE
	{
		EDIT_RECTHOLE_NOTSTART			= 0,
		EDIT_RECTHOLE_CENTER_POINT		= 1,
		EDIT_RECTHOLE_START_POINT		= 2,
		EDIT_RECTHOLE_END_POINT			= 3,
		EDIT_RECTHOLE_THIRD_POINT		= 4,
		EDIT_RECTHOLE_FOUR_POINT		= 5,
	};

public:
	CanvasRectPatternEditAction(PartPlacementPtr pPartPlacement, GlViewPortPtr pViewPort);
	~CanvasRectPatternEditAction(void);

public:  // implement IInteract interface.
	virtual BOOL LButtonDown(double& dPtX, double& dPtY);
	virtual BOOL MovePoint(double& dPtX, double& dPtY);
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

public: // implement IAction interface.
	virtual int GetActionType() const override { return EditRectPatternOnCanvas; }
	virtual int GetActionName() const override { return IDS_GEOMETRY_RECTHOLE_TIP; }
	virtual BOOL DrawBeforeFinish() override;
	virtual void UpdateForUndoRedo() override;

public: // implement IGripEditPatternAction interface.
	virtual void SetGripEditPattern(IPatternPtr pPattern);
	virtual void SetGripEditPt(double dPtX, double dPtY);

private:
	void ScalePattern(double PointX, double PointY);

protected:
	/*
		double StartPointX;
		double StartPointY;
		double EndPointX;
		double EndPointY;
		double Angle;
	*/
	double m_data[5];

	DRAW_RECTHOLE_STATE m_drawState;
	EDIT_RECTHOLE_STATE m_editState;
};

END_CUTLEADER_NAMESPACE()

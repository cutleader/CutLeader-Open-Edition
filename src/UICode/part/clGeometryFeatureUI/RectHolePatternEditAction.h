#pragma once

#include "PatternGripEditActionBase.h"

BEGIN_CUTLEADER_NAMESPACE()

// warning: we have an assumption here! That is, start point is the left top point, end point is right bottom point.
class RectHolePatternEditAction : public PatternGripEditActionBase
{
	typedef enum tagDrawRectHoleState
	{
		DRAW_RECTHOLE_NOTSTART				= 0,
		DRAW_RECTHOLE_START_POINT			= 1,
		DRAW_RECTHOLE_END_POINT				= 2,

	} DRAW_RECTHOLE_STATE;

	typedef enum tagEditRectHoleState
	{
		EDIT_RECTHOLE_NOTSTART				= 0,
		EDIT_RECTHOLE_CENTER_POINT			= 1,
		EDIT_RECTHOLE_START_POINT			= 2,
		EDIT_RECTHOLE_END_POINT				= 3,
		EDIT_RECTHOLE_THIRD_POINT			= 4,
		EDIT_RECTHOLE_FOUR_POINT			= 5,

	} EDIT_RECTHOLE_STATE;

public:
	RectHolePatternEditAction(PartCadDataPtr pPartCadData, GlViewPortPtr pViewPort);
	~RectHolePatternEditAction(void);

public: // implement IInteract interface.
	virtual BOOL LButtonDown(double& dPtX, double& dPtY);
	virtual BOOL MovePoint(double& dPtX, double& dPtY);
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);

public: // implement IAction interface.
	virtual BOOL DrawBeforeFinish() override;
	virtual int GetActionType() const override { return DRAW_RECT_HOLE; }
	virtual int GetActionName() const override { return IDS_GEOMETRY_RECTHOLE_TIP; }
	virtual void ConvertKBInput(double& dVal1, double& dVal2);
	virtual void UpdateForUndoRedo(); // if pattern is lost when grip-edit pattern, return to PatternSelectAction.

public: // implement IGripEditPatternAction interface.
	virtual void SetGripEditPattern(IPatternPtr pPattern);
	virtual void SetGripEditPt(double dPtX, double dPtY);

private:
	void ScalePattern(double PointX, double PointY);

private:
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

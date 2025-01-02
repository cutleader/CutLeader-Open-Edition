#pragma once

#include "PatternGripEditActionBase.h"

BEGIN_CUTLEADER_NAMESPACE()

// this class can create/edit the ellipse pattern.
// notes: the LongRadius is not really "long", so does ShortRadius. Actually they are determined by the oder of drawing.
class EllipsePatternEditAction : public PatternGripEditActionBase
{
	typedef enum tagDrawEllipseState
	{
		DRAW_ELLIPSE_NOTSTART		= 0,
		DRAW_ELLIPSE_FIRST_POINT	= 1,
		DRAW_ELLIPSE_SECOND_POINT	= 2,
		DRAW_ELLIPSE_THIRD_POINT	= 3,

	} DRAW_ELLIPSE_STATE;

	// notes: the "left","top","right","bottom" are not the real...
	typedef enum tagEditEllipseState
	{
		EDIT_ELLIPSE_NOTSTART		= 0,
		EDIT_ELLIPSE_CENTER_POINT	= 1,
		EDIT_ELLIPSE_TOP_POINT		= 2,
		EDIT_ELLIPSE_LEFT_POINT		= 3,
		EDIT_ELLIPSE_BOTTOM_POINT	= 4,
		EDIT_ELLIPSE_RIGHT_POINT	= 5,

	} EDIT_ELLIPSE_STATE;

public:
	EllipsePatternEditAction(PartCadDataPtr pPartCadData, GlViewPortPtr pViewPort);
	~EllipsePatternEditAction(void);

public: // implement IInteract interface.
	virtual BOOL LButtonDown(double& dPtX, double& dPtY);
	virtual BOOL MovePoint(double& dPtX, double& dPtY);
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);

public: // implement IAction interface.
	virtual BOOL DrawBeforeFinish() override;
	virtual int GetActionType() const override { return DRAW_ELLIPSE; }
	virtual int GetActionName() const override { return IDS_GEOMETRY_ELLIPSE; }
	virtual void ConvertKBInput(double& dVal1, double& dVal2) override;
	virtual void UpdateForUndoRedo() override; // if pattern is lost when grip-edit pattern, return to PatternSelectAction.

public: // implement IGripEditPatternAction interface.
	virtual void SetGripEditPattern(IPatternPtr pPattern);
	virtual void SetGripEditPt(double dPtX, double dPtY);

private:
	DRAW_ELLIPSE_STATE m_drawStatus;
	EDIT_ELLIPSE_STATE m_editState;

	// the data for creating ellipse pattern.
	/* 
		CenterPointX 
		CenterPointY 
		LongRadius 
		ShortRadius 
		Angle
	*/
	double m_data[5];
};

END_CUTLEADER_NAMESPACE()

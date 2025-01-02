#pragma once

#include "PatternGripEditActionBase.h"

BEGIN_CUTLEADER_NAMESPACE()

// this class can create/edit the triangle hole pattern.
class TriangleHolePatternEditAction : public PatternGripEditActionBase
{
	typedef enum tagDrawTringleState
	{
		DRAW_TRIANGLE_NOTSTART		= 0,
		DRAW_TRIANGLE_FIRST_POINT	= 1,
		DRAW_TRIANGLE_SECOND_POINT	= 2,
		DRAW_TRIANGLE_THIRD_POINT	= 3,

	} DRAW_TRIANGLE_STATE;

	typedef enum tagEditTringleState
	{
		EDIT_TRIANGLE_NOTSTART		= 0,
		EDIT_TRIANGLE_BASE_POINT	= 1,
		EDIT_TRIANGLE_TOP_POINT		= 2,
		EDIT_TRIANGLE_LEFT_POINT	= 3,
		EDIT_TRIANGLE_RIGHT_POINT	= 4,

	} EDIT_TRIANGLE_STATE;

public:
	TriangleHolePatternEditAction(PartCadDataPtr pPartCadData, GlViewPortPtr pViewPort);
	~TriangleHolePatternEditAction(void);

public: // implement IInteract interface.
	virtual BOOL LButtonDown(double& dPtX, double& dPtY);
	virtual BOOL MovePoint(double& dPtX, double& dPtY);
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);

public: // implement IAction interface.
	virtual BOOL DrawBeforeFinish() override;
	virtual int GetActionType() const override { return DRAW_TR_HOLE; }
	virtual int GetActionName() const override { return IDS_GEOMETRY_TRHOLE_TIP; }
	virtual void ConvertKBInput(double& dVal1, double& dVal2) override;
	virtual void UpdateForUndoRedo() override; // if pattern is lost when grip-edit pattern, return to PatternSelectAction.

public: // implement IGripEditPatternAction interface.
	virtual void SetGripEditPattern(IPatternPtr pPattern);
	virtual void SetGripEditPt(double dPtX, double dPtY);

private:
	DRAW_TRIANGLE_STATE m_drawState;
	EDIT_TRIANGLE_STATE m_editState;

	// the data for creating triangle pattern.
	/*
	  CenterPointX;
	  CenterPointY;
	  EndPointX;
	  EndPointY;
	  TopPointX;
	  TopPointY;
	  Angle;
	*/
	double m_data[7];
};

END_CUTLEADER_NAMESPACE()

#pragma once

#include "ArrayPatternEditActionBase.h"

BEGIN_CUTLEADER_NAMESPACE()

// this class will create or edit Pattern arc.
// notes:
//   1) in this action, we do not support keyboard input value.
class ArcArrayPatternEditAction : public ArrayPatternEditActionBase
{
	typedef enum tagEditGridPatternArcState
	{
		EDIT_PATTERNARC_NOTSTART			= 0,
		EDIT_PATTERNARC_BASEPOINT			= 1,
		EDIT_PATTERNARC_STARTANGLE			= 2,
		EDIT_PATTERNARC_SPACING				= 3,
		EDIT_PATTERNARC_REPEATCOUNT			= 4,

	} EDIT_PATTERNARC_STATE;

public:
	ArcArrayPatternEditAction(PartCadDataPtr pPartCadData, GlViewPortPtr pViewPort);
	~ArcArrayPatternEditAction(void);

public: // implement IInteract interface.
	virtual BOOL LButtonDown(double& dPtX, double& dPtY);
	virtual BOOL MovePoint(double& dPtX, double& dPtY);
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);

public: // implement IAction interface.
	virtual BOOL DrawBeforeFinish() override;
	virtual int GetActionType() const override { return DRAW_PATTERNARC; }
	virtual int GetActionName() const override { return IDS_GEOMETRY_PATARC_TIP; }
	virtual void UpdateForUndoRedo() override;

public:	// implement IGripEditPatternAction interface.
	virtual void SetGripEditPattern(IPatternPtr pPattern);
	virtual void SetGripEditPt(double dPtX, double dPtY);

private:
	void CalcRadiusAngle(double dPtX, double dPtY);
	void CalcSpaceAngle(double dPtX, double dPtY);
	void CalcRepeatCount(double dPtX, double dPtY);

private:
	EDIT_PATTERNARC_STATE m_editState;

	/*
	 	0 CenterPointX;
	 	1 CenterPointY;
	    2 StartAngle;
	    3 Radius;
	    4 MoveAngle;
	 	5 SpaceAngle;
	 	6 RepeatCount;
	*/
	double m_data[7];
};

END_CUTLEADER_NAMESPACE()

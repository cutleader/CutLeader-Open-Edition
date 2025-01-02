#pragma once

#include "PatternEditActionBase.h"

BEGIN_CUTLEADER_NAMESPACE()

// this class can create/edit the 3-points arc.
// notes: 
//   1) we do not need to care about Grip-edit in this class, ArcPatternEditAction will care about this function.
class ArcPattern3PtEditAction : public PatternEditActionBase
{
	typedef enum tagDrawArc3PState
	{
		DRAW_ARC3P_NOTSTART        = 0,
		DRAW_ARC3P_START_POINT	   = 1,
		DRAW_ARC3P_MID_POINT       = 2,
		DRAW_ARC3P_END_POINT       = 3,
	} DRAW_ARC3P_STATE;

public:
	ArcPattern3PtEditAction(PartCadDataPtr pPartCadData, GlViewPortPtr pViewPort);
	~ArcPattern3PtEditAction(void);

public: // implement IInteract interface.
	virtual BOOL LButtonDown(double& dPtX, double& dPtY);
	virtual BOOL MovePoint(double& dPtX, double& dPtY);
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);

public: // implement IAction interface.
	virtual BOOL DrawBeforeFinish() override;

	virtual int GetActionType() const override { return DRAW_ARC_3PT; }
	virtual int GetActionName() const override { return IDS_GEOM_DRAW_3PtArc; }

private:
	// the action state
	DRAW_ARC3P_STATE m_iStatus;

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

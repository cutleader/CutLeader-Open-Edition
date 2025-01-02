#pragma once

#include "PatternEditActionBase.h"

BEGIN_CUTLEADER_NAMESPACE()

// create and edit round corner lines in this class.
class RoundCornerLineArcsDrawAction : public PatternEditActionBase
{
	typedef enum tagDrawRcRectState
	{
		DRAW_RCLINE_NOTSTART		= 0,
		DRAW_RCLINE_FIRST_POINT		= 1,
		DRAW_RCLINE_SECOND_POINT	= 2,
		DRAW_RCLINE_THIRD_POINT		= 3,

	} DRAW_RCLINE_STATE;

public:
	RoundCornerLineArcsDrawAction(PartCadDataPtr pPartCadData, GlViewPortPtr pViewPort);
	~RoundCornerLineArcsDrawAction(void);

public: // implement IInteract interface.
	virtual BOOL LButtonDown(double& dPtX, double& dPtY);
	virtual BOOL MovePoint(double& dPtX, double& dPtY);
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);

public: // implement IAction interface.
	virtual BOOL DrawBeforeFinish() override;
	virtual int GetActionType() const override { return DRAW_RC_LINES; }
	virtual int GetActionName() const override { return IDS_GEOM_DRAW_RCLINEARC_TIP; }
	virtual void ConvertKBInput(double& dVal1, double& dVal2) override;

private:
	// here we should create 4 lines and 4 arcs patterns.
	void CreateRCLinePat();

private:
	/*
		double CenterPointX;
		double CenterPointY;
		double Length;
		double Width;
		double Radius;
		double BaseAngle;
	*/
	double m_data[6];

	DRAW_RCLINE_STATE m_drawState;
};

END_CUTLEADER_NAMESPACE()

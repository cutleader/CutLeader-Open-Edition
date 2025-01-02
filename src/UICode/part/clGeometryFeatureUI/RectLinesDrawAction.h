#pragma once

#include "PatternEditActionBase.h"

BEGIN_CUTLEADER_NAMESPACE()

// notes:
//  1) use four line patterns to store Rectangle Line.
//  2) this class do not need to support Grid edit and rotate..., all these functions are finished by LinePatternEditAction.

// Important! We only need to keep the relative position. data[0,1] is start pt, and data[2,3] is end pt.

//      |
//	   /|\		
//		|	data[3]	   ________________
//		|		      | 			   |
//		|			  |    		       |
//		|	data[1]	  |________________|
//		|       
//		|          data[0]       data[2]
//		|________________________________\
//										 /	
class RectLinesDrawAction : public PatternEditActionBase
{
	typedef enum tagDrawRectLineState
	{
		DRAW_RECTLINE_NOTSTART				= 0,
		DRAW_RECTLINE_START_POINT			= 1,
		DRAW_RECTLINE_END_POINT				= 2,

	} DRAW_RECTLINE_STATE;

public:
	RectLinesDrawAction(PartCadDataPtr pPartCadData, GlViewPortPtr pViewPort);
	~RectLinesDrawAction(void);

public: // implement IInteract interface.
	virtual BOOL LButtonDown(double& dPtX, double& dPtY);
	virtual BOOL MovePoint(double& dPtX, double& dPtY);
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

public: // implement IAction interface.
	virtual BOOL DrawBeforeFinish() override;
	virtual int GetActionType() const override { return DRAW_RECTLINES; }
	virtual int GetActionName() const override { return IDS_GEOM_DRAW_RectLine_TIP; }

private:
	/*
		double StartPointX;
		double StartPointY;
		double EndPointX;
		double EndPointY;
	*/
	double m_data[4];

	DRAW_RECTLINE_STATE m_drawState;
};

END_CUTLEADER_NAMESPACE()

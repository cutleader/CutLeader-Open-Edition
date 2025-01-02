#pragma once

#include "Point2D.h"
#include "PatternGripEditActionBase.h"

BEGIN_CUTLEADER_NAMESPACE()

// this class works as the base class for all array-edit class.
// notes:
//   1) we do not use "PatternSelectAction" to select pattern.
//   2) even though user used "PatternSelectAction" select some pattern, we will re-select pattern in this action.
class ClGeometryFeatureUI_Export ArrayPatternEditActionBase : public PatternGripEditActionBase
{
protected:
	typedef enum tagDrawArrayState
	{
		DRAW_ARRAY_NOTSTART			= 0,
		DRAW_ARRAY_FIRST_POINT		= 1,
		DRAW_ARRAY_SECOND_POINT		= 2,
		DRAW_ARRAY_THIRD_POINT		= 3,

	} DRAW_ARRAY_PATTERN_STATE;

public:
	ArrayPatternEditActionBase(void);
	~ArrayPatternEditActionBase(void);

public: // implement IInteract interface.
	// implement some common operations for sub classes.
	// notes:
	//   1) this function will:
	//      a) check the active pattern is the standard pattern.
	virtual BOOL LButtonDown(double& dPtX, double& dPtY);

public: // implement IAction interface.
	// implement some common operations for sub classes.
	// notes:
	//   1) this function will:
	//      a) draw the cursor for different state.
	//      b) draw the active pattern during select the control pattern.
	virtual BOOL DrawBeforeFinish();

protected:
	/************************************************************************/
	// used for create array pattern.

	// update this when mouse move.
	IPatternPtr m_pActivePat;

	// update this when mouse click.
	IPatternPtr m_pControlPat;
	/************************************************************************/

	DRAW_ARRAY_PATTERN_STATE m_drawState;
};

END_CUTLEADER_NAMESPACE()

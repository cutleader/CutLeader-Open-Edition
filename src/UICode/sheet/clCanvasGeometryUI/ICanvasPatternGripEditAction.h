#pragma once

#include "ICanvasGeometryEditAction.h"

DECLARE_CUTLEADER_CLASS(IPattern)

BEGIN_CUTLEADER_NAMESPACE()

// this interface is used for Grip-editing pattern.
class ICanvasPatternGripEditAction : virtual public ICanvasGeometryEditAction
{
public:
    virtual ~ICanvasPatternGripEditAction() {}

public:
	// set the pattern for Grip-edit.
	// notes:
	//   1) in this function, we should init "m_data" variable using "pPattern".
	virtual void SetGripEditPattern(IPatternPtr pPattern) = 0;

	// set the toggle point of Grip-edit.
	// notes:
	//   1) in this function, "m_editState" will be init.
	virtual void SetGripEditPt(double dPtX, double dPtY) = 0;
};

END_CUTLEADER_NAMESPACE()

#pragma once

#include "IPatternGripEditAction.h"
#include "PatternEditActionBase.h"

BEGIN_CUTLEADER_NAMESPACE()

// this interface is used for Grip-editing pattern.
class PatternGripEditActionBase : virtual public IPatternGripEditAction, public PatternEditActionBase
{
public:
	PatternGripEditActionBase();
	virtual ~PatternGripEditActionBase();

protected:
	// the pattern which will be grip-edit.
	IPatternPtr m_pGripEditPat;
};

END_CUTLEADER_NAMESPACE()

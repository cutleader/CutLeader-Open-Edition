#pragma once

#include "IPatternEditAction.h"

BEGIN_CUTLEADER_NAMESPACE()

// the common interface of modifying pattern. e.g. move, rotate, flip,...
// notes: 
//   1) before modify pattern, we should select pattern.
class IPatternTransformAction : virtual public IPatternEditAction
{
protected:
	typedef enum tagModifyState
	{
		TRANSFORM_PATTERN_NOTSTART		= 0,
		TRANSFORM_PATTERN_BASE_POINT	= 1,
		TRANSFORM_PATTERN_SECOND_POINT	= 2,
	} TRANSFORM_PATTERN_STATE;

public:
	// we will set the Matrix to modify the pattern.
	virtual void SetMatrixValue() = 0;

	// the modify action name
	virtual CString GetActionName() = 0;

	// get the message ID based on the "m_actionState".
	// e.g., "PatternMoveAction" and "PatternRotateAction" have different message in Output bar.
	virtual int GetMsgID() = 0;
};

END_CUTLEADER_NAMESPACE()

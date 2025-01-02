#pragma once

#include "clPartLayoutUICommon.h"
#include "IAction.h"

BEGIN_CUTLEADER_NAMESPACE()

class IPartPlacementTransformAction : virtual public IAction
{
protected:
	typedef enum tagModifyState
	{
		TRANSFORM_PARTPMT_NOTSTART				= 0,
		TRANSFORM_PARTPMT_BASE_PT_SELECTED		= 1,
		TRANSFORM_PARTPMT_SECOND_PT_SELECTED	= 2,
	} TRANSFORM_PARTPMT_STATE;

public:
    virtual ~IPartPlacementTransformAction() {}

public:
	// we will set the Matrix to modify the pattern.
	virtual void SetMatrixValue() = 0;

	// the modify action name
	virtual CString GetActionName() = 0;

	// get the message ID based on the "m_actionState".
	// e.g., "PartPlacementMoveAction" and "PartPlacementRotateAction" have different message in Output bar.
	virtual int GetMsgID() = 0;
};

END_CUTLEADER_NAMESPACE()

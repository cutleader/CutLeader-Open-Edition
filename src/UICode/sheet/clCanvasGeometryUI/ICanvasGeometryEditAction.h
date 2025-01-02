#pragma once

#include "clCanvasGeometryUICommon.h"
#include "IAction.h"

BEGIN_CUTLEADER_NAMESPACE()

// this interface is the common interface for all pattern-related actions.
class ICanvasGeometryEditAction : virtual public IAction
{
public:
    virtual ~ICanvasGeometryEditAction() {}
};

END_CUTLEADER_NAMESPACE()

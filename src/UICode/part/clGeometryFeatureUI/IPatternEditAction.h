#pragma once

#include "clGeometryFeatureUICommon.h"
#include "IAction.h"

BEGIN_CUTLEADER_NAMESPACE()

// this interface is the common interface for all pattern-related actions.
class IPatternEditAction : virtual public IAction
{
public:
    virtual ~IPatternEditAction() {}
};

END_CUTLEADER_NAMESPACE()

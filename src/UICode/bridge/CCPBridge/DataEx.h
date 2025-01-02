#pragma once

#include "ccpBridgeCommon.h"
#include "Point2DEx.h"

BEGIN_CUTLEADER_NAMESPACE()

[InterfaceType(ComInterfaceType::InterfaceIsDual)]
[ComVisible(true)]
public interface class DataEx
{
public:
	virtual long long GetID();
	virtual void SetID(long long iID);
};

END_CUTLEADER_NAMESPACE()
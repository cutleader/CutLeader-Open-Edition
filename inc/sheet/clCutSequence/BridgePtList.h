#pragma once

#include "clCutSequenceCommon.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(BridgePt)

BEGIN_CUTLEADER_NAMESPACE()

class BridgePtList : public std::vector<BridgePtPtr>
{
public:
	BridgePtList(void);
	~BridgePtList(void);
};

END_CUTLEADER_NAMESPACE()

#pragma once

#include "clCutFeatureCommon.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(IPattern)
DECLARE_CUTLEADER_CLASS(LoopToolData)
DECLARE_CUTLEADER_CLASS(LoopToolDataList)

BEGIN_CUTLEADER_NAMESPACE()

class ClCutFeature_Export LoopToolDataList : public std::vector<LoopToolDataPtr>
{
public:
	LoopToolDataList(void);
	~LoopToolDataList(void);

public:
    LoopToolDataPtr GetLoopTool_by_loopID(LONGLONG iLoopID) const;
    LoopToolDataPtr GetItemByID(LONGLONG iID) const;
    void DeleteItemByID(LONGLONG iID);
};

END_CUTLEADER_NAMESPACE()

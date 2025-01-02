#pragma once

#include "clCutFeatureUICommon.h"
#include "Point2D.h"

DECLARE_CUTLEADER_CLASS(IPatternLoop)
DECLARE_CUTLEADER_CLASS(LoopToolDataList)
DECLARE_CUTLEADER_CLASS(PatternLoopList)
DECLARE_CUTLEADER_CLASS(ToolAssignConfigItem)
DECLARE_CUTLEADER_CLASS(LoopTopologyItemList)

BEGIN_CUTLEADER_NAMESPACE()

class ClCutFeatureUI_Export CutFeatureHelper
{
public:
	// 根据分配规则给零件的每个轮廓分配刀具。这里把pPatternLoops看成一个零件。
	static LoopToolDataListPtr AssignLoopTools4Part(const PatternLoopList* pPatternLoops, const LoopTopologyItemList* pLoopTopologyItems,
		const ToolAssignConfigItem* pToolAssignConfigItem);
};

END_CUTLEADER_NAMESPACE()

#pragma once

#include "clPartLayoutCommon.h"
#include "Point2D.h"
#include "PatternPosition.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(LoopStartCutData)
DECLARE_CUTLEADER_CLASS(LoopInstance)
DECLARE_CUTLEADER_CLASS(LoopInstanceList)
DECLARE_CUTLEADER_CLASS(PatternInstance)
DECLARE_CUTLEADER_CLASS(PatternInstanceList)

BEGIN_CUTLEADER_NAMESPACE()

// “图形实例”管理器。
class ClPartLayout_Export PatternInstanceManager
{
public:
	// 从回路实例得到图形实例，这些图形实例遵循切割方向，注意这里只是这些“图形实例”的排序遵循切割方向，每个“图形实例”自身的方向则不一定遵循切割方向。
	// notes: 
	//   1) sometimes the "start" pattern instance also be the last cut pattern instance. this function cannot cover this case.
	static PatternInstanceListPtr GetSortPatternInstanceList(LoopInstancePtr pLoopInstance, const LoopStartCutData* pLoopStartCutData);

	// 从回路实例得到图形实例。
	static PatternInstanceListPtr GetPatternInstanceList(LoopInstancePtr pLoopInstance);
	static PatternInstanceListPtr GetPatternInstanceList(LoopInstanceListPtr pLoopInstList);

	// 得到点“pt”在“图形实例”(pLoopInstance->iLoopNodeIndex)上的“PatternPosition”。
	static PatternPosition GetPatternPosition(LoopInstancePtr pLoopInstance, int iLoopNodeIndex, const Point2D& pt);
};

END_CUTLEADER_NAMESPACE()

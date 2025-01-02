#pragma once

#include "clPartCommon.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(PartTopologyItem)
DECLARE_CUTLEADER_CLASS(PatternLoopList)
DECLARE_CUTLEADER_CLASS(LoopTopologyItemList)

BEGIN_CUTLEADER_NAMESPACE()

// 零件拓扑项集合。
class ClPart_Export PartTopologyItemList : public std::vector<PartTopologyItemPtr>
{
public:
	PartTopologyItemList(void);
	~PartTopologyItemList(void);

public:
	// 递归得到所有零件。
	// 注：
	//   1) 一个集合元素代表一个零件。
	void GetAllParts(std::vector<std::pair<PatternLoopListPtr, LoopTopologyItemListPtr>>& partList);
};

END_CUTLEADER_NAMESPACE()

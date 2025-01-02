#pragma once

#include "clPartCommon.h"
#include "ClData.h"

DECLARE_CUTLEADER_CLASS(IPatternLoop)
DECLARE_CUTLEADER_CLASS(PatternLoopList)
DECLARE_CUTLEADER_CLASS(PartTopologyItemList)
DECLARE_CUTLEADER_CLASS(LoopTopologyItemList)

BEGIN_CUTLEADER_NAMESPACE()

// 零件拓扑项。
class ClPart_Export PartTopologyItem : public ClData
{
public:
	PartTopologyItem(void);
	~PartTopologyItem(void);

public: // get/set functions.
	void SetPatternLoops(PatternLoopListPtr pPatternLoops) { m_pPatternLoops = pPatternLoops; }
	PatternLoopListPtr GetPatternLoops() const { return m_pPatternLoops; }

	void SetLoopTopologyItems(LoopTopologyItemListPtr pLoopTopologyItems) { m_pLoopTopologyItems = pLoopTopologyItems; }
	LoopTopologyItemListPtr GetLoopTopologyItems() const { return m_pLoopTopologyItems; }

	void SetSubItems(PartTopologyItemListPtr pSubItems) { m_pSubItems = pSubItems; }
	PartTopologyItemListPtr GetSubItems() const { return m_pSubItems; }

private:
	// 零件的几何轮廓。
	PatternLoopListPtr m_pPatternLoops;

	// 零件几何轮廓之间的拓扑关系。
	LoopTopologyItemListPtr m_pLoopTopologyItems;

	// 该零件包含的“拓扑项”。
	PartTopologyItemListPtr m_pSubItems;
};

END_CUTLEADER_NAMESPACE()

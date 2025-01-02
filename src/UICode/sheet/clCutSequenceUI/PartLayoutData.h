#pragma once

#include "clCutSequenceUICommon.h"

DECLARE_CUTLEADER_CLASS(PartPlacementList)
DECLARE_CUTLEADER_CLASS(PartInstanceList)
DECLARE_CUTLEADER_CLASS(PartLayoutData)

BEGIN_CUTLEADER_NAMESPACE()

// 零件布局数据。
class ClCutSequenceUI_Export PartLayoutData
{
public:
	PartLayoutData(PartPlacementListPtr pPartPlacements, PartInstanceListPtr pPartInstances);
	~PartLayoutData(void);

public:
	PartPlacementListPtr GetPartPlacements() const { return m_pPartPmtList; }
	PartInstanceListPtr GetPartInstances() const { return m_pPartInstList; }

private:
	PartPlacementListPtr m_pPartPmtList;
	PartInstanceListPtr m_pPartInstList;
};

END_CUTLEADER_NAMESPACE()

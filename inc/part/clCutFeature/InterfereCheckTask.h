#pragma once

#include "clCutFeatureCommon.h"
#include "TaskEx.h"

DECLARE_CUTLEADER_CLASS(LoopCutFeatureList)
DECLARE_CUTLEADER_CLASS(InterfereCheckTask)
DECLARE_CUTLEADER_CLASS(LoopTopologyItemList)

BEGIN_CUTLEADER_NAMESPACE()

class ClCutFeature_Export InterfereCheckTask : public TaskEx
{
public:
	InterfereCheckTask(LoopCutFeatureListPtr pLoopFeatureList, const LoopTopologyItemListPtr pLoopTopologyItems);
	~InterfereCheckTask(void);

public: // inherit from class TaskEx.
	virtual void Run();

private:
	LoopCutFeatureListPtr m_pLoopFeatureList;
	const LoopTopologyItemListPtr m_pLoopTopologyItems;
};

END_CUTLEADER_NAMESPACE()

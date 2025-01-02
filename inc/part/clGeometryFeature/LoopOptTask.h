#pragma once

#include "clGeometryFeatureCommon.h"
#include "TaskEx.h"

DECLARE_CUTLEADER_CLASS(PatternList)
DECLARE_CUTLEADER_CLASS(PatternLoopList)
DECLARE_CUTLEADER_CLASS(LoopOptTask)

BEGIN_CUTLEADER_NAMESPACE()

class ClGeometryFeature_Export LoopOptTask : public TaskEx
{
public:
	LoopOptTask(PatternListPtr pPatList, PatternLoopListPtr pPatternLoopList, double dAllowOffset);
	~LoopOptTask(void);

public: // inherit from class TaskEx.
	virtual void Run();

private:
	// the patterns which will be optimized.
	PatternListPtr m_pPatList;

	// the loops which will be optimized.
	PatternLoopListPtr m_pPatternLoopList;

	// 优化允许的偏移。
	double m_dAllowOffset;
};

END_CUTLEADER_NAMESPACE()

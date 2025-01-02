#pragma once

#include "clSheetCommon.h"
#include "TaskEx.h"

DECLARE_CUTLEADER_CLASS(Sheet)
DECLARE_CUTLEADER_CLASS(AutoSequenceTask)

BEGIN_CUTLEADER_NAMESPACE()

class ClSheet_Export AutoSequenceTask : public TaskEx
{
public:
	AutoSequenceTask(SheetPtr pSheet, bool bResetSequenceData);
	~AutoSequenceTask(void);

public: // inherit from class TaskEx.
	virtual void Run();

public:
    const std::set<LONGLONG>& GetPatternLoops_failedToOffset() const { return m_patternLoops_failedToOffset; }

private:
	// the sheet which will be processed in this task.
	SheetPtr m_pSheet;

	// 是不是要重置工序数据。如果是的话，会先清除掉已有的工序。
	bool m_bResetSequenceData;

    // 此处记录偏移失败的几何轮廓。
    std::set<LONGLONG> m_patternLoops_failedToOffset;
};

END_CUTLEADER_NAMESPACE()

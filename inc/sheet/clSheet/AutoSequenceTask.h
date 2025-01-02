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

	// �ǲ���Ҫ���ù������ݡ�����ǵĻ���������������еĹ���
	bool m_bResetSequenceData;

    // �˴���¼ƫ��ʧ�ܵļ���������
    std::set<LONGLONG> m_patternLoops_failedToOffset;
};

END_CUTLEADER_NAMESPACE()

#pragma once

#include "clDataImportExportCommon.h"
#include "TaskEx.h"

DECLARE_CUTLEADER_CLASS(RecoveryTask)

BEGIN_CUTLEADER_NAMESPACE()

// 数据库恢复任务。
class ClDataImportExport_Export RecoveryTask : public TaskEx
{
public:
	RecoveryTask(CString strBkDBPath);
	~RecoveryTask(void);

public: // inherit from class TaskEx.
	virtual void Run();

private:
	// the backup database files.
	CString m_strBkDBPath;
};

END_CUTLEADER_NAMESPACE()

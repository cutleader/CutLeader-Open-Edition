#pragma once

#include "clDataImportExportCommon.h"
#include "TaskEx.h"

DECLARE_CUTLEADER_CLASS(BackupTask)

BEGIN_CUTLEADER_NAMESPACE()

// 数据库备份任务。
class ClDataImportExport_Export BackupTask : public TaskEx
{
public:
	BackupTask(BOOL bBackupDB, BOOL bBackupLog, CString strBackupLoc);
	~BackupTask(void);

public: // inherit from class TaskEx.
	virtual void Run();

private:
	// backup option.
	// notes:
	//   1) at lease one item is TRUE.
	BOOL m_bBackupDB;
	BOOL m_bBackupLog;

	// the backup location.
	CString m_strBackupLoc;
};

END_CUTLEADER_NAMESPACE()

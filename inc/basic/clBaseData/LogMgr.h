#pragma once

#include "clBaseDataCommon.h"

DECLARE_CUTLEADER_CLASS(LogMessage)
DECLARE_CUTLEADER_CLASS(LogMessageList)

BEGIN_CUTLEADER_NAMESPACE()

#define LOG_MSG_SEPERATOR _T("####")

// manage the log file.
class ClBaseData_Export LogMgr
{
public:
	/************************************************************************/
	// about the task log.

	// get the log file size(M).
	static int GetLogFileSize();

	// write the log message into the log file.
	static void WriteLog(LogMessagePtr pLogMsg);
	static void WriteLog(LogMessageListPtr pLogMsgList);

	// load all log message from the log file.
	static LogMessageListPtr ReadAllLogMsg();

	// empty log file.
	static void EmptyLogFile();
	/************************************************************************/


	/************************************************************************/
	// about the backup date.

	// get the db backup date.
	// notes:
	//   1) return invalid value if did not backup yet.
	static LONGLONG GetDBBackupDate();

	// update the db backup date.
	static void UpdateDBBackupDate(LONGLONG iDate);
	/************************************************************************/


    // debug warning
    static void DebugWarn(CString strMsg, const CWnd* pParentWnd = NULL);
};

END_CUTLEADER_NAMESPACE()

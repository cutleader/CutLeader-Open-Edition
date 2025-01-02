#pragma once

#include "clBaseDataCommon.h"

BEGIN_CUTLEADER_NAMESPACE()

typedef enum tagLogMsgLevel
{
	LOG_MSG_INFO			= 1,
	LOG_MSG_WARNING			= 2,
	LOG_MSG_ERROR			= 3,
} LOG_MSG_LEVEL;

// the log message object.
class ClBaseData_Export LogMessage
{
public:
	LogMessage(void);
	LogMessage(LONGLONG iTimeStamp, LOG_MSG_LEVEL logMsgLevel, CString strTaskName, CString strMsgContent);
	~LogMessage(void);

public: // get/set functions.
	void SetTimeStamp(LONGLONG iTimeStamp) { m_iTimeStamp = iTimeStamp; }
	LONGLONG GetTimeStamp() { return m_iTimeStamp; }

	void SetMsgLevel(LOG_MSG_LEVEL logMsgLevel) { m_logMsgLevel = logMsgLevel; }
	LOG_MSG_LEVEL GetMsgLevel() { return m_logMsgLevel; }

	void SetTaskName(CString strTaskName) { m_strTaskName = strTaskName; }
	CString GetTaskName() { return m_strTaskName; }

	void SetMsgContent(CString strMsgContent) { m_strMsgContent = strMsgContent; }
	CString GetMsgContent() { return m_strMsgContent; }

private:
	LONGLONG m_iTimeStamp;

	// the message level.
	LOG_MSG_LEVEL m_logMsgLevel;

	// the task name.
	CString m_strTaskName;

	// the message content.
	CString m_strMsgContent;
};

END_CUTLEADER_NAMESPACE()

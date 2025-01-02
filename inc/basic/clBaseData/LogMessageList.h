#pragma once

#include "clBaseDataCommon.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(LogMessage)
DECLARE_CUTLEADER_CLASS(LogMessageList)

BEGIN_CUTLEADER_NAMESPACE()

// the LogMessage list.
class ClBaseData_Export LogMessageList : public std::vector<LogMessagePtr>
{
public:
	LogMessageList(void);

	~LogMessageList(void);

public:
	LogMessageListPtr GetLogMsgByTaskName(CString strTaskName);
};

END_CUTLEADER_NAMESPACE()

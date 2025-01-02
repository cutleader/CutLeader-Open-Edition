#pragma once

#include "clUtilityCommon.h"

BEGIN_CUTLEADER_NAMESPACE()

// this class can do something about date.
class ClUtility_Export DateHelper
{
public:
	// get the current time stamp.
    static long long GetCurrentTime_Millisecond();
	static LONGLONG GetCurrentTime();

	// get string from time stamp.
	// notes:
	//   1) format as "2010-3-11 17:23:35" will be returned.
	static CString GetStrFromTimeStamp(long long int iTimeStamp);
	// notes:
	//   1) format as "2010-3-11" will be returned.
	static CString GetStrFromTimeStampA(long long int iTimeStamp);

	// get time stamp from string.
	// notes:
	//   1) format of "strTimeStamp" will be "2010-3-11 17:23:35".
	static LONGLONG GetTimeStampFromStr(CString strTimeStamp);

	// get the interval day.
	static LONGLONG GetIntervalDay(LONGLONG iFromTime, LONGLONG iToTime);
};

END_CUTLEADER_NAMESPACE()

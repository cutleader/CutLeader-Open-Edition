#pragma once

#include "clMachineLibUICommon.h"

BEGIN_CUTLEADER_NAMESPACE()

class ClMachineLibUI_Export MachineLibHelper
{
public:
	// 根据参数配置得到g代码的后缀名。
	static CString GetNcPostFix(LONGLONG iParamConfigID);
};

END_CUTLEADER_NAMESPACE()

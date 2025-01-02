#pragma once

#include "clReportConfigCommon.h"

DECLARE_CUTLEADER_CLASS(TplDesignConfig)

BEGIN_CUTLEADER_NAMESPACE()

class ClReportConfig_Export TplDesignConfigLoader
{
public:
	static TplDesignConfigPtr LoadTplDesignConfig();
};

END_CUTLEADER_NAMESPACE()

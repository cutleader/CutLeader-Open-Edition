#pragma once

#include "clReportConfigCommon.h"

DECLARE_CUTLEADER_CLASS(TplDesignConfig)

BEGIN_CUTLEADER_NAMESPACE()

class ClReportConfig_Export TplDesignConfigWriter
{
public:
	static void SaveTplDesignConfig(TplDesignConfigPtr pTplDesignConfig);
};

END_CUTLEADER_NAMESPACE()

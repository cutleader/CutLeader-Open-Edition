#pragma once

#include "clReportConfigCommon.h"
#include "tinyxml.h"

DECLARE_CUTLEADER_CLASS(TplDisplayParam)

BEGIN_CUTLEADER_NAMESPACE()

class TplDisplayParamLoader
{
public:
	static TplDisplayParamPtr LoadTplDisplayParam(TiXmlElement* pTplDisplayParamElement);
};

END_CUTLEADER_NAMESPACE()

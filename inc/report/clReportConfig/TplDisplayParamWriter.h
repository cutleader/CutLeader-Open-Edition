#pragma once

#include "clReportConfigCommon.h"
#include "tinyxml.h"

DECLARE_CUTLEADER_CLASS(TplDisplayParam)

BEGIN_CUTLEADER_NAMESPACE()

class TplDisplayParamWriter
{
public:
	static void SaveTplDisplayParam(TiXmlElement* pTplDisplayParamElement, TplDisplayParamPtr pTplDisplayParam);
};

END_CUTLEADER_NAMESPACE()

#pragma once

#include "clReportConfigCommon.h"
#include "tinyxml.h"

DECLARE_CUTLEADER_CLASS(TplEditParam)

BEGIN_CUTLEADER_NAMESPACE()

class TplEditParamLoader
{
public:
	static TplEditParamPtr LoadTplEditParam(TiXmlElement* pTplEditParamElement);
};

END_CUTLEADER_NAMESPACE()

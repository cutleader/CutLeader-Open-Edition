#pragma once

#include "clReportConfigCommon.h"
#include "tinyxml.h"

DECLARE_CUTLEADER_CLASS(TplEditParam)

BEGIN_CUTLEADER_NAMESPACE()

class TplEditParamWriter
{
public:
	static void SaveTplEditParam(TiXmlElement* pTplEditParamElement, TplEditParamPtr pTplEditParam);
};

END_CUTLEADER_NAMESPACE()

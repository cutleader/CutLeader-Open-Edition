#pragma once

#include "clReportConfigCommon.h"
#include "tinyxml.h"

DECLARE_CUTLEADER_CLASS(TemplateParam)

BEGIN_CUTLEADER_NAMESPACE()

class TemplateParamWriter
{
public:
	static void SaveTemplateParam(TiXmlElement* pTemplateParamElement, TemplateParamPtr pTemplateParam);
};

END_CUTLEADER_NAMESPACE()

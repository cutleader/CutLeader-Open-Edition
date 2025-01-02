#pragma once

#include "clReportConfigCommon.h"
#include "tinyxml.h"

DECLARE_CUTLEADER_CLASS(TemplateParam)

BEGIN_CUTLEADER_NAMESPACE()

class TemplateParamLoader
{
public:
	static TemplateParamPtr LoadTemplateParam(TiXmlElement* pTemplateParamElement);
};

END_CUTLEADER_NAMESPACE()

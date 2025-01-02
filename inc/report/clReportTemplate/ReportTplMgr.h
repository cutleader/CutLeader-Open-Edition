#pragma once

#include "clReportTemplateCommon.h"

DECLARE_CUTLEADER_CLASS(ReportTplPage)

BEGIN_CUTLEADER_NAMESPACE()

class ClReportTemplate_Export ReportTplMgr
{
public:
	// build the empty report template page.
	static ReportTplPagePtr BuildEmptyReportTpl();
};

END_CUTLEADER_NAMESPACE()

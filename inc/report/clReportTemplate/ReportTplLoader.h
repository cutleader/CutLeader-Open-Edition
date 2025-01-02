#pragma once

#include "clReportTemplateCommon.h"
#include "tinyxml.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(ReportTpl)
DECLARE_CUTLEADER_CLASS(ReportTplList)

BEGIN_CUTLEADER_NAMESPACE()

// the report template loader.
class ClReportTemplate_Export ReportTplLoader
{
public:
	static ReportTplListPtr LoadAllReportTpl(BOOL bLoadEntity = TRUE);

	static ReportTplPtr LoadReportTpl(LONGLONG iTplID);

private:
	// load all data in "report_reportTpl" table.
	static void LoadAllTplPage(std::vector<LONGLONG>& tplIDList, std::vector<LONGLONG>& pageIDList);
};

END_CUTLEADER_NAMESPACE()

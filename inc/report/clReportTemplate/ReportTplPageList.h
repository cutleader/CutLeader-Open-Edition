#pragma once

#include "clReportTemplateCommon.h"
#include "ClData.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(ReportTplPage)
DECLARE_CUTLEADER_CLASS(ReportTplPageList)

BEGIN_CUTLEADER_NAMESPACE()

class ClReportTemplate_Export ReportTplPageList : public std::vector<ReportTplPagePtr>,
											 public ClData
{
public:
	ReportTplPageList(void);
	~ReportTplPageList(void);

public:
	ReportTplPageListPtr GetPageByType(REPORT_TPL_PAGE_TYPE tplPageType);

	ReportTplPagePtr GetPageByID(LONGLONG iTplPageID);

	void DeleteReportTplPage(LONGLONG iReportTplPageID);
};

END_CUTLEADER_NAMESPACE()

#pragma once

#include "clReportTemplateCommon.h"
#include "ClData.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(ReportTpl)
DECLARE_CUTLEADER_CLASS(ReportTplList)

BEGIN_CUTLEADER_NAMESPACE()

class ClReportTemplate_Export ReportTplList : public std::vector<ReportTplPtr>,
										 public ClData
{
public:
	ReportTplList(void);
	~ReportTplList(void);

public:
	ReportTplListPtr GetTplByType(REPORT_TPL_TYPE tplType);

	ReportTplPtr GetTplByID(LONGLONG iTplID);

	void DeleteReportTpl(LONGLONG iReportTplID);
};

END_CUTLEADER_NAMESPACE()

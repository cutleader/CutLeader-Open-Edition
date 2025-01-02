#pragma once

#include "clReportPrinterCommon.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(ReportPage)

BEGIN_CUTLEADER_NAMESPACE()

// “报表页”集合。
class ClReportPrinter_Export ReportPageList : public std::vector<ReportPagePtr>
{
public:
	ReportPageList(void);
	~ReportPageList(void);
};

END_CUTLEADER_NAMESPACE()

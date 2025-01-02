#pragma once

#include "clReportPrinterCommon.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(ReportPage)

BEGIN_CUTLEADER_NAMESPACE()

// ������ҳ�����ϡ�
class ClReportPrinter_Export ReportPageList : public std::vector<ReportPagePtr>
{
public:
	ReportPageList(void);
	~ReportPageList(void);
};

END_CUTLEADER_NAMESPACE()

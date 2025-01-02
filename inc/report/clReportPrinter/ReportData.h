#pragma once

#include "clReportPrinterCommon.h"

DECLARE_CUTLEADER_CLASS(ReportPageList)

BEGIN_CUTLEADER_NAMESPACE()

// ������󣬿��԰������������ҳ����
class ClReportPrinter_Export ReportData
{
public:
	ReportData(void);
	~ReportData(void);

public: // get/set functions.
	void SetReportPageList(ReportPageListPtr pReportPageList) { m_pReportPageList = pReportPageList; }
	ReportPageListPtr GetReportPageList() { return m_pReportPageList; }

private:
	ReportPageListPtr m_pReportPageList;
};

END_CUTLEADER_NAMESPACE()

#pragma once

#include "clReportPrinterCommon.h"

DECLARE_CUTLEADER_CLASS(ReportTplPage)
DECLARE_CUTLEADER_CLASS(RptEntityList)

BEGIN_CUTLEADER_NAMESPACE()

// ±®±Ì“≥°£
class ClReportPrinter_Export ReportPage
{
public:
	ReportPage(ReportTplPagePtr pReportTplPage);
	~ReportPage(void);

public: // get/set functions.
	void SetReportTplPage(ReportTplPagePtr pReportTplPage) { m_pReportTplPage = pReportTplPage; }
	ReportTplPagePtr GetReportTplPage() { return m_pReportTplPage; }

	void SetRptEntityList(RptEntityListPtr pRptEntityList) { m_pRptEntityList = pRptEntityList; }
	RptEntityListPtr GetRptEntityList() { return m_pRptEntityList; }

private:
	ReportTplPagePtr m_pReportTplPage;

	// all report entities in this report page.
	RptEntityListPtr m_pRptEntityList;
};

END_CUTLEADER_NAMESPACE()

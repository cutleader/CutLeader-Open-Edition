#pragma once

#include "clReportPrinterCommon.h"

DECLARE_CUTLEADER_CLASS(IRptEntity)
DECLARE_CUTLEADER_CLASS(ReportPage)

BEGIN_CUTLEADER_NAMESPACE()

// ¡°±¨±íÒ³¡±»æÖÆÆ÷¡£
class ClReportPrinter_Export RptPageDrawer
{
public:
	static void DrawRptPage(ReportPagePtr pReportPage, CDC* pDC);

private:
	static void DrawRptEntity(IRptEntityPtr pRptEntity, CDC* pDC);
};

END_CUTLEADER_NAMESPACE()

#pragma once

#include "clReportTemplateCommon.h"

DECLARE_CUTLEADER_CLASS(ReportTplPage)
DECLARE_CUTLEADER_CLASS(TplEntityList)

BEGIN_CUTLEADER_NAMESPACE()

class ClReportTemplate_Export ReportTplDrawer
{
public:
	static void DrawReportTpl(ReportTplPagePtr pReportTplPage, CDC* pDC, CRect totalRect, double dZoomLevel);
	static void DrawReportTpl(ReportTplPagePtr pReportTplPage, TplEntityListPtr pIgnoreEntityList, CDC* pDC, CRect totalRect, double dZoomLevel);

private:
	static void DrawGrid(ReportTplPagePtr pReportTplPage, CDC* pDC, CRect rect, double dZoomLevel );
	static void DrawMargins(CDC* pDC, CRect rect, double dZoomLevel );
};

END_CUTLEADER_NAMESPACE()

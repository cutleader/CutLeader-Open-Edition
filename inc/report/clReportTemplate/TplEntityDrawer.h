#pragma once

#include "clReportTemplateCommon.h"

DECLARE_CUTLEADER_CLASS(TplEntityList)
DECLARE_CUTLEADER_CLASS(ITplEntity)

BEGIN_CUTLEADER_NAMESPACE()

class ClReportTemplate_Export TplEntityDrawer
{
public:
	static void DrawEntity(TplEntityListPtr pTplEntityList, CDC* pDC, double dZoomLevel, BOOL bDrawMarker = FALSE);
	static void DrawEntity(TplEntityListPtr pTplEntityList, TplEntityListPtr pIgnoreEntityList, CDC* pDC, double dZoomLevel, BOOL bDrawMarker = FALSE);
	static void DrawEntity(ITplEntityPtr pTplEntity, CDC* pDC, double dZoomLevel, BOOL bDrawMarker = FALSE);

	static void DrawMarker(TplEntityListPtr pTplEntityList, CDC* pDC, double dZoomLevel);
	static void DrawMarker(ITplEntityPtr pTplEntity, CDC* pDC, double dZoomLevel);

private:
	static void DrawMarker(ITplEntityPtr pTplEntity, CDC* pDC, CRect rect );
};

END_CUTLEADER_NAMESPACE()

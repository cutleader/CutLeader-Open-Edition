#pragma once

#include "clReportTemplateCommon.h"
#include "tinyxml.h"

DECLARE_CUTLEADER_CLASS(FontParam)
DECLARE_CUTLEADER_CLASS(DBFieldList)
DECLARE_CUTLEADER_CLASS(EntityGroupList)
DECLARE_CUTLEADER_CLASS(ReportTplPage)
DECLARE_CUTLEADER_CLASS(ReportTplPageList)
DECLARE_CUTLEADER_CLASS(TplEntityList)

BEGIN_CUTLEADER_NAMESPACE()

// the report template page loader.
class ClReportTemplate_Export TplPageLoader
{
public:
	// load all template pages.
	static ReportTplPageListPtr LoadAllReportTplPage(BOOL bLoadEntity = TRUE);

	static ReportTplPagePtr LoadReportTplPage(LONGLONG iTplPageID);

private:
	// load all entities of the page.
	static TplEntityListPtr LoadEntityList(LONGLONG iTplPageID);

	// load all entity groups of the page.
	static EntityGroupListPtr LoadGroupList(LONGLONG iTplPageID, TplEntityListPtr pTplEntityList);

	// about loading font.
	static FontParamPtr LoadFontParam(DBFieldListPtr pDBFieldList);
	static FontParamPtr LoadColumnHeaderFont(DBFieldListPtr pDBFieldList);
};

END_CUTLEADER_NAMESPACE()

#pragma once

#include "clReportTemplateCommon.h"
#include "tinyxml.h"

DECLARE_CUTLEADER_CLASS(ReportTpl)
DECLARE_CUTLEADER_CLASS(ReportTplPageList)

BEGIN_CUTLEADER_NAMESPACE()

class ClReportTemplate_Export ReportTplWriter
{
public:
	// create the report template.
	// notes:
	//   1) this function only update "report_reportTpl" and "report_tplPageList" table.
	static BOOL CreateReportTpl(ReportTplPtr pReportTpl, BOOL bTransaction = TRUE);

	// delete a report template.
	static BOOL DeleteReportTpl(LONGLONG iTplID, BOOL bTransaction = TRUE);

	// update the basic data of the template.
	// notes:
	//   1) after call this, must call function "UpdateModifyDate".
	static void UpdateBasicData(ReportTplPtr pReportTpl, BOOL bTransaction = TRUE);

	// update the modified data of the template.
	static void UpdateModifyDate(LONGLONG iTplID, LONGLONG iModifyTimeStamp, BOOL bTransaction = TRUE);

	// update the pages of the template.
	static void UpdateTplPage(LONGLONG iTplID, ReportTplPageListPtr pTplPageList, BOOL bTransaction = TRUE);
};

END_CUTLEADER_NAMESPACE()

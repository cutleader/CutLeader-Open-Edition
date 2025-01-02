#pragma once

#include "clReportTemplateCommon.h"
#include "BusinessData.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(ReportTplPage)
DECLARE_CUTLEADER_CLASS(ReportTplPageList)
DECLARE_CUTLEADER_CLASS(ReportTpl)

BEGIN_CUTLEADER_NAMESPACE()

// ±¨±íÄ£°å¡£
class ClReportTemplate_Export ReportTpl : public std::vector<ReportTplPagePtr>, public BusinessData
{
public:
	ReportTpl(void);
	virtual ~ReportTpl(void);

public: // implement IData interface.
	virtual BOOL IsModified() const override;

public: // implement IBusinessData interface
	virtual BusinessDataType GetBusinessDataType() const override { return BusinessData_ReportTemplate; }

public: // get/set functions.
	void SetReportTplType(REPORT_TPL_TYPE reportTplType) { m_reportTplType = reportTplType; }
	REPORT_TPL_TYPE GetReportTplType() const { return m_reportTplType; }

	void SetReportTplPageList(ReportTplPageListPtr pReportTplPageList) { m_pReportTplPageList = pReportTplPageList; }
	ReportTplPageListPtr GetReportTplPageList() const { return m_pReportTplPageList; }

public: // some static functions.
	static std::vector<REPORT_TPL_TYPE> GetAllTplType();
	static CString GetReportTplTypeName(REPORT_TPL_TYPE reportTplType);

	// get the type of pages which can be added into the report template.
	static std::vector<REPORT_TPL_PAGE_TYPE> GetLegalPageType(REPORT_TPL_TYPE reportTplType);

private:
	REPORT_TPL_TYPE m_reportTplType;

	// the pages in the report template.
	ReportTplPageListPtr m_pReportTplPageList;
};

END_CUTLEADER_NAMESPACE()

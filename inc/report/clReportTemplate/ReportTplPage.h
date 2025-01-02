#pragma once

#include "clReportTemplateCommon.h"
#include "ClData.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(EntityGroupList)
DECLARE_CUTLEADER_CLASS(TplEntityList)

BEGIN_CUTLEADER_NAMESPACE()

// ±¨±íÄ£°åÒ³¡£
class ClReportTemplate_Export ReportTplPage : public ClData
{
public:
	ReportTplPage(void);
	~ReportTplPage(void);

public: // implement IData interface.
	virtual BOOL IsModified() const override;

public: // get/set functions.
	void SetName(CString strName) { m_strName = strName; }
	CString GetName() { return m_strName; }

	void SetTplPageType(REPORT_TPL_PAGE_TYPE tplPageType) { m_tplPageType = tplPageType; }
	REPORT_TPL_PAGE_TYPE GetTplPageType() { return m_tplPageType; }

	void SetPaperWidth(int iWidth) { m_iWidth = iWidth; }
	int GetPaperWidth() { return m_iWidth; }

	void SetPaperHeight(int iHeight) { m_iHeight = iHeight; }
	int GetPaperHeight() { return m_iHeight; }

	void SetLeftMargin(int iLeftMargin) { m_iLeftMargin = iLeftMargin; }
	int GetLeftMargin() { return m_iLeftMargin; }

	void SetRightMargin(int iRightMargin) { m_iRightMargin = iRightMargin; }
	int GetRightMargin() { return m_iRightMargin; }

	void SetTopMargin(int iTopMargin) { m_iTopMargin = iTopMargin; }
	int GetTopMargin() { return m_iTopMargin; }

	void SetBottomMargin(int iBottomMargin) { m_iBottomMargin = iBottomMargin; }
	int GetBottomMargin() { return m_iBottomMargin; }

	void SetEntityGroupList(EntityGroupListPtr pEntityGroupList) { m_pEntityGroupList = pEntityGroupList; }
	EntityGroupListPtr GetEntityGroupList() { return m_pEntityGroupList; }

	void SetTplEntityList(TplEntityListPtr pTplEntityList) { m_pTplEntityList = pTplEntityList; }
	TplEntityListPtr GetTplEntityList() { return m_pTplEntityList; }

public: // some static functions.
	static std::vector<REPORT_TPL_PAGE_TYPE> GetAllPageType();
	static CString GetTplPageTypeName(REPORT_TPL_PAGE_TYPE tplPageType);

public:
	// whether this template page include the grid entity.
	BOOL IncludeGridEntity();

private:
	CString m_strName;

	REPORT_TPL_PAGE_TYPE m_tplPageType;

	// the size of the paper.
	int m_iWidth;
	int m_iHeight;

	// the margin of the paper.
	int m_iLeftMargin;
	int m_iRightMargin;
	int m_iTopMargin;
	int m_iBottomMargin;

	// the group list in the template.
	EntityGroupListPtr m_pEntityGroupList;

	// the entities in the template.
	TplEntityListPtr m_pTplEntityList;
};

END_CUTLEADER_NAMESPACE()

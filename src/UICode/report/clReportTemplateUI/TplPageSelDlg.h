#pragma once

#include "clReportTemplateCommon.h"
#include "clReportTemplateUICommon.h"
#include "taoResource.h"
#include <vector>
#include "SkinManager.h"

DECLARE_CUTLEADER_CLASS(ReportTplPage)
DECLARE_CUTLEADER_CLASS(ReportTplPageList)

BEGIN_CUTLEADER_NAMESPACE()

class ClReportTemplateUI_Export TplPageSelDlg : public CLDialog
{
	DECLARE_DYNCREATE(TplPageSelDlg)

public:
	TplPageSelDlg(void);
	~TplPageSelDlg(void);

	enum { IDD = IDD_clReportTemplateUI_TemplatePage_Select };

protected: // inherit the interface
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

public: // message mapping.
	afx_msg HBRUSH OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor );
	
	afx_msg void OnSelchangePageType();
	afx_msg void OnOK();

	DECLARE_MESSAGE_MAP()

public:
	void Init(std::vector<REPORT_TPL_PAGE_TYPE> tplPageTypeList) { m_tplPageTypeList = tplPageTypeList; }

	ReportTplPagePtr GetSelTplPage() { return m_pReportTplPage; }

private:
	void InitTplPageGrid(REPORT_TPL_PAGE_TYPE tplPageType);

private: // controls.
	CComboBox m_ctrlTplPageType;

	// template page list.
	SkinGridListCtrlGroups m_ctrlTplPageList;

	SkinMFCButton m_btnOK;
	SkinMFCButton m_btnCancel;

private: // data
	// all template pages, do not load entities.
	ReportTplPageListPtr m_pReportTplPageList;

	std::vector<REPORT_TPL_PAGE_TYPE> m_tplPageTypeList;

	// the selected template page.
	ReportTplPagePtr m_pReportTplPage;
};

END_CUTLEADER_NAMESPACE()

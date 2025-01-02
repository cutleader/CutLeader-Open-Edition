#pragma once

#include "clReportConfigCommon.h"
#include "clReportTemplateCommon.h"
#include "clReportTemplateUICommon.h"
#include "NumEdit.h"
#include "taoResource.h"
#include "ReportTplPage.h"
#include "SkinManager.h"


DECLARE_CUTLEADER_CLASS(ReportTplPageList)
DECLARE_CUTLEADER_CLASS(ReportTplPage)


BEGIN_CUTLEADER_NAMESPACE()


class ClReportTemplateUI_Export TplPageDlg : public CLDialog
{
	DECLARE_DYNCREATE(TplPageDlg)

public:
	TplPageDlg(void);
	~TplPageDlg(void);

	enum { IDD = IDD_clReportTemplateUI_TemplatePage };

protected: // inherit the interface
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

public: // message mapping.
	afx_msg HBRUSH OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor );

	afx_msg void OnSelchangePageType();
	afx_msg void OnSelchangePage();

	afx_msg void OnDelTplPage();

	afx_msg void OnApply();
	afx_msg void OnOpen();
	afx_msg void OnCancel();

	DECLARE_MESSAGE_MAP()

public:
	LONGLONG GetCurReportTplPageID() { return m_pReportTplPage->GetID(); }

private:
	void InitListBox(ReportTplPageListPtr pReportTplPageList);

	// init the template page info based on the selected template page.
	void InitTplPageInfo();

	// check whether the template page is modified.
	BOOL IsTplPageModified();

	// save the change to database.
	void SaveChange();

	// update the control status.
	void UpdateCtrlStatus();

private: // controls.
	CComboBox m_ctrlTplPageType;
	CListBox m_ctrlTplPageList;

	// basic info.
	CString m_strTplPageName;
	CString m_strTplPageType;
	CString m_strComment;

	// paper size.
	CNumEdit m_dPaperWidth;
	CNumEdit m_dPaperHeight;

	// paper margin
	CNumEdit m_dLeftMargin;
	CNumEdit m_dRightMargin;
	CNumEdit m_dTopMargin;
	CNumEdit m_dBottomMargin;

	SkinMFCButton m_btnDel;
	SkinMFCButton m_btnApply;
	SkinMFCButton m_btnOpen;
	SkinMFCButton m_btnCancel;

private:
	// all template pages, do not load entities.
	ReportTplPageListPtr m_pReportTplPageList;

	// template page which display currently.
	ReportTplPagePtr m_pReportTplPage;

	TPL_UNIT_TYPE m_unitType;
};

END_CUTLEADER_NAMESPACE()

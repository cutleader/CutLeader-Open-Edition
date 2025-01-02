#pragma once

#include "clReportTemplateUICommon.h"
#include "NumEdit.h"
#include "taoResource.h"
#include "SkinManager.h"

DECLARE_CUTLEADER_CLASS(ReportTplPage)

BEGIN_CUTLEADER_NAMESPACE()

class ClReportTemplateUI_Export NewTpPageDlg : public CLDialog
{
	DECLARE_DYNCREATE(NewTpPageDlg)

public:
	NewTpPageDlg(void);
	~NewTpPageDlg(void);

	enum { IDD = IDD_clReportTemplateUI_NewTemplatePage };

protected: // inherit the interface
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

public: // message mapping.
	afx_msg HBRUSH OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor );

	afx_msg void OnOK();
	afx_msg void OnCancel();

	DECLARE_MESSAGE_MAP()

public:
	ReportTplPagePtr GetReportTplPage() { return m_pReportTplPage; }

private: // controls.
	// basic info.
	CString m_strTplPageName;
	CComboBox m_ctrlTplPageType;

	// paper size.
	CNumEdit m_dPaperWidth;
	CNumEdit m_dPaperHeight;

	// paper margin
	CNumEdit m_dLeftMargin;
	CNumEdit m_dRightMargin;
	CNumEdit m_dTopMargin;
	CNumEdit m_dBottomMargin;

	SkinMFCButton m_btnOK;
	SkinMFCButton m_btnCancel;
private:
	// click OK, this variable will be set.
	ReportTplPagePtr m_pReportTplPage;
};

END_CUTLEADER_NAMESPACE()

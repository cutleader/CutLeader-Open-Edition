#pragma once

#include "clReportTemplateCommon.h"
#include "clReportTemplateUICommon.h"
#include "taoResource.h"
#include "SkinManager.h"

BEGIN_CUTLEADER_NAMESPACE()


class ClReportTemplateUI_Export TplTypeSelDlg : public CLDialog
{
	DECLARE_DYNCREATE(TplTypeSelDlg)

public:
	TplTypeSelDlg(void);
	~TplTypeSelDlg(void);

	enum { IDD = IDD_clReportTemplateUI_TemplateType_Select };

protected: // inherit the interface
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

public: // message mapping.
	afx_msg HBRUSH OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor );

	afx_msg void OnOK();

	DECLARE_MESSAGE_MAP()

public:
	REPORT_TPL_TYPE GetSelTplType() { return m_tplType; }

private: // controls.
	CComboBox m_ctrlTplType;

	SkinMFCButton m_btnOK;
	SkinMFCButton m_btnCancel;

private: // data
	REPORT_TPL_TYPE m_tplType;
};

END_CUTLEADER_NAMESPACE()

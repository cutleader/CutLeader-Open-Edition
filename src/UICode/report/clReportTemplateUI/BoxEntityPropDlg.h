#pragma once

#include "clReportTemplateCommon.h"
#include "NumEdit.h"
#include "ColorStatic.h"
#include "taoResource.h"
#include "SkinManager.h"

DECLARE_CUTLEADER_CLASS(BoxTplEntity)

BEGIN_CUTLEADER_NAMESPACE()

class BoxEntityPropDlg : public CLDialog
{
	DECLARE_DYNCREATE(BoxEntityPropDlg)

public:
	BoxEntityPropDlg();
	~BoxEntityPropDlg(void);

	enum { IDD = IDD_clReportTemplateUI_BoxProperty };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

protected:
	afx_msg HBRUSH OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor );

	afx_msg void OnFillColor();
	afx_msg void OnBorderColor();

	afx_msg void OnOK();
	afx_msg void OnCancel();

	DECLARE_MESSAGE_MAP()

public:
	void Init(BoxTplEntityPtr pBoxTplEntity) { m_pBoxTplEntity = pBoxTplEntity; }

private:
	CString m_strName;
	
	// for fill
	BOOL m_bFill;
	ColorStatic	m_ctrlFillColor;

	// for border.
	BOOL m_bBorder;
	CNumEdit m_iBorderThickness;
	CComboBox m_ctrlBorderStyle;
	ColorStatic	m_ctrlBorderColor;

	SkinMFCButton m_btnFillColor;
	SkinMFCButton m_btnBorderColor;
	SkinMFCButton m_btnOK;
	SkinMFCButton m_btnCancel;

private:
	BoxTplEntityPtr m_pBoxTplEntity;
};

END_CUTLEADER_NAMESPACE()

#pragma once

#include "clReportTemplateCommon.h"
#include "NumEdit.h"
#include "ColorStatic.h"
#include "taoResource.h"
#include "SkinManager.h"

DECLARE_CUTLEADER_CLASS(LineTplEntity)

BEGIN_CUTLEADER_NAMESPACE()

class LineEntityPropDlg : public CLDialog
{
	DECLARE_DYNCREATE(LineEntityPropDlg)

public:
	LineEntityPropDlg();
	~LineEntityPropDlg(void);

	enum { IDD = IDD_clReportTemplateUI_LineProperty };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

protected:
	afx_msg HBRUSH OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor );
	
	afx_msg void OnLineColor();
	afx_msg void OnOK();
	afx_msg void OnCancel();

	DECLARE_MESSAGE_MAP()

public:
	void Init(LineTplEntityPtr pLineTplEntity) { m_pLineTplEntity = pLineTplEntity; }

private: // for control
	CString m_strName;
	CNumEdit m_iLineThickness;
	CComboBox m_ctrlLineStyle;

	ColorStatic	m_ctrlLineColor;

	SkinMFCButton m_btnColor;
	SkinMFCButton m_btnOK;
	SkinMFCButton m_btnCancel;

private:
	LineTplEntityPtr m_pLineTplEntity;
};

END_CUTLEADER_NAMESPACE()

#pragma once

#include "clReportTemplateCommon.h"
#include "taoResource.h"
#include "SkinManager.h"

DECLARE_CUTLEADER_CLASS(StaticTplEntity)

BEGIN_CUTLEADER_NAMESPACE()

class StaticEntityPropDlg : public CLDialog
{
	DECLARE_DYNCREATE(StaticEntityPropDlg)

public:
	StaticEntityPropDlg();
	~StaticEntityPropDlg(void);

	enum { IDD = IDD_clReportTemplateUI_StaticProperty };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

protected:
	afx_msg HBRUSH OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor );

	afx_msg void OnFont();
	afx_msg void OnOK();
	afx_msg void OnCancel();

	DECLARE_MESSAGE_MAP()

public:
	void Init(StaticTplEntityPtr pStaticTplEntity) { m_pStaticTplEntity = pStaticTplEntity; }

private: // controls.
	CString m_strName;
	CString	m_strContent;
	int m_iJustification;

	SkinMFCButton m_btnFont;
	SkinMFCButton m_btnOK;
	SkinMFCButton m_btnCancel;

private:
	StaticTplEntityPtr m_pStaticTplEntity;

	// font info.
	LOGFONT	m_logicFont;
	COLORREF m_fontColor;
};

END_CUTLEADER_NAMESPACE()

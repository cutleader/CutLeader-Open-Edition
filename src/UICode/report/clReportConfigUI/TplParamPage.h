#pragma once

#include "clReportConfigCommon.h"
#include "clReportConfigUICommon.h"
#include "NumEdit.h"
#include "taoResource.h"
#include "CLResizableDialog.h"
#include "SkinManager.h"

DECLARE_CUTLEADER_CLASS(TemplateParam)

BEGIN_CUTLEADER_NAMESPACE()

class TplParamPage : public CLResizableDialog
{
	DECLARE_DYNCREATE(TplParamPage)

public:
	TplParamPage(void);
	~TplParamPage(void);

	enum { IDD = IDD_clReportConfigUI_Template };

protected: // inherit the interface
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

public: // message mapping.
	afx_msg HBRUSH OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor );

	afx_msg void OnFont();

	DECLARE_MESSAGE_MAP()

public:
	void DisplayTemplateParam(TemplateParamPtr pTemplateParam, TPL_UNIT_TYPE unitType);

	// fill the Template Param using the user input.
	TemplateParamPtr GetTemplateParam();

private: // controls.
	CString m_strMsg;

	// paper size.
	CNumEdit m_dPaperWidth;
	CNumEdit m_dPaperHeight;

	// paper margin
	CNumEdit m_dLeftMargin;
	CNumEdit m_dRightMargin;
	CNumEdit m_dTopMargin;
	CNumEdit m_dBottomMargin;

	// text justification type.
	CComboBox m_ctrlTextJustType;
	SkinMFCButton m_btnFont;

private:
	TemplateParamPtr m_pTemplateParam;
	TPL_UNIT_TYPE m_unitType;

	LOGFONT m_logicFont;
	COLORREF m_fontColor;
};

END_CUTLEADER_NAMESPACE()

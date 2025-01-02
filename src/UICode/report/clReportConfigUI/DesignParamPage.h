#pragma once

#include "clReportConfigCommon.h"
#include "clReportConfigUICommon.h"
#include "NumEdit.h"
#include "taoResource.h"
#include "CLResizableDialog.h"
#include "SkinManager.h"

DECLARE_CUTLEADER_CLASS(TplEditParam)


BEGIN_CUTLEADER_NAMESPACE()


class DesignParamPage : public CLResizableDialog
{
	DECLARE_DYNCREATE(DesignParamPage)

public:
	DesignParamPage(void);
	~DesignParamPage(void);

	enum { IDD = IDD_clReportConfigUI_Design };

protected: // inherit the interface
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

public: // message mapping.
	afx_msg HBRUSH OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor );

	afx_msg void OnFont();

	DECLARE_MESSAGE_MAP()

public:
	void DisplayTplEditParam(TplEditParamPtr pTplEditParam, TPL_UNIT_TYPE unitType);

	// fill the TplEditParam using the user input.
	TplEditParamPtr GetTplEditParam();

private: // controls.
	CString m_strMsg;

	// about grid.
	BOOL m_bSnapGrid;
	CNumEdit m_dGridCellXSize;
	CNumEdit m_dGridCellYSize;

private:
	TplEditParamPtr m_pTplEditParam;
	TPL_UNIT_TYPE m_unitType;
};

END_CUTLEADER_NAMESPACE()

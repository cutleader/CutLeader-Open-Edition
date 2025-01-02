#pragma once

#include "taoResource.h"
#include "NumEdit.h"
#include "TplDisplayParam.h"
#include "SkinManager.h"

DECLARE_CUTLEADER_CLASS(BindItemList)
DECLARE_CUTLEADER_CLASS(GridColumn)

BEGIN_CUTLEADER_NAMESPACE()

class GridColumnDlg : public CLDialog
{
public:
	GridColumnDlg();
	~GridColumnDlg();

	enum { IDD = IDD_clReportTemplateUI_GridColumn };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

protected:
	afx_msg HBRUSH OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor );

	afx_msg void OnHeaderFont();
	afx_msg void OnFont();

	afx_msg void OnOK();
	afx_msg void OnCancel();

	DECLARE_MESSAGE_MAP()

public:
	void Init(GridColumnPtr pGridColumn, TPL_UNIT_TYPE unitType, BindItemListPtr pAvailBindItemList);

private: // for control.
	CString	m_strName;
	CString	m_strColumnTitle;
	CNumEdit m_dWidth;
	int	m_iJustification;
	CComboBox m_ctrlBindType;

	SkinMFCButton m_btnHeaderFont;
	SkinMFCButton m_btnFont;
	SkinMFCButton m_btnOK;
	SkinMFCButton m_btnCancel;

private:
	// use this brush to paint the background.
	CBrush m_brush;

	BindItemListPtr m_pAvailBindItemList;
	GridColumnPtr m_pGridColumn;
	TPL_UNIT_TYPE m_unitType;

	// font info.
	LOGFONT	m_logicHeaderFont, m_logicFont;
	COLORREF m_headerFontColor, m_fontColor;
};

END_CUTLEADER_NAMESPACE()

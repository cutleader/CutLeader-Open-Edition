#pragma once

#include "taoResource.h"
#include "NumEdit.h"
#include "TplDisplayParam.h"
#include "ColorStatic.h"
#include "ListBoxEx.h"
#include "SkinManager.h"

DECLARE_CUTLEADER_CLASS(GridTplEntity)
DECLARE_CUTLEADER_CLASS(GridColumn)
DECLARE_CUTLEADER_CLASS(BindItemList)

BEGIN_CUTLEADER_NAMESPACE()

class GridEntityPropDlg : public CLDialog
{
public:
	GridEntityPropDlg();
	~GridEntityPropDlg();

	enum { IDD = IDD_clReportTemplateUI_GridProperty };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

protected:
	afx_msg HBRUSH OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor );

	afx_msg void OnAdd();
	afx_msg void OnUp();
	afx_msg void OnDown();
	afx_msg void OnDelete();

	afx_msg void OnColumnLineColor();
	afx_msg void OnRowLineColor();
	afx_msg void OnBorderLineColor();

	afx_msg void OnOK();
	afx_msg void OnCancel();

	LRESULT OnListboxDblClick( WPARAM, LPARAM );
	LRESULT OnListboxDelete( WPARAM, LPARAM );

	DECLARE_MESSAGE_MAP()

public:
	void Init(GridTplEntityPtr pGridTplEntity, TPL_UNIT_TYPE unitType, BindItemListPtr pAvailBindItemList);

private:
	CString GetColumnText(GridColumnPtr pColumn);
	void RefillColumnList();

private: // for controls.
	// the column name.
	CString	m_strName;

	// the row height.
	CNumEdit m_dRowHeight;

	// the header height.
	CNumEdit m_dHeaderHeight;

	// column list.
	ListBoxEx m_ctrlColumnList;

	// for border.
	BOOL m_bShowBorder;
	CNumEdit m_iBorderThickness;
	CComboBox m_ctrlBorderStyle;
	ColorStatic	m_ctrlBorderColor;

	// for column lines.
	BOOL m_bShowColumnLine;
	CNumEdit m_iColumnLineThickness;
	CComboBox m_ctrlColumnStyle;
	ColorStatic	m_ctrlColumnColor;

	// for row lines.
	BOOL m_bShowRowLine;
	CNumEdit m_iRowLineThickness;
	CComboBox m_ctrlRowStyle;
	ColorStatic	m_ctrlRowColor;

	CComboBox m_ctrlBindType;

	SkinMFCButton m_btnAdd;
	SkinMFCButton m_btnUp;
	SkinMFCButton m_btnDown;
	SkinMFCButton m_btnDel;

	SkinMFCButton m_btnBorderColor;
	SkinMFCButton m_btnColumnColor;
	SkinMFCButton m_btnRowColor;

	SkinMFCButton m_btnOK;
	SkinMFCButton m_btnCancel;

private: // for data.
	GridTplEntityPtr m_pGridTplEntity;
	TPL_UNIT_TYPE m_unitType;
	BindItemListPtr m_pAvailBindItems;
};

END_CUTLEADER_NAMESPACE()

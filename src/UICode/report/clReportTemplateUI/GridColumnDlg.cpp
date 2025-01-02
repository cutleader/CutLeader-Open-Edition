#include "stdafx.h"
#include "GridColumnDlg.h"

#include "UnitHelper.h"
#include "baseConst.h"

#include "reportConst.h"
#include "FontParam.h"
#include "GridColumn.h"

#include "BindItem.h"
#include "BindItemList.h"

#include "Uxtheme.h"
#pragma comment(lib, "Uxtheme.lib")

BEGIN_CUTLEADER_NAMESPACE()

GridColumnDlg::GridColumnDlg()
	: CLDialog(GridColumnDlg::IDD)
{
}

GridColumnDlg::~GridColumnDlg()
{
}

void GridColumnDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	DDX_Text(pDX, IDC_EDIT_COL_HEAD, m_strColumnTitle);
	DDX_Control(pDX, IDC_EDIT_WIDTH, m_dWidth);
	DDX_Radio(pDX, IDC_RADIO_LEFT, m_iJustification);
	DDX_Control(pDX, IDC_COMBO_BIND_TYPE, m_ctrlBindType);

	DDX_Control(pDX, IDC_BTN_HEADER_FONT, m_btnHeaderFont);
	DDX_Control(pDX, IDC_BTN_FONT, m_btnFont);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
}

BOOL GridColumnDlg::OnInitDialog() 
{
	CLDialog::OnInitDialog();

	// set the column name.
	m_strName = m_pGridColumn->GetName();

	// the column title.
	m_strColumnTitle = m_pGridColumn->GetColumnTitle();

	// set the column width.
	if (m_unitType == TPL_UNIT_PIXEL)
		m_dWidth.Init(m_pGridColumn->GetWidth(), 0, FALSE);
	else if (m_unitType == TPL_UNIT_INCH)
		m_dWidth.Init(m_pGridColumn->GetWidth(), TPL_DIGITAL_NUM, FALSE);
	else if (m_unitType == TPL_UNIT_CM)
		m_dWidth.Init(m_pGridColumn->GetWidth(), TPL_DIGITAL_NUM, FALSE);

	// set the column justification.
	m_iJustification = m_pGridColumn->GetTextJustType();

	// init the data bind info.
	int iIndex = 0;
	m_ctrlBindType.AddString(_T(""));
	m_ctrlBindType.SetItemData(0, (DWORD_PTR)BIND_ITEM_NOT_INIT);
	for (unsigned int i = 0; i < m_pAvailBindItemList->size(); i++)
	{
		BindItemPtr pBindItem = m_pAvailBindItemList->at(i);
		m_ctrlBindType.AddString(pBindItem->GetBindItemName());
		m_ctrlBindType.SetItemData(i+1, (DWORD_PTR)pBindItem->GetBindItemType());

		if (pBindItem->GetBindItemType() == m_pGridColumn->GetBindTargetType())
			iIndex = i + 1;
	}
	m_ctrlBindType.SetCurSel(iIndex);

	/************************************************************************/	
	// the font info.

	FontParamPtr pHeaderFont = m_pGridColumn->GetColHeaderFont();
	m_logicHeaderFont = pHeaderFont->GetLogicFont();
	m_headerFontColor = pHeaderFont->GetFontColor();

	FontParamPtr pFontParam = m_pGridColumn->GetFontParam();
	m_logicFont = pFontParam->GetLogicFont();
	m_fontColor = pFontParam->GetFontColor();
	/************************************************************************/

	UpdateData(FALSE);

	SetBackgroundColor( SkinManager::GetBackgroundColor() );

	SetWindowTheme( GetDlgItem( IDC_STATIC_ALIGNMENT )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_RADIO_LEFT )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_RADIO_CENTER )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_RADIO_RIGHT )->GetSafeHwnd(), L"", L"" );

	SetWindowTheme( GetDlgItem( IDC_STATIC_COLUMN_HEADER )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_COMBO_BIND_TYPE )->GetSafeHwnd(), L"", L"" );

	return TRUE;
}

BEGIN_MESSAGE_MAP(GridColumnDlg, CDialogEx)
	ON_WM_CTLCOLOR()

	ON_BN_CLICKED(IDC_BTN_FONT, OnFont)
	ON_BN_CLICKED(IDC_BTN_HEADER_FONT, OnHeaderFont)

	ON_BN_CLICKED(IDOK, OnOK)
	ON_BN_CLICKED(IDCANCEL, OnCancel)
END_MESSAGE_MAP()

HBRUSH GridColumnDlg::OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor )
{
	return SkinManager::GetDialogCtrlColor( pDC, pWnd, nCtlColor );
}

void GridColumnDlg::OnFont() 
{
	LOGFONT	logicFont = m_logicFont;
	logicFont.lfHeight = UnitHelper::PointsToPixels(logicFont.lfHeight);

	CFontDialog	dlg(&logicFont);
	dlg.m_cf.rgbColors = m_fontColor;
	dlg.m_cf.Flags |= CF_EFFECTS;
	if (dlg.DoModal() == IDOK)
	{
		dlg.GetCurrentFont(&m_logicFont);
		m_logicFont.lfHeight = dlg.GetSize();
		m_fontColor = dlg.GetColor();
	}
}

void GridColumnDlg::OnHeaderFont() 
{
	LOGFONT	logicFont = m_logicHeaderFont;
	logicFont.lfHeight = UnitHelper::PointsToPixels(logicFont.lfHeight);

	CFontDialog	dlg(&logicFont);
	dlg.m_cf.rgbColors = m_headerFontColor;
	dlg.m_cf.Flags |= CF_EFFECTS;
	if (dlg.DoModal() == IDOK)
	{
		dlg.GetCurrentFont(&m_logicHeaderFont);
		m_logicHeaderFont.lfHeight = dlg.GetSize();
		m_headerFontColor = dlg.GetColor();
	}
}

void GridColumnDlg::OnOK() 
{
	UpdateData(TRUE);

	// check name.
	if (m_strName != m_pGridColumn->GetName())
	{
		m_pGridColumn->SetName(m_strName);
		m_pGridColumn->SetModified(TRUE);
	}

	// check column title.
	if (m_strColumnTitle != m_pGridColumn->GetColumnTitle())
	{
		m_pGridColumn->SetColumnTitle(m_strColumnTitle);
		m_pGridColumn->SetModified(TRUE);
	}

	// check width.
	int iWidth = (int)m_dWidth.Get_Number();
	if (m_unitType == TPL_UNIT_INCH)
		iWidth = UnitHelper::InchesToPixels(m_dWidth.Get_Number());
	else if (m_unitType == TPL_UNIT_CM)
		iWidth = UnitHelper::CentimeterToPixels(m_dWidth.Get_Number());
	if (iWidth != m_pGridColumn->GetWidth())
	{
		m_pGridColumn->SetWidth(iWidth);
		m_pGridColumn->SetModified(TRUE);
	}

	// check text justification style.
	if (m_iJustification != m_pGridColumn->GetTextJustType())
	{
		m_pGridColumn->SetTextJustType((TEXT_JUST_TYPE)m_iJustification);
		m_pGridColumn->SetModified(TRUE);
	}

	// check data binding.
	DWORD_PTR iData = m_ctrlBindType.GetItemData(m_ctrlBindType.GetCurSel());
	if ((BIND_ITEM_TYPE)iData != m_pGridColumn->GetBindTargetType())
	{
		m_pGridColumn->SetBindTargetType((BIND_ITEM_TYPE)iData);
		m_pGridColumn->SetModified(TRUE);
	}

	// check column header font.
	FontParamPtr pHeaderFontParam = m_pGridColumn->GetColHeaderFont();
	if (!pHeaderFontParam->FitLogicFont(m_logicHeaderFont, m_headerFontColor))
	{
		pHeaderFontParam->UpdateFromLogicFont(m_logicHeaderFont, m_headerFontColor);
		m_pGridColumn->SetModified(TRUE);
	}

	// check font param.
	FontParamPtr pFontParam = m_pGridColumn->GetFontParam();
	if (!pFontParam->FitLogicFont(m_logicFont, m_fontColor))
	{
		pFontParam->UpdateFromLogicFont(m_logicFont, m_fontColor);
		m_pGridColumn->SetModified(TRUE);
	}

	CDialogEx::OnOK();
}

void GridColumnDlg::OnCancel()
{
	CDialogEx::OnCancel();
}

void GridColumnDlg::Init(GridColumnPtr pGridColumn, TPL_UNIT_TYPE unitType, BindItemListPtr pAvailBindItemList)
{
	m_pGridColumn = pGridColumn;
	m_unitType = unitType;
	m_pAvailBindItemList = pAvailBindItemList;
}

END_CUTLEADER_NAMESPACE()

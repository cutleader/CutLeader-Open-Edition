#include "stdafx.h"
#include "GridEntityPropDlg.h"

#include "UnitHelper.h"
#include "baseConst.h"

#include "clReportResource.h"
#include "GridColumn.h"
#include "GridColumnList.h"
#include "GridColumnDlg.h"
#include "GridTplEntity.h"
#include "TplEntityMgr.h"
#include "reportConst.h"

#include "BindItem.h"
#include "BindItemList.h"

#include "Uxtheme.h"
#pragma comment(lib, "Uxtheme.lib")

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

GridEntityPropDlg::GridEntityPropDlg()
	: CLDialog(GridEntityPropDlg::IDD)
{
}

GridEntityPropDlg::~GridEntityPropDlg()
{
}

void GridEntityPropDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	DDX_Control(pDX, IDC_EDIT_ROW_HEIGHT, m_dRowHeight);
	DDX_Control(pDX, IDC_EDIT_HEADER_HEIGHT, m_dHeaderHeight);
	DDX_Control(pDX, IDC_LIST_COLUMNS, m_ctrlColumnList);

	// for border.
	DDX_Check(pDX, IDC_CHECK_BORDER, m_bShowBorder);
	DDX_Control(pDX, IDC_EDIT_BORDER_THICKNESS, m_iBorderThickness);
	DDX_Control(pDX, IDC_COMBO_BORDER_STYLE, m_ctrlBorderStyle);
	DDX_Control(pDX, IDC_STATIC_BORDER_COLOR, m_ctrlBorderColor);

	// for column lines.
	DDX_Check(pDX, IDC_CHECK_COLUMNS, m_bShowColumnLine);
	DDX_Control(pDX, IDC_EDIT_COLUMN_THICKNESS, m_iColumnLineThickness);
	DDX_Control(pDX, IDC_COMBO_COLUMN_STYLE, m_ctrlColumnStyle);
	DDX_Control(pDX, IDC_STATIC_COLUMN_COLOR, m_ctrlColumnColor);

	// for row lines.
	DDX_Check(pDX, IDC_CHECK_ROWS, m_bShowRowLine);
	DDX_Control(pDX, IDC_EDIT_ROW_THICKNESS, m_iRowLineThickness);
	DDX_Control(pDX, IDC_COMBO_ROW_STYLE, m_ctrlRowStyle);
	DDX_Control(pDX, IDC_STATIC_ROW_COLOR, m_ctrlRowColor);

	DDX_Control(pDX, IDC_COMBO_BIND_TYPE, m_ctrlBindType);

	DDX_Control(pDX, IDC_BTN_ADD, m_btnAdd);
	DDX_Control(pDX, IDC_BTN_UP, m_btnUp);
	DDX_Control(pDX, IDC_BTN_DOWN, m_btnDown);
	DDX_Control(pDX, IDC_BTN_DELETE, m_btnDel);

	DDX_Control(pDX, IDC_BTN_BORDER_COLOR, m_btnBorderColor);
	DDX_Control(pDX, IDC_BTN_COLUMN_COLOR, m_btnColumnColor);
	DDX_Control(pDX, IDC_BTN_ROW_COLOR, m_btnRowColor);

	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
}

BOOL GridEntityPropDlg::OnInitDialog() 
{
	CLDialog::OnInitDialog();

	/************************************************************************/
	// fill the combo box.

	CString text;

	// the border line style.
	text = MultiLanguageMgr::GetInstance()->TranslateString(IDS_LINE_STYLE_1);
	m_ctrlBorderStyle.SetItemData(m_ctrlBorderStyle.AddString(text), (int)LINE_STYLE_SOLID);
	text = MultiLanguageMgr::GetInstance()->TranslateString(IDS_LINE_STYLE_2);
	m_ctrlBorderStyle.SetItemData(m_ctrlBorderStyle.AddString(text), (int)LINE_STYLE_DASH);
	text = MultiLanguageMgr::GetInstance()->TranslateString(IDS_LINE_STYLE_3);
	m_ctrlBorderStyle.SetItemData(m_ctrlBorderStyle.AddString(text), (int)LINE_STYLE_DOT);

	// the column line style.
	text = MultiLanguageMgr::GetInstance()->TranslateString(IDS_LINE_STYLE_1);
	m_ctrlColumnStyle.SetItemData(m_ctrlColumnStyle.AddString(text), (int)LINE_STYLE_SOLID);
	text = MultiLanguageMgr::GetInstance()->TranslateString(IDS_LINE_STYLE_2);
	m_ctrlColumnStyle.SetItemData(m_ctrlColumnStyle.AddString(text), (int)LINE_STYLE_DASH);
	text = MultiLanguageMgr::GetInstance()->TranslateString(IDS_LINE_STYLE_3);
	m_ctrlColumnStyle.SetItemData(m_ctrlColumnStyle.AddString(text), (int)LINE_STYLE_DOT);

	// the row line style.
	text = MultiLanguageMgr::GetInstance()->TranslateString(IDS_LINE_STYLE_1);
	m_ctrlRowStyle.SetItemData(m_ctrlRowStyle.AddString(text), (int)LINE_STYLE_SOLID);
	text = MultiLanguageMgr::GetInstance()->TranslateString(IDS_LINE_STYLE_2);
	m_ctrlRowStyle.SetItemData( m_ctrlRowStyle.AddString(text), (int)LINE_STYLE_DASH);
	text = MultiLanguageMgr::GetInstance()->TranslateString(IDS_LINE_STYLE_3 );
	m_ctrlRowStyle.SetItemData( m_ctrlRowStyle.AddString(text), (int)LINE_STYLE_DOT);

	// init the data bind info.
	int iIndex = 0;
	m_ctrlBindType.AddString(_T(""));
	m_ctrlBindType.SetItemData(0, (DWORD_PTR)BIND_ITEM_NOT_INIT);
	for (unsigned int i = 0; i < m_pAvailBindItems->size(); i++)
	{
		BindItemPtr pBindItem = m_pAvailBindItems->at(i);
		m_ctrlBindType.AddString(pBindItem->GetBindItemName());
		m_ctrlBindType.SetItemData(i+1, (DWORD_PTR)pBindItem->GetBindItemType());

		if (pBindItem->GetBindItemType() == m_pGridTplEntity->GetBindTargetType())
			iIndex = i + 1;
	}
	m_ctrlBindType.SetCurSel(iIndex);
	/************************************************************************/

	// the grid name.
	m_strName = m_pGridTplEntity->GetName();

	// set the row height.
	if (m_unitType == TPL_UNIT_PIXEL)
		m_dRowHeight.Init(m_pGridTplEntity->GetRowHeight(), 0, FALSE);
	else if (m_unitType == TPL_UNIT_INCH)
		m_dRowHeight.Init(m_pGridTplEntity->GetRowHeight(), TPL_DIGITAL_NUM, FALSE);
	else if (m_unitType == TPL_UNIT_CM)
		m_dRowHeight.Init(m_pGridTplEntity->GetRowHeight(), TPL_DIGITAL_NUM, FALSE);

	// set the header height.
	if (m_unitType == TPL_UNIT_PIXEL)
		m_dHeaderHeight.Init(m_pGridTplEntity->GetHeaderHeight(), 0, FALSE);
	else if (m_unitType == TPL_UNIT_INCH)
		m_dHeaderHeight.Init(m_pGridTplEntity->GetHeaderHeight(), TPL_DIGITAL_NUM, FALSE);
	else if (m_unitType == TPL_UNIT_CM)
		m_dHeaderHeight.Init(m_pGridTplEntity->GetHeaderHeight(), TPL_DIGITAL_NUM, FALSE);

	// for border.
	m_bShowBorder = m_pGridTplEntity->IsShowBorder();
	m_iBorderThickness.Init(m_pGridTplEntity->GetBorderThickness(), 0, FALSE);
	for (int t = 0; t < m_ctrlBorderStyle.GetCount(); t++)
		if (m_ctrlBorderStyle.GetItemData(t) == (DWORD)m_pGridTplEntity->GetBorderStyle())
			m_ctrlBorderStyle.SetCurSel(t);
	m_ctrlBorderColor.SetColor(m_pGridTplEntity->GetBorderColor());

	// for column lines.
	m_bShowColumnLine = m_pGridTplEntity->IsShowColumnLine();
	m_iColumnLineThickness.Init(m_pGridTplEntity->GetColumnLineThickness(), 0, FALSE);
	for (int t = 0; t < m_ctrlColumnStyle.GetCount(); t++)
		if (m_ctrlColumnStyle.GetItemData(t) == (DWORD)m_pGridTplEntity->GetColumnLineStyle())
			m_ctrlColumnStyle.SetCurSel(t);
	m_ctrlColumnColor.SetColor(m_pGridTplEntity->GetColumnLineColor());

	// for row lines.
	m_bShowRowLine = m_pGridTplEntity->IsShowRowLine();
	m_iRowLineThickness.Init(m_pGridTplEntity->GetRowLineThickness(), 0, FALSE);
	for (int t = 0; t < m_ctrlRowStyle.GetCount(); t++)
		if (m_ctrlRowStyle.GetItemData(t) == (DWORD)m_pGridTplEntity->GetRowLineStyle())
			m_ctrlRowStyle.SetCurSel(t);
	m_ctrlRowColor.SetColor(m_pGridTplEntity->GetRowLineColor());

	// init columns.
	m_ctrlColumnList.ResetContent();
	GridColumnListPtr pGridColumnList = m_pGridTplEntity->GetGridColumnList();
	int index;
	for(unsigned int t = 0; t < pGridColumnList->size(); t++)
	{
		GridColumnPtr pGridColumn = pGridColumnList->at(t);

		// add a row.
		text = GetColumnText(pGridColumn);
		index = m_ctrlColumnList.AddString( text );
		m_ctrlColumnList.SetItemData( index, (DWORD_PTR)(new LONGLONG(pGridColumn->GetID())));
	}

	SetBackgroundColor( SkinManager::GetBackgroundColor() );

	SetWindowTheme( GetDlgItem( IDC_STATIC_BASIC_INFO )->GetSafeHwnd(), L"", L"" );
	
	SetWindowTheme( GetDlgItem( IDC_CHECK_BORDER )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_COMBO_BORDER_STYLE )->GetSafeHwnd(), L"", L"" );
	
	SetWindowTheme( GetDlgItem( IDC_CHECK_COLUMNS )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_COMBO_COLUMN_STYLE )->GetSafeHwnd(), L"", L"" );
	
	SetWindowTheme( GetDlgItem( IDC_CHECK_ROWS )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_COMBO_ROW_STYLE )->GetSafeHwnd(), L"", L"" );
	
	SetWindowTheme( GetDlgItem( IDC_COMBO_BIND_TYPE )->GetSafeHwnd(), L"", L"" );

	UpdateData(FALSE);

	return TRUE;
}

BEGIN_MESSAGE_MAP(GridEntityPropDlg, CDialogEx)
	ON_WM_CTLCOLOR()

	ON_BN_CLICKED(IDC_BTN_ADD, OnAdd)
	ON_BN_CLICKED(IDC_BTN_UP, OnUp)
	ON_BN_CLICKED(IDC_BTN_DOWN, OnDown)
	ON_BN_CLICKED(IDC_BTN_DELETE, OnDelete)

	ON_BN_CLICKED(IDC_BTN_COLUMN_COLOR, OnColumnLineColor)
	ON_BN_CLICKED(IDC_BTN_ROW_COLOR, OnRowLineColor)
	ON_BN_CLICKED(IDC_BTN_BORDER_COLOR, OnBorderLineColor)

	ON_BN_CLICKED(IDOK, OnOK)
	ON_BN_CLICKED(IDCANCEL, OnCancel)

	ON_REGISTERED_MESSAGE(rwm_EXLISTBOX_DBLCLICK, OnListboxDblClick)
	ON_REGISTERED_MESSAGE(rwm_EXLISTBOX_DELETE, OnListboxDelete)
END_MESSAGE_MAP()

HBRUSH GridEntityPropDlg::OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor )
{
	return SkinManager::GetDialogCtrlColor( pDC, pWnd, nCtlColor );
}

void GridEntityPropDlg::OnAdd()
{
	GridColumnListPtr pGridColumnList = m_pGridTplEntity->GetGridColumnList();

	// build a new grid column.
	ITplEntityPtr pTplEntity = TplEntityMgr::BuildDflTplEntity(ENTITY_GRID_COLUMN);
	GridColumnPtr pGridColumn = boost::dynamic_pointer_cast<GridColumn>(pTplEntity);

	// get the sub bindable items of this grid.
	BindItemListPtr pBindItemList;
	BIND_ITEM_TYPE bindItemType = m_pGridTplEntity->GetBindTargetType();
	if (bindItemType != BIND_ITEM_NOT_INIT)
	{
		BindItemPtr pBindItem = m_pAvailBindItems->GetItemByType(bindItemType);
		pBindItemList = pBindItem->GetSubBindItemList();
	}
	else
	{
		pBindItemList.reset(new BindItemList);
	}

	GridColumnDlg dlg;
	dlg.Init(pGridColumn, m_unitType, pBindItemList);
	if (dlg.DoModal() == IDOK)
	{
		pGridColumnList->push_back(pGridColumn);
		pGridColumnList->SetModified(TRUE);

		// add to list box.
		CString text = GetColumnText(pGridColumn);
		int iIndex = m_ctrlColumnList.AddString(text);
		m_ctrlColumnList.SetItemData(iIndex, (DWORD_PTR)(new LONGLONG(pGridColumn->GetID())));

		// select current row.
		m_ctrlColumnList.SetCurSel(iIndex);
		m_ctrlColumnList.SetFocus();
	}
}

void GridEntityPropDlg::OnUp()
{
	GridColumnListPtr pGridColumnList = m_pGridTplEntity->GetGridColumnList();

	int index = m_ctrlColumnList.GetCurSel();
	if( index != LB_ERR && index > 0 )
	{
		GridColumnPtr pCurColumn = pGridColumnList->at(index);
		GridColumnPtr pPreColumn = pGridColumnList->at(index-1);

		// swap these two columns.
		iter_swap(pGridColumnList->begin()+index-1, pGridColumnList->begin()+index);

		CString text1;
		CString text2;
		m_ctrlColumnList.GetText( index, text1 );
		m_ctrlColumnList.GetText( index - 1, text2 );

		// delete two rows.
		m_ctrlColumnList.DeleteString( index );
		m_ctrlColumnList.DeleteString( index - 1 );

		// add two rows.
		m_ctrlColumnList.InsertString( index - 1, text1 );
		m_ctrlColumnList.SetItemData( index - 1, (DWORD_PTR)(new LONGLONG(pCurColumn->GetID())) );
		m_ctrlColumnList.InsertString( index, text2 );
		m_ctrlColumnList.SetItemData( index, (DWORD_PTR)(new LONGLONG(pPreColumn->GetID())) );

		// select current row.
		m_ctrlColumnList.SetCurSel( index - 1 );
		m_ctrlColumnList.SetFocus();
	}
}

void GridEntityPropDlg::OnDown() 
{
	GridColumnListPtr pGridColumnList = m_pGridTplEntity->GetGridColumnList();

	int index = m_ctrlColumnList.GetCurSel();
	if( index != LB_ERR && index < m_ctrlColumnList.GetCount() - 1)
	{
		GridColumnPtr pCurColumn = pGridColumnList->at(index);
		GridColumnPtr pNextColumn = pGridColumnList->at(index+1);

		// swap these two columns.
		iter_swap(pGridColumnList->begin()+index, pGridColumnList->begin()+index+1);

		CString text1;
		CString text2;
		m_ctrlColumnList.GetText( index, text1 );
		m_ctrlColumnList.GetText( index + 1, text2 );

		// delete two rows.
		m_ctrlColumnList.DeleteString( index + 1 );
		m_ctrlColumnList.DeleteString( index );

		// add two rows.
		m_ctrlColumnList.InsertString( index, text2 );
		m_ctrlColumnList.SetItemData( index, (DWORD_PTR)(new LONGLONG(pNextColumn->GetID())) );
		m_ctrlColumnList.InsertString( index + 1, text1 );
		m_ctrlColumnList.SetItemData( index + 1, (DWORD_PTR)(new LONGLONG(pCurColumn->GetID())) );

		// select current row.
		m_ctrlColumnList.SetCurSel( index + 1 );
		m_ctrlColumnList.SetFocus();
	}
}

void GridEntityPropDlg::OnDelete()
{
	GridColumnListPtr pGridColumnList = m_pGridTplEntity->GetGridColumnList();

	int iIndex = m_ctrlColumnList.GetCurSel();
	if (iIndex != LB_ERR)
	{
		// update the data.
		LONGLONG* pColumnID = (LONGLONG*)m_ctrlColumnList.GetItemData(iIndex);
		pGridColumnList->DeleteColumn(*pColumnID);
		pGridColumnList->SetModified(TRUE);

		// update the list box.
		m_ctrlColumnList.DeleteString(iIndex);
		m_ctrlColumnList.SetCurSel(iIndex);
		m_ctrlColumnList.SetFocus();
	}
}

void GridEntityPropDlg::OnColumnLineColor()
{
	CColorDialog dlg(m_pGridTplEntity->GetColumnLineColor());
	if (dlg.DoModal() == IDOK)
	{
		COLORREF color = dlg.GetColor();
		m_ctrlColumnColor.SetColor(color);
	}
}

void GridEntityPropDlg::OnRowLineColor()
{
	CColorDialog dlg( m_pGridTplEntity->GetRowLineColor() );
	if( dlg.DoModal() == IDOK )
	{
		COLORREF color = dlg.GetColor();
		m_ctrlRowColor.SetColor( color );
	}
}

void GridEntityPropDlg::OnBorderLineColor()
{
	CColorDialog dlg( m_pGridTplEntity->GetBorderColor() );
	if( dlg.DoModal() == IDOK )
	{
		COLORREF color = dlg.GetColor();
		m_ctrlBorderColor.SetColor( color );
	}
}

void GridEntityPropDlg::OnOK() 
{
	UpdateData(TRUE);

	// check name.
	if (m_strName != m_pGridTplEntity->GetName())
	{
		m_pGridTplEntity->SetName(m_strName);
		m_pGridTplEntity->SetModified(TRUE);
	}

	// update row height.
	int iRowHeight;
	if (m_unitType == TPL_UNIT_PIXEL)
		iRowHeight = (int)m_dRowHeight.Get_Number();
	else if (m_unitType == TPL_UNIT_INCH)
		iRowHeight = UnitHelper::InchesToPixels(m_dRowHeight.Get_Number());
	else if (m_unitType == TPL_UNIT_CM)
		iRowHeight = UnitHelper::CentimeterToPixels(m_dRowHeight.Get_Number());
	if (iRowHeight != m_pGridTplEntity->GetRowHeight())
	{
		m_pGridTplEntity->SetRowHeight(iRowHeight);
		m_pGridTplEntity->SetModified(TRUE);
	}

	// update header height.
	int iHeaderHeight;
	if (m_unitType == TPL_UNIT_PIXEL)
		iHeaderHeight = (int)m_dHeaderHeight.Get_Number();
	else if (m_unitType == TPL_UNIT_INCH)
		iHeaderHeight = UnitHelper::InchesToPixels(m_dHeaderHeight.Get_Number());
	else if (m_unitType == TPL_UNIT_CM)
		iHeaderHeight = UnitHelper::CentimeterToPixels(m_dHeaderHeight.Get_Number());
	if (iHeaderHeight != m_pGridTplEntity->GetHeaderHeight())
	{
		m_pGridTplEntity->SetHeaderHeight(iHeaderHeight);
		m_pGridTplEntity->SetModified(TRUE);
	}

	/************************************************************************/
	// check border setting.

	// whether show border.
	if (m_bShowBorder != m_pGridTplEntity->IsShowBorder())
	{
		m_pGridTplEntity->IsShowBorder(m_bShowBorder);
        m_pGridTplEntity->SetModified(TRUE);
	}

	// border thickness.
	if ((int)m_iBorderThickness.Get_Number() != m_pGridTplEntity->GetBorderThickness())
	{
		m_pGridTplEntity->SetBorderThickness((int)m_iBorderThickness.Get_Number());
		m_pGridTplEntity->SetModified(TRUE);
	}

	// border color.
	if (m_ctrlBorderColor.GetColor() != m_pGridTplEntity->GetBorderColor())
	{
		m_pGridTplEntity->SetBorderColor(m_ctrlBorderColor.GetColor());
		m_pGridTplEntity->SetModified(TRUE);
	}

	// line style.
	LINE_STYLE_TYPE lineStyle = (LINE_STYLE_TYPE)m_ctrlBorderStyle.GetItemData(m_ctrlBorderStyle.GetCurSel());
	if (lineStyle != m_pGridTplEntity->GetBorderStyle())
	{
		m_pGridTplEntity->SetBorderStyle(lineStyle);
		m_pGridTplEntity->SetModified(TRUE);
	}
	/************************************************************************/

	/************************************************************************/
	// check column setting.

	// whether show column line.
	if (m_bShowColumnLine != m_pGridTplEntity->IsShowColumnLine())
	{
		m_pGridTplEntity->IsShowColumnLine(m_bShowColumnLine);
		m_pGridTplEntity->SetModified(TRUE);
	}

	// column line thickness.
	if ((int)m_iColumnLineThickness.Get_Number() != m_pGridTplEntity->GetColumnLineThickness())
	{
		m_pGridTplEntity->SetColumnLineThickness((int)m_iColumnLineThickness.Get_Number());
		m_pGridTplEntity->SetModified(TRUE);
	}

	// column line color.
	if (m_ctrlColumnColor.GetColor() != m_pGridTplEntity->GetColumnLineColor())
	{
		m_pGridTplEntity->SetColumnLineColor(m_ctrlColumnColor.GetColor());
		m_pGridTplEntity->SetModified(TRUE);
	}

	// column line style.
	LINE_STYLE_TYPE colLineStyle = (LINE_STYLE_TYPE)m_ctrlColumnStyle.GetItemData(m_ctrlColumnStyle.GetCurSel());
	if (lineStyle != m_pGridTplEntity->GetColumnLineStyle())
	{
		m_pGridTplEntity->SetColumnLineStyle(colLineStyle);
		m_pGridTplEntity->SetModified(TRUE);
	}
	/************************************************************************/

	/************************************************************************/
	// check row setting.

	// whether show row line.
	if (m_bShowRowLine != m_pGridTplEntity->IsShowRowLine())
	{
		m_pGridTplEntity->IsShowRowLine(m_bShowRowLine);
		m_pGridTplEntity->SetModified(TRUE);
	}

	// row line thickness.
	if ((int)m_iRowLineThickness.Get_Number() != m_pGridTplEntity->GetRowLineThickness())
	{
		m_pGridTplEntity->SetRowLineThickness((int)m_iRowLineThickness.Get_Number());
		m_pGridTplEntity->SetModified(TRUE);
	}

	// row line color.
	if (m_ctrlRowColor.GetColor() != m_pGridTplEntity->GetRowLineColor())
	{
		m_pGridTplEntity->SetRowLineColor(m_ctrlRowColor.GetColor());
		m_pGridTplEntity->SetModified(TRUE);
	}

	// row line style.
	LINE_STYLE_TYPE rowLineStyle = (LINE_STYLE_TYPE)m_ctrlRowStyle.GetItemData(m_ctrlRowStyle.GetCurSel());
	if (lineStyle != m_pGridTplEntity->GetRowLineStyle())
	{
		m_pGridTplEntity->SetRowLineStyle(rowLineStyle);
		m_pGridTplEntity->SetModified(TRUE);
	}
	/************************************************************************/

	// check data binding.
	DWORD_PTR iData = m_ctrlBindType.GetItemData(m_ctrlBindType.GetCurSel());
	if ((BIND_ITEM_TYPE)iData != m_pGridTplEntity->GetBindTargetType())
	{
		m_pGridTplEntity->SetBindTargetType((BIND_ITEM_TYPE)iData);
		m_pGridTplEntity->SetModified(TRUE);

		// if cancel the data binding, clear data binding for all columns.
		if (iData == BIND_ITEM_NOT_INIT)
		{
			GridColumnListPtr pGridColumnList = m_pGridTplEntity->GetGridColumnList();
			for (vector<GridColumnPtr>::iterator iter = pGridColumnList->begin(); iter != pGridColumnList->end(); iter++)
			{
				GridColumnPtr pGridColumn = (GridColumnPtr)(*iter);
				pGridColumn->SetBindTargetType(BIND_ITEM_NOT_INIT);
			}
		}
	}

	CDialogEx::OnOK();
}

void GridEntityPropDlg::OnCancel() 
{
	CDialogEx::OnCancel();
}

LRESULT GridEntityPropDlg::OnListboxDblClick( WPARAM, LPARAM )
{
	GridColumnListPtr pGridColumnList = m_pGridTplEntity->GetGridColumnList();

	int iIndex = m_ctrlColumnList.GetCurSel();
	if (iIndex != LB_ERR)
	{
		LONGLONG* pColumnID = (LONGLONG*)m_ctrlColumnList.GetItemData(iIndex);
		GridColumnPtr pGridColumn = pGridColumnList->GetItemByID(*pColumnID);

		// get the sub bindable items of this grid.
		BindItemListPtr pBindItemList;
		BIND_ITEM_TYPE bindItemType = m_pGridTplEntity->GetBindTargetType();
		if (bindItemType != BIND_ITEM_NOT_INIT)
		{
			BindItemPtr pBindItem = m_pAvailBindItems->GetItemByType(bindItemType);
			pBindItemList = pBindItem->GetSubBindItemList();
		}
		else
		{
			pBindItemList.reset(new BindItemList);
		}

		GridColumnDlg dlg;
		dlg.Init(pGridColumn, m_unitType, pBindItemList);
		if (dlg.DoModal() == IDOK)
			RefillColumnList();

		// select current row.
		m_ctrlColumnList.SetCurSel(iIndex);
		m_ctrlColumnList.SetFocus();
	}

	return 0;
}

LRESULT GridEntityPropDlg::OnListboxDelete( WPARAM, LPARAM )
{
	OnDelete();
	return 0;
}

void GridEntityPropDlg::Init(GridTplEntityPtr pGridTplEntity, TPL_UNIT_TYPE unitType, BindItemListPtr pAvailBindItemList)
{
	m_pGridTplEntity = pGridTplEntity;
	m_unitType = unitType;
	m_pAvailBindItems = pAvailBindItemList;
}

void GridEntityPropDlg::RefillColumnList()
{
	GridColumnListPtr pGridColumnList = m_pGridTplEntity->GetGridColumnList();

	m_ctrlColumnList.ResetContent();

	int index;
	for (unsigned int t = 0 ; t < pGridColumnList->size() ; t++ )
	{
		GridColumnPtr pGridColumn = pGridColumnList->at( t );

		// add a row.
		CString text;
		text = GetColumnText(pGridColumn);
		index = m_ctrlColumnList.AddString( text );
		m_ctrlColumnList.SetItemData( index, (DWORD_PTR)(new LONGLONG(pGridColumn->GetID())));
	}
}

CString GridEntityPropDlg::GetColumnText(GridColumnPtr pColumn)
{
	CString text;
	CString suffix;

	text = pColumn->GetName();

	if (m_unitType == TPL_UNIT_PIXEL)
		suffix.Format( _T( "\t%i" ), pColumn->GetWidth() );
	else if (m_unitType == TPL_UNIT_INCH)
		suffix.Format( _T( "\t%.02f" ), UnitHelper::PixelsToInches( pColumn->GetWidth() ) );
	else if (m_unitType == TPL_UNIT_CM)
		suffix.Format( _T( "\t%.02f" ), UnitHelper::PixelsToCentimeters( pColumn->GetWidth() ) );

	return text + suffix;
}

END_CUTLEADER_NAMESPACE()

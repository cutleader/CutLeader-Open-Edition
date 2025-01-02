#include "stdafx.h"
#include "DynImgEntityPropDlg.h"

#include "baseConst.h"
#include "baseConst.h"
#include "DynImgEntity.h"
#include "clReportResource.h"
#include "BindItemList.h"

#include "Uxtheme.h"
#pragma comment(lib, "Uxtheme.lib")

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(DynImgEntityPropDlg, CDialogEx)

DynImgEntityPropDlg::DynImgEntityPropDlg()
					: CLDialog(DynImgEntityPropDlg::IDD)
{
}

DynImgEntityPropDlg::~DynImgEntityPropDlg(void)
{

}

void DynImgEntityPropDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);

	DDX_Check(pDX, IDC_CHECK_BORDER, m_bBorder);
	DDX_Text(pDX, IDC_EDIT_BORDER_THICKNESS, m_iBorderThickness);
	DDX_Control(pDX, IDC_COMBO_BORDER_STYLE, m_ctrlBorderStyle);
	DDX_Control(pDX, IDC_STATIC_BORDER_COLOR, m_ctrlBorderColor);

	DDX_Control(pDX, IDC_COMBO_BIND_TYPE, m_ctrlBindType);

	DDX_Control(pDX, IDC_BTN_BORDER_COLOR, m_btnBorderColor);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
}

BOOL DynImgEntityPropDlg::OnInitDialog()
{
	CLDialog::OnInitDialog();

	/************************************************************************/
	// init some style.

	CString text;
	text = MultiLanguageMgr::GetInstance()->TranslateString( IDS_LINE_STYLE_1 );
	m_ctrlBorderStyle.SetItemData( m_ctrlBorderStyle.AddString( text ), PS_SOLID );
	text = MultiLanguageMgr::GetInstance()->TranslateString( IDS_LINE_STYLE_2 );
	m_ctrlBorderStyle.SetItemData( m_ctrlBorderStyle.AddString( text ), PS_DASH );
	text = MultiLanguageMgr::GetInstance()->TranslateString( IDS_LINE_STYLE_3 );
	m_ctrlBorderStyle.SetItemData( m_ctrlBorderStyle.AddString( text ), PS_DOT );
	/************************************************************************/

	/************************************************************************/
	// init the dialog content.

	m_strName = m_pDynImgEntity->GetName();

	// about border.
	m_bBorder = m_pDynImgEntity->IsShowBorder();
	m_iBorderThickness = m_pDynImgEntity->GetBorderThickness();
	m_ctrlBorderColor.SetColor( m_pDynImgEntity->GetBorderColor());
	for( int t = 0 ; t < m_ctrlBorderStyle.GetCount() ; t++ )
		if( m_ctrlBorderStyle.GetItemData( t ) == static_cast< DWORD >( m_pDynImgEntity->GetBorderStyle() ) )
			m_ctrlBorderStyle.SetCurSel( t );
	/************************************************************************/

	/************************************************************************/
	// init the data bind info.
	int iIndex = 0;
	m_ctrlBindType.AddString(_T(""));
	m_ctrlBindType.SetItemData(0, (DWORD_PTR)BIND_ITEM_NOT_INIT);
	for (unsigned int i = 0; i < m_pAvailBindItemList->size(); i++)
	{
		BindItemPtr pBindItem = m_pAvailBindItemList->at(i);
		m_ctrlBindType.AddString(pBindItem->GetBindItemName());
		m_ctrlBindType.SetItemData(i+1, (DWORD_PTR)pBindItem->GetBindItemType());

		if (pBindItem->GetBindItemType() == m_pDynImgEntity->GetBindTargetType())
			iIndex = i + 1;
	}
	m_ctrlBindType.SetCurSel(iIndex);
	/************************************************************************/

	UpdateData(FALSE);

	SetBackgroundColor( SkinManager::GetBackgroundColor() );

	SetWindowTheme( GetDlgItem( IDC_CHECK_BORDER )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_COMBO_BORDER_STYLE )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_COMBO_BIND_TYPE )->GetSafeHwnd(), L"", L"" );

	return TRUE;
}

BEGIN_MESSAGE_MAP(DynImgEntityPropDlg, CDialogEx)
	ON_WM_CTLCOLOR()

	ON_BN_CLICKED(IDC_BTN_BORDER_COLOR, OnBorderColor)
	ON_BN_CLICKED(IDOK, OnOK)
	ON_BN_CLICKED(IDCANCEL, OnCancel)

END_MESSAGE_MAP()

HBRUSH DynImgEntityPropDlg::OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor )
{
	return SkinManager::GetDialogCtrlColor( pDC, pWnd, nCtlColor );
}

void DynImgEntityPropDlg::OnBorderColor()
{
	CColorDialog dlg( m_pDynImgEntity->GetBorderColor() );
	if( dlg.DoModal() == IDOK )
	{
		m_ctrlBorderColor.SetColor( dlg.GetColor() );
	}
}

void DynImgEntityPropDlg::OnOK() 
{
	UpdateData(TRUE);

	// the picture name.
	if (m_strName != m_pDynImgEntity->GetName())
	{
		m_pDynImgEntity->SetName(m_strName);
		m_pDynImgEntity->SetModified(TRUE);
	}

	// for border.
	if (m_bBorder != m_pDynImgEntity->IsShowBorder())
	{
		m_pDynImgEntity->IsShowBorder(m_bBorder);
		m_pDynImgEntity->SetModified(TRUE);
	}
	if (m_iBorderThickness != m_pDynImgEntity->GetBorderThickness())
	{
		m_pDynImgEntity->SetBorderThickness(m_iBorderThickness);
		m_pDynImgEntity->SetModified(TRUE);
	}
	LINE_STYLE_TYPE borderLineStyle = (LINE_STYLE_TYPE)m_ctrlBorderStyle.GetItemData(m_ctrlBorderStyle.GetCurSel());
	if (borderLineStyle != m_pDynImgEntity->GetBorderStyle())
	{
		m_pDynImgEntity->SetBorderStyle(borderLineStyle);
		m_pDynImgEntity->SetModified(TRUE);
	}
	if (m_ctrlBorderColor.GetColor() != m_pDynImgEntity->GetBorderColor())
	{
		m_pDynImgEntity->SetBorderColor(m_ctrlBorderColor.GetColor());
		m_pDynImgEntity->SetModified(TRUE);
	}

	// check data binding.
	DWORD_PTR iData = m_ctrlBindType.GetItemData(m_ctrlBindType.GetCurSel());
	if ((BIND_ITEM_TYPE)iData != m_pDynImgEntity->GetBindTargetType())
	{
		m_pDynImgEntity->SetBindTargetType((BIND_ITEM_TYPE)iData);
		m_pDynImgEntity->SetModified(TRUE);
	}

	CDialogEx::OnOK();
}

void DynImgEntityPropDlg::OnCancel() 
{
	CDialogEx::OnCancel();
}

void DynImgEntityPropDlg::Init(DynImgEntityPtr pDynImgEntity, BindItemListPtr pAvailBindItemList) 
{ 
	m_pDynImgEntity = pDynImgEntity;
	m_pAvailBindItemList = pAvailBindItemList;
}

END_CUTLEADER_NAMESPACE()

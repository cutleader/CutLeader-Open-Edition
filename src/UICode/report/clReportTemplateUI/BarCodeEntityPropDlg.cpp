#include "stdafx.h"
#include "BarCodeEntityPropDlg.h"

#include "baseConst.h"
#include "UnitHelper.h"
#include "ExtPropType.h"
#include "ExtPropTypeList.h"

#include "BarCodeTplEntity.h"
#include "BindItem.h"
#include "BindItemList.h"

#include "Uxtheme.h"
#pragma comment(lib, "Uxtheme.lib")

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(BarCodeEntityPropDlg, CDialogEx)

BarCodeEntityPropDlg::BarCodeEntityPropDlg()
					: CLDialog(BarCodeEntityPropDlg::IDD)
{
}

BarCodeEntityPropDlg::~BarCodeEntityPropDlg(void)
{
}

void BarCodeEntityPropDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	DDX_Text(pDX, IDC_EDIT_CONTENT, m_strBarCodeContent);
	DDX_Control(pDX, IDC_COMBO_BARCODE_TYPE, m_ctrlBarCodeType);

	DDX_Radio(pDX, IDC_RADIO_DATA_PROP, m_iDataBind);
	DDX_Control(pDX, IDC_COMBO_BIND_TYPE, m_ctrlBindType);
	DDX_Control(pDX, IDC_COMBO_EXT_PROP, m_ctrlExtProp);

	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
}

BOOL BarCodeEntityPropDlg::OnInitDialog()
{
	CLDialog::OnInitDialog();

	/************************************************************************/
	// 基本信息。

	m_strName = m_pBarCodeTplEnt->GetName();
	m_strBarCodeContent = m_pBarCodeTplEnt->GetBarCodeContent();

	// 类型。
	m_ctrlBarCodeType.AddString(_T("Code 128"));
	m_ctrlBarCodeType.AddString(_T("Code 39"));
	m_ctrlBarCodeType.AddString(_T("Code 93"));
	m_ctrlBarCodeType.AddString(_T("EAN 13"));
	m_ctrlBarCodeType.SetCurSel(m_pBarCodeTplEnt->GetBarCodeType());
	/************************************************************************/

	/************************************************************************/
	// 数据绑定。

	// 绑定的哪一种数据。
	if (m_pBarCodeTplEnt->GetExtPropName() == _T(""))
		m_iDataBind = 0;
	else
		m_iDataBind = 1;

	// init the data bind info.
	int iIndex = 0;
	m_ctrlBindType.AddString(_T(""));
	m_ctrlBindType.SetItemData(0, (DWORD_PTR)BIND_ITEM_NOT_INIT);
	for (unsigned int i = 0; i < m_pAvailBindItemList->size(); i++)
	{
		BindItemPtr pBindItem = m_pAvailBindItemList->at(i);
		m_ctrlBindType.AddString(pBindItem->GetBindItemName());
		m_ctrlBindType.SetItemData(i+1, (DWORD_PTR)pBindItem->GetBindItemType());

		if (pBindItem->GetBindItemType() == m_pBarCodeTplEnt->GetBindTargetType())
			iIndex = i + 1;
	}
	if (m_iDataBind == 0)
		m_ctrlBindType.SetCurSel(iIndex);
	else
		m_ctrlBindType.SetCurSel(0);

	// 扩展属性。
	int iExtPropIndex = 0;
	m_ctrlExtProp.AddString(_T(""));
	for (unsigned int i = 0; i < m_pExtPropTypes->size(); i++)
	{
		ExtPropTypePtr pExtPropType = m_pExtPropTypes->at(i);
		m_ctrlExtProp.AddString(pExtPropType->GetName());
		if (pExtPropType->GetName() == m_pBarCodeTplEnt->GetExtPropName())
			iExtPropIndex = i + 1;
	}
	if (m_iDataBind == 1)
		m_ctrlExtProp.SetCurSel(iExtPropIndex);
	else
		m_ctrlExtProp.SetCurSel(0);
	/************************************************************************/

	UpdateData(FALSE);

	SetBackgroundColor( SkinManager::GetBackgroundColor() );

	SetWindowTheme( GetDlgItem( IDC_STATIC_BASIC_INFO )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_COMBO_BARCODE_TYPE )->GetSafeHwnd(), L"", L"" );
	
	SetWindowTheme( GetDlgItem( IDC_STATIC_DATA_BIND )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_RADIO_DATA_PROP )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_RADIO_EXT_PROP )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_COMBO_BIND_TYPE )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_COMBO_EXT_PROP )->GetSafeHwnd(), L"", L"" );


	return TRUE;
}

BEGIN_MESSAGE_MAP(BarCodeEntityPropDlg, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDOK, OnOK)
	ON_BN_CLICKED(IDCANCEL, OnCancel)
END_MESSAGE_MAP()

HBRUSH BarCodeEntityPropDlg::OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor )
{
	return SkinManager::GetDialogCtrlColor( pDC, pWnd, nCtlColor );
}

void BarCodeEntityPropDlg::OnOK() 
{
	UpdateData(TRUE);

	// check name.
	if (m_strName != m_pBarCodeTplEnt->GetName())
	{
		m_pBarCodeTplEnt->SetName(m_strName);
		m_pBarCodeTplEnt->SetModified(TRUE);
	}

	// 条码类型。
	BARCODE_TYPE iType = (BARCODE_TYPE)m_ctrlBarCodeType.GetCurSel();
	if (iType != m_pBarCodeTplEnt->GetBarCodeType())
	{
		m_pBarCodeTplEnt->SetBarCodeType(iType);
		m_pBarCodeTplEnt->SetModified(TRUE);
	}

	// 条形码内容。
	if (m_strBarCodeContent != m_pBarCodeTplEnt->GetBarCodeContent())
	{
		m_pBarCodeTplEnt->SetBarCodeContent(m_strBarCodeContent);
		m_pBarCodeTplEnt->SetModified(TRUE);
	}

	// check data binding.
	if (m_iDataBind == 0)
	{
		DWORD_PTR iData = m_ctrlBindType.GetItemData(m_ctrlBindType.GetCurSel());
		if ((BIND_ITEM_TYPE)iData != m_pBarCodeTplEnt->GetBindTargetType())
		{
			m_pBarCodeTplEnt->SetBindTargetType((BIND_ITEM_TYPE)iData);
			m_pBarCodeTplEnt->SetModified(TRUE);
		}
	}
	else
	{
		CString strExtProp;
		m_ctrlExtProp.GetLBText(m_ctrlExtProp.GetCurSel(), strExtProp);
		if (strExtProp != m_pBarCodeTplEnt->GetExtPropName())
		{
			m_pBarCodeTplEnt->SetExtPropName(strExtProp);
			m_pBarCodeTplEnt->SetModified(TRUE);
		}
	}

	CDialogEx::OnOK();
}

void BarCodeEntityPropDlg::OnCancel() 
{
	CDialogEx::OnCancel();
}

void BarCodeEntityPropDlg::Init(BarCodeTplEntityPtr pBarCodeTplEnt, BindItemListPtr pAvailBindItemList, ExtPropTypeListPtr pExtPropTypes)
{ 
	m_pBarCodeTplEnt = pBarCodeTplEnt;
	m_pAvailBindItemList = pAvailBindItemList;
	m_pExtPropTypes = pExtPropTypes;
}

END_CUTLEADER_NAMESPACE()

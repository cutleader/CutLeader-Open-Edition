#include "stdafx.h"
#include "MachineInfoPage.h"

#include "DateHelper.h"
#include "baseConst.h"

#include "MachineItem.h"
#include "clMachineLibResource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const size_t MAX_TIP_TEXT_LENGTH = 1024;

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(MachineInfoPage, CDialogEx)

MachineInfoPage::MachineInfoPage() : CLDialog(MachineInfoPage::IDD)
{
}

MachineInfoPage::~MachineInfoPage()
{
}

void MachineInfoPage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_MAC_NAME, m_strMacName);
	DDX_Text(pDX, IDC_EDIT_MAC_COMMENT, m_strMacComment);

	DDX_Text(pDX, IDC_EDIT_CREATE_DATE, m_strCreateDate);
	DDX_Text(pDX, IDC_EDIT_MODIFY_DATE, m_strModifyDate);
}

BEGIN_MESSAGE_MAP(MachineInfoPage, CDialogEx)
	ON_WM_CTLCOLOR()

	ON_EN_KILLFOCUS(IDC_EDIT_MAC_NAME, OnKillMacName)
	ON_EN_KILLFOCUS(IDC_EDIT_MAC_COMMENT, OnKillMacComment)

END_MESSAGE_MAP()

BOOL MachineInfoPage::OnInitDialog()
{
	__super::OnInitDialog();

	SetBackgroundColor( SkinManager::GetBackgroundColor() );
	
	SetWindowTheme( GetDlgItem( IDC_STATIC_BASIC_INFO )->GetSafeHwnd(), L"", L"" );

	UpdateData(FALSE);

	return TRUE;
}

HBRUSH MachineInfoPage::OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor )
{
	return SkinManager::GetDialogCtrlColor( pDC, pWnd, nCtlColor );
}

void MachineInfoPage::OnKillMacName()
{
	UpdateData(TRUE);

	if (m_pMacItem->GetName() != m_strMacName)
	{
		m_pMacItem->SetName(m_strMacName);
		m_pMacItem->SetModified(TRUE);
	}
}

void MachineInfoPage::OnKillMacComment()
{
	UpdateData(TRUE);

	if (m_pMacItem->GetComment() != m_strMacComment)
	{
		m_pMacItem->SetComment(m_strMacComment);
		m_pMacItem->SetModified(TRUE);
	}
}

void MachineInfoPage::DisplayMac(MachineItemPtr pMacItem)
{
	m_pMacItem = pMacItem;

	m_strMacName = m_pMacItem->GetName();
	m_strMacComment = m_pMacItem->GetComment();

	m_strCreateDate = DateHelper::GetStrFromTimeStamp(m_pMacItem->GetCreateTime());
	m_strModifyDate = DateHelper::GetStrFromTimeStamp(m_pMacItem->GetModifyTime());

	UpdateData(FALSE);
}

void MachineInfoPage::UpdateModifyDate()
{
	m_strModifyDate = DateHelper::GetStrFromTimeStamp(m_pMacItem->GetModifyTime());
	UpdateData(FALSE);
}

END_CUTLEADER_NAMESPACE()

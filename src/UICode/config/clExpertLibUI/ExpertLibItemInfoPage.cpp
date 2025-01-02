#include "stdafx.h"
#include "ExpertLibItemInfoPage.h"

#include "baseConst.h"
#include "DateHelper.h"
#include "clBaseDataResource.h"
#include "DataBaseManager.h"

#include "MachineItem.h"

#include "ExpertLibItem.h"
#include "clExpertLibResource.h"

#include "Uxtheme.h"
#pragma comment(lib, "Uxtheme.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const size_t MAX_TIP_TEXT_LENGTH = 1024;

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(ExpertLibItemInfoPage, CDialogEx)

ExpertLibItemInfoPage::ExpertLibItemInfoPage() : CLDialog(ExpertLibItemInfoPage::IDD)
{
}

ExpertLibItemInfoPage::~ExpertLibItemInfoPage()
{
}

void ExpertLibItemInfoPage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_EXPLIBITEM_NAME, m_strExpLibItemName);
	DDX_Text(pDX, IDC_EDIT_EXPLIBITEM_COMMENT, m_strExpLibItemComment);
	DDX_Text(pDX, IDC_EDIT_CREATE_DATE, m_strCreateDate);
	DDX_Text(pDX, IDC_EDIT_MODIFY_DATE, m_strModifyDate);

	DDX_Text(pDX, IDC_EDIT_MAC_NAME, m_macName);
	DDX_Text(pDX, IDC_EDIT_MAC_COMMENT, m_macComment);
}

BEGIN_MESSAGE_MAP(ExpertLibItemInfoPage, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_EN_KILLFOCUS(IDC_EDIT_EXPLIBITEM_NAME, OnKillExpLibItemName)
	ON_EN_KILLFOCUS(IDC_EDIT_EXPLIBITEM_COMMENT, OnKillExpLibItemComment)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

BOOL ExpertLibItemInfoPage::OnInitDialog()
{
	__super::OnInitDialog();

	SetBackgroundColor( SkinManager::GetBackgroundColor() );

	SetWindowTheme( GetDlgItem( IDC_STATIC_BASIC_INFO )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_STATIC_MACHINE_INFO )->GetSafeHwnd(), L"", L"" );

	UpdateData(FALSE);

	return TRUE;
}

HBRUSH ExpertLibItemInfoPage::OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor )
{
	return SkinManager::GetDialogCtrlColor( pDC, pWnd, nCtlColor );
}

void ExpertLibItemInfoPage::OnKillExpLibItemName()
{
	UpdateData(TRUE);

	if (m_pExpLibItem->GetName() != m_strExpLibItemName)
	{
		m_pExpLibItem->SetName(m_strExpLibItemName);
		m_pExpLibItem->SetModified(TRUE);
	}
}

void ExpertLibItemInfoPage::OnKillExpLibItemComment()
{
	UpdateData(TRUE);

	if (m_pExpLibItem->GetComment() != m_strExpLibItemComment)
	{
		m_pExpLibItem->SetComment(m_strExpLibItemComment);
		m_pExpLibItem->SetModified(TRUE);
	}
}

void ExpertLibItemInfoPage::DisplayExpLibItem(ExpertLibItemPtr pExpLibItem)
{
	m_pExpLibItem = pExpLibItem;

	m_strExpLibItemName = pExpLibItem->GetName();
	m_strExpLibItemComment = pExpLibItem->GetComment();
	m_strCreateDate = DateHelper::GetStrFromTimeStamp(pExpLibItem->GetCreateTime());
	m_strModifyDate = DateHelper::GetStrFromTimeStamp(pExpLibItem->GetModifyTime());

	MachineItemPtr pMacItem = m_pExpLibItem->GetMacItem();
	m_macName = pMacItem->GetName();
	m_macComment = pMacItem->GetComment();

	UpdateData(FALSE);
}

void ExpertLibItemInfoPage::UpdateModifyDate()
{
	m_strModifyDate = DateHelper::GetStrFromTimeStamp(m_pExpLibItem->GetModifyTime());
	UpdateData(FALSE);
}

END_CUTLEADER_NAMESPACE()

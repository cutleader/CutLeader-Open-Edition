#include "StdAfx.h"
#include "ChangeLoopToolActionBar.h"

#include "baseConst.h"
#include "PartPlacementEditAction.h"

#include "Uxtheme.h"
#pragma comment(lib, "Uxtheme.lib")

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(ChangeLoopToolActionBar, ActionBar)

ChangeLoopToolActionBar::ChangeLoopToolActionBar(CWnd* pParent)
	: ActionBar(ChangeLoopToolActionBar::IDD, pParent)
{
}

ChangeLoopToolActionBar::~ChangeLoopToolActionBar(void)
{
}

void ChangeLoopToolActionBar::DoDataExchange(CDataExchange* pDX)
{
	ActionBar::DoDataExchange(pDX);

    DDX_Check(pDX, IDC_CHECK_SelectPart, m_bSelectPart);
	DDX_Check(pDX, IDC_CHECK_SPREAD, m_bSpread);
}

BOOL ChangeLoopToolActionBar::OnInitDialog()
{
	__super::OnInitDialog();

    m_bSelectPart = FALSE;
	m_bSpread = FALSE;
	UpdateData(FALSE);

	SetBackgroundColor( SkinManager::GetBackgroundColor() );

	SetWindowTheme( GetDlgItem( IDC_STATIC_OPTION )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_CHECK_SelectPart )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_CHECK_SPREAD )->GetSafeHwnd(), L"", L"" );
	return TRUE;
}

void ChangeLoopToolActionBar::PostNcDestroy()
{
	ActionBar::PostNcDestroy();
}

BEGIN_MESSAGE_MAP(ChangeLoopToolActionBar, ActionBar)
    ON_BN_CLICKED(IDC_CHECK_SelectPart, OnCheckSelectPart)
	ON_BN_CLICKED(IDC_CHECK_SPREAD, OnCheckSpread)
END_MESSAGE_MAP()

void ChangeLoopToolActionBar::OnCheckSelectPart()
{
    UpdateData(TRUE);
}

void ChangeLoopToolActionBar::OnCheckSpread()
{
	UpdateData(TRUE);
}

void ChangeLoopToolActionBar::ShowChangeLoopToolActionBar()
{
	PositionActionBar();
}

END_CUTLEADER_NAMESPACE()

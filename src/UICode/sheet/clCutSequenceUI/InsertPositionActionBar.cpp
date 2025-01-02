#include "StdAfx.h"
#include "InsertPositionActionBar.h"

#include "baseConst.h"
#include "PartPlacementEditAction.h"
#include "clPartLayoutResource.h"

#include "Uxtheme.h"
#pragma comment(lib, "Uxtheme.lib")

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(InsertPositionActionBar, ActionBar)

InsertPositionActionBar::InsertPositionActionBar(CWnd* pParent)
	: ActionBar(InsertPositionActionBar::IDD, pParent)
{
}

InsertPositionActionBar::~InsertPositionActionBar(void)
{
}

void InsertPositionActionBar::DoDataExchange(CDataExchange* pDX)
{
	ActionBar::DoDataExchange(pDX);

	DDX_Check(pDX, IDC_CHECK_BEHIND, m_bInsertBehind);
}

BOOL InsertPositionActionBar::OnInitDialog()
{
	__super::OnInitDialog();

	m_bInsertBehind = TRUE;
	UpdateData(FALSE);

	SetBackgroundColor( SkinManager::GetBackgroundColor() );
	SetWindowTheme( GetDlgItem( IDC_STATIC_OPTION )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_CHECK_BEHIND )->GetSafeHwnd(), L"", L"" );

	return TRUE;
}

void InsertPositionActionBar::PostNcDestroy()
{
	ActionBar::PostNcDestroy();
	delete this;
}

BEGIN_MESSAGE_MAP(InsertPositionActionBar, ActionBar)
	ON_BN_CLICKED(IDC_CHECK_BEHIND, OnCheckBehind)
END_MESSAGE_MAP()

void InsertPositionActionBar::OnCheckBehind()
{
	UpdateData(TRUE);
}

void InsertPositionActionBar::Show_InsertPositionActionBar()
{
	PositionActionBar();
}

END_CUTLEADER_NAMESPACE()

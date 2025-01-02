#include "StdAfx.h"
#include "SpreadSamePartActionBar.h"

#include "ClWindowFinder.h"

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(SpreadSamePartActionBar, ActionBar)

SpreadSamePartActionBar::SpreadSamePartActionBar(CWnd* pParent)
	: ActionBar(SpreadSamePartActionBar::IDD, pParent)
{
}

SpreadSamePartActionBar::~SpreadSamePartActionBar(void)
{
}

void SpreadSamePartActionBar::DoDataExchange(CDataExchange* pDX)
{
	ActionBar::DoDataExchange(pDX);

	DDX_Check(pDX, IDC_CHECK_SPREAD, m_bSpread);
}

BOOL SpreadSamePartActionBar::OnInitDialog()
{
	__super::OnInitDialog();

	m_bSpread = FALSE;
	UpdateData(FALSE);

	SetBackgroundColor( SkinManager::GetBackgroundColor() );

	SetWindowTheme( GetDlgItem( IDC_STATIC_OPTION )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_CHECK_SPREAD )->GetSafeHwnd(), L"", L"" );

	return TRUE;
}

void SpreadSamePartActionBar::PostNcDestroy()
{
	ActionBar::PostNcDestroy();
}

BEGIN_MESSAGE_MAP(SpreadSamePartActionBar, ActionBar)
	ON_BN_CLICKED(IDC_CHECK_SPREAD, OnCheckSpread)
END_MESSAGE_MAP()

void SpreadSamePartActionBar::OnCheckSpread()
{
	UpdateData(TRUE);
    NotifyAllObserver(INVALID_ID, CComVariant()); // 通知所有监听者
}

void SpreadSamePartActionBar::Show_SpreadSamePartActionBar()
{
	this->PositionActionBar();
}

END_CUTLEADER_NAMESPACE()

#include "StdAfx.h"
#include "ApplyPartActionBar.h"

#include "baseConst.h"
#include "GlViewPort.h"

#include "clPartLayoutResource.h"
#include "PartPlacementEditAction.h"

#include "Uxtheme.h"
#pragma comment(lib, "Uxtheme.lib")

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(ApplyPartActionBar, ActionBar)

ApplyPartActionBar::ApplyPartActionBar(CWnd* pParent)
	: ActionBar(ApplyPartActionBar::IDD, pParent)
{
}

ApplyPartActionBar::~ApplyPartActionBar(void)
{
}

void ApplyPartActionBar::DoDataExchange(CDataExchange* pDX)
{
	ActionBar::DoDataExchange(pDX);

	DDX_Check(pDX, IDC_CHECK_PART, m_bApplyPart);
}

BOOL ApplyPartActionBar::OnInitDialog()
{
	__super::OnInitDialog();

	m_bApplyPart = TRUE;
	UpdateData(FALSE);

	SetBackgroundColor( SkinManager::GetBackgroundColor() );
	SetWindowTheme( GetDlgItem( IDC_STATIC_OPTION )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_CHECK_PART )->GetSafeHwnd(), L"", L"" );

	return TRUE;
}

void ApplyPartActionBar::PostNcDestroy()
{
	ActionBar::PostNcDestroy();
	delete this;
}

BEGIN_MESSAGE_MAP(ApplyPartActionBar, ActionBar)
	ON_BN_CLICKED(IDC_CHECK_PART, OnCheckPart)
END_MESSAGE_MAP()

void ApplyPartActionBar::OnCheckPart()
{
	UpdateData(TRUE);
}

void ApplyPartActionBar::Show_ApplyPartActionBar()
{
	PositionActionBar();
}

END_CUTLEADER_NAMESPACE()

#include "StdAfx.h"
#include "SelectPartOrLoopActionBar.h"

#include "baseConst.h"
#include "ClWindowFinder.h"

#include "clPartLayoutResource.h"
#include "PartPlacementEditAction.h"

#include "Uxtheme.h"
#pragma comment(lib, "Uxtheme.lib")

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(SelectPartOrLoopActionBar, ActionBar)

SelectPartOrLoopActionBar::SelectPartOrLoopActionBar(CWnd* pParent /*=NULL*/)
	: ActionBar(SelectPartOrLoopActionBar::IDD, pParent)
{
	m_iOption = 0;
    m_bAutoSelectSamePartsOrLoops = FALSE;
    m_bHideCutSequence = FALSE;
}

SelectPartOrLoopActionBar::~SelectPartOrLoopActionBar(void)
{
}

void SelectPartOrLoopActionBar::DoDataExchange(CDataExchange* pDX)
{
	ActionBar::DoDataExchange(pDX);

	DDX_Radio(pDX, IDC_RADIO_SelectPart, m_iOption);
    DDX_Check(pDX, IDC_CHECK_AutoSelectSamePartsOrLoops, m_bAutoSelectSamePartsOrLoops);
    DDX_Check(pDX, IDC_CHECK_HideCutSequence, m_bHideCutSequence);
}

BOOL SelectPartOrLoopActionBar::OnInitDialog()
{
	__super::OnInitDialog();

    UpdateData(FALSE);

	SetBackgroundColor( SkinManager::GetBackgroundColor() );

	SetWindowTheme( GetDlgItem( IDC_STATIC_OPTION )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem(IDC_RADIO_SelectPart)->GetSafeHwnd(), L"", L"" );
	SetWindowTheme(GetDlgItem(IDC_RADIO_SelectLoop)->GetSafeHwnd(), L"", L"");
	SetWindowTheme( GetDlgItem( IDC_CHECK_AutoSelectSamePartsOrLoops )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_CHECK_HideCutSequence )->GetSafeHwnd(), L"", L"" );
	return TRUE;
}

void SelectPartOrLoopActionBar::PostNcDestroy()
{
	ActionBar::PostNcDestroy();
	delete this;
}

BEGIN_MESSAGE_MAP(SelectPartOrLoopActionBar, ActionBar)
    ON_BN_CLICKED(IDC_RADIO_SelectPart, OnSelectPart)
	ON_BN_CLICKED(IDC_RADIO_SelectLoop, OnSelectLoop)
    ON_BN_CLICKED(IDC_CHECK_AutoSelectSamePartsOrLoops, OnAutoSelectSamePartsOrLoops)
    ON_BN_CLICKED(IDC_CHECK_HideCutSequence, OnHideCutSequence)
END_MESSAGE_MAP()

void SelectPartOrLoopActionBar::OnSelectPart()
{
	UpdateData(TRUE);
	PartPlacementEditAction::PostMessage2EnterPartPlacementSelectAction();
}

void SelectPartOrLoopActionBar::OnSelectLoop()
{
    UpdateData(TRUE);
	PartPlacementEditAction::PostMessage2EnterLoopInstanceSelectAction();
}

void SelectPartOrLoopActionBar::OnAutoSelectSamePartsOrLoops()
{
    UpdateData(TRUE);
}

void SelectPartOrLoopActionBar::OnHideCutSequence()
{
	UpdateData(TRUE);

	if (m_bHideCutSequence) {
        ClWindowFinder::GetCurrentView_1()->PostMessage(WM_HideOrShowCutPath, 0, 0);
    }
	else {
		ClWindowFinder::GetCurrentView_1()->PostMessage(WM_HideOrShowCutPath, 1, 0);
    }
}

void SelectPartOrLoopActionBar::Show_SelectPartOrLoopActionBar(bool bSelectPart)
{
	PositionActionBar();

	if (bSelectPart) {
		m_iOption = 0;
	}
	else {
		m_iOption = 1;
	}
	UpdateData(FALSE);
}

END_CUTLEADER_NAMESPACE()

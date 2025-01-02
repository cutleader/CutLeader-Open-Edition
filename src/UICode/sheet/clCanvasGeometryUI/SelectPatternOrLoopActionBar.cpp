#include "StdAfx.h"
#include "SelectPatternOrLoopActionBar.h"

#include "ClWindowFinder.h"
#include "LogMgr.h"

#include "clPartLayoutResource.h"

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(SelectPatternOrLoopActionBar, ActionBar)

SelectPatternOrLoopActionBar::SelectPatternOrLoopActionBar(CWnd* pParent /*=NULL*/)
	: ActionBar(SelectPatternOrLoopActionBar::IDD, pParent)
{
    m_iOption = 0;
    m_bHideCutSequence = FALSE;
}

SelectPatternOrLoopActionBar::~SelectPatternOrLoopActionBar(void)
{
}

void SelectPatternOrLoopActionBar::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

    DDX_Radio(pDX, IDC_RADIO_SelectLoop, m_iOption);
    DDX_Check(pDX, IDC_CHECK_HideCutSequence, m_bHideCutSequence);
}

BOOL SelectPatternOrLoopActionBar::OnInitDialog()
{
	__super::OnInitDialog();

    UpdateData(FALSE);

	SetBackgroundColor( SkinManager::GetBackgroundColor() );

	SetWindowTheme( GetDlgItem( IDC_STATIC_OPTION )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem(IDC_RADIO_SelectPattern)->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem(IDC_RADIO_SelectLoop)->GetSafeHwnd(), L"", L"" );
    SetWindowTheme(GetDlgItem(IDC_CHECK_HideCutSequence)->GetSafeHwnd(), L"", L"");
	return TRUE;
}

void SelectPatternOrLoopActionBar::PostNcDestroy()
{
	ActionBar::PostNcDestroy();
	delete this;
}

BEGIN_MESSAGE_MAP(SelectPatternOrLoopActionBar, ActionBar)
    ON_BN_CLICKED(IDC_RADIO_SelectPattern, OnSelectPattern)
    ON_BN_CLICKED(IDC_RADIO_SelectLoop, OnSelectPatternLoop)
    ON_BN_CLICKED(IDC_CHECK_HideCutSequence, OnHideCutSequence)
END_MESSAGE_MAP()

void SelectPatternOrLoopActionBar::OnSelectPattern()
{
    UpdateData(TRUE);
}

void SelectPatternOrLoopActionBar::OnSelectPatternLoop()
{
    UpdateData(TRUE);
}

void SelectPatternOrLoopActionBar::OnHideCutSequence()
{
	UpdateData(TRUE);

	if (m_bHideCutSequence)
		ClWindowFinder::GetCurrentView_1()->PostMessage(WM_HideOrShowCutPath, 0, 0);
	else
		ClWindowFinder::GetCurrentView_1()->PostMessage(WM_HideOrShowCutPath, 1, 0);
}

void SelectPatternOrLoopActionBar::DisplayActionBar(LoopOrPatternSelectMode emLoopOrPatternSelectMode)
{
	PositionActionBar();

	// 设置图形选择模式。
	if (emLoopOrPatternSelectMode != OnlyRepositionBar) {
		InitSelectMode(emLoopOrPatternSelectMode);
	}
}

void SelectPatternOrLoopActionBar::InitSelectMode(LoopOrPatternSelectMode emLoopOrPatternSelectMode)
{
	if (emLoopOrPatternSelectMode == OnlyEnableLoopSelect) // 只能选择轮廓。
	{
		if (GetDlgItem(IDC_RADIO_SelectPattern)->IsWindowEnabled())
			GetDlgItem(IDC_RADIO_SelectPattern)->EnableWindow(FALSE);
		if (!GetDlgItem(IDC_RADIO_SelectLoop)->IsWindowEnabled())
			GetDlgItem(IDC_RADIO_SelectLoop)->EnableWindow(TRUE);

		if (m_iOption != 0) {
			m_iOption = 0;
			UpdateData(FALSE);
		}
	}
	else if (emLoopOrPatternSelectMode == OnlyEnablePatternSelect) // 只能选择图元。
	{
		if (!GetDlgItem(IDC_RADIO_SelectPattern)->IsWindowEnabled())
			GetDlgItem(IDC_RADIO_SelectPattern)->EnableWindow(TRUE);
		if (GetDlgItem(IDC_RADIO_SelectLoop)->IsWindowEnabled())
			GetDlgItem(IDC_RADIO_SelectLoop)->EnableWindow(FALSE);

		if (m_iOption != 1) {
			m_iOption = 1;
			UpdateData(FALSE);
		}
	}
	else if (emLoopOrPatternSelectMode == SelectLoop) // 当前进行轮廓选择，不过可以切换到图元选择。
	{
		if (!GetDlgItem(IDC_RADIO_SelectPattern)->IsWindowEnabled())
			GetDlgItem(IDC_RADIO_SelectPattern)->EnableWindow(TRUE);
		if (!GetDlgItem(IDC_RADIO_SelectLoop)->IsWindowEnabled())
			GetDlgItem(IDC_RADIO_SelectLoop)->EnableWindow(TRUE);

		if (m_iOption != 0) {
			m_iOption = 0;
			UpdateData(FALSE);
		}
	}
	else if (emLoopOrPatternSelectMode == SelectPattern) // 当前进行图元选择，不过可以切换到轮廓选择。
	{
		if (!GetDlgItem(IDC_RADIO_SelectPattern)->IsWindowEnabled())
			GetDlgItem(IDC_RADIO_SelectPattern)->EnableWindow(TRUE);
		if (!GetDlgItem(IDC_RADIO_SelectLoop)->IsWindowEnabled())
			GetDlgItem(IDC_RADIO_SelectLoop)->EnableWindow(TRUE);

		if (m_iOption != 1) {
			m_iOption = 1;
			UpdateData(FALSE);
		}
	}
}

END_CUTLEADER_NAMESPACE()

#include "stdafx.h"
#include "ActionBar.h"

#include "ClWindowFinder.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(ActionBar, CLDialog)

ActionBar::ActionBar()
{
}

ActionBar::ActionBar(UINT nIDTemplate, CWnd* pParentWnd)
	: CLDialog(nIDTemplate, pParentWnd)
{
}

ActionBar::~ActionBar()
{
}

BEGIN_MESSAGE_MAP(ActionBar, CLDialog)
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
END_MESSAGE_MAP()

HBRUSH ActionBar::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	return SkinManager::GetDialogCtrlColor(pDC, pWnd, nCtlColor);
}

void ActionBar::OnPaint()
{
	// 这里加个保护，防止action bar在视图之前构造时出问题。
	CView* pCurrentView = ClWindowFinder::GetCurrentView_1_WithoutDebugWarn();
	if (!pCurrentView) {
		return;
	}

	if (this->IsWindowVisible()) {
		CRect parentRect, winRect;
		pCurrentView->GetWindowRect(parentRect);
		this->GetWindowRect(winRect);
		this->MoveWindow(parentRect.Width() - winRect.Width() - 2, 2, winRect.Width(), winRect.Height());
	}
	__super::OnPaint();
}

void ActionBar::PositionActionBar()
{
	CRect parentRect, winRect;
	ClWindowFinder::GetCurrentView_1()->GetWindowRect(parentRect);
	this->GetWindowRect(winRect);
	this->MoveWindow(parentRect.Width() - winRect.Width() - 2, 2, winRect.Width(), winRect.Height());

	if (!this->IsWindowVisible()) {
		this->SetParent(ClWindowFinder::GetCurrentView_1());
		this->ShowWindow(SW_SHOW);
	}
}

void ActionBar::HideActionBar()
{
	if (this->IsWindowVisible()) {
		this->ShowWindow(SW_HIDE);
	}
}

END_CUTLEADER_NAMESPACE()

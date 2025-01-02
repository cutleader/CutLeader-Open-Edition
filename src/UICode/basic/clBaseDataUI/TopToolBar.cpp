#include "stdafx.h"
#include "TopToolBar.h"

#include "SubToolBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(TopToolBar, CLDialog)

TopToolBar::TopToolBar()
{
}

TopToolBar::TopToolBar(UINT nIDTemplate, CWnd* pParentWnd)
	: CLDialog(nIDTemplate, pParentWnd)
{
}

TopToolBar::~TopToolBar()
{
}

BEGIN_MESSAGE_MAP(TopToolBar, CLDialog)
	ON_WM_CTLCOLOR()
	ON_WM_VSCROLL()
	ON_WM_SIZE()
END_MESSAGE_MAP()

HBRUSH TopToolBar::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	return SkinManager::GetDialogCtrlColor(pDC, pWnd, nCtlColor);
}

void TopToolBar::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int nDelta;
	int nMaxPos = m_rcOriginalRect.Height() - m_nCurHeight;

	switch (nSBCode)
	{
	case SB_LINEDOWN:
		if (m_nScrollPos >= nMaxPos)
			return;
		nDelta = min(max(nMaxPos / 20, 5), nMaxPos - m_nScrollPos);
		break;
	case SB_LINEUP:
		if (m_nScrollPos <= 0)
			return;
		nDelta = -min(max(nMaxPos / 20, 5), m_nScrollPos);
		break;
	case SB_PAGEDOWN:
		if (m_nScrollPos >= nMaxPos)
			return;
		nDelta = min(max(nMaxPos / 10, 5), nMaxPos - m_nScrollPos);
		break;
	case SB_THUMBTRACK:
	case SB_THUMBPOSITION:
		nDelta = (int)nPos - m_nScrollPos;
		break;
	case SB_PAGEUP:
		if (m_nScrollPos <= 0)
			return;
		nDelta = -min(max(nMaxPos / 10, 5), m_nScrollPos);
		break;

	default:
		return;
	}
	m_nScrollPos += nDelta;
	SetScrollPos(SB_VERT, m_nScrollPos, TRUE);
	ScrollWindow(0, -nDelta);

	CLDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void TopToolBar::OnSize(UINT nType, int cx, int cy)
{
	m_nCurHeight = cy;

	SCROLLINFO si;
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_ALL;
	si.nMin = 0;
	si.nMax = m_rcOriginalRect.Height();
	si.nPage = cy;
	si.nPos = m_nScrollPos;
	SetScrollInfo(SB_VERT, &si, TRUE);

	CLDialog::OnSize(nType, cx, cy);
}

void TopToolBar::CheckWhetherDisplaySubToolBar(const SkinMFCButton* pButton, SubToolBar* pSubToolBar)
{
	POINT point;
	GetCursorPos(&point);

	CRect buttonRect, subDialogRect;
	pButton->GetWindowRect(&buttonRect);
	pSubToolBar->GetWindowRect(subDialogRect);

	if (buttonRect.PtInRect(point)) // 鼠标在按钮上。
	{
		// 调整工具条位置，以主窗口客户区的左上角为原点。
		AfxGetApp()->GetMainWnd()->ScreenToClient(&buttonRect);
		pSubToolBar->MoveWindow(buttonRect.right, buttonRect.top, subDialogRect.Width(), subDialogRect.Height());

		if (!pSubToolBar->IsWindowVisible()) {
			pSubToolBar->SetParent(AfxGetApp()->GetMainWnd());
			pSubToolBar->ShowWindow(SW_SHOW);
		}
	}
	else { // 鼠标不在按钮上。
		if (subDialogRect.PtInRect(point)) { // 光标在子工具条上，此时不做任何处理。
			;
		}
		else { // 鼠标不在按钮上也不在子工具条上，需要隐藏子工具条。
			if (pSubToolBar->IsWindowVisible()) {
				pSubToolBar->ShowWindow(SW_HIDE);
			}
		}
	}
}

END_CUTLEADER_NAMESPACE()

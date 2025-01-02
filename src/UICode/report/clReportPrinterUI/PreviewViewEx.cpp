#include "StdAfx.h"
#include "PreviewViewEx.h"

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(PreviewViewEx, CPreviewView)

PreviewViewEx::PreviewViewEx(void)
{
	m_pPrintWnd = NULL;
}

PreviewViewEx::~PreviewViewEx(void)
{
}

BEGIN_MESSAGE_MAP(PreviewViewEx, CPreviewView)
	ON_UPDATE_COMMAND_UI(AFX_ID_PREVIEW_NUMPAGE, OnUpdateNumPageChange)
	ON_COMMAND(AFX_ID_PREVIEW_PRINT, OnPreviewPrint)
END_MESSAGE_MAP()

void PreviewViewEx::OnDraw(CDC* pDC)
{
	CPreviewView::OnDraw(pDC);
	m_pToolBar->PostMessage(WM_IDLEUPDATECMDUI,(WPARAM) TRUE);
}

void PreviewViewEx::OnUpdateNumPageChange(CCmdUI* pCmdUI)
{
	CPreviewView::OnUpdateNumPageChange(pCmdUI);

	// set text of button to opposite of current state
	UINT nPages = (m_nZoomState == ZOOM_OUT ? m_nPages : m_nZoomOutPages);
	CString strTwoPage(MAKEINTRESOURCE(AFX_IDS_TWOPAGE));
	CString strOnePage(MAKEINTRESOURCE(AFX_IDS_ONEPAGE));
	pCmdUI->SetText(nPages == 1 ? strTwoPage : strOnePage);
}

void PreviewViewEx::OnPreviewPrint()
{
	if (m_pPrintWnd != NULL)
	{
		m_pPrintWnd->PrintData();
	}
	else
	{
		// the view created for the dialog needs to set the m_pPrintWnd pointer
		ASSERT(FALSE);
	}

	// force close of Preview
	OnPreviewClose();
}

END_CUTLEADER_NAMESPACE()

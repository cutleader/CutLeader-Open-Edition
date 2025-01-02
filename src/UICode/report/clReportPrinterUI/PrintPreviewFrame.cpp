#include "StdAfx.h"
#include "PrintPreviewFrame.h"

#include "WorkingView.h"
#include "PreviewViewEx.h"

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(PrintPreviewFrame, CFrameWnd);

PrintPreviewFrame::PrintPreviewFrame(void)
{
}

BEGIN_MESSAGE_MAP(PrintPreviewFrame, CFrameWnd)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

void PrintPreviewFrame::OnClose()
{
	PreviewViewEx* pPreviewView = (PreviewViewEx*)GetActiveView();
	WorkingView* pWorkingView = (WorkingView*)pPreviewView->GetOrigView();
	pWorkingView->IsDestroyParentFrm(FALSE);

	CFrameWnd::OnClose();
}

END_CUTLEADER_NAMESPACE()

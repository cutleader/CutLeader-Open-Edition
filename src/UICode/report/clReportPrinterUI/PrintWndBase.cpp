#include "StdAfx.h"
#include "PrintWndBase.h"

#include "taoResource.h"
#include "WorkingView.h"
#include "PrintPreviewFrame.h"

BEGIN_CUTLEADER_NAMESPACE()

PrintWndBase::PrintWndBase(void)
{
	m_pTemplate = NULL;
}

PrintWndBase::~PrintWndBase(void)
{
}

void PrintWndBase::PreparePrinting(CPrintInfo* pInfo)
{
}

void PrintWndBase::BeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
}

void PrintWndBase::PrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
}

void PrintWndBase::Print(CDC* pDC, CPrintInfo* pInfo)
{
}

void PrintWndBase::EndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
}

void PrintWndBase::PreviewData()
{
	CFrameWnd* pOldFrame = (CFrameWnd*)AfxGetApp()->m_pMainWnd;
	pOldFrame->ShowWindow(SW_HIDE);

	// so load frame is happy. This matches the IDR_MAINFRAME value from your application!
	if (!m_pTemplate)
	{
		m_pTemplate = new CSingleDocTemplate(IDR_MAINFRAME, NULL, RUNTIME_CLASS(PrintPreviewFrame), 
											 RUNTIME_CLASS(WorkingView));
	}

	// init frame
	CFrameWnd* pFrameWnd = m_pTemplate->CreateNewFrame(NULL, NULL);
	m_pTemplate->InitialUpdateFrame(pFrameWnd, NULL, FALSE);
	AfxGetApp()->m_pMainWnd = pFrameWnd;
	pFrameWnd->SetWindowText(_T("Print preview"));
	pFrameWnd->ShowWindow(SW_SHOWMAXIMIZED);

	// init view.
	WorkingView* pWorkingView = (WorkingView*)pFrameWnd->GetActiveView();
	pWorkingView->SetPrintWnd(this);
	pWorkingView->m_pCurrentFrame = pOldFrame;

	// print.
	pWorkingView->OnPrintPreview();
}

void PrintWndBase::PrintData()
{
	WorkingView	workingView;
	workingView.SetPrintWnd(this);
	workingView.OnFilePrint();
}

END_CUTLEADER_NAMESPACE()

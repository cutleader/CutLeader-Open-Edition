#include "stdafx.h"
#include "clTemplateDesign.h"

#include "ReportTplPage.h"

#include "ChildFrm.h"
#include "TplPageTracker.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWndEx)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWndEx)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

CChildFrame::CChildFrame()
{
}

CChildFrame::~CChildFrame()
{
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying the CREATESTRUCT cs
	if( !CMDIChildWndEx::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}

void CChildFrame::OnClose()
{
	// check whether the current document has been modified.
	CDocument* pTmpDoc = GetActiveDocument();
	CtplDesignDoc* pDoc = dynamic_cast<CtplDesignDoc*>(pTmpDoc);
	pDoc->SetModifyStatus();

	CMDIChildWndEx::OnClose();
}

void CChildFrame::OnDestroy()
{
	CDocument* pTmpDoc = GetActiveDocument();
	CtplDesignDoc* pDoc = dynamic_cast<CtplDesignDoc*>(pTmpDoc);
	if (pDoc)
		TplPageTracker::GetInstance()->RemoveItem(pDoc->GetReportTplPage()->GetID());
}

END_CUTLEADER_NAMESPACE()

#include "stdafx.h"
#include "clKnowledgeBaseConfig.h"

#include "ChildFrm.h"
#include "KBConfigDoc.h"
#include "KBConfigView.h"
#include "clKnowledgeBaseConfigResource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWndEx)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWndEx)
	ON_WM_CLOSE()

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
	KBConfigDoc* pCamConfigDoc = dynamic_cast<KBConfigDoc*>(pTmpDoc);
	pCamConfigDoc->SetModifyStatus();

	CMDIChildWndEx::OnClose();
}

END_CUTLEADER_NAMESPACE()

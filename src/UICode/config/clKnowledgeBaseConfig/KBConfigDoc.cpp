#include "stdafx.h"
#include "clKnowledgeBaseConfig.h"

#include "KBConfigDoc.h"
#include "KBConfigView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(KBConfigDoc, CDocument)

BEGIN_MESSAGE_MAP(KBConfigDoc, CDocument)
END_MESSAGE_MAP()

KBConfigDoc::KBConfigDoc()
{
}

KBConfigDoc::~KBConfigDoc()
{
}

BOOL KBConfigDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	return TRUE;
}

BOOL KBConfigDoc::OnSaveDocument()
{
	return TRUE;
}

void KBConfigDoc::SetModifyStatus()
{
	POSITION pos = GetFirstViewPosition();
	KBConfigView* pCamConfigView = (KBConfigView*)GetNextView(pos);

	BOOL bModified = pCamConfigView->DocModified();
	SetModifiedFlag(bModified);
}

END_CUTLEADER_NAMESPACE()

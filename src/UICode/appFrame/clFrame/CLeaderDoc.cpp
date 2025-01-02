#include "stdafx.h"
#include "cLeaderDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(CLeaderDoc, CDocument)

BEGIN_MESSAGE_MAP(CLeaderDoc, CDocument)
END_MESSAGE_MAP()

CLeaderDoc::CLeaderDoc()
{
}

CLeaderDoc::~CLeaderDoc()
{
}

BOOL CLeaderDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	return TRUE;
}

BOOL CLeaderDoc::OnSaveDocument()
{
	return TRUE;
}

END_CUTLEADER_NAMESPACE()

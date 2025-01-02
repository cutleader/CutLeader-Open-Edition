#include "stdafx.h"
#include "clKnowledgeBaseConfig.h"

#include "ExpertLibDoc.h"
#include "ExpertLibView.h"
#include "clKnowledgeBaseConfigResource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(ExpertLibDoc, KBConfigDoc)

ExpertLibDoc::ExpertLibDoc()
{

}

ExpertLibDoc::~ExpertLibDoc()
{
}

BOOL ExpertLibDoc::OnNewDocument()
{
	// set the title.
	CString strTitle;
	strTitle = MultiLanguageMgr::GetInstance()->TranslateString(IDS_EXPERTLIB_MGR);
	SetTitle(strTitle);

	return KBConfigDoc::OnNewDocument();
}

BOOL ExpertLibDoc::OnSaveDocument()
{
	// get the relative view.
	POSITION pos = GetFirstViewPosition();
	ExpertLibView* pExpertLibView = (ExpertLibView*)GetNextView(pos);

	// delegate the save action to ExpertLibView.
	pExpertLibView->SaveCurrentPage();

	return TRUE;
}

END_CUTLEADER_NAMESPACE()

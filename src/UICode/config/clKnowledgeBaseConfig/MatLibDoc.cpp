#include "stdafx.h"
#include "clKnowledgeBaseConfig.h"

#include "MatLibDoc.h"
#include "MatLibView.h"
#include "clKnowledgeBaseConfigResource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(MatLibDoc, KBConfigDoc)

MatLibDoc::MatLibDoc()
{

}

MatLibDoc::~MatLibDoc()
{
}

BOOL MatLibDoc::OnNewDocument()
{
	// set the title.
	CString strTitle;
	strTitle = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MATLIB_MGR);
	SetTitle(strTitle);

	return KBConfigDoc::OnNewDocument();
}

BOOL MatLibDoc::OnSaveDocument()
{
	// get the relative view.
	POSITION pos = GetFirstViewPosition();
	MatLibView* pMatLibView = (MatLibView*)GetNextView(pos);

	// delegate the save action to MatLibView.
	pMatLibView->SaveCurrentPage();

	return TRUE;
}

END_CUTLEADER_NAMESPACE()

#include "stdafx.h"
#include "clKnowledgeBaseConfig.h"

#include "MacMgrDoc.h"
#include "MacMgrView.h"
#include "clKnowledgeBaseConfigResource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(MacMgrDoc, KBConfigDoc)

MacMgrDoc::MacMgrDoc()
{
}

MacMgrDoc::~MacMgrDoc()
{
}

BOOL MacMgrDoc::OnNewDocument()
{
	// set the title.
	CString strTitle;
	strTitle = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MAC_MGR);
	SetTitle(strTitle);

	return KBConfigDoc::OnNewDocument();
}

BOOL MacMgrDoc::OnSaveDocument()
{
	// get the relative view.
	POSITION pos = GetFirstViewPosition();
	MacMgrView* pMacMgrView = (MacMgrView*)GetNextView(pos);

	// delegate the save action to MacMgrView.
	pMacMgrView->SaveCurrentPage();

	return TRUE;
}

END_CUTLEADER_NAMESPACE()

#include "stdafx.h"
#include "tplDesignDoc.h"

#include "DataBaseManager.h"
#include "clBaseDataResource.h"

#include "clReportResource.h"
#include "NewTpPageDlg.h"
#include "TplPageDlg.h"
#include "ReportTplPage.h"
#include "TplPageWriter.h"
#include "TplPageLoader.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(CtplDesignDoc, CDocument)

BEGIN_MESSAGE_MAP(CtplDesignDoc, CDocument)
END_MESSAGE_MAP()

CtplDesignDoc::CtplDesignDoc()
{
}

CtplDesignDoc::~CtplDesignDoc()
{
}

BOOL CtplDesignDoc::OnNewDocument()
{
	// create the new template page.
	NewTpPageDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		m_pReportTplPage = dlg.GetReportTplPage();
		SetTitle(m_pReportTplPage->GetName());
	}
	else
		return FALSE;

	return TRUE;
}

BOOL CtplDesignDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	TplPageDlg dlg;
	if (dlg.DoModal() == IDC_BTN_OPEN)
	{
		m_pReportTplPage = TplPageLoader::LoadReportTplPage(dlg.GetCurReportTplPageID());
		SetTitle(m_pReportTplPage->GetName());
		return TRUE;
	}

	return FALSE;
}

BOOL CtplDesignDoc::OnSaveDocument()
{
	DBConnectPtr pDBConnect = DataBaseManager::GetInstance()->GetRptLibDBConnect();

	CString strProductName, strMsgContent;
	strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);

	// check whether the template page is changed.
	if (m_pReportTplPage->IsModified())
	{
		pDBConnect->BeginTransaction();
		TplPageWriter::DeleteTplPage(m_pReportTplPage->GetID(), FALSE, FALSE);
		TplPageWriter::CreateTplPage(m_pReportTplPage, FALSE);
		pDBConnect->CommitTransaction();

		strMsgContent = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_TEMPLATE_PAGE_SAVED);
		strMsgContent.Replace(_T("WILLBEREPLACED1"), m_pReportTplPage->GetName());
	}
	else
	{
		strMsgContent = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_TEMPLATE_PAGE_NOT_CHANGED);
		strMsgContent.Replace(_T("WILLBEREPLACED1"), m_pReportTplPage->GetName());
	}

	// prompt user.
	POSITION pos = GetFirstViewPosition();
	CView* pFirstView = GetNextView(pos);
	MessageBox(pFirstView->m_hWnd, strMsgContent, strProductName, MB_OK | MB_ICONINFORMATION);

	return TRUE;
}

void CtplDesignDoc::SetModifyStatus()
{
	BOOL bModified = m_pReportTplPage->IsModified();
	SetModifiedFlag(bModified);
}

END_CUTLEADER_NAMESPACE()

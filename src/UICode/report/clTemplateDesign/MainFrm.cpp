#include "stdafx.h"
#include "MainFrm.h"

#include "tplDesignView.h"
#include "tplDesignDoc.h"
#include "clReportResource.h"
#include "taoResource.h"
#include "SkinManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWndEx)
	ON_WM_CREATE()
	ON_WM_CLOSE()

	ON_REGISTERED_MESSAGE(AFX_WM_ON_BEFORE_SHOW_RIBBON_ITEM_MENU, OnShowRibbonItemMenu)
	ON_MESSAGE(WM_UPDATE_STATUS_BAR, OnUpdateStatusBar)
END_MESSAGE_MAP()

CMainFrame::CMainFrame()
{
}

CMainFrame::~CMainFrame()
{
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWndEx::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// set the visual manager used to draw all user interface elements
	CMFCVisualManager::SetDefaultManager( RUNTIME_CLASS( SkinManager ) );
	
	CMDITabInfo mdiTabParams;
	mdiTabParams.m_style = CMFCTabCtrl::STYLE_3D_ONENOTE; // other styles available...
	mdiTabParams.m_bActiveTabCloseButton = TRUE;      // set to FALSE to place close button at right of tab area
	mdiTabParams.m_bTabIcons = TRUE;    // set to TRUE to enable document icons on MDI taba
	mdiTabParams.m_bAutoColor = FALSE;    // set to FALSE to disable auto-coloring of MDI tabs
	mdiTabParams.m_bDocumentMenu = TRUE; // enable the document menu at the right edge of the tab area
	EnableMDITabbedGroups(TRUE, mdiTabParams);

	// init the ribbon bar.
	InitializeRibbon();

	// init the status bar.
	m_statusBar.Create(this);
	m_statusBar.AddElement(new CMFCRibbonStatusBarPane(0, _T(""), TRUE), _T(""));

	// enable Visual Studio 2005 style docking window behavior
	CDockingManager::SetDockingMode(DT_SMART);

	// enable Visual Studio 2005 style docking window auto-hide behavior
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	return 0;
}

void CMainFrame::OnClose()
{
	/************************************************************************/
	// check whether some documents have been modified.

	CDocManager* pDocManager = AfxGetApp()->m_pDocManager;
	POSITION pos = pDocManager->GetFirstDocTemplatePosition();
	while (pos)
	{
		CMultiDocTemplate* pDocTemp = (CMultiDocTemplate*)pDocManager->GetNextDocTemplate(pos);
		if (pDocTemp)
		{
			POSITION docPos = pDocTemp->GetFirstDocPosition();
			while (docPos)
			{
				CDocument* pDoc = pDocTemp->GetNextDoc(docPos);
				CtplDesignDoc* pTmpDoc = dynamic_cast<CtplDesignDoc*>(pDoc);
				pTmpDoc->SetModifyStatus();
			}
		}
	}
	/************************************************************************/

	CMDIFrameWndEx::OnClose();
}

LRESULT CMainFrame::OnUpdateStatusBar(WPARAM wParam, LPARAM lParam)
{
	LPCTSTR lpszInfo = (LPCTSTR)wParam;
	m_statusBar.SetInformation(lpszInfo);

	return true;
}

LRESULT CMainFrame::OnShowRibbonItemMenu(WPARAM, LPARAM lp)
{
	CMFCRibbonBaseElement* pElem = (CMFCRibbonBaseElement*)lp;

	// get the active view.
	CMDIChildWnd* pChild = (CMDIChildWnd*)GetActiveFrame();
	if (pChild)
	{
		CView* pView = pChild->GetActiveView();
		CtplDesignView* pTmpView = dynamic_cast<CtplDesignView*>(pView);
		if (pTmpView)
			pTmpView->UpdateUndoRedoDropdownList(pElem);
	}

	return 0;
}

void CMainFrame::InitializeRibbon()
{
	CString str;

	// create the ribbon.
	m_ribbonBar.Create(this);

	// init main button.
	m_mainButton.SetImage(IDB_MAIN);
	m_ribbonBar.SetApplicationButton(&m_mainButton, CSize (45, 45));

	/************************************************************************/
	// init the "control panel" category.

	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_CTRL_PANEL);
	CMFCRibbonCategory* pTplDesignCategory = m_ribbonBar.AddCategory(str, IDB_QUICKACCESS, IDB_TPLDESIGN_BAR);

	// the "Save" button
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_TPL_SAVE);
	pTplDesignCategory->AddHidden(new CMFCRibbonButton(IDC_TPL_SAVE, str, 0));

	// add hidden buttons for UNDO/REDO
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_TPL_UNDO);
	pTplDesignCategory->AddHidden(new CMFCRibbonUndoButton(IDC_TPL_UNDO, str, 1));
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_TPL_REDO);
	pTplDesignCategory->AddHidden(new CMFCRibbonUndoButton(IDC_TPL_REDO, str, 2));
	/************************************************************************/

	/************************************************************************/
	// add "general" panel.

	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_TPL_GENERAL);
	CMFCRibbonPanel* pGeneralPanel = pTplDesignCategory->AddPanel(str);

	// add "Preference" button.
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_TPL_PREFERENCE);
	pGeneralPanel->Add(new CMFCRibbonButton(IDC_TPL_PREFERENCE, str, -1, 0));

	// add "Report Template Management" button.
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MGR_REPORT_TPL);
	pGeneralPanel->Add(new CMFCRibbonButton(IDC_MGR_REPORT_TPL, str, -1, 1));

	pGeneralPanel->AddSeparator();

	// add "New Template Page" button.
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_NEW_TPL_PAGE);
	pGeneralPanel->Add(new CMFCRibbonButton(IDC_NEW_TPL_PAGE, str, -1, 2));

	// the "template page management" button.
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MGR_TPL_PAGE);
	CMFCRibbonButton* pBtnOpenTpl = new CMFCRibbonButton(IDC_MGR_TPL_PAGE, str, -1, 3);
	pGeneralPanel->Add(pBtnOpenTpl);
	/************************************************************************/

	/************************************************************************/
	// add "draw" panel.

	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_ENTITY_DRAW);
	CMFCRibbonPanel* pDrawPanel = pTplDesignCategory->AddPanel(str);

	// “静态文本”按钮。
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_DRAW_STATIC);
	pDrawPanel->Add(new CMFCRibbonButton(IDC_DRAW_STATIC, str, -1, 4));

	// "field" button
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_DRAW_FIELD);
	pDrawPanel->Add(new CMFCRibbonButton(IDC_DRAW_FIELD, str, -1, 5));

	// "static picture" button
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_DRAW_STATIC_IMG);
	pDrawPanel->Add(new CMFCRibbonButton(IDC_DRAW_STATIC_IMG, str, -1, 6));

	// "dynamic picture" button
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_DRAW_DYN_IMG);
	pDrawPanel->Add(new CMFCRibbonButton(IDC_DRAW_DYN_IMG, str, -1, 7));

	// "grid" button
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_DRAW_GRID);
	pDrawPanel->Add(new CMFCRibbonButton(IDC_DRAW_GRID, str, -1, 8));

	// "line" button
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_DRAW_LINE);
	pDrawPanel->Add(new CMFCRibbonButton(IDC_DRAW_LINE, str, -1, 9));

	// "box" button
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_DRAW_BOX);
	pDrawPanel->Add(new CMFCRibbonButton(IDC_DRAW_BOX, str, -1, 10));

	// "ellipse" button
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_DRAW_ELLIPSE);
	pDrawPanel->Add(new CMFCRibbonButton(IDC_DRAW_ELLIPSE, str, -1, 11));

	// "label" button
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_DRAW_LABEL);
	pDrawPanel->Add(new CMFCRibbonButton(IDC_DRAW_LABEL, str, -1, 12));

	// "barcode" button
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_DRAW_BARCODE);
	pDrawPanel->Add(new CMFCRibbonButton(IDC_DRAW_BARCODE, str, -1, 13));
	/************************************************************************/

	/************************************************************************/
	// add "edit" panel.

	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_ENTITY_EDIT);
	CMFCRibbonPanel* pEditPanel = pTplDesignCategory->AddPanel(str);

	// "cut" button
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_ENTITY_CUT);
	pEditPanel->Add(new CMFCRibbonButton(IDC_ENTITY_CUT, str, -1, 14));

	// "copy" button
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_ENTITY_COPY);
	pEditPanel->Add(new CMFCRibbonButton(IDC_ENTITY_COPY, str, -1, 15));

	// "paste" button
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_ENTITY_PASTE);
	pEditPanel->Add(new CMFCRibbonButton(IDC_ENTITY_PASTE, str, -1, 16));

	// "delete" button
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_ENTITY_DELETE);
	pEditPanel->Add(new CMFCRibbonButton(IDC_ENTITY_DELETE, str, -1, 17));

	pEditPanel->AddSeparator();

	// "Group" button
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_ENTITY_GROUP);
	pEditPanel->Add(new CMFCRibbonButton(IDC_ENTITY_GROUP, str, -1, 18));

	// "UnGroup" button
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_ENTITY_UNGROUP);
	pEditPanel->Add(new CMFCRibbonButton(IDC_ENTITY_UNGROUP, str, -1, 19));

	pEditPanel->AddSeparator();

	// "property" button
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_ENTITY_PROP);
	pEditPanel->Add(new CMFCRibbonButton(IDC_ENTITY_PROP, str, -1, 20));
	/************************************************************************/

	/************************************************************************/
	// add "view" panel.

	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_ENTITY_VIEW);
	CMFCRibbonPanel* pViewPanel = pTplDesignCategory->AddPanel(str);

	// "zoom in" button
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_ENTITY_ZOOM_IN);
	pViewPanel->Add(new CMFCRibbonButton(IDC_ENTITY_ZOOM_IN, str, -1, 21));

	// "zoom out" button
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_ENTITY_ZOOM_OUT);
	pViewPanel->Add(new CMFCRibbonButton(IDC_ENTITY_ZOOM_OUT, str, -1, 22));

	// "no zoom" button
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_ENTITY_NO_ZOOM);
	pViewPanel->Add(new CMFCRibbonButton(IDC_ENTITY_NO_ZOOM, str, -1, 23));
	/************************************************************************/

	/************************************************************************/	
	// Add quick access toolbar commands.

	CList<UINT, UINT> quickAccessBtns;
	quickAccessBtns.AddTail(IDC_TPL_SAVE);
	quickAccessBtns.AddTail(IDC_TPL_UNDO);
	quickAccessBtns.AddTail(IDC_TPL_REDO);

	m_ribbonBar.SetQuickAccessCommands(quickAccessBtns);
	/************************************************************************/
}

END_CUTLEADER_NAMESPACE()

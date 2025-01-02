#include "stdafx.h"
#include "clKnowledgeBaseConfig.h"

#include "clUtilityResource.h"
#include "ProcessMgr.h"

#include "clKnowledgeBaseConfigResource.h"

#include "taoresource.h"
#include "KBConfigDoc.h"
#include "MainFrm.h"
#include "clFrameResource.h"
#include "SkinManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWndEx)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_COPYDATA()

	ON_COMMAND(ID_WINDOW_MANAGER, &CMainFrame::OnWindowManager)

END_MESSAGE_MAP()

CMainFrame::CMainFrame()
{
}

CMainFrame::~CMainFrame()
{
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

	// enable docking for global.
	EnableDocking(CBRS_ALIGN_ANY);

	// init the ribbon bar.
	m_wndRibbonBar.Create(this);
	InitializeRibbon();

	// enable Visual Studio 2005 style docking window behavior
	CDockingManager::SetDockingMode(DT_SMART);
	// enable Visual Studio 2005 style docking window auto-hide behavior
	EnableAutoHidePanes(CBRS_ALIGN_ANY);
	// Enable enhanced windows management dialog
	EnableWindowsDialog(ID_WINDOW_MANAGER, _T(""), TRUE);

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
				KBConfigDoc* pTmpDoc = dynamic_cast<KBConfigDoc*>(pDoc);
				pTmpDoc->SetModifyStatus();
			}
		}
	} 
	/************************************************************************/

	CMDIFrameWndEx::OnClose();
}

BOOL CMainFrame::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct) 
{
	WndMsg* pWndMsg = (WndMsg*)(pCopyDataStruct->lpData);
	if (pWndMsg->iWndMsgType == WNDMSG_ACTIVE_MAIN)
	{
		ShowWindow(SW_SHOWMAXIMIZED);
		::SetForegroundWindow(m_hWnd);
	}
	else if (pWndMsg->iWndMsgType == WNDMSG_ACTIVE_DOC)
	{
		// send message to app class.
		AfxGetApp()->PostThreadMessage(WM_OPEN_LIB, (WPARAM)pWndMsg->iMessageValue, NULL);

		//
		ShowWindow(SW_SHOWMAXIMIZED);
		::SetForegroundWindow(m_hWnd);
	}

	return CMDIFrameWndEx::OnCopyData(pWnd, pCopyDataStruct);
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWndEx::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}

void CMainFrame::InitializeRibbon()
{
	CString str;

	// Init main button.
	m_MainButton.SetImage(IDB_MAIN);
	m_wndRibbonBar.SetApplicationButton(&m_MainButton, CSize(45, 45));

	/************************************************************************/
	// create buttons in main category and the quick access toolbar.

	// create main category.
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_FILE);
	CMFCRibbonMainPanel* pMainCategory = m_wndRibbonBar.AddMainCategory(str, IDB_FILESMALL, IDB_FILELARGE);

	// "Save" menu
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SAVE);
	pMainCategory->Add(new CMFCRibbonButton(IDC_CONFIG_FILE_SAVE, str, 2, 2));

	// Add quick access toolbar commands.
	CList<UINT, UINT> quickAccessBtns;
	// we always need a "save" button.
	quickAccessBtns.AddTail(IDC_CONFIG_FILE_SAVE);
	m_wndRibbonBar.SetQuickAccessCommands(quickAccessBtns);

	// "Exit" menu
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_EXIT);
	pMainCategory->Add(new CMFCRibbonButton(ID_APP_EXIT, str, 4, 4));
	/************************************************************************/

	/************************************************************************/
	// init "CAM Library" category.

	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_CAM_LIBRARY);
	CMFCRibbonCategory* pCamConfigCategory = m_wndRibbonBar.AddCategory(str, IDB_CAMCONFIG_BAR, IDB_CAMCONFIG_BAR);
	CMFCRibbonPanel* pCamConfigPanel = pCamConfigCategory->AddPanel(_T(""));

	// add "Machine Library Management" button.
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MAC_MGR);
	CMFCRibbonButton* pMacMgrBtn = new CMFCRibbonButton(IDC_MAC_MGR, str, -1, 1);
	pCamConfigPanel->Add(pMacMgrBtn);

	// add "Expert Library Management" button.
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_EXPERTLIB_MGR);
	CMFCRibbonButton* pExpertLibMgrBtn = new CMFCRibbonButton(IDC_EXPERTLIB_MGR, str, -1, 2);
	pCamConfigPanel->Add(pExpertLibMgrBtn);

	// add "Material Library Management" button.
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MATLIB_MGR);
	CMFCRibbonButton* pMatLibMgrBtn = new CMFCRibbonButton(IDC_MATLIB_MGR, str, -1, 3);
	pCamConfigPanel->Add(pMatLibMgrBtn);
	/************************************************************************/

	// 帮助栏
	{
		// add "help" category
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_HELP);
		CMFCRibbonCategory* pHelpCategory = m_wndRibbonBar.AddCategory(str, IDB_UNDO_REDO, IDB_HELP_BAR);
		CMFCRibbonPanel* pHelpPanel = pHelpCategory->AddPanel(_T(""));

		// “更新加密狗”按钮
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_HELP_UPDATE_DONGLE);
		CMFCRibbonButton* pLicMgrBtn = new CMFCRibbonButton(IDC_HELP_LIC_MGR, str, -1, 3);
		pLicMgrBtn->SetToolTipText(str);
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_HELP_UPDATE_DONGLE_TIP);
		pLicMgrBtn->SetDescription(str);
		pHelpPanel->Add(pLicMgrBtn);
	}

	m_wndRibbonBar.ForceRecalcLayout();
}

void CMainFrame::OnWindowManager()
{
	ShowWindowsDialog();
}

END_CUTLEADER_NAMESPACE()

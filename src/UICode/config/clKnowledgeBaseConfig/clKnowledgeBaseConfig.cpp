#include "stdafx.h"
#include "afxwinappex.h"
#include "clKnowledgeBaseConfig.h"

#include "ProcessMgr.h"
#include "IntegerList.h"
#include "DateHelper.h"
#include "RandomGen.h"

#include "clSecureResource.h"
#include "SplashThread.h"
#include "ProductFileDefine.h"
#include "ClFolderHelper.h"
#include "clBaseDataResource.h"
#include "DataBaseManager.h"
#include "MultiLanguageMgr.h"
#include "ClFileHelper.h"

#include "ExpertLibView.h"
#include "ExpertLibDoc.h"
#include "MatLibDoc.h"
#include "MatLibView.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "MacMgrView.h"
#include "MacMgrDoc.h"
#include "taoResource.h"
#include "clKnowledgeBaseConfigResource.h"

#include "ProductParam.h"
#include "CLOptionLoader.h"
#include "CLOptionInfo.h"
#include "SkinManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(ClKnowledgeBaseConfigApp, CWinAppEx)
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)

	ON_COMMAND(IDC_CONFIG_FILE_SAVE, &ClKnowledgeBaseConfigApp::OnFileSave)

	ON_COMMAND(IDC_MAC_MGR, &ClKnowledgeBaseConfigApp::OnMacMgr)
	ON_COMMAND(IDC_EXPERTLIB_MGR, &ClKnowledgeBaseConfigApp::OnExpertLibMgr)
	ON_COMMAND(IDC_MATLIB_MGR, &ClKnowledgeBaseConfigApp::OnMatLibMgr)

	ON_THREAD_MESSAGE(WM_OPEN_LIB, OpenLibrary)
END_MESSAGE_MAP()

ClKnowledgeBaseConfigApp::ClKnowledgeBaseConfigApp()
{
}

ClKnowledgeBaseConfigApp theApp;

void ClKnowledgeBaseConfigApp::Init()
{
	// init the random value manager.
	RandomGen::Initialize((unsigned long long)time(NULL));

	// init the option information.
	CString strPath = ClFolderHelper::GetOptionPath();
	strPath += USER_OPTION_FILE;
	CLOptionInfoPtr pCLOptionInfo = CLOptionLoader::LoadCLOption(strPath);
	CLOptionInfo::Init(pCLOptionInfo);

	// init the database connection which will be used by app.
	DataBaseManager::GetInstance()->InitDB();

	// 加载资源dll
	ProductParamPtr pProductParam = pCLOptionInfo->GetProductParam();
    HINSTANCE hInstRes = ::LoadLibrary(_T(CL_RES_FILE));
	if (hInstRes)
    {
		AfxSetResourceHandle(hInstRes);
    }

    // 加载当前语言对应的翻译数据，这一行调用需要在加载资源之后。
    MultiLanguageMgr::GetInstance()->LoadTranslationData(ClFileHelper::GetTranslationLibPath(), pProductParam->GetLang());
}

BOOL ClKnowledgeBaseConfigApp::InitInstance()
{
	// init the COM apartment
	CoInitialize(NULL);

	// init.
	Init();

	/************************************************************************/
	// first of all, make sure only one instance is active.
	// notes:
	//   1) place after Init call, so we can use the resource.

	TCHAR strCurExePath[MAXLENGTH_STR];
	GetModuleFileName(NULL, strCurExePath, MAXLENGTH_STR);
	IntegerListPtr pProcessIDList = ProcessMgr::IsProcessRun(strCurExePath);
	if (pProcessIDList->size() == 1)
	{
		// this is the first time that run this app, ok, let it run.
		;
	}
	else if (pProcessIDList->size() == 2)
	{
		// an other instance of this application is running.
		// so here we should send msg to that instance to activate it.

		// remove the current process ID.
		int iCurProcessID = GetCurrentProcessId();
		pProcessIDList->RemoveInt(iCurProcessID);

		// get the main frame of the running app.
		CString str;
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);
		HWND hMainWnd = ProcessMgr::GetProcessMainWnd((int)pProcessIDList->at(0), str);

		// send msg to the main frame of the running app.
		WndMsg wndMsg;
		wndMsg.iWndMsgType = WNDMSG_ACTIVE_MAIN;
		ProcessMgr::SendCopyMsg(hMainWnd, wndMsg);

		return FALSE;
	}
	/************************************************************************/

	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();

	// 注册表键值。
	CString strRegKey;
	strRegKey = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_REG_KEY);
	SetRegistryKey(strRegKey);
	LoadStdProfileSettings(0);  // Load standard INI file options

	InitContextMenuManager();
	InitKeyboardManager();
	InitTooltipManager();

	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL, RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	/************************************************************************/
	// create each document template.
	// notes:
	//   1) do not need to create a new C***Frame class.

	// for Machine Management.
	m_pMacMgrDocTpl = new CMultiDocTemplate(IDR_camConfigTYPE,
											RUNTIME_CLASS(MacMgrDoc),
											RUNTIME_CLASS(CChildFrame),
											RUNTIME_CLASS(MacMgrView));
	if (!m_pMacMgrDocTpl)
		return FALSE;
	AddDocTemplate(m_pMacMgrDocTpl);

	// for Expert Library Management.
	m_pExpertLibDocTpl = new CMultiDocTemplate(IDR_camConfigTYPE,
											   RUNTIME_CLASS(ExpertLibDoc),
											   RUNTIME_CLASS(CChildFrame),
											   RUNTIME_CLASS(ExpertLibView));
	if (!m_pExpertLibDocTpl)
		return FALSE;
	AddDocTemplate(m_pExpertLibDocTpl);

	// for Material Library Management.
	m_pMatLibDocTpl = new CMultiDocTemplate(IDR_camConfigTYPE,
										    RUNTIME_CLASS(MatLibDoc),
										    RUNTIME_CLASS(CChildFrame),
										    RUNTIME_CLASS(MatLibView));
	if (!m_pMatLibDocTpl)
		return FALSE;
	AddDocTemplate(m_pMatLibDocTpl);
	/************************************************************************/

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;
	// call DragAcceptFiles only if there's a suffix
	//  In an MDI app, this should occur immediately after setting m_pMainWnd

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	if (cmdInfo.m_nShellCommand == CCommandLineInfo::FileNew)
		;
	else
	{
		// Dispatch commands specified on the command line.  Will return FALSE if
		// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
		if (!ProcessShellCommand(cmdInfo))
			return FALSE;
	}

	// The main window has been initialized, so show and update it
	pMainFrame->ShowWindow(SW_SHOWMAXIMIZED);
	pMainFrame->UpdateWindow();

	return TRUE;
}

int ClKnowledgeBaseConfigApp::ExitInstance()
{
	RandomGen::UnInitialize();

    // 清理掉翻译库
    MultiLanguageMgr* pMultiLanguageMgr = MultiLanguageMgr::GetInstance();
    delete pMultiLanguageMgr;

	return CWinAppEx::ExitInstance();
}

void ClKnowledgeBaseConfigApp::OnFileSave()
{
	KBConfigDoc* pDoc = GetActiveDocument();
	if (pDoc)
	{
		pDoc->OnSaveDocument();
	}
}

void ClKnowledgeBaseConfigApp::OpenLibrary(WPARAM wParam, LPARAM lParam)
{
	// get the tab which will be active.
	int iTabNameRes = (int)wParam;

	if (iTabNameRes == IDS_MAC_MGR)
		OnMacMgr();
	else if (iTabNameRes == IDS_EXPERTLIB_MGR)
		OnExpertLibMgr();
	else if (iTabNameRes == IDS_MATLIB_MGR)
		OnMatLibMgr();

	m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);
}

void ClKnowledgeBaseConfigApp::OnMacMgr()
{
	CString strTitle;
	strTitle = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MAC_MGR);

	// check whether we have opened the machine management form.
	BOOL bFind = FALSE;
	POSITION docpos = m_pMacMgrDocTpl->GetFirstDocPosition();
	while (docpos)
	{
		CDocument* pDoc = m_pMacMgrDocTpl->GetNextDoc(docpos);
		CString strTmpTitle = pDoc->GetTitle();

		// we have opened the resource explorer.
		if (strTmpTitle == strTitle)
		{
			// get the relative view of the document.
			POSITION pos = pDoc->GetFirstViewPosition();
			CView* pFirstView = pDoc->GetNextView(pos);

			// activate the view.
			CMDIFrameWnd* pFrame = (CMDIFrameWnd*)AfxGetMainWnd();
			pFrame->MDIActivate(pFirstView->GetParentFrame());

			bFind = TRUE;
			break;
		}
	}

	if (!bFind)
		m_pMacMgrDocTpl->OpenDocumentFile(NULL);
}

void ClKnowledgeBaseConfigApp::OnExpertLibMgr()
{
	CString strTitle;
	strTitle = MultiLanguageMgr::GetInstance()->TranslateString(IDS_EXPERTLIB_MGR);

	// check whether we have opened the ExpLib management form.
	BOOL bFind = FALSE;
	POSITION docpos = m_pExpertLibDocTpl->GetFirstDocPosition();
	while (docpos)
	{
		CDocument* pDoc = m_pExpertLibDocTpl->GetNextDoc(docpos);
		CString strTmpTitle = pDoc->GetTitle();

		// we have opened the resource explorer.
		if (strTmpTitle == strTitle)
		{
			// get the relative view of the document.
			POSITION pos = pDoc->GetFirstViewPosition();
			CView* pFirstView = pDoc->GetNextView(pos);

			// activate the view.
			CMDIFrameWnd* pFrame = (CMDIFrameWnd*)AfxGetMainWnd();
			pFrame->MDIActivate(pFirstView->GetParentFrame());

			bFind = TRUE;
			break;
		}
	}

	if (!bFind)
		m_pExpertLibDocTpl->OpenDocumentFile(NULL);
}

void ClKnowledgeBaseConfigApp::OnMatLibMgr()
{
	CString strTitle;
	strTitle = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MATLIB_MGR);

	// check whether we have opened the MatLib management form.
	BOOL bFind = FALSE;
	POSITION docpos = m_pMatLibDocTpl->GetFirstDocPosition();
	while (docpos)
	{
		CDocument* pDoc = m_pMatLibDocTpl->GetNextDoc(docpos);
		CString strTmpTitle = pDoc->GetTitle();

		// we have opened the resource explorer.
		if (strTmpTitle == strTitle)
		{
			// get the relative view of the document.
			POSITION pos = pDoc->GetFirstViewPosition();
			CView* pFirstView = pDoc->GetNextView(pos);

			// activate the view.
			CMDIFrameWnd* pFrame = (CMDIFrameWnd*)AfxGetMainWnd();
			pFrame->MDIActivate(pFirstView->GetParentFrame());

			bFind = TRUE;
			break;
		}
	}

	if (!bFind)
		m_pMatLibDocTpl->OpenDocumentFile(NULL);
}

KBConfigDoc* ClKnowledgeBaseConfigApp::GetActiveDocument()
{
	CMDIFrameWnd *pFrame = (CMDIFrameWnd*)AfxGetApp()->GetMainWnd();
	// Get the active MDI child window.
	CMDIChildWnd *pChild = (CMDIChildWnd*)pFrame->GetActiveFrame();

	// Get the active view attached to the active MDI child window.
	KBConfigView *pView = (KBConfigView*)pChild->GetActiveView();
	if (!pView)
		return NULL;

	KBConfigDoc* pDoc = pView->GetDocument();

	return pDoc;
}

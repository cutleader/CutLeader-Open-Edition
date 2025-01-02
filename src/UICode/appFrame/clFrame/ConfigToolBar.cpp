#include "stdafx.h"
#include "ConfigToolBar.h"

#include "LogMgr.h"
#include "ClWindowFinder.h"
#include "ClFolderHelper.h"
#include "ProductFileDefine.h"
#include "ProcessMgr.h"
#include "IntegerList.h"
#include "clBaseDataResource.h"

#include "CLOptionSheet.h"
#include "clOptionResource.h"

#include "clCutFeatureResource.h"
#include "SheetSubView.h"
#include "clKnowledgeBaseConfigResource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(ConfigToolBar, SubToolBar)

ConfigToolBar::ConfigToolBar() : SubToolBar(ConfigToolBar::IDD)
{
}

ConfigToolBar::~ConfigToolBar()
{
}

void ConfigToolBar::DoDataExchange(CDataExchange* pDX)
{
	SubToolBar::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BTN_SYSTEM_CONFIG, m_btnSystemConfig);
	DDX_Control(pDX, IDC_BTN_MATLIB, m_btnMatLib);
}

BEGIN_MESSAGE_MAP(ConfigToolBar, SubToolBar)
	ON_BN_CLICKED(IDC_BTN_SYSTEM_CONFIG, OnSystemConfig)
	ON_BN_CLICKED(IDC_BTN_MATLIB, OnMatLib)
END_MESSAGE_MAP()

BOOL ConfigToolBar::OnInitDialog()
{
	__super::OnInitDialog();

	m_btnSystemConfig.SetImage(IDB_SYS_CONFIG_32);
	m_btnSystemConfig.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_OPTIONS));

	m_btnMatLib.SetImage(IDB_MATLIB_32);
	m_btnMatLib.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_MATLIB_MGR));

	SetBackgroundColor(SkinManager::GetBackgroundColor());
	return TRUE;
}

void ConfigToolBar::OnSystemConfig()
{
	this->ShowWindow(SW_HIDE);

	CLOptionSheet clOptionSheet;
	clOptionSheet.DoModal();
}

void ConfigToolBar::OnMatLib()
{
	this->ShowWindow(SW_HIDE);

	// first, make sure one instance of cam config app is running.
	int iProcessID = INVALID_ID;
	{
		// get the exe path of cam config app.
		CString strCAMConfigPath = ClFolderHelper::GetProductPath();
		strCAMConfigPath += _T(KB_CONFIG_EXE_FILE);

		// check whether cam config app is running.
		BOOL bRunning = FALSE;
		IntegerListPtr pProcessIDList = ProcessMgr::IsProcessRun(strCAMConfigPath);
		if (pProcessIDList->size() > 0) {
			bRunning = TRUE;
			iProcessID = (int)pProcessIDList->at(0);
		}

		// if no instance of cam config app is running, run it.
		if (!bRunning) {
			iProcessID = ProcessMgr::RunProcess(strCAMConfigPath);
		}
	}

	// send msg to that instance to activate the proper doc.
	{
		// get the main frame of the running app.
		CString str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);
		HWND hMainWnd = ProcessMgr::GetProcessMainWnd(iProcessID, str);

		// send msg to the main frame of the running app.
		WndMsg wndMsg;
		wndMsg.iWndMsgType = WNDMSG_ACTIVE_DOC;
		wndMsg.iMessageValue = IDS_MATLIB_MGR;
		ProcessMgr::SendCopyMsg(hMainWnd, wndMsg);
	}
}

END_CUTLEADER_NAMESPACE()

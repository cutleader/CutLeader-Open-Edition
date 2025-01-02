#include "stdafx.h"
#include "clDataBackuper.h"

#include "ProductFileDefine.h"
#include "ClFolderHelper.h"
#include "MultiLanguageMgr.h"
#include "ClFileHelper.h"

#include "DataBackupDlg.h"

#include "ProductParam.h"
#include "CLOptionInfo.h"
#include "CLOptionLoader.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(ClDataBackuperApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

ClDataBackuperApp::ClDataBackuperApp()
{
}

ClDataBackuperApp theApp;

BOOL ClDataBackuperApp::InitInstance()
{
	/************************************************************************/
	// init the resource of the product.

	// init the option information.
	CString strPath = ClFolderHelper::GetOptionPath();
	strPath += USER_OPTION_FILE;
	CLOptionInfoPtr pCLOptionInfo = CLOptionLoader::LoadCLOption(strPath);
	CLOptionInfo::Init(pCLOptionInfo);

	// 加载资源dll
	ProductParamPtr pProductParam = pCLOptionInfo->GetProductParam();
    HINSTANCE hInstRes = ::LoadLibrary(_T(CL_RES_FILE));
	if (hInstRes)
    {
		AfxSetResourceHandle(hInstRes);
    }

    // 加载当前语言对应的翻译数据，这一行调用需要在加载资源之后。
    MultiLanguageMgr::GetInstance()->LoadTranslationData(ClFileHelper::GetTranslationLibPath(), pProductParam->GetLang());
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

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	DataBackupDlg dlg;
	m_pMainWnd = &dlg;

	dlg.Init(FALSE, FALSE);
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
	}
	else if (nResponse == IDCANCEL)
	{
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

int ClDataBackuperApp::ExitInstance()
{
    // 清理掉翻译库
    MultiLanguageMgr* pMultiLanguageMgr = MultiLanguageMgr::GetInstance();
    delete pMultiLanguageMgr;

    return CWinAppEx::ExitInstance();
}
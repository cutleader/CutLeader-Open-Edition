#include "stdafx.h"
#include "afxwinappex.h"
#include "clApp.h"

#include "ProcessMgr.h"
#include "IntegerList.h"
#include "DateHelper.h"
#include "RandomGen.h"
#include "UnitHelper.h"
#include "DBConnect.h"
#include "clUILibResource.h"
#include "ProductFileDefine.h"
#include "ClFolderHelper.h"
#include "clBaseDataResource.h"
#include "clSecureResource.h"
#include "DataBaseManager.h"
#include "LogMgr.h"
#include "DataBaseManager.h"
#include "baseConst.h"
#include "clSecureResource.h"
#include "Polygon2DProcessor.h"
#include "MultiLanguageMgr.h"
#include "ClFileHelper.h"
#include "DataCenterItemWriter.h"
#include "AboutDlg.h"
#include "DataItem.h"
#include "DataCenterItemList.h"
#include "FolderNode.h"
#include "DataCenterItemLoader.h"
#include "StringUtil.h"
#include "SplashThread.h"

#include "CLOptionSheet.h"
#include "CLOptionLoader.h"
#include "CLOptionWriter.h"
#include "SheetEditParam.h"
#include "ProductParam.h"
#include "ConfigItemWriter.h"
#include "clMaterialLibResource.h"
#include "clExpertLibResource.h"
#include "clOptionResource.h"
#include "clKnowledgeBaseConfigResource.h"
#include "DflTplDlg.h"

#include "RecentDataLoader.h"
#include "RecentDataWriter.h"
#include "MainFrame.h"
#include "ChildFrm.h"
#include "UIManager.h"
#include "DataItemTracker.h"
#include "FolderContentPage.h"
#include "clDataImportExportResource.h"
#include "PartFrame.h"
#include "PartDocument.h"
#include "PartView.h"
#include "CommandManager.h"
#include "ValueInputManager.h"
#include "CutNodeSimulator.h"
#include "TaskFrame.h"
#include "TaskView.h"
#include "TaskDocument.h"
#include "clBaseDataUIResource.h"
#include "SkinManager.h"
#include "DocManager.h"
#include "RecentDataList.h"
#include "RecentData.h"
#include "DataCenterDlg.h"
#include "PartItem.h"
#include "clFrameUIResource.h"
#include "ChangeLoopToolPane.h"
#include "ForegroundCamConnector.h"
#include "clPartLayoutUIResource.h"
#include "PartInplaceEditData.h"
#include "PartInplaceEditManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

BEGIN_MESSAGE_MAP(ClApp, CWinAppEx)
	ON_COMMAND(IDC_DATA_MANAGEMENT, OnDataMgr)
	ON_COMMAND(IDC_TAO_FILE_SAVE, OnFileSave)
	ON_COMMAND(IDC_TAO_FILE_SAVEAS, OnFileSaveAs)
	ON_COMMAND(IDC_NewEmptyPart, OnNewEmptyPart)
	ON_COMMAND(IDC_NewPartFromDxfdwg, OnNewPartFromDxfdwg)
	ON_COMMAND(IDC_NewTaskFromEmptySheet, OnNewTaskFromEmptySheet)
	ON_COMMAND(IDC_NewTaskFromNesting, OnNewTaskFromNesting)
	ON_COMMAND(IDC_SetDefaultParam, OnSetDefaultParam)

	// Configuration Category.
	ON_COMMAND(IDC_OPTIONS, OnCLOption)
	ON_COMMAND(IDC_MAC_MGR, OnMacLib)
	ON_COMMAND(IDC_EXPERTLIB_MGR, OnExpertLib)
	ON_COMMAND(IDC_MATLIB_MGR, OnMatLib)
	ON_COMMAND(IDC_DFL_REPORT_TPL, OnDflReportTpl)
	ON_COMMAND(IDC_DESIGN_REPORT_TPL, OnDesignReportTpl)

	// "Help" Category.
	ON_COMMAND(IDC_HELP_ABOUT, OnAbout)

	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)

	ON_COMMAND_EX_RANGE(IDC_RECENT_DATA_BASE, IDC_RECENT_DATA_BASE + RECENT_DATA_COUNT, OnOpenRecentData)
	ON_COMMAND(IDC_RECENT_DATA, OnRecentData)

	ON_THREAD_MESSAGE(WM_NewTaskAndAddDxf2Canvas, OnNewTaskAndAddDxf2Canvas)
	ON_THREAD_MESSAGE(WM_OpenSpecifiedSheet, OnOpenSpecifiedSheet)
	ON_THREAD_MESSAGE(WM_InPlaceEditPart, OnInPlaceEditPart)

	ON_UPDATE_COMMAND_UI(IDC_TAO_FILE_SAVE, OnUpdateUI_Save)
	ON_UPDATE_COMMAND_UI(IDC_TAO_FILE_SAVEAS, OnUpdateUI_SaveAs)
	ON_UPDATE_COMMAND_UI(ID_APP_EXIT, OnUpdateUI_Exit)
END_MESSAGE_MAP()

ClApp::ClApp()
{
	m_bNotLaunch = FALSE;
}

ClApp theApp;

BOOL ClApp::InitInstance()
{
	//VLDGlobalDisable(); // 关闭vld检测。

	// init the COM apartment
	CoInitialize(NULL);

	// init the unit conversion resolution value.
	int iResolution = GetDeviceCaps(::GetDC(NULL), LOGPIXELSX); // 水平方向每逻辑英寸多少个像素点。
	UnitHelper::Init(iResolution);

	// init the random value manager.
	RandomGen::Initialize((unsigned long long)time(NULL));

	// init the preference information.
	CString strPath = ClFolderHelper::GetOptionPath();
	strPath += USER_OPTION_FILE;
	CLOptionInfoPtr pCLOptionInfo = CLOptionLoader::LoadCLOption(strPath);
	CLOptionInfo::Init(pCLOptionInfo);

	// 初始化Polygon2DProcessor中的变量。
	SheetEditParamPtr pSheetEditParam = pCLOptionInfo->GetSheetEditParam();
	Polygon2DProcessor::Init(pSheetEditParam->EnableSleekCtrl(), pSheetEditParam->GetSleekRate());

	// 加载资源。
	ProductParamPtr pProductParam = pCLOptionInfo->GetProductParam();
	HINSTANCE hInstRes = ::LoadLibrary(_T(CL_RES_FILE));
	if (hInstRes)
		AfxSetResourceHandle(hInstRes);
	MultiLanguageMgr::GetInstance()->LoadTranslationData(ClFileHelper::GetTranslationLibPath(), pProductParam->GetLang()); // 加载当前语言对应的翻译数据，这一行调用需要在加载资源之后。

	// 只能有一个实例启动。
	TCHAR strCurExePath[MAXLENGTH_STR];
	GetModuleFileName(NULL, strCurExePath, MAXLENGTH_STR);
	IntegerListPtr pProcessIDList = ProcessMgr::IsProcessRun(strCurExePath);
	if (pProcessIDList->size() == 1)
	{
		;
	}
	else if (pProcessIDList->size() == 2)
	{
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

		m_bNotLaunch = TRUE;
		return FALSE;
	}

	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);
	CWinAppEx::InitInstance();
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
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)

	InitContextMenuManager();
	InitKeyboardManager();
	InitTooltipManager();

	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL, RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// display the splasher
	BeginSplash();

	// Register the application's document templates.  Document templates serve as the connection between documents, frame windows and views.
	{
		// init the part doc template.
		m_pPartDocTemplate = new CMultiDocTemplate(IDR_PART_DOC_TYPE, RUNTIME_CLASS(PartDocument), RUNTIME_CLASS(PartFrame), RUNTIME_CLASS(PartView));
		if (!m_pPartDocTemplate)
			return FALSE;
		AddDocTemplate(m_pPartDocTemplate);

		// 初始化任务文档模板。
		m_pTaskDocTemplate = new CMultiDocTemplate(IDR_TASK_DOC_TYPE, RUNTIME_CLASS(TaskDocument), RUNTIME_CLASS(TaskFrame), RUNTIME_CLASS(TaskView));
		if (!m_pTaskDocTemplate)
			return FALSE;
		AddDocTemplate(m_pTaskDocTemplate);
	}

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	if (cmdInfo.m_nShellCommand == CCommandLineInfo::FileNew)
		;
	else
	{
		if (!ProcessShellCommand(cmdInfo))
			return FALSE;
	}

	// The main window has been initialized, so show and update it
	pMainFrame->ShowWindow(m_nCmdShow);
	::SetForegroundWindow(pMainFrame->m_hWnd);
	pMainFrame->UpdateWindow();

	//
	Init();

	// erase the splasher
	EndSplash();

	if (!pProductParam->GetIsRunAsBackgroundApp())
		DocManager::NewDefaultTaskDoc(m_pTaskDocTemplate);
	else // 是以后台cam的方式启动。
		ForegroundCamConnector::GetInstance()->StartWatchThread();

	return TRUE;
}

int ClApp::ExitInstance()
{
	// clear something.
	if (!m_bNotLaunch)
		UnInit();

	// 清理随机数生成器
	RandomGen::UnInitialize();

	CMFCVisualManager::DestroyInstance(TRUE); // 如果不调用会导致大量的泄露。

	return CWinAppEx::ExitInstance();
}

void ClApp::Init()
{
	// init the database connection which will be used by app.
	DataBaseManager::GetInstance()->InitDB();

	// init data which opened recently.
	DBConnectPtr pMainDBCon = DataBaseManager::GetInstance()->GetMainDBConnect();
	RecentDataListPtr pRecentDataList = RecentDataLoader::LoadRecentData(pMainDBCon);
	DataItemTracker::GetInstance()->SetRecentData(pRecentDataList);

	// hide all "prop" bar.
	UIManager::GetInstance()->GetBarManager()->HideAllBar();

	// update the UI.
	UIManager::GetInstance()->UpdateRecentDataMenu();

	// 检查工作区和零件仓库目录是否存在，不在就创建。
	DataCenterItemWriter::CheckWorkspaceFolder(pMainDBCon);
	DataCenterItemWriter::CheckPartLibraryFolder(pMainDBCon);

	// to be commented, 检查数据库，看看要不要修复，正常运行时不需下面的代码。
	{
		//ConfigItemWriter::CheckData_of_toolAssignInfo(DataBaseManager::GetInstance()->GetExpLibDBConnect());
	}
}

void ClApp::UnInit()
{
	// release UI resource.
	delete UIManager::GetInstance();


	/************************************************************************/
	// 检查是否要备份文件。

	CString strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);

	// some params.
	ProductParamPtr pProductParam = CLOptionInfo::GetInstance()->GetProductParam();
	int iBkInterval = pProductParam->GetDBBackupDay();
	int iLogBkSize = pProductParam->GetLogBackupSize();

	// get the db backup date, if not backup yet, update the date.
	LONGLONG iDBBackupDate = LogMgr::GetDBBackupDate();
	if (iDBBackupDate == INVALID_VALUE)
	{
		iDBBackupDate = DateHelper::GetCurrentTime();
		LogMgr::UpdateDBBackupDate(iDBBackupDate);
	}

	LONGLONG iDayIterval = DateHelper::GetIntervalDay(iDBBackupDate, DateHelper::GetCurrentTime());
	int iLogFileSize = LogMgr::GetLogFileSize();
	CString strBKAppPath = ClFolderHelper::GetProductPath();
	strBKAppPath += _T(DATA_BK_EXE_FILE);
	if (iDayIterval >= iBkInterval && iLogFileSize >= iLogBkSize)
	{
		if (MessageBox(NULL, MultiLanguageMgr::GetInstance()->TranslateString(IDS_BACKUP_DB_AND_LOG), strProductName, MB_YESNO | MB_ICONQUESTION) == IDYES)
			ProcessMgr::RunProcess(strBKAppPath);
	}
	else if (iDayIterval >= iBkInterval)
	{
		if (MessageBox(NULL, MultiLanguageMgr::GetInstance()->TranslateString(IDS_BACKUP_DB), strProductName, MB_YESNO | MB_ICONQUESTION) == IDYES)
			ProcessMgr::RunProcess(strBKAppPath);
	}
	else if (iLogFileSize >= iLogBkSize)
	{
		if (MessageBox(NULL, MultiLanguageMgr::GetInstance()->TranslateString(IDS_BACKUP_LOG), strProductName, MB_YESNO | MB_ICONQUESTION) == IDYES)
			ProcessMgr::RunProcess(strBKAppPath);
	}
	/************************************************************************/


	// update the RecentData list to database and release the data tracker.
	RecentDataListPtr pRecentDataList = DataItemTracker::GetInstance()->GetRecentData();
	DBConnectPtr pMainDBCon = DataBaseManager::GetInstance()->GetMainDBConnect();
	RecentDataWriter::SaveRecentData(pMainDBCon, pRecentDataList);
	delete DataItemTracker::GetInstance();

	// release database.
	DataBaseManager* pDataBaseMgr = DataBaseManager::GetInstance();
	pDataBaseMgr->CloseDB();
	delete pDataBaseMgr;

	// 清理掉翻译库
	MultiLanguageMgr* pMultiLanguageMgr = MultiLanguageMgr::GetInstance();
	delete pMultiLanguageMgr;

	// 清理和释放命令
	CommandManager* pCommandManager = CommandManager::GetInstance();
	delete pCommandManager;

	// 释放键盘输入管理器
	ValueInputManager* pValueInputManager = ValueInputManager::GetInstance();
	delete pValueInputManager;

	// 释放“切割节点模拟器”。
	CutNodeSimulator* pCutNodeSimulator = CutNodeSimulator::GetInstance();
	if (!pCutNodeSimulator->IsFinished()) {
		pCutNodeSimulator->StopSimulation();
	}
	delete pCutNodeSimulator;

	ForegroundCamConnector::GetInstance()->EndWatchThread();

	// 释放零件在位编辑管理器。
	PartInplaceEditData* pPartInplaceEditData = PartInplaceEditData::GetInstance();
	delete pPartInplaceEditData;
}

void ClApp::OnDataMgr()
{
	DataCenterDlg dataMgrDlg;

	// navigate to the last select data node.
	LONGLONG iLastSelNodeID = DataItemTracker::GetInstance()->GetLastSelNodeID();
	dataMgrDlg.InitNaviNodeID(iLastSelNodeID);

	// display the data management window.
	if (dataMgrDlg.DoModal() == IDC_BTN_EXPLORER_OPEN)
	{
		DataItemPtr pDataItem = dataMgrDlg.GetOpeningDataItem();

		// open the data item.
		if (pDataItem->GetDataType() == DATANODE_PART)
		{
			if (PartInplaceEditData::GetInstance()->GetIsUnderInPlaceEditMode()) {
				DocManager::OpenPartDocUnderPartInplaceEditMode(pDataItem, m_pPartDocTemplate);
			} else {
				DocManager::OpenPartDoc(pDataItem, m_pPartDocTemplate);
			}
		}
		else if (pDataItem->GetDataType() == DATANODE_Task)
		{
			if (CLOptionInfo::GetInstance()->GetProductParam()->GetIsRunAsBackgroundApp()) // 此时每次只能打开一个任务对象。
				DocManager::OpenTaskDocBySingleDocMode(pDataItem, m_pTaskDocTemplate);
			else
				DocManager::OpenTaskDoc(pDataItem, m_pTaskDocTemplate);
		}
	}
}

void ClApp::OnNewEmptyPart()
{
	if (PartInplaceEditData::GetInstance()->GetIsUnderInPlaceEditMode())
		DocManager::NewPartDocUnderPartInplaceEditMode(m_pPartDocTemplate);
	else
		DocManager::NewPartDoc(m_pPartDocTemplate);
}

void ClApp::OnNewPartFromDxfdwg()
{
	if (PartInplaceEditData::GetInstance()->GetIsUnderInPlaceEditMode()) {
		DocManager::NewPartDocFromDxfDwgUnderPartInplaceEditMode(m_pPartDocTemplate);
	} else {
		DocManager::NewPartDocFromDxfDwg(m_pPartDocTemplate);
	}
}

void ClApp::OnNewTaskFromEmptySheet()
{
	if (CLOptionInfo::GetInstance()->GetProductParam()->GetIsRunAsBackgroundApp()) // 此时每次只能打开一个任务对象。
		DocManager::NewTaskDocBySingleDocMode(m_pTaskDocTemplate);
	else
		DocManager::NewTaskDoc(m_pTaskDocTemplate);
}

void ClApp::OnSetDefaultParam()
{
	DocManager::SetDefaultParam();
}

void ClApp::OnNewTaskFromNesting()
{
	bool bSingleDocMode = false;
	if (CLOptionInfo::GetInstance()->GetProductParam()->GetIsRunAsBackgroundApp())
		bSingleDocMode = true;
	DocManager::NewTaskDocFromNesting(m_pTaskDocTemplate, bSingleDocMode);
}

void ClApp::OnFileSave()
{
	CLeaderDoc* pDoc = DocManager::GetActiveDocument();

	// whether current doc is part.
	PartDocument* pPartDoc = dynamic_cast<PartDocument*>(pDoc);
	if (pPartDoc)
	{
		pPartDoc->OnSaveDocument();
	}

	// whether current doc is task.
	TaskDocument* pTaskDoc = dynamic_cast<TaskDocument*>(pDoc);
	if (pTaskDoc)
	{
		pTaskDoc->OnSaveDocument();
	}
}

void ClApp::OnFileSaveAs()
{
	CLeaderDoc* pDoc = DocManager::GetActiveDocument();

	// whether current doc is part.
	PartDocument* pPartDoc = dynamic_cast<PartDocument*>(pDoc);
	if (pPartDoc)
	{
		DocManager::SaveAsPart(pPartDoc);
	}

	// whether current doc is task.
	TaskDocument* pTaskDoc = dynamic_cast<TaskDocument*>(pDoc);
	if (pTaskDoc)
	{
		DocManager::SaveAsTask(pTaskDoc);
	}
}

BOOL ClApp::OnOpenRecentData(UINT nID)
{
	// the RecentData to be opened.
	RecentDataListPtr pRecentDataList = DataItemTracker::GetInstance()->GetRecentData();
	RecentDataPtr pRecentData = pRecentDataList->at(nID - (IDC_RECENT_DATA_BASE));

	/************************************************************************/
	// get the DataItem object and open it.

	// get the DataItem object.
	DBConnectPtr pMainDBCon = DataBaseManager::GetInstance()->GetMainDBConnect();
	FolderNodePtr pFolderNode = DataCenterItemLoader::LoadProductTree(pMainDBCon);
	DataCenterItemListPtr pAllDataItem(new DataCenterItemList);
	pFolderNode->GetAllDataItem(pAllDataItem);
	IDataCenterItemPtr pDataCenterItem = pAllDataItem->GetItemByDataID(pRecentData->GetDataID());

	// whether exist.
	if (!pDataCenterItem)
	{
		// if the data not exist, delete it from the recent data list.
		pRecentDataList->RemoveItemByID(pRecentData->GetID());

		// prompt user.
		CString strProductName, strMsgContent;
		strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);
		strMsgContent = MultiLanguageMgr::GetInstance()->TranslateString(IDS_DATA_NOT_FIND);
		strMsgContent.Replace(_T("WILLBEREPLACED1"), pRecentData->GetDataPath());
		MessageBox(GetMainWnd()->m_hWnd, strMsgContent, strProductName, MB_OK | MB_ICONINFORMATION);
	}
	else
	{
		// open the data.
		DataItemPtr pDataItem = boost::dynamic_pointer_cast<DataItem>(pDataCenterItem);
		if (pDataItem->GetDataType() == DATANODE_PART)
		{
			if (CLOptionInfo::GetInstance()->GetProductParam()->GetIsRunAsBackgroundApp()) // 处于后台模式。
			{
				if (PartInplaceEditData::GetInstance()->GetIsUnderInPlaceEditMode()) { // 处于在位编辑零件模式。
					DocManager::OpenPartDocUnderPartInplaceEditMode(pDataItem, m_pPartDocTemplate);
				}
				else { // 不处于在位编辑零件模式。
					return TRUE;
				}
			}
			else { // 不处于后台模式。
				if (PartInplaceEditData::GetInstance()->GetIsUnderInPlaceEditMode()) { // 处于在位编辑零件模式。
					DocManager::OpenPartDocUnderPartInplaceEditMode(pDataItem, m_pPartDocTemplate);
				}
				else { // 不处于在位编辑零件模式。
					DocManager::OpenPartDoc(pDataItem, m_pPartDocTemplate);
				}
			}
		}
		else if (pDataItem->GetDataType() == DATANODE_Task)
		{
			if (CLOptionInfo::GetInstance()->GetProductParam()->GetIsRunAsBackgroundApp()) // 处于后台模式。
			{
				if (PartInplaceEditData::GetInstance()->GetIsUnderInPlaceEditMode()) { // 处于在位编辑零件模式。
					return TRUE;
				}
				else { // 不处于在位编辑零件模式。
					DocManager::OpenTaskDocBySingleDocMode(pDataItem, m_pTaskDocTemplate);
				}
			}
			else { // 不处于后台模式。
				if (PartInplaceEditData::GetInstance()->GetIsUnderInPlaceEditMode()) { // 处于在位编辑零件模式。
					return TRUE;
				}
				else { // 不处于在位编辑零件模式。
					DocManager::OpenTaskDoc(pDataItem, m_pTaskDocTemplate);
				}
			}
		}
	}

	// update the UI.
	UIManager::GetInstance()->UpdateRecentDataMenu();
	/************************************************************************/

	return TRUE;
}

void ClApp::OnRecentData()
{
	// dummy.
	;
}

void ClApp::OnAbout()
{
	AboutDlg aboutDlg;
	aboutDlg.DoModal();
}

void ClApp::OnCLOption()
{
	CLOptionSheet clOptionSheet;
	clOptionSheet.DoModal();
}

void ClApp::OnMacLib()
{
	/************************************************************************/
	// first, make sure one instance of cam config app is running.

	int iProcessID = INVALID_ID;

	// get the exe path of cam config app.
	CString strCAMConfigPath = ClFolderHelper::GetProductPath();
	strCAMConfigPath += _T(KB_CONFIG_EXE_FILE);

	// check whether cam config app is running.
	BOOL bRunning = FALSE;
	IntegerListPtr pProcessIDList = ProcessMgr::IsProcessRun(strCAMConfigPath);
	if (pProcessIDList->size() > 0)
	{
		bRunning = TRUE;
		iProcessID = (int)pProcessIDList->at(0);
	}

	// if no instance of cam config app is running, run it.
	if (!bRunning)
		iProcessID = ProcessMgr::RunProcess(strCAMConfigPath);
	/************************************************************************/

	/************************************************************************/
	// we should send msg to that instance to activate the proper doc.

	// get the main frame of the running app.
	CString str;
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);
	HWND hMainWnd = ProcessMgr::GetProcessMainWnd(iProcessID, str);

	// send msg to the main frame of the running app.
	WndMsg wndMsg;
	wndMsg.iWndMsgType = WNDMSG_ACTIVE_DOC;
	wndMsg.iMessageValue = IDS_MAC_MGR;
	ProcessMgr::SendCopyMsg(hMainWnd, wndMsg);
	/************************************************************************/
}

void ClApp::OnExpertLib()
{
	/************************************************************************/
	// first, make sure one instance of cam config app is running.

	int iProcessID = INVALID_ID;

	// get the exe path of cam config app.
	CString strCAMConfigPath = ClFolderHelper::GetProductPath();
	strCAMConfigPath += _T(KB_CONFIG_EXE_FILE);

	// check whether cam config app is running.
	BOOL bRunning = FALSE;
	IntegerListPtr pProcessIDList = ProcessMgr::IsProcessRun(strCAMConfigPath);
	if (pProcessIDList->size() > 0)
	{
		bRunning = TRUE;
		iProcessID = (int)pProcessIDList->at(0);
	}

	// if no instance of cam config app is running, run it.
	if (!bRunning)
		iProcessID = ProcessMgr::RunProcess(strCAMConfigPath);
	/************************************************************************/

	/************************************************************************/
	// we should send msg to that instance to activate the proper doc.

	// get the main frame of the running app.
	CString str;
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);
	HWND hMainWnd = ProcessMgr::GetProcessMainWnd(iProcessID, str);

	// send msg to the main frame of the running app.
	WndMsg wndMsg;
	wndMsg.iWndMsgType = WNDMSG_ACTIVE_DOC;
	wndMsg.iMessageValue = IDS_EXPERTLIB_MGR;
	ProcessMgr::SendCopyMsg(hMainWnd, wndMsg);
	/************************************************************************/
}

void ClApp::OnMatLib()
{
	/************************************************************************/
	// first, make sure one instance of cam config app is running.

	int iProcessID = INVALID_ID;

	// get the exe path of cam config app.
	CString strCAMConfigPath = ClFolderHelper::GetProductPath();
	strCAMConfigPath += _T(KB_CONFIG_EXE_FILE);

	// check whether cam config app is running.
	BOOL bRunning = FALSE;
	IntegerListPtr pProcessIDList = ProcessMgr::IsProcessRun(strCAMConfigPath);
	if (pProcessIDList->size() > 0)
	{
		bRunning = TRUE;
		iProcessID = (int)pProcessIDList->at(0);
	}

	// if no instance of cam config app is running, run it.
	if (!bRunning)
		iProcessID = ProcessMgr::RunProcess(strCAMConfigPath);
	/************************************************************************/

	/************************************************************************/
	// we should send msg to that instance to activate the proper doc.

	// get the main frame of the running app.
	CString str;
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);
	HWND hMainWnd = ProcessMgr::GetProcessMainWnd(iProcessID, str);

	// send msg to the main frame of the running app.
	WndMsg wndMsg;
	wndMsg.iWndMsgType = WNDMSG_ACTIVE_DOC;
	wndMsg.iMessageValue = IDS_MATLIB_MGR;
	ProcessMgr::SendCopyMsg(hMainWnd, wndMsg);
	/************************************************************************/
}

void ClApp::OnDflReportTpl()
{
	DflTplDlg dlg;
	dlg.DoModal();
}

void ClApp::OnDesignReportTpl()
{
	int iProcessID = INVALID_ID;

	// 报表模板设计器的路径。
	CString strPath = ClFolderHelper::GetProductPath();
	strPath += _T(TPL_DESIGN_EXE_FILE);

	// check whether cam config app is running.
	BOOL bRunning = FALSE;
	IntegerListPtr pProcessIDList = ProcessMgr::IsProcessRun(strPath);
	if (pProcessIDList->size() > 0)
	{
		bRunning = TRUE;
		iProcessID = (int)pProcessIDList->at(0);
	}

	// if no instance of cam config app is running, run it.
	if (!bRunning)
		iProcessID = ProcessMgr::RunProcess(strPath);
}

void ClApp::OnNewTaskAndAddDxf2Canvas(WPARAM wParam, LPARAM lParam)
{
	CString* pString = (CString*)wParam;
	CString strDxfFilePath = *pString;
	delete pString;

	DocManager::NewTaskAndAddDxf2Canvas(m_pTaskDocTemplate, strDxfFilePath);
}

void ClApp::OnOpenSpecifiedSheet(WPARAM wParam, LPARAM lParam)
{
	pair<long long, long long>* pTaskAndSheetID = (pair<long long, long long>*)wParam;
	long long iTaskID = pTaskAndSheetID->first;
	long long iSheetID = pTaskAndSheetID->second;
	delete pTaskAndSheetID;

	DocManager::OpenSpecifiedSheet(m_pTaskDocTemplate, iTaskID, iSheetID);
}

void ClApp::OnInPlaceEditPart(WPARAM wParam, LPARAM lParam)
{
	if (!PartInplaceEditData::GetInstance()->GetIsUnderInPlaceEditMode()) {
		LogMgr::DebugWarn(_T("456211"));
		return;
	}

	// 打开零件进行在位编辑。
	PartInplaceEditManager::OpenPartDocByInPlaceEditMode(m_pPartDocTemplate);
}

void ClApp::OnUpdateUI_Exit(CCmdUI* pCmdUI)
{
	if (CLOptionInfo::GetInstance()->GetProductParam()->GetIsRunAsBackgroundApp() ||
		PartInplaceEditData::GetInstance()->GetIsUnderInPlaceEditMode()) // 禁用“退出”按钮。
	{
		pCmdUI->Enable(FALSE);
	} else {
		pCmdUI->Enable(TRUE);
	}
}

void ClApp::OnUpdateUI_Save(CCmdUI* pCmdUI)
{
	if (PartInplaceEditData::GetInstance()->GetIsUnderInPlaceEditMode()) // 禁用“保存”按钮。
	{
		pCmdUI->Enable(FALSE);
	}
	else {
		pCmdUI->Enable(TRUE);
	}
}

void ClApp::OnUpdateUI_SaveAs(CCmdUI* pCmdUI)
{
	if (PartInplaceEditData::GetInstance()->GetIsUnderInPlaceEditMode()) // 禁用“另存”按钮。
	{
		pCmdUI->Enable(FALSE);
	}
	else {
		pCmdUI->Enable(TRUE);
	}
}

void ClApp::BeginSplash()
{
	// Bring up the splash screen in a secondary UI thread
	m_pSplashThread = (SplashThread*)AfxBeginThread(RUNTIME_CLASS(SplashThread), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	ASSERT(m_pSplashThread->IsKindOf(RUNTIME_CLASS(SplashThread)));

	m_pSplashThread->SetBitmapToUse(IDB_SPLASHER);

	// Resume the thread now that we have set it up 
	m_pSplashThread->ResumeThread();

	// Simulate some slow loading process.
	Sleep(1000);
}

void ClApp::EndSplash()
{
	m_pSplashThread->HideSplash();
}

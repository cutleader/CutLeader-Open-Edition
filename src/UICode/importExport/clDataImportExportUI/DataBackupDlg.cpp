#include "StdAfx.h"
#include "DataBackupDlg.h"

#include "baseConst.h"
#include "IntegerList.h"
#include "ProcessMgr.h"
#include "ProgressData.h"
#include "ProgressDlg.h"
#include "baseConst.h"
#include "clBaseDataResource.h"
#include "ProductFileDefine.h"
#include "ClFolderHelper.h"
#include "Uxtheme.h"
#pragma comment(lib, "Uxtheme.lib")

#include "ProductParam.h"
#include "CLOptionInfo.h"
#include "clOptionResource.h"

#include "BackupTask.h"
#include "RecoveryTask.h"
#include "clDataImportExportResource.h"


BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(DataBackupDlg, CDialogEx)

DataBackupDlg::DataBackupDlg() : CLDialog(DataBackupDlg::IDD)
{
	m_strMsg = MultiLanguageMgr::GetInstance()->TranslateString(IDS_BK_IN_SAFE_LOC);
}

DataBackupDlg::~DataBackupDlg()
{
}

void DataBackupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_BK_MSG, m_strMsg);

	DDX_Check(pDX, IDC_CHECK_DB, m_bBackupDB);
	DDX_Check(pDX, IDC_CHECK_LOG, m_bBackupLog);

	DDX_Text(pDX, IDC_EDIT_BK_LOCATION, m_strBackupDir);

	DDX_Control( pDX, IDC_BTN_CHANGE_LOC, m_btnChangeLog );
	DDX_Control( pDX, IDC_BTN_BACKUP, m_btnBackup );
	DDX_Control( pDX, IDC_BTN_RECOVERY, m_btnRecovery );
	DDX_Control( pDX, IDC_BTN_CANCEL, m_btnCancel );
}

BOOL DataBackupDlg::OnInitDialog() 
{
	__super::OnInitDialog();

	// init the option information
	ProductParamPtr pProductParam = CLOptionInfo::GetInstance()->GetProductParam();
	m_strBackupDir = pProductParam->GetBkLocation();

	SetBackgroundColor( SkinManager::GetBackgroundColor() );

	SetWindowTheme( GetDlgItem( IDC_STATIC_BACKUP_OPTION )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_EDIT_BK_MSG )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_CHECK_DB )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_CHECK_LOG )->GetSafeHwnd(), L"", L"" );

	UpdateData(FALSE);

	return TRUE;
}

BEGIN_MESSAGE_MAP(DataBackupDlg, CDialogEx)
	ON_WM_CTLCOLOR()

	ON_BN_CLICKED(IDC_BTN_CHANGE_LOC, OnSelectDir)
	ON_BN_CLICKED(IDC_BTN_BACKUP, OnBackup)
	ON_BN_CLICKED(IDC_BTN_RECOVERY, OnRecovery)
	ON_BN_CLICKED(IDC_BTN_CANCEL, OnCancel)
END_MESSAGE_MAP()

HBRUSH DataBackupDlg::OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor )
{
	return SkinManager::GetDialogCtrlColor( pDC, pWnd, nCtlColor );
}

void DataBackupDlg::OnSelectDir()
{
	CString str, strFolder;
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_SEL_BK_FOLDER);

	// select the folder.
	BROWSEINFO bi;
	memset(&bi, 0, sizeof(BROWSEINFO));
	bi.hwndOwner = GetSafeHwnd();
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT;
	bi.lpszTitle = str;
	LPITEMIDLIST idl = SHBrowseForFolder(&bi);
	if (idl == NULL)
		return;
	SHGetPathFromIDList(idl, strFolder.GetBuffer(MAX_PATH));
	strFolder.ReleaseBuffer();

	// update UI.
	m_strBackupDir = strFolder;
	UpdateData(FALSE);
}

void DataBackupDlg::OnBackup()
{
	UpdateData(TRUE);

	CString str, strProductName;
	strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);

	/************************************************************************/	
	// check the user selections.

	if (!m_bBackupDB && !m_bBackupLog)
	{
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_BK_SEL_BK_FOLDER);
		MessageBox(str, strProductName, MB_OK | MB_ICONWARNING);
		return;
	}

	// check whether the backup folder exist.
	if (!PathFileExists(m_strBackupDir))
	{
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_BK_FOLDER_NOT_FOUND);
		MessageBox(str, strProductName, MB_OK | MB_ICONWARNING);
		return;
	}
	/************************************************************************/

	// make sure the apps are closed.
	CString strMainAppPath = ClFolderHelper::GetProductPath();
	strMainAppPath += _T(CAM_EXE_FILE);
	CString strCAMConfigPath = ClFolderHelper::GetProductPath();
	strCAMConfigPath += _T(KB_CONFIG_EXE_FILE);
	IntegerListPtr pProcessIDList1 = ProcessMgr::IsProcessRun(strMainAppPath);
	IntegerListPtr pProcessIDList2 = ProcessMgr::IsProcessRun(strCAMConfigPath);
	if (pProcessIDList1->size() != 0 || pProcessIDList2->size() != 0)
	{
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SHUTDOWN_ALL_APP);
		MessageBox(str, strProductName, MB_OK | MB_ICONWARNING);
		return;
	}

	/************************************************************************/
	// create a task to backup data.

	// init the task.
	BackupTaskPtr pBackupTask(new BackupTask(m_bBackupDB, m_bBackupLog, m_strBackupDir));

	// display the progress of the task.
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_BACKUP_TASK);
	ProgressDlg progressDlg;
	progressDlg.Init(str, pBackupTask);
	progressDlg.DoModal();
	/************************************************************************/
}

void DataBackupDlg::OnRecovery()
{
	CString str, strProductName;
	strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);

	// make sure the apps are closed.
	CString strMainAppPath = ClFolderHelper::GetProductPath();
	strMainAppPath += _T(CAM_EXE_FILE);
	CString strCAMConfigPath = ClFolderHelper::GetProductPath();
	strCAMConfigPath += _T(KB_CONFIG_EXE_FILE);
	IntegerListPtr pProcessIDList1 = ProcessMgr::IsProcessRun(strMainAppPath);
	IntegerListPtr pProcessIDList2 = ProcessMgr::IsProcessRun(strCAMConfigPath);
	if (pProcessIDList1->size() != 0 || pProcessIDList2->size() != 0)
	{
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SHUTDOWN_ALL_APP);
		MessageBox(str, strProductName, MB_OK | MB_ICONWARNING);
		return;
	}

	// display the "import" dialog.
	CString strFilter, strDflExt;
	strFilter = MultiLanguageMgr::GetInstance()->TranslateString(IDS_IMPEXP_BK_DB_FILEEXT);
	strDflExt = MultiLanguageMgr::GetInstance()->TranslateString(IDS_IMPEXP_BK_DB_EXT);
	CFileDialog impFileDlg(FALSE, strDflExt, NULL, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, strFilter);
	if (impFileDlg.DoModal() == IDOK)
	{
		// get the file which will be imported.
		CString strFilePath = impFileDlg.GetPathName();

		/************************************************************************/
		// create a task to auto sequence.

		// init the task.
		RecoveryTaskPtr pRecoveryTask(new RecoveryTask(strFilePath));

		// display the progress of the task.
		CString str;
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_RECOVERY_TASK);
		ProgressDlg progressDlg;
		progressDlg.Init(str, pRecoveryTask);
		progressDlg.DoModal();
		/************************************************************************/
	}
}

void DataBackupDlg::OnCancel()
{
	CDialogEx::OnCancel();
}

void DataBackupDlg::Init(BOOL bBackupDB, BOOL bBackupLog)
{
	m_bBackupDB = bBackupDB;
	m_bBackupLog = bBackupLog;
}

END_CUTLEADER_NAMESPACE()

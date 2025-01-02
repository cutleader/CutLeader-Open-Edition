#include "StdAfx.h"
#include "AboutDlg.h"

#include "baseConst.h"
#include "clUtilityResource.h"
#include "DBConnect.h"
#include "versionInfo.h"
#include "LogMgr.h"
#include "clBaseDataResource.h"
#include "clBaseDataUIResource.h"
#include "SkinManager.h"

#include "Uxtheme.h"
#pragma comment(lib, "Uxtheme.lib")

BEGIN_CUTLEADER_NAMESPACE()

AboutDlg::AboutDlg() : CLDialog(AboutDlg::IDD)
{
}

void AboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BTN_PRODUCT_NAME, m_btnProductName);

	DDX_Control(pDX, IDC_STATIC_APP_VER, m_appVersion);
	DDX_Control(pDX, IDC_STATIC_DB_VER, m_dbVersion);
	DDX_Control(pDX, IDC_STATIC_PART_VER, m_partVersion);
	DDX_Control(pDX, IDC_STATIC_TASK_VER, m_taskVersion);
	DDX_Control(pDX, IDC_STATIC_DRI_VER, m_driVersion);
	DDX_Control(pDX, IDC_STATIC_EXPLIB_VER, m_expLibVersion);
	DDX_Control(pDX, IDC_STATIC_MATLIB_VER, m_matLibVersion);
	DDX_Control(pDX, IDC_STATIC_RPTLIB_VER, m_rptLibVersion);
}

BEGIN_MESSAGE_MAP(AboutDlg, CDialogEx)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

BOOL AboutDlg::OnInitDialog()
{
	__super::OnInitDialog();

	m_btnProductName.LoadBitmaps(IDB_ProductName_OpenEdition);

	// app version.
	CString strAppVer;
	strAppVer = MultiLanguageMgr::GetInstance()->TranslateString(IDS_APP_VERSION);
	strAppVer.Replace(_T("WILLBEREPLACED1"), _T("<font size=\"+5\">"));
	strAppVer.Replace(_T("WILLBEREPLACED2"), APP_VERSION);
	m_appVersion.SetWindowText(strAppVer);
	InitXHTMLStaticStyle( &m_appVersion );

	// database version.
	CString strDBVer;
	strDBVer = MultiLanguageMgr::GetInstance()->TranslateString(IDS_DB_VERSION);
	strDBVer.Replace(_T("WILLBEREPLACED1"), DB_VERSION);
	m_dbVersion.SetWindowText(strDBVer);
	InitXHTMLStaticStyle( &m_dbVersion );
	
	// 零件版本。
	CString strPartVer;
	strPartVer = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PART_VERSION);
	strPartVer.Replace(_T("WILLBEREPLACED1"), PART_VERSION);
	m_partVersion.SetWindowText(strPartVer);
	InitXHTMLStaticStyle( &m_partVersion );
	
	// 任务版本。
	CString strShtVer;
	strShtVer = MultiLanguageMgr::GetInstance()->TranslateString(IDS_Task_VERSION);
	strShtVer.Replace(_T("WILLBEREPLACED1"), TASK_VERSION);
	m_taskVersion.SetWindowText(strShtVer);
	InitXHTMLStaticStyle( &m_taskVersion );
	
	// driver version.
	CString strDriVer;
	strDriVer = MultiLanguageMgr::GetInstance()->TranslateString(IDS_DRI_VERSION);
	strDriVer.Replace(_T("WILLBEREPLACED1"), DRIVER_VERSION);
	m_driVersion.SetWindowText(strDriVer);
	InitXHTMLStaticStyle( &m_driVersion );
	
	// 专家库版本。
	CString strExpLibVer;
	strExpLibVer = MultiLanguageMgr::GetInstance()->TranslateString(IDS_EXPLIB_VERSION);
	strExpLibVer.Replace(_T("WILLBEREPLACED1"), EXPLIB_VERSION);
	m_expLibVersion.SetWindowText(strExpLibVer);
	InitXHTMLStaticStyle( &m_expLibVersion );
	
	// 材料库版本。
	CString strMatLibVer;
	strMatLibVer = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MATLIB_VERSION);
	strMatLibVer.Replace(_T("WILLBEREPLACED1"), MATLIB_VERSION);
	m_matLibVersion.SetWindowText(strMatLibVer);
	InitXHTMLStaticStyle( &m_matLibVersion );
	
	// 报表库版本。
	CString strRptLibVer;
	strRptLibVer = MultiLanguageMgr::GetInstance()->TranslateString(IDS_RPTLIB_VERSION);
	strRptLibVer.Replace(_T("WILLBEREPLACED1"), RPTLIB_VERSION);
	m_rptLibVersion.SetWindowText(strRptLibVer);
	InitXHTMLStaticStyle( &m_rptLibVersion );
	
	UpdateData(FALSE);

	SetBackgroundColor( SkinManager::GetBackgroundColor() );

	SetWindowTheme( GetDlgItem( IDC_STATIC_LICENSE_INFO )->GetSafeHwnd(), L"", L"" );
	return TRUE;
}

HBRUSH AboutDlg::OnCtlColor( CDC* pDC, CWnd* pWnd, UINT nCtlColor )
{
	return SkinManager::GetDialogCtrlColor( pDC, pWnd, nCtlColor );
}

void AboutDlg::InitXHTMLStaticStyle( XHTMLStatic *TargetXHTMLStatic )
{
	TargetXHTMLStatic->SetBkColor( SkinManager::GetBackgroundColor() );
	TargetXHTMLStatic->SetTextColor( SkinManager::GetTextColor() );
}

END_CUTLEADER_NAMESPACE()

#include "stdafx.h"
#include "ProductParamPage.h"

#include <set>
#include "baseConst.h"
#include "baseConst.h"
#include "clUILibResource.h"
#include "clUtilityResource.h"
#include "clBaseDataResource.h"
#include "MultiLanguageMgr.h"
#include "ContentTranslationDlg.h"

#include "ProductParam.h"
#include "ImpExpConfig.h"
#include "clOptionResource.h"

#include "Uxtheme.h"
#pragma comment(lib, "Uxtheme.lib")

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const size_t MAX_TIP_TEXT_LENGTH = 1024;

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(ProductParamPage, CResizableDialog)

ProductParamPage::ProductParamPage() : CLResizableDialog ( ProductParamPage::IDD )
{
}

ProductParamPage::~ProductParamPage()
{
}

void ProductParamPage::DoDataExchange( CDataExchange* pDX )
{
	CResizableDialog::DoDataExchange( pDX );

	DDX_Control( pDX, IDC_COMBO_UNIT, m_ctrlDimUnit );
	DDX_Control( pDX, IDC_COMBO_LANG, m_ctrlLang );

	DDX_Text( pDX, IDC_EDIT_DB_BK_LOC, m_strBackupDir );
	DDX_Control( pDX, IDC_EDIT_DB_BK_DAY, m_dDbBackUpDay );
	DDX_Control( pDX, IDC_EDIT_LOG_BK_SIZE, m_dLogFileSize );
	DDX_Control( pDX, IDC_BTN_TRANSLATE, m_btnTranslate );
	DDX_Control( pDX, IDC_BTN_CHANGE_LOC, m_btnChangeLocation );
}

BEGIN_MESSAGE_MAP( ProductParamPage, CResizableDialog )
	ON_WM_CTLCOLOR()

	ON_CBN_SELCHANGE( IDC_COMBO_LANG, OnSelchangeLanguage )
	ON_BN_CLICKED( IDC_BTN_CHANGE_LOC, OnSelectDir )
	ON_BN_CLICKED( IDC_BTN_TRANSLATE, OnTranslate )
END_MESSAGE_MAP()

BOOL ProductParamPage::OnInitDialog()
{
	CLResizableDialog::OnInitDialog();


	/************************************************************************/
	// layout controls.

	// "general param" group.
	AddAnchor(IDC_GROUP_SYS_PARAM, TOP_LEFT, TOP_RIGHT);
	// unit.
	AddAnchor(IDC_TXT_UNIT, TOP_LEFT);
	AddAnchor(IDC_COMBO_UNIT, TOP_LEFT);
	// language
	AddAnchor(IDC_TXT_LANG, TOP_LEFT);
	AddAnchor(IDC_COMBO_LANG, TOP_LEFT);
    AddAnchor(IDC_BTN_TRANSLATE, TOP_LEFT);

	// "data backup" group.
	AddAnchor(IDC_GROUP_BK_PARAM, TOP_LEFT, TOP_RIGHT);
	// backup location
	AddAnchor(IDC_TXT_DB_BK_LOC, TOP_LEFT);
	AddAnchor(IDC_EDIT_DB_BK_LOC, TOP_LEFT);
	AddAnchor(IDC_BTN_CHANGE_LOC, TOP_LEFT);
	// database
	AddAnchor(IDC_TXT_DB_BK, TOP_LEFT);
	AddAnchor(IDC_EDIT_DB_BK_DAY, TOP_LEFT);
	AddAnchor(IDC_STATIC_DB_BK_DAY, TOP_LEFT);
	// log file.
	AddAnchor(IDC_TXT_LOG_BK, TOP_LEFT);
	AddAnchor(IDC_EDIT_LOG_BK_SIZE, TOP_LEFT);
	AddAnchor(IDC_STATIC_LOG_BK_SIZE, TOP_LEFT);
	/************************************************************************/


	SetBackgroundColor( SkinManager::GetBackgroundColor() );
	
	SetWindowTheme( GetDlgItem( IDC_GROUP_SYS_PARAM )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_COMBO_UNIT )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_COMBO_LANG )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme(GetDlgItem(IDC_COMBO_StartupMode)->GetSafeHwnd(), L"", L"");
	SetWindowTheme( GetDlgItem( IDC_GROUP_BK_PARAM )->GetSafeHwnd(), L"", L"" );

	return TRUE;
}

HBRUSH ProductParamPage::OnCtlColor( CDC* pDC, CWnd* pWnd, UINT nCtlColor )
{
	return SkinManager::GetDialogCtrlColor( pDC, pWnd, nCtlColor );
}

void ProductParamPage::OnSelchangeLanguage()
{
	LANG_TYPE selLangType = (LANG_TYPE)m_ctrlLang.GetCurSel();
	if (selLangType != m_pProductParam->GetLang())
	{
		CString strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);
		CString strMsg = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_CHANGE_NEEDTO_RESTART);
		MessageBox(strMsg, strProductName, MB_OK | MB_ICONINFORMATION);
	}
}

void ProductParamPage::OnSelectDir()
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

void ProductParamPage::OnTranslate()
{
	if (m_pProductParam->GetLang() == LANG_ENG) {
		CString str = _T("Current language is english, TAOSoft has translated all strings. You can use translation function if switch to other languages.");
		CString strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);
		MessageBox(str, strProductName, MB_OK | MB_ICONINFORMATION);
	}
	else {
		ContentTranslationDlg dlg(this);
		dlg.Init(m_pProductParam->GetLang());
		dlg.DoModal();
	}
}

void ProductParamPage::DisplayProductParam(ProductParamPtr pProductParam)
{
	m_pProductParam = pProductParam;

	/************************************************************************/	
	// display the unit type

	CString str1, str2;
	str1 = MultiLanguageMgr::GetInstance()->TranslateString(IDS_INCH);
	str2 = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MM);
	m_ctrlDimUnit.ResetContent();
	m_ctrlDimUnit.AddString(str1);
	m_ctrlDimUnit.AddString(str2);

	if (m_pProductParam->GetDimUnitType() == (int)DIM_UNIT_INCH)
		m_ctrlDimUnit.SetCurSel(0);
	else if (m_pProductParam->GetDimUnitType() == (int)DIM_UNIT_MM)
		m_ctrlDimUnit.SetCurSel(1);
	/************************************************************************/

	// 显示语言信息
    {
        // 列出所有支持的语言（没有翻译的不管）
        map<LANG_TYPE, int> allSupportedLangs = MultiLanguageMgr::GetAllSupportedLangs();
        m_ctrlLang.ResetContent();
        for (map<LANG_TYPE, int>::const_iterator iter = allSupportedLangs.begin(); iter != allSupportedLangs.end(); iter++)
        {
            CString strLanguageName = MultiLanguageMgr::GetInstance()->TranslateString(iter->second);
            m_ctrlLang.AddString(strLanguageName);
        }

        // 当前语言
        m_ctrlLang.SetCurSel((int)m_pProductParam->GetLang());
    }

	// for backup
	m_strBackupDir = m_pProductParam->GetBkLocation();
	m_dDbBackUpDay.Init(m_pProductParam->GetDBBackupDay(), 0, FALSE);
	m_dLogFileSize.Init(m_pProductParam->GetLogBackupSize(), 0, FALSE);

	UpdateData(FALSE);
}

ProductParamPtr ProductParamPage::GetProductParam()
{
	UpdateData( TRUE );

	// dim.
	DIM_UNIT_TYPE selDimUnit;
	int nIndex = m_ctrlDimUnit.GetCurSel();
	if ( nIndex == 0 ) {
		selDimUnit = DIM_UNIT_INCH;
	}
	else if ( nIndex == 1 ) {
		selDimUnit = DIM_UNIT_MM;
	}
	if ( selDimUnit != m_pProductParam->GetDimUnitType() ) {
		m_pProductParam->SetDimUnitType( selDimUnit );
		m_pProductParam->SetModified( TRUE );
	}

	// language.
	LANG_TYPE selLangType = ( LANG_TYPE )m_ctrlLang.GetCurSel();
	if ( selLangType != m_pProductParam->GetLang() ) {
		m_pProductParam->SetLang( selLangType );
		m_pProductParam->SetModified( TRUE );
	}

	// for backup.
	if ( m_pProductParam->GetBkLocation() != m_strBackupDir ) {
		m_pProductParam->SetBkLocation( m_strBackupDir );
		m_pProductParam->SetModified( TRUE );
	}
	if ( m_pProductParam->GetDBBackupDay() != m_dDbBackUpDay.Get_Number() ) {
		m_pProductParam->SetDBBackupDay( ( int )m_dDbBackUpDay.Get_Number() );
		m_pProductParam->SetModified( TRUE );
	}
	if ( m_pProductParam->GetLogBackupSize() != m_dLogFileSize.Get_Number() ) {
		m_pProductParam->SetLogBackupSize( ( int )m_dLogFileSize.Get_Number() );
		m_pProductParam->SetModified( TRUE );
	}

	return m_pProductParam;
}

END_CUTLEADER_NAMESPACE()

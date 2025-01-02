#include "stdafx.h"
#include "SearchPartDlg.h"

#include "baseConst.h"
#include "FolderNode.h"
#include "DataCenterItemList.h"
#include "SelectDataCenterFolderDlg.h"
#include "DataCenterManager.h"

#include "Uxtheme.h"
#pragma comment(lib, "Uxtheme.lib")

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNAMIC(SearchPartDlg, CDialog)

BEGIN_MESSAGE_MAP(SearchPartDlg, CDialogEx)
	ON_WM_CTLCOLOR()

	ON_BN_CLICKED(IDC_CHECK_FOLDER, OnCheckFolderFilter)
	ON_BN_CLICKED(IDC_BTN_FOLDER, OnSelectFilterFolder)
	ON_BN_CLICKED(IDC_CHECK_NAME, OnCheckNameFilter)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

SearchPartDlg::SearchPartDlg(CWnd* pParent /*=NULL*/)
	: CLDialog(SearchPartDlg::IDD, pParent)
{
	m_bFolderFilter = FALSE;
	m_bNameFilter = FALSE;
}

SearchPartDlg::~SearchPartDlg()
{
}

void SearchPartDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Check(pDX, IDC_CHECK_FOLDER, m_bFolderFilter);
	DDX_Check(pDX, IDC_CHECK_NAME, m_bNameFilter);
	DDX_Control( pDX, IDOK, m_btnOk );
	DDX_Control( pDX, IDCANCEL, m_btnCancel );
	DDX_Control( pDX, IDC_BTN_FOLDER, m_btnFolder );
	DDX_Text(pDX, IDC_EDIT_PART_NAME, m_strNameFilter);
}

BOOL SearchPartDlg::OnInitDialog()
{
	__super::OnInitDialog();

	m_btnFolder.SetWindowText( m_pFilterFolder->GetFullPath() );
	m_btnFolder.EnableWindow( FALSE );

	((CWnd*)GetDlgItem(IDC_EDIT_PART_NAME))->EnableWindow(FALSE);

	SetBackgroundColor( SkinManager::GetBackgroundColor() );

	SetWindowTheme( GetDlgItem( IDC_STATIC_SEARCH_CONDITIONS )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_CHECK_FOLDER )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_CHECK_NAME )->GetSafeHwnd(), L"", L"" );

	return TRUE;
}

HBRUSH SearchPartDlg::OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor )
{
	return SkinManager::GetDialogCtrlColor( pDC, pWnd, nCtlColor );
}

void SearchPartDlg::OnCheckFolderFilter()
{
	UpdateData(TRUE);

	// init the folder filter.
	if ( m_bFolderFilter ) {
		m_btnFolder.EnableWindow( TRUE );
	}
	else {
		m_btnFolder.EnableWindow( FALSE );
	}
}

void SearchPartDlg::OnCheckNameFilter()
{
	UpdateData(TRUE);

	// init the part name filter.
	if ( m_bNameFilter ) {
		( ( CWnd *)GetDlgItem( IDC_EDIT_PART_NAME ) )->EnableWindow( TRUE );
	}
	else {
		( ( CWnd *)GetDlgItem( IDC_EDIT_PART_NAME ) )->EnableWindow( FALSE );
	}
}

void SearchPartDlg::OnSelectFilterFolder()
{
	SelectDataCenterFolderDlg dlg;
	if (dlg.DoModal() == IDOK) {
		m_pFilterFolder = dlg.GetSelectFolder();

		// get the path of the folder.
		CString strPath = m_pFilterFolder->GetItemPath();
		strPath += _T( "\\" );
		strPath += m_pFilterFolder->GetName();

		// put the folder path to the button.
		m_btnFolder.SetWindowText( strPath );
	}
}

void SearchPartDlg::OnBnClickedOk()
{
	UpdateData(TRUE);

	// name filter
	m_strNameFilter.Trim();
	if (m_bNameFilter && m_strNameFilter.GetLength() < 1)
		m_bNameFilter = FALSE;

	// search parts using the conditions.
	m_pProductItemList = DataCenterManager::SearchDataItem(m_pFilterFolder, TRUE, DATANODE_PART, m_bNameFilter, m_strNameFilter);

	OnOK();
}

END_CUTLEADER_NAMESPACE()

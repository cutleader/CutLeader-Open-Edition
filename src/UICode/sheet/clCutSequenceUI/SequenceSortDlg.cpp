#include "StdAfx.h"
#include "SequenceSortDlg.h"

#include "Uxtheme.h"
#pragma comment(lib, "Uxtheme.lib")

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(SequenceSortDlg, CDialogEx)

SequenceSortDlg::SequenceSortDlg()
		: CLDialog(SequenceSortDlg::IDD)
{
	m_bSortFromStart = FALSE;
}

SequenceSortDlg::~SequenceSortDlg(void)
{
}

void SequenceSortDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Check(pDX, IDC_CHECK_START, m_bSortFromStart);
	DDX_Control( pDX, IDOK, m_btnOk );
	DDX_Control( pDX, IDCANCEL, m_btnCancel );
}

BEGIN_MESSAGE_MAP(SequenceSortDlg, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

BOOL SequenceSortDlg::OnInitDialog()
{
	__super::OnInitDialog();
	
	SetBackgroundColor( SkinManager::GetBackgroundColor() );

	SetWindowTheme( GetDlgItem( IDC_CHECK_START )->GetSafeHwnd(), L"", L"");

	UpdateData(FALSE);

	return TRUE;
}

HBRUSH SequenceSortDlg::OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor )
{
	return SkinManager::GetDialogCtrlColor( pDC, pWnd, nCtlColor );
}

void SequenceSortDlg::OnBnClickedOk()
{
	UpdateData(TRUE);

	OnOK();
}

END_CUTLEADER_NAMESPACE()

#include "StdAfx.h"
#include "AllResultDlg.h"

#include "Uxtheme.h"
#pragma comment(lib, "Uxtheme.lib")

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(AllResultDlg, CDialogEx)

AllResultDlg::AllResultDlg()
		: CLDialog(AllResultDlg::IDD)
{
	m_iNestResultCount = 0;
	m_iSelectedResultIndex = -1;
}

AllResultDlg::~AllResultDlg(void)
{
}

void AllResultDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	
	DDX_Control(pDX, IDC_RESULT_LIST, m_resultList);
	DDX_Control(pDX, IDC_BTN_OK, m_btnOK);
	DDX_Control(pDX, IDC_BTN_CANCEL, m_btnCancel);
}

BOOL AllResultDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	for (unsigned int i = 0; i < m_iNestResultCount; i++)
	{
		CString str;
		str.Format(_T("Nest Result %d"), i+1);
		m_resultList.InsertString(i, str);
	}
	if (m_iNestResultCount > 0)
	{
		m_resultList.SetCurSel(0);
	}

	SetBackgroundColor( SkinManager::GetBackgroundColor() );

	SetWindowTheme( GetDlgItem( IDC_STATIC_NEST_RESULT_LIST )->GetSafeHwnd(), L"", L"" );

	UpdateData(FALSE);

	return TRUE;
}

BEGIN_MESSAGE_MAP(AllResultDlg, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_OPEN, OnBnClickedOk)
END_MESSAGE_MAP()

HBRUSH AllResultDlg::OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor )
{
	return SkinManager::GetDialogCtrlColor( pDC, pWnd, nCtlColor );
}

void AllResultDlg::OnBnClickedOk()
{
	m_iSelectedResultIndex = m_resultList.GetCurSel();
	if (m_iSelectedResultIndex < 0)
	{
		AfxMessageBox(_T("No nest result is selected."));
	}
	else
	{
		OnOK();
	}
}

END_CUTLEADER_NAMESPACE()
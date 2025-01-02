#include "StdAfx.h"
#include "NCConfigTypeNameDlg.h"

#include "baseConst.h"

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNAMIC(NCConfigTypeNameDlg, CDialog)

BEGIN_MESSAGE_MAP(NCConfigTypeNameDlg, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDOK, OnBnClickedOk)

END_MESSAGE_MAP()

NCConfigTypeNameDlg::NCConfigTypeNameDlg(CWnd* pParent /*=NULL*/)
						: CLDialog(NCConfigTypeNameDlg::IDD, pParent)
{
}

NCConfigTypeNameDlg::~NCConfigTypeNameDlg(void)
{
}

BOOL NCConfigTypeNameDlg::OnInitDialog()
{
	__super::OnInitDialog();

	SetBackgroundColor( SkinManager::GetBackgroundColor() );

	UpdateData(FALSE);

	return TRUE;
}

HBRUSH NCConfigTypeNameDlg::OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor )
{
	return SkinManager::GetDialogCtrlColor( pDC, pWnd, nCtlColor );
}

void NCConfigTypeNameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_NCCONFIG_NAME, m_strName);
	DDX_Control( pDX, IDOK, m_btnOk );
	DDX_Control( pDX, IDCANCEL, m_btnCancel );
}

void NCConfigTypeNameDlg::OnBnClickedOk()
{
	UpdateData(TRUE);

	OnOK();
}

END_CUTLEADER_NAMESPACE()

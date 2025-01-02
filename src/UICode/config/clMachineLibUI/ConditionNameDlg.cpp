#include "StdAfx.h"
#include "ConditionNameDlg.h"

#include "baseConst.h"

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNAMIC(ConditionNameDlg, CDialog)

BEGIN_MESSAGE_MAP(ConditionNameDlg, CDialogEx)
	ON_WM_CTLCOLOR()

	ON_BN_CLICKED(IDOK, OnBnClickedOk)

END_MESSAGE_MAP()

ConditionNameDlg::ConditionNameDlg(CWnd* pParent /*=NULL*/)
						: CLDialog(ConditionNameDlg::IDD, pParent)
{
}

ConditionNameDlg::~ConditionNameDlg(void)
{
}

BOOL ConditionNameDlg::OnInitDialog()
{
	__super::OnInitDialog();

	SetBackgroundColor( SkinManager::GetBackgroundColor() );

	UpdateData(FALSE);

	return TRUE;
}

HBRUSH ConditionNameDlg::OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor )
{
	return SkinManager::GetDialogCtrlColor( pDC, pWnd, nCtlColor );
}

void ConditionNameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_CONDNAME, m_strName);
	DDX_Control( pDX, IDOK, m_btnOk );
	DDX_Control( pDX, IDCANCEL, m_btnCancel );
}

void ConditionNameDlg::OnBnClickedOk()
{
	UpdateData(TRUE);

	OnOK();
}

END_CUTLEADER_NAMESPACE()

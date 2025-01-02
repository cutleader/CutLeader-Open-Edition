#include "StdAfx.h"
#include "ChangeLoopSizeDlg.h"

#include "baseConst.h"

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(ChangeLoopSizeDlg, CDialogEx)

ChangeLoopSizeDlg::ChangeLoopSizeDlg()
	: CLDialog(ChangeLoopSizeDlg::IDD)
{
	m_dLoopWidth = 50.0;
	m_dLoopHeight = 50.0;
}

ChangeLoopSizeDlg::~ChangeLoopSizeDlg(void)
{
}

void ChangeLoopSizeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_EDIT_LoopWidth, m_ctrlLoopWidth);
	DDX_Control(pDX, IDC_EDIT_LoopHeight, m_ctrlLoopHeight);

	DDX_Control( pDX, IDOK, m_btnOk );
	DDX_Control( pDX, IDCANCEL, m_btnCancel );
}

BOOL ChangeLoopSizeDlg::OnInitDialog()
{
	__super::OnInitDialog();

	m_ctrlLoopWidth.Init_WithoutMsg(m_dLoopWidth, 4, false, 0.1, 0.1);
	m_ctrlLoopHeight.Init_WithoutMsg(m_dLoopHeight, 4, false, 0.1, 0.1);

	SetBackgroundColor( SkinManager::GetBackgroundColor() );
	return TRUE;
}

BEGIN_MESSAGE_MAP(ChangeLoopSizeDlg, CDialogEx)
	ON_WM_CTLCOLOR()

	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

HBRUSH ChangeLoopSizeDlg::OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor )
{
	return SkinManager::GetDialogCtrlColor( pDC, pWnd, nCtlColor );
}

void ChangeLoopSizeDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	m_dLoopWidth = m_ctrlLoopWidth.Get_Number();
	m_dLoopHeight = m_ctrlLoopHeight.Get_Number();

	OnOK();
}
void ChangeLoopSizeDlg::SetSize(double dLoopWidth, double dLoopHeight)
{
	m_dLoopWidth = dLoopWidth;
	m_dLoopHeight = dLoopHeight;
}

void ChangeLoopSizeDlg::GetSize(double& dLoopWidth, double& dLoopHeight) const
{
	dLoopWidth = m_dLoopWidth;
	dLoopHeight = m_dLoopHeight;
}

END_CUTLEADER_NAMESPACE()

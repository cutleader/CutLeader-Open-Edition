#include "StdAfx.h"
#include "ChangeLoopGapDlg.h"

#include "baseConst.h"

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(ChangeLoopGapDlg, CDialogEx)

ChangeLoopGapDlg::ChangeLoopGapDlg()
	: CLDialog(ChangeLoopGapDlg::IDD)
{
	m_dGapSize = 2.0;
	m_dMinLoopWidth = 50.0;
	m_dMinLoopHeight = 50.0;
}

ChangeLoopGapDlg::~ChangeLoopGapDlg(void)
{
}

void ChangeLoopGapDlg::DoDataExchange(CDataExchange* pDX)
{
	CLDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_EDIT_GapSize, m_ctrlGapSize);
	DDX_Control(pDX, IDC_EDIT_MinLoopWidth, m_ctrlMinLoopWidth);
	DDX_Control(pDX, IDC_EDIT_MinLoopHeight, m_ctrlMinLoopHeight);

	DDX_Control( pDX, IDOK, m_btnOk );
	DDX_Control( pDX, IDCANCEL, m_btnCancel );
}

BOOL ChangeLoopGapDlg::OnInitDialog()
{
	__super::OnInitDialog();

	m_ctrlGapSize.Init_WithoutMsg(m_dGapSize, 4, false, 0.1, 0.1);
	m_ctrlMinLoopWidth.Init_WithoutMsg(m_dMinLoopWidth, 4, false, 0.1, 0.1);
	m_ctrlMinLoopHeight.Init_WithoutMsg(m_dMinLoopHeight, 4, false, 0.1, 0.1);

	SetBackgroundColor( SkinManager::GetBackgroundColor() );
	return TRUE;
}

BEGIN_MESSAGE_MAP(ChangeLoopGapDlg, CLDialog)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

HBRUSH ChangeLoopGapDlg::OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor )
{
	return SkinManager::GetDialogCtrlColor( pDC, pWnd, nCtlColor );
}

void ChangeLoopGapDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	m_dGapSize = m_ctrlGapSize.Get_Number();
	m_dMinLoopWidth = m_ctrlMinLoopWidth.Get_Number();
	m_dMinLoopHeight = m_ctrlMinLoopHeight.Get_Number();

	OnOK();
}

END_CUTLEADER_NAMESPACE()

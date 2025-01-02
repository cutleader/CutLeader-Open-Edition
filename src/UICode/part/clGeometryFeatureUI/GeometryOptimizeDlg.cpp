#include "StdAfx.h"
#include "GeometryOptimizeDlg.h"

#include "baseConst.h"

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(GeometryOptimizeDlg, CDialogEx)

GeometryOptimizeDlg::GeometryOptimizeDlg()
	: CLDialog(GeometryOptimizeDlg::IDD)
{
}

GeometryOptimizeDlg::~GeometryOptimizeDlg(void)
{
}

void GeometryOptimizeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_EDIT_ALLOW_OFFSET, m_ctrlAllowOffset);
	DDX_Control( pDX, IDOK, m_btnOk );
	DDX_Control( pDX, IDCANCEL, m_btnCancel );
}

BOOL GeometryOptimizeDlg::OnInitDialog()
{
	__super::OnInitDialog();

	// display the params.
	m_ctrlAllowOffset.Init(m_dAllowOffset, 4, FALSE);

	SetBackgroundColor( SkinManager::GetBackgroundColor() );
	UpdateData(FALSE);

	return TRUE;
}

BEGIN_MESSAGE_MAP(GeometryOptimizeDlg, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	
	ON_EN_KILLFOCUS(IDC_EDIT_ALLOW_OFFSET, OnKillAllowOffset)
END_MESSAGE_MAP()

HBRUSH GeometryOptimizeDlg::OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor )
{
	return SkinManager::GetDialogCtrlColor( pDC, pWnd, nCtlColor );
}

void GeometryOptimizeDlg::OnKillAllowOffset()
{
	m_dAllowOffset = m_ctrlAllowOffset.Get_Number();
}

void GeometryOptimizeDlg::OnBnClickedOk()
{
	OnOK();
}

END_CUTLEADER_NAMESPACE()

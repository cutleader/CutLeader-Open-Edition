#include "StdAfx.h"
#include "GridCutParamDlg.h"

#include "GridCutParam.h"

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(GridCutParamDlg, CDialogEx)

GridCutParamDlg::GridCutParamDlg()
	: CLDialog(GridCutParamDlg::IDD)
{
}

GridCutParamDlg::GridCutParamDlg(GridCutParamPtr pGridCutParam)
				: CLDialog(GridCutParamDlg::IDD)
{
	m_pGridCutParam = pGridCutParam;
}

GridCutParamDlg::~GridCutParamDlg(void)
{
}

void GridCutParamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Check(pDX, IDC_CHECK_VERTICAL_4_INNER, m_bVertical_4_inner);
	DDX_Check(pDX, IDC_CHECK_CCW_4_OUTER, m_bCCW_4_outer);
	DDX_Control( pDX, IDOK, m_btnOk );
	DDX_Control( pDX, IDCANCEL, m_btnCancel );
}

BEGIN_MESSAGE_MAP(GridCutParamDlg, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

BOOL GridCutParamDlg::OnInitDialog()
{
	__super::OnInitDialog();

	DisplayGridCutParam();

	SetBackgroundColor( SkinManager::GetBackgroundColor() );

	SetWindowTheme( GetDlgItem( IDC_CHECK_VERTICAL_4_INNER )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_CHECK_CCW_4_OUTER )->GetSafeHwnd(), L"", L"" );
	return TRUE;
}

void GridCutParamDlg::DisplayGridCutParam()
{
	m_bVertical_4_inner = m_pGridCutParam->IsVertical_4_inner();
	m_bCCW_4_outer = m_pGridCutParam->IsCCW_4_outer();
	UpdateData(FALSE);
}

HBRUSH GridCutParamDlg::OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor )
{
	return SkinManager::GetDialogCtrlColor( pDC, pWnd, nCtlColor );
}

void GridCutParamDlg::OnBnClickedOk()
{
	UpdateData(TRUE);

	if (m_pGridCutParam)
	{
		if (m_pGridCutParam->IsVertical_4_inner() != m_bVertical_4_inner)
		{
			m_pGridCutParam->IsVertical_4_inner(m_bVertical_4_inner);
			m_pGridCutParam->SetModified(TRUE);
		}

		if (m_pGridCutParam->IsCCW_4_outer() != m_bCCW_4_outer)
		{
			m_pGridCutParam->IsCCW_4_outer(m_bCCW_4_outer);
			m_pGridCutParam->SetModified(TRUE);
		}
	}

	OnOK();
}

END_CUTLEADER_NAMESPACE()

#include "StdAfx.h"
#include "FlyCutParamDlg.h"

#include "baseConst.h"
#include "FlyCutParam.h"

#include "Uxtheme.h"
#pragma comment(lib, "Uxtheme.lib")

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(FlyCutParamDlg, CDialogEx)

FlyCutParamDlg::FlyCutParamDlg()
	: CLDialog(FlyCutParamDlg::IDD)
{
}

FlyCutParamDlg::FlyCutParamDlg(FlyCutParamPtr pFlyCutParam)
				: CLDialog(FlyCutParamDlg::IDD)
{
	m_pFlyCutParam = pFlyCutParam;
}

FlyCutParamDlg::~FlyCutParamDlg(void)
{
}

void FlyCutParamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_VERTICAL_CUT, m_bVerticalCut);
	DDX_Control(pDX, IDC_EDIT_MAX_TURN_RADIUS, m_dMax_turning_radius);
	DDX_Control( pDX, IDOK, m_btnOk );
	DDX_Control( pDX, IDCANCEL, m_btnCancel );
}

BEGIN_MESSAGE_MAP(FlyCutParamDlg, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

BOOL FlyCutParamDlg::OnInitDialog()
{
	__super::OnInitDialog();

	DisplayFlyCutParam();

	SetBackgroundColor( SkinManager::GetBackgroundColor() );

	SetWindowTheme( GetDlgItem( IDC_CHECK_VERTICAL_CUT )->GetSafeHwnd(), L"", L"" );
	return TRUE;
}

void FlyCutParamDlg::DisplayFlyCutParam()
{
	m_bVerticalCut = m_pFlyCutParam->IsVerticalCut();
	m_dMax_turning_radius.Init(m_pFlyCutParam->GetMax_turing_radius(), 4, FALSE);
	UpdateData(FALSE);
}

HBRUSH FlyCutParamDlg::OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor )
{
	return SkinManager::GetDialogCtrlColor( pDC, pWnd, nCtlColor );
}

void FlyCutParamDlg::OnBnClickedOk()
{
	UpdateData(TRUE);

	if (m_pFlyCutParam)
	{
		if (m_bVerticalCut != m_pFlyCutParam->IsVerticalCut())
		{
			m_pFlyCutParam->IsVerticalCut(m_bVerticalCut);
			m_pFlyCutParam->SetModified(TRUE);
		}

		if (m_pFlyCutParam->GetMax_turing_radius() != m_dMax_turning_radius.Get_Number())
		{
			m_pFlyCutParam->SetMax_turing_radius(m_dMax_turning_radius.Get_Number());
			m_pFlyCutParam->SetModified(TRUE);
		}
	}

	OnOK();
}

END_CUTLEADER_NAMESPACE()

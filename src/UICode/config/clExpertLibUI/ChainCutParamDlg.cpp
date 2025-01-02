#include "StdAfx.h"
#include "ChainCutParamDlg.h"

#include "ChainCutParam.h"

#include "Uxtheme.h"
#pragma comment(lib, "Uxtheme.lib")

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(ChainCutParamDlg, CDialogEx)

ChainCutParamDlg::ChainCutParamDlg()
	: CLDialog(ChainCutParamDlg::IDD)
{
}

ChainCutParamDlg::ChainCutParamDlg(ChainCutParamPtr pChainCutParam)
				: CLDialog(ChainCutParamDlg::IDD)
{
	m_pChainCutParam = pChainCutParam;
}

ChainCutParamDlg::~ChainCutParamDlg(void)
{
}

void ChainCutParamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_AVOID_ALONG_CONTOUR, m_bCutAlongContour);
	DDX_Control(pDX, IDC_EDIT_AVOID_DISTANCE, m_dAvoidDis);
	DDX_Control( pDX, IDOK, m_btnOk );
	DDX_Control( pDX, IDCANCEL, m_btnCancel );
}

BEGIN_MESSAGE_MAP(ChainCutParamDlg, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

BOOL ChainCutParamDlg::OnInitDialog()
{
	__super::OnInitDialog();

	DisplayChainCutParam();

	SetBackgroundColor( SkinManager::GetBackgroundColor() );

	SetWindowTheme( GetDlgItem( IDC_CHECK_AVOID_ALONG_CONTOUR )->GetSafeHwnd(), L"", L"" );
	return TRUE;
}

void ChainCutParamDlg::DisplayChainCutParam()
{
	m_bCutAlongContour = m_pChainCutParam->IsCutAlongContour();
	m_dAvoidDis.Init(m_pChainCutParam->GetAvoidDis(), 4, FALSE);
	UpdateData(FALSE);
}

HBRUSH ChainCutParamDlg::OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor )
{
	return SkinManager::GetDialogCtrlColor( pDC, pWnd, nCtlColor );
}

void ChainCutParamDlg::OnBnClickedOk()
{
	UpdateData(TRUE);

	if (m_pChainCutParam)
	{
		if (m_bCutAlongContour != m_pChainCutParam->IsCutAlongContour())
		{
			m_pChainCutParam->IsCutAlongContour(m_bCutAlongContour);
			m_pChainCutParam->SetModified(TRUE);
		}

		if (m_pChainCutParam->GetAvoidDis() != m_dAvoidDis.Get_Number())
		{
			m_pChainCutParam->SetAvoidDis(m_dAvoidDis.Get_Number());
			m_pChainCutParam->SetModified(TRUE);
		}
	}

	OnOK();
}

END_CUTLEADER_NAMESPACE()

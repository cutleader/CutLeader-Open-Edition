#include "StdAfx.h"
#include "BridgeCutParamDlg.h"

#include "BridgeCutParam.h"

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(BridgeCutParamDlg, CDialogEx)

BridgeCutParamDlg::BridgeCutParamDlg()
	: CLDialog(BridgeCutParamDlg::IDD)
{
}

BridgeCutParamDlg::BridgeCutParamDlg(BridgeCutParamPtr pBdgeCutParam)
				: CLDialog(BridgeCutParamDlg::IDD)
{
	m_pBdgeCutParam = pBdgeCutParam;
}

BridgeCutParamDlg::~BridgeCutParamDlg(void)
{
}

void BridgeCutParamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_EDIT_BDGE_WIDTH, m_dBdgeWidth);
	DDX_Control( pDX, IDOK, m_btnOk );
	DDX_Control( pDX, IDCANCEL, m_btnCancel );
}

BEGIN_MESSAGE_MAP(BridgeCutParamDlg, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

BOOL BridgeCutParamDlg::OnInitDialog()
{
	__super::OnInitDialog();

	DisplayComCutParam();

	SetBackgroundColor( SkinManager::GetBackgroundColor() );

	return TRUE;
}

HBRUSH BridgeCutParamDlg::OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor )
{
	return SkinManager::GetDialogCtrlColor( pDC, pWnd, nCtlColor );
}

void BridgeCutParamDlg::DisplayComCutParam()
{
	m_dBdgeWidth.Init(m_pBdgeCutParam->GetBdgeWidth(), 4, TRUE);
	UpdateData(FALSE);
}

void BridgeCutParamDlg::OnBnClickedOk()
{
	UpdateData(TRUE);

	if (m_pBdgeCutParam)
	{
		if (m_pBdgeCutParam->GetBdgeWidth() != m_dBdgeWidth.Get_Number())
		{
			m_pBdgeCutParam->SetBdgeWidth(m_dBdgeWidth.Get_Number());
			m_pBdgeCutParam->SetModified(TRUE);
		}
	}

	OnOK();
}

END_CUTLEADER_NAMESPACE()

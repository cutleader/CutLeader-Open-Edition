#include "StdAfx.h"
#include "GeomOptParamDlg.h"

#include "baseConst.h"
#include "clUtilityResource.h"

#include "ImpExpConfig.h"

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(GeomOptParamDlg, CDialogEx)

GeomOptParamDlg::GeomOptParamDlg()
: CLDialog(GeomOptParamDlg::IDD)
{
}

GeomOptParamDlg::GeomOptParamDlg(ImpExpConfigPtr pImpExpConfig)
				: CLDialog(GeomOptParamDlg::IDD)
{
	m_pImpExpConfig = pImpExpConfig;
}

GeomOptParamDlg::~GeomOptParamDlg(void)
{
}

void GeomOptParamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_EDIT_ALLOW_OFFSET, m_ctrlAllowOffset);
	DDX_Control(pDX, IDC_EDIT_MERGE_GEOM_SIZE, m_dPatFilterSize);
	DDX_Radio(pDX, IDC_RADIO_MOREARC, m_nMoreArc);
	DDX_Control( pDX, IDOK, m_btnOk );
	DDX_Control( pDX, IDCANCEL, m_btnCancel );
}

BOOL GeomOptParamDlg::OnInitDialog()
{
	__super::OnInitDialog();

	m_ctrlAllowOffset.Init(m_pImpExpConfig->GetAllowOffset(), 4, FALSE);
	m_dPatFilterSize.Init(m_pImpExpConfig->GetPatFilterSize(), 4, FALSE);
	if (m_pImpExpConfig->IsMoreArc())
	{
		m_nMoreArc = 0;
	}
	else
	{
		m_nMoreArc = 1;
	}

	UpdateData(FALSE);

	SetBackgroundColor( SkinManager::GetBackgroundColor() );

	SetWindowTheme( GetDlgItem( IDC_RADIO_MOREARC )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_RADIO_MORELINE )->GetSafeHwnd(), L"", L"" );
	return TRUE;
}

BEGIN_MESSAGE_MAP(GeomOptParamDlg, CDialogEx)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()

	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

HBRUSH GeomOptParamDlg::OnCtlColor( CDC* pDC, CWnd* pWnd, UINT nCtlColor )
{
	return SkinManager::GetDialogCtrlColor( pDC, pWnd, nCtlColor );
}

void GeomOptParamDlg::OnBnClickedOk()
{
	UpdateData(TRUE);

	if (m_pImpExpConfig)
	{
		if (m_pImpExpConfig->GetAllowOffset() != m_ctrlAllowOffset.Get_Number())
		{
			m_pImpExpConfig->SetAllowOffset(m_ctrlAllowOffset.Get_Number());
			m_pImpExpConfig->SetModified(TRUE);
		}

		if (m_pImpExpConfig->GetPatFilterSize() != m_dPatFilterSize.Get_Number())
		{
			m_pImpExpConfig->SetPatFilterSize(m_dPatFilterSize.Get_Number());
			m_pImpExpConfig->SetModified(TRUE);
		}

		bool bMoreArc = true;
		if (m_nMoreArc == 1)
		{
			bMoreArc = false;
		}
		if (bMoreArc != m_pImpExpConfig->IsMoreArc())
		{
			m_pImpExpConfig->IsMoreArc(bMoreArc);
			m_pImpExpConfig->SetModified(TRUE);
		}
	}

	OnOK();
}

END_CUTLEADER_NAMESPACE()

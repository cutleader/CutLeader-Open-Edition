#include "StdAfx.h"
#include "ComCutParamDlg.h"

#include "baseConst.h"
#include "clGeometryBaseResource.h"

#include "ComCutParam.h"

#include "Uxtheme.h"
#pragma comment(lib, "Uxtheme.lib")

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(ComCutParamDlg, CDialogEx)

ComCutParamDlg::ComCutParamDlg()
				: CLDialog(ComCutParamDlg::IDD)
{
}

ComCutParamDlg::ComCutParamDlg(ComCutParamPtr pComCutParam)
				: CLDialog(ComCutParamDlg::IDD)
{
	m_pComCutParam = pComCutParam;
}

ComCutParamDlg::~ComCutParamDlg(void)
{
}

void ComCutParamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_EDIT_COMCUT_CUTOVER, m_dOverCut);
	DDX_Control(pDX, IDC_COMBO_COMCUT_DIRECTION, m_cutDir);
	DDX_Control( pDX, IDOK, m_btnOk );
	DDX_Control( pDX, IDCANCEL, m_btnCancel );
}

BEGIN_MESSAGE_MAP(ComCutParamDlg, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

BOOL ComCutParamDlg::OnInitDialog()
{
	__super::OnInitDialog();

	DisplayComCutParam();
	UpdateData(FALSE);

	SetBackgroundColor( SkinManager::GetBackgroundColor() );

	SetWindowTheme( GetDlgItem( IDC_COMBO_COMCUT_DIRECTION )->GetSafeHwnd(), L"", L"" );
	return TRUE;
}

HBRUSH ComCutParamDlg::OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor )
{
	return SkinManager::GetDialogCtrlColor( pDC, pWnd, nCtlColor );
}

void ComCutParamDlg::InitComCutDir(DIRECTION cutDir)
{
	m_cutDir.ResetContent();

	CString str;
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_CW);
	m_cutDir.AddString(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_CCW);
	m_cutDir.AddString(str);

	if (cutDir == CW)
	{
		m_cutDir.SetCurSel(0);
	}
	else if (cutDir == CCW)
	{
		m_cutDir.SetCurSel(1);
	}
}

void ComCutParamDlg::DisplayComCutParam()
{
	InitComCutDir(m_pComCutParam->GetCutDir());

	m_dOverCut.Init(m_pComCutParam->GetOverCut(), 4, FALSE);
}

void ComCutParamDlg::OnBnClickedOk()
{
	UpdateData(TRUE);

	int iIndex = m_cutDir.GetCurSel();
	if (iIndex == 0)
	{
		if (m_pComCutParam->GetCutDir() != CW)
		{
			m_pComCutParam->SetCutDir(CW);
			m_pComCutParam->SetModified(TRUE);
		}
	}
	else if (iIndex == 1)
	{
		if (m_pComCutParam->GetCutDir() != CCW)
		{
			m_pComCutParam->SetCutDir(CCW);
			m_pComCutParam->SetModified(TRUE);
		}
	}

	if (m_pComCutParam)
	{
		if (m_pComCutParam->GetOverCut() != m_dOverCut.Get_Number())
		{
			m_pComCutParam->SetOverCut(m_dOverCut.Get_Number());
			m_pComCutParam->SetModified(TRUE);
		}
	}

	OnOK();
}

END_CUTLEADER_NAMESPACE()

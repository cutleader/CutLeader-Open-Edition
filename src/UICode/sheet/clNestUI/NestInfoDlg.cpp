#include "StdAfx.h"
#include "NestInfoDlg.h"

#include "baseConst.h"
#include "clNestResource.h"

#include "Uxtheme.h"
#pragma comment(lib, "Uxtheme.lib")

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNAMIC(NestInfoDlg, CDialogEx)

NestInfoDlg::NestInfoDlg(CWnd* pParent /*=NULL*/)
		: CLDialog(NestInfoDlg::IDD, pParent)
{
}

NestInfoDlg::~NestInfoDlg(void)
{
}

BEGIN_MESSAGE_MAP(NestInfoDlg, CDialogEx)
	ON_WM_CTLCOLOR()

	ON_BN_CLICKED(IDC_CHECK_NEST_COUNT, OnCheckNestCount)
	ON_BN_CLICKED(IDC_CHECK_ROT_ANG, OnCheckRotAng)
	ON_BN_CLICKED(IDOK, OnOK)
END_MESSAGE_MAP()

void NestInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Check(pDX, IDC_CHECK_NEST_COUNT, m_bEnableNestCount);
	DDX_Control(pDX, IDC_EDIT_NEST_COUNT, m_ctrlNestNum);

	DDX_Check(pDX, IDC_CHECK_ROT_ANG, m_bEnableRotAng);
	DDX_Control(pDX, IDC_COMBO_ROT_ANG, m_ctrlRotateAng);

	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
}

BOOL NestInfoDlg::OnInitDialog()
{
	__super::OnInitDialog();

	// 零件数量。
	m_bEnableNestCount = TRUE;
	((CWnd*)GetDlgItem(IDC_EDIT_NEST_COUNT))->EnableWindow(TRUE);
	m_ctrlNestNum.Init(10, 0, false);

	// 旋转角度。
	m_bEnableRotAng = FALSE;
	((CWnd*)GetDlgItem(IDC_COMBO_ROT_ANG))->EnableWindow(FALSE);
	CString str;
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PART_ROTATE_FREE);
	m_ctrlRotateAng.AddString(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PART_ROTATE_90_INCREMENT);
	m_ctrlRotateAng.AddString(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PART_ROTATE_0_FIXED);
	m_ctrlRotateAng.AddString(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PART_ROTATE_90_FIXED);
	m_ctrlRotateAng.AddString(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PART_ROTATE_180_FIXED);
	m_ctrlRotateAng.AddString(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PART_ROTATE_270_FIXED);
	m_ctrlRotateAng.AddString(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PART_ROTATE_HORIZONTAL);
	m_ctrlRotateAng.AddString(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PART_ROTATE_VERTICAL);
	m_ctrlRotateAng.AddString(str);
	m_ctrlRotateAng.SetCurSel(1);

	UpdateData(FALSE);

	SetBackgroundColor( SkinManager::GetBackgroundColor() );

	SetWindowTheme( GetDlgItem( IDC_CHECK_NEST_COUNT )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_CHECK_ROT_ANG )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_COMBO_ROT_ANG )->GetSafeHwnd(), L"", L"" );

	return TRUE;
}


HBRUSH NestInfoDlg::OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor )
{
	return SkinManager::GetDialogCtrlColor( pDC, pWnd, nCtlColor );
}

void NestInfoDlg::OnCheckNestCount()
{
	UpdateData(TRUE);

	if (m_bEnableNestCount)
		((CWnd*)GetDlgItem(IDC_EDIT_NEST_COUNT))->EnableWindow(TRUE);
	else
		((CWnd*)GetDlgItem(IDC_EDIT_NEST_COUNT))->EnableWindow(FALSE);
}

void NestInfoDlg::OnCheckRotAng()
{
	UpdateData(TRUE);

	if (m_bEnableRotAng)
		((CWnd*)GetDlgItem(IDC_COMBO_ROT_ANG))->EnableWindow(TRUE);
	else
		((CWnd*)GetDlgItem(IDC_COMBO_ROT_ANG))->EnableWindow(FALSE);
}

void NestInfoDlg::OnOK()
{
	UpdateData(TRUE);
	if (!m_bEnableNestCount && !m_bEnableRotAng)
		return;

	// 零件数量。
	m_iNestCount = (int)m_ctrlNestNum.Get_Number();

	// 旋转角度。
	m_iRotAng = (PartRotateType)m_ctrlRotateAng.GetCurSel();

	CDialogEx::OnOK();
}

END_CUTLEADER_NAMESPACE()

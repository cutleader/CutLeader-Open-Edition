#include "StdAfx.h"
#include "BatchEditTaskPartDlg_2.h"

#include "baseConst.h"
#include "ClNestResource.h"
#include <vector>
#include "NestPart.h"

using namespace std;


BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNAMIC(BatchEditTaskPartDlg_2, CDialogEx)

BatchEditTaskPartDlg_2::BatchEditTaskPartDlg_2(CWnd* pParent /*=NULL*/)
		: CLDialog(BatchEditTaskPartDlg_2::IDD, pParent)
{
}

BatchEditTaskPartDlg_2::~BatchEditTaskPartDlg_2(void)
{
}

BEGIN_MESSAGE_MAP(BatchEditTaskPartDlg_2, CDialogEx)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()

	ON_BN_CLICKED(IDC_CHECK_NestingRotateAngle, OnNestingRotateAngle)
	ON_BN_CLICKED(IDOK, OnOK)
END_MESSAGE_MAP()

void BatchEditTaskPartDlg_2::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Check(pDX, IDC_CHECK_NestingRotateAngle, m_bEnableNestingRotateAngle);
	DDX_Control(pDX, IDC_COMBO_NestingRotateAngle, m_comboBoxNestingRotateAngle);

	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
}

BOOL BatchEditTaskPartDlg_2::OnInitDialog()
{
	__super::OnInitDialog();

	// 自动排版旋转角度。
	m_bEnableNestingRotateAngle = FALSE;
	((CWnd*)GetDlgItem(IDC_COMBO_NestingRotateAngle))->EnableWindow(FALSE);
	vector<CString> allRotateStyles = NestPart::GetAllRotateStyles();
    for (unsigned int i = 0; i < allRotateStyles.size(); i++)
    {
        m_comboBoxNestingRotateAngle.AddString(allRotateStyles[i]);
    }
    m_comboBoxNestingRotateAngle.SetCurSel(0);

	UpdateData(FALSE);

	SetBackgroundColor(SkinManager::GetBackgroundColor());
	return TRUE;
}

BOOL BatchEditTaskPartDlg_2::OnEraseBkgnd(CDC* pDC)
{
	CRect r;
	GetClientRect(&r);
	if ((HBRUSH)m_brush == NULL)
		m_brush.CreateSolidBrush(SkinManager::GetBackgroundColor());
	pDC->FillRect(&r, &m_brush);

	return TRUE;
}

HBRUSH BatchEditTaskPartDlg_2::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	return SkinManager::GetDialogCtrlColor(pDC, pWnd, nCtlColor);
}

void BatchEditTaskPartDlg_2::OnNestingRotateAngle()
{
	UpdateData(TRUE);

	if (m_bEnableNestingRotateAngle)
    {
		((CWnd*)GetDlgItem(IDC_COMBO_NestingRotateAngle))->EnableWindow(TRUE);
    }
	else
    {
		((CWnd*)GetDlgItem(IDC_COMBO_NestingRotateAngle))->EnableWindow(FALSE);
    }
}

void BatchEditTaskPartDlg_2::OnOK()
{
	UpdateData(TRUE);
	if (!m_bEnableNestingRotateAngle)
    {
		return;
    }

	// 自动排版旋转角度。
	m_emNestingRotateAngle = (PartRotateType)m_comboBoxNestingRotateAngle.GetCurSel();

	CDialogEx::OnOK();
}

END_CUTLEADER_NAMESPACE()

#include "StdAfx.h"
#include "BatchEditTaskPartDlg.h"

#include "baseConst.h"
#include "ClNestResource.h"
#include <vector>
#include "NestPart.h"

using namespace std;


BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNAMIC(BatchEditTaskPartDlg, CDialogEx)

BatchEditTaskPartDlg::BatchEditTaskPartDlg(CWnd* pParent /*=NULL*/)
		: CLDialog(BatchEditTaskPartDlg::IDD, pParent)
{
}

BatchEditTaskPartDlg::~BatchEditTaskPartDlg(void)
{
}

BEGIN_MESSAGE_MAP(BatchEditTaskPartDlg, CDialogEx)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()

	ON_BN_CLICKED(IDC_CHECK_PlanningCount, OnCheckPlanningCount)
	ON_BN_CLICKED(IDC_CHECK_NestingRotateAngle, OnNestingRotateAngle)
	ON_BN_CLICKED(IDOK, OnOK)
END_MESSAGE_MAP()

void BatchEditTaskPartDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Check(pDX, IDC_CHECK_PlanningCount, m_bEnablePlanningCount);
	DDX_Control(pDX, IDC_EDIT_PlanningCount, m_editBoxPlanningCount);

	DDX_Check(pDX, IDC_CHECK_NestingRotateAngle, m_bEnableNestingRotateAngle);
	DDX_Control(pDX, IDC_COMBO_NestingRotateAngle, m_comboBoxNestingRotateAngle);

	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
}

BOOL BatchEditTaskPartDlg::OnInitDialog()
{
	__super::OnInitDialog();

	// 计划零件数量。
	m_bEnablePlanningCount = false;
	m_editBoxPlanningCount.Init(1, 0, false);
    ((CWnd*)GetDlgItem(IDC_EDIT_PlanningCount))->EnableWindow(FALSE);

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

BOOL BatchEditTaskPartDlg::OnEraseBkgnd(CDC* pDC)
{
	CRect r;
	GetClientRect(&r);
	if ((HBRUSH)m_brush == NULL)
		m_brush.CreateSolidBrush(SkinManager::GetBackgroundColor());
	pDC->FillRect(&r, &m_brush);

	return TRUE;
}

HBRUSH BatchEditTaskPartDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	return SkinManager::GetDialogCtrlColor(pDC, pWnd, nCtlColor);
}

void BatchEditTaskPartDlg::OnCheckPlanningCount()
{
	UpdateData(TRUE);

	if (m_bEnablePlanningCount)
    {
		((CWnd*)GetDlgItem(IDC_EDIT_PlanningCount))->EnableWindow(TRUE);
    }
	else
    {
		((CWnd*)GetDlgItem(IDC_EDIT_PlanningCount))->EnableWindow(FALSE);
    }
}

void BatchEditTaskPartDlg::OnNestingRotateAngle()
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

void BatchEditTaskPartDlg::OnOK()
{
	UpdateData(TRUE);
	if (!m_bEnablePlanningCount && !m_bEnableNestingRotateAngle)
    {
		return;
    }

	// 计划零件数量。
	m_iPlanningCount = (int)m_editBoxPlanningCount.Get_Number();

	// 自动排版旋转角度。
	m_emNestingRotateAngle = (PartRotateType)m_comboBoxNestingRotateAngle.GetCurSel();

	CDialogEx::OnOK();
}

END_CUTLEADER_NAMESPACE()

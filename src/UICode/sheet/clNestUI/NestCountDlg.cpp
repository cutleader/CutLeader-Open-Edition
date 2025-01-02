#include "StdAfx.h"
#include "NestCountDlg.h"

#include "productConst.h"

BEGIN_NAMESPACE_3(CL, Nest, UI)

IMPLEMENT_DYNAMIC(NestCountDlg, CDialogEx)

NestCountDlg::NestCountDlg(CWnd* pParent /*=NULL*/)
		: CDialogEx(NestCountDlg::IDD, pParent)
{
}

NestCountDlg::~NestCountDlg(void)
{
}

BEGIN_MESSAGE_MAP(NestCountDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, OnOK)
END_MESSAGE_MAP()

void NestCountDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_EDIT_NEST_COUNT, m_ctrlNestNum);
}

BOOL NestCountDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 零件数量。
	m_ctrlNestNum.Init(10, 0, false);
	UpdateData(FALSE);

	SetBackgroundColor(UI_COLOR);
	return TRUE;
}

void NestCountDlg::OnOK()
{
	UpdateData(TRUE);

	// 零件数量。
	m_iNestCount = (int)m_ctrlNestNum.Get_Number();

	CDialogEx::OnOK();
}

END_NAMESPACE_3()

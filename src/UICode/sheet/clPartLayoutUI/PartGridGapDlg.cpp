#include "StdAfx.h"
#include "PartGridGapDlg.h"

#include "baseConst.h"
#include "DataBaseManager.h"

#include "clPartLayoutResource.h"

#include "CLOptionInfo.h"
#include "SheetEditParam.h"

#include "Uxtheme.h"
#pragma comment(lib, "Uxtheme.lib")

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(PartGridGapDlg, CDialogEx)

PartGridGapDlg::PartGridGapDlg(CWnd* pParent /*=NULL*/)
	: CLDialog(PartGridGapDlg::IDD, pParent)
{
}

PartGridGapDlg::~PartGridGapDlg(void)
{
}

void PartGridGapDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Check(pDX, IDC_CHECK_COMCUT, m_bComCut);
	DDX_Control(pDX, IDC_EDIT_PART_GAP, m_ctrlPartDis);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
}

BOOL PartGridGapDlg::OnInitDialog()
{
	__super::OnInitDialog();

	// 零件距离。
	SheetEditParamPtr pSheetEditParam = CLOptionInfo::GetInstance()->GetSheetEditParam();
	m_ctrlPartDis.Init(pSheetEditParam->GetPartDis(), 4, FALSE);

	SetBackgroundColor( SkinManager::GetBackgroundColor() );
	
	SetWindowTheme( GetDlgItem( IDC_CHECK_COMCUT )->GetSafeHwnd(), L"", L"" );

	return TRUE;
}

BEGIN_MESSAGE_MAP(PartGridGapDlg, CDialogEx)
	ON_WM_CTLCOLOR()

	ON_BN_CLICKED(IDC_CHECK_COMCUT, OnCheckComcut)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

HBRUSH PartGridGapDlg::OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor )
{
	return SkinManager::GetDialogCtrlColor( pDC, pWnd, nCtlColor );
}

void PartGridGapDlg::OnCheckComcut()
{
	UpdateData(TRUE);

	if (m_bComCut)
		((CWnd*)GetDlgItem(IDC_EDIT_PART_GAP))->EnableWindow(FALSE);
	else
		((CWnd*)GetDlgItem(IDC_EDIT_PART_GAP))->EnableWindow(TRUE);
}

void PartGridGapDlg::OnBnClickedOk()
{
	UpdateData(TRUE);

	if (!m_bComCut)
	{
		m_dPartDis = m_ctrlPartDis.Get_Number();
	}

	OnOK();
}

double PartGridGapDlg::GetPartDis(BOOL& bEnableComcut)
{ 
    bEnableComcut = m_bComCut;
    return m_dPartDis;
}

END_CUTLEADER_NAMESPACE()

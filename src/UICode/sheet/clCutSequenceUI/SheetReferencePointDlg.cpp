#include "StdAfx.h"
#include "SheetReferencePointDlg.h"

#include "SheetRefPtData.h"

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(SheetReferencePointDlg, CDialogEx)

SheetReferencePointDlg::SheetReferencePointDlg()
	: CLDialog(SheetReferencePointDlg::IDD)
{
}

SheetReferencePointDlg::~SheetReferencePointDlg(void)
{
}

void SheetReferencePointDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Radio(pDX, IDC_RADIO_SheetUpperLeft, m_iSheetRefPtPosition);

	DDX_Control( pDX, IDOK, m_btnOk );
	DDX_Control( pDX, IDCANCEL, m_btnCancel );
}

BEGIN_MESSAGE_MAP(SheetReferencePointDlg, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

BOOL SheetReferencePointDlg::OnInitDialog()
{
	__super::OnInitDialog();

	if (m_pSheetRefPtData->GetSheetRefPtPosition() == LEFT_TOP)
		m_iSheetRefPtPosition = 0;
	else if (m_pSheetRefPtData->GetSheetRefPtPosition() == LEFT_BOTTOM)
		m_iSheetRefPtPosition = 2;
	else if (m_pSheetRefPtData->GetSheetRefPtPosition() == RIGHT_TOP)
		m_iSheetRefPtPosition = 1;
	else if (m_pSheetRefPtData->GetSheetRefPtPosition() == RIGHT_BOTTOM)
		m_iSheetRefPtPosition = 3;

	UpdateData(FALSE);

	SetBackgroundColor( SkinManager::GetBackgroundColor() );
	return TRUE;
}

HBRUSH SheetReferencePointDlg::OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor )
{
	return SkinManager::GetDialogCtrlColor( pDC, pWnd, nCtlColor );
}

void SheetReferencePointDlg::OnBnClickedOk()
{
	UpdateData(TRUE);

	if (m_iSheetRefPtPosition == 0 && m_pSheetRefPtData->GetSheetRefPtPosition() != LEFT_TOP) {
		m_pSheetRefPtData->SetSheetRefPtPosition(LEFT_TOP);
		m_pSheetRefPtData->SetModified(TRUE);
	}
	else if (m_iSheetRefPtPosition == 1 && m_pSheetRefPtData->GetSheetRefPtPosition() != RIGHT_TOP) {
		m_pSheetRefPtData->SetSheetRefPtPosition(RIGHT_TOP);
		m_pSheetRefPtData->SetModified(TRUE);
	}
	else if (m_iSheetRefPtPosition == 2 && m_pSheetRefPtData->GetSheetRefPtPosition() != LEFT_BOTTOM) {
		m_pSheetRefPtData->SetSheetRefPtPosition(LEFT_BOTTOM);
		m_pSheetRefPtData->SetModified(TRUE);
	}
	else if (m_iSheetRefPtPosition == 3 && m_pSheetRefPtData->GetSheetRefPtPosition() != RIGHT_BOTTOM) {
		m_pSheetRefPtData->SetSheetRefPtPosition(RIGHT_BOTTOM);
		m_pSheetRefPtData->SetModified(TRUE);
	}

	OnOK();
}

END_CUTLEADER_NAMESPACE()

#include "StdAfx.h"
#include "ReferencePointDlg.h"

#include "SequenceConfigItem.h"

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(ReferencePointDlg, CDialogEx)

ReferencePointDlg::ReferencePointDlg()
	: CLDialog(ReferencePointDlg::IDD)
{
}

ReferencePointDlg::~ReferencePointDlg(void)
{
}

void ReferencePointDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Radio(pDX, IDC_RADIO_GeometryUpperLeft, m_iCanvasRefPtPosition);
	DDX_Check(pDX, IDC_CHECK_ExcludeUncutGeometry, m_bExcludeUncutGeometry);
	DDX_Check(pDX, IDC_CHECK_CalculateCamFeature, m_bCalculateCamFeature);
	DDX_Radio(pDX, IDC_RADIO_SheetUpperLeft, m_iSheetRefPtPosition);

	DDX_Control( pDX, IDOK, m_btnOk );
	DDX_Control( pDX, IDCANCEL, m_btnCancel );
}

BEGIN_MESSAGE_MAP(ReferencePointDlg, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

BOOL ReferencePointDlg::OnInitDialog()
{
	__super::OnInitDialog();

	if (m_pSequenceConfigItem->GetCanvasRefPtPosition() == CanvasRefPt_LeftTop) {
		m_iCanvasRefPtPosition = 0;
	}
	else if (m_pSequenceConfigItem->GetCanvasRefPtPosition() == CanvasRefPt_RightTop) {
		m_iCanvasRefPtPosition = 1;
	}
	else if (m_pSequenceConfigItem->GetCanvasRefPtPosition() == CanvasRefPt_LeftBottom) {
		m_iCanvasRefPtPosition = 2;
	}
	else if (m_pSequenceConfigItem->GetCanvasRefPtPosition() == CanvasRefPt_RightBottom) {
		m_iCanvasRefPtPosition = 3;
	}
	else if (m_pSequenceConfigItem->GetCanvasRefPtPosition() == CanvasRefPt_CenterPt) {
		m_iCanvasRefPtPosition = 4;
	}

	m_bExcludeUncutGeometry = m_pSequenceConfigItem->GetIsExcludeUncutGeometry();
	m_bCalculateCamFeature = m_pSequenceConfigItem->GetIsCalculateCamFeature();

	if (m_pSequenceConfigItem->GetSheetRefPtPosition() == LEFT_TOP)
		m_iSheetRefPtPosition = 0;
	else if (m_pSequenceConfigItem->GetSheetRefPtPosition() == LEFT_BOTTOM)
		m_iSheetRefPtPosition = 2;
	else if (m_pSequenceConfigItem->GetSheetRefPtPosition() == RIGHT_TOP)
		m_iSheetRefPtPosition = 1;
	else if (m_pSequenceConfigItem->GetSheetRefPtPosition() == RIGHT_BOTTOM)
		m_iSheetRefPtPosition = 3;

	UpdateData(FALSE);

	SetBackgroundColor( SkinManager::GetBackgroundColor() );
	return TRUE;
}

HBRUSH ReferencePointDlg::OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor )
{
	return SkinManager::GetDialogCtrlColor( pDC, pWnd, nCtlColor );
}

void ReferencePointDlg::OnBnClickedOk()
{
	UpdateData(TRUE);

	if (m_iCanvasRefPtPosition == 0 && m_pSequenceConfigItem->GetCanvasRefPtPosition() != CanvasRefPt_LeftTop) {
		m_pSequenceConfigItem->SetCanvasRefPtPosition(CanvasRefPt_LeftTop);
		m_pSequenceConfigItem->SetModified(TRUE);
	}
	else if (m_iCanvasRefPtPosition == 1 && m_pSequenceConfigItem->GetCanvasRefPtPosition() != CanvasRefPt_RightTop) {
		m_pSequenceConfigItem->SetCanvasRefPtPosition(CanvasRefPt_RightTop);
		m_pSequenceConfigItem->SetModified(TRUE);
	}
	else if (m_iCanvasRefPtPosition == 2 && m_pSequenceConfigItem->GetCanvasRefPtPosition() != CanvasRefPt_LeftBottom) {
		m_pSequenceConfigItem->SetCanvasRefPtPosition(CanvasRefPt_LeftBottom);
		m_pSequenceConfigItem->SetModified(TRUE);
	}
	else if (m_iCanvasRefPtPosition == 3 && m_pSequenceConfigItem->GetCanvasRefPtPosition() != CanvasRefPt_RightBottom) {
		m_pSequenceConfigItem->SetCanvasRefPtPosition(CanvasRefPt_RightBottom);
		m_pSequenceConfigItem->SetModified(TRUE);
	}
	else if (m_iCanvasRefPtPosition == 4 && m_pSequenceConfigItem->GetCanvasRefPtPosition() != CanvasRefPt_CenterPt) {
		m_pSequenceConfigItem->SetCanvasRefPtPosition(CanvasRefPt_CenterPt);
		m_pSequenceConfigItem->SetModified(TRUE);
	}

	if (m_bExcludeUncutGeometry && !m_pSequenceConfigItem->GetIsExcludeUncutGeometry()) {
		m_pSequenceConfigItem->SetIsExcludeUncutGeometry(true);
		m_pSequenceConfigItem->SetModified(TRUE);
	}
	else if (!m_bExcludeUncutGeometry && m_pSequenceConfigItem->GetIsExcludeUncutGeometry()) {
		m_pSequenceConfigItem->SetIsExcludeUncutGeometry(false);
		m_pSequenceConfigItem->SetModified(TRUE);
	}

	if (m_bCalculateCamFeature && !m_pSequenceConfigItem->GetIsCalculateCamFeature()) {
		m_pSequenceConfigItem->SetIsCalculateCamFeature(true);
		m_pSequenceConfigItem->SetModified(TRUE);
	}
	else if (!m_bCalculateCamFeature && m_pSequenceConfigItem->GetIsCalculateCamFeature()) {
		m_pSequenceConfigItem->SetIsCalculateCamFeature(false);
		m_pSequenceConfigItem->SetModified(TRUE);
	}

	if (m_iSheetRefPtPosition == 0 && m_pSequenceConfigItem->GetSheetRefPtPosition() != LEFT_TOP) {
		m_pSequenceConfigItem->SetSheetRefPtPosition(LEFT_TOP);
		m_pSequenceConfigItem->SetModified(TRUE);
	}
	else if (m_iSheetRefPtPosition == 1 && m_pSequenceConfigItem->GetSheetRefPtPosition() != RIGHT_TOP) {
		m_pSequenceConfigItem->SetSheetRefPtPosition(RIGHT_TOP);
		m_pSequenceConfigItem->SetModified(TRUE);
	}
	else if (m_iSheetRefPtPosition == 2 && m_pSequenceConfigItem->GetSheetRefPtPosition() != LEFT_BOTTOM) {
		m_pSequenceConfigItem->SetSheetRefPtPosition(LEFT_BOTTOM);
		m_pSequenceConfigItem->SetModified(TRUE);
	}
	else if (m_iSheetRefPtPosition == 3 && m_pSequenceConfigItem->GetSheetRefPtPosition() != RIGHT_BOTTOM) {
		m_pSequenceConfigItem->SetSheetRefPtPosition(RIGHT_BOTTOM);
		m_pSequenceConfigItem->SetModified(TRUE);
	}

	OnOK();
}

END_CUTLEADER_NAMESPACE()

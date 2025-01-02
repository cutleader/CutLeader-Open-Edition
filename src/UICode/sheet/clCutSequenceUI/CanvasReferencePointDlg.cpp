#include "StdAfx.h"
#include "CanvasReferencePointDlg.h"

#include "CanvasRefPtData.h"

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(CanvasReferencePointDlg, CDialogEx)

CanvasReferencePointDlg::CanvasReferencePointDlg()
	: CLDialog(CanvasReferencePointDlg::IDD)
{
}

CanvasReferencePointDlg::~CanvasReferencePointDlg(void)
{
}

void CanvasReferencePointDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Radio(pDX, IDC_RADIO_GeometryUpperLeft, m_iCanvasRefPtPosition);
	DDX_Check(pDX, IDC_CHECK_ExcludeUncutGeometry, m_bExcludeUncutGeometry);
	DDX_Check(pDX, IDC_CHECK_CalculateCamFeature, m_bCalculateCamFeature);

	DDX_Control( pDX, IDOK, m_btnOk );
	DDX_Control( pDX, IDCANCEL, m_btnCancel );
}

BEGIN_MESSAGE_MAP(CanvasReferencePointDlg, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

BOOL CanvasReferencePointDlg::OnInitDialog()
{
	__super::OnInitDialog();

	if (m_pCanvasRefPtData->GetCanvasRefPtPosition() == CanvasRefPt_LeftTop) {
		m_iCanvasRefPtPosition = 0;
	}
	else if (m_pCanvasRefPtData->GetCanvasRefPtPosition() == CanvasRefPt_RightTop) {
		m_iCanvasRefPtPosition = 1;
	}
	else if (m_pCanvasRefPtData->GetCanvasRefPtPosition() == CanvasRefPt_LeftBottom) {
		m_iCanvasRefPtPosition = 2;
	}
	else if (m_pCanvasRefPtData->GetCanvasRefPtPosition() == CanvasRefPt_RightBottom) {
		m_iCanvasRefPtPosition = 3;
	}
	else if (m_pCanvasRefPtData->GetCanvasRefPtPosition() == CanvasRefPt_CenterPt) {
		m_iCanvasRefPtPosition = 4;
	}

	m_bExcludeUncutGeometry = m_pCanvasRefPtData->GetIsExcludeUncutGeometry();
	m_bCalculateCamFeature = m_pCanvasRefPtData->GetIsCalculateCamFeature();

	UpdateData(FALSE);

	SetBackgroundColor( SkinManager::GetBackgroundColor() );
	return TRUE;
}

HBRUSH CanvasReferencePointDlg::OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor )
{
	return SkinManager::GetDialogCtrlColor( pDC, pWnd, nCtlColor );
}

void CanvasReferencePointDlg::OnBnClickedOk()
{
	UpdateData(TRUE);

	if (m_iCanvasRefPtPosition == 0 && m_pCanvasRefPtData->GetCanvasRefPtPosition() != CanvasRefPt_LeftTop) {
		m_pCanvasRefPtData->SetCanvasRefPtPosition(CanvasRefPt_LeftTop);
		m_pCanvasRefPtData->SetModified(TRUE);
	}
	else if (m_iCanvasRefPtPosition == 1 && m_pCanvasRefPtData->GetCanvasRefPtPosition() != CanvasRefPt_RightTop) {
		m_pCanvasRefPtData->SetCanvasRefPtPosition(CanvasRefPt_RightTop);
		m_pCanvasRefPtData->SetModified(TRUE);
	}
	else if (m_iCanvasRefPtPosition == 2 && m_pCanvasRefPtData->GetCanvasRefPtPosition() != CanvasRefPt_LeftBottom) {
		m_pCanvasRefPtData->SetCanvasRefPtPosition(CanvasRefPt_LeftBottom);
		m_pCanvasRefPtData->SetModified(TRUE);
	}
	else if (m_iCanvasRefPtPosition == 3 && m_pCanvasRefPtData->GetCanvasRefPtPosition() != CanvasRefPt_RightBottom) {
		m_pCanvasRefPtData->SetCanvasRefPtPosition(CanvasRefPt_RightBottom);
		m_pCanvasRefPtData->SetModified(TRUE);
	}
	else if (m_iCanvasRefPtPosition == 4 && m_pCanvasRefPtData->GetCanvasRefPtPosition() != CanvasRefPt_CenterPt) {
		m_pCanvasRefPtData->SetCanvasRefPtPosition(CanvasRefPt_CenterPt);
		m_pCanvasRefPtData->SetModified(TRUE);
	}

	if (m_bExcludeUncutGeometry && !m_pCanvasRefPtData->GetIsExcludeUncutGeometry()) {
		m_pCanvasRefPtData->SetIsExcludeUncutGeometry(true);
		m_pCanvasRefPtData->SetModified(TRUE);
	}
	else if (!m_bExcludeUncutGeometry && m_pCanvasRefPtData->GetIsExcludeUncutGeometry()) {
		m_pCanvasRefPtData->SetIsExcludeUncutGeometry(false);
		m_pCanvasRefPtData->SetModified(TRUE);
	}

	if (m_bCalculateCamFeature && !m_pCanvasRefPtData->GetIsCalculateCamFeature()) {
		m_pCanvasRefPtData->SetIsCalculateCamFeature(true);
		m_pCanvasRefPtData->SetModified(TRUE);
	}
	else if (!m_bCalculateCamFeature && m_pCanvasRefPtData->GetIsCalculateCamFeature()) {
		m_pCanvasRefPtData->SetIsCalculateCamFeature(false);
		m_pCanvasRefPtData->SetModified(TRUE);
	}

	OnOK();
}

END_CUTLEADER_NAMESPACE()

#include "stdafx.h"
#include "SnapPtParamPage.h"

#include "clOptionResource.h"
#include "SnapPtParam.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(SnapPtParamPage, CLResizableDialog )

SnapPtParamPage::SnapPtParamPage() : CLResizableDialog (SnapPtParamPage::IDD )
{
}

SnapPtParamPage::~SnapPtParamPage()
{
}

void SnapPtParamPage::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);

	DDX_Check(pDX, IDC_CHECK_SnapPoint, m_bEnableSnapPt);
	DDX_Control(pDX, IDC_EDIT_SnapDistance, m_ctrlSnapRange);

	DDX_Check(pDX, IDC_CHECK_SnapEndPt, m_bSnapEndPt);
	DDX_Check(pDX, IDC_CHECK_SnapMiddlePt, m_bSnapMiddlePt);
	DDX_Check(pDX, IDC_CHECK_SnapCenterPt, m_bSnapCenterPt);
	DDX_Check(pDX, IDC_CHECK_SnapClosestPt, m_bSnapClosestPt);
	DDX_Check(pDX, IDC_CHECK_SnapFootPt, m_bSnapFootPt);
	DDX_Check(pDX, IDC_CHECK_SnapTangencyPt, m_bSnapTangencyPt);
	DDX_Check(pDX, IDC_CHECK_SnapCrossPt, m_bSnapCrossPt);
}

BEGIN_MESSAGE_MAP(SnapPtParamPage, CLResizableDialog )
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_CHECK_SnapPoint, OnCheckSnapPoint)
END_MESSAGE_MAP()

BOOL SnapPtParamPage::OnInitDialog()
{
	CLResizableDialog::OnInitDialog();

	// layout controls.
	{
		AddAnchor(IDC_GROUP_Option, TOP_LEFT, TOP_RIGHT);
		AddAnchor(IDC_CHECK_SnapPoint, TOP_LEFT);
		AddAnchor(IDC_STATIC_SnapDistance, TOP_LEFT);
		AddAnchor(IDC_EDIT_SnapDistance, TOP_LEFT);
		AddAnchor(IDC_STATIC_Pixels, TOP_LEFT);

		AddAnchor(IDC_CHECK_SnapEndPt, TOP_LEFT);
		AddAnchor(IDC_CHECK_SnapMiddlePt, TOP_LEFT);
		AddAnchor(IDC_CHECK_SnapCenterPt, TOP_LEFT);
		AddAnchor(IDC_CHECK_SnapClosestPt, TOP_LEFT);
		AddAnchor(IDC_CHECK_SnapFootPt, TOP_LEFT);
		AddAnchor(IDC_CHECK_SnapTangencyPt, TOP_LEFT);
		AddAnchor(IDC_CHECK_SnapCrossPt, TOP_LEFT);
	}

	SetBackgroundColor( SkinManager::GetBackgroundColor() );

	// setup ctrls for don't follow windows setting
	SetWindowTheme( GetDlgItem(IDC_GROUP_Option)->GetSafeHwnd(), L"", L"" );
	SetWindowTheme(GetDlgItem(IDC_CHECK_SnapPoint)->GetSafeHwnd(), L"", L"");
	SetWindowTheme( GetDlgItem(IDC_CHECK_SnapEndPt)->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem(IDC_CHECK_SnapMiddlePt)->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem(IDC_CHECK_SnapCenterPt)->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem(IDC_CHECK_SnapClosestPt)->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem(IDC_CHECK_SnapFootPt)->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem(IDC_CHECK_SnapTangencyPt)->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem(IDC_CHECK_SnapCrossPt)->GetSafeHwnd(), L"", L"" );

	return TRUE;
}

HBRUSH SnapPtParamPage::OnCtlColor( CDC* pDC, CWnd* pWnd, UINT nCtlColor )
{
	return SkinManager::GetDialogCtrlColor( pDC, pWnd, nCtlColor );
}

void SnapPtParamPage::OnCheckSnapPoint()
{
	UpdateData(TRUE);
	if (m_bEnableSnapPt)
		EnableOtherOptions();
	else
		DisableOtherOptions();
}

void SnapPtParamPage::DisableOtherOptions()
{
	GetDlgItem(IDC_EDIT_SnapDistance)->EnableWindow(FALSE);
	GetDlgItem(IDC_CHECK_SnapEndPt)->EnableWindow(FALSE);
	GetDlgItem(IDC_CHECK_SnapMiddlePt)->EnableWindow(FALSE);
	GetDlgItem(IDC_CHECK_SnapCenterPt)->EnableWindow(FALSE);
	GetDlgItem(IDC_CHECK_SnapClosestPt)->EnableWindow(FALSE);
	GetDlgItem(IDC_CHECK_SnapFootPt)->EnableWindow(FALSE);
	GetDlgItem(IDC_CHECK_SnapTangencyPt)->EnableWindow(FALSE);
	GetDlgItem(IDC_CHECK_SnapCrossPt)->EnableWindow(FALSE);
}

void SnapPtParamPage::EnableOtherOptions()
{
	GetDlgItem(IDC_EDIT_SnapDistance)->EnableWindow(TRUE);
	GetDlgItem(IDC_CHECK_SnapEndPt)->EnableWindow(TRUE);
	GetDlgItem(IDC_CHECK_SnapMiddlePt)->EnableWindow(TRUE);
	GetDlgItem(IDC_CHECK_SnapCenterPt)->EnableWindow(TRUE);
	GetDlgItem(IDC_CHECK_SnapClosestPt)->EnableWindow(TRUE);
	GetDlgItem(IDC_CHECK_SnapFootPt)->EnableWindow(TRUE);
	GetDlgItem(IDC_CHECK_SnapTangencyPt)->EnableWindow(TRUE);
	GetDlgItem(IDC_CHECK_SnapCrossPt)->EnableWindow(TRUE);
}

void SnapPtParamPage::DisplaySnapPtParam(SnapPtParamPtr pSnapPtParam)
{
	m_pSnapPtParam = pSnapPtParam;

	m_bEnableSnapPt = m_pSnapPtParam->GetIsEnableSnapPt();
	m_ctrlSnapRange.Init(m_pSnapPtParam->GetSnapRange(), 0, FALSE);

	m_bSnapEndPt = m_pSnapPtParam->GetIsSnapEndPt();
	m_bSnapMiddlePt = m_pSnapPtParam->GetIsSnapMiddlePt();
	m_bSnapCenterPt = m_pSnapPtParam->GetIsSnapCenterPt();
	m_bSnapClosestPt = m_pSnapPtParam->GetIsSnapClosestPt();
	m_bSnapFootPt = m_pSnapPtParam->GetIsSnapFootPt();
	m_bSnapTangencyPt = m_pSnapPtParam->GetIsSnapTangencyPt();
	m_bSnapCrossPt = m_pSnapPtParam->GetIsSnapCrossPt();

	if (m_bEnableSnapPt)
		EnableOtherOptions();
	else
		DisableOtherOptions();

	UpdateData(FALSE);
}

SnapPtParamPtr SnapPtParamPage::GetSnapPtParam()
{
	UpdateData(TRUE);

	if (m_bEnableSnapPt != m_pSnapPtParam->GetIsEnableSnapPt())
	{
		m_pSnapPtParam->SetIsEnableSnapPt(m_bEnableSnapPt);
		m_pSnapPtParam->SetModified(TRUE);
	}
	m_ctrlSnapRange.UpdateNumEdit_fromUI();
	if ((int)m_ctrlSnapRange.Get_Number() != m_pSnapPtParam->GetSnapRange())
	{
		m_pSnapPtParam->SetSnapRange((int)m_ctrlSnapRange.Get_Number());
		m_pSnapPtParam->SetModified(TRUE);
	}

	if (m_bSnapEndPt != m_pSnapPtParam->GetIsSnapEndPt())
	{
		m_pSnapPtParam->SetIsSnapEndPt(m_bSnapEndPt);
		m_pSnapPtParam->SetModified(TRUE);
	}
	if (m_bSnapMiddlePt != m_pSnapPtParam->GetIsSnapMiddlePt())
	{
		m_pSnapPtParam->SetIsSnapMiddlePt(m_bSnapMiddlePt);
		m_pSnapPtParam->SetModified(TRUE);
	}
	if (m_bSnapCenterPt != m_pSnapPtParam->GetIsSnapCenterPt())
	{
		m_pSnapPtParam->SetIsSnapCenterPt(m_bSnapCenterPt);
		m_pSnapPtParam->SetModified(TRUE);
	}
	if (m_bSnapClosestPt != m_pSnapPtParam->GetIsSnapClosestPt())
	{
		m_pSnapPtParam->SetIsSnapClosestPt(m_bSnapClosestPt);
		m_pSnapPtParam->SetModified(TRUE);
	}
	if (m_bSnapFootPt != m_pSnapPtParam->GetIsSnapFootPt())
	{
		m_pSnapPtParam->SetIsSnapFootPt(m_bSnapFootPt);
		m_pSnapPtParam->SetModified(TRUE);
	}
	if (m_bSnapTangencyPt != m_pSnapPtParam->GetIsSnapTangencyPt())
	{
		m_pSnapPtParam->SetIsSnapTangencyPt(m_bSnapTangencyPt);
		m_pSnapPtParam->SetModified(TRUE);
	}
	if (m_bSnapCrossPt != m_pSnapPtParam->GetIsSnapCrossPt())
	{
		m_pSnapPtParam->SetIsSnapCrossPt(m_bSnapCrossPt);
		m_pSnapPtParam->SetModified(TRUE);
	}

	return m_pSnapPtParam;
}

END_CUTLEADER_NAMESPACE()

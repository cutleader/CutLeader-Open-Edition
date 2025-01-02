#include "StdAfx.h"
#include "CollisionAvoidParamDlg.h"

#include "CollisionAvoidParam.h"
#include "clUtilityResource.h"

#include "Uxtheme.h"
#pragma comment(lib, "Uxtheme.lib")

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(CollisionAvoidParamDlg, CDialogEx)

CollisionAvoidParamDlg::CollisionAvoidParamDlg()
: CLDialog(CollisionAvoidParamDlg::IDD)
{
}

CollisionAvoidParamDlg::CollisionAvoidParamDlg(CollisionAvoidParamPtr pCollisionAvoidParam)
				: CLDialog(CollisionAvoidParamDlg::IDD)
{
	m_pCollisionAvoidParam = pCollisionAvoidParam;
}

CollisionAvoidParamDlg::~CollisionAvoidParamDlg(void)
{
}

void CollisionAvoidParamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Check(pDX, IDC_CHECK_ADJUST_STARTCUT_POINT, m_bAdjustStartCutPt);
	DDX_Control(pDX, IDC_COMBO_ADJUST_STARTCUT_TYPE, m_ctrlAdjustStartCutPtType);

	DDX_Check(pDX, IDC_CHECK_GENERATE_AVOID_PATH, m_bGenerateAvoidPath);
	DDX_Control(pDX, IDC_EDIT_CUTOUT_AVOID, m_dCutOutAvoid);
	DDX_Control(pDX, IDC_EDIT_AVOID_DIS, m_dAvoidDis);
	DDX_Control(pDX, IDC_EDIT_AVOID_RATIO, m_dAvoidRatio);
	DDX_Control( pDX, IDOK, m_btnOk );
	DDX_Control( pDX, IDCANCEL, m_btnCancel );
}

BEGIN_MESSAGE_MAP(CollisionAvoidParamDlg, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

BOOL CollisionAvoidParamDlg::OnInitDialog()
{
	__super::OnInitDialog();
	
	// display the cut avoid params.
	DisplayCollisionAvoidParam();

	SetBackgroundColor( SkinManager::GetBackgroundColor() );

	SetWindowTheme( GetDlgItem( IDC_CHECK_ADJUST_STARTCUT_POINT )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_COMBO_ADJUST_STARTCUT_TYPE )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_CHECK_GENERATE_AVOID_PATH )->GetSafeHwnd(), L"", L"" );

	UpdateData(FALSE);

	return TRUE;
}

void CollisionAvoidParamDlg::DisplayCollisionAvoidParam()
{
	m_bAdjustStartCutPt = m_pCollisionAvoidParam->GetIsAdjustStartCutPt();
	m_ctrlAdjustStartCutPtType.ResetContent();
	CString str;
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_ADJUST_STARTPT_INNERLOOP);
	m_ctrlAdjustStartCutPtType.AddString(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_ADJUST_STARTPT_ALLLOOP);
	m_ctrlAdjustStartCutPtType.AddString(str);
	m_ctrlAdjustStartCutPtType.SetCurSel((int)m_pCollisionAvoidParam->GetAdjustStartCutPtType());

	m_bGenerateAvoidPath = m_pCollisionAvoidParam->GetIsGenerateAvoidPath();
	m_dCutOutAvoid.Init(m_pCollisionAvoidParam->GetCutOutAvoid(), 4, FALSE);
	m_dAvoidDis.Init(m_pCollisionAvoidParam->GetAvoidDis(), 4, FALSE);
	m_dAvoidRatio.Init(m_pCollisionAvoidParam->GetAvoidRatio(), 4, FALSE);
}

HBRUSH CollisionAvoidParamDlg::OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor )
{
	return SkinManager::GetDialogCtrlColor( pDC, pWnd, nCtlColor );
}

void CollisionAvoidParamDlg::OnBnClickedOk()
{
	UpdateData(TRUE);

	if (m_pCollisionAvoidParam)
	{
		// “调整起切点”子规则
		if (m_bAdjustStartCutPt != m_pCollisionAvoidParam->GetIsAdjustStartCutPt())
		{
			m_pCollisionAvoidParam->SetIsAdjustStartCutPt(m_bAdjustStartCutPt);
			m_pCollisionAvoidParam->SetModified(TRUE);
		}
		if ((AdjustStartCutPtType)m_ctrlAdjustStartCutPtType.GetCurSel() != m_pCollisionAvoidParam->GetAdjustStartCutPtType())
		{
			m_pCollisionAvoidParam->SetAdjustStartCutPtType((AdjustStartCutPtType)m_ctrlAdjustStartCutPtType.GetCurSel());
			m_pCollisionAvoidParam->SetModified(TRUE);
		}

		// “生成避让路径”子规则
		if (m_bGenerateAvoidPath != m_pCollisionAvoidParam->GetIsGenerateAvoidPath())
		{
			m_pCollisionAvoidParam->SetIsGenerateAvoidPath(m_bGenerateAvoidPath);
			m_pCollisionAvoidParam->SetModified(TRUE);
		}
		if (m_pCollisionAvoidParam->GetCutOutAvoid() != m_dCutOutAvoid.Get_Number())
		{
			m_pCollisionAvoidParam->SetCutOutAvoid(m_dCutOutAvoid.Get_Number());
			m_pCollisionAvoidParam->SetModified(TRUE);
		}
		if (m_pCollisionAvoidParam->GetAvoidDis() != m_dAvoidDis.Get_Number())
		{
			m_pCollisionAvoidParam->SetAvoidDis(m_dAvoidDis.Get_Number());
			m_pCollisionAvoidParam->SetModified(TRUE);
		}
		if (m_pCollisionAvoidParam->GetAvoidRatio() != m_dAvoidRatio.Get_Number())
		{
			m_pCollisionAvoidParam->SetAvoidRatio(m_dAvoidRatio.Get_Number());
			m_pCollisionAvoidParam->SetModified(TRUE);
		}
	}

	OnOK();
}

END_CUTLEADER_NAMESPACE()

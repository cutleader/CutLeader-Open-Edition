#include "stdafx.h"
#include "ImpExpConfigPage.h"

#include "baseConst.h"
#include "baseConst.h"
#include "clUILibResource.h"
#include "clUtilityResource.h"
#include "clBaseDataResource.h"

#include "ImpExpConfig.h"
#include "clOptionResource.h"
#include "GeomOptParamDlg.h"
#include "GeomMergeParamDlg.h"

#include "Uxtheme.h"
#pragma comment(lib, "Uxtheme.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const size_t MAX_TIP_TEXT_LENGTH = 1024;

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE( ImpExpConfigPage, CLResizableDialog )

ImpExpConfigPage::ImpExpConfigPage() : CLResizableDialog( ImpExpConfigPage::IDD )
{
}

ImpExpConfigPage::~ImpExpConfigPage()
{
}

void ImpExpConfigPage::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);

	// 导入选项
	DDX_Control(pDX, IDC_COMBO_IMP_UNIT, m_ctrlImpUnit);
	DDX_Control(pDX, IDC_EDIT_IMP_TOL, m_dImpTol);
	DDX_Control(pDX, IDC_EDIT_FILTER_SIZE, m_dLoopFilterSize);
	DDX_Control(pDX, IDC_EDIT_OPT_COUNT, m_iOptimizeLimit);
	DDX_Control(pDX, IDC_EDIT_IGNORE_COUNT, m_iIgnoreCount);
	DDX_Control( pDX, IDC_BTN_OPT_CONFIG, m_btnOptConfig );

	// 导出选项
	DDX_Control(pDX, IDC_COMBO_EXP_UNIT, m_ctrlExpUnit);
	DDX_Control(pDX, IDC_COMBO_EXP_VERSION, m_ctrlDxfVer);
	DDX_Check(pDX, IDC_CHECK_EXP_SHT_BORDER, m_bExpShtBorder);
	DDX_Control( pDX, IDC_BTN_MERGE_GEOM, m_btnMergeGeom );
}

BEGIN_MESSAGE_MAP( ImpExpConfigPage, CResizableDialog )
	ON_WM_CTLCOLOR()

	ON_BN_CLICKED(IDC_BTN_OPT_CONFIG, OnGeomOptAdvConfig)
	ON_BN_CLICKED(IDC_BTN_MERGE_GEOM, OnGeomMergeConfig)
END_MESSAGE_MAP()

BOOL ImpExpConfigPage::OnInitDialog()
{
	CLResizableDialog::OnInitDialog();

	/************************************************************************/
	// layout controls.

	// “导入选项”分组.
	AddAnchor(IDC_GROUP_IMPORT, TOP_LEFT, TOP_RIGHT);
	//
	AddAnchor(IDC_TXT_IMP_UNIT, TOP_LEFT);
	AddAnchor(IDC_COMBO_IMP_UNIT, TOP_LEFT);
	//
	AddAnchor(IDC_TXT_IMP_TOL, TOP_LEFT);
	AddAnchor(IDC_EDIT_IMP_TOL, TOP_LEFT);
	//
	AddAnchor(IDC_TXT_FILTER_SIZE, TOP_LEFT);
	AddAnchor(IDC_EDIT_FILTER_SIZE, TOP_LEFT);
	//
	AddAnchor(IDC_TXT_OPT_COUNT, TOP_LEFT);
	AddAnchor(IDC_EDIT_OPT_COUNT, TOP_LEFT);
	AddAnchor(IDC_BTN_OPT_CONFIG, TOP_LEFT);
	//
	AddAnchor(IDC_TXT_IGNORE_COUNT, TOP_LEFT);
	AddAnchor(IDC_EDIT_IGNORE_COUNT, TOP_LEFT);
	AddAnchor(IDC_BTN_MERGE_GEOM, TOP_LEFT);

	// “导出选项”分组.
	AddAnchor(IDC_GROUP_EXPORT, TOP_LEFT, TOP_RIGHT);
	//
	AddAnchor(IDC_TXT_EXP_UNIT, TOP_LEFT);
	AddAnchor(IDC_COMBO_EXP_UNIT, TOP_LEFT);
	//
	AddAnchor(IDC_TXT_EXP_VERSION, TOP_LEFT);
	AddAnchor(IDC_COMBO_EXP_VERSION, TOP_LEFT);
	//
	AddAnchor(IDC_CHECK_EXP_SHT_BORDER, TOP_LEFT);
	/************************************************************************/

	SetBackgroundColor( SkinManager::GetBackgroundColor() );

	SetWindowTheme( GetDlgItem( IDC_EDIT_MSG )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_GROUP_IMPORT )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_COMBO_IMP_UNIT )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_COMBO_EXP_UNIT )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_COMBO_EXP_VERSION )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_GROUP_EXPORT )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_CHECK_EXP_SHT_BORDER )->GetSafeHwnd(), L"", L"" );
	return TRUE;
}

HBRUSH ImpExpConfigPage::OnCtlColor( CDC* pDC, CWnd* pWnd, UINT nCtlColor )
{
	return SkinManager::GetDialogCtrlColor( pDC, pWnd, nCtlColor );
}

void ImpExpConfigPage::OnGeomOptAdvConfig()
{
	GeomOptParamDlg dlg(m_pImpExpConfig);
	dlg.DoModal();
}

void ImpExpConfigPage::OnGeomMergeConfig()
{
	GeomMergeParamDlg dlg(m_pImpExpConfig);
	dlg.DoModal();
}

void ImpExpConfigPage::DisplayImpExpConfig(ImpExpConfigPtr pImpExpConfig)
{
	m_pImpExpConfig = pImpExpConfig;

	CString str;


	/************************************************************************/	
	// “导入选项”分组。

	// import unit.
	m_ctrlImpUnit.ResetContent();
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_INCH);
	m_ctrlImpUnit.AddString(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MM);
	m_ctrlImpUnit.AddString(str);
	if (m_pImpExpConfig->GetImpUnitType() == (int)DIM_UNIT_INCH)
		m_ctrlImpUnit.SetCurSel(0);
	else if (m_pImpExpConfig->GetImpUnitType() == (int)DIM_UNIT_MM)
		m_ctrlImpUnit.SetCurSel(1);

	m_dImpTol.Init(m_pImpExpConfig->GetImpTol(), DIGITAL_NUM, FALSE);
	m_dLoopFilterSize.Init(m_pImpExpConfig->GetLoopFilterSize(), DIGITAL_NUM, FALSE);
	m_iOptimizeLimit.Init(m_pImpExpConfig->GetOptimizeLimit(), 0, FALSE);
	m_iIgnoreCount.Init(m_pImpExpConfig->GetAssignLimit(), 0, FALSE);
	/************************************************************************/


	/************************************************************************/
	// “导出选项”分组。

	// EXPORT unit.
	m_ctrlExpUnit.ResetContent();
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_INCH);
	m_ctrlExpUnit.AddString(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MM);
	m_ctrlExpUnit.AddString(str);
	if (m_pImpExpConfig->GetExpUnitType() == DIM_UNIT_INCH)
		m_ctrlExpUnit.SetCurSel(0);
	else if (m_pImpExpConfig->GetExpUnitType() == DIM_UNIT_MM)
		m_ctrlExpUnit.SetCurSel(1);

	// 导出dxf/dwg版本。
	m_ctrlDxfVer.ResetContent();
	m_ctrlDxfVer.AddString(_T("R12"));
	m_ctrlDxfVer.AddString(_T("2004"));
	m_ctrlDxfVer.AddString(_T("2010"));
	if (m_pImpExpConfig->GetExpDxfVer() == DXF_VER_R12)
		m_ctrlDxfVer.SetCurSel(0);
	else if (m_pImpExpConfig->GetExpDxfVer() == DXF_VER_2004)
		m_ctrlDxfVer.SetCurSel(1);
	else if (m_pImpExpConfig->GetExpDxfVer() == DXF_VER_2010)
		m_ctrlDxfVer.SetCurSel(2);

	m_bExpShtBorder = m_pImpExpConfig->IsExpShtBorder();
	/************************************************************************/


	UpdateData(FALSE);
}

ImpExpConfigPtr ImpExpConfigPage::GetImpExpConfig()
{
	UpdateData(TRUE);


	/************************************************************************/	
	// “导入选项”。

	DIM_UNIT_TYPE selDimUnit;
	int iIndex = m_ctrlImpUnit.GetCurSel();
	if (iIndex == 0)
		selDimUnit = DIM_UNIT_INCH;
	else if (iIndex == 1)
		selDimUnit = DIM_UNIT_MM;
	if (selDimUnit != m_pImpExpConfig->GetImpUnitType())
	{
		m_pImpExpConfig->SetImpUnitType(selDimUnit);
		m_pImpExpConfig->SetModified(TRUE);
	}

	if (m_dImpTol.Get_Number() != m_pImpExpConfig->GetImpTol())
	{
		m_pImpExpConfig->SetImpTol(m_dImpTol.Get_Number());
		m_pImpExpConfig->SetModified(TRUE);
	}

	// loop filter size.
	if (m_dLoopFilterSize.Get_Number() != m_pImpExpConfig->GetLoopFilterSize())
	{
		m_pImpExpConfig->SetLoopFilterSize(m_dLoopFilterSize.Get_Number());
		m_pImpExpConfig->SetModified(TRUE);
	}

	// about "geometry optimize"
	if (m_iOptimizeLimit.Get_Number() != m_pImpExpConfig->GetOptimizeLimit())
	{
		m_pImpExpConfig->SetOptimizeLimit((int)m_iOptimizeLimit.Get_Number());
		m_pImpExpConfig->SetModified(TRUE);
	}

	if (m_iIgnoreCount.Get_Number() != m_pImpExpConfig->GetAssignLimit())
	{
		m_pImpExpConfig->SetAssignLimit((int)m_iIgnoreCount.Get_Number());
		m_pImpExpConfig->SetModified(TRUE);
	}
	/************************************************************************/


	/************************************************************************/
	// “导出选项”。

	// export unit.
	iIndex = m_ctrlExpUnit.GetCurSel();
	if (iIndex == 0)
		selDimUnit = DIM_UNIT_INCH;
	else if (iIndex == 1)
		selDimUnit = DIM_UNIT_MM;
	if (selDimUnit != m_pImpExpConfig->GetExpUnitType())
	{
		m_pImpExpConfig->SetExpUnitType(selDimUnit);
		m_pImpExpConfig->SetModified(TRUE);
	}

	// 导出dxf/dwg版本。
	DXF_VER_TYPE iExpDxfVer;
	iIndex = m_ctrlDxfVer.GetCurSel();
	if (iIndex == 0)
		iExpDxfVer = DXF_VER_R12;
	else if (iIndex == 1)
		iExpDxfVer = DXF_VER_2004;
	else if (iIndex == 2)
		iExpDxfVer = DXF_VER_2010;
	if (iExpDxfVer != m_pImpExpConfig->GetExpDxfVer())
	{
		m_pImpExpConfig->SetExpDxfVer(iExpDxfVer);
		m_pImpExpConfig->SetModified(TRUE);
	}

	if (m_bExpShtBorder != m_pImpExpConfig->IsExpShtBorder())
	{
		m_pImpExpConfig->IsExpShtBorder(m_bExpShtBorder);
		m_pImpExpConfig->SetModified(TRUE);
	}
	/************************************************************************/

	return m_pImpExpConfig;
}

END_CUTLEADER_NAMESPACE()

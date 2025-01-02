#include "StdAfx.h"
#include "TplDisplayParamPage.h"

#include "baseConst.h"
#include "UnitHelper.h"

#include "clReportResource.h"
#include "TplDisplayParam.h"

#include "Uxtheme.h"
#pragma comment(lib, "Uxtheme.lib")

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(TplDisplayParamPage, CResizableDialog)

TplDisplayParamPage::TplDisplayParamPage(void) : CLResizableDialog(TplDisplayParamPage::IDD)
{
	m_strMsg = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_RPT_DISPLAY_PARAM);
}

TplDisplayParamPage::~TplDisplayParamPage(void)
{
}

void TplDisplayParamPage::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_MSG, m_strMsg);
	DDX_Control(pDX, IDC_COMBO_DISPLAY_UNIT, m_ctrlDisplayUnit);

	// paper margin.
	DDX_Check(pDX, IDC_CHECK_SHOW_MARGIN, m_bShowMargin);
	DDX_Control(pDX, IDC_PIC_MARGIN_COLOR, m_ctrlMarginColor);

	// grid.
	DDX_Check(pDX, IDC_CHECK_SHOW_GRID, m_bShowGrid);
	DDX_Control(pDX, IDC_PIC_GRID_COLOR, m_ctrlGridColor);

	DDX_Control(pDX, IDC_BTN_MARGIN_COLOR, m_btnMarginColor);
	DDX_Control(pDX, IDC_BTN_GRID_COLOR, m_btnGridColor);
}

BEGIN_MESSAGE_MAP(TplDisplayParamPage, CResizableDialog)
	ON_WM_CTLCOLOR()

	ON_BN_CLICKED(IDC_BTN_MARGIN_COLOR, OnMarginColor)
	ON_BN_CLICKED(IDC_BTN_GRID_COLOR, OnGridColor)
END_MESSAGE_MAP()

BOOL TplDisplayParamPage::OnInitDialog()
{
	CLResizableDialog::OnInitDialog();

	/************************************************************************/
	// layout controls.

	AddAnchor(IDC_STATIC_INFO, TOP_LEFT);
	AddAnchor(IDC_EDIT_MSG, TOP_LEFT, TOP_RIGHT);

	AddAnchor(IDC_TEXT_DISPLAY_UNIT, TOP_LEFT);
	AddAnchor(IDC_COMBO_DISPLAY_UNIT, TOP_LEFT);

	// paper margin.
	AddAnchor(IDC_GROUP_PAPER_MARGIN, TOP_LEFT, TOP_RIGHT);
	AddAnchor(IDC_CHECK_SHOW_MARGIN, TOP_LEFT);
	AddAnchor(IDC_TEXT_MARGIN_COLOR, TOP_LEFT);
	AddAnchor(IDC_PIC_MARGIN_COLOR, TOP_LEFT);
	AddAnchor(IDC_BTN_MARGIN_COLOR, TOP_LEFT);

	// grid.
	AddAnchor(IDC_GROUP_GRID, TOP_LEFT, TOP_RIGHT);
	AddAnchor(IDC_CHECK_SHOW_GRID, TOP_LEFT);
	AddAnchor(IDC_TEXT_GRID_COLOR, TOP_LEFT);
	AddAnchor(IDC_PIC_GRID_COLOR, TOP_LEFT);
	AddAnchor(IDC_BTN_GRID_COLOR, TOP_LEFT);
	/************************************************************************/

	/************************************************************************/
	// init data.

	// init the combo box.
	CString str;
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_TPL_UNIT_PIXEL);
	m_ctrlDisplayUnit.AddString(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_TPL_UNIT_INCH);
	m_ctrlDisplayUnit.AddString(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_TPL_UNIT_CM);
	m_ctrlDisplayUnit.AddString(str);
	/************************************************************************/

	SetBackgroundColor( SkinManager::GetBackgroundColor() );
	
	SetWindowTheme( GetDlgItem( IDC_COMBO_DISPLAY_UNIT )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_GROUP_PAPER_MARGIN )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_CHECK_SHOW_MARGIN )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_GROUP_GRID )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_CHECK_SHOW_GRID )->GetSafeHwnd(), L"", L"" );

	UpdateData(FALSE);

	return TRUE;
}

HBRUSH TplDisplayParamPage::OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor )
{
	return SkinManager::GetDialogCtrlColor( pDC, pWnd, nCtlColor );
}

void TplDisplayParamPage::OnMarginColor()
{
	CColorDialog dlg(m_pTplDisplayParam->GetMarginColor());
	if (dlg.DoModal() == IDOK)
	{
		m_ctrlMarginColor.SetColor(dlg.GetColor());
	}
}

void TplDisplayParamPage::OnGridColor()
{
	CColorDialog dlg(m_pTplDisplayParam->GetGridColor());
	if (dlg.DoModal() == IDOK)
	{
		m_ctrlGridColor.SetColor(dlg.GetColor());
	}
}

void TplDisplayParamPage::DisplayDisParam(TplDisplayParamPtr pTplDisplayParam)
{
	m_pTplDisplayParam = pTplDisplayParam;

	// unit type.
	if (m_pTplDisplayParam->GetTplUnit() == TPL_UNIT_PIXEL)
		m_ctrlDisplayUnit.SetCurSel(0);
	else if (m_pTplDisplayParam->GetTplUnit() == TPL_UNIT_INCH)
		m_ctrlDisplayUnit.SetCurSel(1);
	else if (m_pTplDisplayParam->GetTplUnit() == TPL_UNIT_CM)
		m_ctrlDisplayUnit.SetCurSel(2);

	// paper margin.
	m_bShowMargin = m_pTplDisplayParam->IsShowMargin();
	m_ctrlMarginColor.SetColor(m_pTplDisplayParam->GetMarginColor());

	// grid.
	m_bShowGrid = m_pTplDisplayParam->IsShowGrid();
	m_ctrlGridColor.SetColor(m_pTplDisplayParam->GetGridColor());

	UpdateData(FALSE);
}

TplDisplayParamPtr TplDisplayParamPage::GetTplDisplayParam()
{
	UpdateData(TRUE);

	// display unit.
	if (m_ctrlDisplayUnit.GetCurSel() != (int)m_pTplDisplayParam->GetTplUnit())
	{
		m_pTplDisplayParam->SetTplUnit((TPL_UNIT_TYPE)m_ctrlDisplayUnit.GetCurSel());
		m_pTplDisplayParam->SetModified(TRUE);
	}

	// about paper margin.
	if (m_bShowMargin != m_pTplDisplayParam->IsShowMargin())
	{
		m_pTplDisplayParam->IsShowMargin(m_bShowMargin);
		m_pTplDisplayParam->SetModified(TRUE);
	}
	if (m_ctrlMarginColor.GetColor() != m_pTplDisplayParam->GetMarginColor())
	{
		m_pTplDisplayParam->SetMarginColor(m_ctrlMarginColor.GetColor());
		m_pTplDisplayParam->SetModified(TRUE);
	}

	// about grid.
	if (m_bShowGrid != m_pTplDisplayParam->IsShowGrid())
	{
		m_pTplDisplayParam->IsShowGrid(m_bShowGrid);
		m_pTplDisplayParam->SetModified(TRUE);
	}
	if (m_ctrlGridColor.GetColor() != m_pTplDisplayParam->GetGridColor())
	{
		m_pTplDisplayParam->SetGridColor(m_ctrlGridColor.GetColor());
		m_pTplDisplayParam->SetModified(TRUE);
	}

	return m_pTplDisplayParam;
}

END_CUTLEADER_NAMESPACE()

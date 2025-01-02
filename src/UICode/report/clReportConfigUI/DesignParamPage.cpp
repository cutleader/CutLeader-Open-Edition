#include "StdAfx.h"
#include "DesignParamPage.h"

#include "baseConst.h"
#include "UnitHelper.h"

#include "reportConst.h"
#include "TplEditParam.h"
#include "clReportResource.h"

#include "Uxtheme.h"
#pragma comment(lib, "Uxtheme.lib")

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(DesignParamPage, CResizableDialog)

DesignParamPage::DesignParamPage(void) : CLResizableDialog(DesignParamPage::IDD)
{
	m_strMsg = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_DESIGN_PARAM);
}

DesignParamPage::~DesignParamPage(void)
{
}

void DesignParamPage::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_MSG, m_strMsg);

	// about grid.
	DDX_Check(pDX, IDC_CHECK_SNAP_GRID, m_bSnapGrid);
	DDX_Control(pDX, IDC_EDIT_GRID_X_SIZE, m_dGridCellXSize);
	DDX_Control(pDX, IDC_EDIT_GRID_Y_SIZE, m_dGridCellYSize);
}

BEGIN_MESSAGE_MAP(DesignParamPage, CResizableDialog)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

BOOL DesignParamPage::OnInitDialog()
{
	CLResizableDialog::OnInitDialog();

	/************************************************************************/
	// layout controls.

	AddAnchor(IDC_STATIC_INFO, TOP_LEFT);
	AddAnchor(IDC_EDIT_MSG, TOP_LEFT, TOP_RIGHT);

	// grid.
	AddAnchor(IDC_GROUP_GRID, TOP_LEFT, TOP_RIGHT);
	AddAnchor(IDC_CHECK_SNAP_GRID, TOP_LEFT);
	AddAnchor(IDC_TEXT_GRID_X_SIZE, TOP_LEFT);
	AddAnchor(IDC_EDIT_GRID_X_SIZE, TOP_LEFT);
	AddAnchor(IDC_TEXT_GRID_Y_SIZE, TOP_LEFT);
	AddAnchor(IDC_EDIT_GRID_Y_SIZE, TOP_LEFT);
	/************************************************************************/

	SetBackgroundColor( SkinManager::GetBackgroundColor() );

	SetWindowTheme( GetDlgItem( IDC_GROUP_GRID )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_CHECK_SNAP_GRID )->GetSafeHwnd(), L"", L"" );

	UpdateData(FALSE);

	return TRUE;
}


HBRUSH DesignParamPage::OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor )
{
	return SkinManager::GetDialogCtrlColor( pDC, pWnd, nCtlColor );
}

void DesignParamPage::DisplayTplEditParam(TplEditParamPtr pTplEditParam, TPL_UNIT_TYPE unitType)
{
	m_pTplEditParam = pTplEditParam;
	m_unitType = unitType;

	// about grid.
	m_bSnapGrid = m_pTplEditParam->IsSnapToGrid();
	if (m_unitType == TPL_UNIT_PIXEL)
	{
		m_dGridCellXSize.Init(m_pTplEditParam->GetGridCellSizeX(), 0, FALSE);
		m_dGridCellYSize.Init(m_pTplEditParam->GetGridCellSizeY(), 0, FALSE);
	}
	else if (m_unitType == TPL_UNIT_INCH)
	{
		m_dGridCellXSize.Init(UnitHelper::PixelsToInches(m_pTplEditParam->GetGridCellSizeX()), TPL_DIGITAL_NUM, FALSE);
		m_dGridCellYSize.Init(UnitHelper::PixelsToInches(m_pTplEditParam->GetGridCellSizeY()), TPL_DIGITAL_NUM, FALSE);
	}
	else if (m_unitType == TPL_UNIT_CM)
	{
		m_dGridCellXSize.Init(UnitHelper::PixelsToCentimeters(m_pTplEditParam->GetGridCellSizeX()), TPL_DIGITAL_NUM, FALSE);
		m_dGridCellYSize.Init(UnitHelper::PixelsToCentimeters(m_pTplEditParam->GetGridCellSizeY()), TPL_DIGITAL_NUM, FALSE);
	}

	UpdateData(FALSE);
}

TplEditParamPtr DesignParamPage::GetTplEditParam()
{
	UpdateData(TRUE);

	if (m_bSnapGrid != m_pTplEditParam->IsSnapToGrid())
	{
		m_pTplEditParam->IsSnapToGrid(m_bSnapGrid);
		m_pTplEditParam->SetModified(TRUE);
	}

	/************************************************************************/
	// grid size.

	// get the data from the UI.
	int iGridCellXSize, iGridCellYSize;
	if (m_unitType == TPL_UNIT_PIXEL)
	{
		iGridCellXSize = (int)m_dGridCellXSize.Get_Number();
		iGridCellYSize = (int)m_dGridCellYSize.Get_Number();
	}
	else if (m_unitType == TPL_UNIT_INCH)
	{
		iGridCellXSize = UnitHelper::InchesToPixels(m_dGridCellXSize.Get_Number());
		iGridCellYSize = UnitHelper::InchesToPixels(m_dGridCellYSize.Get_Number());
	}
	else if (m_unitType == TPL_UNIT_CM)
	{
		iGridCellXSize = UnitHelper::CentimeterToPixels(m_dGridCellXSize.Get_Number());
		iGridCellYSize = UnitHelper::CentimeterToPixels(m_dGridCellYSize.Get_Number());
	}

	// update "m_pTemplateParam".
	if (iGridCellXSize != m_pTplEditParam->GetGridCellSizeX())
	{
		m_pTplEditParam->SetGridCellSizeX(iGridCellXSize);
		m_pTplEditParam->SetModified(TRUE);
	}
	if (iGridCellYSize != m_pTplEditParam->GetGridCellSizeY())
	{
		m_pTplEditParam->SetGridCellSizeY(iGridCellYSize);
		m_pTplEditParam->SetModified(TRUE);
	}
	/************************************************************************/

	return m_pTplEditParam;
}

END_CUTLEADER_NAMESPACE()

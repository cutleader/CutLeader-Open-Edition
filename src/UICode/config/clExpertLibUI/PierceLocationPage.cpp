#include "stdafx.h"
#include "PierceLocationPage.h"

#include "CGridColumnManagerProfile.h"
#include "CGridColumnTraitEdit.h"
#include "CGridColumnTraitCombo.h"
#include "CGridRowTraitXP.h"
#include "clUtilityResource.h"
#include "clUILibResource.h"
#include "clExpertLibResource.h"
#include "GplusViewPort.h"
#include "baseConst.h"
#include "DataBaseManager.h"

#include "ConditionTypeList.h"
#include "MachineItemLoader.h"

#include "ConditionValueList.h"
#include "PierceLocationInfo.h"
#include "PierceConfigItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const size_t MAX_TIP_TEXT_LENGTH = 1024;

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(PierceLocationPage, CResizableDialog)

PierceLocationPage::PierceLocationPage() : CLResizableDialog(PierceLocationPage::IDD)
{
	m_pGDIPlusDrawer.reset(new GplusViewPort());

	m_strMsg = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_CHANGE_PIERCE_PT);
}

PierceLocationPage::~PierceLocationPage()
{
}

void PierceLocationPage::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_MSG, m_strMsg);

	// for all kinds shapes.
	DDX_Control(pDX, IDC_ROUND, m_roundStatic);
	DDX_Control(pDX, IDC_OBROUND, m_obroundStatic);
	DDX_Control(pDX, IDC_RECT, m_rectStatic);
	DDX_Control(pDX, IDC_RADRECT, m_rcRectStatic);
	DDX_Control(pDX, IDC_SINGLED, m_singleDStatic);
	DDX_Control(pDX, IDC_DOUBLED, m_doubleDStatic);
	DDX_Control(pDX, IDC_TRIANGLE, m_triStatic);
	DDX_Control(pDX, IDC_RADSLOT, m_radSlotStatic);
	DDX_Control(pDX, IDC_INPOLY, m_inPolyStatic);
	DDX_Control(pDX, IDC_OUTPOLY, m_outPolyStatic);

	// for cut direction group.
	DDX_Control(pDX, ID_COMBO_INSIDE, m_ctrlInSideDir);
	DDX_Control(pDX, ID_COMBO_OUTSIDE, m_ctrlOutSideDir);

    DDX_Check(pDX, IDC_CHECK_SET_PIERCE_PT_BASED_ON_SHEET, m_bSetPiercePosBasedOnSheet);
}

BEGIN_MESSAGE_MAP(PierceLocationPage, CResizableDialog)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()

	ON_WM_CTLCOLOR()

	// the direction combo boxes
	ON_CBN_SELCHANGE(ID_COMBO_INSIDE, OnSelchangeInSide)
	ON_CBN_SELCHANGE(ID_COMBO_OUTSIDE, OnSelchangeOutSide)

	ON_BN_CLICKED(IDC_CHECK_SET_PIERCE_PT_BASED_ON_SHEET, OnCheck_SetPiercePosBasedOnSheet)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

BOOL PierceLocationPage::OnInitDialog()
{
	CLResizableDialog::OnInitDialog();

	// init something here.
	Init();

	SetBackgroundColor( SkinManager::GetBackgroundColor() );

	SetWindowTheme( GetDlgItem( IDC_EDIT_MSG )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_GROUP_STD_SHAPE )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_GROUP_POLY_SHAPE )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_GROUP_CUT_DIR )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( ID_COMBO_INSIDE )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( ID_COMBO_OUTSIDE )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_CHECK_SET_PIERCE_PT_BASED_ON_SHEET )->GetSafeHwnd(), L"", L"" );

	UpdateData(FALSE);

	return TRUE;
}

void PierceLocationPage::OnDestroy()
{
	CResizableDialog::OnDestroy();
}

void PierceLocationPage::OnLButtonDown(UINT nFlags, CPoint point)
{
	/************************************************************************/
	// get the rect of each shape, and transform to client coordinate.

	CRect roundStatic;
	m_roundStatic.GetWindowRect(&roundStatic);
	ScreenToClient(roundStatic);

	CRect obroundStatic;
	m_obroundStatic.GetWindowRect(&obroundStatic);
	ScreenToClient(obroundStatic);

	CRect rectStatic;
	m_rectStatic.GetWindowRect(&rectStatic);
	ScreenToClient(rectStatic);

	CRect rcRectStatic;
	m_rcRectStatic.GetWindowRect(&rcRectStatic);
	ScreenToClient(rcRectStatic);

	CRect singleDStatic;
	m_singleDStatic.GetWindowRect(&singleDStatic);
	ScreenToClient(singleDStatic);

	CRect doubleDStatic;
	m_doubleDStatic.GetWindowRect(&doubleDStatic);
	ScreenToClient(doubleDStatic);

	CRect triStatic;
	m_triStatic.GetWindowRect(&triStatic);
	ScreenToClient(triStatic);

	CRect radSlotStatic;
	m_radSlotStatic.GetWindowRect(&radSlotStatic);
	ScreenToClient(radSlotStatic);

	CRect inPolyStatic;
	m_inPolyStatic.GetWindowRect(&inPolyStatic);
	ScreenToClient(inPolyStatic);

	CRect outPolyStatic;
	m_outPolyStatic.GetWindowRect(&outPolyStatic);
	ScreenToClient(outPolyStatic);
	/************************************************************************/

	/************************************************************************/
	// tell PierceDrawStatic that i clicked you.
	// notes:
	//   1) if no PierceDrawStatic is clicked, do nothing.

	PierceDrawStatic* pTmpStatic = NULL;

	// decide which PierceDrawStatic was clicked.
	if (roundStatic.PtInRect(point))
		pTmpStatic = &m_roundStatic;
	else if (obroundStatic.PtInRect(point))
		pTmpStatic = &m_obroundStatic;
	else if (rectStatic.PtInRect(point))
		pTmpStatic = &m_rectStatic;
	else if (rcRectStatic.PtInRect(point))
		pTmpStatic = &m_rcRectStatic;
	else if (singleDStatic.PtInRect(point))
		pTmpStatic = &m_singleDStatic;
	else if (doubleDStatic.PtInRect(point))
		pTmpStatic = &m_doubleDStatic;
	else if (triStatic.PtInRect(point))
		pTmpStatic = &m_triStatic;
	else if (radSlotStatic.PtInRect(point))
		pTmpStatic = &m_radSlotStatic;
	else if (inPolyStatic.PtInRect(point))
		pTmpStatic = &m_inPolyStatic;
	else if (outPolyStatic.PtInRect(point))
		pTmpStatic = &m_outPolyStatic;
	else
		return;

	ClientToScreen(&point);
	pTmpStatic->ScreenToClient(&point);
	pTmpStatic->OnLButtonDown(nFlags, point);
	/************************************************************************/

	/************************************************************************/
	// update the data.

	// then get the pierce location angle after click.
	double dNewAngle = pTmpStatic->GetPierceLocAngle();

	// get the position
	PIERCE_POSITION pos = PierceLocationInfo::AngleToPosition(dNewAngle);

	// update the PierceLocationInfo using the new position.
	PierceLocationInfoPtr pierceInfoPtr = m_pierceSetting->GetPierceLocInfo(pTmpStatic->GetShape());
	pierceInfoPtr->SetPiercePos(pos);

	m_pierceSetting->SetModified(TRUE);
	/************************************************************************/

	CResizableDialog::OnLButtonDown(nFlags, point);
}

void PierceLocationPage::OnSelchangeInSide()
{
	DIRECTION insideCutDir;
	if (m_ctrlInSideDir.GetCurSel() == 0)
		insideCutDir = CCW;
	else
		insideCutDir = CW;

	/************************************************************************/
	// update data and UI.

	//
	m_inPolyStatic.SetDirection(insideCutDir);
	m_inPolyStatic.OnPaint();

	// for inside loop cut direction. these still should be updated.
	m_roundStatic.SetDirection(insideCutDir);
	m_roundStatic.OnPaint();
	m_obroundStatic.SetDirection(insideCutDir);
	m_obroundStatic.OnPaint();
	m_rectStatic.SetDirection(insideCutDir);
	m_rectStatic.OnPaint();
	m_rcRectStatic.SetDirection(insideCutDir);
	m_rcRectStatic.OnPaint();
	m_singleDStatic.SetDirection(insideCutDir);
	m_singleDStatic.OnPaint();
	m_doubleDStatic.SetDirection(insideCutDir);
	m_doubleDStatic.OnPaint();
	m_triStatic.SetDirection(insideCutDir);
	m_triStatic.OnPaint();
	m_radSlotStatic.SetDirection(insideCutDir);
	m_radSlotStatic.OnPaint();

	//
	m_pierceSetting->SetInsideDir(insideCutDir);
	m_pierceSetting->SetModified(TRUE);
	/************************************************************************/
}

void PierceLocationPage::OnSelchangeOutSide()
{
	if (m_ctrlOutSideDir.GetCurSel() == 0)
	{
		m_outPolyStatic.SetDirection(CCW);
		m_pierceSetting->SetOutSideDir(CCW);
	}
	else
	{
		m_outPolyStatic.SetDirection(CW);
		m_pierceSetting->SetOutSideDir(CW);
	}
	m_pierceSetting->SetModified(TRUE);

	//
	m_outPolyStatic.OnPaint();
}

void PierceLocationPage::OnCheck_SetPiercePosBasedOnSheet()
{
    UpdateData(TRUE);
    m_pierceSetting->SetIs_SetPiercePosBasedOnSheet(m_bSetPiercePosBasedOnSheet);
    m_pierceSetting->SetModified(TRUE);
}

void PierceLocationPage::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	//
	m_roundStatic.OnPaint();
	m_obroundStatic.OnPaint();
	m_rectStatic.OnPaint();
	m_rcRectStatic.OnPaint();
	m_singleDStatic.OnPaint();
	m_doubleDStatic.OnPaint();
	m_triStatic.OnPaint();
	m_radSlotStatic.OnPaint();

	// for polygon shape.
	m_inPolyStatic.OnPaint();
	m_outPolyStatic.OnPaint();
}

HBRUSH PierceLocationPage::OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor )
{
	return SkinManager::GetDialogCtrlColor( pDC, pWnd, nCtlColor );
}

void PierceLocationPage::DisplayPierceSetting(PierceConfigItemPtr pierceSetting)
{
	m_pierceSetting = pierceSetting;

	/************************************************************************/	
	// display pierce location for all kinds shapes.

	// for standard shape.
	InitShape(PIERCE_ROUND);
	InitShape(PIERCE_OBROUND);
	InitShape(PIERCE_RECT);
	InitShape(PIERCE_RCRECT);
	InitShape(PIERCE_SINGLED);
	InitShape(PIERCE_DOUBLED);
	InitShape(PIERCE_TRIANGLE);
	InitShape(PIERCE_RADSLOT);

	// for polygon shape.
	InitShape(PIERCE_INSIDEPOLYGON);
	InitShape(PIERCE_EXTERIORPOLYGON);
	/************************************************************************/

	// init the cut direction group.
	InitCutDir();

    m_bSetPiercePosBasedOnSheet = pierceSetting->GetIs_SetPiercePosBasedOnSheet();

	UpdateData(FALSE);

	// now we did not change anything yet.
	m_pierceSetting->SetModified(FALSE);
}

void PierceLocationPage::LayoutCtrl()
{
	AddAnchor(IDC_STATIC_INFO, TOP_LEFT);
	AddAnchor(IDC_EDIT_MSG, TOP_LEFT, TOP_RIGHT);

	/************************************************************************/
	// for standard shape.

	AddAnchor(IDC_GROUP_STD_SHAPE, TOP_LEFT);

	//
	AddAnchor(IDC_ROUND, TOP_LEFT);
	AddAnchor(IDC_STATIC_ROUND, TOP_LEFT);
	//
	AddAnchor(IDC_OBROUND, TOP_LEFT);
	AddAnchor(IDC_STATIC_OBROUND, TOP_LEFT);
	//
	AddAnchor(IDC_RECT, TOP_LEFT);
	AddAnchor(IDC_STATIC_SQR, TOP_LEFT);
	//
	AddAnchor(IDC_RADRECT, TOP_LEFT);
	AddAnchor(IDC_STATIC_RADRECT, TOP_LEFT);
	//
	AddAnchor(IDC_SINGLED, TOP_LEFT);
	AddAnchor(IDC_STATIC_SINGLED, TOP_LEFT);
	//
	AddAnchor(IDC_DOUBLED, TOP_LEFT);
	AddAnchor(IDC_STATIC_DOUBLED, TOP_LEFT);
	//
	AddAnchor(IDC_TRIANGLE, TOP_LEFT);
	AddAnchor(IDC_STATIC_TRIANGLE, TOP_LEFT);
	//
	AddAnchor(IDC_RADSLOT, TOP_LEFT);
	AddAnchor(IDC_STATIC_RSLOT, TOP_LEFT);
	/************************************************************************/

	/************************************************************************/
	// for polygon shape.

	AddAnchor(IDC_GROUP_POLY_SHAPE, TOP_LEFT);

	//
	AddAnchor(IDC_INPOLY, TOP_LEFT);
	AddAnchor(IDC_STATIC_INSIDE, TOP_LEFT);
	//
	AddAnchor(IDC_OUTPOLY, TOP_LEFT);
	AddAnchor(IDC_STATIC_OUTSIDE, TOP_LEFT);
	/************************************************************************/

	/************************************************************************/
	// for cut direction group.

	AddAnchor(IDC_GROUP_CUT_DIR, TOP_LEFT);

	//
	AddAnchor(IDC_STATIC_INSIDE_DIR, TOP_LEFT);
	AddAnchor(ID_COMBO_INSIDE, TOP_LEFT);
	//
	AddAnchor(IDC_STATIC_OUTSIDE_DIR, TOP_LEFT);
	AddAnchor(ID_COMBO_OUTSIDE, TOP_LEFT);
	/************************************************************************/

    AddAnchor(IDC_CHECK_SET_PIERCE_PT_BASED_ON_SHEET, TOP_LEFT);
}

void PierceLocationPage::InitShape(PIERCE_SHAPETYPE shape)
{
	PierceLocationInfoPtr pPierceLoc = m_pierceSetting->GetPierceLocInfo(shape);
	PIERCE_POSITION pos = pPierceLoc->GetPiercePos();

	// get the angle from the pierce position.
	double dAngle = PierceLocationInfo::PositionToAngle(pos);

	/************************************************************************/
	// set the direction.
	// notes:
	//   1) the standard shape always use the inside cut direction.

	PierceDrawStatic* pTmpStatic = NULL;
	if (shape == PIERCE_ROUND)
	{
		pTmpStatic = &m_roundStatic;
		pTmpStatic->SetDirection(m_pierceSetting->GetInsideDir());
	}
	else if (shape == PIERCE_OBROUND)
	{
		pTmpStatic = &m_obroundStatic;
		pTmpStatic->SetDirection(m_pierceSetting->GetInsideDir());
	}
	else if (shape == PIERCE_RECT)
	{
		pTmpStatic = &m_rectStatic;
		pTmpStatic->SetDirection(m_pierceSetting->GetInsideDir());
	}
	else if (shape == PIERCE_RCRECT)
	{
		pTmpStatic = &m_rcRectStatic;
		pTmpStatic->SetDirection(m_pierceSetting->GetInsideDir());
	}
	else if (shape == PIERCE_SINGLED)
	{
		pTmpStatic = &m_singleDStatic;
		pTmpStatic->SetDirection(m_pierceSetting->GetInsideDir());
	}
	else if (shape == PIERCE_DOUBLED)
	{
		pTmpStatic = &m_doubleDStatic;
		pTmpStatic->SetDirection(m_pierceSetting->GetInsideDir());
	}
	else if (shape == PIERCE_TRIANGLE)
	{
		pTmpStatic = &m_triStatic;
		pTmpStatic->SetDirection(m_pierceSetting->GetInsideDir());
	}
	else if (shape == PIERCE_RADSLOT)
	{
		pTmpStatic = &m_radSlotStatic;
		pTmpStatic->SetDirection(m_pierceSetting->GetInsideDir());
	}

	//
	if (shape == PIERCE_INSIDEPOLYGON)
	{
		pTmpStatic = &m_inPolyStatic;
		pTmpStatic->SetDirection(m_pierceSetting->GetInsideDir());
	}
	else if (shape == PIERCE_EXTERIORPOLYGON)
	{
		pTmpStatic = &m_outPolyStatic;
		pTmpStatic->SetDirection(m_pierceSetting->GetOutSideDir());
	}
	/************************************************************************/

	pTmpStatic->SetShape(shape);
	pTmpStatic->SetPierceLocAngle(dAngle);
}

void PierceLocationPage::InitCutDir()
{
	DIRECTION insideCutDir = m_pierceSetting->GetInsideDir();
	if ( insideCutDir == CCW ) {
		m_ctrlInSideDir.SetCurSel( 0 );
	}
	else if ( insideCutDir == CW ) {
		m_ctrlInSideDir.SetCurSel(1);
	}

	// show ComboBox
	m_ctrlInSideDir.Invalidate( TRUE );

	DIRECTION outsideCutDir = m_pierceSetting->GetOutSideDir();
	if ( outsideCutDir == CCW ) {
		m_ctrlOutSideDir.SetCurSel( 0 );
	}
	else if ( outsideCutDir == CW ) {
		m_ctrlOutSideDir.SetCurSel( 1 );
	}

	// show ComboBox
	m_ctrlOutSideDir.Invalidate( TRUE );
}

void PierceLocationPage::Init()
{
	m_roundStatic.SetDrawer(m_pGDIPlusDrawer);
	m_obroundStatic.SetDrawer(m_pGDIPlusDrawer);
	m_rectStatic.SetDrawer(m_pGDIPlusDrawer);
	m_rcRectStatic.SetDrawer(m_pGDIPlusDrawer);
	m_singleDStatic.SetDrawer(m_pGDIPlusDrawer);
	m_doubleDStatic.SetDrawer(m_pGDIPlusDrawer);
	m_triStatic.SetDrawer(m_pGDIPlusDrawer);
	m_radSlotStatic.SetDrawer(m_pGDIPlusDrawer);
	m_inPolyStatic.SetDrawer(m_pGDIPlusDrawer);
	m_outPolyStatic.SetDrawer(m_pGDIPlusDrawer);

	// layout the controls.
	LayoutCtrl();
}

END_CUTLEADER_NAMESPACE()

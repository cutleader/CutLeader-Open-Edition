#include "stdafx.h"
#include "MicroJointAssignmentPage.h"

#include "baseConst.h"
#include "CLOptionInfo.h"
#include "ProductParam.h"

#include "clExpertLibResource.h"

#include "Uxtheme.h"
#pragma comment(lib, "Uxtheme.lib")

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(MicroJointAssignmentPage, CResizableDialog)

MicroJointAssignmentPage::MicroJointAssignmentPage() : CLResizableDialog(MicroJointAssignmentPage::IDD)
{
	m_bDisKillFocusEnt = FALSE;
}

MicroJointAssignmentPage::~MicroJointAssignmentPage()
{
}

void MicroJointAssignmentPage::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);

	/************************************************************************/
	// “自动分配”分组。

	DDX_Control(pDX, IDC_TAB_RANGE, m_rangeTab);
	DDX_Control(pDX, IDC_EDIT_HOLES_SIZE, m_dHoleJointPtSize);
	DDX_Control(pDX, IDC_COMBO_HOLES_QUANTITY, m_holeJointPtCount);
	DDX_Control(pDX, IDC_EDIT_HOLES_MAXSIZEX, m_dHoleXSize);
	DDX_Control(pDX, IDC_EDIT_HOLES_MAXSIZEY, m_dHoleYSize);
	DDX_Control(pDX, IDC_STATIC_HOLE, m_holeImg);

	DDX_Control(pDX, IDC_EDIT_PERIMETER_SIZE, m_dPeriJointPtSize);
	DDX_Control(pDX, IDC_COMBO_PERIMETER_QUANTITY, m_periJointPtCount);
	DDX_Control(pDX, IDC_EDIT_PERIMETER_MAXSIZEX, m_dPeriXSize);
	DDX_Control(pDX, IDC_EDIT_PERIMETER_MAXSIZEY, m_dPeriYSize);
	DDX_Control(pDX, IDC_STATIC_PERI, m_periImg);
	/************************************************************************/

	/************************************************************************/
	// lead in/out info

	DDX_Check(pDX, IDC_CHECK_ENABLELEADIN, m_enableLeadIn);
	DDX_Control(pDX, IDC_EDIT_LEADIN_LEN, m_dLeanInLen);
	DDX_Control(pDX, IDC_EDIT_LEADIN_ANGLE, m_dLeadInAngle);

	DDX_Check(pDX, IDC_CHECK_ENABLELEADOUT, m_enableLeadOut);
	DDX_Control(pDX, IDC_EDIT_LEADOUT_LEN, m_dLeanOutLen);
	DDX_Control(pDX, IDC_EDIT_LEADOUT_ANGLE, m_dLeadOutAngle);
	/************************************************************************/

	DDX_Control(pDX, IDC_EDIT_M_MJ_SIZE, m_dManualMJWid);
}

BEGIN_MESSAGE_MAP(MicroJointAssignmentPage, CResizableDialog)
	ON_WM_CTLCOLOR()

	/************************************************************************/
	// “自动分配”分组。

	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_RANGE, OnSelchangeTab)

	ON_CBN_SELCHANGE(IDC_COMBO_HOLES_QUANTITY, OnSelChangeHoleQuantity)
	ON_EN_KILLFOCUS(IDC_EDIT_HOLES_SIZE, OnKillFocusHolesSize)
	ON_EN_KILLFOCUS(IDC_EDIT_HOLES_MAXSIZEX, OnKillFocusHolesSizeX)
	ON_EN_KILLFOCUS(IDC_EDIT_HOLES_MAXSIZEY, OnKillFocusHolesSizeY)

	ON_CBN_SELCHANGE(IDC_COMBO_PERIMETER_QUANTITY, OnSelchangePeriQuantity)
	ON_EN_KILLFOCUS(IDC_EDIT_PERIMETER_SIZE, OnKillFocusPerimeterSize)
	ON_EN_KILLFOCUS(IDC_EDIT_PERIMETER_MAXSIZEX, OnKillFocusPerimeterSizeX)
	ON_EN_KILLFOCUS(IDC_EDIT_PERIMETER_MAXSIZEY, OnKillFocusPerimeterSizeY)

	ON_WM_LBUTTONDOWN()
	/************************************************************************/

	// "Lead In/out" group.
	ON_BN_CLICKED(IDC_CHECK_ENABLELEADIN, OnCheckEnableLeadIn)
	ON_EN_KILLFOCUS(IDC_EDIT_LEADIN_LEN, OnKillFocusLeadInLen)
	ON_EN_KILLFOCUS(IDC_EDIT_LEADIN_ANGLE, OnKillFocusLeadInAngle)
	ON_BN_CLICKED(IDC_CHECK_ENABLELEADOUT, OnCheckEnableLeadOut)
	ON_EN_KILLFOCUS(IDC_EDIT_LEADOUT_LEN, OnKillFocusLeadOutLen)
	ON_EN_KILLFOCUS(IDC_EDIT_LEADOUT_ANGLE, OnKillFocusLeadOutAngle)

	ON_EN_KILLFOCUS(IDC_EDIT_M_MJ_SIZE, OnKillFocusManualWid)
END_MESSAGE_MAP()

BOOL MicroJointAssignmentPage::OnInitDialog()
{
	CLResizableDialog::OnInitDialog();

	// init something
	Init();

	SetBackgroundColor( SkinManager::GetBackgroundColor() );

	SetWindowTheme( GetDlgItem( IDC_STATIC_AUTO_MJ )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_GROUP_HOLE )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_STATIC_LOOP_RECT1 )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_COMBO_HOLES_QUANTITY )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_STATIC_BOUNDARY )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_STATIC_LOOP_RECT2 )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_COMBO_PERIMETER_QUANTITY )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_STATIC_M_ASSIGN )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_STATIC_MJ_LEAD )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_CHECK_ENABLELEADIN )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_CHECK_ENABLELEADOUT )->GetSafeHwnd(), L"", L"" );

	UpdateData(FALSE);

	return TRUE;
}

HBRUSH MicroJointAssignmentPage::OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor )
{
	return SkinManager::GetDialogCtrlColor( pDC, pWnd, nCtlColor );
}

void MicroJointAssignmentPage::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_bDisKillFocusEnt = TRUE;

	// init the micro joint rules for specified range.
	int iSel = m_rangeTab.GetCurSel();
	MicroJointRangePtr pRange = m_pMicroJointSetting->GetRangeList()->at(iSel);
	InitRange(pRange);

	m_bDisKillFocusEnt = FALSE;

	*pResult = 0;
}

void MicroJointAssignmentPage::OnSelChangeHoleQuantity()
{
	// get the current range info.
	int iRangeIndex = m_rangeTab.GetCurSel();
	MicroJointRangeInfoPtr pRangeInfo = m_pMicroJointSetting->GetRangeInfo(iRangeIndex, TRUE);

	// update "MicroJointRangeInfo::m_iQuantity".
	pRangeInfo->SetQuantity(m_holeJointPtCount.GetCurSel());
	m_pMicroJointSetting->SetModified(TRUE);

	// then we update the preview window.
	InitPosPreview(pRangeInfo->GetQuantity(), pRangeInfo->GetMicroJointPos(), TRUE);
}

void MicroJointAssignmentPage::OnSelchangePeriQuantity()
{
	// get the current range info.
	int iRangeIndex = m_rangeTab.GetCurSel();
	MicroJointRangeInfoPtr pMicroJointRangeInfo = m_pMicroJointSetting->GetRangeInfo(iRangeIndex, FALSE);

	// update "MicroJointRangeInfo::m_iQuantity".
	pMicroJointRangeInfo->SetQuantity(m_periJointPtCount.GetCurSel());
	m_pMicroJointSetting->SetModified(TRUE);

	// then we update the preview window.
	InitPosPreview(pMicroJointRangeInfo->GetQuantity(), pMicroJointRangeInfo->GetMicroJointPos(), FALSE);
}

void MicroJointAssignmentPage::OnKillFocusManualWid()
{
	if (m_pMicroJointSetting->GetManualWidth() != m_dManualMJWid.Get_Number())
	{
		m_pMicroJointSetting->SetManualWidth(m_dManualMJWid.Get_Number());
		m_pMicroJointSetting->SetModified(TRUE);
	}
}

void MicroJointAssignmentPage::OnLButtonDown(UINT nFlags, CPoint point)
{
	CRect holeStaticRect, periStaticRect;
	m_holeImg.GetWindowRect(&holeStaticRect);
	m_periImg.GetWindowRect(&periStaticRect);

	ScreenToClient(holeStaticRect);
	ScreenToClient(periStaticRect);

	BOOL bHole = FALSE;
	int iJointCount = 0, iImgIdx = 0;
	if (holeStaticRect.PtInRect(point)) {
		m_holeImg.OnLButtonDown(nFlags, point);
		bHole = TRUE;
		iJointCount = m_holeJointPtCount.GetCurSel();
		iImgIdx = m_holeImg.GetCurrentImage();
	}
	else if (periStaticRect.PtInRect(point)) {
		m_periImg.OnLButtonDown(nFlags, point);
		bHole = FALSE;
		iJointCount = m_periJointPtCount.GetCurSel();
		iImgIdx = m_periImg.GetCurrentImage();
	}
	else {
		return;
	}

	MicroJointRangeInfoPtr pMicroJointRangeInfo = m_pMicroJointSetting->GetRangeInfo(m_rangeTab.GetCurSel(), bHole); // get range info
	MicroJointPosition emMicroJointPosition = ImgIdxToMicroJointPos(iImgIdx, iJointCount); // get current image index.
	pMicroJointRangeInfo->SetMicroJointPos(emMicroJointPosition); // set micro joint position.
	m_pMicroJointSetting->SetModified(TRUE);

	CDialogEx::OnLButtonDown(nFlags, point);
}

void MicroJointAssignmentPage::OnKillFocusHolesSize()
{
	if (m_bDisKillFocusEnt)
		return;

	UpdateData(TRUE);

	// get rangeinfo
	int iRangeIdx = m_rangeTab.GetCurSel();
	MicroJointRangeInfoPtr rangeInfo = m_pMicroJointSetting->GetRangeInfo(iRangeIdx, TRUE);

	// set the joint size
	if (rangeInfo->GetJointWidth() != m_dHoleJointPtSize.Get_Number())
	{
		rangeInfo->SetJointWidth(m_dHoleJointPtSize.Get_Number());
		m_pMicroJointSetting->SetModified(TRUE);
	}
}

void MicroJointAssignmentPage::OnKillFocusHolesSizeX()
{
	if (m_bDisKillFocusEnt)
		return;

	UpdateData(TRUE);

	// get rangeinfo
	int iRangeIdx = m_rangeTab.GetCurSel();
	MicroJointRangeInfoPtr rangeInfo = m_pMicroJointSetting->GetRangeInfo(iRangeIdx, TRUE);

	if (rangeInfo->GetRangeX() != m_dHoleXSize.Get_Number())
	{
		rangeInfo->SetRangeX(m_dHoleXSize.Get_Number());
		m_pMicroJointSetting->SetModified(TRUE);
	}
}

void MicroJointAssignmentPage::OnKillFocusHolesSizeY()
{
	if (m_bDisKillFocusEnt)
		return;

	UpdateData(TRUE);

	// get rangeinfo
	int iRangeIdx = m_rangeTab.GetCurSel();
	MicroJointRangeInfoPtr rangeInfo = m_pMicroJointSetting->GetRangeInfo(iRangeIdx, TRUE);

	if (rangeInfo->GetRangeY() != m_dHoleYSize.Get_Number())
	{
		rangeInfo->SetRangeY(m_dHoleYSize.Get_Number());
		m_pMicroJointSetting->SetModified(TRUE);
	}
}

void MicroJointAssignmentPage::OnKillFocusPerimeterSize()
{
	if (m_bDisKillFocusEnt)
		return;

	UpdateData(TRUE);

	// get rangeinfo
	int iRangeIdx = m_rangeTab.GetCurSel();
	MicroJointRangeInfoPtr rangeInfo = m_pMicroJointSetting->GetRangeInfo(iRangeIdx, FALSE);

	// set the joint size.
	if (rangeInfo->GetJointWidth() != m_dPeriJointPtSize.Get_Number())
	{
		rangeInfo->SetJointWidth(m_dPeriJointPtSize.Get_Number());
		m_pMicroJointSetting->SetModified(TRUE);
	}
}

void MicroJointAssignmentPage::OnKillFocusPerimeterSizeX()
{
	if (m_bDisKillFocusEnt)
		return;

	UpdateData(TRUE);

	// get rangeinfo
	int iRangeIdx = m_rangeTab.GetCurSel();
	MicroJointRangeInfoPtr rangeInfo = m_pMicroJointSetting->GetRangeInfo(iRangeIdx, FALSE);

	if (rangeInfo->GetRangeX() != m_dPeriXSize.Get_Number())
	{
		rangeInfo->SetRangeX(m_dPeriXSize.Get_Number());
		m_pMicroJointSetting->SetModified(TRUE);
	}
}

void MicroJointAssignmentPage::OnKillFocusPerimeterSizeY()
{
	if (m_bDisKillFocusEnt)
		return;

	UpdateData(TRUE);

	// get rangeinfo
	int iRangeIdx = m_rangeTab.GetCurSel();
	MicroJointRangeInfoPtr rangeInfo = m_pMicroJointSetting->GetRangeInfo(iRangeIdx, FALSE);

	if (rangeInfo->GetRangeY() != m_dPeriYSize.Get_Number())
	{
		rangeInfo->SetRangeY(m_dPeriYSize.Get_Number());
		m_pMicroJointSetting->SetModified(TRUE);
	}
}

void MicroJointAssignmentPage::OnCheckEnableLeadIn()
{
	if (m_bDisKillFocusEnt)
		return;

	UpdateData(TRUE);

	m_pMicroJointSetting->IsLeadIn(m_enableLeadIn);
	m_pMicroJointSetting->SetModified(TRUE);

	if (m_enableLeadIn)
	{
		m_dLeanInLen.EnableWindow(TRUE);
		m_dLeadInAngle.EnableWindow(TRUE);
	}
	else
	{
		m_dLeanInLen.EnableWindow(FALSE);
		m_dLeadInAngle.EnableWindow(FALSE);
	}
}

void MicroJointAssignmentPage::OnKillFocusLeadInLen()
{
	if (m_bDisKillFocusEnt)
		return;

	UpdateData(TRUE);

	if (m_pMicroJointSetting->GetLeadInLen() != m_dLeanInLen.Get_Number())
	{
		m_pMicroJointSetting->SetLeadInLen(m_dLeanInLen.Get_Number());
		m_pMicroJointSetting->SetModified(TRUE);
	}
}

void MicroJointAssignmentPage::OnKillFocusLeadInAngle()
{
	if (m_bDisKillFocusEnt)
		return;

	UpdateData(TRUE);

	if (m_pMicroJointSetting->GetLeadInAngle() != m_dLeadInAngle.Get_Number())
	{
		m_pMicroJointSetting->SetLeadInAngle(m_dLeadInAngle.Get_Number());
		m_pMicroJointSetting->SetModified(TRUE);
	}
}

void MicroJointAssignmentPage::OnCheckEnableLeadOut()
{
	if (m_bDisKillFocusEnt)
		return;

	UpdateData(TRUE);

	m_pMicroJointSetting->IsLeadOut(m_enableLeadOut);
	m_pMicroJointSetting->SetModified(TRUE);

	if (m_enableLeadOut)
	{
		m_dLeanOutLen.EnableWindow(TRUE);
		m_dLeadOutAngle.EnableWindow(TRUE);
	}
	else
	{
		m_dLeanOutLen.EnableWindow(FALSE);
		m_dLeadOutAngle.EnableWindow(FALSE);
	}
}

void MicroJointAssignmentPage::OnKillFocusLeadOutLen()
{
	if (m_bDisKillFocusEnt)
		return;

	UpdateData(TRUE);

	if (m_pMicroJointSetting->GetLeadOutLen() != m_dLeanOutLen.Get_Number())
	{
		m_pMicroJointSetting->SetLeadOutLen(m_dLeanOutLen.Get_Number());
		m_pMicroJointSetting->SetModified(TRUE);
	}
}

void MicroJointAssignmentPage::OnKillFocusLeadOutAngle()
{
	if (m_bDisKillFocusEnt)
		return;

	UpdateData(TRUE);

	if (m_pMicroJointSetting->GetLeadOutAngle() != m_dLeadOutAngle.Get_Number())
	{
		m_pMicroJointSetting->SetLeadOutAngle(m_dLeadOutAngle.Get_Number());
		m_pMicroJointSetting->SetModified(TRUE);
	}
}

void MicroJointAssignmentPage::DisplayMicroJointSetting(MicroJointConfigItemPtr pMicroJointConfigItem)
{
	m_pMicroJointSetting = pMicroJointConfigItem;

	/************************************************************************/
	//  init the content in the "range" group.

	// get the current MicroJointRange object.
	int iRangeIdx = m_rangeTab.GetCurSel();
	const vector<MicroJointRangePtr>* pRangeList = pMicroJointConfigItem->GetRangeList();
	MicroJointRangePtr pMJRange = pRangeList->at(iRangeIdx);

	// init this micro joint range info.
	InitRange(pMJRange);
	/************************************************************************/

	m_dManualMJWid.Init(pMicroJointConfigItem->GetManualWidth(), DIGITAL_NUM, FALSE);

	/************************************************************************/
	// init lead in/out info.

	m_dLeanInLen.Init(pMicroJointConfigItem->GetLeadInLen(), DIGITAL_NUM, FALSE);
	m_dLeadInAngle.Init(pMicroJointConfigItem->GetLeadInAngle(), DIGITAL_NUM, FALSE);
	if (pMicroJointConfigItem->IsLeadIn())
	{
		m_dLeanInLen.EnableWindow(TRUE);
		m_dLeadInAngle.EnableWindow(TRUE);
	}
	else
	{
		m_dLeanInLen.EnableWindow(FALSE);
		m_dLeadInAngle.EnableWindow(FALSE);
	}

	m_dLeanOutLen.Init(pMicroJointConfigItem->GetLeadOutLen(), DIGITAL_NUM, FALSE);
	m_dLeadOutAngle.Init(pMicroJointConfigItem->GetLeadOutAngle(), DIGITAL_NUM, FALSE);
	if (pMicroJointConfigItem->IsLeadOut())
	{
		m_dLeanOutLen.EnableWindow(TRUE);
		m_dLeadOutAngle.EnableWindow(TRUE);
	}
	else
	{
		m_dLeanOutLen.EnableWindow(FALSE);
		m_dLeadOutAngle.EnableWindow(FALSE);
	}

	m_enableLeadIn = pMicroJointConfigItem->IsLeadIn();
	m_enableLeadOut = pMicroJointConfigItem->IsLeadOut();
	/************************************************************************/

	UpdateData(FALSE);
}

void MicroJointAssignmentPage::Init()
{
	/************************************************************************/
	// init the tab control.

	CString str;
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_RANGEONE);
	m_rangeTab.InsertItem(0, str);

	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_RANGETWO);
	m_rangeTab.InsertItem(1, str);

	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_RANGETHREE);
	m_rangeTab.InsertItem(2, str);

	for (int i = 0; i < m_rangeTab.GetItemCount(); i++ ) {
		m_rangeTab.SetTabColor( i, SkinManager::GetBackgroundColor() );
	}
	/************************************************************************/

	LayoutControl();
}

void MicroJointAssignmentPage::LayoutControl()
{
	/************************************************************************/
	// “自动分配”分组。

	AddAnchor(IDC_STATIC_AUTO_MJ, TOP_LEFT);
	AddAnchor(IDC_TAB_RANGE, TOP_LEFT);

	// 内部轮廓分组。
	AddAnchor(IDC_GROUP_HOLE, TOP_LEFT);
	AddAnchor(IDC_STATIC_LOOP_RECT1, TOP_LEFT);
	//
	AddAnchor(IDC_STATIC_QTY, TOP_LEFT);
	AddAnchor(IDC_COMBO_HOLES_QUANTITY, TOP_LEFT);
	//
	AddAnchor(IDC_STATIC_SIZE, TOP_LEFT);
	AddAnchor(IDC_EDIT_HOLES_SIZE, TOP_LEFT);
	//
	AddAnchor(IDC_STATIC_X1, TOP_LEFT);
	AddAnchor(IDC_EDIT_HOLES_MAXSIZEX, TOP_LEFT);
	//
	AddAnchor(IDC_STATIC_Y1, TOP_LEFT);
	AddAnchor(IDC_EDIT_HOLES_MAXSIZEY, TOP_LEFT);
	//
	AddAnchor(IDC_STATIC_HOLE, TOP_LEFT);

	// 外部轮廓分组。
	AddAnchor(IDC_STATIC_BOUNDARY, TOP_LEFT);
	AddAnchor(IDC_STATIC_LOOP_RECT2, TOP_LEFT);
	//
	AddAnchor(IDC_STATIC_QTY2, TOP_LEFT);
	AddAnchor(IDC_COMBO_PERIMETER_QUANTITY, TOP_LEFT);
	//
	AddAnchor(IDC_STATIC_SIZE2, TOP_LEFT);
	AddAnchor(IDC_EDIT_PERIMETER_SIZE, TOP_LEFT);
	//
	AddAnchor(IDC_STATIC_X2, TOP_LEFT);
	AddAnchor(IDC_EDIT_PERIMETER_MAXSIZEX, TOP_LEFT);
	//
	AddAnchor(IDC_STATIC_Y2, TOP_LEFT);
	AddAnchor(IDC_EDIT_PERIMETER_MAXSIZEY, TOP_LEFT);
	//
	AddAnchor(IDC_STATIC_PERI, TOP_LEFT);
	/************************************************************************/

	// “手动分配”分组。
	AddAnchor(IDC_STATIC_M_ASSIGN, TOP_LEFT);
	AddAnchor(IDC_STATIC_M_MJ_SIZE, TOP_LEFT);
	AddAnchor(IDC_EDIT_M_MJ_SIZE, TOP_LEFT);

	/************************************************************************/
	// “引入引出线”分组。

	AddAnchor(IDC_STATIC_MJ_LEAD, TOP_LEFT);

	AddAnchor(IDC_CHECK_ENABLELEADIN, TOP_LEFT);
	AddAnchor(IDC_STATIC_LENGTH1, TOP_LEFT);
	AddAnchor(IDC_EDIT_LEADIN_LEN, TOP_LEFT);
	AddAnchor(IDC_STATIC_ANGLE1, TOP_LEFT);
	AddAnchor(IDC_EDIT_LEADIN_ANGLE, TOP_LEFT);

	AddAnchor(IDC_CHECK_ENABLELEADOUT, TOP_LEFT);
	AddAnchor(IDC_STATIC_LENGTH2, TOP_LEFT);
	AddAnchor(IDC_EDIT_LEADOUT_LEN, TOP_LEFT);
	AddAnchor(IDC_STATIC_ANGLE2, TOP_LEFT);
	AddAnchor(IDC_EDIT_LEADOUT_ANGLE, TOP_LEFT);
	/************************************************************************/
}

void MicroJointAssignmentPage::InitRange( MicroJointRangePtr range )
{
	/************************************************************************/
	// init "Hole" group

	// get joint point count
	MicroJointRangeInfoPtr rangeInfo = range->GetHoleRange();
	int iCount = rangeInfo->GetQuantity();

	// init the "Quantity" combo box.
	CString str;
	str.Format( _T( "%d" ), iCount );
	for ( int i = 0; i < m_holeJointPtCount.GetCount(); i++ ) {
		CString tmpStr;
		m_holeJointPtCount.GetLBText( i, tmpStr );
		if ( tmpStr.Compare( str ) == 0 ) {
			m_holeJointPtCount.SetCurSel( i );
			break;
		}
	}

	// show ComboBox
	m_holeJointPtCount.Invalidate( TRUE );

	// init joint size.
	m_dHoleJointPtSize.Init( rangeInfo->GetJointWidth(), DIGITAL_NUM, FALSE );

	// init "Maximum Overall" values
	m_dHoleXSize.Init( rangeInfo->GetRangeX(), DIGITAL_NUM, FALSE );
	m_dHoleYSize.Init( rangeInfo->GetRangeY(), DIGITAL_NUM, FALSE );

	// display the image in ImgStatic control.
	InitPosPreview( iCount, rangeInfo->GetMicroJointPos(), TRUE) ;
	/************************************************************************/

	/************************************************************************/
	// init "Perimeter" group

	// get joint point count
	MicroJointRangeInfoPtr rangeInfo1 = range->GetPerimeterRange();
	int iCount1 = rangeInfo1->GetQuantity();

	// init the "Quantity" combo box.
	CString str1;
	str1.Format( _T( "%d" ), iCount1 );
	for ( int j = 0; j < m_periJointPtCount.GetCount(); j++ ) {
		CString tmpStr1;
		m_periJointPtCount.GetLBText( j, tmpStr1 );
		if ( tmpStr1.Compare( str1 ) == 0 ) {
			m_periJointPtCount.SetCurSel( j );
			break;
		}
	}

	// show ComboBox
	m_periJointPtCount.Invalidate( TRUE );

	// init joint size.
	m_dPeriJointPtSize.Init( rangeInfo1->GetJointWidth(), DIGITAL_NUM, FALSE );

	// init "Maximum Overall" values
	m_dPeriXSize.Init( rangeInfo1->GetRangeX(), DIGITAL_NUM, FALSE );
	m_dPeriYSize.Init( rangeInfo1->GetRangeY(), DIGITAL_NUM, FALSE );

	// display the image in ImgStatic control.
	InitPosPreview( iCount1, rangeInfo1->GetMicroJointPos(), FALSE );
	/************************************************************************/
}

int MicroJointAssignmentPage::MicroJointPosToImgIdx(MicroJointPosition emMicroJointPosition)
{
	switch(emMicroJointPosition)
	{
	case ONE_BOTTOM:
		return 3;

	case ONE_RIGHT:
		return 0;

	case ONE_TOP:
		return 1;

	case ONE_LEFT:
		return 2;

	case TWO_VERTICAL:
		return 0;

	case TWO_HORIZONTAL:
		return 1;

	case THREE_BOTTOM:
		return 1;

	case THREE_RIGHT:
		return 2;

	case THREE_TOP:
		return 3;

	case THREE_LEFT:
		return 0;

	case FOUR_FULL:
		return 0;

	case SIX_DOWN:
		return 0;

	case SIX_UP:
		return 1;

	default:
		break;
	}

	// i think we cannot reach here.
	return 3;
}

MicroJointPosition MicroJointAssignmentPage::ImgIdxToMicroJointPos(int imgIdx, int iJointCount)
{
	switch(iJointCount)
	{
	case 1:
		{
			switch(imgIdx)
			{
			case 0:
				return ONE_RIGHT;
			case 1:
				return ONE_TOP;
			case 2:
				return ONE_LEFT;
			case 3:
				return ONE_BOTTOM;
			default:
				break;
			}
			break;
		}
	case 2:
		{
			switch(imgIdx)
			{
			case 0:
				return TWO_VERTICAL;
			case 1:
				return TWO_HORIZONTAL;
			default:
				break;
			}
			break;
		}
	case 3:
		{
			switch(imgIdx)
			{
			case 0:
				return THREE_LEFT;
			case 1:
				return THREE_BOTTOM;
			case 2:
				return THREE_RIGHT;
			case 3:
				return THREE_TOP;
			default:
				break;
			}
			break;
		}
	case 4:
		{
			switch(imgIdx)
			{
			case 0:
				return FOUR_FULL;
			default:
				break;
			}
			break;
		}
	case 6:
		{
			switch(imgIdx)
			{
			case 0:
				return SIX_DOWN;
			case 1:
				return SIX_UP;
			default:
				break;
			}
			break;
		}
	default:
		break;
	}

	// i think we cannot reach here.
	return ONE_RIGHT;
}

void MicroJointAssignmentPage::InitPosPreview(int iJointCount, MicroJointPosition emMicroJointPosition, BOOL bHole)
{
	// get the image index.
	int iImgIdx = MicroJointPosToImgIdx(emMicroJointPosition);

	int iImgCount = 0;
	// create the proper image list
	CImageList* tmpImgList;
	COLORREF crBkColor = RGB(255,0,0);//SkinManager::GetBackgroundColor();
	if (iJointCount == 0)
	{
		m_MJ0ImgList.DeleteImageList();
		m_MJ0ImgList.Create(IDB_MJ_QUANTITY0, 64, 1, crBkColor );
		m_MJ0ImgList.SetBkColor(RGB(0,255,0));
		tmpImgList = &m_MJ0ImgList;

		iImgCount = 1;
	}
	else if (iJointCount == 1)
	{
		m_MJ1ImgList.DeleteImageList();
		m_MJ1ImgList.Create(IDB_MJ_QUANTITY1, 64, 4, crBkColor );
		tmpImgList = &m_MJ1ImgList;

		iImgCount = 4;
	}
	else if (iJointCount == 2)
	{
		m_MJ2ImgList.DeleteImageList();
		m_MJ2ImgList.Create(IDB_MJ_QUANTITY2, 64, 2, crBkColor );
		tmpImgList = &m_MJ2ImgList;

		iImgCount = 2;
	}
	else if (iJointCount == 3)
	{
		m_MJ3ImgList.DeleteImageList();
		m_MJ3ImgList.Create(IDB_MJ_QUANTITY3, 64, 4, crBkColor );
		tmpImgList = &m_MJ3ImgList;

		iImgCount = 4;
	}
	else if (iJointCount == 4)
	{
		m_MJ4ImgList.DeleteImageList();
		m_MJ4ImgList.Create(IDB_MJ_QUANTITY4, 64, 1, crBkColor );
		tmpImgList = &m_MJ4ImgList;

		iImgCount = 1;

	}
	else if (iJointCount == 6)
	{
		m_MJ6ImgList.DeleteImageList();
		m_MJ6ImgList.Create(IDB_MJ_QUANTITY6, 64, 2, crBkColor );
		tmpImgList = &m_MJ6ImgList;

		iImgCount = 2;
	}

	// display the image in ImgStatic control.
	if (bHole)
	{
		m_holeImg.SetPicImageList(tmpImgList, iImgCount);
		m_holeImg.SetCurrentImage(iImgIdx);
	}
	else
	{
		m_periImg.SetPicImageList(tmpImgList, iImgCount);
		m_periImg.SetCurrentImage(iImgIdx);
	}
}

END_CUTLEADER_NAMESPACE()

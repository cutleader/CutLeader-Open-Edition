#include "StdAfx.h"
#include "NestConfigItemPage.h"

#include "baseConst.h"
#include "Rect2D.h"
#include "clBaseDataResource.h"

#include "NestConfigItem.h"
#include "clExpertLibResource.h"
#include "NestAdvancedParamDlg.h"

#include "Uxtheme.h"
#pragma comment(lib, "Uxtheme.lib")

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(NestConfigItemPage, CResizableDialog)

NestConfigItemPage::NestConfigItemPage(void)
			: CLResizableDialog(NestConfigItemPage::IDD)
{
	m_pDC = NULL;
}

NestConfigItemPage::~NestConfigItemPage(void)
{
}

void NestConfigItemPage::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);

	// “矩形材料边距”分组。
	DDX_Control(pDX, IDC_BTN_BORD_SPACE, m_borderSpace);
	DDX_Control(pDX, IDC_EDIT_LEFT, m_dToLeft);
	DDX_Control(pDX, IDC_EDIT_RIGHT, m_dToRight);
	DDX_Control(pDX, IDC_EDIT_TOP, m_dToTop);
	DDX_Control(pDX, IDC_EDIT_BOTTOM, m_dToBottom);
	
	// “不规则材料边距”分组。
	DDX_Control(pDX, IDC_BTN_POLY_BORD_SPACE, m_polyBorderSpace);
	DDX_Control(pDX, IDC_EDIT_POLYMAT_MARGIN, m_dPolyMargin);

	// for "Start Corner" group.
	DDX_Control(pDX, IDC_BTN_UPPER_LEFT, m_bmpTopLeft);
	DDX_Control(pDX, IDC_BTN_UPPER_RIGHT, m_bmpTopRight);
	DDX_Control(pDX, IDC_BTN_LOWER_LEFT, m_bmpBottomLeft);
	DDX_Control(pDX, IDC_BTN_LOWER_RIGHT, m_bmpBottomRight);

	// for "Nest Direction" group.
	DDX_Control(pDX, IDC_BTN_NEST_DIR_X, m_bmpXDir);
	DDX_Control(pDX, IDC_BTN_NEST_DIR_Y, m_bmpYDir);

	// “其他设置”分组。
	DDX_Control(pDX, IDC_EDIT_PART_DIS, m_dPartDis);
	DDX_Check(pDX, IDC_CHECK_ENABLE_COMCUT, m_bEnableComcut);
	DDX_Check(pDX, IDC_CHECK_NEST_PARTINPART, m_bPartInPart);
	DDX_Check(pDX, IDC_CHECK_EXCLUDE_CAM, m_bExcludeCAM);

	DDX_Control( pDX, IDC_BTN_NEST_ADV, m_btnNestAdv );
}

BEGIN_MESSAGE_MAP(NestConfigItemPage, CResizableDialog)
	ON_WM_CTLCOLOR()

	// for "Nest Border" group.
	ON_EN_KILLFOCUS(IDC_EDIT_LEFT, OnKillLeft)
	ON_EN_KILLFOCUS(IDC_EDIT_RIGHT, OnKillRight)
	ON_EN_KILLFOCUS(IDC_EDIT_TOP, OnKillTop)
	ON_EN_KILLFOCUS(IDC_EDIT_BOTTOM, OnKillBottom)
	ON_EN_KILLFOCUS(IDC_EDIT_POLYMAT_MARGIN, OnKillPolyMatMargin)

	// for "start corner" group
	ON_BN_CLICKED(IDC_BTN_UPPER_LEFT, OnTopLeft)
	ON_BN_CLICKED(IDC_BTN_UPPER_RIGHT, OnTopRight)
	ON_BN_CLICKED(IDC_BTN_LOWER_LEFT, OnBottomLeft)
	ON_BN_CLICKED(IDC_BTN_LOWER_RIGHT, OnBottomRight)

	// for "Nest Direction" group.
	ON_BN_CLICKED(IDC_BTN_NEST_DIR_Y, OnDirY)
	ON_BN_CLICKED(IDC_BTN_NEST_DIR_X, OnDirX)

	// 其他设置。
	ON_EN_KILLFOCUS(IDC_EDIT_PART_DIS, OnKillPartDis)
	ON_BN_CLICKED(IDC_CHECK_ENABLE_COMCUT, OnCheckComcut)
	ON_BN_CLICKED(IDC_CHECK_NEST_PARTINPART, OnCheckPartInPart)
	ON_BN_CLICKED(IDC_CHECK_EXCLUDE_CAM, OnCheckExcludeCAM)
	ON_BN_CLICKED(IDC_BTN_NEST_ADV, OnAdvSetting)

	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

BOOL NestConfigItemPage::OnInitDialog()
{
	CLResizableDialog::OnInitDialog();

	// init the layout of the controls.
	InitLayout();

	// init the button's bitmap.
	InitButtonBMP();

	SetBackgroundColor( SkinManager::GetBackgroundColor() );

	SetWindowTheme( GetDlgItem( IDC_GROUP_BORDER )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_GROUP_POLY_BORDER )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_GROUP_START_CORNER )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_GROUP_NEST_DIR )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_GROUP_OTHER )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_CHECK_ENABLE_COMCUT )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_CHECK_EXCLUDE_CAM )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_CHECK_NEST_PARTINPART )->GetSafeHwnd(), L"", L"" );
	return TRUE;
}

void NestConfigItemPage::InitLayout()
{
	/************************************************************************/
	// “矩形材料边距”分组。

	AddAnchor(IDC_GROUP_BORDER, TOP_LEFT);
	AddAnchor(IDC_BTN_BORD_SPACE, TOP_LEFT);

	//
	AddAnchor(IDC_STATIC_LEFT, TOP_LEFT);
	AddAnchor(IDC_EDIT_LEFT, TOP_LEFT);
	//
	AddAnchor(IDC_STATIC_RIGHT, TOP_LEFT);
	AddAnchor(IDC_EDIT_RIGHT, TOP_LEFT);
	//
	AddAnchor(IDC_STATIC_BOTTOM, TOP_LEFT);
	AddAnchor(IDC_EDIT_BOTTOM, TOP_LEFT);
	//
	AddAnchor(IDC_STATIC_TOP, TOP_LEFT);
	AddAnchor(IDC_EDIT_TOP, TOP_LEFT);
	/************************************************************************/

	/************************************************************************/
	// “不规则材料边距”分组。

	AddAnchor(IDC_GROUP_POLY_BORDER, TOP_LEFT);
	AddAnchor(IDC_BTN_POLY_BORD_SPACE, TOP_LEFT);

	AddAnchor(IDC_STATIC_MARGIN, TOP_LEFT);
	AddAnchor(IDC_EDIT_POLYMAT_MARGIN, TOP_LEFT);
	/************************************************************************/

	// for "start corner" group
	AddAnchor(IDC_GROUP_START_CORNER, TOP_LEFT);
	AddAnchor(IDC_BTN_UPPER_LEFT, TOP_LEFT);
	AddAnchor(IDC_BTN_UPPER_RIGHT, TOP_LEFT);
	AddAnchor(IDC_BTN_LOWER_LEFT, TOP_LEFT);
	AddAnchor(IDC_BTN_LOWER_RIGHT, TOP_LEFT);

	// for "Nest Direction" group.
	AddAnchor(IDC_GROUP_NEST_DIR, TOP_LEFT);
	AddAnchor(IDC_BTN_NEST_DIR_X, TOP_LEFT);
	AddAnchor(IDC_BTN_NEST_DIR_Y, TOP_LEFT);

	// “其他选项”分组。
	AddAnchor(IDC_GROUP_OTHER, TOP_LEFT);
	AddAnchor(IDC_STATIC_SPACE, TOP_LEFT);
	AddAnchor(IDC_EDIT_PART_DIS, TOP_LEFT);
	AddAnchor(IDC_CHECK_ENABLE_COMCUT, TOP_LEFT);
	AddAnchor(IDC_CHECK_NEST_PARTINPART, TOP_LEFT);
	AddAnchor(IDC_CHECK_EXCLUDE_CAM, TOP_LEFT);
	AddAnchor(IDC_BTN_NEST_ADV, TOP_LEFT);
}

void NestConfigItemPage::OnTimer(UINT nIDEvent) 
{
	/************************************************************************/
	// highlight the active corner.

	if (m_pNestConfigItem->GetStartCorner() == LEFT_TOP)
		SelectBitmap(IDC_BTN_UPPER_LEFT);
	else if (m_pNestConfigItem->GetStartCorner() == RIGHT_TOP)
		SelectBitmap(IDC_BTN_UPPER_RIGHT);
	else if (m_pNestConfigItem->GetStartCorner() == LEFT_BOTTOM)
		SelectBitmap(IDC_BTN_LOWER_LEFT);
	else if (m_pNestConfigItem->GetStartCorner() == RIGHT_BOTTOM)
		SelectBitmap(IDC_BTN_LOWER_RIGHT);
	/************************************************************************/

	/************************************************************************/
	// highlight the active nest direction.

	if (m_pNestConfigItem->GetNestDirection() == XY_DIR_X)
		SelectBitmap(IDC_BTN_NEST_DIR_X);
	else
		SelectBitmap(IDC_BTN_NEST_DIR_Y);
	/************************************************************************/

	CResizableDialog::OnTimer(nIDEvent);
}

void NestConfigItemPage::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CResizableDialog::OnShowWindow(bShow, nStatus);

	if (bShow != 0)
		m_nTimer = SetTimer(1, 10, 0);
	else
		KillTimer(m_nTimer);
}

void NestConfigItemPage::DisplayNestConfigItem(NestConfigItemPtr pNestConfigItem)
{
	m_pNestConfigItem = pNestConfigItem;

	// “材料边距”。
	m_dToLeft.Init(m_pNestConfigItem->GetToLeft(), DIGITAL_NUM, FALSE);
	m_dToRight.Init(m_pNestConfigItem->GetToRight(), DIGITAL_NUM, FALSE);
	m_dToTop.Init(m_pNestConfigItem->GetToTop(), DIGITAL_NUM, FALSE);
	m_dToBottom.Init(m_pNestConfigItem->GetToBottom(), DIGITAL_NUM, FALSE);
	m_dPolyMargin.Init(m_pNestConfigItem->GetMatMargin(), DIGITAL_NUM, FALSE);

	// start corner.
	if (m_pNestConfigItem->GetStartCorner() == LEFT_TOP)
	{
		((CButton*)GetDlgItem(IDC_BTN_UPPER_LEFT))->SetCheck(1);
		SelectBitmap(IDC_BTN_UPPER_LEFT);
	}
	else if (m_pNestConfigItem->GetStartCorner() == RIGHT_TOP)
	{
		((CButton*)GetDlgItem(IDC_BTN_UPPER_RIGHT))->SetCheck(1);
		SelectBitmap(IDC_BTN_UPPER_RIGHT);
	}
	else if (m_pNestConfigItem->GetStartCorner() == LEFT_BOTTOM)
	{
		((CButton*)GetDlgItem(IDC_BTN_LOWER_LEFT))->SetCheck(1);
		SelectBitmap(IDC_BTN_LOWER_LEFT);
	}
	else if (m_pNestConfigItem->GetStartCorner() == RIGHT_BOTTOM)
	{
		((CButton*)GetDlgItem(IDC_BTN_LOWER_RIGHT))->SetCheck(1);
		SelectBitmap(IDC_BTN_LOWER_RIGHT);
	}

	// nest direction.
	if (m_pNestConfigItem->GetNestDirection() == XY_DIR_X)
	{
		((CButton*)GetDlgItem(IDC_BTN_NEST_DIR_X))->SetCheck(1);
		SelectBitmap(IDC_BTN_NEST_DIR_X);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_BTN_NEST_DIR_Y))->SetCheck(1);
		SelectBitmap(IDC_BTN_NEST_DIR_Y);
	}

	// “其他设置”。
	m_dPartDis.Init(m_pNestConfigItem->GetPartDis(), DIGITAL_NUM, FALSE);
	m_bEnableComcut = m_pNestConfigItem->IsEnableComcut();
	m_bPartInPart = m_pNestConfigItem->IsPartInPart();
	m_bExcludeCAM = m_pNestConfigItem->IsExcludeCAM();
	UpdateData(FALSE); // 放到最后一行会导致勾选框的显示不对。
	if (m_bEnableComcut)
		((CWnd*)GetDlgItem(IDC_EDIT_PART_DIS))->EnableWindow(FALSE);
	else
		((CWnd*)GetDlgItem(IDC_EDIT_PART_DIS))->EnableWindow(TRUE);
}

NestConfigItemPtr NestConfigItemPage::GetNestConfigItem() 
{
	UpdateData(TRUE);

	return m_pNestConfigItem; 
}

void NestConfigItemPage::InitButtonBMP()
{
	// 材料边距。
	m_borderSpace.LoadBitmaps(IDB_RECTMAT_MARGIN);
	m_polyBorderSpace.LoadBitmaps(IDB_POLYMAT_MARGIN);

	/************************************************************************/
	// set the BMP of "start corner" group.

	m_bmpTopLeft.ModifyStyle(0, BS_OWNERDRAW);
	m_bmpTopLeft.LoadBitmaps(IDB_STARTCORNER_LT);

	m_bmpTopRight.ModifyStyle(0, BS_OWNERDRAW);
	m_bmpTopRight.LoadBitmaps(IDB_STARTCORNER_TR);

	m_bmpBottomLeft.ModifyStyle(0, BS_OWNERDRAW);
	m_bmpBottomLeft.LoadBitmaps(IDB_STARTCORNER_LB);

	m_bmpBottomRight.ModifyStyle(0, BS_OWNERDRAW);
	m_bmpBottomRight.LoadBitmaps(IDB_STARTCORNER_BR);
	/************************************************************************/

	// set the BMP of "nest direction" group.
	m_bmpYDir.ModifyStyle(0, BS_OWNERDRAW);
	m_bmpYDir.LoadBitmaps(IDB_NEST_DIR_Y);
	m_bmpXDir.ModifyStyle(0, BS_OWNERDRAW);
	m_bmpXDir.LoadBitmaps(IDB_NEST_DIR_X);
}

void NestConfigItemPage::SelectBitmap(int iBtnID)
{
	CWnd* pWnd = (CWnd*)GetDlgItem(iBtnID);
	if (!pWnd)
		return;

	RECT rect;
	pWnd->GetWindowRect(&rect);
	ScreenToClient(&rect);

	CBrush redBrush(RGB(255,0,0));
	if (m_pDC == NULL)
		m_pDC = GetDC();
	
	if (m_pDC) 
		m_pDC->FrameRect(&rect, &redBrush);
	
	if (m_pDC != NULL)
	{
		ReleaseDC(m_pDC);
		m_pDC = NULL;
	}
}

void NestConfigItemPage::OnKillLeft()
{
	UpdateData(TRUE);

	if (m_pNestConfigItem)
	{
		if (m_pNestConfigItem->GetToLeft() != m_dToLeft.Get_Number())
		{
			m_pNestConfigItem->SetToLeft(m_dToLeft.Get_Number());
			m_pNestConfigItem->SetModified(TRUE);
		}
	}
}

void NestConfigItemPage::OnKillRight()
{
	UpdateData(TRUE);

	if (m_pNestConfigItem)
	{
		if (m_pNestConfigItem->GetToRight() != m_dToRight.Get_Number())
		{
			m_pNestConfigItem->SetToRight(m_dToRight.Get_Number());
			m_pNestConfigItem->SetModified(TRUE);
		}
	}
}

void NestConfigItemPage::OnKillTop()
{
	UpdateData(TRUE);

	if (m_pNestConfigItem)
	{
		if (m_pNestConfigItem->GetToTop() != m_dToTop.Get_Number())
		{
			m_pNestConfigItem->SetToTop(m_dToTop.Get_Number());
			m_pNestConfigItem->SetModified(TRUE);
		}
	}
}

void NestConfigItemPage::OnKillBottom()
{
	UpdateData(TRUE);

	if (m_pNestConfigItem)
	{
		if (m_pNestConfigItem->GetToBottom() != m_dToBottom.Get_Number())
		{
			m_pNestConfigItem->SetToBottom(m_dToBottom.Get_Number());
			m_pNestConfigItem->SetModified(TRUE);
		}
	}
}

void NestConfigItemPage::OnKillPolyMatMargin()
{
	UpdateData(TRUE);

	if (m_pNestConfigItem)
	{
		if (m_pNestConfigItem->GetMatMargin() != m_dPolyMargin.Get_Number())
		{
			m_pNestConfigItem->SetMatMargin(m_dPolyMargin.Get_Number());
			m_pNestConfigItem->SetModified(TRUE);
		}
	}
}

void NestConfigItemPage::OnTopLeft()
{
	if (m_pNestConfigItem->GetStartCorner() != LEFT_TOP)
	{
		m_pNestConfigItem->SetStartCorner(LEFT_TOP);
		m_pNestConfigItem->SetModified(TRUE);
	}

	Invalidate();
}

void NestConfigItemPage::OnTopRight()
{
	if (m_pNestConfigItem->GetStartCorner() != RIGHT_TOP)
	{
		m_pNestConfigItem->SetStartCorner(RIGHT_TOP);
		m_pNestConfigItem->SetModified(TRUE);
	}

	Invalidate();
}

void NestConfigItemPage::OnBottomLeft()
{
	if (m_pNestConfigItem->GetStartCorner() != LEFT_BOTTOM)
	{
		m_pNestConfigItem->SetStartCorner(LEFT_BOTTOM);
		m_pNestConfigItem->SetModified(TRUE);
	}

	Invalidate();
}

void NestConfigItemPage::OnBottomRight()
{
	if (m_pNestConfigItem->GetStartCorner() != RIGHT_BOTTOM)
	{
		m_pNestConfigItem->SetStartCorner(RIGHT_BOTTOM);
		m_pNestConfigItem->SetModified(TRUE);
	}

	Invalidate();
}

void NestConfigItemPage::OnDirY()
{
	if (m_pNestConfigItem->GetNestDirection() != XY_DIR_Y)
	{
		m_pNestConfigItem->SetNestDirection(XY_DIR_Y);
		m_pNestConfigItem->SetModified(TRUE);
	}

	Invalidate();
}

void NestConfigItemPage::OnDirX()
{
	if (m_pNestConfigItem->GetNestDirection() != XY_DIR_X)
	{
		m_pNestConfigItem->SetNestDirection(XY_DIR_X);
		m_pNestConfigItem->SetModified(TRUE);
	}

	Invalidate();
}

void NestConfigItemPage::OnKillPartDis()
{
	UpdateData(TRUE);

	if (m_pNestConfigItem)
	{
		if (m_pNestConfigItem->GetPartDis() != m_dPartDis.Get_Number())
		{
			m_pNestConfigItem->SetPartDis(m_dPartDis.Get_Number());
			m_pNestConfigItem->SetModified(TRUE);
		}
	}
}

void NestConfigItemPage::OnCheckComcut()
{
	UpdateData(TRUE);

	if (m_bEnableComcut)
		((CWnd*)GetDlgItem(IDC_EDIT_PART_DIS))->EnableWindow(FALSE);
	else
		((CWnd*)GetDlgItem(IDC_EDIT_PART_DIS))->EnableWindow(TRUE);

	m_pNestConfigItem->IsEnableComcut(m_bEnableComcut);
	m_pNestConfigItem->SetModified(TRUE);
}

void NestConfigItemPage::OnCheckPartInPart()
{
	UpdateData(TRUE);

	m_pNestConfigItem->IsPartInPart(m_bPartInPart);
	m_pNestConfigItem->SetModified(TRUE);
}

void NestConfigItemPage::OnCheckExcludeCAM()
{
	UpdateData(TRUE);

	m_pNestConfigItem->IsExcludeCAM(m_bExcludeCAM);
	m_pNestConfigItem->SetModified(TRUE);
}

void NestConfigItemPage::OnAdvSetting()
{
	NestAdvancedParamDlg dlg(m_pNestConfigItem);
	dlg.DoModal();
}

HBRUSH NestConfigItemPage::OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor )
{
	return SkinManager::GetDialogCtrlColor( pDC, pWnd, nCtlColor );
}

END_CUTLEADER_NAMESPACE()

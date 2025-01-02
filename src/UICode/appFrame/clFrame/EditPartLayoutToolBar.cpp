#include "stdafx.h"
#include "EditPartLayoutToolBar.h"

#include "LogMgr.h"
#include "ClWindowFinder.h"

#include "ClGeometryFeatureResource.h"
#include "SheetSubView.h"
#include "clPartLayoutResource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(EditPartLayoutToolBar, SubToolBar)

EditPartLayoutToolBar::EditPartLayoutToolBar() : SubToolBar(EditPartLayoutToolBar::IDD)
{
}

EditPartLayoutToolBar::~EditPartLayoutToolBar()
{
}

void EditPartLayoutToolBar::DoDataExchange(CDataExchange* pDX)
{
	SubToolBar::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BTN_MovePartPlacement, m_btnMovePartPlacement);
	DDX_Control(pDX, IDC_BTN_RotatePartPlacement, m_btnRotatePartPlacement);
	DDX_Control(pDX, IDC_BTN_GridPartPlacement, m_btnGridPartPlacement);
}

BEGIN_MESSAGE_MAP(EditPartLayoutToolBar, SubToolBar)
	ON_BN_CLICKED(IDC_BTN_MovePartPlacement, OnMovePartPlacement)
	ON_BN_CLICKED(IDC_BTN_RotatePartPlacement, OnRotatePartPlacement)
	ON_BN_CLICKED(IDC_BTN_GridPartPlacement, OnGridPartPlacement)
END_MESSAGE_MAP()

BOOL EditPartLayoutToolBar::OnInitDialog()
{
	__super::OnInitDialog();

	m_btnMovePartPlacement.SetImage(IDB_MovePart_32);
	m_btnMovePartPlacement.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_PARTPMT_MOVE_TIP));

	m_btnRotatePartPlacement.SetImage(IDB_RotatePart_32);
	m_btnRotatePartPlacement.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_PARTPMT_ROTATE_TIP));

	m_btnGridPartPlacement.SetImage(IDB_CREATE_PART_GRID_32);
	m_btnGridPartPlacement.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_PARTPMT_CREATE_GRID_TIP));

	SetBackgroundColor(SkinManager::GetBackgroundColor());
	return TRUE;
}

void EditPartLayoutToolBar::OnMovePartPlacement()
{
	((SheetSubView*)ClWindowFinder::GetCurrentView_1())->OnPartPmtMove();
	this->ShowWindow(SW_HIDE);
}

void EditPartLayoutToolBar::OnRotatePartPlacement()
{
	((SheetSubView*)ClWindowFinder::GetCurrentView_1())->OnPartPmtRotate();
	this->ShowWindow(SW_HIDE);
}

void EditPartLayoutToolBar::OnGridPartPlacement()
{
	((SheetSubView*)ClWindowFinder::GetCurrentView_1())->OnPartPmtGrid();
	this->ShowWindow(SW_HIDE);
}

END_CUTLEADER_NAMESPACE()

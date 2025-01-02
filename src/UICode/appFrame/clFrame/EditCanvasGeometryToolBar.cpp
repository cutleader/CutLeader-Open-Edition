#include "stdafx.h"
#include "EditCanvasGeometryToolBar.h"

#include "LogMgr.h"
#include "ClWindowFinder.h"

#include "ClGeometryFeatureResource.h"
#include "SheetSubView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(EditCanvasGeometryToolBar, SubToolBar)

EditCanvasGeometryToolBar::EditCanvasGeometryToolBar() : SubToolBar(EditCanvasGeometryToolBar::IDD)
{
}

EditCanvasGeometryToolBar::~EditCanvasGeometryToolBar()
{
}

void EditCanvasGeometryToolBar::DoDataExchange(CDataExchange* pDX)
{
	SubToolBar::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BTN_MoveCanvasPattern, m_btnMoveCanvasPattern);
	DDX_Control(pDX, IDC_BTN_RotateCanvasPattern, m_btnRotateCanvasPattern);
	DDX_Control(pDX, IDC_BTN_GridCanvasPattern, m_btnGridCanvasPattern);
}

BEGIN_MESSAGE_MAP(EditCanvasGeometryToolBar, SubToolBar)
	ON_BN_CLICKED(IDC_BTN_MoveCanvasPattern, OnMoveCanvasPattern)
	ON_BN_CLICKED(IDC_BTN_RotateCanvasPattern, OnRotateCanvasPattern)
	ON_BN_CLICKED(IDC_BTN_GridCanvasPattern, OnGridCanvasPattern)
END_MESSAGE_MAP()

BOOL EditCanvasGeometryToolBar::OnInitDialog()
{
	__super::OnInitDialog();

	m_btnMoveCanvasPattern.SetImage(IDB_MovePart_32);
	m_btnMoveCanvasPattern.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_MOVE_GEOM_TIP));

	m_btnRotateCanvasPattern.SetImage(IDB_RotatePart_32);
	m_btnRotateCanvasPattern.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_ROTATE_GEOM_TIP));

	m_btnGridCanvasPattern.SetImage(IDB_CREATE_PART_GRID_32);
	m_btnGridCanvasPattern.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_GEOMETRY_PATGRID_TIP));

	SetBackgroundColor(SkinManager::GetBackgroundColor());
	return TRUE;
}

void EditCanvasGeometryToolBar::OnMoveCanvasPattern()
{
	((SheetSubView*)ClWindowFinder::GetCurrentView_1())->OnMoveCanvasPattern();
	this->ShowWindow(SW_HIDE);
}

void EditCanvasGeometryToolBar::OnRotateCanvasPattern()
{
	((SheetSubView*)ClWindowFinder::GetCurrentView_1())->OnRotateCanvasPattern();
	this->ShowWindow(SW_HIDE);
}

void EditCanvasGeometryToolBar::OnGridCanvasPattern()
{
	((SheetSubView*)ClWindowFinder::GetCurrentView_1())->OnGridCanvasPattern();
	this->ShowWindow(SW_HIDE);
}

END_CUTLEADER_NAMESPACE()

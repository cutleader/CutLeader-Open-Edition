#include "stdafx.h"
#include "DrawCanvasGeometryToolBar.h"

#include "LogMgr.h"
#include "ClWindowFinder.h"

#include "ClGeometryFeatureResource.h"
#include "SheetSubView.h"
#include "clGeometryFeatureUIResource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(DrawCanvasGeometryToolBar, SubToolBar)

DrawCanvasGeometryToolBar::DrawCanvasGeometryToolBar() : SubToolBar(DrawCanvasGeometryToolBar::IDD)
{
}

DrawCanvasGeometryToolBar::~DrawCanvasGeometryToolBar()
{
}

void DrawCanvasGeometryToolBar::DoDataExchange(CDataExchange* pDX)
{
	SubToolBar::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BTN_DrawCanvasArcPattern, m_btnDrawCanvasArc);
	DDX_Control(pDX, IDC_BTN_DrawCanvasPolyLine, m_btnDrawCanvasPolyLine);
	DDX_Control(pDX, IDC_BTN_DrawCanvasRectLine, m_btnDrawCanvasRectLine);
	DDX_Control(pDX, IDC_BTN_DrawCanvasCircle, m_ctrlDrawCanvasCircle);
	DDX_Control(pDX, IDC_BTN_DrawCanvasText, m_ctrlDrawCanvasText);
}

BEGIN_MESSAGE_MAP(DrawCanvasGeometryToolBar, SubToolBar)
	ON_BN_CLICKED(IDC_BTN_DrawCanvasArcPattern, OnDrawCanvasArc)
	ON_BN_CLICKED(IDC_BTN_DrawCanvasPolyLine, OnDrawCanvasPolyLine)
	ON_BN_CLICKED(IDC_BTN_DrawCanvasRectLine, OnDrawCanvasRectLine)
	ON_BN_CLICKED(IDC_BTN_DrawCanvasCircle, OnDrawCanvasCircle)
	ON_BN_CLICKED(IDC_BTN_DrawCanvasText, OnDrawCanvasText)
END_MESSAGE_MAP()

BOOL DrawCanvasGeometryToolBar::OnInitDialog()
{
	__super::OnInitDialog();

	m_btnDrawCanvasArc.SetImage(IDB_DRAW_ARC_32);
	m_btnDrawCanvasArc.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_GEOMETRY_ARC_TIP));

	m_btnDrawCanvasPolyLine.SetImage(IDB_DRAW_POLYLINE_32);
	m_btnDrawCanvasPolyLine.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_GEOMETRY_MULTILINE_TIP));

	m_btnDrawCanvasRectLine.SetImage(IDB_DRAW_RECT_32);
	m_btnDrawCanvasRectLine.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_GEOM_DRAW_RectLine_TIP));

	m_ctrlDrawCanvasCircle.SetImage(IDB_DRAW_CIRCLE_32);
	m_ctrlDrawCanvasCircle.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_GEOMETRY_ROUNDHOLE_TIP));

	m_ctrlDrawCanvasText.SetImage(IDB_DrawText_32);
	m_ctrlDrawCanvasText.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_DrawText));

	SetBackgroundColor(SkinManager::GetBackgroundColor());
	return TRUE;
}

void DrawCanvasGeometryToolBar::OnDrawCanvasArc()
{
	((SheetSubView*)ClWindowFinder::GetCurrentView_1())->OnDrawCanvasArc();
	this->ShowWindow(SW_HIDE);
}

void DrawCanvasGeometryToolBar::OnDrawCanvasPolyLine()
{
	((SheetSubView*)ClWindowFinder::GetCurrentView_1())->OnDrawCanvasPolyLine();
	this->ShowWindow(SW_HIDE);
}

void DrawCanvasGeometryToolBar::OnDrawCanvasRectLine()
{
	((SheetSubView*)ClWindowFinder::GetCurrentView_1())->OnDrawCanvasRectLine();
	this->ShowWindow(SW_HIDE);
}

void DrawCanvasGeometryToolBar::OnDrawCanvasCircle()
{
	((SheetSubView*)ClWindowFinder::GetCurrentView_1())->OnDrawCanvasCircle();
	this->ShowWindow(SW_HIDE);
}

void DrawCanvasGeometryToolBar::OnDrawCanvasText()
{
	((SheetSubView*)ClWindowFinder::GetCurrentView_1())->OnDrawCanvasText();
	this->ShowWindow(SW_HIDE);
}

END_CUTLEADER_NAMESPACE()

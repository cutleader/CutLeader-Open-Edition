#include "stdafx.h"
#include "DrawPartGeometryToolBar.h"

#include "LogMgr.h"
#include "ClWindowFinder.h"

#include "ClGeometryFeatureResource.h"
#include "PartView.h"
#include "clGeometryFeatureUIResource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(DrawPartGeometryToolBar, SubToolBar)

DrawPartGeometryToolBar::DrawPartGeometryToolBar() : SubToolBar(DrawPartGeometryToolBar::IDD)
{
}

DrawPartGeometryToolBar::~DrawPartGeometryToolBar()
{
}

void DrawPartGeometryToolBar::DoDataExchange(CDataExchange* pDX)
{
	SubToolBar::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BTN_DrawArcPattern, m_btnDrawArc);
	DDX_Control(pDX, IDC_BTN_DrawPolyLine, m_btnDrawPolyLine);
	DDX_Control(pDX, IDC_BTN_DrawRectLine, m_btnDrawRectLine);
	DDX_Control(pDX, IDC_BTN_DrawCircle, m_ctrlDrawCircle);
	DDX_Control(pDX, IDC_BTN_DrawText, m_ctrlDrawText);
}

BEGIN_MESSAGE_MAP(DrawPartGeometryToolBar, SubToolBar)
	ON_BN_CLICKED(IDC_BTN_DrawArcPattern, OnDrawArc)
	ON_BN_CLICKED(IDC_BTN_DrawPolyLine, OnDrawPolyLine)
	ON_BN_CLICKED(IDC_BTN_DrawRectLine, OnDrawRectLine)
	ON_BN_CLICKED(IDC_BTN_DrawCircle, OnDrawCircle)
	ON_BN_CLICKED(IDC_BTN_DrawText, OnDrawText)
END_MESSAGE_MAP()

BOOL DrawPartGeometryToolBar::OnInitDialog()
{
	__super::OnInitDialog();

	m_btnDrawArc.SetImage(IDB_DRAW_ARC_32);
	m_btnDrawArc.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_GEOMETRY_ARC_TIP));

	m_btnDrawPolyLine.SetImage(IDB_DRAW_POLYLINE_32);
	m_btnDrawPolyLine.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_GEOMETRY_MULTILINE_TIP));

	m_btnDrawRectLine.SetImage(IDB_DRAW_RECT_32);
	m_btnDrawRectLine.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_GEOM_DRAW_RectLine_TIP));

	m_ctrlDrawCircle.SetImage(IDB_DRAW_CIRCLE_32);
	m_ctrlDrawCircle.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_GEOMETRY_ROUNDHOLE_TIP));

	m_ctrlDrawText.SetImage(IDB_DrawText_32);
	m_ctrlDrawText.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_DrawText));

	SetBackgroundColor(SkinManager::GetBackgroundColor());
	return TRUE;
}

void DrawPartGeometryToolBar::OnDrawArc()
{
	((PartView*)ClWindowFinder::GetCurrentView_1())->OnDrawArc3pointsarc();
	this->ShowWindow(SW_HIDE);
}

void DrawPartGeometryToolBar::OnDrawPolyLine()
{
	((PartView*)ClWindowFinder::GetCurrentView_1())->OnDrawMultiplelines();
	this->ShowWindow(SW_HIDE);
}

void DrawPartGeometryToolBar::OnDrawRectLine()
{
	((PartView*)ClWindowFinder::GetCurrentView_1())->OnDrawRectanglelines();
	this->ShowWindow(SW_HIDE);
}

void DrawPartGeometryToolBar::OnDrawCircle()
{
	((PartView*)ClWindowFinder::GetCurrentView_1())->OnDrawRoundHole();
	this->ShowWindow(SW_HIDE);
}

void DrawPartGeometryToolBar::OnDrawText()
{
	((PartView*)ClWindowFinder::GetCurrentView_1())->OnDrawText();
	this->ShowWindow(SW_HIDE);
}

END_CUTLEADER_NAMESPACE()

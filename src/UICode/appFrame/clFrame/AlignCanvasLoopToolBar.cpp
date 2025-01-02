#include "stdafx.h"
#include "AlignCanvasLoopToolBar.h"

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

IMPLEMENT_DYNCREATE(AlignCanvasLoopToolBar, SubToolBar)

AlignCanvasLoopToolBar::AlignCanvasLoopToolBar() : SubToolBar(AlignCanvasLoopToolBar::IDD)
{
}

AlignCanvasLoopToolBar::~AlignCanvasLoopToolBar()
{
}

void AlignCanvasLoopToolBar::DoDataExchange(CDataExchange* pDX)
{
	SubToolBar::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BTN_LeftAlignCanvasLoop, m_btnLeftAlignCanvasLoop);
	DDX_Control(pDX, IDC_BTN_RightAlignCanvasLoop, m_btnRightAlignCanvasLoop);
	DDX_Control(pDX, IDC_BTN_HorizontalCenterAlignCanvasLoop, m_btnHorizontalCenterAlignCanvasLoop);
	DDX_Control(pDX, IDC_BTN_TopAlignCanvasLoop, m_ctrlTopAlignCanvasLoop);
	DDX_Control(pDX, IDC_BTN_BottomAlignCanvasLoop, m_ctrlBottomAlignCanvasLoop);
	DDX_Control(pDX, IDC_BTN_VerticalCenterAlignCanvasLoop, m_ctrlVerticalCenterAlignCanvasLoop);
}

BEGIN_MESSAGE_MAP(AlignCanvasLoopToolBar, SubToolBar)
	ON_BN_CLICKED(IDC_BTN_LeftAlignCanvasLoop, OnLeftAlignCanvasLoop)
	ON_BN_CLICKED(IDC_BTN_RightAlignCanvasLoop, OnRightAlignCanvasLoop)
	ON_BN_CLICKED(IDC_BTN_HorizontalCenterAlignCanvasLoop, OnHorizontalCenterAlignCanvasLoop)
	ON_BN_CLICKED(IDC_BTN_TopAlignCanvasLoop, OnTopAlignCanvasLoop)
	ON_BN_CLICKED(IDC_BTN_BottomAlignCanvasLoop, OnBottomAlignCanvasLoop)
	ON_BN_CLICKED(IDC_BTN_VerticalCenterAlignCanvasLoop, OnVerticalCenterAlignCanvasLoop)
END_MESSAGE_MAP()

BOOL AlignCanvasLoopToolBar::OnInitDialog()
{
	__super::OnInitDialog();

	m_btnLeftAlignCanvasLoop.SetImage(IDB_AlignLoop_Left_32);
	m_btnLeftAlignCanvasLoop.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_AlignLoop_Left));

	m_btnRightAlignCanvasLoop.SetImage(IDB_AlignLoop_Right_32);
	m_btnRightAlignCanvasLoop.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_AlignLoop_Right));

	m_btnHorizontalCenterAlignCanvasLoop.SetImage(IDB_AlignLoop_HorizontalCenter_32);
	m_btnHorizontalCenterAlignCanvasLoop.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_AlignLoop_HorizontalCenter));

	m_ctrlTopAlignCanvasLoop.SetImage(IDB_AlignLoop_Top_32);
	m_ctrlTopAlignCanvasLoop.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_AlignLoop_Top));

	m_ctrlBottomAlignCanvasLoop.SetImage(IDB_AlignLoop_Bottom_32);
	m_ctrlBottomAlignCanvasLoop.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_AlignLoop_Bottom));

	m_ctrlVerticalCenterAlignCanvasLoop.SetImage(IDB_AlignLoop_VerticalCenter_32);
	m_ctrlVerticalCenterAlignCanvasLoop.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_AlignLoop_VerticalCenter));

	SetBackgroundColor(SkinManager::GetBackgroundColor());
	return TRUE;
}

void AlignCanvasLoopToolBar::OnLeftAlignCanvasLoop()
{
	((SheetSubView*)ClWindowFinder::GetCurrentView_1())->OnLeftAlignCanvasLoop();
	this->ShowWindow(SW_HIDE);
}

void AlignCanvasLoopToolBar::OnRightAlignCanvasLoop()
{
	((SheetSubView*)ClWindowFinder::GetCurrentView_1())->OnRightAlignCanvasLoop();
	this->ShowWindow(SW_HIDE);
}

void AlignCanvasLoopToolBar::OnHorizontalCenterAlignCanvasLoop()
{
	((SheetSubView*)ClWindowFinder::GetCurrentView_1())->OnHorizontalCenterAlignCanvasLoop();
	this->ShowWindow(SW_HIDE);
}

void AlignCanvasLoopToolBar::OnTopAlignCanvasLoop()
{
	((SheetSubView*)ClWindowFinder::GetCurrentView_1())->OnTopAlignCanvasLoop();
	this->ShowWindow(SW_HIDE);
}

void AlignCanvasLoopToolBar::OnBottomAlignCanvasLoop()
{
	((SheetSubView*)ClWindowFinder::GetCurrentView_1())->OnBottomAlignCanvasLoop();
	this->ShowWindow(SW_HIDE);
}

void AlignCanvasLoopToolBar::OnVerticalCenterAlignCanvasLoop()
{
	((SheetSubView*)ClWindowFinder::GetCurrentView_1())->OnVerticalCenterAlignCanvasLoop();
	this->ShowWindow(SW_HIDE);
}

END_CUTLEADER_NAMESPACE()

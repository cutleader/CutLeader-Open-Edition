#include "stdafx.h"
#include "PartModeAlignLoopToolBar.h"

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

IMPLEMENT_DYNCREATE(PartModeAlignLoopToolBar, SubToolBar)

PartModeAlignLoopToolBar::PartModeAlignLoopToolBar() : SubToolBar(PartModeAlignLoopToolBar::IDD)
{
}

PartModeAlignLoopToolBar::~PartModeAlignLoopToolBar()
{
}

void PartModeAlignLoopToolBar::DoDataExchange(CDataExchange* pDX)
{
	SubToolBar::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BTN_LeftAlignLoop, m_btnLeftAlignLoop);
	DDX_Control(pDX, IDC_BTN_RightAlignLoop, m_btnRightAlignLoop);
	DDX_Control(pDX, IDC_BTN_HorizontalCenterAlignLoop, m_btnHorizontalCenterAlignLoop);
	DDX_Control(pDX, IDC_BTN_TopAlignLoop, m_ctrlTopAlignLoop);
	DDX_Control(pDX, IDC_BTN_BottomAlignLoop, m_ctrlBottomAlignLoop);
	DDX_Control(pDX, IDC_BTN_VerticalCenterAlignLoop, m_ctrlVerticalCenterAlignLoop);
}

BEGIN_MESSAGE_MAP(PartModeAlignLoopToolBar, SubToolBar)
	ON_BN_CLICKED(IDC_BTN_LeftAlignLoop, OnLeftAlignLoop)
	ON_BN_CLICKED(IDC_BTN_RightAlignLoop, OnRightAlignLoop)
	ON_BN_CLICKED(IDC_BTN_HorizontalCenterAlignLoop, OnHorizontalCenterAlignLoop)
	ON_BN_CLICKED(IDC_BTN_TopAlignLoop, OnTopAlignLoop)
	ON_BN_CLICKED(IDC_BTN_BottomAlignLoop, OnBottomAlignLoop)
	ON_BN_CLICKED(IDC_BTN_VerticalCenterAlignLoop, OnVerticalCenterAlignLoop)
END_MESSAGE_MAP()

BOOL PartModeAlignLoopToolBar::OnInitDialog()
{
	__super::OnInitDialog();

	m_btnLeftAlignLoop.SetImage(IDB_AlignLoop_Left_32);
	m_btnLeftAlignLoop.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_AlignLoop_Left));

	m_btnRightAlignLoop.SetImage(IDB_AlignLoop_Right_32);
	m_btnRightAlignLoop.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_AlignLoop_Right));

	m_btnHorizontalCenterAlignLoop.SetImage(IDB_AlignLoop_HorizontalCenter_32);
	m_btnHorizontalCenterAlignLoop.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_AlignLoop_HorizontalCenter));

	m_ctrlTopAlignLoop.SetImage(IDB_AlignLoop_Top_32);
	m_ctrlTopAlignLoop.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_AlignLoop_Top));

	m_ctrlBottomAlignLoop.SetImage(IDB_AlignLoop_Bottom_32);
	m_ctrlBottomAlignLoop.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_AlignLoop_Bottom));

	m_ctrlVerticalCenterAlignLoop.SetImage(IDB_AlignLoop_VerticalCenter_32);
	m_ctrlVerticalCenterAlignLoop.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_AlignLoop_VerticalCenter));

	SetBackgroundColor(SkinManager::GetBackgroundColor());
	return TRUE;
}

void PartModeAlignLoopToolBar::OnLeftAlignLoop()
{
	((PartView*)ClWindowFinder::GetCurrentView_1())->OnLeftAlignLoop();
	this->ShowWindow(SW_HIDE);
}

void PartModeAlignLoopToolBar::OnRightAlignLoop()
{
	((PartView*)ClWindowFinder::GetCurrentView_1())->OnRightAlignLoop();
	this->ShowWindow(SW_HIDE);
}

void PartModeAlignLoopToolBar::OnHorizontalCenterAlignLoop()
{
	((PartView*)ClWindowFinder::GetCurrentView_1())->OnHorizontalCenterAlignLoop();
	this->ShowWindow(SW_HIDE);
}

void PartModeAlignLoopToolBar::OnTopAlignLoop()
{
	((PartView*)ClWindowFinder::GetCurrentView_1())->OnTopAlignLoop();
	this->ShowWindow(SW_HIDE);
}

void PartModeAlignLoopToolBar::OnBottomAlignLoop()
{
	((PartView*)ClWindowFinder::GetCurrentView_1())->OnBottomAlignLoop();
	this->ShowWindow(SW_HIDE);
}

void PartModeAlignLoopToolBar::OnVerticalCenterAlignLoop()
{
	((PartView*)ClWindowFinder::GetCurrentView_1())->OnVerticalCenterAlignLoop();
	this->ShowWindow(SW_HIDE);
}

END_CUTLEADER_NAMESPACE()

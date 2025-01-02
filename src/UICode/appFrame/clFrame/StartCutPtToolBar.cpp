#include "stdafx.h"
#include "StartCutPtToolBar.h"

#include "LogMgr.h"
#include "ClWindowFinder.h"

#include "clCutFeatureResource.h"
#include "SheetSubView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(StartCutPtToolBar, SubToolBar)

StartCutPtToolBar::StartCutPtToolBar() : SubToolBar(StartCutPtToolBar::IDD)
{
}

StartCutPtToolBar::~StartCutPtToolBar()
{
}

void StartCutPtToolBar::DoDataExchange(CDataExchange* pDX)
{
	SubToolBar::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BTN_ResetStartCutPt, m_btnResetStartCutPt);

	DDX_Control(pDX, IDC_BTN_SwitchCutDirection, m_btnSwitchCutDirection);
	DDX_Control(pDX, IDC_BTN_CcwCutDirection, m_btnCcwCutDirection);
	DDX_Control(pDX, IDC_BTN_CwCutDirection, m_btnCwCutDirection);
}

BEGIN_MESSAGE_MAP(StartCutPtToolBar, SubToolBar)
	ON_BN_CLICKED(IDC_BTN_ResetStartCutPt, OnResetStartCutPt)

	ON_BN_CLICKED(IDC_BTN_SwitchCutDirection, OnSwitchCutDirection)
	ON_BN_CLICKED(IDC_BTN_CcwCutDirection, OnCcwCutDirection)
	ON_BN_CLICKED(IDC_BTN_CwCutDirection, OnCwCutDirection)
END_MESSAGE_MAP()

BOOL StartCutPtToolBar::OnInitDialog()
{
	__super::OnInitDialog();

	m_btnResetStartCutPt.SetImage(IDB_ResetStartCutPt_32);
	m_btnResetStartCutPt.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_ResetStartCutPt));

	m_btnSwitchCutDirection.SetImage(IDB_CUT_DIR_32);
	m_btnSwitchCutDirection.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_SwitchCutDirection));
	m_btnCcwCutDirection.SetImage(IDB_CutDir_CCW_32);
	m_btnCcwCutDirection.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_CcwCutDirection));
	m_btnCwCutDirection.SetImage(IDB_CutDir_CW_32);
	m_btnCwCutDirection.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_CwCutDirection));

	SetBackgroundColor(SkinManager::GetBackgroundColor());
	return TRUE;
}

void StartCutPtToolBar::OnResetStartCutPt()
{
	((SheetSubView*)ClWindowFinder::GetCurrentView_1())->OnReAssignStartCutPoint();
	this->ShowWindow(SW_HIDE);
}

void StartCutPtToolBar::OnSwitchCutDirection()
{
	((SheetSubView*)ClWindowFinder::GetCurrentView_1())->OnSwitchCutDirection();
	this->ShowWindow(SW_HIDE);
}

void StartCutPtToolBar::OnCcwCutDirection()
{
	((SheetSubView*)ClWindowFinder::GetCurrentView_1())->OnCcwCutDirection();
	this->ShowWindow(SW_HIDE);
}

void StartCutPtToolBar::OnCwCutDirection()
{
	((SheetSubView*)ClWindowFinder::GetCurrentView_1())->OnCwCutDirection();
	this->ShowWindow(SW_HIDE);
}

END_CUTLEADER_NAMESPACE()

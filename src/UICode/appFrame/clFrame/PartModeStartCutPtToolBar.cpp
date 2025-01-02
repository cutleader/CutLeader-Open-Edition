#include "stdafx.h"
#include "PartModeStartCutPtToolBar.h"

#include "LogMgr.h"
#include "ClWindowFinder.h"

#include "clCutFeatureResource.h"
#include "PartView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(PartModeStartCutPtToolBar, SubToolBar)

PartModeStartCutPtToolBar::PartModeStartCutPtToolBar() : SubToolBar(PartModeStartCutPtToolBar::IDD)
{
}

PartModeStartCutPtToolBar::~PartModeStartCutPtToolBar()
{
}

void PartModeStartCutPtToolBar::DoDataExchange(CDataExchange* pDX)
{
	SubToolBar::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BTN_SwitchCutDirection, m_btnSwitchCutDirection);
	DDX_Control(pDX, IDC_BTN_CcwCutDirection, m_btnCcwCutDirection);
	DDX_Control(pDX, IDC_BTN_CwCutDirection, m_btnCwCutDirection);
}

BEGIN_MESSAGE_MAP(PartModeStartCutPtToolBar, SubToolBar)
	ON_BN_CLICKED(IDC_BTN_SwitchCutDirection, OnSwitchCutDirection)
	ON_BN_CLICKED(IDC_BTN_CcwCutDirection, OnCcwCutDirection)
	ON_BN_CLICKED(IDC_BTN_CwCutDirection, OnCwCutDirection)
END_MESSAGE_MAP()

BOOL PartModeStartCutPtToolBar::OnInitDialog()
{
	__super::OnInitDialog();

	m_btnSwitchCutDirection.SetImage(IDB_CUT_DIR_32);
	m_btnSwitchCutDirection.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_SwitchCutDirection));
	m_btnCcwCutDirection.SetImage(IDB_CutDir_CCW_32);
	m_btnCcwCutDirection.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_CcwCutDirection));
	m_btnCwCutDirection.SetImage(IDB_CutDir_CW_32);
	m_btnCwCutDirection.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_CwCutDirection));

	SetBackgroundColor(SkinManager::GetBackgroundColor());
	return TRUE;
}

void PartModeStartCutPtToolBar::OnSwitchCutDirection()
{
	((PartView*)ClWindowFinder::GetCurrentView_1())->OnSwitchCutDirection();
	this->ShowWindow(SW_HIDE);
}

void PartModeStartCutPtToolBar::OnCcwCutDirection()
{
	((PartView*)ClWindowFinder::GetCurrentView_1())->OnCcwCutDirection();
	this->ShowWindow(SW_HIDE);
}

void PartModeStartCutPtToolBar::OnCwCutDirection()
{
	((PartView*)ClWindowFinder::GetCurrentView_1())->OnCwCutDirection();
	this->ShowWindow(SW_HIDE);
}

END_CUTLEADER_NAMESPACE()

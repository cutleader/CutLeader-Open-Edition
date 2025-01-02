#include "stdafx.h"
#include "PartModeLeadLineToolBar.h"

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

IMPLEMENT_DYNCREATE(PartModeLeadLineToolBar, SubToolBar)

PartModeLeadLineToolBar::PartModeLeadLineToolBar() : SubToolBar(PartModeLeadLineToolBar::IDD)
{
}

PartModeLeadLineToolBar::~PartModeLeadLineToolBar()
{
}

void PartModeLeadLineToolBar::DoDataExchange(CDataExchange* pDX)
{
	SubToolBar::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BTN_CutOutSide, m_btnCutOutSide);
	DDX_Control(pDX, IDC_BTN_CutInSide, m_btnCutInSide);
	DDX_Control(pDX, IDC_BTN_CutOn, m_btnCutOn);
}

BEGIN_MESSAGE_MAP(PartModeLeadLineToolBar, SubToolBar)
	ON_BN_CLICKED(IDC_BTN_CutOutSide, OnCutOutSide)
	ON_BN_CLICKED(IDC_BTN_CutInSide, OnCutInSide)
	ON_BN_CLICKED(IDC_BTN_CutOn, OnCutOn)
END_MESSAGE_MAP()

BOOL PartModeLeadLineToolBar::OnInitDialog()
{
	__super::OnInitDialog();

	m_btnCutOutSide.SetImage(IDB_CutOutSide_32);
	m_btnCutOutSide.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_SetAsCutOutside));
	m_btnCutInSide.SetImage(IDB_CutInSide_32);
	m_btnCutInSide.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_SetAsCutInside));
	m_btnCutOn.SetImage(IDB_CutOn_32);
	m_btnCutOn.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_SetAsCutOn));

	SetBackgroundColor(SkinManager::GetBackgroundColor());
	return TRUE;
}

void PartModeLeadLineToolBar::OnCutOutSide()
{
	((PartView*)ClWindowFinder::GetCurrentView_1())->OnSetLoopAsCutOutside();
	this->ShowWindow(SW_HIDE);
}

void PartModeLeadLineToolBar::OnCutInSide()
{
	((PartView*)ClWindowFinder::GetCurrentView_1())->OnSetLoopAsCutInside();
	this->ShowWindow(SW_HIDE);
}

void PartModeLeadLineToolBar::OnCutOn()
{
	((PartView*)ClWindowFinder::GetCurrentView_1())->OnSetLoopAsCutOn();
	this->ShowWindow(SW_HIDE);
}

END_CUTLEADER_NAMESPACE()

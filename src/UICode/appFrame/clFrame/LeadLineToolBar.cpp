#include "stdafx.h"
#include "LeadLineToolBar.h"

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

IMPLEMENT_DYNCREATE(LeadLineToolBar, SubToolBar)

LeadLineToolBar::LeadLineToolBar() : SubToolBar(LeadLineToolBar::IDD)
{
}

LeadLineToolBar::~LeadLineToolBar()
{
}

void LeadLineToolBar::DoDataExchange(CDataExchange* pDX)
{
	SubToolBar::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BTN_ResetLeadLine, m_btnResetLeadLine);
	DDX_Control(pDX, IDC_BTN_LoopGap, m_btnLoopGap);

	DDX_Control(pDX, IDC_BTN_CutOutSide, m_btnCutOutSide);
	DDX_Control(pDX, IDC_BTN_CutInSide, m_btnCutInSide);
	DDX_Control(pDX, IDC_BTN_CutOn, m_btnCutOn);
}

BEGIN_MESSAGE_MAP(LeadLineToolBar, SubToolBar)
	ON_BN_CLICKED(IDC_BTN_ResetLeadLine, OnResetLeadLine)
	ON_BN_CLICKED(IDC_BTN_LoopGap, OnLoopGap)

	ON_BN_CLICKED(IDC_BTN_CutOutSide, OnCutOutSide)
	ON_BN_CLICKED(IDC_BTN_CutInSide, OnCutInSide)
	ON_BN_CLICKED(IDC_BTN_CutOn, OnCutOn)
END_MESSAGE_MAP()

BOOL LeadLineToolBar::OnInitDialog()
{
	__super::OnInitDialog();

	m_btnResetLeadLine.SetImage(IDB_ResetLeadLine_32);
	m_btnResetLeadLine.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_ResetLeadLine));

	m_btnLoopGap.SetImage(IDB_LoopGap_32);
	m_btnLoopGap.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_SetLoopGap));

	m_btnCutOutSide.SetImage(IDB_CutOutSide_32);
	m_btnCutOutSide.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_SetAsCutOutside));
	m_btnCutInSide.SetImage(IDB_CutInSide_32);
	m_btnCutInSide.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_SetAsCutInside));
	m_btnCutOn.SetImage(IDB_CutOn_32);
	m_btnCutOn.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_SetAsCutOn));

	SetBackgroundColor(SkinManager::GetBackgroundColor());
	return TRUE;
}

void LeadLineToolBar::OnResetLeadLine()
{
	((SheetSubView*)ClWindowFinder::GetCurrentView_1())->OnReAssignLeadLine();
	this->ShowWindow(SW_HIDE);
}

void LeadLineToolBar::OnLoopGap()
{
	((SheetSubView*)ClWindowFinder::GetCurrentView_1())->ChangeLoopGap();
	this->ShowWindow(SW_HIDE);
}

void LeadLineToolBar::OnCutOutSide()
{
	((SheetSubView*)ClWindowFinder::GetCurrentView_1())->OnSetLoopAsCutOutside();
	this->ShowWindow(SW_HIDE);
}

void LeadLineToolBar::OnCutInSide()
{
	((SheetSubView*)ClWindowFinder::GetCurrentView_1())->OnSetLoopAsCutInside();
	this->ShowWindow(SW_HIDE);
}

void LeadLineToolBar::OnCutOn()
{
	((SheetSubView*)ClWindowFinder::GetCurrentView_1())->OnSetLoopAsCutOn();
	this->ShowWindow(SW_HIDE);
}

END_CUTLEADER_NAMESPACE()

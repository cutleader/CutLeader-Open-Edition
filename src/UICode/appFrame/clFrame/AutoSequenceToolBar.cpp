#include "stdafx.h"
#include "AutoSequenceToolBar.h"

#include "LogMgr.h"
#include "ClWindowFinder.h"

#include "clCutFeatureResource.h"
#include "SheetSubView.h"
#include "clCutSequenceResource.h"
#include "clFrameUIResource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(AutoSequenceToolBar, SubToolBar)

AutoSequenceToolBar::AutoSequenceToolBar() : SubToolBar(AutoSequenceToolBar::IDD)
{
}

AutoSequenceToolBar::~AutoSequenceToolBar()
{
}

void AutoSequenceToolBar::DoDataExchange(CDataExchange* pDX)
{
	SubToolBar::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BTN_ResetSequence, m_btnResetSequence);
	DDX_Control(pDX, IDC_BTN_ClearSequence, m_btnClearSequence);
	DDX_Control(pDX, IDC_BTN_SetReferencePt, m_btnSetReferencePt);
}

BEGIN_MESSAGE_MAP(AutoSequenceToolBar, SubToolBar)
	ON_BN_CLICKED(IDC_BTN_ResetSequence, OnResetSequence)
	ON_BN_CLICKED(IDC_BTN_ClearSequence, OnClearSequence)
	ON_BN_CLICKED(IDC_BTN_SetReferencePt, OnSetReferencePt)
END_MESSAGE_MAP()

BOOL AutoSequenceToolBar::OnInitDialog()
{
	__super::OnInitDialog();

	m_btnResetSequence.SetImage(IDB_ResetSequence_32);
	m_btnResetSequence.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_ResetSequence));

	m_btnClearSequence.SetImage(IDB_ClearAllSequence_32);
	m_btnClearSequence.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_CLEARALLSEQUENCE));

	m_btnSetReferencePt.SetImage(IDB_SetReferencePt_32);
	m_btnSetReferencePt.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_SetReferencePt));

	SetBackgroundColor(SkinManager::GetBackgroundColor());
	return TRUE;
}

void AutoSequenceToolBar::OnResetSequence()
{
	this->ShowWindow(SW_HIDE);
	((SheetSubView*)ClWindowFinder::GetCurrentView_1())->OnResetSequence();
}

void AutoSequenceToolBar::OnClearSequence()
{
	this->ShowWindow(SW_HIDE);
	((SheetSubView*)ClWindowFinder::GetCurrentView_1())->OnClearAllSequence();
}

void AutoSequenceToolBar::OnSetReferencePt()
{
	this->ShowWindow(SW_HIDE);
	((SheetSubView*)ClWindowFinder::GetCurrentView_1())->OnSetReferencePt();
}

END_CUTLEADER_NAMESPACE()

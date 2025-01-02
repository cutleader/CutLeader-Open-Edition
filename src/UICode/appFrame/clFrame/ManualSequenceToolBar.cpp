#include "stdafx.h"
#include "ManualSequenceToolBar.h"

#include "LogMgr.h"
#include "ClWindowFinder.h"

#include "clCutFeatureResource.h"
#include "SheetSubView.h"
#include "clCutSequenceResource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(ManualSequenceToolBar, SubToolBar)

ManualSequenceToolBar::ManualSequenceToolBar() : SubToolBar(ManualSequenceToolBar::IDD)
{
}

ManualSequenceToolBar::~ManualSequenceToolBar()
{
}

void ManualSequenceToolBar::DoDataExchange(CDataExchange* pDX)
{
	SubToolBar::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BTN_AddLoopSequence, m_btnAddLoopSequence);
	DDX_Control(pDX, IDC_BTN_SortSequence, m_btnSortSequence);
	DDX_Control(pDX, IDC_BTN_DeleteSequence, m_btnDeleteSequence);
}

BEGIN_MESSAGE_MAP(ManualSequenceToolBar, SubToolBar)
	ON_BN_CLICKED(IDC_BTN_AddLoopSequence, OnAddLoopSequence)
	ON_BN_CLICKED(IDC_BTN_SortSequence, OnSortSequence)
	ON_BN_CLICKED(IDC_BTN_DeleteSequence, OnDeleteSequence)
END_MESSAGE_MAP()

BOOL ManualSequenceToolBar::OnInitDialog()
{
	__super::OnInitDialog();

	m_btnAddLoopSequence.SetImage(IDB_AddLoopSequence_32);
	m_btnAddLoopSequence.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_ADD_LOOP_SEQ));

	m_btnSortSequence.SetImage(IDB_ORDER_SEQ_32);
	m_btnSortSequence.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_SORTSEQUENCE));

	m_btnDeleteSequence.SetImage(IDB_DELETE_SEQ_32);
	m_btnDeleteSequence.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_REMOVESEQUENCE));

	SetBackgroundColor(SkinManager::GetBackgroundColor());
	return TRUE;
}

void ManualSequenceToolBar::OnAddLoopSequence()
{
	this->ShowWindow(SW_HIDE);
	((SheetSubView*)ClWindowFinder::GetCurrentView_1())->OnAddLoopSeq();
}

void ManualSequenceToolBar::OnSortSequence()
{
	this->ShowWindow(SW_HIDE);
	((SheetSubView*)ClWindowFinder::GetCurrentView_1())->OnSortSequence();
}

void ManualSequenceToolBar::OnDeleteSequence()
{
	this->ShowWindow(SW_HIDE);
	((SheetSubView*)ClWindowFinder::GetCurrentView_1())->OnRemoveSequence();
}

END_CUTLEADER_NAMESPACE()

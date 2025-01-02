#include "stdafx.h"
#include "PartModeCornerFeatureToolBar.h"

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

IMPLEMENT_DYNCREATE(PartModeCornerFeatureToolBar, SubToolBar)

PartModeCornerFeatureToolBar::PartModeCornerFeatureToolBar() : SubToolBar(PartModeCornerFeatureToolBar::IDD)
{
}

PartModeCornerFeatureToolBar::~PartModeCornerFeatureToolBar()
{
}

void PartModeCornerFeatureToolBar::DoDataExchange(CDataExchange* pDX)
{
	SubToolBar::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BTN_EditCornerFeature, m_btnEditCornerFeature);
	DDX_Control(pDX, IDC_BTN_DeleteCornerFeature, m_btnDeleteCornerFeature);
}

BEGIN_MESSAGE_MAP(PartModeCornerFeatureToolBar, SubToolBar)
	ON_BN_CLICKED(IDC_BTN_EditCornerFeature, OnEditCornerFeature)
	ON_BN_CLICKED(IDC_BTN_DeleteCornerFeature, OnDeleteCornerFeature)
END_MESSAGE_MAP()

BOOL PartModeCornerFeatureToolBar::OnInitDialog()
{
	__super::OnInitDialog();

	m_btnEditCornerFeature.SetImage(IDB_EditCornerFeature_32);
	m_btnEditCornerFeature.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_EditCornerFeature));

	m_btnDeleteCornerFeature.SetImage(IDB_DeleteCornerFeature_32);
	m_btnDeleteCornerFeature.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_DeleteCornerFeature));

	SetBackgroundColor(SkinManager::GetBackgroundColor());
	return TRUE;
}

void PartModeCornerFeatureToolBar::OnEditCornerFeature()
{
	((PartView*)ClWindowFinder::GetCurrentView_1())->OnEditCornerFeature();
	this->ShowWindow(SW_HIDE);
}

void PartModeCornerFeatureToolBar::OnDeleteCornerFeature()
{
	((PartView*)ClWindowFinder::GetCurrentView_1())->OnDeleteCornerFeature();
	this->ShowWindow(SW_HIDE);
}

END_CUTLEADER_NAMESPACE()

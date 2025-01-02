#include "stdafx.h"
#include "CornerFeatureToolBar.h"

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

IMPLEMENT_DYNCREATE(CornerFeatureToolBar, SubToolBar)

CornerFeatureToolBar::CornerFeatureToolBar() : SubToolBar(CornerFeatureToolBar::IDD)
{
}

CornerFeatureToolBar::~CornerFeatureToolBar()
{
}

void CornerFeatureToolBar::DoDataExchange(CDataExchange* pDX)
{
	SubToolBar::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BTN_ResetCornerFeature, m_btnResetCornerFeature);
	DDX_Control(pDX, IDC_BTN_EditCornerFeature, m_btnEditCornerFeature);
	DDX_Control(pDX, IDC_BTN_DeleteCornerFeature, m_btnDeleteCornerFeature);
}

BEGIN_MESSAGE_MAP(CornerFeatureToolBar, SubToolBar)
	ON_BN_CLICKED(IDC_BTN_ResetCornerFeature, OnResetCornerFeature)
	ON_BN_CLICKED(IDC_BTN_EditCornerFeature, OnEditCornerFeature)
	ON_BN_CLICKED(IDC_BTN_DeleteCornerFeature, OnDeleteCornerFeature)
END_MESSAGE_MAP()

BOOL CornerFeatureToolBar::OnInitDialog()
{
	__super::OnInitDialog();

	m_btnResetCornerFeature.SetImage(IDB_ResetCornerFeature_32);
	m_btnResetCornerFeature.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_ResetCorner));

	m_btnEditCornerFeature.SetImage(IDB_EditCornerFeature_32);
	m_btnEditCornerFeature.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_EditCornerFeature));

	m_btnDeleteCornerFeature.SetImage(IDB_DeleteCornerFeature_32);
	m_btnDeleteCornerFeature.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_DeleteCornerFeature));

	SetBackgroundColor(SkinManager::GetBackgroundColor());
	return TRUE;
}

void CornerFeatureToolBar::OnResetCornerFeature()
{
	((SheetSubView*)ClWindowFinder::GetCurrentView_1())->OnReAssignCornerFeature();
	this->ShowWindow(SW_HIDE);
}

void CornerFeatureToolBar::OnEditCornerFeature()
{
	((SheetSubView*)ClWindowFinder::GetCurrentView_1())->OnEditCorner();
	this->ShowWindow(SW_HIDE);
}

void CornerFeatureToolBar::OnDeleteCornerFeature()
{
	((SheetSubView*)ClWindowFinder::GetCurrentView_1())->OnDelCorner();
	this->ShowWindow(SW_HIDE);
}

END_CUTLEADER_NAMESPACE()

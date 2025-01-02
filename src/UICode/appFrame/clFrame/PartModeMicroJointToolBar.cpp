#include "stdafx.h"
#include "PartModeMicroJointToolBar.h"

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

IMPLEMENT_DYNCREATE(PartModeMicroJointToolBar, SubToolBar)

PartModeMicroJointToolBar::PartModeMicroJointToolBar() : SubToolBar(PartModeMicroJointToolBar::IDD)
{
}

PartModeMicroJointToolBar::~PartModeMicroJointToolBar()
{
}

void PartModeMicroJointToolBar::DoDataExchange(CDataExchange* pDX)
{
	SubToolBar::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BTN_EditMicroJoint, m_btnEditMicroJoint);
	DDX_Control(pDX, IDC_BTN_DeleteMicroJoint, m_btnDeleteMicroJoint);
}

BEGIN_MESSAGE_MAP(PartModeMicroJointToolBar, SubToolBar)
	ON_BN_CLICKED(IDC_BTN_EditMicroJoint, OnEditMicroJoint)
	ON_BN_CLICKED(IDC_BTN_DeleteMicroJoint, OnDeleteMicroJoint)
END_MESSAGE_MAP()

BOOL PartModeMicroJointToolBar::OnInitDialog()
{
	__super::OnInitDialog();

	m_btnEditMicroJoint.SetImage(IDB_EditMicroJoint_32);
	m_btnEditMicroJoint.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_EditMicroJoint));

	m_btnDeleteMicroJoint.SetImage(IDB_DeleteMicroJoint_32);
	m_btnDeleteMicroJoint.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_DeleteMicroJoint));

	SetBackgroundColor(SkinManager::GetBackgroundColor());
	return TRUE;
}

void PartModeMicroJointToolBar::OnEditMicroJoint()
{
	((PartView*)ClWindowFinder::GetCurrentView_1())->OnEditMicrojoint();
	this->ShowWindow(SW_HIDE);
}

void PartModeMicroJointToolBar::OnDeleteMicroJoint()
{
	((PartView*)ClWindowFinder::GetCurrentView_1())->OnDeleteMicrojoint();
	this->ShowWindow(SW_HIDE);
}

END_CUTLEADER_NAMESPACE()

#include "stdafx.h"
#include "MicroJointToolBar.h"

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

IMPLEMENT_DYNCREATE(MicroJointToolBar, SubToolBar)

MicroJointToolBar::MicroJointToolBar() : SubToolBar(MicroJointToolBar::IDD)
{
}

MicroJointToolBar::~MicroJointToolBar()
{
}

void MicroJointToolBar::DoDataExchange(CDataExchange* pDX)
{
	SubToolBar::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BTN_ResetMicroJoint, m_btnResetMicroJoint);
	DDX_Control(pDX, IDC_BTN_EditMicroJoint, m_btnEditMicroJoint);
	DDX_Control(pDX, IDC_BTN_DeleteMicroJoint, m_btnDeleteMicroJoint);
}

BEGIN_MESSAGE_MAP(MicroJointToolBar, SubToolBar)
	ON_BN_CLICKED(IDC_BTN_ResetMicroJoint, OnResetMicroJoint)
	ON_BN_CLICKED(IDC_BTN_EditMicroJoint, OnEditMicroJoint)
	ON_BN_CLICKED(IDC_BTN_DeleteMicroJoint, OnDeleteMicroJoint)
END_MESSAGE_MAP()

BOOL MicroJointToolBar::OnInitDialog()
{
	__super::OnInitDialog();

	m_btnResetMicroJoint.SetImage(IDB_ResetMicroJoint_32);
	m_btnResetMicroJoint.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_ResetMicroJoint));

	m_btnEditMicroJoint.SetImage(IDB_EditMicroJoint_32);
	m_btnEditMicroJoint.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_EditMicroJoint));

	m_btnDeleteMicroJoint.SetImage(IDB_DeleteMicroJoint_32);
	m_btnDeleteMicroJoint.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_DeleteMicroJoint));

	SetBackgroundColor(SkinManager::GetBackgroundColor());
	return TRUE;
}

void MicroJointToolBar::OnResetMicroJoint()
{
	((SheetSubView*)ClWindowFinder::GetCurrentView_1())->OnReAssignMicroJoint();
	this->ShowWindow(SW_HIDE);
}

void MicroJointToolBar::OnEditMicroJoint()
{
	((SheetSubView*)ClWindowFinder::GetCurrentView_1())->OnEditMicrojoint();
	this->ShowWindow(SW_HIDE);
}

void MicroJointToolBar::OnDeleteMicroJoint()
{
	((SheetSubView*)ClWindowFinder::GetCurrentView_1())->OnDelMicrojoint();
	this->ShowWindow(SW_HIDE);
}

END_CUTLEADER_NAMESPACE()

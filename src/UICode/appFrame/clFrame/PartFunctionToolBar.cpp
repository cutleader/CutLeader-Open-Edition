#include "stdafx.h"
#include "PartFunctionToolBar.h"

#include "ClWindowFinder.h"

#include "clOptionResource.h"
#include "clExpertLibResource.h"
#include "CLOptionSheet.h"

#include "clGeometryFeatureResource.h"
#include "clCutFeatureResource.h"
#include "clGeometryFeatureUIResource.h"
#include "DrawPartGeometryToolBar.h"
#include "PartView.h"
#include "PartModeAlignLoopToolBar.h"
#include "PartModeStartCutPtToolBar.h"
#include "PartModeMicroJointToolBar.h"
#include "PartModeCornerFeatureToolBar.h"
#include "PartModeEditGeometryToolBar.h"
#include "PartModeLeadLineToolBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(PartFunctionToolBar, TopToolBar)

PartFunctionToolBar::PartFunctionToolBar() : TopToolBar(PartFunctionToolBar::IDD)
{
}

PartFunctionToolBar::~PartFunctionToolBar()
{
}

void PartFunctionToolBar::DoDataExchange(CDataExchange* pDX)
{
	TopToolBar::DoDataExchange(pDX);

	// 画图功能。
	DDX_Control(pDX, IDC_BTN_DRAW_LINE, m_ctrlDrawLineBtn);
	DDX_Control(pDX, IDC_BTN_CopyPattern, m_btnCopyPattern);
	DDX_Control(pDX, IDC_BTN_CenterAlignLoop, m_btnCenterAlignLoop);
	DDX_Control(pDX, IDC_BTN_SEL_GEOM, m_ctrlSelGeomBtn);
	DDX_Control(pDX, IDC_BTN_DEL_GEOM, m_ctrlDelGeomBtn);

	// 编辑工艺。
	DDX_Control(pDX, IDC_BTN_RESET_CAM, m_ctrlResetCAMBtn);
	DDX_Control(pDX, IDC_BTN_EDIT_LOOP_LEAD, m_ctrlLoopLeadBtn);
	DDX_Control(pDX, IDC_BTN_CHANGE_START_PT, m_ctrlStartPtBtn);
	DDX_Control(pDX, IDC_BTN_AddMicroJoint, m_ctrlAddMicroJointBtn);
	DDX_Control(pDX, IDC_BTN_AddCornerFeature, m_ctrlAddCornerFeatureBtn);

	// 设置。
	DDX_Control(pDX, IDC_BTN_PARAM_CONFIG, m_ctrlParamBtn);
	DDX_Control(pDX, IDC_BTN_SYSTEM_CONFIG, m_ctrlSysSetBtn);
}

BEGIN_MESSAGE_MAP(PartFunctionToolBar, TopToolBar)
	ON_WM_TIMER()
	ON_WM_DESTROY()

	// 几何相关功能。
	ON_BN_CLICKED(IDC_BTN_DRAW_LINE, OnDrawLine)
	ON_BN_CLICKED(IDC_BTN_CopyPattern, OnCopyPattern)
	ON_BN_CLICKED(IDC_BTN_CenterAlignLoop, OnCenterAlignLoop)
	ON_BN_CLICKED(IDC_BTN_SEL_GEOM, OnSelGeom)
	ON_BN_CLICKED(IDC_BTN_DEL_GEOM, OnDelGeom)

	// 编辑工艺。
	ON_BN_CLICKED(IDC_BTN_RESET_CAM, OnResetCam)
	ON_BN_CLICKED(IDC_BTN_EDIT_LOOP_LEAD, OnLoopLead)
	ON_BN_CLICKED(IDC_BTN_CHANGE_START_PT, OnStartPt)
	ON_BN_CLICKED(IDC_BTN_AddMicroJoint, OnAddMicroJoint)
	ON_BN_CLICKED(IDC_BTN_AddCornerFeature, OnAddCornerFeature)

	// 设置。
	ON_BN_CLICKED(IDC_BTN_PARAM_CONFIG, OnParamConfig)
	ON_BN_CLICKED(IDC_BTN_SYSTEM_CONFIG, OnSystemConfig)
END_MESSAGE_MAP()

BOOL PartFunctionToolBar::OnInitDialog()
{
	__super::OnInitDialog();

	// 几何相关功能。
	{
		m_ctrlDrawLineBtn.SetImage(IDB_DRAW_LINE_32);
		m_ctrlDrawLineBtn.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_GEOMETRY_LINE_TIP));

		m_btnCopyPattern.SetImage(IDB_CopyPart_32);
		m_btnCopyPattern.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_COPY_GEOM_TIP));

		m_btnCenterAlignLoop.SetImage(IDB_AlignLoop_Center_32);
		m_btnCenterAlignLoop.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_AlignLoop_Center));

		m_ctrlSelGeomBtn.SetImage(IDB_SEL_GEOM_32);
		m_ctrlSelGeomBtn.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_SELECT_GEOM));

		m_ctrlDelGeomBtn.SetImage(IDB_DEL_GEOM_32);
		m_ctrlDelGeomBtn.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_DELETE_GEOM_TIP));
	}

	// 编辑工艺。
	{
		m_ctrlResetCAMBtn.SetImage(IDB_RESET_CAM_32);
		m_ctrlResetCAMBtn.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_RESET_CUTFEATURE));
		InitSenfengMFCButtonForStyle(&m_ctrlResetCAMBtn);

		m_ctrlLoopLeadBtn.SetImage(IDB_LOOP_LEAD_32);
		m_ctrlLoopLeadBtn.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_EDIT_LOOP_LEAD));
		InitSenfengMFCButtonForStyle(&m_ctrlLoopLeadBtn);

		m_ctrlStartPtBtn.SetImage(IDB_LOOP_START_PT_32);
		m_ctrlStartPtBtn.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_EDIT_LOOP_START_PT));
		InitSenfengMFCButtonForStyle(&m_ctrlStartPtBtn);

		m_ctrlAddMicroJointBtn.SetImage(IDB_AddMicroJoint_32);
		m_ctrlAddMicroJointBtn.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_AddMicroJoint));

		m_ctrlAddCornerFeatureBtn.SetImage(IDB_AddCornerFeature_32);
		m_ctrlAddCornerFeatureBtn.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_AddCornerFeature));
	}

	// 设置。
	{
		m_ctrlParamBtn.SetImage(IDB_PARAM_CONFIG_32);
		m_ctrlParamBtn.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_CAM_PARAM));
		InitSenfengMFCButtonForStyle(&m_ctrlParamBtn);

		m_ctrlSysSetBtn.SetImage(IDB_SYS_CONFIG_32);
		m_ctrlSysSetBtn.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_OPTIONS));
		InitSenfengMFCButtonForStyle(&m_ctrlSysSetBtn);
	}

	// 创建子对话框。
	{
		m_pDrawPartGeometryToolBar = new DrawPartGeometryToolBar();
		m_pDrawPartGeometryToolBar->Create(DrawPartGeometryToolBar::IDD);
		m_pDrawPartGeometryToolBar->ShowWindow(SW_HIDE);

		m_pPartModeEditGeometryToolBar = new PartModeEditGeometryToolBar();
		m_pPartModeEditGeometryToolBar->Create(PartModeEditGeometryToolBar::IDD);
		m_pPartModeEditGeometryToolBar->ShowWindow(SW_HIDE);

		m_pPartModeAlignLoopToolBar = new PartModeAlignLoopToolBar();
		m_pPartModeAlignLoopToolBar->Create(PartModeAlignLoopToolBar::IDD);
		m_pPartModeAlignLoopToolBar->ShowWindow(SW_HIDE);

		m_pPartModeLeadLineToolBar = new PartModeLeadLineToolBar();
		m_pPartModeLeadLineToolBar->Create(PartModeLeadLineToolBar::IDD);
		m_pPartModeLeadLineToolBar->ShowWindow(SW_HIDE);

		m_pPartModeStartCutPtToolBar = new PartModeStartCutPtToolBar();
		m_pPartModeStartCutPtToolBar->Create(PartModeStartCutPtToolBar::IDD);
		m_pPartModeStartCutPtToolBar->ShowWindow(SW_HIDE);

		m_pPartModeMicroJointToolBar = new PartModeMicroJointToolBar();
		m_pPartModeMicroJointToolBar->Create(PartModeMicroJointToolBar::IDD);
		m_pPartModeMicroJointToolBar->ShowWindow(SW_HIDE);

		m_pPartModeCornerFeatureToolBar = new PartModeCornerFeatureToolBar();
		m_pPartModeCornerFeatureToolBar->Create(PartModeCornerFeatureToolBar::IDD);
		m_pPartModeCornerFeatureToolBar->ShowWindow(SW_HIDE);
	}

	// about scroll bar.
	GetWindowRect(m_rcOriginalRect); // get the original size
	m_nScrollPos = 0; // initial scroll position

	m_nTimer = SetTimer(73, 300, 0);

	SetBackgroundColor( SkinManager::GetBackgroundColor() );
	return TRUE;
}

void PartFunctionToolBar::InitSenfengMFCButtonForStyle( SkinMFCButton *CSenfengMFCButton )
{
	//CSenfengMFCButton->m_bTransparent = TRUE;
	//CSenfengMFCButton->SizeToContent( TRUE );
	//CSenfengMFCButton->m_nFlatStyle = CMFCButton::BUTTONSTYLE_FLAT;
	//CSenfengMFCButton->SetMouseCursorHand();
}

void PartFunctionToolBar::OnTimer(UINT nIDEvent)
{
	// 该对话框不可见时，禁用定时器。
	if (!this->IsWindowVisible()) {
		return;
	}

	CheckWhetherDisplaySubToolBar(&m_ctrlDrawLineBtn, m_pDrawPartGeometryToolBar);
	CheckWhetherDisplaySubToolBar(&m_btnCopyPattern, m_pPartModeEditGeometryToolBar);
	CheckWhetherDisplaySubToolBar(&m_btnCenterAlignLoop, m_pPartModeAlignLoopToolBar);

	CheckWhetherDisplaySubToolBar(&m_ctrlLoopLeadBtn, m_pPartModeLeadLineToolBar);
	CheckWhetherDisplaySubToolBar(&m_ctrlStartPtBtn, m_pPartModeStartCutPtToolBar);
	CheckWhetherDisplaySubToolBar(&m_ctrlAddMicroJointBtn, m_pPartModeMicroJointToolBar);
	CheckWhetherDisplaySubToolBar(&m_ctrlAddCornerFeatureBtn, m_pPartModeCornerFeatureToolBar);

	TopToolBar::OnTimer(nIDEvent);
}

void PartFunctionToolBar::OnDestroy()
{
	KillTimer(m_nTimer);
	TopToolBar::OnDestroy();
}

void PartFunctionToolBar::OnDrawLine()
{
	((PartView*)ClWindowFinder::GetCurrentView_1())->OnDrawStandardline();
}

void PartFunctionToolBar::OnSelGeom()
{
	((PartView*)ClWindowFinder::GetCurrentView_1())->OnSelectPattern();
}

void PartFunctionToolBar::OnCopyPattern()
{
	((PartView*)ClWindowFinder::GetCurrentView_1())->OnCopyPattern();
}

void PartFunctionToolBar::OnCenterAlignLoop()
{
	((PartView*)ClWindowFinder::GetCurrentView_1())->OnCenterAlignLoop();
}

void PartFunctionToolBar::OnDelGeom()
{
	((PartView*)ClWindowFinder::GetCurrentView_1())->OnDeletePattern();
}

void PartFunctionToolBar::OnResetCam()
{
	((PartView*)ClWindowFinder::GetCurrentView_1())->OnResetCutFeature();
}

void PartFunctionToolBar::OnLoopLead()
{
	((PartView*)ClWindowFinder::GetCurrentView_1())->OnEditLoopLead();
}

void PartFunctionToolBar::OnStartPt()
{
	((PartView*)ClWindowFinder::GetCurrentView_1())->OnEditLoopStartPt();
}

void PartFunctionToolBar::OnAddMicroJoint()
{
	((PartView*)ClWindowFinder::GetCurrentView_1())->OnAddMicrojoint();
}

void PartFunctionToolBar::OnAddCornerFeature()
{
	((PartView*)ClWindowFinder::GetCurrentView_1())->OnAddCornerFeature();
}

void PartFunctionToolBar::OnParamConfig()
{
	((PartView*)ClWindowFinder::GetCurrentView_1())->SetParamConfig();
}

void PartFunctionToolBar::OnSystemConfig()
{
	CLOptionSheet clOptionSheet;
	clOptionSheet.DoModal();
}

END_CUTLEADER_NAMESPACE()

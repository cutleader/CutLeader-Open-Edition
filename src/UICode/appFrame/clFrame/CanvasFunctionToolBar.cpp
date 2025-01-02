#include "stdafx.h"
#include "CanvasFunctionToolBar.h"

#include "LogMgr.h"
#include "baseConst.h"
#include "ClBaseDataResource.h"
#include "ClFolderHelper.h"
#include "ClWindowFinder.h"

#include "ClExpertLibResource.h"
#include "ProductParam.h"

#include "ClGeometryFeatureResource.h"
#include "ClCutFeatureResource.h"
#include "ClPartLayoutResource.h"
#include "ClCutSequenceResource.h"
#include "ClPostProcessorResource.h"
#include "TaskView.h"
#include "DrawCanvasGeometryToolBar.h"
#include "EditCanvasGeometryToolBar.h"
#include "clCanvasGeometryUIResource.h"
#include "AlignCanvasLoopToolBar.h"
#include "StartCutPtToolBar.h"
#include "LeadLineToolBar.h"
#include "MicroJointToolBar.h"
#include "CornerFeatureToolBar.h"
#include "ConfigToolBar.h"
#include "AutoSequenceToolBar.h"
#include "ManualSequenceToolBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(CanvasFunctionToolBar, TopToolBar)

CanvasFunctionToolBar::CanvasFunctionToolBar() : TopToolBar(CanvasFunctionToolBar::IDD)
{
}

CanvasFunctionToolBar::~CanvasFunctionToolBar()
{
}

void CanvasFunctionToolBar::DoDataExchange(CDataExchange* pDX)
{
	TopToolBar::DoDataExchange(pDX);

	// 几何特征功能。
	DDX_Control(pDX, IDC_BTN_AddPatternFromDxf, m_btnAddPatternFromDxf);
	DDX_Control(pDX, IDC_BTN_DrawCanvasLine, m_btnDrawCanvasLine);
	DDX_Control(pDX, IDC_BTN_CopyCanvasPattern, m_btnCopyCanvasPattern);
	DDX_Control(pDX, IDC_BTN_CenterAlignCanvasLoop, m_btnCenterAlignCanvasLoop);
	DDX_Control(pDX, IDC_BTN_AddCanvasPattern2Task, m_btnAddCanvasPattern2Task);

	// 编辑工艺。
	DDX_Control(pDX, IDC_BTN_RESET_CAM, m_ctrlResetCAMBtn);
	DDX_Control(pDX, IDC_BTN_EDIT_LOOP_LEAD, m_ctrlLoopLeadBtn);
	DDX_Control(pDX, IDC_BTN_CHANGE_START_PT, m_ctrlStartPtBtn);
	DDX_Control(pDX, IDC_BTN_AddMicroJoint, m_ctrlAddMicroJointBtn);
	DDX_Control(pDX, IDC_BTN_AddCornerFeature, m_ctrlAddCornerFeatureBtn);

	// 编辑工序。
	DDX_Control(pDX, IDC_BTN_AutoSequence, m_btnAutoSeq);
	DDX_Control(pDX, IDC_BTN_AddFlyCutSequence, m_btnAddFlyCutSequence);
	DDX_Control(pDX, IDC_BTN_SIMULATE_SEQ, m_btnSimulateSeq);
	DDX_Control(pDX, IDC_BTN_GENERATE_NC, m_btnGenerateNC);

	// 设置。
	DDX_Control(pDX, IDC_BTN_PARAM_CONFIG, m_ctrlParamBtn);
}

BEGIN_MESSAGE_MAP(CanvasFunctionToolBar, TopToolBar)
	ON_WM_TIMER()
	ON_WM_DESTROY()

	// 几何特征功能。
	ON_BN_CLICKED(IDC_BTN_AddPatternFromDxf, OnAddPatternFromDxf)
	ON_BN_CLICKED(IDC_BTN_DrawCanvasLine, OnDrawCanvasLine)
	ON_BN_CLICKED(IDC_BTN_CopyCanvasPattern, OnCopyCanvasPattern)
	ON_BN_CLICKED(IDC_BTN_CenterAlignCanvasLoop, OnCenterAlignCanvasLoop)
	ON_BN_CLICKED(IDC_BTN_AddCanvasPattern2Task, OnAddCanvasPattern2Task)

	// 编辑工艺。
	ON_BN_CLICKED(IDC_BTN_RESET_CAM, OnResetCam)
	ON_BN_CLICKED(IDC_BTN_EDIT_LOOP_LEAD, OnLoopLead)
	ON_BN_CLICKED(IDC_BTN_CHANGE_START_PT, OnStartPt)
	ON_BN_CLICKED(IDC_BTN_AddMicroJoint, OnAddMicroJoint)
	ON_BN_CLICKED(IDC_BTN_AddCornerFeature, OnAddCornerFeature)

	// 编辑工序。
	ON_BN_CLICKED(IDC_BTN_AutoSequence, OnAutoSeq)
	ON_BN_CLICKED(IDC_BTN_AddFlyCutSequence, OnAddFlyCutSequence)
	ON_BN_CLICKED(IDC_BTN_SIMULATE_SEQ, OnSimulateSeq)
	ON_BN_CLICKED(IDC_BTN_GENERATE_NC, OnGenerateNC)

	// 设置。
	ON_BN_CLICKED(IDC_BTN_PARAM_CONFIG, OnParamConfig)
END_MESSAGE_MAP()

BOOL CanvasFunctionToolBar::OnInitDialog()
{
	__super::OnInitDialog();

	// 几何特征功能。
	{
		m_btnAddPatternFromDxf.SetImage(IDB_ADD_PART_FROM_DXF_32);
		m_btnAddPatternFromDxf.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_AddPatternFromDxf));

		m_btnDrawCanvasLine.SetImage(IDB_DRAW_LINE_32);
		m_btnDrawCanvasLine.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_GEOMETRY_LINE_TIP));

		m_btnCopyCanvasPattern.SetImage(IDB_CopyPart_32);
		m_btnCopyCanvasPattern.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_COPY_GEOM_TIP));

		m_btnCenterAlignCanvasLoop.SetImage(IDB_AlignLoop_Center_32);
		m_btnCenterAlignCanvasLoop.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_AlignLoop_Center));

		m_btnAddCanvasPattern2Task.SetImage(IDB_AddCanvasGeometryAsPart_32);
		m_btnAddCanvasPattern2Task.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_AddCanvasPattern2Task));
	}

	// 编辑工艺。
	{
		m_ctrlResetCAMBtn.SetImage(IDB_RESET_CAM_32);
		m_ctrlResetCAMBtn.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_RESET_CUTFEATURE));

		m_ctrlLoopLeadBtn.SetImage(IDB_LOOP_LEAD_32);
		m_ctrlLoopLeadBtn.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_EDIT_LOOP_LEAD));

		m_ctrlStartPtBtn.SetImage(IDB_LOOP_START_PT_32);
		m_ctrlStartPtBtn.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_EDIT_LOOP_START_PT));

		m_ctrlAddMicroJointBtn.SetImage(IDB_AddMicroJoint_32);
		m_ctrlAddMicroJointBtn.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_AddMicroJoint));

		m_ctrlAddCornerFeatureBtn.SetImage(IDB_AddCornerFeature_32);
		m_ctrlAddCornerFeatureBtn.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_AddCornerFeature));
	}

	// 编辑工序。
	{
		m_btnAutoSeq.SetImage(IDB_ASSIGN_SEQ_32);
		m_btnAutoSeq.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_AUTOSEQUENCE));

		m_btnAddFlyCutSequence.SetImage(IDB_AddFlyCutSequence_32);
		m_btnAddFlyCutSequence.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_AddFlyCut));

		m_btnSimulateSeq.SetImage(IDB_SIMULATE_SEQ_32);
		m_btnSimulateSeq.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_VIEWSEQUENCE));

		m_btnGenerateNC.SetImage(IDB_GENERATE_NC_32);
		m_btnGenerateNC.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_MACROUTER_VIEW_NC));
	}

	// 设置。
	{
		m_ctrlParamBtn.SetImage(IDB_PARAM_CONFIG_32);
		m_ctrlParamBtn.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_CAM_PARAM));
	}

	// 创建子对话框。
	{
		m_pDrawCanvasGeometryToolBar = new DrawCanvasGeometryToolBar();
		m_pDrawCanvasGeometryToolBar->Create(DrawCanvasGeometryToolBar::IDD);
		m_pDrawCanvasGeometryToolBar->ShowWindow(SW_HIDE);

		m_pEditCanvasGeometryToolBar = new EditCanvasGeometryToolBar();
		m_pEditCanvasGeometryToolBar->Create(EditCanvasGeometryToolBar::IDD);
		m_pEditCanvasGeometryToolBar->ShowWindow(SW_HIDE);

		m_pAlignCanvasLoopToolBar = new AlignCanvasLoopToolBar();
		m_pAlignCanvasLoopToolBar->Create(AlignCanvasLoopToolBar::IDD);
		m_pAlignCanvasLoopToolBar->ShowWindow(SW_HIDE);

		m_pStartCutPtToolBar = new StartCutPtToolBar();
		m_pStartCutPtToolBar->Create(StartCutPtToolBar::IDD);
		m_pStartCutPtToolBar->ShowWindow(SW_HIDE);

		m_pLeadLineToolBar = new LeadLineToolBar();
		m_pLeadLineToolBar->Create(LeadLineToolBar::IDD);
		m_pLeadLineToolBar->ShowWindow(SW_HIDE);

		m_pMicroJointToolBar = new MicroJointToolBar();
		m_pMicroJointToolBar->Create(MicroJointToolBar::IDD);
		m_pMicroJointToolBar->ShowWindow(SW_HIDE);

		m_pCornerFeatureToolBar = new CornerFeatureToolBar();
		m_pCornerFeatureToolBar->Create(CornerFeatureToolBar::IDD);
		m_pCornerFeatureToolBar->ShowWindow(SW_HIDE);

		m_pAutoSequenceToolBar = new AutoSequenceToolBar();
		m_pAutoSequenceToolBar->Create(AutoSequenceToolBar::IDD);
		m_pAutoSequenceToolBar->ShowWindow(SW_HIDE);

		m_pManualSequenceToolBar = new ManualSequenceToolBar();
		m_pManualSequenceToolBar->Create(ManualSequenceToolBar::IDD);
		m_pManualSequenceToolBar->ShowWindow(SW_HIDE);

		m_pConfigToolBar = new ConfigToolBar();
		m_pConfigToolBar->Create(ConfigToolBar::IDD);
		m_pConfigToolBar->ShowWindow(SW_HIDE);
	}

	// about scroll bar.
	GetWindowRect(m_rcOriginalRect); // get the original size
	m_nScrollPos = 0; // initial scroll position

	m_nTimer = SetTimer(93, 300, 0);

	SetBackgroundColor(SkinManager::GetBackgroundColor());
	return TRUE;
}

void CanvasFunctionToolBar::OnDestroy()
{
	KillTimer(m_nTimer);
	TopToolBar::OnDestroy();
}

void CanvasFunctionToolBar::OnTimer(UINT nIDEvent)
{
	// 该对话框不可见时，禁用定时器。
	if (!this->IsWindowVisible()) {
		return;
	}

	CheckWhetherDisplaySubToolBar(&m_btnDrawCanvasLine, m_pDrawCanvasGeometryToolBar); // 看看要不要显示绘图工具条。
	CheckWhetherDisplaySubToolBar(&m_btnCopyCanvasPattern, m_pEditCanvasGeometryToolBar); // 看看要不要显示编辑工具条。
	CheckWhetherDisplaySubToolBar(&m_btnCenterAlignCanvasLoop, m_pAlignCanvasLoopToolBar); // 看看要不要显示对齐工具条。
	CheckWhetherDisplaySubToolBar(&m_ctrlLoopLeadBtn, m_pLeadLineToolBar); // 看看要不要显示工具条。
	CheckWhetherDisplaySubToolBar(&m_ctrlStartPtBtn, m_pStartCutPtToolBar); // 看看要不要显示起点编辑工具条。
	CheckWhetherDisplaySubToolBar(&m_ctrlAddMicroJointBtn, m_pMicroJointToolBar); // 看看要不要显示微连工具条。
	CheckWhetherDisplaySubToolBar(&m_ctrlAddCornerFeatureBtn, m_pCornerFeatureToolBar); // 看看要不要显示角特征工具条。
	CheckWhetherDisplaySubToolBar(&m_btnAutoSeq, m_pAutoSequenceToolBar);
	CheckWhetherDisplaySubToolBar(&m_btnAddFlyCutSequence, m_pManualSequenceToolBar);
	CheckWhetherDisplaySubToolBar(&m_ctrlParamBtn, m_pConfigToolBar);

	TopToolBar::OnTimer(nIDEvent);
}

void CanvasFunctionToolBar::OnAddPatternFromDxf()
{
	((SheetSubView*)ClWindowFinder::GetCurrentView_1())->OnAddPatternFromDxf();
}

void CanvasFunctionToolBar::OnDrawCanvasLine()
{
	((SheetSubView*)ClWindowFinder::GetCurrentView_1())->OnDrawCanvasLine();
}

void CanvasFunctionToolBar::OnCopyCanvasPattern()
{
	((SheetSubView*)ClWindowFinder::GetCurrentView_1())->OnCopyCanvasPattern();
}

void CanvasFunctionToolBar::OnCenterAlignCanvasLoop()
{
	((SheetSubView*)ClWindowFinder::GetCurrentView_1())->OnCenterAlignCanvasLoop();
}

void CanvasFunctionToolBar::OnAddCanvasPattern2Task()
{
	((SheetSubView*)ClWindowFinder::GetCurrentView_1())->OnAddCanvasPattern2Task();
}

void CanvasFunctionToolBar::OnResetCam()
{
	((SheetSubView*)ClWindowFinder::GetCurrentView_1())->OnResetCutFeature();
}

void CanvasFunctionToolBar::OnLoopLead()
{
	((SheetSubView*)ClWindowFinder::GetCurrentView_1())->OnEditLoopLead();
}

void CanvasFunctionToolBar::OnStartPt()
{
	((SheetSubView*)ClWindowFinder::GetCurrentView_1())->OnEditLoopStartPt();
}

void CanvasFunctionToolBar::OnAddMicroJoint()
{
	((SheetSubView*)ClWindowFinder::GetCurrentView_1())->OnAddMicrojoint();
}

void CanvasFunctionToolBar::OnAddCornerFeature()
{
	((SheetSubView*)ClWindowFinder::GetCurrentView_1())->OnAddCorner();
}

void CanvasFunctionToolBar::OnAutoSeq()
{
	((SheetSubView*)ClWindowFinder::GetCurrentView_1())->OnAutoSequence();
}

void CanvasFunctionToolBar::OnAddFlyCutSequence()
{
	((SheetSubView*)ClWindowFinder::GetCurrentView_1())->OnAddFlyCutSeq();
}

void CanvasFunctionToolBar::OnSimulateSeq()
{
	((SheetSubView*)ClWindowFinder::GetCurrentView_1())->OnViewSequence();
}

void CanvasFunctionToolBar::OnGenerateNC()
{
	((SheetSubView*)ClWindowFinder::GetCurrentView_1())->OnViewNC();
}

void CanvasFunctionToolBar::OnParamConfig()
{
	((SheetSubView*)ClWindowFinder::GetCurrentView_1())->SetParamConfig();
}

END_CUTLEADER_NAMESPACE()

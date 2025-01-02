#include "stdafx.h"
#include "SheetFunctionToolBar.h"

#include "LogMgr.h"
#include "baseConst.h"
#include "ClBaseDataResource.h"
#include "ClFolderHelper.h"
#include "ClWindowFinder.h"

#include "CLOptionSheet.h"
#include "ClOptionResource.h"
#include "ClExpertLibResource.h"
#include "ProductParam.h"

#include "ClGeometryFeatureResource.h"
#include "ClCutFeatureResource.h"
#include "ClPartLayoutResource.h"
#include "ClCutSequenceResource.h"
#include "ClPostProcessorResource.h"
#include "SheetSubView.h"
#include "EditPartLayoutToolBar.h"
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

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(SheetFunctionToolBar, TopToolBar)

SheetFunctionToolBar::SheetFunctionToolBar() : TopToolBar(SheetFunctionToolBar::IDD)
{
}

SheetFunctionToolBar::~SheetFunctionToolBar()
{
}

void SheetFunctionToolBar::DoDataExchange(CDataExchange* pDX)
{
	TopToolBar::DoDataExchange(pDX);

	// ������ֹ��ܡ�
	DDX_Control(pDX, IDC_BTN_PART_FROM_DB, m_btnPartFromDB);
	DDX_Control(pDX, IDC_BTN_PART_FROM_DXF, m_btnPartFromDxf);
	DDX_Control(pDX, IDC_BTN_NestTaskParts, m_btnNestTaskParts);
	DDX_Control(pDX, IDC_BTN_MEASURE_DISTANCE, m_btnMeasureDis);
	DDX_Control(pDX, IDC_BTN_CopyPartPlacement, m_ctrlCopyPartlacementButton);

	// �༭���ա�
	DDX_Control(pDX, IDC_BTN_RESET_CAM, m_ctrlResetCAMBtn);
	DDX_Control(pDX, IDC_BTN_EDIT_LOOP_LEAD, m_ctrlLoopLeadBtn);
	DDX_Control(pDX, IDC_BTN_CHANGE_START_PT, m_ctrlStartPtBtn);
	DDX_Control(pDX, IDC_BTN_AddMicroJoint_2, m_ctrlAddMicroJointBtn);
	DDX_Control(pDX, IDC_BTN_AddCornerFeature_2, m_ctrlAddCornerFeatureBtn);

	// �༭����
	DDX_Control(pDX, IDC_BTN_AutoSequence, m_btnAutoSeq);
	DDX_Control(pDX, IDC_BTN_AddFlyCutSequence, m_btnAddFlyCutSequence);
	DDX_Control(pDX, IDC_BTN_SIMULATE_SEQ, m_btnSimulateSeq);
	DDX_Control(pDX, IDC_BTN_GENERATE_NC, m_btnGenerateNC);

	// ���á�
	DDX_Control(pDX, IDC_BTN_PARAM_CONFIG, m_ctrlParamBtn);
}

BEGIN_MESSAGE_MAP(SheetFunctionToolBar, TopToolBar)
	ON_WM_TIMER()
	ON_WM_DESTROY()

	// ������ֹ��ܡ�
	ON_BN_CLICKED(IDC_BTN_PART_FROM_DB, OnAddPartFromDB)
	ON_BN_CLICKED(IDC_BTN_PART_FROM_DXF, OnAddPartFromDxf)
	ON_BN_CLICKED(IDC_BTN_NestTaskParts, OnNestTaskParts)
	ON_BN_CLICKED(IDC_BTN_MEASURE_DISTANCE, OnMeasureDis)
	ON_BN_CLICKED(IDC_BTN_CopyPartPlacement, OnCopyPartPlacement)


	// �༭���ա�
	ON_BN_CLICKED(IDC_BTN_RESET_CAM, OnResetCam)
	ON_BN_CLICKED(IDC_BTN_EDIT_LOOP_LEAD, OnLoopLead)
	ON_BN_CLICKED(IDC_BTN_CHANGE_START_PT, OnStartPt)
	ON_BN_CLICKED(IDC_BTN_AddMicroJoint_2, OnAddMicroJoint)
	ON_BN_CLICKED(IDC_BTN_AddCornerFeature_2, OnAddCornerFeature)

	// �༭����
	ON_BN_CLICKED(IDC_BTN_AutoSequence, OnAutoSeq)
	ON_BN_CLICKED(IDC_BTN_AddFlyCutSequence, OnAddFlyCutSequence)
	ON_BN_CLICKED(IDC_BTN_SIMULATE_SEQ, OnSimulateSeq)
	ON_BN_CLICKED(IDC_BTN_GENERATE_NC, OnGenerateNC)

	// ���á�
	ON_BN_CLICKED(IDC_BTN_PARAM_CONFIG, OnParamConfig)
END_MESSAGE_MAP()

BOOL SheetFunctionToolBar::OnInitDialog()
{
	__super::OnInitDialog();

	// ������ֹ��ܡ�
	{
		m_btnPartFromDB.SetImage(IDB_ADD_PART_FROM_DB_32);
		m_btnPartFromDB.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_PARTPMT_FROM_DB_TIP));

		m_btnPartFromDxf.SetImage(IDB_ADD_PART_FROM_DXF_32);
		m_btnPartFromDxf.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_PARTPMT_FROM_DXF_TIP));

		m_btnNestTaskParts.SetImage(IDB_NestTaskPart_32);
		m_btnNestTaskParts.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_NestTaskParts_TIP));

		m_btnMeasureDis.SetImage(IDB_MEASURE_DIS_32);
		m_btnMeasureDis.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_MEASURE_DISTANCE_TIP));

		m_ctrlCopyPartlacementButton.SetImage(IDB_CopyPart_32);
		m_ctrlCopyPartlacementButton.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_PARTPMT_COPY_TIP));
	}

	// �༭���ա�
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

	// �༭����
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

	// ���á�
	{
		m_ctrlParamBtn.SetImage(IDB_PARAM_CONFIG_32);
		m_ctrlParamBtn.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_CAM_PARAM));
	}

	// �����ӶԻ���
	{
		m_pEditPartLayoutToolBar = new EditPartLayoutToolBar();
		m_pEditPartLayoutToolBar->Create(IDD_clFrame_EditPartLayoutToolBar);
		m_pEditPartLayoutToolBar->ShowWindow(SW_HIDE);

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

	m_nTimer = SetTimer(27, 300, 0);

	SetBackgroundColor(SkinManager::GetBackgroundColor());
	return TRUE;
}

void SheetFunctionToolBar::OnDestroy()
{
	KillTimer(m_nTimer);
	TopToolBar::OnDestroy();
}

void SheetFunctionToolBar::OnTimer(UINT nIDEvent)
{
	// �öԻ��򲻿ɼ�ʱ�����ö�ʱ����
	if (!this->IsWindowVisible()) {
		return;
	}

	CheckWhetherDisplaySubToolBar(&m_ctrlCopyPartlacementButton, m_pEditPartLayoutToolBar); // ����Ҫ��Ҫ��ʾ���ֱ༭��������
	CheckWhetherDisplaySubToolBar(&m_ctrlLoopLeadBtn, m_pLeadLineToolBar); // ����Ҫ��Ҫ��ʾ��������
	CheckWhetherDisplaySubToolBar(&m_ctrlStartPtBtn, m_pStartCutPtToolBar); // ����Ҫ��Ҫ��ʾ���༭��������
	CheckWhetherDisplaySubToolBar(&m_ctrlAddMicroJointBtn, m_pMicroJointToolBar); // ����Ҫ��Ҫ��ʾ΢����������
	CheckWhetherDisplaySubToolBar(&m_ctrlAddCornerFeatureBtn, m_pCornerFeatureToolBar); // ����Ҫ��Ҫ��ʾ��������������
	CheckWhetherDisplaySubToolBar(&m_btnAutoSeq, m_pAutoSequenceToolBar);
	CheckWhetherDisplaySubToolBar(&m_btnAddFlyCutSequence, m_pManualSequenceToolBar);
	CheckWhetherDisplaySubToolBar(&m_ctrlParamBtn, m_pConfigToolBar);

	TopToolBar::OnTimer(nIDEvent);
}

void SheetFunctionToolBar::OnAddPartFromDB()
{
	((SheetSubView*)ClWindowFinder::GetCurrentView_1())->OnPartPmtAddPart();
}

void SheetFunctionToolBar::OnAddPartFromDxf()
{
	((SheetSubView*)ClWindowFinder::GetCurrentView_1())->OnPartPmtAddDxf();
}

void SheetFunctionToolBar::OnNestTaskParts()
{
	((SheetSubView*)ClWindowFinder::GetCurrentView_1())->OnNestTaskParts();
}

void SheetFunctionToolBar::OnCopyPartPlacement()
{
	((SheetSubView*)ClWindowFinder::GetCurrentView_1())->OnPartPmtCopy();
}

void SheetFunctionToolBar::OnMeasureDis()
{
	((SheetSubView*)ClWindowFinder::GetCurrentView_1())->OnMeasureDis();
}

void SheetFunctionToolBar::OnResetCam()
{
	((SheetSubView*)ClWindowFinder::GetCurrentView_1())->OnResetCutFeature();
}

void SheetFunctionToolBar::OnLoopLead()
{
	((SheetSubView*)ClWindowFinder::GetCurrentView_1())->OnEditLoopLead();
}

void SheetFunctionToolBar::OnStartPt()
{
	((SheetSubView*)ClWindowFinder::GetCurrentView_1())->OnEditLoopStartPt();
}

void SheetFunctionToolBar::OnAddMicroJoint()
{
	((SheetSubView*)ClWindowFinder::GetCurrentView_1())->OnAddMicrojoint();
}

void SheetFunctionToolBar::OnAddCornerFeature()
{
	((SheetSubView*)ClWindowFinder::GetCurrentView_1())->OnAddCorner();
}

void SheetFunctionToolBar::OnAutoSeq()
{
	((SheetSubView*)ClWindowFinder::GetCurrentView_1())->OnAutoSequence();
}

void SheetFunctionToolBar::OnAddFlyCutSequence()
{
	((SheetSubView*)ClWindowFinder::GetCurrentView_1())->OnAddFlyCutSeq();
}

void SheetFunctionToolBar::OnSimulateSeq()
{
	((SheetSubView*)ClWindowFinder::GetCurrentView_1())->OnViewSequence();
}

void SheetFunctionToolBar::OnGenerateNC()
{
	((SheetSubView*)ClWindowFinder::GetCurrentView_1())->OnViewNC();
}

void SheetFunctionToolBar::OnParamConfig()
{
	((SheetSubView*)ClWindowFinder::GetCurrentView_1())->SetParamConfig();
}

END_CUTLEADER_NAMESPACE()

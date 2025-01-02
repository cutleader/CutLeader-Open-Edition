#include "StdAfx.h"
#include "BarManager.h"

#include "PatternPropertyBar.h"
#include "LoopToolEditPanel.h"
#include "CornerPropertyBar.h"
#include "LoopLeadPropertyBar.h"
#include "MicroJointPropertyBar.h"
#include "PartPlacementPropertyBar.h"
#include "LoopInstancePropertyBar.h"
#include "PartPlacementViewBar.h"
#include "PartPlacementAddPanel.h"
#include "PartTopologyBar.h"
#include "CutSequencePane.h"
#include "CutNodeViewBar.h"
#include "NCViewBar.h"
#include "ChangeLoopToolPane.h"
#include "PartCommonFunctionPanel.h"
#include "TaskSheetManagementBar.h"
#include "TaskPartManagementBar.h"
#include "TaskCommonFunctionPanel.h"
#include "CanvasPatternPropertyBar.h"

BEGIN_CUTLEADER_NAMESPACE()

BarManager::BarManager(void)
{
	m_pPatPropBar = NULL;
	m_pLoopToolEditPane = NULL;
	m_pLoopLeadPropBar = NULL;
	m_pMicroJointPropBar = NULL;
	m_pCornerPropBar = NULL;
	m_pAddPrtPmtPane = NULL;
	m_pPartPmtPropBar = NULL;
	m_pLoopInstancePropertyBar = NULL;
	m_pPartPmtViewBar = NULL;
	m_pPartTopBar = NULL;
	m_pSeqUnitPane = NULL;
	m_pCutNodeViewBar = NULL;
	m_pNCViewBar = NULL;
	m_pPartCommonFunctionPanel = NULL;
	m_pTaskCommonFunctionPanel = NULL;
	m_pTaskPartManagementBar = NULL;
	m_pTaskSheetManagementBar = NULL;
	m_pChangeLoopToolPanel = NULL;
	m_pCanvasPatternPropertyBar = NULL;
}

BarManager::~BarManager(void)
{
	if (m_pPatPropBar) {
		delete m_pPatPropBar;
		m_pPatPropBar = NULL;
	}

    if (m_pLoopToolEditPane) {
        delete m_pLoopToolEditPane;
        m_pLoopToolEditPane = NULL;
    }
    if (m_pLoopLeadPropBar) {
        delete m_pLoopLeadPropBar;
        m_pLoopLeadPropBar = NULL;
    }
	if (m_pMicroJointPropBar) {
		delete m_pMicroJointPropBar;
		m_pMicroJointPropBar = NULL;
	}
	if (m_pCornerPropBar) {
		delete m_pCornerPropBar;
		m_pCornerPropBar = NULL;
	}
	if (m_pChangeLoopToolPanel) {
		delete m_pChangeLoopToolPanel;
		m_pChangeLoopToolPanel = NULL;
	}

	if (m_pAddPrtPmtPane) {
		delete m_pAddPrtPmtPane;
		m_pAddPrtPmtPane = NULL;
	}
	if (m_pPartPmtPropBar) {
		delete m_pPartPmtPropBar;
		m_pPartPmtPropBar = NULL;
	}
	if (m_pLoopInstancePropertyBar) {
		delete m_pLoopInstancePropertyBar;
		m_pLoopInstancePropertyBar = NULL;
	}
	if (m_pPartTopBar) {
		delete m_pPartTopBar;
		m_pPartTopBar = NULL;
	}
	if (m_pPartPmtViewBar) {
		delete m_pPartPmtViewBar;
		m_pPartPmtViewBar = NULL;
	}

	if (m_pSeqUnitPane) {
		delete m_pSeqUnitPane;
		m_pSeqUnitPane = NULL;
	}
	if (m_pCutNodeViewBar) {
		delete m_pCutNodeViewBar;
		m_pCutNodeViewBar = NULL;
	}

	if (m_pNCViewBar) {
		delete m_pNCViewBar;
		m_pNCViewBar = NULL;
	}

	if (m_pPartCommonFunctionPanel) {
		delete m_pPartCommonFunctionPanel;
		m_pPartCommonFunctionPanel = NULL;
	}
	if (m_pTaskCommonFunctionPanel) {
		delete m_pTaskCommonFunctionPanel;
		m_pTaskCommonFunctionPanel = NULL;
	}
	if (m_pTaskPartManagementBar) {
		delete m_pTaskPartManagementBar;
		m_pTaskPartManagementBar = NULL;
	}
	if (m_pTaskSheetManagementBar) {
		delete m_pTaskSheetManagementBar;
		m_pTaskSheetManagementBar = NULL;
	}

	if (m_pCanvasPatternPropertyBar) {
		delete m_pCanvasPatternPropertyBar;
		m_pCanvasPatternPropertyBar = NULL;
	}
}

void BarManager::Init(CDockingManager* pDockingMgr, CWnd* pParent)
{ 
	m_pDockingMgr = pDockingMgr;
	m_pParent = pParent;

	CreateTaskPartManagementBar();
	CreatePartCommonFunctionPanel();
	CreateTaskCommonFunctionPanel();

	// "geometry" module
	CreatePatternPropBar();

	// "cam feature" module
	CreateMicroJointPropBar();
	CreateCornerPropBar();
	CreateLoopLeadPropBar();
    CreateLoopToolEditPane();

	// 底图几何特征模块。
	CreateCanvasPatternPropBar();

	// "part layout" module.
	CreatePartPmtPropBar();
	CreateLoopInstancePropertyBar();
	CreateAddPrtInstPane();
	CreatePartPmtViewBar();
	CreatePartTopBar();

	// "cutSeq" module.
	CreateSeqUnitPane();
	CreateCutNodeViewBar();

	// "postProcessor" module.
	CreateNCViewBar();

	// 在底部的面板之前创建这个面板，这样这个面板可以占据整个右侧区域。
	CreateCncControlPane1();

	// create bars on the bottom side.
	CreateTaskSheetManagementBar();
}

void BarManager::HideAllBar()
{
	// geometry category.
	m_pPatPropBar->ShowBar(FALSE);

	// cam feature category.
    m_pLoopToolEditPane->ShowBar(FALSE);
	m_pLoopLeadPropBar->ShowBar(FALSE);
	m_pCornerPropBar->ShowBar(FALSE);
	m_pMicroJointPropBar->ShowBar(FALSE);

	// 用于底图几何特征模块。
	m_pCanvasPatternPropertyBar->ShowBar(FALSE);

	// part pmt category.
	m_pPartPmtViewBar->ShowBar(FALSE);
	m_pAddPrtPmtPane->ShowBar(FALSE);
	m_pPartPmtPropBar->ShowBar(FALSE);
	m_pLoopInstancePropertyBar->ShowBar(FALSE);
	m_pPartTopBar->ShowBar(FALSE);

	// sequence category.
	m_pSeqUnitPane->ShowBar(FALSE);
	m_pCutNodeViewBar->ShowBar(FALSE);
	m_pNCViewBar->ShowBar(FALSE);

	m_pPartCommonFunctionPanel->ShowBar(FALSE);
	m_pTaskCommonFunctionPanel->ShowBar(FALSE);
	m_pTaskPartManagementBar->ShowBar(FALSE, FALSE);
	m_pTaskSheetManagementBar->ShowBar(FALSE, FALSE);

	m_pChangeLoopToolPanel->ShowBar(FALSE);
}

void BarManager::HideAllRightBar()
{
	// geometry category.
	m_pPatPropBar->ShowBar(FALSE);

	// cam feature category.
    m_pLoopToolEditPane->ShowBar(FALSE);
	m_pLoopLeadPropBar->ShowBar(FALSE);
	m_pCornerPropBar->ShowBar(FALSE);
	m_pMicroJointPropBar->ShowBar(FALSE);

	// 用于底图几何特征模块。
	m_pCanvasPatternPropertyBar->ShowBar(FALSE);

	// part pmt category.
	m_pPartPmtViewBar->ShowBar(FALSE);
	m_pAddPrtPmtPane->ShowBar(FALSE);
	m_pPartPmtPropBar->ShowBar(FALSE);
	m_pLoopInstancePropertyBar->ShowBar(FALSE);
	m_pPartTopBar->ShowBar(FALSE);

	// sequence category.
	m_pSeqUnitPane->ShowBar(FALSE);
	m_pCutNodeViewBar->ShowBar(FALSE);
	m_pNCViewBar->ShowBar(FALSE);
}

BOOL BarManager::CreatePatternPropBar()
{
	m_pPatPropBar = PatternPropertyBar::GetInstance(m_pParent);
	if (!m_pPatPropBar)
		return FALSE;

	m_pPatPropBar->EnableDocking(CBRS_ALIGN_RIGHT);
	m_pDockingMgr->DockPane(m_pPatPropBar);

	return TRUE;
}

BOOL BarManager::CreateLoopToolEditPane()
{
    m_pLoopToolEditPane = LoopToolEditPanel::GetInstance(m_pParent);
    if (!m_pLoopToolEditPane)
        return FALSE;

    m_pLoopToolEditPane->EnableDocking(CBRS_ALIGN_RIGHT);
    m_pDockingMgr->DockPane(m_pLoopToolEditPane);

    return TRUE;
}

BOOL BarManager::CreateLoopLeadPropBar()
{
	m_pLoopLeadPropBar = LoopLeadPropertyBar::GetInstance(m_pParent);
	if (!m_pLoopLeadPropBar)
		return FALSE;

	m_pLoopLeadPropBar->EnableDocking(CBRS_ALIGN_RIGHT);
	m_pDockingMgr->DockPane(m_pLoopLeadPropBar);

	return TRUE;
}

BOOL BarManager::CreateMicroJointPropBar()
{
	m_pMicroJointPropBar = MicroJointPropertyBar::GetInstance(m_pParent);
	if (!m_pMicroJointPropBar)
		return FALSE;

	m_pMicroJointPropBar->EnableDocking(CBRS_ALIGN_RIGHT);
	m_pDockingMgr->DockPane(m_pMicroJointPropBar);

	return TRUE;
}

BOOL BarManager::CreatePartPmtViewBar()
{
	m_pPartPmtViewBar = PartPlacementViewBar::GetInstance(m_pParent);
	if (!m_pPartPmtViewBar)
		return FALSE;

	m_pPartPmtViewBar->EnableDocking(CBRS_ALIGN_RIGHT);
	m_pDockingMgr->DockPane(m_pPartPmtViewBar);

	return TRUE;
}

BOOL BarManager::CreatePartTopBar()
{
	m_pPartTopBar = PartTopologyBar::GetInstance(m_pParent);
	if (!m_pPartTopBar)
		return FALSE;

	m_pPartTopBar->EnableDocking(CBRS_ALIGN_RIGHT);
	m_pDockingMgr->DockPane(m_pPartTopBar);

	return TRUE;
}

BOOL BarManager::CreateSeqUnitPane()
{
	m_pSeqUnitPane = CutSequencePane::GetInstance(m_pParent);
	if (!m_pSeqUnitPane)
		return FALSE;

	m_pSeqUnitPane->EnableDocking(CBRS_ALIGN_RIGHT);
	m_pDockingMgr->DockPane(m_pSeqUnitPane);

	return TRUE;
}

BOOL BarManager::CreateCutNodeViewBar()
{
	m_pCutNodeViewBar = CutNodeViewBar::GetInstance(m_pParent);
	if (!m_pCutNodeViewBar)
		return FALSE;

	m_pCutNodeViewBar->EnableDocking(CBRS_ALIGN_RIGHT);
	m_pDockingMgr->DockPane(m_pCutNodeViewBar);

	return TRUE;
}

BOOL BarManager::CreateNCViewBar()
{
	m_pNCViewBar = NCViewBar::GetInstance(m_pParent);
	if (!m_pNCViewBar)
		return FALSE;

	m_pNCViewBar->EnableDocking(CBRS_ALIGN_RIGHT);
	m_pDockingMgr->DockPane(m_pNCViewBar);

	return TRUE;
}

BOOL BarManager::CreatePartCommonFunctionPanel()
{
	m_pPartCommonFunctionPanel = PartCommonFunctionPanel::GetInstance(m_pParent);
	if (!m_pPartCommonFunctionPanel)
		return FALSE;

	m_pPartCommonFunctionPanel->EnableDocking(CBRS_ALIGN_LEFT);
	m_pDockingMgr->DockPane(m_pPartCommonFunctionPanel);

	return TRUE;
}

BOOL BarManager::CreateTaskCommonFunctionPanel()
{
	m_pTaskCommonFunctionPanel = TaskCommonFunctionPanel::GetInstance(m_pParent);
	if (!m_pTaskCommonFunctionPanel)
		return FALSE;

	m_pTaskCommonFunctionPanel->EnableDocking(CBRS_ALIGN_LEFT);
	m_pDockingMgr->DockPane(m_pTaskCommonFunctionPanel);

	return TRUE;
}

BOOL BarManager::CreateTaskPartManagementBar()
{
	m_pTaskPartManagementBar = TaskPartManagementBar::GetInstance(m_pParent);
	if (!m_pTaskPartManagementBar)
		return FALSE;

	m_pTaskPartManagementBar->EnableDocking(CBRS_ALIGN_ANY);
	m_pDockingMgr->DockPane(m_pTaskPartManagementBar);

	return TRUE;
}

BOOL BarManager::CreateTaskSheetManagementBar()
{
	m_pTaskSheetManagementBar = TaskSheetManagementBar::GetInstance(m_pParent);
	if (!m_pTaskSheetManagementBar)
		return FALSE;

	m_pTaskSheetManagementBar->EnableDocking(CBRS_ALIGN_ANY);
	m_pDockingMgr->DockPane(m_pTaskSheetManagementBar);

	return TRUE;
}

BOOL BarManager::CreateCncControlPane1()
{
	m_pChangeLoopToolPanel = ChangeLoopToolPanel::GetInstance(m_pParent);
	if (!m_pChangeLoopToolPanel)
		return FALSE;

	m_pChangeLoopToolPanel->EnableDocking(CBRS_ALIGN_ANY);
	m_pDockingMgr->DockPane(m_pChangeLoopToolPanel);

	return TRUE;
}

BOOL BarManager::CreateAddPrtInstPane()
{
	m_pAddPrtPmtPane = PartPlacementAddPanel::GetInstance(m_pParent);
	if (!m_pAddPrtPmtPane)
		return FALSE;

	m_pAddPrtPmtPane->EnableDocking(CBRS_ALIGN_RIGHT);
	m_pDockingMgr->DockPane(m_pAddPrtPmtPane);

	return TRUE;
}

BOOL BarManager::CreatePartPmtPropBar()
{
	m_pPartPmtPropBar = PartPlacementPropertyBar::GetInstance(m_pParent);
	if (!m_pPartPmtPropBar)
		return FALSE;

	m_pPartPmtPropBar->EnableDocking(CBRS_ALIGN_RIGHT);
	m_pDockingMgr->DockPane(m_pPartPmtPropBar);

	return TRUE;
}

BOOL BarManager::CreateLoopInstancePropertyBar()
{
	m_pLoopInstancePropertyBar = LoopInstancePropertyBar::GetInstance(m_pParent);
	if (!m_pLoopInstancePropertyBar)
		return FALSE;

	m_pLoopInstancePropertyBar->EnableDocking(CBRS_ALIGN_RIGHT);
	m_pDockingMgr->DockPane(m_pLoopInstancePropertyBar);

	return TRUE;
}

BOOL BarManager::CreateCornerPropBar()
{
	m_pCornerPropBar = CornerPropertyBar::GetInstance(m_pParent);
	if (!m_pCornerPropBar)
		return FALSE;

	m_pCornerPropBar->EnableDocking(CBRS_ALIGN_RIGHT);
	m_pDockingMgr->DockPane(m_pCornerPropBar);

	return TRUE;
}

BOOL BarManager::CreateCanvasPatternPropBar()
{
	m_pCanvasPatternPropertyBar = CanvasPatternPropertyBar::GetInstance(m_pParent);
	if (!m_pCanvasPatternPropertyBar)
		return FALSE;

	m_pCanvasPatternPropertyBar->EnableDocking(CBRS_ALIGN_RIGHT);
	m_pDockingMgr->DockPane(m_pCanvasPatternPropertyBar);

	return TRUE;
}

END_CUTLEADER_NAMESPACE()

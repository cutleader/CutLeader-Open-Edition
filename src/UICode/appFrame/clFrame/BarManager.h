#pragma once

#include "clFrameCommon.h"
#include "taoResource.h"

DECLARE_CUTLEADER_CLASS(PatternPropertyBar)
DECLARE_CUTLEADER_CLASS(LoopToolEditPanel)
DECLARE_CUTLEADER_CLASS(LoopLeadPropertyBar)
DECLARE_CUTLEADER_CLASS(MicroJointPropertyBar)
DECLARE_CUTLEADER_CLASS(CornerPropertyBar)
DECLARE_CUTLEADER_CLASS(PartPlacementViewBar)
DECLARE_CUTLEADER_CLASS(PartPlacementPropertyBar)
DECLARE_CUTLEADER_CLASS(LoopInstancePropertyBar)
DECLARE_CUTLEADER_CLASS(PartPlacementAddPanel)
DECLARE_CUTLEADER_CLASS(PartTopologyBar)
DECLARE_CUTLEADER_CLASS(CutSequencePane)
DECLARE_CUTLEADER_CLASS(CutNodeViewBar)
DECLARE_CUTLEADER_CLASS(NCViewBar)
DECLARE_CUTLEADER_CLASS(ChangeLoopToolPanel)
DECLARE_CUTLEADER_CLASS(NowAlarmInfoPane)
DECLARE_CUTLEADER_CLASS(AllAlarmInfoPane)
DECLARE_CUTLEADER_CLASS(CncMonitorPane)
DECLARE_CUTLEADER_CLASS(PartCommonFunctionPanel)
DECLARE_CUTLEADER_CLASS(TaskCommonFunctionPanel)
DECLARE_CUTLEADER_CLASS(TaskPartManagementBar)
DECLARE_CUTLEADER_CLASS(TaskSheetManagementBar)
DECLARE_CUTLEADER_CLASS(CanvasPatternPropertyBar)

BEGIN_CUTLEADER_NAMESPACE()

// 面板管理器。
class CLFRAME_Export BarManager
{
public:
	BarManager(void);
	~BarManager(void);

public:
	void Init(CDockingManager* pDockingMgr, CWnd* pParent);

	CWnd* GetMainFrm() const { return m_pParent; }

	// hide all bars.
	void HideAllBar();
	void HideAllRightBar();

private:
	// for "geometry" module.
	BOOL CreatePatternPropBar();

	// for "cam feature" module.
    BOOL CreateLoopToolEditPane();
	BOOL CreateLoopLeadPropBar();
	BOOL CreateMicroJointPropBar();
	BOOL CreateCornerPropBar();

	// 用于底图几何特征模块。
	BOOL CreateCanvasPatternPropBar();

	// for "part placement" module.
	BOOL CreateAddPrtInstPane();
	BOOL CreatePartPmtPropBar();
	BOOL CreateLoopInstancePropertyBar();
	BOOL CreatePartPmtViewBar();
	BOOL CreatePartTopBar();

	// for "cutSeq" module.
	BOOL CreateSeqUnitPane();
	BOOL CreateCutNodeViewBar();

	// for "postProcessor" module.
	BOOL CreateNCViewBar();

	// 常用功能面板。
	BOOL CreatePartCommonFunctionPanel();
	BOOL CreateTaskCommonFunctionPanel();

	BOOL CreateTaskPartManagementBar();
	BOOL CreateTaskSheetManagementBar();

	// cnc相关面板。
	BOOL CreateCncControlPane1();

private:
	CWnd* m_pParent;
	CDockingManager* m_pDockingMgr;

	// for "geometry" module.
	PatternPropertyBar* m_pPatPropBar;

	// for "CAM Feature" module.
    LoopToolEditPanel* m_pLoopToolEditPane;
	LoopLeadPropertyBar* m_pLoopLeadPropBar;
	MicroJointPropertyBar* m_pMicroJointPropBar;
	CornerPropertyBar* m_pCornerPropBar;
	ChangeLoopToolPanel* m_pChangeLoopToolPanel; // 修改轮廓刀具的面板。

	// 用于底图几何特征模块。
	CanvasPatternPropertyBar* m_pCanvasPatternPropertyBar;

	// for "part Layout" module.
	PartPlacementAddPanel* m_pAddPrtPmtPane;
	PartPlacementPropertyBar* m_pPartPmtPropBar;
	LoopInstancePropertyBar* m_pLoopInstancePropertyBar;
	PartPlacementViewBar* m_pPartPmtViewBar;
	PartTopologyBar* m_pPartTopBar;

	// for "cutSeq" module.
	CutSequencePane* m_pSeqUnitPane;
	CutNodeViewBar* m_pCutNodeViewBar;

	// for "postProcessor" module.
	NCViewBar* m_pNCViewBar;

	// 常用功能面板。
	PartCommonFunctionPanel* m_pPartCommonFunctionPanel;
	TaskCommonFunctionPanel* m_pTaskCommonFunctionPanel;

	// 任务管理面板。
	TaskPartManagementBar* m_pTaskPartManagementBar;
	TaskSheetManagementBar* m_pTaskSheetManagementBar;
};

END_CUTLEADER_NAMESPACE()

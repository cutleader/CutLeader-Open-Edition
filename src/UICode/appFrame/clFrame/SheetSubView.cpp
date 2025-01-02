#include "StdAfx.h"
#include "SheetSubView.h"

#include "XMenu.h"
#include "clUtilityResource.h"
#include "ValueInputManager.h"
#include "UnitHelper.h"
#include "clGeometryBaseResource.h"
#include "CommandList.h"
#include "CommandManager.h"
#include "ICommand.h"
#include "UIManager.h"
#include "IAction.h"
#include "DataBaseManager.h"
#include "clBaseDataResource.h"
#include "LogMgr.h"
#include "ProgressDlg.h"
#include "DataCenterItemLoader.h"
#include "PathHelper.h"
#include "ClWindowFinder.h"
#include "ClFolderHelper.h"
#include "RandomGen.h"
#include "locale.h"
#include "LineArc2DList.h"
#include "StringUtil.h"
#include "DateHelper.h"

#include "CLOptionInfo.h"
#include "DisplayParam.h"
#include "ImpExpConfig.h"
#include "clOptionResource.h"
#include "clExpertLibResource.h"
#include "ParamConfig.h"
#include "ParamConfigLoader.h"
#include "SequenceConfigItem.h"
#include "ParamConfigDlg.h"
#include "IMaterialSize.h"
#include "MaterialLibLoader.h"
#include "ConfigData.h"
#include "ReportData.h"
#include "ReportTplList.h"
#include "ReportTpl.h"
#include "ReportPageList.h"
#include "RptPageDrawer.h"
#include "ReportTplLoader.h"
#include "clReportResource.h"
#include "ConfigItemLoader.h"
#include "ExpertLibItemLoader.h"
#include "ConfigNcParamDlg.h"

#include "LoopStartCutDataList.h"
#include "clCutFeatureResource.h"
#include "CornerPropertyBar.h"
#include "MicroJointPropertyBar.h"
#include "LoopLeadPropertyBar.h"
#include "LoopToolEditPanel.h"
#include "PartPlacement.h"
#include "PartLayoutEditor.h"
#include "clPartLayoutResource.h"
#include "PartPlacementSelectAction.h"
#include "PartPlacementList.h"
#include "PartPlacementViewBar.h"
#include "PartPlacementDrawer.h"
#include "PartPlacementPropertyBar.h"
#include "PartPlacementAddPanel.h"
#include "CutSequenceList.h"
#include "clCutSequenceResource.h"
#include "SequenceEditor.h"
#include "CutSequencePane.h"
#include "PartLayoutData.h"
#include "SequenceSimulateShapeList.h"
#include "CutNodeList.h"
#include "clPostProcessorResource.h"
#include "MacRouterEditor.h"
#include "NCViewBar.h"
#include "Sheet.h"
#include "SheetProcessor.h"
#include "SheetDrawer.h"
#include "CutSequenceDrawer.h"
#include "clFrameResource.h"
#include "CutSequenceDeleteCommand.h"
#include "LoopInstanceList.h"
#include "Part.h"
#include "PartCadData.h"
#include "PartCamData.h"
#include "InterfereCheckTaskEx.h"
#include "PartBuilder.h"
#include "PartTopologyBar.h"
#include "PartInstanceList.h"
#include "PartPlacementManager.h"
#include "PatternLoopList.h"
#include "RemnantMatInfoDlg.h"
#include "clDataImportExportResource.h"
#include "clSheetResource.h"
#include "LoopInstanceManager.h"
#include "SequenceSortDlg.h"
#include "ImportDxfDwg2SheetDlg.h"
#include "DxfDwgConvertor.h"
#include "SheetManager.h"
#include "ToolAssignConfigItem.h"
#include "LoopCutFeatureList.h"
#include "ToolInstanceList.h"
#include "ResetLoopToolDlg.h"
#include "ResetSequenceDlg.h"
#include "clFrameUIResource.h"
#include "CanvasGeometryEditor.h"
#include "TaskCommonFunctionPanel.h"
#include "CanvasPatternPropertyBar.h"
#include "ImportDxfDwg2CanvasDlg.h"
#include "GeometryFeatureHelper.h"
#include "clCanvasGeometryUIResource.h"
#include "PatternList.h"
#include "SnapPtManager.h"
#include "SnapPt.h"
#include "clPartLayoutUIResource.h"
#include "clGeometryFeatureResource.h"
#include "CanvasGeometryEditAction.h"
#include "PartLayoutHelper.h"
#include "SequenceHelper.h"
#include "CanvasReferencePointDlg.h"
#include "SheetReferencePointDlg.h"
#include "SheetViewHelper.h"
#include "CanvasRefPtData.h"
#include "SheetRefPtData.h"
#include "SequenceData.h"

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(SheetSubView, CLeaderView)

BEGIN_MESSAGE_MAP(SheetSubView, CLeaderView)
	ON_WM_CREATE()
	ON_WM_SETCURSOR()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_DESTROY()

	// mouse event handler
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MBUTTONDOWN()
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()

	// keyboard event handler
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()


	/************************************************************************/
	// 右键菜单

	ON_COMMAND(IDC_PARTPMT_FROM_DB_PopupMenu, OnPartPmtAddPart_PopupMenu)
	ON_COMMAND(IDC_PARTPMT_FROM_DXF_PopupMenu, OnPartPmtAddDxf_PopupMenu)

	ON_COMMAND(IDC_InPlaceEditPart_PopupMenu, OnInPlaceEditPart_PopupMenu)

	ON_COMMAND(IDC_PARTPMT_SELECT_PopupMenu, OnPartPmtSelect_PopupMenu)
	ON_COMMAND(IDC_PARTPMT_EDIT_Delete_PopupMenu, OnPartPmtDelete_PopupMenu)
	ON_COMMAND(IDC_PARTPMT_EDIT_COPY_PopupMenu, OnPartPmtCopy_PopupMenu)
	ON_COMMAND(IDC_PARTPMT_EDIT_ROTATE_PopupMenu, OnPartPmtRotate_PopupMenu)
	ON_COMMAND(IDC_PARTPMT_EDIT_MOVE_PopupMenu, OnPartPmtMove_PopupMenu)

	ON_COMMAND(IDC_CUTFEATURE_LOOP_LEAD_EDIT_PopupMenu, OnEditLoopLead_PopupMenu)
	ON_COMMAND(IDC_CUTFEATURE_LOOP_START_PT_PopupMenu, OnEditLoopStartPt_PopupMenu)

	ON_COMMAND(IDC_SEQ_AUTOSEQUENCE_PopupMenu, OnAutoSequence_PopupMenu)
	ON_COMMAND(IDC_SEQ_VIEWSEQUENCE_PopupMenu, OnViewSequence_PopupMenu)
	ON_COMMAND(IDC_MACROUTER_VIEW_NC_PopupMenu, OnViewNC_PopupMenu)

	ON_COMMAND(IDC_AddPatternFromDxf_PopupMenu, OnAddPatternFromDxf_PopupMenu)
	ON_COMMAND(IDC_AddCanvasPattern2Task_PopupMenu, OnAddCanvasPattern2Task_PopupMenu)
	ON_COMMAND(IDC_SelectCanvasPattern_PopupMenu, OnSelectCanvasPattern_PopupMenu)
	ON_COMMAND(IDC_DeleteCanvasPattern_PopupMenu, OnDeleteCanvasPattern_PopupMenu)
	ON_COMMAND(IDC_CopyCanvasPattern_PopupMenu, OnCopyCanvasPattern_PopupMenu)
	ON_COMMAND(IDC_MoveCanvasPattern_PopupMenu, OnMoveCanvasPattern_PopupMenu)
	ON_COMMAND(IDC_RotateCanvasPattern_PopupMenu, OnRotateCanvasPattern_PopupMenu)
	/************************************************************************/


	/************************************************************************/
	// 几何特征工具条。

	ON_COMMAND(IDC_GEOM_DRAW_STDLINE, OnDrawCanvasLine)
	ON_UPDATE_COMMAND_UI(IDC_GEOM_DRAW_STDLINE, OnUpdateUI_DrawCanvasLine)
	ON_COMMAND(IDC_GEOM_DRAW_MULTILINE, OnDrawCanvasPolyLine)
	ON_COMMAND(IDC_GEOM_DRAW_RECTLINE, OnDrawCanvasRectLine)

	ON_COMMAND(IDC_GEOM_DRAW_ARC3PT, OnDrawCanvasArc)
	ON_UPDATE_COMMAND_UI(IDC_GEOM_DRAW_ARC3PT, OnUpdateUI_DrawCanvasArc)

	ON_COMMAND(IDC_GEOM_DRAW_ROUNDHOLE, OnDrawCanvasCircle)
	ON_UPDATE_COMMAND_UI(IDC_GEOM_DRAW_ROUNDHOLE, OnUpdateUI_DrawCanvasCircle)

	ON_COMMAND(IDC_DrawText, OnDrawCanvasText)
	ON_UPDATE_COMMAND_UI(IDC_DrawText, OnUpdateUI_DrawCanvasText)

	ON_COMMAND(IDC_GEOM_EDIT_SELECTPAT, OnSelectCanvasPattern)
	ON_UPDATE_COMMAND_UI(IDC_GEOM_EDIT_SELECTPAT, OnUpdateUI_SelectCanvasPattern)

	ON_COMMAND(IDC_GEOM_EDIT_COPY, OnCopyCanvasPattern)
	ON_UPDATE_COMMAND_UI(IDC_GEOM_EDIT_COPY, OnUpdateUI_CopyCanvasPattern)
	ON_COMMAND(IDC_GEOM_EDIT_MOVE, OnMoveCanvasPattern)
	ON_UPDATE_COMMAND_UI(IDC_GEOM_EDIT_MOVE, OnUpdateUI_MoveCanvasPattern)
	ON_COMMAND(IDC_GEOM_EDIT_ROTATE, OnRotateCanvasPattern)
	ON_UPDATE_COMMAND_UI(IDC_GEOM_EDIT_ROTATE, OnUpdateUI_RotateCanvasPattern)

	// 有关轮廓对齐。
	ON_COMMAND(IDC_AlignLoop_Left, OnLeftAlignCanvasLoop)
	ON_COMMAND(IDC_AlignLoop_Right, OnRightAlignCanvasLoop)
	ON_COMMAND(IDC_AlignLoop_Top, OnTopAlignCanvasLoop)
	ON_COMMAND(IDC_AlignLoop_Bottom, OnBottomAlignCanvasLoop)
	ON_COMMAND(IDC_AlignLoop_Center, OnCenterAlignCanvasLoop)
	ON_UPDATE_COMMAND_UI(IDC_AlignLoop_Center, OnUpdateUI_CenterAlignCanvasLoop)
	ON_COMMAND(IDC_AlignLoop_HorizontalCenter, OnHorizontalCenterAlignCanvasLoop)
	ON_COMMAND(IDC_AlignLoop_VerticalCenter, OnVerticalCenterAlignCanvasLoop)
	/************************************************************************/


	/************************************************************************/
	// Commands under "Part Placement" category.

	// “添加零件”面板。
	ON_COMMAND(IDC_PARTPMT_FROM_DB, OnPartPmtAddPart)
	ON_COMMAND(IDC_PARTPMT_FROM_DXF, OnPartPmtAddDxf)
	ON_COMMAND(IDC_NestTaskParts, OnNestTaskParts)
	ON_UPDATE_COMMAND_UI(IDC_PARTPMT_FROM_DB, OnUpdateUI_PartPmtAddPart)
	ON_UPDATE_COMMAND_UI(IDC_PARTPMT_FROM_DXF, OnUpdateUI_PartPmtAddDxf)
	ON_UPDATE_COMMAND_UI(IDC_NestTaskParts, OnUpdateUI_NestTaskParts)

	// “编辑零件布局”面板。
	ON_COMMAND(IDC_PARTPMT_SELECT, OnPartPmtSelect)
	ON_UPDATE_COMMAND_UI(IDC_PARTPMT_SELECT, OnUpdateUI_PartPmtSelect)
	ON_COMMAND(IDC_PARTPMT_EDIT_COPY, OnPartPmtCopy)
	ON_UPDATE_COMMAND_UI(IDC_PARTPMT_EDIT_COPY, OnUpdateUI_PartPmtCopy)
	ON_COMMAND(IDC_PARTPMT_EDIT_DELETE, OnPartPmtDelete)
	ON_UPDATE_COMMAND_UI(IDC_PARTPMT_EDIT_DELETE, OnUpdateUI_PartPmtDelete)
	ON_COMMAND(IDC_PARTPMT_EDIT_ROTATE, OnPartPmtRotate)
	ON_UPDATE_COMMAND_UI(IDC_PARTPMT_EDIT_ROTATE, OnUpdateUI_PartPmtRotate)
	ON_COMMAND(IDC_PARTPMT_EDIT_MOVE, OnPartPmtMove)
	ON_UPDATE_COMMAND_UI(IDC_PARTPMT_EDIT_MOVE, OnUpdateUI_PartPmtMove)
	ON_COMMAND(IDC_PARTPMT_EDIT_GRID, OnPartPmtGrid)
	ON_UPDATE_COMMAND_UI(IDC_PARTPMT_EDIT_GRID, OnUpdateUI_PartPmtGrid)
	ON_COMMAND(IDC_PARTPMT_EXPLODE_GRID, OnExplodeGrid)
	ON_UPDATE_COMMAND_UI(IDC_PARTPMT_EXPLODE_GRID, OnUpdateUI_ExplodeGrid)

	ON_COMMAND(IDC_GEN_SKELE_REMNANT, OnGenRemnantMat)
	ON_UPDATE_COMMAND_UI(IDC_GEN_SKELE_REMNANT, OnUpdateUI_GenRemnantMat)
	ON_COMMAND(IDC_MEASURE_DISTANCE, OnMeasureDis)

	// “显示比例”面板。
	ON_COMMAND(IDC_PARTPMT_VIEW_ZOOMALL, OnZoomAll)
	ON_COMMAND(IDC_PARTPMT_VIEW_ZOOMIN, OnZoomIn)
	ON_COMMAND(IDC_PARTPMT_VIEW_ZOOMOUT, OnZoomOut)

	// “显示/隐藏”面板。
	ON_COMMAND(IDC_PARTPMT_VIEWBAR_PARTPMT, OnShowPartPmtTreeView)
	ON_UPDATE_COMMAND_UI(IDC_PARTPMT_VIEWBAR_PARTPMT, OnUpdatePartPmtTreeView)

	// 用于调试。
	ON_COMMAND(IDC_PARTPMT_VIEW_OFFSET, OnViewOffsetLoop)
	ON_UPDATE_COMMAND_UI(IDC_PARTPMT_VIEW_OFFSET, OnUpdateOffsetLoop)
	ON_COMMAND(IDC_PARTPMT_VIEWBAR_PARTTOP, OnShowPartTopTreeView)
	ON_UPDATE_COMMAND_UI(IDC_PARTPMT_VIEWBAR_PARTTOP, OnUpdatePartTopTreeView)
	/************************************************************************/


	/************************************************************************/
	// “加工工艺”category下的命令。

	ON_COMMAND(IDC_CUTFEATURE_ASSIGN, OnAssignCutFeature)
	ON_COMMAND(IDC_CUTFEATURE_RESET, OnResetCutFeature)
	ON_COMMAND(IDC_CHECK_INTERFERE, OnCheckInterfere)

	// 轮廓刀具
	ON_COMMAND(IDC_CUTFEATURE_LOOPTOOL_EDIT, OnEditLoopTool)
	ON_COMMAND(IDC_ReAssignLoopTool, OnReAssignLoopTool)

	ON_COMMAND(IDC_CUTFEATURE_LOOP_LEAD_EDIT, OnEditLoopLead)
	ON_COMMAND(IDC_ReAssignLeadLine, OnReAssignLeadLine)
	ON_COMMAND(IDC_CUTFEATURE_LOOP_START_PT, OnEditLoopStartPt)
	ON_COMMAND(IDC_ReAssignStartCutPoint, OnReAssignStartCutPoint)
	ON_COMMAND(IDC_CutOutSide, OnSetLoopAsCutOutside)
	ON_COMMAND(IDC_CutInside, OnSetLoopAsCutInside)
	ON_COMMAND(IDC_CutOn, OnSetLoopAsCutOn)
	ON_COMMAND(IDC_SwitchCutDirection, OnSwitchCutDirection)
	ON_COMMAND(IDC_CcwCutDirection, OnCcwCutDirection)
	ON_COMMAND(IDC_CwCutDirection, OnCwCutDirection)

	ON_COMMAND(IDC_CUTFEATURE_MJ_ADD, OnAddMicrojoint)
	ON_COMMAND(IDC_ReAssignMicroJoint, OnReAssignMicroJoint)
	ON_COMMAND(IDC_CUTFEATURE_MJ_EDIT, OnEditMicrojoint)
	ON_COMMAND(IDC_CUTFEATURE_MJ_DELETE, OnDelMicrojoint)

	ON_COMMAND(IDC_CUTFEATURE_CORNER_ADD, OnAddCorner)
	ON_COMMAND(IDC_ReAssignCornerFeature, OnReAssignCornerFeature)
	ON_COMMAND(IDC_CUTFEATURE_CORNER_EDIT, OnEditCorner)
	ON_COMMAND(IDC_CUTFEATURE_CORNER_DELETE, OnDelCorner)
	/************************************************************************/


	/************************************************************************/
	// Commands under "Sequence" category.

	// "Auto" panel
	ON_COMMAND(IDC_SEQ_AUTOSEQUENCE, OnAutoSequence)
	ON_COMMAND(IDC_ResetSheetSequence, OnResetSequence)
	ON_COMMAND(IDC_SEQ_CLEARALLSEQUENCE, OnClearAllSequence)

	ON_COMMAND(IDC_SEQ_SELECTSEQUENCE, OnSelectSequence)
	ON_COMMAND(IDC_SEQ_REMOVESEQUENCE, OnRemoveSequence)
	ON_COMMAND(IDC_SEQ_SORTSEQUENCE, OnSortSequence)
	ON_COMMAND(IDC_SEQ_REPLACE_HOLE_SEQ, OnReplaceHoleSeq)

	ON_COMMAND(IDC_SEQ_ADD_LOOPSEQ, OnAddLoopSeq)
	ON_COMMAND(IDC_SEQ_INSERT_LOOPSEQ, OnInsertLoopSeq)

	ON_COMMAND(IDC_SEQ_ADD_BDGESEQ, OnAddBdgeSeq)
	ON_UPDATE_COMMAND_UI(IDC_SEQ_ADD_BDGESEQ, OnUpdateUI_AddBdgeSeq)
	ON_COMMAND(IDC_SEQ_BREAK_BDGESEQ, OnBreakBdgeSeq)
	ON_UPDATE_COMMAND_UI(IDC_SEQ_BREAK_BDGESEQ, OnUpdateUI_BreakBdgeSeq)
	ON_COMMAND(IDC_SEQ_BDGE_START_LOOP, OnBdgeStartLoop)
	ON_UPDATE_COMMAND_UI(IDC_SEQ_BDGE_START_LOOP, OnUpdateUI_BdgeStartLoop)

	ON_COMMAND(IDC_SEQ_ADD_CHAINSEQ, OnAddChainSeq)
	ON_UPDATE_COMMAND_UI(IDC_SEQ_ADD_CHAINSEQ, OnUpdateUI_AddChainSeq)
	ON_COMMAND(IDC_SEQ_INSERT_CHAIN_NODE, OnInsertChainNode)
	ON_UPDATE_COMMAND_UI(IDC_SEQ_INSERT_CHAIN_NODE, OnUpdateUI_InsertChainNode)
	ON_COMMAND(IDC_SEQ_DEL_CHAIN_NODE, OnDelChainNode)
	ON_UPDATE_COMMAND_UI(IDC_SEQ_DEL_CHAIN_NODE, OnUpdateUI_DelChainNode)
	ON_COMMAND(IDC_SEQ_INSERT_RESET_INSTRUCTION, OnInsertResetInstruction)
	ON_COMMAND(IDC_SEQ_DEL_RESET_INSTRUCTION, OnDelResetInstruction)

	ON_COMMAND(IDC_SEQ_ADD_GRIDSEQ, OnAddGridSeq)
	ON_UPDATE_COMMAND_UI(IDC_SEQ_ADD_GRIDSEQ, OnUpdateUI_AddGridSeq)
	ON_COMMAND(IDC_SEQ_ADD_FLYCUTSEQ, OnAddFlyCutSeq)

	ON_COMMAND(IDC_SEQ_ADD_MOV_PT, OnAddMovePoint)
	ON_UPDATE_COMMAND_UI(IDC_SEQ_ADD_MOV_PT, OnUpdateUI_AddMovePoint)
	ON_COMMAND(IDC_SEQ_DEL_MOV_PT, OnDelMovePoint)
	ON_UPDATE_COMMAND_UI(IDC_SEQ_DEL_MOV_PT, OnUpdateUI_DelMovePoint)

	ON_COMMAND(IDC_SEQ_AUTO_REM_CUTLINE, OnAutoRemCutline)
	ON_UPDATE_COMMAND_UI(IDC_SEQ_AUTO_REM_CUTLINE, OnUpdateUI_AutoRemCutline)
	ON_COMMAND(IDC_SEQ_CREATE_REM_CUTLINE, OnAddRemCutline)
	ON_UPDATE_COMMAND_UI(IDC_SEQ_CREATE_REM_CUTLINE, OnUpdateUI_AddRemCutline)
	ON_COMMAND(IDC_SEQ_DELETE_REM_CUTLINE, OnDelRemCutline)
	ON_UPDATE_COMMAND_UI(IDC_SEQ_DELETE_REM_CUTLINE, OnUpdateUI_DelRemCutline)

	ON_COMMAND(IDC_SEQ_VIEWSEQUENCE, OnViewSequence)
	ON_COMMAND(IDC_MACROUTER_VIEW_NC, OnViewNC)
	ON_COMMAND(IDC_ConfigNcParam, OnConfigNcParam)

	// “显示/隐藏”面板。
	ON_COMMAND(IDC_SEQ_VIEWBAR_SEQUNIT, OnShowSeqUnitPane)
	ON_UPDATE_COMMAND_UI(IDC_SEQ_VIEWBAR_SEQUNIT, OnUpdateSeqUnitPane)
	/************************************************************************/


	ON_COMMAND(IDC_PreviewTaskReport, OnPreviewTaskReport)
	ON_COMMAND(IDC_PrintTaskReport, OnPrintTaskReport)
	ON_UPDATE_COMMAND_UI(IDC_PrintTaskReport, OnUpdateUI_PrintTaskReport)
	ON_COMMAND(IDC_PreviewSheetReport, OnPreviewSheetReport)
	ON_COMMAND(IDC_PrintSheetReport, OnPrintSheetReport)
	ON_UPDATE_COMMAND_UI(IDC_PrintSheetReport, OnUpdateUI_PrintSheetReport)
	ON_COMMAND(IDC_PreviewPartLabel, OnPreviewPartLabel)
	ON_COMMAND(IDC_PrintPartLabel, OnPrintPartLabel)
	ON_UPDATE_COMMAND_UI(IDC_PrintPartLabel, OnUpdateUI_PrintPartLabel)
	ON_COMMAND(IDC_PARAM_CONFIG, SetParamConfig)

	ON_MESSAGE(WM_EnterCanvasLoopSelectAction, OnEnterCanvasLoopSelectAction)
	ON_MESSAGE(WM_EnterCanvasPatternSelectAction_PreSelectOnePattern, OnEnterCanvasPatternSelectAction_PreSelectOnePattern)
	ON_MESSAGE(WM_EnterCanvasLoopSelectAction_PreSelectLoopAndText, OnEnterCanvasLoopSelectAction_PreSelectLoopAndText)

	ON_MESSAGE(WM_EnterSelectPartPlacementOrLoopInstanceAction, OnEnterSelectPartPlacementOrLoopInstanceAction)
	ON_MESSAGE(WM_EnterSelectPartPlacementAction, OnEnterSelectPartPlacementAction)
	ON_MESSAGE(WM_EnterSelectLoopInstanceAction, OnEnterSelectLoopInstanceAction)

	ON_MESSAGE(WM_HideOrShowCutPath, OnHideOrShowCutPath)
	ON_MESSAGE(WM_QueryOtherLoopTools, OnQueryOtherLoopTools)
	ON_MESSAGE(WM_displayCurrentWorkModeOnStatusBar, OnDisplayCurrentWorkModeOnStatusBar)
	ON_MESSAGE(WM_updateSheetPreviewData, OnUpdateSheetPreviewData)
	ON_MESSAGE(WM_TellSheetView_toChangeToolOfSelectedLoopInstances, OnChangeToolOfSelectedLoopInstances)
END_MESSAGE_MAP()

SheetSubView::SheetSubView(void)
{
	m_pSnapPtManager.reset(new SnapPtManager(m_pViewPort));
	m_pPartLayoutEditor.reset(new PartLayoutEditor(m_pViewPort));
	m_pSequenceEditor.reset(new SequenceEditor(m_pViewPort));
	m_pMacRouterEditor.reset(new MacRouterEditor(m_pViewPort));
	m_pCanvasGeometryEditor.reset(new CanvasGeometryEditor(m_pViewPort));

	m_pSheetDrawer.reset(new SheetDrawer(m_pViewPort));

	m_pOffPartPlacementList.reset();
	m_pSequenceSimulateShapes.reset(new SequenceSimulateShapeList);
	m_bHideSequenceSimulateShapes = false;
	m_iLatestModifyTimeStamp_ofPartPlacement = 0;
	m_iLatestModifyTimeStamp_ofSequenceData = 0;
}

SheetSubView::~SheetSubView(void)
{
}

void SheetSubView::OnDraw(CDC* pDC)
{
	// call base class to draw something.
	CLeaderView::OnDraw(pDC);

	if (!m_pSheet) {
		return;
	}
	if (!m_pCurEditor) {
		return;
	}

	DisplayParamPtr pDisplayParam = CLOptionInfo::GetInstance()->GetDisplayParam();
	IActionPtr pAction = m_pCurEditor->GetActionObject();
	if (pAction && (pAction->GetActionType() == SEQUENCE_VIEW || pAction->GetActionType() == MACROUTER_NC_SIMULATE ||
		pAction->GetActionType() == SEQUENCE_ADD_RESET_INSTRUCT)) // “模拟工序”等动作时，不显示加工工艺。
	{
		// 保存原有的显示选项。
		BOOL bShowLead = pDisplayParam->GetShowLead();
		BOOL bShowMicroJoint = pDisplayParam->GetShowMicroJoint();
		BOOL bShowCorner = pDisplayParam->GetShowCorner();

		// 禁止显示“切割特征”。
		pDisplayParam->SetShowLead(FALSE);
		pDisplayParam->SetShowMicroJoint(FALSE);
		pDisplayParam->SetShowCorner(FALSE);

		// 绘制。
		COLORREF dwOldColor;
		m_pViewPort->GetDrawColor(dwOldColor);
		m_pViewPort->SetDrawColor(*pDisplayParam->GetPatColor());
		m_pSheetDrawer->DrawSheet();
		m_pViewPort->SetDrawColor(dwOldColor);

		// 恢复原有的显示选项。
		pDisplayParam->SetShowLead(bShowLead);
		pDisplayParam->SetShowMicroJoint(bShowMicroJoint);
		pDisplayParam->SetShowCorner(bShowCorner);
	}
	else {
		COLORREF dwOldColor;
		m_pViewPort->GetDrawColor(dwOldColor);
		m_pViewPort->SetDrawColor(*pDisplayParam->GetPatColor());
		m_pSheetDrawer->DrawSheet();
		m_pViewPort->SetDrawColor(dwOldColor);
	}

	// 更新工序并绘制切割路径。
	SheetViewHelper::UpdateSequenceAndDrawCutPath(m_pSheet.get(), m_bHideSequenceSimulateShapes, m_iLatestModifyTimeStamp_ofPartPlacement,
		m_iLatestModifyTimeStamp_ofSequenceData, m_sequenceDeleteCommandData, m_pSequenceSimulateShapes, m_pViewPort);

	// 绘制偏移后的“零件放置”对象。
	if (m_pOffPartPlacementList) {
		PartPlacementDrawer partPlacementDrawer(m_pViewPort);
		partPlacementDrawer.DrawPartPlacements(m_pOffPartPlacementList.get());
	}

	// 显示当前action相关的内容。
	if (pAction) {
		pAction->DrawBeforeFinish(); // 显示当前action特有的内容。
		this->PostMessage(WM_displayCurrentWorkModeOnStatusBar, 0, 0); // 显示当前工作模式。
		DisplayPromptMessageOfAction(pAction.get()); // 把这个action的提示消息显示在状态栏。
	}

	// redraw the draw assist information.
	m_pSnapPtManager->DrawAssistInfo();

	// swap buffer
	m_pViewPort->SwapBuffers();
}

void SheetSubView::ProcessAfterUndo(const ICommand* pCommand)
{
	const CString& strEditorName = pCommand->GetEditorData().GetEditorName();
	if (strEditorName == MultiLanguageMgr::GetInstance()->TranslateString(IDS_PART_LAYOUT)) // 这个命令是“零件布局编辑器”中执行的。
	{
		// “零件布局编辑器”中执行的命令可能导致无效工序的删除，这里需要恢复这些工序。
		map<LONGLONG, CutSequenceDeleteCommandPtr>::iterator iter = m_sequenceDeleteCommandData.find(pCommand->GetID());
		if (iter != m_sequenceDeleteCommandData.end()) {
			iter->second->UnDo();
			m_sequenceDeleteCommandData.erase(iter);
		}

		// 此时去刷新零件管理器和板材管理器。
		this->SendMessage(WM_updateSheetPreviewData, 0, 0);
		ClWindowFinder::GetCurrentView_2()->SendMessage(WM_redrawSheetThumbnailOnSheetManagementPanel, 0, 0);
		ClWindowFinder::GetCurrentView_2()->SendMessage(WM_ReDisplayNestedCountOfEachPart, 0, 0);
	}

	// 重置捕捉状态。
	m_pSnapPtManager->ResetSnapStatus();

	Invalidate();
}

void SheetSubView::ProcessAfterRedo(const ICommand* pCommand)
{
	const CString& strEditorName = pCommand->GetEditorData().GetEditorName();
	if (strEditorName == MultiLanguageMgr::GetInstance()->TranslateString(IDS_PART_LAYOUT)) { // 这个命令是“零件布局编辑器”中执行的。
		// 此时去刷新零件管理器和板材管理器。
		this->SendMessage(WM_updateSheetPreviewData, 0, 0);
		ClWindowFinder::GetCurrentView_2()->SendMessage(WM_redrawSheetThumbnailOnSheetManagementPanel, 0, 0);
		ClWindowFinder::GetCurrentView_2()->SendMessage(WM_ReDisplayNestedCountOfEachPart, 0, 0);
	}

	// 重置捕捉状态。
	m_pSnapPtManager->ResetSnapStatus();

	Invalidate();
}

void SheetSubView::UpdateUndoRedoDropdownList(CMFCRibbonBaseElement* pElem)
{
	SheetViewHelper::UpdateUndoRedoDropdownList(pElem, m_pUndoCmdList, m_pRedoCmdList);
}

void SheetSubView::Print(CDC* pDC, CPrintInfo* pInfo)
{
	// the report page which will display.
	UINT iCurPageIndex = pInfo->m_nCurPage;
	ReportPageListPtr pReportPages = m_pReportData->GetReportPageList();
	ReportPagePtr pReportPage = pReportPages->at(iCurPageIndex - 1);

	// draw the current report page.
	RptPageDrawer::DrawRptPage(pReportPage, pDC);
}

void SheetSubView::PreparePrinting(CPrintInfo* pInfo)
{
	// set the report page count.
	ReportPageListPtr pReportPages = m_pReportData->GetReportPageList();
	pInfo->SetMaxPage(pReportPages->size());
}

CString SheetSubView::GetWndText()
{
	CString str;
	GetWindowText(str);
	return str;
}

void SheetSubView::SetWndFocus()
{
	SetFocus();
}

int SheetSubView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	ModifyStyle(0, WS_CLIPCHILDREN);

	// init the OpenGL drawer.
	m_pViewPort->InitEnv(m_hWnd, 0.00001, 10000);

	// the drawing area.
	CRect wndRect;
	GetClientRect(&wndRect);

	// the rect of the geometry items.
	Rect2D geomRect(.0, 60.0, .0, 60.0);

	// set the drawing area.
	double dLeftBottomPtX = .0, dLeftBottomPtY = .0;
	double dXDirRange = m_pViewPort->GetFitAllParam(wndRect.Width(), wndRect.Height(), geomRect.GetXMin(), geomRect.GetXMax(),
		geomRect.GetYMin(), geomRect.GetYMax(), 1.1, dLeftBottomPtX, dLeftBottomPtY);
	m_pViewPort->SetDrawingArea(1.1 * dXDirRange, wndRect.Width(), wndRect.Height(), dLeftBottomPtX, dLeftBottomPtY);

	// 创建“返回到前台cam”面板。
	if (CLOptionInfo::GetInstance()->GetProductParam()->GetIsRunAsBackgroundApp()) {
		m_pReturn2ForegroundCamBar = new Return2ForegroundCamBar();
		m_pReturn2ForegroundCamBar->Create(IDD_clFrame_Return2ForegroundCam);
	}

	return CView::OnCreate(lpCreateStruct);
}

BOOL SheetSubView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if (m_bPopupShow) {
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
	}
	else {
		SetCursor(NULL);
	}

	return FALSE;
}

void SheetSubView::OnMButtonDown(UINT nFlags, CPoint point)
{
	m_OffSetPoint = point;
	CLeaderView::OnMButtonDown(nFlags, point);
}

BOOL SheetSubView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void SheetSubView::OnSize(UINT nType, int cx, int cy)
{
	CLeaderView::OnSize(nType, cx, cy);

	m_pViewPort->SetClientSize(cx, cy);

	// 显示“返回到前台cam”面板。
	if (CLOptionInfo::GetInstance()->GetProductParam()->GetIsRunAsBackgroundApp()) {
		CRect parentRect, winRect;
		this->GetWindowRect(parentRect);
		m_pReturn2ForegroundCamBar->GetWindowRect(winRect);
		m_pReturn2ForegroundCamBar->MoveWindow(parentRect.Width() - winRect.Width() - 1, parentRect.Height() - winRect.Height() - 1, winRect.Width(), winRect.Height());
		m_pReturn2ForegroundCamBar->SetParent(this);
		m_pReturn2ForegroundCamBar->ShowWindow(SW_SHOW);
	}
}

void SheetSubView::OnDestroy()
{
	// 销毁“返回到前台cam”面板。
	if (CLOptionInfo::GetInstance()->GetProductParam()->GetIsRunAsBackgroundApp()) {
		delete m_pReturn2ForegroundCamBar;
	}

	CLeaderView::OnDestroy();
}

BOOL SheetSubView::OnMouseWheel(UINT nFlags, short iDelta, CPoint pt)
{
	// figure out the zoom rate.
	double rate = 1.25;
	if (iDelta > 0)
		rate = 1 / rate;

	// adjust the drawer in the main thread.
	CPoint PtClient = pt;
	ScreenToClient(&PtClient);
	double PointX = PtClient.x;
	double PointY = PtClient.y;
	m_pViewPort->ZoomViewPort(rate, PointX, PointY);

	// 重置捕捉状态。
	m_pSnapPtManager->ResetSnapStatus();

	Invalidate();

	return TRUE;
}

void SheetSubView::OnLButtonDown(UINT nFlags, CPoint point)
{
	double dx = point.x;
	double dy = point.y;
	m_pViewPort->DToW(dx, dy);

	// 检查是不是点中了一个捕获点。
	SnapPtPtr pSnapPt = m_pSnapPtManager->GetSnapPt();
	if (pSnapPt) {
		dx = pSnapPt->GetSnapPtPosition().X();
		dy = pSnapPt->GetSnapPtPosition().Y();
	}

	// 重置捕捉状态。
	m_pSnapPtManager->ResetSnapStatus();

	// delegate to action object
	m_pCurEditor->LButtonDown(dx, dy);

	Invalidate();

	CLeaderView::OnLButtonDown(nFlags, point);
}

void SheetSubView::OnLButtonUp(UINT nFlags, CPoint point)
{
	double dx = point.x;
	double dy = point.y;
	m_pViewPort->DToW(dx, dy);
	m_pCurEditor->LButtonUp(dx, dy);
	Invalidate();

	CLeaderView::OnLButtonUp(nFlags, point);
}

void SheetSubView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	double dx = point.x;
	double dy = point.y;
	m_pViewPort->DToW(dx, dy);
	m_pCurEditor->LButtonDBClk(dx, dy);
	Invalidate();

	CLeaderView::OnLButtonDblClk(nFlags, point);
}

void SheetSubView::OnContextMenu(CWnd*, CPoint point)
{
	// 当前动作要是想处理右键消息，不弹出右键菜单。
	if (m_pCurEditor->GetActionObject()->IsProcessRightClickEvent()) {
		return;
	}

	m_bPopupShow = TRUE;
	SheetViewHelper::CreatePopupMenu(m_pSheet.get(), point);
	m_bPopupShow = FALSE;
}

void SheetSubView::OnRButtonUp(UINT nFlags, CPoint point)
{
	double dx = point.x;
	double dy = point.y;
	m_pViewPort->DToW(dx, dy);
	m_pCurEditor->RButtonUp(dx, dy);
	Invalidate();

	CLeaderView::OnRButtonUp(nFlags, point);
}

void SheetSubView::OnMouseMove(UINT nFlags, CPoint point)
{
	CLeaderView::OnMouseMove(nFlags, point);

	// when move cursor upon the view, set the focus.
	SetFocus();

	if (nFlags & MK_MBUTTON) { // middle button down.
		CPoint ptOffSet = point - m_OffSetPoint;
		m_pViewPort->PanViewPort(ptOffSet.x, ptOffSet.y);
		m_OffSetPoint = point;
	}
	else { // delegate the "MouseMove" event to the action.
		double dx = point.x;
		double dy = point.y;
		m_pViewPort->DToW(dx, dy);

		// 计算捕捉点。
		IActionPtr pCurrentAction = m_pCurEditor->GetActionObject();
		if (pCurrentAction) {
			SnapInputPtr pSnapInput = pCurrentAction->GetSnapInput();
			m_pSnapPtManager->CalculateSnapPt(pSnapInput->GetSnapStyle(), pSnapInput->GetPatterns(), pSnapInput->GetTransformMatrix(), pCurrentAction->GetFirstInput(), dx, dy);
		}

		// delegate to action object
		m_pCurEditor->MovePoint(dx, dy);
	}

	// 更新状态栏显示。
	this->PostMessage(WM_displayCursorPositionOnStatusBar, 0, 0);

	Invalidate();
}

void SheetSubView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CLeaderView::OnKeyDown(nChar, nRepCnt, nFlags);

	if (ValueInputManager::GetInstance()->IsInputS(nChar) && (::GetKeyState(VK_CONTROL) & 0x8000)) {
		GetDocument()->OnSaveDocument();
	}
	else if (ValueInputManager::GetInstance()->IsInputZ(nChar) && (::GetKeyState(VK_CONTROL) & 0x8000)) {
		OnEditUndo();
	}
	else {
		m_pCurEditor->OnKeyDown(nChar, nRepCnt, nFlags);
	}

	Invalidate();
}

void SheetSubView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CLeaderView::OnKeyUp(nChar, nRepCnt, nFlags);

	if (ValueInputManager::GetInstance()->IsInputS(nChar) && (::GetKeyState(VK_CONTROL) & 0x8000)) {
		;
	}
	else if (ValueInputManager::GetInstance()->IsInputZ(nChar) && (::GetKeyState(VK_CONTROL) & 0x8000)) {
		;
	}
	else {
		m_pCurEditor->OnKeyUp(nChar, nRepCnt, nFlags);
	}
}

void SheetSubView::OnAddPatternFromDxf()
{
	ImportDxfDwg2CanvasDlg dlg(this);
	if (dlg.DoModal() == IDOK) {
		// 导入dxf/dwg文件为PartCadData对象，此时不要生成阵列回路，否则会影响拓扑关系识别。
		CString strDxfDwgFilePath = dlg.GetPathName();
		BOOL bIsDwg = PathHelper::GetFileExt(strDxfDwgFilePath).CompareNoCase(MultiLanguageMgr::GetInstance()->TranslateString(IDS_IMPEXP_DWG_EXT)) == 0 ? TRUE : FALSE;
		PartCadDataPtr pPartCadData = DxfDwgConvertor::DxfDwg2CadData(strDxfDwgFilePath, bIsDwg);

		// 进入action。
		SwitchEditor(m_pCanvasGeometryEditor);
		m_pCanvasGeometryEditor->AddPatternFromDxf(pPartCadData->GetPatternList(), pPartCadData->GetPatternLoopList(), pPartCadData->GetTextStructs(),
			pPartCadData->GetLoopTopologyItems_FW());
	}
}

void SheetSubView::OnDrawCanvasLine()
{
	SwitchEditor(m_pCanvasGeometryEditor);
	m_pCanvasGeometryEditor->SetEditAction(EditLinePatternOnCanvas);
}

void SheetSubView::OnDrawCanvasArc()
{
	SwitchEditor(m_pCanvasGeometryEditor);
	m_pCanvasGeometryEditor->SetEditAction(EditArcPatternOnCanvas);
}

void SheetSubView::OnDrawCanvasPolyLine()
{
	SwitchEditor(m_pCanvasGeometryEditor);
	m_pCanvasGeometryEditor->SetEditAction(EditMultiLinePatternOnCanvas);
}

void SheetSubView::OnDrawCanvasRect()
{
	SwitchEditor(m_pCanvasGeometryEditor);
	m_pCanvasGeometryEditor->SetEditAction(EditRectPatternOnCanvas);
}

void SheetSubView::OnDrawCanvasRectLine()
{
	SwitchEditor(m_pCanvasGeometryEditor);
	m_pCanvasGeometryEditor->SetEditAction(EditRectLinePatternOnCanvas);
}

void SheetSubView::OnDrawCanvasCircle()
{
	SwitchEditor(m_pCanvasGeometryEditor);
	m_pCanvasGeometryEditor->SetEditAction(EditCirclePatternOnCanvas);
}

void SheetSubView::OnDrawCanvasText()
{
	SwitchEditor(m_pCanvasGeometryEditor);
	m_pCanvasGeometryEditor->SetEditAction(DrawTextStructOnCanvas);
}

void SheetSubView::OnSelectCanvasPattern()
{
	SwitchEditor(m_pCanvasGeometryEditor);
	m_pCanvasGeometryEditor->StartLoopSelectAction();
}

void SheetSubView::OnDeleteCanvasPattern()
{
	SwitchEditor(m_pCanvasGeometryEditor);
	m_pCanvasGeometryEditor->SetEditAction(DeletePatternOnCanvas);
}

void SheetSubView::OnCopyCanvasPattern()
{
	SwitchEditor(m_pCanvasGeometryEditor);
	m_pCanvasGeometryEditor->SetEditAction(CopyPatternOnCanvas);
}

void SheetSubView::OnMoveCanvasPattern()
{
	SwitchEditor(m_pCanvasGeometryEditor);
	m_pCanvasGeometryEditor->SetEditAction(MovePatternOnCanvas);
}

void SheetSubView::OnRotateCanvasPattern()
{
	SwitchEditor(m_pCanvasGeometryEditor);
	m_pCanvasGeometryEditor->SetEditAction(RotatePatternOnCanvas);
}

void SheetSubView::OnGridCanvasPattern()
{
	SwitchEditor(m_pCanvasGeometryEditor);
	m_pCanvasGeometryEditor->SetEditAction(GridPatternOnCanvas);
}

void SheetSubView::OnAddCanvasPattern2Task()
{
	SwitchEditor(m_pCanvasGeometryEditor);
	m_pCanvasGeometryEditor->SetEditAction(AddCanvasPattern2Task);
}

void SheetSubView::OnLeftAlignCanvasLoop()
{
	AlignLoops(AlignLoop_Left);
}

void SheetSubView::OnRightAlignCanvasLoop()
{
	AlignLoops(AlignLoop_Right);
}

void SheetSubView::OnTopAlignCanvasLoop()
{
	AlignLoops(AlignLoop_Top);
}

void SheetSubView::OnBottomAlignCanvasLoop()
{
	AlignLoops(AlignLoop_Bottom);
}

void SheetSubView::OnCenterAlignCanvasLoop()
{
	AlignLoops(AlignLoop_Center);
}

void SheetSubView::OnHorizontalCenterAlignCanvasLoop()
{
	AlignLoops(AlignLoop_HorizontalCenter);
}

void SheetSubView::OnVerticalCenterAlignCanvasLoop()
{
	AlignLoops(AlignLoop_VerticalCenter);
}

void SheetSubView::OnPreviewTaskReport()
{
	ClWindowFinder::GetCurrentView_2()->SendMessage(WM_PreviewTaskReport, 0, 0);
}

void SheetSubView::OnPrintTaskReport()
{
	ClWindowFinder::GetCurrentView_2()->SendMessage(WM_PrintTaskReport, 0, 0);
}

void SheetSubView::OnPreviewSheetReport()
{
	if (SheetViewHelper::GenerateSheetReport(m_pSheet, m_pReportData))
		PreviewData();
}

void SheetSubView::OnPrintSheetReport()
{
	if (SheetViewHelper::GenerateSheetReport(m_pSheet, m_pReportData))
		PrintData();
}

void SheetSubView::OnPreviewPartLabel()
{
	if (SheetViewHelper::GenerateLabelReport(m_pSheet, m_pReportData))
		PreviewData();
}

void SheetSubView::OnPrintPartLabel()
{
	if (SheetViewHelper::GenerateLabelReport(m_pSheet, m_pReportData))
		PrintData();
}

void SheetSubView::SetParamConfig()
{
	LONGLONG iParamConfigID = m_pSheet->GetParamConfigID();
	ParamConfigDlg dlg(this);
	dlg.Init(iParamConfigID);
	if (dlg.DoModal() == IDOK) {
		ParamConfigPtr pParamConfig = ParamConfigLoader::LoadParamConfig(DataBaseManager::GetInstance()->GetExpLibDBConnect(), iParamConfigID);
		m_pPartLayoutEditor->SetParamConfig(pParamConfig);
		m_pSequenceEditor->SetParamConfig(pParamConfig);
	}
}

void SheetSubView::InitSheetView(SheetPtr pSheet)
{
	m_pSheet = pSheet;

	// 参数配置
	LONGLONG iParamConfigID = pSheet->GetParamConfigID();
	DBConnectPtr pExpLibDBCon = DataBaseManager::GetInstance()->GetExpLibDBConnect();
	ParamConfigPtr pParamConfig = ParamConfigLoader::LoadParamConfig(pExpLibDBCon, iParamConfigID);

	// 板材用的材料尺寸
	DBConnectPtr pMatLibDBCon = DataBaseManager::GetInstance()->GetMatLibDBConnect();
	IMaterialSizePtr pMaterialSize = MaterialLibLoader::LoadMatSize(pMatLibDBCon, pSheet->GetMaterialSizeID());


	/************************************************************************/
	// init the editors.

	PartPlacementListPtr pPartPlacements = pSheet->GetPartPlacements();
	PartInstanceListPtr pPartInstances = pSheet->GetPartInstances();
	PartGroupListPtr pPartGroups = pSheet->GetPartGroupList();

	if (m_pSheet->GetIsCanvas()) { // 初始化底图几何特征编辑器。
		m_pCanvasGeometryEditor->SetPartPlacement(pPartPlacements->front());
		m_pCanvasGeometryEditor->SetPartInstance(pPartInstances->front());
		m_pCanvasGeometryEditor->SetParamConfig(pParamConfig);
	}

	// init the sheet layout editor.
	m_pPartLayoutEditor->SetPartPlacements(pPartPlacements);
	m_pPartLayoutEditor->SetPartInsts(pPartInstances);
	m_pPartLayoutEditor->SetPartGroups(pPartGroups);
	m_pPartLayoutEditor->SetMatSize(pMaterialSize);
	m_pPartLayoutEditor->SetParamConfig(pParamConfig);

	// init the sequence editor.
	PartLayoutDataPtr pPartLayoutData(new PartLayoutData(pPartPlacements, pPartInstances));
	m_pSequenceEditor->SetPartLayoutData(pPartLayoutData);
	SequenceDataPtr pSequenceData(new SequenceData(pSheet->GetRefPtData(), pSheet->GetCutSequences()));
	m_pSequenceEditor->SetSequenceData(pSequenceData);
	m_pSequenceEditor->SetMatSize(pMaterialSize);
	m_pSequenceEditor->SetParamConfig(pParamConfig);

	// init machine router editor.
	m_pMacRouterEditor->SetMatSize(pMaterialSize);

	if (m_pSheet->GetIsCanvas())
	{
		TaskCommonFunctionPanel::GetInstance()->ShowCanvasFunctionToolBar();
		if (m_pCurEditor) {
			SwitchEditor(m_pCanvasGeometryEditor);
			m_pCanvasGeometryEditor->StartLoopSelectAction();
		}
		else {
			m_pCanvasGeometryEditor->StartLoopSelectAction();
			m_pCurEditor = m_pCanvasGeometryEditor;
		}
	}
	else {
		TaskCommonFunctionPanel::GetInstance()->ShowSheetFunctionToolBar();
		if (m_pCurEditor) {
			SwitchEditor(m_pPartLayoutEditor);
			m_pPartLayoutEditor->SetEditAction(EDIT_SELECTPRTPMT);
		}
		else {
			m_pPartLayoutEditor->SetEditAction(EDIT_SELECTPRTPMT);
			m_pCurEditor = m_pPartLayoutEditor;
		}
	}
	/************************************************************************/


	// init the sheet drawer.
	m_pSheetDrawer->Init(pSheet, pMaterialSize);

	// 重置这两个值，切割路径模拟信息。
	m_iLatestModifyTimeStamp_ofPartPlacement = 0;
	m_iLatestModifyTimeStamp_ofSequenceData = 0;
	m_sequenceDeleteCommandData.clear();

	// 适配视图区域的显示。
	OnZoomAll();
}

void SheetSubView::PlacePartOnCurrentSheetSubView(LONGLONG iPartID)
{
	if (m_pSheet->GetIsCanvas()) {
		CString str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_CannotPlacePartOnCanvas);
		MessageBox(str, MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME), MB_OK | MB_ICONWARNING);
		return;
	}

	SwitchEditor(m_pPartLayoutEditor);
	m_pPartLayoutEditor->AddPartPmtFromDataCenter(iPartID, m_pSheet->GetParamConfigID());
}

void SheetSubView::SwitchEditor(IEditorPtr pNextEditor)
{
	CString strNextEditorName = pNextEditor->GetEditorName();
	if (m_pCurEditor->GetEditorName() == strNextEditorName) { // 不需要切换编辑器。
		return;
	}
	if (strNextEditorName == MultiLanguageMgr::GetInstance()->TranslateString(IDS_CanvasGeometry)) {
		m_pCurEditor = m_pCanvasGeometryEditor;
		m_pCurEditor->SetDummyAction();

		// 隐藏相关的界面。
		m_pPartLayoutEditor->HideAllActionBars();
		m_pPartLayoutEditor->HideAllPanels();
		m_pSequenceEditor->HideAllActionBars();
		m_pSequenceEditor->HideAllPanels();
		NCViewBar::GetInstance()->ShowBar(FALSE);
	}
	else if (strNextEditorName == MultiLanguageMgr::GetInstance()->TranslateString(IDS_PART_LAYOUT)) { // 切换到了“零件布局编辑器”。
		m_pCurEditor = m_pPartLayoutEditor;
		m_pCurEditor->SetDummyAction();

		// 隐藏相关的界面。
		m_pCanvasGeometryEditor->HideAllActionBars();
		m_pCanvasGeometryEditor->HideAllPanels();
		m_pSequenceEditor->HideAllActionBars();
		m_pSequenceEditor->HideAllPanels();
		NCViewBar::GetInstance()->ShowBar(FALSE);
	}
	else if (strNextEditorName == MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ)) { // 切换到了“工序编辑器”。
		m_pCurEditor = m_pSequenceEditor;
		m_pCurEditor->SetDummyAction();

		// 隐藏相关的界面。
		m_pCanvasGeometryEditor->HideAllActionBars();
		m_pCanvasGeometryEditor->HideAllPanels();
		m_pPartLayoutEditor->HideAllActionBars();
		m_pPartLayoutEditor->HideAllPanels();
		NCViewBar::GetInstance()->ShowBar(FALSE);
	}
	else if (strNextEditorName == MultiLanguageMgr::GetInstance()->TranslateString(IDS_NC)) { // 切换到了“NC编辑器”。
		m_pCurEditor = m_pMacRouterEditor;

		// 隐藏相关的界面。
		m_pCanvasGeometryEditor->HideAllActionBars();
		m_pCanvasGeometryEditor->HideAllPanels();
		m_pPartLayoutEditor->HideAllActionBars();
		m_pPartLayoutEditor->HideAllPanels();
		m_pSequenceEditor->HideAllActionBars();
		m_pSequenceEditor->HideAllPanels();
	}
}

LRESULT SheetSubView::OnHideOrShowCutPath(WPARAM wParam, LPARAM lParam)
{
	if (wParam == 0) {
		m_bHideSequenceSimulateShapes = true;
	}
	else if (wParam == 1) {
		m_bHideSequenceSimulateShapes = false;
	}

	Invalidate();

	return 0;
}

LRESULT SheetSubView::OnEnterSelectPartPlacementOrLoopInstanceAction(WPARAM wParam, LPARAM lParam)
{
	SwitchEditor(m_pPartLayoutEditor);
	if (wParam == 1)
		m_pPartLayoutEditor->SetEditAction(EDIT_SelectLoopInstance);
	else if (wParam == 2)
		m_pPartLayoutEditor->SetEditAction(EDIT_SELECTPRTPMT);
	else if (wParam == 3)
		m_pPartLayoutEditor->SetEditAction(EDIT_SelectPartOrLoopInstance);

	return 0;
}

LRESULT SheetSubView::OnEnterCanvasLoopSelectAction(WPARAM wParam, LPARAM lParam)
{
	SwitchEditor(m_pCanvasGeometryEditor);
	m_pCanvasGeometryEditor->StartLoopSelectAction();

	return 0;
}

LRESULT SheetSubView::OnEnterCanvasPatternSelectAction_PreSelectOnePattern(WPARAM wParam, LPARAM lParam)
{
	SwitchEditor(m_pCanvasGeometryEditor);
	m_pCanvasGeometryEditor->StartPatternSelectAction_PreSelectOnePattern((LONGLONG*)wParam);
	this->Invalidate(); // 这里需要刷新一下视图。
	return 0;
}

LRESULT SheetSubView::OnEnterCanvasLoopSelectAction_PreSelectLoopAndText(WPARAM wParam, LPARAM lParam)
{
	SwitchEditor(m_pCanvasGeometryEditor);
	m_pCanvasGeometryEditor->StartLoopSelectAction_PreSelectLoopAndText((vector<LONGLONG>*)wParam, (vector<LONGLONG>*)lParam);
	this->Invalidate(); // 这里需要刷新一下视图。
	return 0;
}

LRESULT SheetSubView::OnEnterSelectPartPlacementAction(WPARAM wParam, LPARAM lParam)
{
	SwitchEditor(m_pPartLayoutEditor);
	m_pPartLayoutEditor->StartPartPlacementSelectAction_andPreSelect((vector<LONGLONG>*)wParam); // 启动零件放置选择action，并预先选择指定的零件放置。
	this->Invalidate(); // 这里需要刷新一下视图。
	return 0;
}

LRESULT SheetSubView::OnEnterSelectLoopInstanceAction(WPARAM wParam, LPARAM lParam)
{
	SwitchEditor(m_pPartLayoutEditor);
	m_pPartLayoutEditor->StartLoopInstanceSelectAction_andPreSelect((vector<pair<LONGLONG, LONGLONG>>*)wParam); // 启动轮廓实例选择action，并预先选择指定的轮廓实例。
	this->Invalidate(); // 这里需要刷新一下视图。
	return 0;
}

LRESULT SheetSubView::OnQueryOtherLoopTools(WPARAM wParam, LPARAM lParam)
{
	SheetViewHelper::QueryOtherLoopTools(m_pSheet.get(), (const LoopToolDataList*)wParam, (LoopToolDataList*)lParam);
	return 0;
}

void SheetSubView::OnAssignCutFeature()
{
	SheetViewHelper::AssignCutFeature(m_pSheet);
	EnterCanvasLoopOrPartSelectAction();
}

void SheetSubView::OnResetCutFeature()
{
	SheetViewHelper::ResetCutFeature(m_pSheet);
	EnterCanvasLoopOrPartSelectAction();
}

void SheetSubView::OnCheckInterfere()
{
	SheetViewHelper::CheckInterfere(m_pSheet);
	EnterCanvasLoopOrPartSelectAction();
	Invalidate();
}

void SheetSubView::OnEditLoopTool()
{
	SwitchEditor(m_pPartLayoutEditor);
	m_pPartLayoutEditor->SetEditAction(LOOPTOOL_ADJUST);
}

void SheetSubView::OnReAssignLoopTool()
{
	// change the CAM param and reset the loop tools.
	ResetLoopToolDlg dlg(this);
	dlg.Init(m_pSheet);
	if (dlg.DoModal() == IDOK) {
		EnterCanvasLoopOrPartSelectAction();
	}
}

void SheetSubView::OnEditLoopLead()
{
	LoopInstanceListPtr pSelectedLoopInstances = SheetViewHelper::GetSelectedLoopInstances(m_pCurEditor.get()); // 注意这一行要在SwitchEditor之前。
	SwitchEditor(m_pPartLayoutEditor);
	m_pPartLayoutEditor->EditLoopLeadLine(pSelectedLoopInstances);
}

void SheetSubView::OnReAssignLeadLine()
{
	if (SheetViewHelper::ReAssignLeadLine(m_pSheet, m_pCanvasGeometryEditor, m_pPartLayoutEditor, m_pSequenceEditor)){
		EnterCanvasLoopOrPartSelectAction();
	}
}

void SheetSubView::ChangeLoopGap()
{
	SheetViewHelper::ChangeGapOfSelectedLoopInstances(m_pCurEditor.get(), m_pPartLayoutEditor.get());
}

void SheetSubView::OnReAssignStartCutPoint()
{
	if (SheetViewHelper::ReAssignStartCutPoint(m_pSheet, m_pCanvasGeometryEditor, m_pPartLayoutEditor, m_pSequenceEditor)) {
		EnterCanvasLoopOrPartSelectAction();
	}
}

void SheetSubView::OnReAssignMicroJoint()
{
	if (SheetViewHelper::ReAssignMicroJoint(m_pSheet, m_pCanvasGeometryEditor, m_pPartLayoutEditor, m_pSequenceEditor)) {
		EnterCanvasLoopOrPartSelectAction();
	}
}

void SheetSubView::OnReAssignCornerFeature()
{
	if (SheetViewHelper::ReAssignCornerFeature(m_pSheet, m_pCanvasGeometryEditor, m_pPartLayoutEditor, m_pSequenceEditor)) {
		EnterCanvasLoopOrPartSelectAction();
	}
}

void SheetSubView::OnEditLoopStartPt()
{
	SwitchEditor(m_pPartLayoutEditor);
	m_pPartLayoutEditor->SetEditAction(START_PT_ADJUST);
}

void SheetSubView::OnSetLoopAsCutOutside()
{
	ChangeLoopCutSide(PatternLoopCutOffset_Outside);
}

void SheetSubView::OnSetLoopAsCutInside()
{
	ChangeLoopCutSide(PatternLoopCutOffset_Inside);
}

void SheetSubView::OnSetLoopAsCutOn()
{
	ChangeLoopCutSide(PatternLoopCutOffset_On);
}

void SheetSubView::OnSwitchCutDirection()
{
	ChangeLoopCutDirection(CutDirectionChange_Switch);
}

void SheetSubView::OnCcwCutDirection()
{
	ChangeLoopCutDirection(CutDirectionChange_CCW);
}

void SheetSubView::OnCwCutDirection()
{
	ChangeLoopCutDirection(CutDirectionChange_CW);
}

void SheetSubView::OnAddMicrojoint()
{
	SwitchEditor(m_pPartLayoutEditor);
	m_pPartLayoutEditor->SetEditAction(PMT_MJ_ADD);
}

void SheetSubView::OnEditMicrojoint()
{
	SwitchEditor(m_pPartLayoutEditor);
	m_pPartLayoutEditor->SetEditAction(PMT_MJ_EDIT);
}

void SheetSubView::OnDelMicrojoint()
{
	SwitchEditor(m_pPartLayoutEditor);
	m_pPartLayoutEditor->SetEditAction(PMT_MJ_DELETE);
}

void SheetSubView::OnAddCorner()
{
	SwitchEditor(m_pPartLayoutEditor);
	m_pPartLayoutEditor->SetEditAction(PMT_CORNER_ADD);

	// 显示角特征创建界面。
	DBConnectPtr pExpLibConnect = DataBaseManager::GetInstance()->GetExpLibDBConnect();
	CornerConfigItemPtr pCornerConfigItem = ConfigItemLoader::LoadCornerConfigItem(pExpLibConnect, m_pSheet->GetParamConfigID());
	CornerPropertyBar::GetInstance()->DisplayCornerData_4_create(pCornerConfigItem->GetDflCornerType(), pCornerConfigItem->GetDflCornerVal());
}

void SheetSubView::OnEditCorner()
{
	SwitchEditor(m_pPartLayoutEditor);
	m_pPartLayoutEditor->SetEditAction(PMT_CORNER_EDIT);
}

void SheetSubView::OnDelCorner()
{
	SwitchEditor(m_pPartLayoutEditor);
	m_pPartLayoutEditor->SetEditAction(PMT_CORNER_DELETE);
}

void SheetSubView::OnPartPmtAddPart_PopupMenu()
{
	OnPartPmtAddPart();
}

void SheetSubView::OnPartPmtAddDxf_PopupMenu()
{
	OnPartPmtAddDxf();
}

void SheetSubView::OnInPlaceEditPart_PopupMenu()
{
	OnInPlaceEditPart();
}

void SheetSubView::OnPartPmtSelect_PopupMenu()
{
	OnPartPmtSelect();
}

void SheetSubView::OnPartPmtDelete_PopupMenu()
{
	OnPartPmtDelete();
}

void SheetSubView::OnPartPmtCopy_PopupMenu()
{
	OnPartPmtCopy();
}

void SheetSubView::OnPartPmtRotate_PopupMenu()
{
	OnPartPmtRotate();
}

void SheetSubView::OnPartPmtMove_PopupMenu()
{
	OnPartPmtMove();
}

void SheetSubView::OnEditLoopLead_PopupMenu()
{
	OnEditLoopLead();
}

void SheetSubView::OnEditLoopStartPt_PopupMenu()
{
	OnEditLoopStartPt();
}

void SheetSubView::OnAutoSequence_PopupMenu()
{
	OnAutoSequence();
}

void SheetSubView::OnViewSequence_PopupMenu()
{
	OnViewSequence();
}

void SheetSubView::OnViewNC_PopupMenu()
{
	OnViewNC();
}

void SheetSubView::OnAddPatternFromDxf_PopupMenu()
{
	OnAddPatternFromDxf();
}

void SheetSubView::OnAddCanvasPattern2Task_PopupMenu()
{
	OnAddCanvasPattern2Task();
}

void SheetSubView::OnSelectCanvasPattern_PopupMenu()
{
	OnSelectCanvasPattern();
}

void SheetSubView::OnDeleteCanvasPattern_PopupMenu()
{
	OnDeleteCanvasPattern();
}

void SheetSubView::OnCopyCanvasPattern_PopupMenu()
{
	OnCopyCanvasPattern();
}

void SheetSubView::OnMoveCanvasPattern_PopupMenu()
{
	OnMoveCanvasPattern();
}

void SheetSubView::OnRotateCanvasPattern_PopupMenu()
{
	OnRotateCanvasPattern();
}

void SheetSubView::OnPartPmtAddPart()
{
	PartPlacementAddPanel::GetInstance()->DisplayPartList();
}

void SheetSubView::OnNestTaskParts()
{
	ClWindowFinder::GetCurrentView_2()->PostMessage(WM_NestTaskParts, 0, 0);
}

void SheetSubView::OnPartPmtAddDxf()
{
	PartItemListPtr pPartItems;
	map<LONGLONG, vector<pair<LONGLONG, Matrix2D>>> matrixData_of_sameShapePart;
	FolderNodePtr pSaveFolder;
	if (SheetViewHelper::LoadDxfAsPart(m_pSheet.get(), pPartItems, matrixData_of_sameShapePart, pSaveFolder)) {
		SwitchEditor(m_pPartLayoutEditor);
		m_pPartLayoutEditor->AddPartPmtFromDxfdwg(pPartItems, matrixData_of_sameShapePart, pSaveFolder);
	}
}

void SheetSubView::OnInPlaceEditPart()
{
	SheetViewHelper::InPlaceEditPart(m_pPartLayoutEditor.get());
}

void SheetSubView::OnPartPmtSelect()
{
	SwitchEditor(m_pPartLayoutEditor);
	m_pPartLayoutEditor->SetEditAction(EDIT_SELECTPRTPMT);
}

void SheetSubView::OnZoomAll()
{
	SheetViewHelper::ZoomAll(m_pSheet.get(), m_pViewPort);
}

void SheetSubView::OnZoomIn()
{
	SwitchEditor(m_pPartLayoutEditor);
	m_pPartLayoutEditor->SetViewAction(ACTION_ZOOM_IN);
	Invalidate();
}

void SheetSubView::OnZoomOut()
{
	SwitchEditor(m_pPartLayoutEditor);
	m_pPartLayoutEditor->SetViewAction(ACTION_ZOOM_OUT);
	Invalidate();
}

void SheetSubView::OnPartPmtCopy()
{
	SwitchEditor(m_pPartLayoutEditor);
	m_pPartLayoutEditor->SetEditAction(EDIT_COPYPRTPMT);
}

void SheetSubView::OnPartPmtDelete()
{
	SwitchEditor(m_pPartLayoutEditor);
	m_pPartLayoutEditor->SetEditAction(EDIT_DELETEPARTPMT);
}

void SheetSubView::OnPartPmtRotate()
{
	SwitchEditor(m_pPartLayoutEditor);
	m_pPartLayoutEditor->SetEditAction(EDIT_ROTATEPARTPMT);
}

void SheetSubView::OnPartPmtMove()
{
	SwitchEditor(m_pPartLayoutEditor);
	m_pPartLayoutEditor->SetEditAction(EDIT_MOVEPRTPMT);
}

void SheetSubView::OnPartPmtGrid()
{
	SwitchEditor(m_pPartLayoutEditor);
	m_pPartLayoutEditor->SetEditAction(EDIT_GRIDPRTPMT);
}

void SheetSubView::OnExplodeGrid()
{
	SwitchEditor(m_pPartLayoutEditor);
	m_pPartLayoutEditor->SetEditAction(EDIT_EXPLODE_GRID);
}

void SheetSubView::OnGenRemnantMat()
{
	RemnantMatInfoDlg dlg(this);
	dlg.Init(m_pSheet);
	if (dlg.DoModal() == IDC_BTN_SAVE) {
		CString str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_REM_MAT_SAVED);
		MessageBox(str, MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME), MB_OK | MB_ICONINFORMATION);
	}
}

void SheetSubView::OnMeasureDis()
{
	SwitchEditor(m_pPartLayoutEditor);
	m_pPartLayoutEditor->SetEditAction(MEASURE_PT_DISTANCE);
}

void SheetSubView::OnShowPartPmtTreeView()
{
	if (!PartPlacementViewBar::GetInstance()->IsVisible()) {
		PartPlacementViewBar::GetInstance()->DisplayPartPmt(m_pSheet->GetPartPlacements(), m_pSheet->GetPartInstances(), m_pPartLayoutEditor);
	}
	else {
		PartPlacementViewBar::GetInstance()->ShowBar(FALSE);
	}
}

void SheetSubView::OnUpdatePartPmtTreeView(CCmdUI* pCmdUI)
{
	if (m_pSheet->GetIsCanvas())
		pCmdUI->Enable(FALSE);
	else {
		pCmdUI->Enable(TRUE);
		pCmdUI->SetCheck(PartPlacementViewBar::GetInstance()->IsVisible());
	}
}

void SheetSubView::OnShowPartTopTreeView()
{
	if (!PartTopologyBar::GetInstance()->IsVisible()) {
		PartInstanceListPtr pPartInstances = m_pSheet->GetPartInstances();
		PartInstanceTopologyItemListPtr pPartInstanceTopologyItemList = PartPlacementManager::CalcPartTopStruct(pPartInstances, pPartInstances->GetLoopInstanceRect());
		PartTopologyBar::GetInstance()->DisplayPartTop(pPartInstanceTopologyItemList, m_pPartLayoutEditor);
	}
	else {
		PartTopologyBar::GetInstance()->ShowBar(FALSE);
	}
}

void SheetSubView::OnUpdatePartTopTreeView(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(PartTopologyBar::GetInstance()->IsVisible());
}

void SheetSubView::OnViewOffsetLoop()
{
	if (m_pOffPartPlacementList) {
		m_pOffPartPlacementList.reset();
	}
	else {
		m_pOffPartPlacementList.reset(new PartPlacementList);
		PartInstanceListPtr pNewPartInsts(new PartInstanceList);
		PartPlacementManager::OffsetPartPmts(m_pSheet->GetPartPlacements().get(), m_pSheet->GetPartInstances().get(), m_pOffPartPlacementList, pNewPartInsts);
	}

	Invalidate(TRUE);
}

void SheetSubView::OnUpdateOffsetLoop(CCmdUI* pCmdUI)
{
	if (m_pOffPartPlacementList)
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
}

void SheetSubView::OnViewNC()
{
	CString strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);
	if (m_pSheet->GetCutSequences()->size() == 0) { // 板材中有没有工序。
		if (MessageBox(MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_GEN_NC_NO_SEQ), strProductName, MB_YESNO | MB_ICONQUESTION) != IDYES)
			return;
	}
	SwitchEditor(m_pMacRouterEditor);
	SheetViewHelper::GenerateAndViewNcCode(m_pSheet, m_pMacRouterEditor);
}

void SheetSubView::OnConfigNcParam()
{
	ConfigNcParamDlg dlg(this);
	dlg.Init(m_pSheet->GetParamConfigID());
	dlg.DoModal();
}

void SheetSubView::OnAutoSequence()
{
	CString strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);
	PartInstanceListPtr pUnCutPartInstanceList = m_pSheet->GetCutSequences()->FilterUnSeqPartInst(m_pSheet->GetPartInstances());
	if (pUnCutPartInstanceList->size() == 0) { // 所有轮廓都加了工序。
		::MessageBox(m_hWnd, MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_ALL_INST_ASSIGN_SEQ), strProductName, MB_OK | MB_ICONINFORMATION);
		EnterCanvasLoopOrPartSelectAction();
	}
	else {
		set<LONGLONG> patternLoops_failedToOffset; // 此处记录偏移失败的几何轮廓。
		SheetViewHelper::AutoSequence(m_pSheet, patternLoops_failedToOffset);
		if (m_pSheet->GetIsCanvas()) // 进入几何特征选择动作。
			OnSelectCanvasPattern();
		else { // 进入零件实例选择动作。
			OnPartPmtSelect();

			// 在零件选择action中显示偏移失败的几何轮廓。
			if (patternLoops_failedToOffset.size() > 0) {
				MessageBox(MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_LoopHaveWrongToolSize), strProductName, MB_OK | MB_ICONWARNING);
				PartPlacementSelectActionPtr pPartPlacementSelectAction = boost::dynamic_pointer_cast<PartPlacementSelectAction>(m_pPartLayoutEditor->GetActionObject());
				LoopInstanceListPtr pAllLoopInstances = LoopInstanceManager::GetLoopInstanceList(m_pSheet->GetPartInstances());
				pPartPlacementSelectAction->GetWrongToolSizeLoopInstances()->Init(pAllLoopInstances.get(), patternLoops_failedToOffset);
			}
		}
		this->Invalidate(); // 及时更新板材视图中的切割路径，且如果接下来的操作会在单独的线程中访问数据库的话，就不会和主线程冲突。
	}
}

void SheetSubView::OnResetSequence()
{
	ResetSequenceDlg dlg(this);
	dlg.Init(m_pSheet);
	if (dlg.DoModal() == IDOK) {
		EnterCanvasLoopOrPartSelectAction();
		this->Invalidate(); // 及时更新板材视图中的切割路径，且如果接下来的操作会在单独的线程中访问数据库的话，就不会和主线程冲突。
	}
}

void SheetSubView::OnClearAllSequence()
{
	SheetViewHelper::ClearAllSequence(m_pSheet);
	EnterCanvasLoopOrPartSelectAction();
}

void SheetSubView::OnSetReferencePt()
{
	if (CanvasRefPtDataPtr pCanvasRefPtData = boost::dynamic_pointer_cast<CanvasRefPtData>(m_pSheet->GetRefPtData())) {
		CanvasReferencePointDlg dlg;
		dlg.Init(pCanvasRefPtData);
		if (dlg.DoModal() == IDOK) {
			this->Invalidate();
			EnterCanvasLoopOrPartSelectAction();
		}
	}
	else if (SheetRefPtDataPtr pSheetRefPtData = boost::dynamic_pointer_cast<SheetRefPtData>(m_pSheet->GetRefPtData())) {
		SheetReferencePointDlg dlg;
		dlg.Init(pSheetRefPtData);
		if (dlg.DoModal() == IDOK) {
			this->Invalidate();
			EnterCanvasLoopOrPartSelectAction();
		}
	}
}

void SheetSubView::OnSelectSequence()
{
	SwitchEditor(m_pSequenceEditor);
	m_pSequenceEditor->SetAction(SEQUENCE_SELECT);
}

void SheetSubView::OnSortSequence()
{
	SequenceSortDlg dlg;
	if (dlg.DoModal() == IDOK) {
		SwitchEditor(m_pSequenceEditor);
		if (dlg.IsSortFromStart())
			m_pSequenceEditor->SetAction(SEQUENCE_SORT_A);
		else
			m_pSequenceEditor->SetAction(SEQUENCE_SORT_B);
	}
}

void SheetSubView::OnReplaceHoleSeq()
{
	SwitchEditor(m_pSequenceEditor);
	m_pSequenceEditor->SetAction(SEQUENCE_REPLACE_HOLE_SEQ);
}

void SheetSubView::OnAddBdgeSeq()
{
	SwitchEditor(m_pSequenceEditor);
	m_pSequenceEditor->SetAction(SEQUENCE_ADD_BDGESEQ);
}

void SheetSubView::OnAddChainSeq()
{
	SwitchEditor(m_pSequenceEditor);
	m_pSequenceEditor->SetAction(SEQUENCE_ADD_CHAINSEQ);
}

void SheetSubView::OnAddGridSeq()
{
	SwitchEditor(m_pSequenceEditor);
	m_pSequenceEditor->SetAction(SEQUENCE_ADD_GRIDSEQ);
}

void SheetSubView::OnAddFlyCutSeq()
{
	LoopInstanceListPtr pSelectedLoopInstances = SheetViewHelper::GetSelectedLoopInstances(m_pCurEditor.get());
	if (pSelectedLoopInstances->size() > 0) { // 基于选中的轮廓创建飞切。
		SheetViewHelper::AddFlyCut2SelectedLoopInstances(pSelectedLoopInstances, m_pCurEditor.get(), m_pSequenceEditor.get());
	} else { // 通过action来创建飞切工序。
		SwitchEditor(m_pSequenceEditor);
		m_pSequenceEditor->SetAction(SEQUENCE_Add_FLYCUT);
	}
}

void SheetSubView::OnInsertChainNode()
{
	SwitchEditor(m_pSequenceEditor);
	m_pSequenceEditor->SetAction(SEQUENCE_INSERT_CHAINNODE);
}

void SheetSubView::OnDelChainNode()
{
	SwitchEditor(m_pSequenceEditor);
	m_pSequenceEditor->SetAction(SEQUENCE_DEL_CHAINNODE);
}

void SheetSubView::OnInsertResetInstruction()
{
	SwitchEditor(m_pSequenceEditor);
	m_pSequenceEditor->SetAction(SEQUENCE_ADD_RESET_INSTRUCT);
}

void SheetSubView::OnDelResetInstruction()
{
	SwitchEditor(m_pSequenceEditor);
	m_pSequenceEditor->SetAction(SEQUENCE_DEL_RESET_INSTRUCT);
}

void SheetSubView::OnBreakBdgeSeq()
{
	SwitchEditor(m_pSequenceEditor);
	m_pSequenceEditor->SetAction(SEQUENCE_BREAK_BDGESEQ);
}

void SheetSubView::OnBdgeStartLoop()
{
	SwitchEditor(m_pSequenceEditor);
	m_pSequenceEditor->SetAction(SEQUENCE_BDGE_START_CHANGE);
}

void SheetSubView::OnAddLoopSeq()
{
	SwitchEditor(m_pSequenceEditor);
	m_pSequenceEditor->SetAction(SEQUENCE_ADD_LOOPSEQ);
}

void SheetSubView::OnInsertLoopSeq()
{
	SwitchEditor(m_pSequenceEditor);
	m_pSequenceEditor->SetAction(SEQUENCE_INSERT_LOOPSEQ);
}

void SheetSubView::OnRemoveSequence()
{
	SwitchEditor(m_pSequenceEditor);
	m_pSequenceEditor->SetAction(SEQUENCE_REMOVE);
}

void SheetSubView::OnAddMovePoint()
{
	SwitchEditor(m_pSequenceEditor);
	m_pSequenceEditor->SetAction(SEQUENCE_ADD_MOV_PT);
}

void SheetSubView::OnDelMovePoint()
{
	SwitchEditor(m_pSequenceEditor);
	m_pSequenceEditor->SetAction(SEQUENCE_DEL_MOV_PT);
}

void SheetSubView::OnAutoRemCutline()
{
	SwitchEditor(m_pSequenceEditor);
	m_pSequenceEditor->SetAction(SEQUENCE_AUTO_REM_CUTLINE);
}

void SheetSubView::OnAddRemCutline()
{
	SwitchEditor(m_pSequenceEditor);
	m_pSequenceEditor->SetAction(SEQUENCE_ADD_REM_CUTLINE);
}

void SheetSubView::OnDelRemCutline()
{
	SwitchEditor(m_pSequenceEditor);
	m_pSequenceEditor->SetAction(SEQUENCE_DEL_REM_CUTLINE);
}

void SheetSubView::OnViewSequence()
{
	SwitchEditor(m_pSequenceEditor);
	m_pSequenceEditor->SetAction(SEQUENCE_VIEW);
}

void SheetSubView::OnShowSeqUnitPane()
{
	SwitchEditor(m_pSequenceEditor);
	if (!CutSequencePane::GetInstance()->IsVisible()) {
		CutSequencePane::GetInstance()->DisplaySeqUnit(m_pSheet->GetCutSequences(), m_pSequenceEditor);
	}
	else {
		CutSequencePane::GetInstance()->ShowBar(FALSE);
	}
}

void SheetSubView::OnUpdateSeqUnitPane(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(CutSequencePane::GetInstance()->IsVisible());
}

LRESULT SheetSubView::OnDisplayCurrentWorkModeOnStatusBar(WPARAM wParam, LPARAM lParam)
{
	CString strWorkMode = MultiLanguageMgr::GetInstance()->TranslateString(IDS_WORK_MODE);
	if (m_pSheet->GetIsCanvas()) {
		strWorkMode.Replace(_T("WILLBEREPLACED1"), MultiLanguageMgr::GetInstance()->TranslateString(IDS_Canvas_Mode));
	}
	else {
		strWorkMode.Replace(_T("WILLBEREPLACED1"), MultiLanguageMgr::GetInstance()->TranslateString(IDS_SHEET_MODE));
	}
	int iActionName = m_pCurEditor->GetActionObject()->GetActionName();
	strWorkMode.Replace(_T("WILLBEREPLACED2"), MultiLanguageMgr::GetInstance()->TranslateString(iActionName));
	UIManager::GetInstance()->GetStatusBar()->UpdateStatusBar(strWorkMode, ID_WorkMode_Pane);

	return 0;
}

LRESULT SheetSubView::OnUpdateSheetPreviewData(WPARAM wParam, LPARAM lParam)
{
	m_pSheet->SetPreviewData(SheetManager::CalcPreviewData(m_pSheet.get()));
	return 0;
}

LRESULT SheetSubView::OnChangeToolOfSelectedLoopInstances(WPARAM wParam, LPARAM lParam)
{
	int iToolIndex = wParam;
	LoopInstanceListPtr pSelectedLoopInstances = SheetViewHelper::GetSelectedLoopInstances(m_pCurEditor.get());
	if (pSelectedLoopInstances->size() > 0) {
		SheetViewHelper::ChangeToolOfSelectedLoopInstances(pSelectedLoopInstances, iToolIndex, m_pCurEditor.get(), m_pPartLayoutEditor.get());
	}
	else {
		SwitchEditor(m_pPartLayoutEditor);
		m_pPartLayoutEditor->ChangeToolOfSelectedLoopInstances(iToolIndex);
	}

	return 0;
}

void SheetSubView::AlignLoops(AlignLoopStyle emAlignStyle)
{
	pair<PatternListPtr, TextStructListPtr> selectedPatterns = SheetViewHelper::GetSelectedPatterns(m_pCurEditor.get());
	if (selectedPatterns.first->size() > 0) {
		SheetViewHelper::AlignSelectedLoops(selectedPatterns, emAlignStyle, m_pCurEditor.get(), m_pCanvasGeometryEditor.get());
	}
	else {
		SwitchEditor(m_pCanvasGeometryEditor);
		m_pCanvasGeometryEditor->AlignCanvasLoop(emAlignStyle);
	}
}

void SheetSubView::ChangeLoopCutSide(PatternLoopCutOffsetType emCutOffsetType)
{
	LoopInstanceListPtr pSelectedLoopInstances = SheetViewHelper::GetSelectedLoopInstances(m_pCurEditor.get());
	if (pSelectedLoopInstances->size() > 0) {
		SheetViewHelper::ChangeCutSideOfSelectedLoopInstances(pSelectedLoopInstances, emCutOffsetType, m_pCurEditor.get(), m_pPartLayoutEditor.get());
	} else {
		SwitchEditor(m_pPartLayoutEditor);
		m_pPartLayoutEditor->ChangeLoopCutOffset_ByAction(emCutOffsetType);
	}
}

void SheetSubView::ChangeLoopCutDirection(CutDirectionChangeType emCutDirectionChangeType)
{
	LoopInstanceListPtr pSelectedLoopInstances = SheetViewHelper::GetSelectedLoopInstances(m_pCurEditor.get());
	if (pSelectedLoopInstances->size() > 0) {
		SheetViewHelper::ChangeCutDirectionOfSelectedLoopInstances(pSelectedLoopInstances, emCutDirectionChangeType, m_pCurEditor.get(), m_pPartLayoutEditor.get());
	} else {
		SwitchEditor(m_pPartLayoutEditor);
		m_pPartLayoutEditor->ChangeLoopCutDirection_ByAction(emCutDirectionChangeType);
	}
}

void SheetSubView::EnterCanvasLoopOrPartSelectAction()
{
	if (m_pSheet->GetIsCanvas()) {
		CanvasGeometryEditAction::PostMessage2EnterCanvasLoopSelectAction();
	}
	else {
		PartPlacementEditAction::PostMessage2EnterPartPlacementSelectAction();
	}
}

END_CUTLEADER_NAMESPACE()

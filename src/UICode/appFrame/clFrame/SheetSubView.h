#pragma once

#include "CLeaderView.h"
#include "PrintWndBase.h"
#include "Return2ForegroundCamBar.h"
#include "clGeometryFeatureCommon.h"
#include "clCutFeatureCommon.h"
#include "Sheet.h"

DECLARE_CUTLEADER_CLASS(PartPlacementList)
DECLARE_CUTLEADER_CLASS(PartLayoutEditor)
DECLARE_CUTLEADER_CLASS(SequenceEditor)
DECLARE_CUTLEADER_CLASS(MacRouterEditor)
DECLARE_CUTLEADER_CLASS(Sheet)
DECLARE_CUTLEADER_CLASS(SheetDrawer)
DECLARE_CUTLEADER_CLASS(ReportData)
DECLARE_CUTLEADER_CLASS(ParamConfig)
DECLARE_CUTLEADER_CLASS(IMaterialSize)
DECLARE_CUTLEADER_CLASS(SequenceSimulateShapeList)
DECLARE_CUTLEADER_CLASS(CutSequenceDeleteCommand)
DECLARE_CUTLEADER_CLASS(ICommand)
DECLARE_CUTLEADER_CLASS(ConditionTypeList)
DECLARE_CUTLEADER_CLASS(ToolAssignConfigItem)
DECLARE_CUTLEADER_CLASS(CanvasGeometryEditor)
DECLARE_CUTLEADER_CLASS(SnapPtManager)
DECLARE_CUTLEADER_CLASS(LoopInstanceList)
DECLARE_CUTLEADER_CLASS(PatternList)
DECLARE_CUTLEADER_CLASS(TextStructList)

BEGIN_CUTLEADER_NAMESPACE()

// 板材视图。
class CLFRAME_Export SheetSubView : public CLeaderView, public PrintWndBase
{
	DECLARE_DYNCREATE(SheetSubView)

public:
	SheetSubView(void);
	~SheetSubView(void);

public: // inherit from the super class.
	virtual void OnDraw(CDC* pDC);
	virtual void UpdateUndoRedoDropdownList(CMFCRibbonBaseElement* pElem) override;
	virtual void ProcessAfterUndo(const ICommand* pCommand) override;
	virtual void ProcessAfterRedo(const ICommand* pCommand)override;

	virtual void PreparePrinting(CPrintInfo* pInfo);
	virtual void Print(CDC* pDC, CPrintInfo* pInfo);
	virtual CString GetWndText();
	virtual void SetWndFocus();

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();

	// mouse event handler
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short iDelta, CPoint pt);

	// keyboard event handler
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);


	/************************************************************************/
	// 右键菜单

	afx_msg void OnPartPmtAddPart_PopupMenu();
	afx_msg void OnPartPmtAddDxf_PopupMenu();

	afx_msg void OnInPlaceEditPart_PopupMenu();

	afx_msg void OnPartPmtSelect_PopupMenu();
	afx_msg void OnPartPmtDelete_PopupMenu();
	afx_msg void OnPartPmtCopy_PopupMenu();
	afx_msg void OnPartPmtRotate_PopupMenu();
	afx_msg void OnPartPmtMove_PopupMenu();

	afx_msg void OnEditLoopLead_PopupMenu();
	afx_msg void OnEditLoopStartPt_PopupMenu();

	afx_msg void OnAutoSequence_PopupMenu();
	afx_msg void OnViewSequence_PopupMenu();
	afx_msg void OnViewNC_PopupMenu();

	afx_msg void OnAddPatternFromDxf_PopupMenu();
	afx_msg void OnAddCanvasPattern2Task_PopupMenu();

	afx_msg void OnSelectCanvasPattern_PopupMenu();
	afx_msg void OnDeleteCanvasPattern_PopupMenu();
	afx_msg void OnCopyCanvasPattern_PopupMenu();
	afx_msg void OnMoveCanvasPattern_PopupMenu();
	afx_msg void OnRotateCanvasPattern_PopupMenu();
	/************************************************************************/


	/************************************************************************/
	// 底图上几何特征功能。

	afx_msg void OnAddPatternFromDxf();

	afx_msg void OnDrawCanvasLine();
	afx_msg void OnUpdateUI_DrawCanvasLine(CCmdUI* pCmdUI) { m_pSheet->GetIsCanvas() ? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE); }
	afx_msg void OnDrawCanvasArc();
	afx_msg void OnUpdateUI_DrawCanvasArc(CCmdUI* pCmdUI) { m_pSheet->GetIsCanvas() ? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE); }
	afx_msg void OnDrawCanvasPolyLine();
	afx_msg void OnDrawCanvasRect();
	afx_msg void OnDrawCanvasRectLine();
	afx_msg void OnDrawCanvasCircle();
	afx_msg void OnUpdateUI_DrawCanvasCircle(CCmdUI* pCmdUI) { m_pSheet->GetIsCanvas() ? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE); }
	afx_msg void OnDrawCanvasText();
	afx_msg void OnUpdateUI_DrawCanvasText(CCmdUI* pCmdUI) { m_pSheet->GetIsCanvas() ? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE); }

	afx_msg void OnSelectCanvasPattern();
	afx_msg void OnUpdateUI_SelectCanvasPattern(CCmdUI* pCmdUI) { m_pSheet->GetIsCanvas() ? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE); }
	afx_msg void OnDeleteCanvasPattern();

	afx_msg void OnCopyCanvasPattern();
	afx_msg void OnUpdateUI_CopyCanvasPattern(CCmdUI* pCmdUI) { m_pSheet->GetIsCanvas() ? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE); }
	afx_msg void OnMoveCanvasPattern();
	afx_msg void OnUpdateUI_MoveCanvasPattern(CCmdUI* pCmdUI) { m_pSheet->GetIsCanvas() ? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE); }
	afx_msg void OnRotateCanvasPattern();
	afx_msg void OnUpdateUI_RotateCanvasPattern(CCmdUI* pCmdUI) { m_pSheet->GetIsCanvas() ? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE); }
	afx_msg void OnGridCanvasPattern();

	afx_msg void OnAddCanvasPattern2Task();

	// 对齐轮廓。
	afx_msg void OnLeftAlignCanvasLoop();
	afx_msg void OnRightAlignCanvasLoop();
	afx_msg void OnTopAlignCanvasLoop();
	afx_msg void OnBottomAlignCanvasLoop();
	afx_msg void OnCenterAlignCanvasLoop();
	afx_msg void OnUpdateUI_CenterAlignCanvasLoop(CCmdUI* pCmdUI) { m_pSheet->GetIsCanvas() ? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE); }
	afx_msg void OnHorizontalCenterAlignCanvasLoop();
	afx_msg void OnVerticalCenterAlignCanvasLoop();
	/************************************************************************/


	/************************************************************************/
	// “零件布局”category下的命令。

	// “添加零件”面板。
	afx_msg void OnPartPmtAddPart();
	afx_msg void OnPartPmtAddDxf();
	afx_msg void OnNestTaskParts();
	afx_msg void OnUpdateUI_PartPmtAddPart(CCmdUI* pCmdUI) { m_pSheet->GetIsCanvas() ? pCmdUI->Enable(FALSE) : pCmdUI->Enable(TRUE); }
	afx_msg void OnUpdateUI_PartPmtAddDxf(CCmdUI* pCmdUI) { m_pSheet->GetIsCanvas() ? pCmdUI->Enable(FALSE) : pCmdUI->Enable(TRUE); }
	afx_msg void OnUpdateUI_NestTaskParts(CCmdUI* pCmdUI) { m_pSheet->GetIsCanvas() ? pCmdUI->Enable(FALSE) : pCmdUI->Enable(TRUE); }

	afx_msg void OnInPlaceEditPart();

	// “编辑零件布局”面板。
	afx_msg void OnPartPmtSelect();
	afx_msg void OnPartPmtCopy();
	afx_msg void OnPartPmtDelete();
	afx_msg void OnPartPmtRotate();
	afx_msg void OnPartPmtMove();
	afx_msg void OnPartPmtGrid();
	afx_msg void OnExplodeGrid();
	afx_msg void OnUpdateUI_PartPmtSelect(CCmdUI* pCmdUI) { m_pSheet->GetIsCanvas() ? pCmdUI->Enable(FALSE) : pCmdUI->Enable(TRUE); }
	afx_msg void OnUpdateUI_PartPmtCopy(CCmdUI* pCmdUI) { m_pSheet->GetIsCanvas() ? pCmdUI->Enable(FALSE) : pCmdUI->Enable(TRUE); }
	afx_msg void OnUpdateUI_PartPmtDelete(CCmdUI* pCmdUI) { m_pSheet->GetIsCanvas() ? pCmdUI->Enable(FALSE) : pCmdUI->Enable(TRUE); }
	afx_msg void OnUpdateUI_PartPmtRotate(CCmdUI* pCmdUI) { m_pSheet->GetIsCanvas() ? pCmdUI->Enable(FALSE) : pCmdUI->Enable(TRUE); }
	afx_msg void OnUpdateUI_PartPmtMove(CCmdUI* pCmdUI) { m_pSheet->GetIsCanvas() ? pCmdUI->Enable(FALSE) : pCmdUI->Enable(TRUE); }
	afx_msg void OnUpdateUI_PartPmtGrid(CCmdUI* pCmdUI) { m_pSheet->GetIsCanvas() ? pCmdUI->Enable(FALSE) : pCmdUI->Enable(TRUE); }
	afx_msg void OnUpdateUI_ExplodeGrid(CCmdUI* pCmdUI) { m_pSheet->GetIsCanvas() ? pCmdUI->Enable(FALSE) : pCmdUI->Enable(TRUE); }

	afx_msg void OnGenRemnantMat();
	afx_msg void OnUpdateUI_GenRemnantMat(CCmdUI* pCmdUI) { m_pSheet->GetIsCanvas() ? pCmdUI->Enable(FALSE) : pCmdUI->Enable(TRUE); }
	afx_msg void OnMeasureDis();

	// “显示比例”面板
	afx_msg void OnZoomAll();
	afx_msg void OnZoomIn();
	afx_msg void OnZoomOut();

	// “显示/隐藏”面板。
	afx_msg void OnShowPartPmtTreeView();
	afx_msg void OnUpdatePartPmtTreeView(CCmdUI* pCmdUI);

	// 用于调试。
	afx_msg void OnViewOffsetLoop();
	afx_msg void OnUpdateOffsetLoop(CCmdUI* pCmdUI);
	afx_msg void OnShowPartTopTreeView();
	afx_msg void OnUpdatePartTopTreeView(CCmdUI* pCmdUI);
	/************************************************************************/


	/************************************************************************/
	// “加工特征”category下的命令。

	afx_msg void OnAssignCutFeature();
	afx_msg void OnResetCutFeature();
	afx_msg void OnCheckInterfere();

	afx_msg void OnEditLoopTool();
	afx_msg void OnReAssignLoopTool();

	// start/end cut panel.
	afx_msg void OnEditLoopLead();
	afx_msg void OnReAssignLeadLine();
	afx_msg void ChangeLoopGap();
	afx_msg void OnEditLoopStartPt();
	afx_msg void OnReAssignStartCutPoint();
	afx_msg void OnSetLoopAsCutOutside();
	afx_msg void OnSetLoopAsCutInside();
	afx_msg void OnSetLoopAsCutOn();
	afx_msg void OnSwitchCutDirection();
	afx_msg void OnCcwCutDirection();
	afx_msg void OnCwCutDirection();

	// 微连接
	afx_msg void OnAddMicrojoint();
	afx_msg void OnReAssignMicroJoint();
	afx_msg void OnEditMicrojoint();
	afx_msg void OnDelMicrojoint();

	// 角特征。
	afx_msg void OnAddCorner();
	afx_msg void OnReAssignCornerFeature();
	afx_msg void OnEditCorner();
	afx_msg void OnDelCorner();
	/************************************************************************/


	/************************************************************************/
	// Commands under "Sequence and NC" category.

	// "Auto" panel
	afx_msg void OnAutoSequence();
	afx_msg void OnResetSequence();
	afx_msg void OnClearAllSequence();

	afx_msg void OnSelectSequence();
	afx_msg void OnRemoveSequence();
	afx_msg void OnSortSequence();
	afx_msg void OnReplaceHoleSeq();

	afx_msg void OnAddLoopSeq();
	afx_msg void OnInsertLoopSeq();

	// 桥接。
	afx_msg void OnAddBdgeSeq();
	afx_msg void OnUpdateUI_AddBdgeSeq(CCmdUI* pCmdUI) { m_pSheet->GetIsCanvas() ? pCmdUI->Enable(FALSE) : pCmdUI->Enable(TRUE); }
	afx_msg void OnBreakBdgeSeq();
	afx_msg void OnUpdateUI_BreakBdgeSeq(CCmdUI* pCmdUI) { m_pSheet->GetIsCanvas() ? pCmdUI->Enable(FALSE) : pCmdUI->Enable(TRUE); }
	afx_msg void OnBdgeStartLoop();
	afx_msg void OnUpdateUI_BdgeStartLoop(CCmdUI* pCmdUI) { m_pSheet->GetIsCanvas() ? pCmdUI->Enable(FALSE) : pCmdUI->Enable(TRUE); }

	// 连割。
	afx_msg void OnAddChainSeq();
	afx_msg void OnUpdateUI_AddChainSeq(CCmdUI* pCmdUI) { m_pSheet->GetIsCanvas() ? pCmdUI->Enable(FALSE) : pCmdUI->Enable(TRUE); }
	afx_msg void OnInsertChainNode();
	afx_msg void OnUpdateUI_InsertChainNode(CCmdUI* pCmdUI) { m_pSheet->GetIsCanvas() ? pCmdUI->Enable(FALSE) : pCmdUI->Enable(TRUE); }
	afx_msg void OnDelChainNode();
	afx_msg void OnUpdateUI_DelChainNode(CCmdUI* pCmdUI) { m_pSheet->GetIsCanvas() ? pCmdUI->Enable(FALSE) : pCmdUI->Enable(TRUE); }
	afx_msg void OnInsertResetInstruction();
	afx_msg void OnDelResetInstruction();

	// 田字切割。
	afx_msg void OnAddGridSeq();
	afx_msg void OnUpdateUI_AddGridSeq(CCmdUI* pCmdUI) { m_pSheet->GetIsCanvas() ? pCmdUI->Enable(FALSE) : pCmdUI->Enable(TRUE); }

	// 飞行切割。
	afx_msg void OnAddFlyCutSeq();

	afx_msg void OnAddMovePoint();
	afx_msg void OnUpdateUI_AddMovePoint(CCmdUI* pCmdUI) { m_pSheet->GetIsCanvas() ? pCmdUI->Enable(FALSE) : pCmdUI->Enable(TRUE); }
	afx_msg void OnDelMovePoint();
	afx_msg void OnUpdateUI_DelMovePoint(CCmdUI* pCmdUI) { m_pSheet->GetIsCanvas() ? pCmdUI->Enable(FALSE) : pCmdUI->Enable(TRUE); }

	// 余料切割线
	afx_msg void OnAutoRemCutline();
	afx_msg void OnUpdateUI_AutoRemCutline(CCmdUI* pCmdUI) { m_pSheet->GetIsCanvas() ? pCmdUI->Enable(FALSE) : pCmdUI->Enable(TRUE); }
	afx_msg void OnAddRemCutline();
	afx_msg void OnUpdateUI_AddRemCutline(CCmdUI* pCmdUI) { m_pSheet->GetIsCanvas() ? pCmdUI->Enable(FALSE) : pCmdUI->Enable(TRUE); }
	afx_msg void OnDelRemCutline();
	afx_msg void OnUpdateUI_DelRemCutline(CCmdUI* pCmdUI) { m_pSheet->GetIsCanvas() ? pCmdUI->Enable(FALSE) : pCmdUI->Enable(TRUE); }

	afx_msg void OnViewSequence();
	afx_msg void OnViewNC();
	afx_msg void OnConfigNcParam();

	// “显示/隐藏”面板。
	afx_msg void OnShowSeqUnitPane();
	afx_msg void OnUpdateSeqUnitPane(CCmdUI* pCmdUI);

	afx_msg void OnSetReferencePt();
	/************************************************************************/


	// 报表。
	afx_msg void OnPreviewTaskReport();
	afx_msg void OnPrintTaskReport();
	afx_msg void OnPreviewSheetReport();
	afx_msg void OnPrintSheetReport();
	afx_msg void OnPreviewPartLabel();
	afx_msg void OnPrintPartLabel();
	afx_msg void OnUpdateUI_PrintTaskReport(CCmdUI* pCmdUI) { m_pSheet->GetIsCanvas() ? pCmdUI->Enable(FALSE) : pCmdUI->Enable(TRUE); }
	afx_msg void OnUpdateUI_PrintSheetReport(CCmdUI* pCmdUI) { m_pSheet->GetIsCanvas() ? pCmdUI->Enable(FALSE) : pCmdUI->Enable(TRUE); }
	afx_msg void OnUpdateUI_PrintPartLabel(CCmdUI* pCmdUI) { m_pSheet->GetIsCanvas() ? pCmdUI->Enable(FALSE) : pCmdUI->Enable(TRUE); }

	// 修改这个板材相关的“参数配置”。
	// 注：
	//  1) 需要保存到数据库及更新“工艺编辑器”中的“参数配置”。
	afx_msg void SetParamConfig();

	afx_msg LRESULT OnEnterCanvasLoopSelectAction(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEnterCanvasPatternSelectAction_PreSelectOnePattern(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEnterCanvasLoopSelectAction_PreSelectLoopAndText(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnEnterSelectPartPlacementOrLoopInstanceAction(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEnterSelectPartPlacementAction(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEnterSelectLoopInstanceAction(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnQueryOtherLoopTools(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnHideOrShowCutPath(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDisplayCurrentWorkModeOnStatusBar(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUpdateSheetPreviewData(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnChangeToolOfSelectedLoopInstances(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

public:
	// 初始化板材视图。
	// 注：
	// 1）显示不同板材时，需要调用这个函数来来更新板材视图。这个函数需要刷新板材视图中所有和板材相关的数据。
	void InitSheetView(SheetPtr pSheet);

	SheetPtr GetSheet() const { return m_pSheet; }

	// 执行动作，在当前的板材子视图上摆放零件。
	void PlacePartOnCurrentSheetSubView(LONGLONG iPartID);

private:
	// 切换编辑器。
	// 注：
	//  1) 执行动作时需要做这样的检查，因为编辑器的变化会导致一些后台的处理。
	void SwitchEditor(IEditorPtr pNextEditor);

	// 对齐轮廓，各个对齐请求都汇集到该函数。
	void AlignLoops(AlignLoopStyle emAlignStyle);

	// 修改轮廓的内外切，各个内外切修改请求都汇集到该函数。
	void ChangeLoopCutSide(PatternLoopCutOffsetType emCutOffsetType);

	// 修改轮廓的切割方向，各个修改请求都汇集到该函数。
	void ChangeLoopCutDirection(CutDirectionChangeType emCutDirectionChangeType);

	// 转到选择底图轮廓或者板材零件的action。
	// 注：
	// 1) 进入这两个action时不会预选对象。
	void EnterCanvasLoopOrPartSelectAction();

private:
	SheetPtr m_pSheet;

	// the editors.
	CanvasGeometryEditorPtr m_pCanvasGeometryEditor;
	PartLayoutEditorPtr m_pPartLayoutEditor;
	SequenceEditorPtr m_pSequenceEditor;
	MacRouterEditorPtr m_pMacRouterEditor;

	// the sheet drawer.
	SheetDrawerPtr m_pSheetDrawer;

	// 偏移后的“零件放置”对象。
	PartPlacementListPtr m_pOffPartPlacementList;

	// the report data.
	ReportDataPtr m_pReportData;

	// 切割路径模拟信息。
	bool m_bHideSequenceSimulateShapes; // 是否隐藏
	SequenceSimulateShapeListPtr m_pSequenceSimulateShapes;

	// 时间戳，需要在OnDraw中根据最新修改时间戳实时更新。
	LONGLONG m_iLatestModifyTimeStamp_ofPartPlacement; // 板材中零件放置的修改时间戳。
	LONGLONG m_iLatestModifyTimeStamp_ofSequenceData; // 板材中工序的修改时间戳，要考虑参考点数据和工序集合的时间戳。

	// “零件布局编辑器”中执行的命令可能导致无效工序的删除，这个变量记录了这些信息。
	//  注：
	//  1）“零件布局编辑器”中的命令执行后，会触发OnDraw，接着就会检查会不会导致无效工序。
	//  2）这个变量记录了“零件布局编辑器”中的哪个命令导致了某次工序删除，该变量中的命令不会放入undo/redo菜单中，undo完该命令后需要设置为空。
	std::map<LONGLONG, CutSequenceDeleteCommandPtr> m_sequenceDeleteCommandData;

	SnapPtManagerPtr m_pSnapPtManager;

	// 退回到前台cam的面板。
	Return2ForegroundCamBar* m_pReturn2ForegroundCamBar;
};

END_CUTLEADER_NAMESPACE()

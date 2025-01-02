#pragma once

#include "CLeaderView.h"
#include "LeavePartInplaceEditBar.h"

DECLARE_CUTLEADER_CLASS(GeometryFeatureEditor)
DECLARE_CUTLEADER_CLASS(DrawAssistor)
DECLARE_CUTLEADER_CLASS(CutFeatureEditor)
DECLARE_CUTLEADER_CLASS(PartItem)
DECLARE_CUTLEADER_CLASS(PartCadData)
DECLARE_CUTLEADER_CLASS(SnapPtManager)

BEGIN_CUTLEADER_NAMESPACE()

class CLFRAME_Export PartView : public CLeaderView
{
	DECLARE_DYNCREATE(PartView)

public:
	PartView(void);
	~PartView(void);

public: // inherit from the super class.
	virtual void OnDraw(CDC* pDC);
	virtual void UpdateUndoRedoDropdownList(CMFCRibbonBaseElement* pElem);
	virtual void ProcessAfterUndo(const ICommand* pCommand) override;
	virtual void ProcessAfterRedo(const ICommand* pCommand)override;

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg BOOL OnEraseBkgnd( CDC *pDC );
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

	afx_msg void OnSelectPattern_PopupMenu();
	afx_msg void OnDeletePattern_PopupMenu();
	afx_msg void OnCopyPattern_PopupMenu();
	afx_msg void OnMovePattern_PopupMenu();
	afx_msg void OnRotatePattern_PopupMenu();

	afx_msg void OnEditLoopLead_PopupMenu();
	afx_msg void OnEditLoopStartPt_PopupMenu();
	/************************************************************************/


	/************************************************************************/
	// draw panel within geometry category.

	// line.
	afx_msg void OnDrawStandardline();
	afx_msg void OnDrawMultiplelines();
	afx_msg void OnDrawRectanglelines();

	// hole
	afx_msg void OnDrawRectangleHole();
	afx_msg void OnDrawTriangleHole();

	// round hole
	afx_msg void OnDrawRoundHole();
	afx_msg void OnDrawOBround();
	afx_msg void OnDrawEllipse();

	// arc
	afx_msg void OnDrawArcStandardarc();
	afx_msg void OnDrawArc3pointsarc();

	// D shape
	afx_msg void OnDrawSingleD();
	afx_msg void OnDrawDoubleD();

	// Round Corner
	afx_msg void OnDrawRCHole();
	afx_msg void OnDrawRCLines();

	// radius slot
	afx_msg void OnDrawRadSlot();

	// grid
	afx_msg void OnDrawPatternLine();
	afx_msg void OnDrawPatternArc();
	afx_msg void OnDrawPatternCircle();
	afx_msg void OnDrawPatternGrid();

	afx_msg void OnDrawText();
	/************************************************************************/


	/************************************************************************/
	// "edit" panel within geometry category.

	afx_msg void OnSelectPattern();
	afx_msg void OnDeletePattern();
	afx_msg void OnExplodeGridPat();
	afx_msg void OnCopyPattern();
	afx_msg void OnMovePattern();
	afx_msg void OnRotatePattern();
	afx_msg void OnOptimizePattern();
	afx_msg void OnOffsetLoop();

	afx_msg void OnLeftAlignLoop();
	afx_msg void OnRightAlignLoop();
	afx_msg void OnTopAlignLoop();
	afx_msg void OnBottomAlignLoop();
	afx_msg void OnCenterAlignLoop();
	afx_msg void OnHorizontalCenterAlignLoop();
	afx_msg void OnVerticalCenterAlignLoop();
	/************************************************************************/


	// "view" panel within geometry category.
	afx_msg void OnZoomAll();
	afx_msg void OnZoomIn();
	afx_msg void OnZoomOut();


	/************************************************************************/
	// CAM Feature category.

	afx_msg void OnAssignCutFeature();
	afx_msg void OnResetCutFeature();
	afx_msg void OnCheckInterfere();

	afx_msg void OnEditLoopTool();

	// start/end cut panel
	afx_msg void OnEditLoopLead();
	afx_msg void OnEditLoopStartPt();
	afx_msg void OnSetLoopAsCutOutside();
	afx_msg void OnSetLoopAsCutInside();
	afx_msg void OnSetLoopAsCutOn();
	afx_msg void OnSwitchCutDirection();
	afx_msg void OnCcwCutDirection();
	afx_msg void OnCwCutDirection();

	// micro joint panel
	afx_msg void OnAddMicrojoint();
	afx_msg void OnEditMicrojoint();
	afx_msg void OnDeleteMicrojoint();

	// Corner panel
	afx_msg void OnAddCornerFeature();
	afx_msg void OnEditCornerFeature();
	afx_msg void OnDeleteCornerFeature();
	/************************************************************************/


	// 修改这个零件相关的“参数配置”。
	// 注：
	//  1) 需要保存到数据库及更新“工艺编辑器”中的“参数配置”。
	afx_msg void SetParamConfig();

	afx_msg LRESULT OnEnterSelectGeometryAction(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEnterSelectGeometryAction_AndPreSelect(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDisplayCurrentWorkModeOnStatusBar(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

public:
	// 初始化视图。
	void Init();

	// 在指定位置显示LeavePartInplaceEditBar。
	void PositionAndShow_LeavePartInplaceEditBar();

	// get the part associated with this "PartView"
	PartItemPtr GetCurrentPart();

private:
	// 切换编辑器。
	// 注：
	//  1) 执行动作时需要做这样的检查，因为编辑器的变化会导致一些后台的处理。
	void SwitchEditor(IEditorPtr pNextEditor);

private:
	// the geometry editor
	GeometryFeatureEditorPtr m_pGeometryEditor;

	// we will only use this under Part mode.
	CutFeatureEditorPtr m_pCutFeatureEditor;

	SnapPtManagerPtr m_pSnapPtManager;

	// 退回到任务文档的面板。该面板总是被创建出来，只有在位编辑零件模式下面才显示。
	LeavePartInplaceEditBar* m_pLeavePartInplaceEditBar;
};

END_CUTLEADER_NAMESPACE()

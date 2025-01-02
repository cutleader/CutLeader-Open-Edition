#include "StdAfx.h"
#include "PartView.h"

#include "ValueInputManager.h"
#include "LineArc2DList.h"
#include "clUILibResource.h"
#include "IAction.h"
#include "CommandList.h"
#include "ICommand.h"
#include "CommandManager.h"
#include "EditorData.h"
#include "DataBaseManager.h"
#include "LogMgr.h"
#include "ProgressDlg.h"
#include "clBaseDataResource.h"
#include "XMenu.h"

#include "clExpertLibResource.h"
#include "ParamConfig.h"
#include "ParamConfigLoader.h"
#include "ParamConfigDlg.h"
#include "CLOptionInfo.h"
#include "DisplayParam.h"
#include "ImpExpConfig.h"
#include "ConfigItemLoader.h"

#include "PatternList.h"
#include "PatternLoopList.h"
#include "clGeometryFeatureResource.h"
#include "GeometryFeatureEditor.h"
#include "PatternPropertyBar.h"
#include "Part.h"
#include "clCutFeatureResource.h"
#include "CutFeatureEditor.h"
#include "LoopCutFeature.h"
#include "LoopCutFeatureList.h"
#include "LoopStartCutDataList.h"
#include "LoopStartCutData.h"
#include "MicroJointList.h"
#include "MicroJoint.h"
#include "LoopMicroJoint.h"
#include "LoopMicroJointList.h"
#include "LoopCornerData.h"
#include "LoopCornerDataList.h"
#include "LoopCutFeatureManager.h"
#include "LoopStartCutProcessor.h"
#include "CornerPropertyBar.h"
#include "MicroJointPropertyBar.h"
#include "LoopLeadPropertyBar.h"
#include "LoopToolEditPanel.h"
#include "TextStruct.h"
#include "TextStructList.h"
#include "SnapPt.h"
#include "PartCadData.h"
#include "PartItem.h"
#include "PartCamData.h"
#include "PartDocument.h"
#include "PartDrawer.h"
#include "UIManager.h"
#include "clFrameResource.h"
#include "SnapPtManager.h"
#include "clGeometryFeatureUIResource.h"
#include "GeometryOptimizeDlg.h"
#include "LoopOptTask.h"
#include "InterfereCheckTask.h"
#include "PartInplaceEditData.h"
#include "clFrameUIResource.h"

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(PartView, CLeaderView)

BEGIN_MESSAGE_MAP(PartView, CLeaderView)
	ON_WM_CREATE()
	ON_WM_SETCURSOR()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_DESTROY()

	// mouse event handler
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_CONTEXTMENU()
	ON_WM_MBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()

	// keyboard event handler
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()


	/************************************************************************/
	// 右键菜单

	ON_COMMAND(IDC_SelectPattern_PopupMenu, OnSelectPattern_PopupMenu)
	ON_COMMAND(IDC_DeletePattern_PopupMenu, OnDeletePattern_PopupMenu)
	ON_COMMAND(IDC_CopyPattern_PopupMenu, OnCopyPattern_PopupMenu)
	ON_COMMAND(IDC_MovePattern_PopupMenu, OnMovePattern_PopupMenu)
	ON_COMMAND(IDC_RotatePattern_PopupMenu, OnRotatePattern_PopupMenu)

	ON_COMMAND(IDC_CUTFEATURE_LOOP_LEAD_EDIT_PopupMenu, OnEditLoopLead_PopupMenu)
	ON_COMMAND(IDC_CUTFEATURE_LOOP_START_PT_PopupMenu, OnEditLoopStartPt_PopupMenu)
	/************************************************************************/


	/************************************************************************/
	// draw panel within geometry category.

	// line.
	ON_COMMAND(IDC_GEOM_DRAW_STDLINE, OnDrawStandardline)
	ON_COMMAND(IDC_GEOM_DRAW_MULTILINE, OnDrawMultiplelines)
	ON_COMMAND(IDC_GEOM_DRAW_RECTLINE, OnDrawRectanglelines)

	// hole
	ON_COMMAND(IDC_GEOM_DRAW_RECTHOLE, OnDrawRectangleHole)
	ON_COMMAND(IDC_GEOM_DRAW_TRIHOLE, OnDrawTriangleHole)

	// round hole
	ON_COMMAND(IDC_GEOM_DRAW_ROUNDHOLE, OnDrawRoundHole)
	ON_COMMAND(IDC_GEOM_DRAW_OBROUND, OnDrawOBround)
	ON_COMMAND(IDC_GEOM_DRAW_ELLIPSE, OnDrawEllipse)

	// arc
	ON_COMMAND(IDC_GEOM_DRAW_STDARC, OnDrawArcStandardarc)
	ON_COMMAND(IDC_GEOM_DRAW_ARC3PT, OnDrawArc3pointsarc)

	// D shape
	ON_COMMAND(IDC_GEOM_DRAW_SINGLED, OnDrawSingleD)
	ON_COMMAND(IDC_GEOM_DRAW_DOUBLED, OnDrawDoubleD)

	// Round Corner
	ON_COMMAND(IDC_GEOM_DRAW_RCHOLE, OnDrawRCHole)
	ON_COMMAND(IDC_GEOM_DRAW_RCLINEARC, OnDrawRCLines)

	// radius slot
	ON_COMMAND(IDC_GEOM_DRAW_RADSLOT, OnDrawRadSlot)

	// grid
	ON_COMMAND(IDC_GEOM_DRAW_PATLINE, OnDrawPatternLine)
	ON_COMMAND(IDC_GEOM_DRAW_PATGRID, OnDrawPatternGrid)
	ON_COMMAND(IDC_GEOM_DRAW_PATCIRCLE, OnDrawPatternCircle)
	ON_COMMAND(IDC_GEOM_DRAW_PATARC, OnDrawPatternArc)

	ON_COMMAND(IDC_DrawText, OnDrawText)
	/************************************************************************/


	// "View" panel within geometry category.
	ON_COMMAND(IDC_GEOM_VIEW_ZOOMALL, OnZoomAll)
	ON_COMMAND(IDC_GEOM_VIEW_ZOOMIN, OnZoomIn)
	ON_COMMAND(IDC_GEOM_VIEW_ZOOMOUT, OnZoomOut)


	/************************************************************************/
	// "Edit" panel within geometry category.

	ON_COMMAND(IDC_GEOM_EDIT_SELECTPAT, OnSelectPattern)
	ON_COMMAND(IDC_GEOM_EDIT_COPY, OnCopyPattern)
	ON_COMMAND(IDC_GEOM_EDIT_DELETE, OnDeletePattern)
	ON_COMMAND(IDC_GEOM_EDIT_EXPLODE, OnExplodeGridPat)

	ON_COMMAND(IDC_GEOM_EDIT_MOVE, OnMovePattern)
	ON_COMMAND(IDC_GEOM_EDIT_ROTATE, OnRotatePattern)
	ON_COMMAND(IDC_GEOM_EDIT_OPTIMIZE, OnOptimizePattern)
	ON_COMMAND(IDC_GEOM_OFFSET_LOOP, OnOffsetLoop)

	// 有关轮廓对齐。
	ON_COMMAND(IDC_AlignLoop_Left, OnLeftAlignLoop)
	ON_COMMAND(IDC_AlignLoop_Right, OnRightAlignLoop)
	ON_COMMAND(IDC_AlignLoop_Top, OnTopAlignLoop)
	ON_COMMAND(IDC_AlignLoop_Bottom, OnBottomAlignLoop)
	ON_COMMAND(IDC_AlignLoop_Center, OnCenterAlignLoop)
	ON_COMMAND(IDC_AlignLoop_HorizontalCenter, OnHorizontalCenterAlignLoop)
	ON_COMMAND(IDC_AlignLoop_VerticalCenter, OnVerticalCenterAlignLoop)
	/************************************************************************/


	/************************************************************************/
	// CAM Feature category.

	ON_COMMAND(IDC_CUTFEATURE_ASSIGN, OnAssignCutFeature)
	ON_COMMAND(IDC_CUTFEATURE_RESET, OnResetCutFeature)
	ON_COMMAND(IDC_CHECK_INTERFERE, OnCheckInterfere)

    // 轮廓刀具
    ON_COMMAND(IDC_CUTFEATURE_LOOPTOOL_EDIT, OnEditLoopTool)

	// start/end cut panel
	ON_COMMAND(IDC_CUTFEATURE_LOOP_LEAD_EDIT, OnEditLoopLead)
	ON_COMMAND(IDC_CUTFEATURE_LOOP_START_PT, OnEditLoopStartPt)
	ON_COMMAND(IDC_CutOutSide, OnSetLoopAsCutOutside)
	ON_COMMAND(IDC_CutInside, OnSetLoopAsCutInside)
	ON_COMMAND(IDC_CutOn, OnSetLoopAsCutOn)
	ON_COMMAND(IDC_SwitchCutDirection, OnSwitchCutDirection)
	ON_COMMAND(IDC_CcwCutDirection, OnCcwCutDirection)
	ON_COMMAND(IDC_CwCutDirection, OnCwCutDirection)

	// micro joint panel
	ON_COMMAND(IDC_CUTFEATURE_MJ_ADD, OnAddMicrojoint)
	ON_COMMAND(IDC_CUTFEATURE_MJ_EDIT, OnEditMicrojoint)
	ON_COMMAND(IDC_CUTFEATURE_MJ_DELETE, OnDeleteMicrojoint)

	// corner panel
	ON_COMMAND(IDC_CUTFEATURE_CORNER_ADD, OnAddCornerFeature)
	ON_COMMAND(IDC_CUTFEATURE_CORNER_EDIT, OnEditCornerFeature)
	ON_COMMAND(IDC_CUTFEATURE_CORNER_DELETE, OnDeleteCornerFeature)
	/************************************************************************/


	ON_COMMAND(IDC_PARAM_CONFIG, SetParamConfig)

	ON_MESSAGE(WM_EnterSelectGeometryAction, OnEnterSelectGeometryAction)
	ON_MESSAGE(WM_EnterSelectGeometryAction_AndPreSelect, OnEnterSelectGeometryAction_AndPreSelect)
	ON_MESSAGE(WM_displayCurrentWorkModeOnStatusBar, OnDisplayCurrentWorkModeOnStatusBar)
END_MESSAGE_MAP()

PartView::PartView(void)
{
	m_pSnapPtManager.reset(new SnapPtManager(m_pViewPort));
	m_pGeometryEditor.reset(new GeometryFeatureEditor(m_pViewPort));
	m_pCutFeatureEditor.reset(new CutFeatureEditor(m_pViewPort));
}

PartView::~PartView(void)
{
}

void PartView::OnDraw(CDC* pDC)
{
	// call base class to draw something.
	CLeaderView::OnDraw(pDC);


	/************************************************************************/
	// 绘制零件。

	// if still no part item associate with the document, do not draw.
	PartItemPtr pPartItem = GetCurrentPart();
	if (!pPartItem)
		return;

	// if no cad data in part item, do not draw.
	PartCadDataPtr pPartCadData = pPartItem->GetPart()->GetCadData();
	if (!pPartCadData)
		return;

	// 是否初始化了编辑器。
	if (!m_pCurEditor)
		return;

	// 要不要画工艺。
	BOOL bDrawCamFeature = FALSE;
	CString str;
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_CUTFEATURE);
	if (m_pCurEditor->GetEditorName() == str)
		bDrawCamFeature = TRUE;

	// 绘制零件。
	PartDrawer partDrawer(m_pViewPort);
	partDrawer.DrawPart(pPartItem.get(), bDrawCamFeature);
	/************************************************************************/


	// draw something based on different action.
	IActionPtr pAction = m_pCurEditor->GetActionObject();
	pAction->DrawBeforeFinish();

	this->PostMessage(WM_displayCurrentWorkModeOnStatusBar, 0, 0); // 显示当前工作模式。
	DisplayPromptMessageOfAction(pAction.get()); // 把这个action的提示消息显示在状态栏。

	// redraw the draw assist information.
	m_pSnapPtManager->DrawAssistInfo();

	// swap buffer
	m_pViewPort->SwapBuffers();
}

void PartView::UpdateUndoRedoDropdownList(CMFCRibbonBaseElement* pElem)
{
	// 当前的编辑器信息。
	EditorData editorData(this, m_pCurEditor->GetEditorName());

	switch (pElem->GetID())
	{
	case IDC_TAO_UNDO:
		{
			CMFCRibbonUndoButton* pUndo = DYNAMIC_DOWNCAST(CMFCRibbonUndoButton, pElem);
			pUndo->CleanUpUndoList();

			// get all undo items for this view.
			CommandListPtr pUndoCommands = CommandManager::GetInstance()->GetUndoCommands();
			m_pUndoCmdList = pUndoCommands->GetCmdByEditor(editorData);

			// get command names from end to begin.
			int iCount = m_pUndoCmdList->size();
			for (unsigned int i = 0; i < m_pUndoCmdList->size(); i++)
			{
				ICommandPtr pCommand = m_pUndoCmdList->at(m_pUndoCmdList->size()-i-1);
				pUndo->AddUndoAction(pCommand->GetCommandName());
			}
		}
		break;
	case IDC_TAO_REDO:
		{
			CMFCRibbonUndoButton* pRedo = DYNAMIC_DOWNCAST(CMFCRibbonUndoButton, pElem);
			pRedo->CleanUpUndoList();

			// get all redo items for this view.
			CommandListPtr pRedoCommands = CommandManager::GetInstance()->GetRedoCommands();
			m_pRedoCmdList = pRedoCommands->GetCmdByEditor(editorData);

			// get command names from end to begin.
			for (unsigned int i = 0; i < m_pRedoCmdList->size(); i++)
			{
				ICommandPtr pCommand = m_pRedoCmdList->at(m_pRedoCmdList->size()-i-1);
				pRedo->AddUndoAction(pCommand->GetCommandName());
			}
		}
		break;
	}
}

void PartView::ProcessAfterUndo(const ICommand* pCommand)
{
	// 重置捕捉状态。
	m_pSnapPtManager->ResetSnapStatus();

	Invalidate();
}

void PartView::ProcessAfterRedo(const ICommand* pCommand)
{
	// 重置捕捉状态。
	m_pSnapPtManager->ResetSnapStatus();

	Invalidate();
}

int PartView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	// init the OpenGL drawer.
	m_pViewPort->InitEnv(m_hWnd, 0.00001, 10000);

	// the drawing area.
	CRect wndRect;
	GetClientRect(&wndRect);

	// the rect of the geometry items.
	Rect2D geomRect(.0, 10.0, .0, 10.0);

	// set the drawing area.
	double dLeftBottomPtX = .0, dLeftBottomPtY = .0;
	double dXDirRange = m_pViewPort->GetFitAllParam(wndRect.Width(), wndRect.Height(), geomRect.GetXMin(), geomRect.GetXMax(),
													geomRect.GetYMin(), geomRect.GetYMax(), 1.2, dLeftBottomPtX, dLeftBottomPtY);
	m_pViewPort->SetDrawingArea(1.1 * dXDirRange, wndRect.Width(), wndRect.Height(), dLeftBottomPtX, dLeftBottomPtY);

	// 创建“离开在位编辑零件”面板。
	{
		m_pLeavePartInplaceEditBar = new LeavePartInplaceEditBar();
		m_pLeavePartInplaceEditBar->Create(IDD_clFrame_LeavePartInplaceEdit);
	}

	return CView::OnCreate(lpCreateStruct);
}

BOOL PartView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if (m_bPopupShow) {
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
	}
	else {
		SetCursor(NULL);
	}

	return FALSE;
}

void PartView::OnMButtonDown(UINT nFlags, CPoint point)
{
	m_OffSetPoint = point;

	CLeaderView::OnMButtonDown(nFlags, point);
}

BOOL PartView::OnEraseBkgnd( CDC *pDC )
{
	return TRUE;
}

void PartView::OnSize(UINT nType, int cx, int cy)
{
	CLeaderView::OnSize(nType, cx, cy);

	m_pViewPort->SetClientSize(cx, cy);

	if (PartInplaceEditData::GetInstance()->GetIsUnderInPlaceEditMode()) { // 在指定位置显示LeavePartInplaceEditBar。
		PositionAndShow_LeavePartInplaceEditBar(); 
	}
}

void PartView::OnDestroy()
{
	if (m_pLeavePartInplaceEditBar) {
		delete m_pLeavePartInplaceEditBar;
	}

	CLeaderView::OnDestroy();
}

BOOL PartView::OnMouseWheel(UINT nFlags, short iDelta, CPoint pt)
{
	double dRate = 1.25;
	if (iDelta > 0)
		dRate = 1/dRate;

	CPoint PtClient = pt;
	ScreenToClient(&PtClient);
	double dPointX = PtClient.x;
	double dPointY = PtClient.y;
	m_pViewPort->ZoomViewPort(dRate, dPointX, dPointY);

	// 重置捕捉状态。
	m_pSnapPtManager->ResetSnapStatus();

	Invalidate();

	return TRUE;
}

void PartView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// get the action object to perform left button down event.
	double dx = point.x;
	double dy = point.y;
	m_pViewPort->DToW(dx, dy);

	// 检查是不是点中了一个捕获点。
	SnapPtPtr pSnapPt = m_pSnapPtManager->GetSnapPt();
	if (pSnapPt)
	{
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

void PartView::OnLButtonUp(UINT nFlags, CPoint point)
{
	/************************************************************************/
	// get the action object to perform left button up event.

	double dx = point.x;
	double dy = point.y;
	m_pViewPort->DToW(dx, dy);

	// delegate to action object
	m_pCurEditor->LButtonUp(dx, dy);
	/************************************************************************/

	Invalidate();

	CLeaderView::OnLButtonUp(nFlags, point);
}

void PartView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	/************************************************************************/
	// get the action object to perform left button db-click event.

	//
	double dx = point.x;
	double dy = point.y;
	m_pViewPort->DToW(dx, dy);

	// delegate to action object
	m_pCurEditor->LButtonDBClk(dx, dy);
	/************************************************************************/

	Invalidate();

	CLeaderView::OnLButtonDblClk(nFlags, point);
}

void PartView::OnContextMenu(CWnd*, CPoint point)
{
	// 当前动作要是想处理右键消息，不弹出右键菜单。
	if (m_pCurEditor->GetActionObject()->IsProcessRightClickEvent()) {
		return;
	}

	m_bPopupShow = TRUE;

	// 创建菜单。
	XMenu xMenu;
	xMenu.CreatePopupMenu();

	// 创建各个菜单项。
	{
		CString str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SELECT_GEOM);
		xMenu.AppendMenu(MF_STRING, IDC_SelectPattern_PopupMenu, str.AllocSysString());
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_DELETE_GEOM_TIP);
		xMenu.AppendMenu(MF_STRING, IDC_DeletePattern_PopupMenu, str.AllocSysString());

		xMenu.AppendMenu(MF_SEPARATOR); // 分隔符

		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_COPY_GEOM_TIP);
		xMenu.AppendMenu(MF_STRING, IDC_CopyPattern_PopupMenu, str.AllocSysString());
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MOVE_GEOM_TIP);
		xMenu.AppendMenu(MF_STRING, IDC_MovePattern_PopupMenu, str.AllocSysString());
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_ROTATE_GEOM_TIP);
		xMenu.AppendMenu(MF_STRING, IDC_RotatePattern_PopupMenu, str.AllocSysString());

		xMenu.AppendMenu(MF_SEPARATOR); // 分隔符

		// 编辑工艺
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_EDIT_LOOP_LEAD);
		xMenu.AppendMenu(MF_STRING, IDC_CUTFEATURE_LOOP_LEAD_EDIT_PopupMenu, str.AllocSysString());
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_EDIT_LOOP_START_PT);
		xMenu.AppendMenu(MF_STRING, IDC_CUTFEATURE_LOOP_START_PT_PopupMenu, str.AllocSysString());
	}

	// 显示。
	xMenu.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, point.x, point.y, this);

	m_bPopupShow = FALSE;
}

void PartView::OnRButtonUp(UINT nFlags, CPoint point)
{
	// get the action object to perform left button down event.
	double dx = point.x;
	double dy = point.y;
	m_pViewPort->DToW(dx, dy);

	// delegate to action object
	m_pCurEditor->RButtonUp(dx, dy);

	Invalidate();

	CLeaderView::OnRButtonUp(nFlags, point);
}

void PartView::OnMouseMove(UINT nFlags, CPoint point)
{
	CLeaderView::OnMouseMove(nFlags, point);

	// when move cursor upon the view, set the focus.
	SetFocus();

	if (nFlags & MK_MBUTTON) {
		CPoint ptOffSet = point - m_OffSetPoint;
		m_pViewPort->PanViewPort(ptOffSet.x, ptOffSet.y);
		m_OffSetPoint = point;
	}
	else {
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

void PartView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CLeaderView::OnKeyDown(nChar, nRepCnt, nFlags);

	if (ValueInputManager::GetInstance()->IsInputS(nChar) && (::GetKeyState(VK_CONTROL) & 0x8000))
	{
		CLeaderDoc* pDoc = GetDocument();
		pDoc->OnSaveDocument();
	}
	else if (ValueInputManager::GetInstance()->IsInputZ(nChar) && (::GetKeyState(VK_CONTROL) & 0x8000))
	{
		OnEditUndo();
	}
	else
	{
		// delegate to action object
		m_pCurEditor->OnKeyDown(nChar, nRepCnt, nFlags);
	}

	Invalidate();
}

void PartView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CLeaderView::OnKeyUp(nChar, nRepCnt, nFlags);

	if (ValueInputManager::GetInstance()->IsInputS(nChar) && (::GetKeyState(VK_CONTROL) & 0x8000))
	{
		;
	}
	else if (ValueInputManager::GetInstance()->IsInputZ(nChar) && (::GetKeyState(VK_CONTROL) & 0x8000))
	{
		;
	}
	else
	{
		// delegate to action object
		m_pCurEditor->OnKeyUp( nChar, nRepCnt, nFlags);
	}
}

void PartView::OnSelectPattern_PopupMenu()
{
	OnSelectPattern();
}

void PartView::OnDeletePattern_PopupMenu()
{
	OnDeletePattern();
}

void PartView::OnCopyPattern_PopupMenu()
{
	OnCopyPattern();
}

void PartView::OnMovePattern_PopupMenu()
{
	OnMovePattern();
}

void PartView::OnRotatePattern_PopupMenu()
{
	OnRotatePattern();
}

void PartView::OnEditLoopLead_PopupMenu()
{
	OnEditLoopLead();
}

void PartView::OnEditLoopStartPt_PopupMenu()
{
	OnEditLoopStartPt();
}

LRESULT PartView::OnEnterSelectGeometryAction(WPARAM wParam, LPARAM lParam)
{
	SwitchEditor(m_pGeometryEditor);
	m_pGeometryEditor->SetEditAction(EDIT_SELECTPATTERN);

	return 0;
}

LRESULT PartView::OnEnterSelectGeometryAction_AndPreSelect(WPARAM wParam, LPARAM lParam)
{
	SwitchEditor(m_pGeometryEditor);
	m_pGeometryEditor->SetEditAction(EDIT_SELECTPATTERN);

	const PartCadData* pPartCadData = GetCurrentPart()->GetPart()->GetCadData().get();

	// 得到预选择的几何轮廓。
	PatternListPtr pPatterns_preSelect(new PatternList);
	{
		vector<LONGLONG>* pPatternIDs_preSelect = (vector<LONGLONG>*)wParam;
		for (unsigned int i = 0; i < pPatternIDs_preSelect->size(); i++)
		{
			IPatternPtr pPattern = pPartCadData->GetPatternList()->GetPatternFromID(pPatternIDs_preSelect->operator [](i));
			pPatterns_preSelect->push_back(pPattern);
		}
		if (pPatterns_preSelect->size() == 0)
		{
			LogMgr::DebugWarn(_T("653748"), this);
			return 0;
		}
		delete pPatternIDs_preSelect; // 这个资源由消息接收者释放。
	}

	// 得到预选择的文字对象。
	TextStructListPtr pTextStructs_preSelect(new TextStructList);
	{
		if (!lParam)
		{
			LogMgr::DebugWarn(_T("354782"), this);
			return 0;
		}
		vector<LONGLONG>* pTextStructIDs_preSelect = (vector<LONGLONG>*)lParam;
		for (unsigned int i = 0; i < pTextStructIDs_preSelect->size(); i++)
		{
			TextStructPtr pTextStruct = pPartCadData->GetTextStructs()->GetTextStructByID(pTextStructIDs_preSelect->operator [](i));
			pTextStructs_preSelect->push_back(pTextStruct);
		}
		delete pTextStructIDs_preSelect; // 这个资源由消息接收者释放。
	}

	// 启动几何轮廓选择动作，并预先选择指定的几何轮廓。
	m_pGeometryEditor->StartPatternSelectAction_andPreSelect(pPatterns_preSelect, pTextStructs_preSelect);

	return 0;
}

void PartView::SetParamConfig()
{
	// 得到这个零件相关的“参数配置”。
	ParamConfigPtr pParamConfig = m_pCutFeatureEditor->GetParamConfig();
	LONGLONG iParamConfigID = pParamConfig->GetID();

	ParamConfigDlg dlg(this);
	dlg.Init(iParamConfigID);
	if (dlg.DoModal() == IDOK)
	{
		// 更新零件用到的参数配置。
		DBConnectPtr pExpLibDBCon = DataBaseManager::GetInstance()->GetExpLibDBConnect();
		pParamConfig = ParamConfigLoader::LoadParamConfig(pExpLibDBCon, iParamConfigID);
		m_pCutFeatureEditor->SetParamConfig(pParamConfig);
	}
}

void PartView::Init()
{
	// prepare some data.
	PartItemPtr pPartItem = GetCurrentPart();
	PartCadDataPtr pPartCadData = pPartItem->GetPart()->GetCadData();
	PatternListPtr pPatList = pPartCadData->GetPatternList();
	PatternLoopListPtr pPatternLoopList = pPartCadData->GetPatternLoopList();
	LoopTopologyItemListPtr pLoopTopologyItems = pPartCadData->GetLoopTopologyItems_FW();
	PartCamDataPtr pPartCamData = pPartItem->GetCamData();
	LoopCutFeatureListPtr pLoopFeatureList = pPartCamData->GetLoopFeatureList();
	DBConnectPtr pExpLibDBCon = DataBaseManager::GetInstance()->GetExpLibDBConnect();
	ParamConfigPtr pParamConfig = ParamConfigLoader::LoadParamConfig(pExpLibDBCon, pPartCamData->GetParamConfigID());

	/************************************************************************/
	// 初始化各个“编辑器”。

	// init Geometry Editor.
	m_pGeometryEditor->SetPartCadData(pPartCadData);

	// init cam feature editor
	m_pCutFeatureEditor->SetPatternList(pPatList);
	m_pCutFeatureEditor->SetPatternLoopList(pPatternLoopList);
	m_pCutFeatureEditor->SetLoopTopologyItems(pLoopTopologyItems);
	m_pCutFeatureEditor->SetLoopFeatureList(pLoopFeatureList);
	m_pCutFeatureEditor->SetParamConfig(pParamConfig);

	// use the GeometryFeatureEditor as the current editor.
	m_pGeometryEditor->SetEditAction(EDIT_SELECTPATTERN);
	m_pCurEditor = m_pGeometryEditor;
	/************************************************************************/

	// 适配视图区域的显示。
	OnZoomAll();
}

PartItemPtr PartView::GetCurrentPart()
{
	CLeaderDoc* pDoc = GetDocument();
	PartDocument* pPartDoc = dynamic_cast<PartDocument*>(pDoc);
	return pPartDoc->GetPartItem();
}

void PartView::SwitchEditor(IEditorPtr pNextEditor)
{
	CString strCurEditorName = m_pCurEditor->GetEditorName();
	CString strNextEditorName = pNextEditor->GetEditorName();

	// 不需要切换编辑器。
	if (strCurEditorName == strNextEditorName)
		return;

	// 几个编辑器名称。
	CString strCadEditor, strCamEditor;
	strCadEditor = MultiLanguageMgr::GetInstance()->TranslateString(IDS_GEOMETRY);
	strCamEditor = MultiLanguageMgr::GetInstance()->TranslateString(IDS_CUTFEATURE);

	// 切换到了“图形编辑器”。
	if (strCurEditorName == strCamEditor && strNextEditorName == strCadEditor)
	{
		m_pCurEditor = m_pGeometryEditor;

		// 隐藏相关的面板。
		CornerPropertyBar::GetInstance()->ShowBar(FALSE);
		LoopLeadPropertyBar::GetInstance()->ShowBar(FALSE);
        LoopToolEditPanel::GetInstance()->ShowBar(FALSE);
		MicroJointPropertyBar::GetInstance()->ShowBar(FALSE);
	}

	// 切换到了“加工工艺编辑器”。
	if (strCurEditorName == strCadEditor && strNextEditorName == strCamEditor)
	{
		m_pCurEditor = m_pCutFeatureEditor;

		// 隐藏相关的面板。
		PatternPropertyBar::GetInstance()->ShowBar(FALSE);

		// 编辑器从“图形编辑器”切换到了“加工工艺编辑器”。
		if (strCurEditorName == strCadEditor)
		{
			// 清理对应“图形编辑器”的undo/redo列表。
			EditorData editorData(this, strCadEditor);
			CommandListPtr pUndoCommands = CommandManager::GetInstance()->GetUndoCommands();
			CommandListPtr pTmpUndoCommands = pUndoCommands->GetCmdByEditor(editorData);
			pUndoCommands->RemoveCommand(pTmpUndoCommands);
			CommandListPtr pRedoCommands = CommandManager::GetInstance()->GetRedoCommands();
			CommandListPtr pTmpRedoCommands = pRedoCommands->GetCmdByEditor(editorData);
			pRedoCommands->RemoveCommand(pTmpRedoCommands);

			// 处理“加工工艺”。
			PartItemPtr pPartItem = GetCurrentPart();
			PartCadDataPtr pPartCadData = pPartItem->GetPart()->GetCadData();
			PartCamDataPtr pPartCamData = pPartItem->GetCamData();
			LoopCutFeatureListPtr pLoopFeatureList = pPartCamData->GetLoopFeatureList();
			DBConnectPtr pExpLibDBCon = DataBaseManager::GetInstance()->GetExpLibDBConnect();
			ParamConfigPtr pParamConfig = ParamConfigLoader::LoadParamConfig(pExpLibDBCon, pPartCamData->GetParamConfigID());
			LoopCutFeatureManager::CheckIntegrity(pPartCadData->GetPatternLoopList().get(), pPartCadData->GetLoopTopologyItems(), pLoopFeatureList,
				pParamConfig.get(), TRUE, FALSE, FALSE, Matrix2D());
			for (unsigned int i = 0; i < pLoopFeatureList->size(); i++)
				LoopStartCutProcessor::UpdateCache(pLoopFeatureList->at(i)->GetLoopStartCutData());
		}
	}

	// 刷新视图。
	Invalidate();
}


void PartView::OnDrawStandardline()
{
	SwitchEditor(m_pGeometryEditor);
	m_pGeometryEditor->SetDrawAction(DRAW_LINE);
}

void PartView::OnDrawMultiplelines()
{
	SwitchEditor(m_pGeometryEditor);
	m_pGeometryEditor->SetDrawAction(DRAW_MULTILINE);
}

void PartView::OnDrawRectanglelines()
{
	SwitchEditor(m_pGeometryEditor);
	m_pGeometryEditor->SetDrawAction(DRAW_RECTLINES);
}

void PartView::OnDrawRectangleHole()
{
	SwitchEditor(m_pGeometryEditor);
	m_pGeometryEditor->SetDrawAction(DRAW_RECT_HOLE);
}

void PartView::OnDrawTriangleHole()
{
	SwitchEditor(m_pGeometryEditor);
	m_pGeometryEditor->SetDrawAction(DRAW_TR_HOLE);
}

void PartView::OnDrawArcStandardarc()
{
	SwitchEditor(m_pGeometryEditor);
	m_pGeometryEditor->SetDrawAction(DRAW_ARC_STD);
}

void PartView::OnDrawArc3pointsarc()
{
	SwitchEditor(m_pGeometryEditor);
	m_pGeometryEditor->SetDrawAction(DRAW_ARC_3PT);
}

void PartView::OnDrawRCHole()
{
	SwitchEditor(m_pGeometryEditor);
	m_pGeometryEditor->SetDrawAction(DRAW_RC_HOLE);
}

void PartView::OnDrawRCLines()
{
	SwitchEditor(m_pGeometryEditor);
	m_pGeometryEditor->SetDrawAction(DRAW_RC_LINES);
}

void PartView::OnDrawRoundHole()
{
	SwitchEditor(m_pGeometryEditor);
	m_pGeometryEditor->SetDrawAction(DRAW_ROUNDHOLE);
}

void PartView::OnDrawOBround()
{
	m_pGeometryEditor->SetDrawAction(DRAW_OBROUND);
	m_pCurEditor = m_pGeometryEditor;
}

void PartView::OnDrawEllipse()
{
	SwitchEditor(m_pGeometryEditor);
	m_pGeometryEditor->SetDrawAction(DRAW_ELLIPSE);
}

void PartView::OnDrawSingleD()
{
	SwitchEditor(m_pGeometryEditor);
	m_pGeometryEditor->SetDrawAction(DRAW_SINGLED);
}

void PartView::OnDrawDoubleD()
{
	SwitchEditor(m_pGeometryEditor);
	m_pGeometryEditor->SetDrawAction(DRAW_DOUBLED);
}

void PartView::OnDrawPatternLine()
{
	m_pGeometryEditor->SetDrawAction(DRAW_PATTERNLINE);
	m_pCurEditor = m_pGeometryEditor;
}

void PartView::OnDrawPatternArc()
{
	SwitchEditor(m_pGeometryEditor);
	m_pGeometryEditor->SetDrawAction(DRAW_PATTERNARC);
}

void PartView::OnDrawPatternCircle()
{
	SwitchEditor(m_pGeometryEditor);
	m_pGeometryEditor->SetDrawAction(DRAW_PATTERNCIRCLE);
}

void PartView::OnDrawPatternGrid()
{
	SwitchEditor(m_pGeometryEditor);
	m_pGeometryEditor->SetDrawAction(DRAW_PATTERNGRID);
}

void PartView::OnDrawRadSlot()
{
	SwitchEditor(m_pGeometryEditor);
	m_pGeometryEditor->SetDrawAction(DRAW_RADSLOT);
}

void PartView::OnDrawText()
{
	SwitchEditor(m_pGeometryEditor);
	m_pGeometryEditor->SetDrawAction(DRAW_Text);
}

void PartView::OnZoomAll()
{
	// 客户区。
	CRect wndRect;
	GetClientRect(&wndRect);

	PartCadDataPtr pPartCadData = GetCurrentPart()->GetPart()->GetCadData();
	PatternListPtr pPatList = pPartCadData->GetPatternList();
	if (pPatList->size() > 0)
	{
		// 图形的外围矩形。
		Rect2D geomRect = pPartCadData->GetPatternLoopList()->GetRectBox();

		// 设置绘图区域。
		double dLeftBottomPtX = .0, dLeftBottomPtY = .0;
		double dXDirRange = m_pViewPort->GetFitAllParam(wndRect.Width(), wndRect.Height(), geomRect.GetXMin(), geomRect.GetXMax(),
			geomRect.GetYMin(), geomRect.GetYMax(), 1.2, dLeftBottomPtX, dLeftBottomPtY);
		m_pViewPort->SetDrawingArea(1.1 * dXDirRange, wndRect.Width(), wndRect.Height(), dLeftBottomPtX, dLeftBottomPtY);
	}
	else
	{
		// 设置绘图区域。
		double dLeftBottomPtX = .0, dLeftBottomPtY = .0;
		double dXDirRange = m_pViewPort->GetFitAllParam(wndRect.Width(), wndRect.Height(), 0, 50.0, 0, 50.0, 1.2, dLeftBottomPtX, dLeftBottomPtY);
		m_pViewPort->SetDrawingArea(1.1 * dXDirRange, wndRect.Width(), wndRect.Height(), dLeftBottomPtX, dLeftBottomPtY);
	}

	Invalidate();
}

void PartView::OnZoomIn()
{
	SwitchEditor(m_pGeometryEditor);
	m_pGeometryEditor->SetViewAction(ACTION_ZOOM_IN);

	Invalidate();
}

void PartView::OnZoomOut()
{
	SwitchEditor(m_pGeometryEditor);
	m_pGeometryEditor->SetViewAction(ACTION_ZOOM_OUT);

	Invalidate();
}

void PartView::OnSelectPattern()
{
	SwitchEditor(m_pGeometryEditor);
	m_pGeometryEditor->SetEditAction(EDIT_SELECTPATTERN);
}

void PartView::OnDeletePattern()
{
	SwitchEditor(m_pGeometryEditor);
	m_pGeometryEditor->SetEditAction(EDIT_DELETE);

	Invalidate();
}

void PartView::OnExplodeGridPat()
{
	SwitchEditor(m_pGeometryEditor);
	m_pGeometryEditor->SetEditAction(EDIT_EXPLODE);

	Invalidate();
}

void PartView::OnCopyPattern()
{
	SwitchEditor(m_pGeometryEditor);
	m_pGeometryEditor->SetEditAction(EDIT_COPY);
}

void PartView::OnMovePattern()
{
	SwitchEditor(m_pGeometryEditor);
	m_pGeometryEditor->SetEditAction(EDIT_MOVE);
}

void PartView::OnRotatePattern()
{
	SwitchEditor(m_pGeometryEditor);
	m_pGeometryEditor->SetEditAction(EDIT_ROTATE);
}

void PartView::OnOptimizePattern()
{
	// get the pattern and loop list of the part.
	PartItemPtr pPartItem = GetCurrentPart();
	PartCadDataPtr pPartCadData = pPartItem->GetPart()->GetCadData();
	PatternListPtr pPatList = pPartCadData->GetPatternList();
	PatternLoopListPtr pPatternLoopList = pPartCadData->GetPatternLoopList();

	// 参数设置。
	ImpExpConfigPtr pImpExpConfig = CLOptionInfo::GetInstance()->GetImpExpConfig();
	GeometryOptimizeDlg dlg;
	dlg.SetdAllowOffset(pImpExpConfig->GetAllowOffset());
	if (dlg.DoModal() != IDOK)
		return;

	/************************************************************************/
	// create a task to optimize the loop.

	// init the task.
	double dAllowOffset = dlg.GetdAllowOffset();
	LoopOptTaskPtr pLoopOptTask(new LoopOptTask(pPatList, pPatternLoopList, dAllowOffset));

	// display the progress of the task.
	CString str;
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_OPT_TASK);
	ProgressDlg progressDlg;
	progressDlg.Init(str, pLoopOptTask);
	progressDlg.DoModal();
	/************************************************************************/

	// 通知图形集合的所有观察者。
	pPatList->NotifyAllObserver(INVALID_ID, CComVariant());

	// 设置当前动作为“空动作”。
	m_pGeometryEditor->SetDummyAction();
	Invalidate();
}

void PartView::OnOffsetLoop()
{
	SwitchEditor(m_pGeometryEditor);
	m_pGeometryEditor->SetEditAction(EDIT_OFFSET);
}

void PartView::OnLeftAlignLoop()
{
	SwitchEditor(m_pGeometryEditor);
	m_pGeometryEditor->AlignLoop(AlignLoop_Left);
}

void PartView::OnRightAlignLoop()
{
	SwitchEditor(m_pGeometryEditor);
	m_pGeometryEditor->AlignLoop(AlignLoop_Right);
}

void PartView::OnTopAlignLoop()
{
	SwitchEditor(m_pGeometryEditor);
	m_pGeometryEditor->AlignLoop(AlignLoop_Top);
}

void PartView::OnBottomAlignLoop()
{
	SwitchEditor(m_pGeometryEditor);
	m_pGeometryEditor->AlignLoop(AlignLoop_Bottom);
}

void PartView::OnCenterAlignLoop()
{
	SwitchEditor(m_pGeometryEditor);
	m_pGeometryEditor->AlignLoop(AlignLoop_Center);
}

void PartView::OnHorizontalCenterAlignLoop()
{
	SwitchEditor(m_pGeometryEditor);
	m_pGeometryEditor->AlignLoop(AlignLoop_HorizontalCenter);
}

void PartView::OnVerticalCenterAlignLoop()
{
	SwitchEditor(m_pGeometryEditor);
	m_pGeometryEditor->AlignLoop(AlignLoop_VerticalCenter);
}

void PartView::OnAssignCutFeature()
{
	// 切换编辑器。
	SwitchEditor(m_pCutFeatureEditor);
	m_pCutFeatureEditor->SetDummyAction();

	// 给零件分配加工工艺。
	DBConnectPtr pExpLibDBCon = DataBaseManager::GetInstance()->GetExpLibDBConnect();
	PartItemPtr pPartItem = GetCurrentPart();
	PartCadDataPtr pPartCadData = pPartItem->GetPart()->GetCadData();
	PartCamDataPtr pPartCamData = pPartItem->GetCamData();
	ParamConfigPtr pParamConfig = ParamConfigLoader::LoadParamConfig(pExpLibDBCon, pPartCamData->GetParamConfigID());
	LoopCutFeatureManager::CheckIntegrity(pPartCadData->GetPatternLoopList().get(), pPartCadData->GetLoopTopologyItems(), pPartCamData->GetLoopFeatureList(),
		pParamConfig.get(), FALSE, TRUE, TRUE, Matrix2D());

	// show message to user.
	CString str, strProductName;
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_ASSIGN_CUTFEATURE);
	strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);
	MessageBox(str, strProductName, MB_OK | MB_ICONINFORMATION);
}

void PartView::OnResetCutFeature()
{
	// 切换编辑器。
	SwitchEditor(m_pCutFeatureEditor);
	m_pCutFeatureEditor->SetDummyAction();

	CString str, strProductName;
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_RESET_CUTFEATURE);
	strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);
	if (MessageBox(str, strProductName, MB_YESNO | MB_ICONQUESTION) == IDYES)
	{
		PartItemPtr pPartItem = GetCurrentPart();
		PartCadDataPtr pPartCadData = pPartItem->GetPart()->GetCadData();
		PartCamDataPtr pPartCamData = pPartItem->GetCamData();
		DBConnectPtr pExpLibDBCon = DataBaseManager::GetInstance()->GetExpLibDBConnect();
		ParamConfigPtr pParamConfig = ParamConfigLoader::LoadParamConfig(pExpLibDBCon, pPartCamData->GetParamConfigID());
		LoopCutFeatureManager::ResetLoopFeature(pPartCadData->GetPatternLoopList(), pPartCadData->GetLoopTopologyItems(), pPartCamData->GetLoopFeatureList(), pParamConfig.get(), Matrix2D());

		// notify all observers.
		pPartCamData->GetLoopFeatureList()->NotifyAllObserver(INVALID_ID, CComVariant());
	}
}

void PartView::OnCheckInterfere()
{
	// 切换编辑器。
	SwitchEditor(m_pCutFeatureEditor);
	m_pCutFeatureEditor->SetDummyAction();

	/************************************************************************/
	// create a task to auto sequence.

	// init the task.
	PartItemPtr pPartItem = GetCurrentPart();
	PartCamDataPtr pPartCamData = pPartItem->GetCamData();
	LoopCutFeatureListPtr pLoopFeatureList = pPartCamData->GetLoopFeatureList();
	LoopTopologyItemListPtr pLoopTopologyItems = pPartItem->GetPart()->GetCadData()->GetLoopTopologyItems_FW();
	InterfereCheckTaskPtr pIntfCheckTask(new InterfereCheckTask(pLoopFeatureList, pLoopTopologyItems));

	// display the progress of the task.
	CString str;
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_INTF_CHECK_TASK);
	ProgressDlg progressDlg;
	progressDlg.Init(str, pIntfCheckTask);
	progressDlg.DoModal();
	/************************************************************************/

	Invalidate();
}

void PartView::OnEditLoopTool()
{
	SwitchEditor(m_pCutFeatureEditor);
	m_pCutFeatureEditor->SetAction(LOOPTOOL_EDIT);
}

void PartView::OnEditLoopLead()
{
	SwitchEditor(m_pCutFeatureEditor);
	m_pCutFeatureEditor->SetAction(LEAD_EDIT);
}

void PartView::OnEditLoopStartPt()
{
	SwitchEditor(m_pCutFeatureEditor);
	m_pCutFeatureEditor->SetAction(START_PT_CHANGE);
}

void PartView::OnSetLoopAsCutOutside()
{
	SwitchEditor(m_pCutFeatureEditor);
	m_pCutFeatureEditor->ChangeLoopCutOffset_ByAction(PatternLoopCutOffset_Outside);
}

void PartView::OnSetLoopAsCutInside()
{
	SwitchEditor(m_pCutFeatureEditor);
	m_pCutFeatureEditor->ChangeLoopCutOffset_ByAction(PatternLoopCutOffset_Inside);
}

void PartView::OnSetLoopAsCutOn()
{
	SwitchEditor(m_pCutFeatureEditor);
	m_pCutFeatureEditor->ChangeLoopCutOffset_ByAction(PatternLoopCutOffset_On);
}

void PartView::OnSwitchCutDirection()
{
	SwitchEditor(m_pCutFeatureEditor);
	m_pCutFeatureEditor->ChangeLoopCutDirection_ByAction(CutDirectionChange_Switch);
}

void PartView::OnCcwCutDirection()
{
	SwitchEditor(m_pCutFeatureEditor);
	m_pCutFeatureEditor->ChangeLoopCutDirection_ByAction(CutDirectionChange_CCW);
}

void PartView::OnCwCutDirection()
{
	SwitchEditor(m_pCutFeatureEditor);
	m_pCutFeatureEditor->ChangeLoopCutDirection_ByAction(CutDirectionChange_CW);
}

void PartView::OnAddMicrojoint()
{
	SwitchEditor(m_pCutFeatureEditor);
	m_pCutFeatureEditor->SetAction(MicroJoint_ADD);
}

void PartView::OnEditMicrojoint()
{
	SwitchEditor(m_pCutFeatureEditor);
	m_pCutFeatureEditor->SetAction(MicroJoint_EDIT);
}

void PartView::OnDeleteMicrojoint()
{
	SwitchEditor(m_pCutFeatureEditor);
	m_pCutFeatureEditor->SetAction(MicroJoint_DELETE);
}

void PartView::OnAddCornerFeature()
{
	SwitchEditor(m_pCutFeatureEditor);
	m_pCutFeatureEditor->SetAction(CORNER_ADD);

	// 显示角特征创建界面。
	PartCamDataPtr pPartCamData = GetCurrentPart()->GetCamData();
	DBConnectPtr pExpLibConnect = DataBaseManager::GetInstance()->GetExpLibDBConnect();
	CornerConfigItemPtr pCornerConfigItem = ConfigItemLoader::LoadCornerConfigItem(pExpLibConnect, pPartCamData->GetParamConfigID());
	CornerPropertyBar::GetInstance()->DisplayCornerData_4_create(pCornerConfigItem->GetDflCornerType(), pCornerConfigItem->GetDflCornerVal());
}

void PartView::OnEditCornerFeature()
{
	SwitchEditor(m_pCutFeatureEditor);
	m_pCutFeatureEditor->SetAction(CORNER_EDIT);
}

void PartView::OnDeleteCornerFeature()
{
	SwitchEditor(m_pCutFeatureEditor);
	m_pCutFeatureEditor->SetAction(CORNER_DELETE);
}

void PartView::PositionAndShow_LeavePartInplaceEditBar()
{
	CRect parentRect, winRect;
	this->GetWindowRect(parentRect);
	m_pLeavePartInplaceEditBar->GetWindowRect(winRect);
	m_pLeavePartInplaceEditBar->MoveWindow(parentRect.Width() - winRect.Width() - 1, parentRect.Height() - winRect.Height() - 1, winRect.Width(), winRect.Height());
	m_pLeavePartInplaceEditBar->SetParent(this);
	m_pLeavePartInplaceEditBar->ShowWindow(SW_SHOW);
}

LRESULT PartView::OnDisplayCurrentWorkModeOnStatusBar(WPARAM wParam, LPARAM lParam)
{
	CString strWorkMode = MultiLanguageMgr::GetInstance()->TranslateString(IDS_WORK_MODE);
	strWorkMode.Replace(_T("WILLBEREPLACED1"), MultiLanguageMgr::GetInstance()->TranslateString(IDS_PART_MODE));
	int iActionName = m_pCurEditor->GetActionObject()->GetActionName();
	strWorkMode.Replace(_T("WILLBEREPLACED2"), MultiLanguageMgr::GetInstance()->TranslateString(iActionName));
	UIManager::GetInstance()->GetStatusBar()->UpdateStatusBar(strWorkMode, ID_WorkMode_Pane);

	return 0;
}

END_CUTLEADER_NAMESPACE()

#include "StdAfx.h"
#include "GeometryFeatureEditor.h"

#include "baseConst.h"
#include "GlViewPort.h"
#include "ZoomBaseAction.h"
#include "ZoomInAction.h"
#include "ZoomOutAction.h"
#include "DummyAction.h"
#include "clBaseDataResource.h"
#include "LogMgr.h"

#include "clGeometryFeatureResource.h"
#include "IPattern.h"
#include "PatternList.h"
#include "PatternPropertyBar.h"
#include "ArcPatternEditAction.h"
#include "ArcPattern3PtEditAction.h"
#include "MultiLinePatternEditAction.h"
#include "RectLinesDrawAction.h"
#include "RoundCornerHolePatternEditAction.h"
#include "EllipsePatternEditAction.h"
#include "RoundCornerLineArcsDrawAction.h"
#include "RectHolePatternEditAction.h"
#include "CirclePatternEditAction.h"
#include "SingleDPatternEditAction.h"
#include "DoubleDPatternEditAction.h"
#include "LineArrayPatternEditAction.h"
#include "ArcArrayPatternEditAction.h"
#include "CircleArrayPatternEditAction.h"
#include "GridArrayPatternEditAction.h"
#include "TriangleHolePatternEditAction.h"
#include "OBroundPatternEditAction.h"
#include "RadSlotPatternEditAction.h"
#include "PatternCopyAction.h"
#include "PatternSelectAction.h"
#include "PatternExplodeAction.h"
#include "PatternDeleteAction.h"
#include "PatternMoveAction.h"
#include "PatternRotateAction.h"
#include "PatternLoopOffsetAction.h"
#include "PartCadData.h"
#include "TextStruct.h"
#include "TextStructList.h"
#include "TextDrawAction.h"
#include "PatternLoopAlignAction.h"

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

GeometryFeatureEditor::GeometryFeatureEditor(GlViewPortPtr pViewPort)
{
	m_pViewPort = pViewPort;
	m_pAction.reset(new DummyAction(m_pViewPort));

	{
		m_pTextParamActionBar = new TextParamActionBar();
		m_pTextParamActionBar->Create(IDD_clGeometryFeatureUI_TextParamActionBar);
		m_pTextParamActionBar->ShowWindow(SW_HIDE);

		// 修改文字尺寸。
		TextParamPtr pTextParam = m_pTextParamActionBar->GetTextParam_FW();
		pTextParam->SetTextHeight(20.0);
		pTextParam->SetTextWidth(20.0);
		m_pTextParamActionBar->InitTextParam(pTextParam);
	}
}

GeometryFeatureEditor::~GeometryFeatureEditor(void)
{
}

BOOL GeometryFeatureEditor::LButtonDown(double& dPtX, double& dPtY)
{
	if (m_pAction)
	{
		m_pAction->LButtonDown(dPtX, dPtY);

		// check whether we will enter other action.
		IActionPtr pNextAction = m_pAction->GetNextAction();
		if (pNextAction)
			m_pAction = pNextAction;
	}

	return TRUE;
}

BOOL GeometryFeatureEditor::LButtonUp(double& dPtX, double& dPtY)
{
	if (m_pAction)
	{
		m_pAction->LButtonUp(dPtX, dPtY);

		// check whether we will enter other action.
		IActionPtr pNextAction = m_pAction->GetNextAction();
		if (pNextAction)
			m_pAction = pNextAction;
	}

	return TRUE;
}

BOOL GeometryFeatureEditor::LButtonDBClk(double& dPtX, double& dPtY)
{
	if (m_pAction)
	{
		m_pAction->LButtonDBClk(dPtX, dPtY);

		// check whether we will enter other action.
		IActionPtr pNextAction = m_pAction->GetNextAction();
		if (pNextAction)
			m_pAction = pNextAction;
	}

	return TRUE;
}

BOOL GeometryFeatureEditor::RButtonUp(double& dPtX, double& dPtY)
{
	if (m_pAction)
	{
		m_pAction->RButtonUp(dPtX, dPtY);

		// check whether we will enter other action.
		IActionPtr pNextAction = m_pAction->GetNextAction();
		if (pNextAction)
			m_pAction = pNextAction;
	}

	return TRUE;
}

BOOL GeometryFeatureEditor::MovePoint(double& dPtX, double& dPtY)
{
	if (m_pAction)
	{
		m_pAction->MovePoint(dPtX, dPtY);

		// check whether we will enter other action.
		IActionPtr pNextAction = m_pAction->GetNextAction();
		if (pNextAction)
		{
			m_pAction = pNextAction;
		}
	}

	return TRUE;
}

BOOL GeometryFeatureEditor::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (m_pAction)
	{
		m_pAction->OnKeyDown(nChar, nRepCnt, nFlags);

		// check whether we will enter other action.
		IActionPtr pNextAction = m_pAction->GetNextAction();
		if (pNextAction)
			m_pAction = pNextAction;
	}

	return TRUE;
}

BOOL GeometryFeatureEditor::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (m_pAction)
	{
		m_pAction->OnKeyUp(nChar, nRepCnt, nFlags);

		// check whether we will enter other action.
		IActionPtr pNextAction = m_pAction->GetNextAction();
		if (pNextAction)
			m_pAction = pNextAction;
	}

	return TRUE;
}

CString GeometryFeatureEditor::GetEditorName() const
{
	CString str;
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_GEOMETRY);
	return str;
}

void GeometryFeatureEditor::StartPatternSelectAction_andPreSelect(PatternListPtr pPatterns_preSelect, TextStructListPtr pTextStructs_preSelect)
{
	PatternSelectAction* pPatternSelectAction = new PatternSelectAction(m_pPartCadData, m_pTextParamActionBar, m_pViewPort);
	pPatternSelectAction->SelectPats(pPatterns_preSelect, pTextStructs_preSelect);
	m_pAction.reset(pPatternSelectAction);
}

BOOL GeometryFeatureEditor::SetDrawAction(DRAW_ACTION_TYPE drawType)
{
	GlViewPortPtr pViewPort = m_pViewPort;

	// hide pattern prop bar.
	PatternPropertyBar::GetInstance()->ShowPane(FALSE, FALSE, TRUE);

	if (drawType == DRAW_LINE)
		m_pAction.reset(new LinePatternEditAction(m_pPartCadData, pViewPort));
	else if(drawType == DRAW_MULTILINE)
		m_pAction.reset(new MultiLinePatternEditAction(m_pPartCadData, pViewPort));
	else if(drawType == DRAW_RECTLINES)
		m_pAction.reset(new RectLinesDrawAction(m_pPartCadData, pViewPort));
	else if(drawType == DRAW_RECT_HOLE)
		m_pAction.reset(new RectHolePatternEditAction(m_pPartCadData, pViewPort));
	else if(drawType == DRAW_TR_HOLE)
		m_pAction.reset(new TriangleHolePatternEditAction(m_pPartCadData, pViewPort));
	else if(drawType == DRAW_ROUNDHOLE)
		m_pAction.reset(new CirclePatternEditAction(m_pPartCadData, pViewPort));
	else if(drawType == DRAW_OBROUND)
		m_pAction.reset(new OBroundPatternEditAction(m_pPartCadData, pViewPort));
	else if(drawType == DRAW_ELLIPSE)
		m_pAction.reset(new EllipsePatternEditAction(m_pPartCadData, pViewPort));
	else if(drawType == DRAW_ARC_STD)
		m_pAction.reset(new ArcPatternEditAction(m_pPartCadData, pViewPort));
	else if(drawType == DRAW_ARC_3PT)
		m_pAction.reset(new ArcPattern3PtEditAction(m_pPartCadData, pViewPort));
	else if(drawType == DRAW_SINGLED)
		m_pAction.reset(new SingleDPatternEditAction(m_pPartCadData, pViewPort));
	else if(drawType == DRAW_DOUBLED)
		m_pAction.reset(new DoubleDPatternEditAction(m_pPartCadData, pViewPort));
	else if(drawType == DRAW_RC_HOLE)
		m_pAction.reset(new RoundCornerHolePatternEditAction(m_pPartCadData, pViewPort));
	else if(drawType == DRAW_RC_LINES)
		m_pAction.reset(new RoundCornerLineArcsDrawAction(m_pPartCadData, pViewPort));
	else if(drawType == DRAW_PATTERNLINE)
		m_pAction.reset(new LineArrayPatternEditAction(m_pPartCadData, pViewPort));
	else if(drawType == DRAW_PATTERNARC)
		m_pAction.reset(new ArcArrayPatternEditAction(m_pPartCadData, pViewPort));
	else if(drawType == DRAW_PATTERNCIRCLE)
		m_pAction.reset(new CircleArrayPatternEditAction(m_pPartCadData, pViewPort));
	else if(drawType == DRAW_PATTERNGRID)
		m_pAction.reset(new GridArrayPatternEditAction(m_pPartCadData, pViewPort));
	else if(drawType == DRAW_RADSLOT)
		m_pAction.reset(new RadSlotPatternEditAction(m_pPartCadData, pViewPort));
	else if (drawType == DRAW_Text)
		m_pAction.reset(new TextDrawAction(m_pPartCadData, m_pTextParamActionBar, pViewPort));
	else
		LogMgr::DebugWarn(_T("915396"));

	return TRUE;
}

BOOL GeometryFeatureEditor::SetViewAction(VIEW_ACTION_TYPE viewType)
{
	// hide pattern prop bar.
	PatternPropertyBar::GetInstance()->ShowPane(FALSE, FALSE, TRUE);

	if (viewType == ACTION_ZOOM_IN)
	{
		m_pAction.reset(new ZoomInAction(m_pViewPort));
	}
	else if(viewType == ACTION_ZOOM_OUT)
	{
		m_pAction.reset(new ZoomOutAction(m_pViewPort));
	}

	return TRUE;
}

BOOL GeometryFeatureEditor::SetEditAction(EDIT_ACTION_TYPE editType)
{
	GlViewPortPtr pViewPort = m_pViewPort;

	// 先隐藏所有panel。
	m_pTextParamActionBar->HideActionBar();
	PatternPropertyBar::GetInstance()->ShowPane(FALSE, FALSE, TRUE);

	if (editType == EDIT_SELECTPATTERN) {
		m_pAction.reset(new PatternSelectAction(m_pPartCadData, m_pTextParamActionBar, pViewPort));
		return TRUE;
	}

	pair<PatternListPtr, TextStructListPtr> selectedPatterns = GetSelectedPatterns();
	if (editType == EDIT_COPY) {
		m_pAction.reset(new PatternCopyAction(m_pPartCadData, pViewPort, selectedPatterns.first, selectedPatterns.second));
	}
	else if (editType == EDIT_DELETE) {
		PatternDeleteActionPtr pPatternDeleteAction(new PatternDeleteAction(m_pPartCadData, pViewPort, selectedPatterns.first, selectedPatterns.second));
		if (selectedPatterns.first->size() > 0) { // 删除选中的图元，之后不需要进入删除action。
			pPatternDeleteAction->DeleteSelectedPats();
		}
		else {
			m_pAction = pPatternDeleteAction;
		}
	}
	else if (editType == EDIT_EXPLODE) {
		PatternExplodeActionPtr pExplodePatAction(new PatternExplodeAction(m_pPartCadData, pViewPort));
		PatternListPtr pSelectedPats = selectedPatterns.first;
		if (pSelectedPats->size() == 1 && (pSelectedPats->at(0)->IsArrayPattern() || pSelectedPats->at(0)->IsStdPattern())) { // selected a std/array pattern, explode it.
			pExplodePatAction->ExplodePat(pSelectedPats->at(0));
		}
		m_pAction = pExplodePatAction;
	}
	else if (editType == EDIT_MOVE) {
		m_pAction.reset(new PatternMoveAction(m_pPartCadData, pViewPort, selectedPatterns.first, selectedPatterns.second));
	}
	else if (editType == EDIT_ROTATE)
	{
		// cannot rotate array patterns.
		for (unsigned int i = 0; i < selectedPatterns.first->size(); i++) {
			if (selectedPatterns.first->at(i)->IsArrayPattern()) {
				CString strMsg = MultiLanguageMgr::GetInstance()->TranslateString(IDS_OUTPUT_ROTATEPAT_ARRAY);
				MessageBox(m_pViewPort->GetWnd(), strMsg, MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME), MB_OK | MB_ICONWARNING);
				return TRUE;
			}
		}

		m_pAction.reset(new PatternRotateAction(m_pPartCadData, pViewPort, selectedPatterns.first, selectedPatterns.second));
	}
	else if (editType == EDIT_OFFSET) {
		m_pAction.reset(new PatternLoopOffsetAction(m_pPartCadData, pViewPort));
	}

	return TRUE;
}

void GeometryFeatureEditor::AlignLoop(AlignLoopStyle emAlignLoopStyle)
{
	pair<PatternListPtr, TextStructListPtr> selectedPatterns = GetSelectedPatterns();
	PatternLoopAlignActionPtr pPatternLoopAlignAction(new PatternLoopAlignAction(m_pPartCadData, m_pViewPort, selectedPatterns.first, selectedPatterns.second, emAlignLoopStyle));
	if (selectedPatterns.first->size() > 0 && pPatternLoopAlignAction->AlignLoop()) { // 此时成功对齐了轮廓，进入轮廓选择action了。
		PatternEditActionBase::PostMessage2EnterPatternSelectAction(selectedPatterns.first.get(), selectedPatterns.second.get());
	}
	else {
		m_pAction = pPatternLoopAlignAction;
	}
}

pair<PatternListPtr, TextStructListPtr> GeometryFeatureEditor::GetSelectedPatterns()
{
	pair<PatternListPtr, TextStructListPtr> selectedPatterns;

	// 设置初始值。
	{
		PatternListPtr pPatterns(new PatternList);
		TextStructListPtr pTextStructs(new TextStructList);
		selectedPatterns.first = pPatterns;
		selectedPatterns.second = pTextStructs;
	}

	PatternSelectActionPtr pPatternSelectAction = boost::dynamic_pointer_cast<PatternSelectAction>(m_pAction);
	if (pPatternSelectAction) {
		TextStructListPtr pSelectedTextStructs(new TextStructList);
		PatternListPtr pSelectedPatterns = pPatternSelectAction->GetSelectedPats(pSelectedTextStructs);
		selectedPatterns.first = pSelectedPatterns;
		selectedPatterns.second = pSelectedTextStructs;
	}

	return selectedPatterns;
}

END_CUTLEADER_NAMESPACE()

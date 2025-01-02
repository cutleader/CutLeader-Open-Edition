#include "StdAfx.h"
#include "CanvasGeometryEditor.h"

#include "GlViewPort.h"
#include "ZoomInAction.h"
#include "ZoomOutAction.h"
#include "DummyAction.h"
#include "clBaseDataResource.h"
#include "LogMgr.h"
#include "ClFolderHelper.h"
#include "ClWindowFinder.h"

#include "ClOptionInfo.h"
#include "ToolAssignConfigItem.h"
#include "ParamConfig.h"

#include "CanvasPatternSelectAction.h"
#include "CanvasLinePatternEditAction.h"
#include "CanvasCirclePatternEditAction.h"
#include "clGeometryFeatureResource.h"
#include "CanvasPatternDeleteAction.h"
#include "clCanvasGeometryUIResource.h"
#include "PartCadData.h"
#include "PartPlacement.h"
#include "Part.h"
#include "CanvasMultiLinePatternEditAction.h"
#include "CanvasArcPatternEditAction.h"
#include "CanvasRectPatternEditAction.h"
#include "CanvasPatternCopyAction.h"
#include "PatternList.h"
#include "PatternLoopList.h"
#include "CanvasPatternMoveAction.h"
#include "CanvasPatternRotateAction.h"
#include "AddPatternsFromDxfdwgAction.h"
#include "AddPartFromCanvasAction.h"
#include "CanvasTextDrawAction.h"
#include "TextStructList.h"
#include "CanvasRectLinePatternEditAction.h"
#include "CanvasGeometryHelper.h"
#include "CanvasPatternGridAction.h"
#include "GeometryFeatureHelper.h"
#include "CanvasPatternPropertyBar.h"
#include "CanvasLoopAlignAction.h"
#include "clPartLayoutResource.h"
#include "LoopInstanceList.h"
#include "LoopInstanceManager.h"
#include "LoopProcessor.h"

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

CanvasGeometryEditor::CanvasGeometryEditor( GlViewPortPtr pViewPort)
{
	m_pViewPort = pViewPort;
	m_pAction.reset( new DummyAction( m_pViewPort ) );

	{
		m_pSelectPatternOrLoopActionBar = new SelectPatternOrLoopActionBar();
		m_pSelectPatternOrLoopActionBar->Create(SelectPatternOrLoopActionBar::IDD);
		m_pSelectPatternOrLoopActionBar->ShowWindow(SW_HIDE);

		m_pTextParamActionBar = new TextParamActionBar();
		m_pTextParamActionBar->Create(IDD_clGeometryFeatureUI_TextParamActionBar);
		m_pTextParamActionBar->ShowWindow(SW_HIDE);

		m_pCreateGeometryGridActionBar = new CreateGeometryGridActionBar();
		m_pCreateGeometryGridActionBar->Create(CreateGeometryGridActionBar::IDD);
		m_pCreateGeometryGridActionBar->ShowWindow(SW_HIDE);

		m_pLoopPositionDetectActionBar = new LoopPositionDetectActionBar();
		m_pLoopPositionDetectActionBar->Create(LoopPositionDetectActionBar::IDD);
		m_pLoopPositionDetectActionBar->ShowWindow(SW_HIDE);
	}
}

CanvasGeometryEditor::~CanvasGeometryEditor( void )
{
	// 这里先销毁action，因为有的action可能会监听这些action bar。一旦action bar先于action销毁时，那么action析构函数中RemoveObserver调用会崩溃。
	m_pAction.reset();
	if (m_pTextParamActionBar) {
		delete m_pTextParamActionBar;
		m_pTextParamActionBar = NULL;
	}
	if (m_pCreateGeometryGridActionBar) {
		delete m_pCreateGeometryGridActionBar;
		m_pCreateGeometryGridActionBar = NULL;
	}
}

BOOL CanvasGeometryEditor::LButtonDown(double& dPtX, double& dPtY)
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

BOOL CanvasGeometryEditor::LButtonUp(double& dPtX, double& dPtY)
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

BOOL CanvasGeometryEditor::LButtonDBClk(double& dPtX, double& dPtY)
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

BOOL CanvasGeometryEditor::RButtonUp(double& dPtX, double& dPtY)
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

BOOL CanvasGeometryEditor::MovePoint(double& dPtX, double& dPtY)
{
	if (m_pAction)
	{
		m_pAction->MovePoint(dPtX, dPtY);

		// check whether we will enter other action.
		IActionPtr pNextAction = m_pAction->GetNextAction();
		if (pNextAction)
			m_pAction = pNextAction;
	}

	return TRUE;
}

BOOL CanvasGeometryEditor::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
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

BOOL CanvasGeometryEditor::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (m_pAction)
	{
		m_pAction->OnKeyUp(nChar, nRepCnt, nFlags);
	}

	return TRUE;
}

CString CanvasGeometryEditor::GetEditorName() const
{
	CString str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_CanvasGeometry);
	return str;
}

BOOL CanvasGeometryEditor::SetEditAction(EditCanvasGeometryActionType emEditCanvasGeometryActionType)
{
	if (m_pAction->GetActionType() == emEditCanvasGeometryActionType) { // 不需要隐藏所有action bar和面板。
		;
	}
	else { // 先隐藏所有action bar和所有面板。
		HideAllActionBars();
		HideAllPanels();
	}

	if (emEditCanvasGeometryActionType == SelectPatternOnCanvas) { // 应该在其他地方启动这个action。
		LogMgr::DebugWarn(_T("498241"));
	}
	else if (emEditCanvasGeometryActionType == EditLinePatternOnCanvas)
	{
		m_pAction.reset(new CanvasLinePatternEditAction(m_pPartPlacement, m_pViewPort));
	}
	else if (emEditCanvasGeometryActionType == EditArcPatternOnCanvas)
	{
		m_pAction.reset(new CanvasArcPatternEditAction(m_pPartPlacement, m_pViewPort));
	}
	else if (emEditCanvasGeometryActionType == EditMultiLinePatternOnCanvas)
	{
		m_pAction.reset(new CanvasMultiLinePatternEditAction(m_pPartPlacement, m_pViewPort));
	}
	else if (emEditCanvasGeometryActionType == EditCirclePatternOnCanvas)
	{
		m_pAction.reset(new CanvasCirclePatternEditAction(m_pPartPlacement, m_pViewPort));
	}
	else if (emEditCanvasGeometryActionType == EditRectPatternOnCanvas)
	{
		m_pAction.reset(new CanvasRectPatternEditAction(m_pPartPlacement, m_pViewPort));
	}
	else if (emEditCanvasGeometryActionType == DrawTextStructOnCanvas)
	{
		m_pAction.reset(new CanvasTextDrawAction(m_pPartPlacement, m_pTextParamActionBar, m_pViewPort));
	}
	else if (emEditCanvasGeometryActionType == DeletePatternOnCanvas)
	{
		pair<PatternListPtr, TextStructListPtr> selectedPatterns = GetSelectedPatterns();
		CanvasPatternDeleteAction* pCanvasPatternDeleteAction = new CanvasPatternDeleteAction(m_pPartPlacement, selectedPatterns.first, selectedPatterns.second,
			m_pSelectPatternOrLoopActionBar, m_pViewPort);
		if (selectedPatterns.first->size() > 0) { // 删除选中的图元，之后不需要进入删除action。
			pCanvasPatternDeleteAction->DeleteSelectedPatterns();
		}
		else {
			m_pAction.reset(pCanvasPatternDeleteAction);
		}
	}
	else if (emEditCanvasGeometryActionType == CopyPatternOnCanvas)
	{
		pair<PatternListPtr, TextStructListPtr> selectedPatterns = GetSelectedPatterns();
		PatternLoopListPtr pAffectedPatLoops(new PatternLoopList);
		Point2D referencePt;
		if (selectedPatterns.first->size() > 0) {
			pAffectedPatLoops = GeometryFeatureHelper::FindAffectedPatternLoops(m_pPartPlacement->GetPart()->GetCadData().get(), selectedPatterns.first.get());
			referencePt = pAffectedPatLoops->GetRectBox().GetCenterPt();
		}
		LoopTopologyItemListPtr pLoopTopologyItems = LoopProcessor::CalcLoopTopStruct(pAffectedPatLoops.get());
		CanvasPatternCopyAction* pCanvasPatternCopyAction = new CanvasPatternCopyAction(m_pPartPlacement, selectedPatterns.first, selectedPatterns.second,
			pAffectedPatLoops, pLoopTopologyItems, referencePt, m_pSelectPatternOrLoopActionBar, m_pLoopPositionDetectActionBar, m_pViewPort);
		m_pAction.reset(pCanvasPatternCopyAction);
	}
	else if (emEditCanvasGeometryActionType == MovePatternOnCanvas)
	{
		pair<PatternListPtr, TextStructListPtr> selectedPatterns = GetSelectedPatterns();
		Point2D referencePt;
		if (selectedPatterns.first->size() > 0) {
			PatternLoopListPtr pSelectedPatLoops = GeometryFeatureHelper::FindAffectedPatternLoops(m_pPartPlacement->GetPart()->GetCadData().get(), selectedPatterns.first.get());
			referencePt = pSelectedPatLoops->GetRectBox().GetCenterPt();
		}
		CanvasPatternMoveAction* pCanvasPatternMoveAction = new CanvasPatternMoveAction(m_pPartPlacement, selectedPatterns.first, selectedPatterns.second, referencePt,
			m_pSelectPatternOrLoopActionBar, m_pLoopPositionDetectActionBar, m_pViewPort);
		m_pAction.reset(pCanvasPatternMoveAction);
	}
	else if (emEditCanvasGeometryActionType == RotatePatternOnCanvas)
	{
		pair<PatternListPtr, TextStructListPtr> selectedPatterns = GetSelectedPatterns();
		CanvasPatternRotateAction* pCanvasPatternRotateAction = new CanvasPatternRotateAction(m_pPartPlacement, selectedPatterns.first, selectedPatterns.second,
			m_pSelectPatternOrLoopActionBar, m_pViewPort);
		m_pAction.reset(pCanvasPatternRotateAction);
	}
	else if (emEditCanvasGeometryActionType == GridPatternOnCanvas)
	{
		pair<PatternListPtr, TextStructListPtr> selectedPatterns = GetSelectedPatterns();
		CanvasPatternGridAction* pCanvasPatternGridAction = new CanvasPatternGridAction(m_pPartPlacement, selectedPatterns.first, selectedPatterns.second,
			m_pSelectPatternOrLoopActionBar, m_pCreateGeometryGridActionBar, m_pViewPort);
		m_pAction.reset(pCanvasPatternGridAction);
	}
	else if (emEditCanvasGeometryActionType == AddCanvasPattern2Task)
	{
		pair<PatternListPtr, TextStructListPtr> selectedPatterns = GetSelectedPatterns();
		AddPartFromCanvasAction* pAddPartFromCanvasAction = new AddPartFromCanvasAction(m_pPartPlacement, selectedPatterns.first, m_pSelectPatternOrLoopActionBar, m_pViewPort);
		if (selectedPatterns.first->size() > 0)
			pAddPartFromCanvasAction->AddSelectedPattern2PartLibrary();
		m_pAction.reset(pAddPartFromCanvasAction);
	}
	else if( emEditCanvasGeometryActionType == EditRectLinePatternOnCanvas )
	{
		m_pAction.reset(new CanvasRectLinePatternEditAction(m_pPartPlacement, m_pViewPort));
	}
	return TRUE;
}

void CanvasGeometryEditor::AddPatternFromDxf(PatternListPtr pNewPatterns, PatternLoopListPtr pNewPatternLoops,
	TextStructListPtr pNewTextStructs, LoopTopologyItemListPtr pNewLoopTopologyItems)
{
	// 先隐藏所有action bar和面板。
	HideAllActionBars();
	HideAllPanels();

	Point2D referencePt = pNewPatternLoops->GetRectBox().GetCenterPt();
	ToolAssignConfigItemPtr pToolAssignConfigItem = boost::dynamic_pointer_cast<ToolAssignConfigItem>(m_pParamConfig->GetItemByType(PARAMCONFIG_ITEM_TOOL_ASSIGN));
	m_pAction.reset(new AddPatternsFromDxfdwgAction(m_pPartPlacement, pNewPatterns, pNewPatternLoops, pNewTextStructs, pNewLoopTopologyItems, referencePt,
		m_pSelectPatternOrLoopActionBar, m_pLoopPositionDetectActionBar, m_pViewPort, pToolAssignConfigItem));
}

void CanvasGeometryEditor::AlignCanvasLoop(AlignLoopStyle emAlignLoopStyle)
{
	if (m_pAction->GetActionType() == AlignLoopsOnCanvas) { // 当前就是对齐action，不需要隐藏所有action bar和面板。
		;
	}
	else { // 先隐藏所有action bar和所有面板。
		HideAllActionBars();
		HideAllPanels();
	}

	CanvasLoopAlignActionPtr pCanvasLoopAlignAction(new CanvasLoopAlignAction(m_pPartPlacement, emAlignLoopStyle, m_pSelectPatternOrLoopActionBar, m_pViewPort));
	m_pAction = pCanvasLoopAlignAction;
}

void CanvasGeometryEditor::StartLoopSelectAction()
{
	if (m_pAction->GetActionType() == SelectPatternOnCanvas) { // 不需要隐藏所有action bar和面板。
		;
	}
	else { // 先隐藏所有action bar和面板。
		HideAllActionBars();
		HideAllPanels();
	}

	CanvasPatternSelectActionPtr pCanvasPatternSelectAction(new CanvasPatternSelectAction(m_pPartPlacement, m_pSelectPatternOrLoopActionBar,
		m_pTextParamActionBar, m_pLoopPositionDetectActionBar, m_pViewPort));
	m_pAction = pCanvasPatternSelectAction;
}

void CanvasGeometryEditor::StartLoopSelectAction_PreSelectLoopAndText(vector<LONGLONG>* pPatternLoopIDs_preSelect, vector<LONGLONG>* pTextStructIDs_preSelect)
{
	const PartCadData* pPartCadData = m_pPartPlacement->GetPart()->GetCadData().get();

	// 得到预选择的几何轮廓。
	PatternLoopListPtr pPatternLoops_preSelect(new PatternLoopList);
	{
		for (unsigned int i = 0; i < pPatternLoopIDs_preSelect->size(); i++) {
			IPatternLoopPtr pPatternLoop = pPartCadData->GetPatternLoopList()->GetLoopByID(pPatternLoopIDs_preSelect->operator [](i));
			pPatternLoops_preSelect->push_back(pPatternLoop);
		}
		if (pPatternLoops_preSelect->size() == 0) {
			LogMgr::DebugWarn(_T("765847"));
			return;
		}
		delete pPatternLoopIDs_preSelect;
	}

	// 得到预选择的文字对象。
	TextStructListPtr pTextStructs_preSelect(new TextStructList);
	{
		if (!pTextStructIDs_preSelect) {
			LogMgr::DebugWarn(_T("643892"));
			return;
		}
		for (unsigned int i = 0; i < pTextStructIDs_preSelect->size(); i++) {
			TextStructPtr pTextStruct = pPartCadData->GetTextStructs()->GetTextStructByID(pTextStructIDs_preSelect->operator [](i));
			pTextStructs_preSelect->push_back(pTextStruct);
		}
		delete pTextStructIDs_preSelect;
	}

	if (m_pAction->GetActionType() == SelectPatternOnCanvas) { // 不需要隐藏所有action bar和面板。
		;
	}
	else { // 先隐藏所有action bar和面板。
		HideAllActionBars();
		HideAllPanels();
	}

	CanvasPatternSelectActionPtr pCanvasPatternSelectAction(new CanvasPatternSelectAction(m_pPartPlacement, m_pSelectPatternOrLoopActionBar,
		m_pTextParamActionBar, m_pLoopPositionDetectActionBar, m_pViewPort));
	pCanvasPatternSelectAction->SelectLoopAndText(pPatternLoops_preSelect, pTextStructs_preSelect);
	m_pAction = pCanvasPatternSelectAction;
}

void CanvasGeometryEditor::StartPatternSelectAction_PreSelectOnePattern(LONGLONG* pPatternID_preSelect)
{
	// 得到预选择的几何特征。
	IPatternPtr pPattern_preSelect;
	{
		const PartCadData* pPartCadData = m_pPartPlacement->GetPart()->GetCadData().get();
		pPattern_preSelect = pPartCadData->GetPatternList()->GetPatternFromID(*pPatternID_preSelect);
		if (!pPattern_preSelect) {
			LogMgr::DebugWarn(_T("400980"));
			return;
		}
		delete pPatternID_preSelect;
	}

	if (m_pAction->GetActionType() == SelectPatternOnCanvas) { // 不需要隐藏所有action bar和面板。
		;
	}
	else { // 先隐藏所有action bar和面板。
		HideAllActionBars();
		HideAllPanels();
	}

	CanvasPatternSelectActionPtr pCanvasPatternSelectAction(new CanvasPatternSelectAction(m_pPartPlacement, m_pSelectPatternOrLoopActionBar,
		m_pTextParamActionBar, m_pLoopPositionDetectActionBar, m_pViewPort));
	pCanvasPatternSelectAction->SelectOnePattern(pPattern_preSelect);
	m_pAction = pCanvasPatternSelectAction;
}

LoopInstanceListPtr CanvasGeometryEditor::GetSelectedLoopInstances() const
{
	LoopInstanceListPtr pSelectedLoopInstances(new LoopInstanceList);

	// 得到选中的几何特征。
	pair<PatternListPtr, TextStructListPtr> selectedPatterns = GetSelectedPatterns();

	// 得到选中的轮廓实例。
	if (selectedPatterns.first->size() > 0) {
		// 所有涉及到的几何轮廓。
		PartCadDataPtr pPartCadData = m_pPartPlacement->GetPart()->GetCadData();
		map<LONGLONG, PatternListPtr> patternsInLoop; // 这个结构记录了要删除的几何特征分别在哪个几何轮廓中。
		PatternLoopListPtr pSelectedPatternLoops = GeometryFeatureHelper::FindAffectedPatternLoops(pPartCadData.get(), selectedPatterns.first.get(), patternsInLoop);

		LoopInstanceListPtr pAllLoopInstances = LoopInstanceManager::GetLoopInstanceList(m_pPartInstance);
		for (unsigned int i = 0; i < pSelectedPatternLoops->size(); i++) {
			LONGLONG iPatternLoopID = pSelectedPatternLoops->operator[](i)->GetID();
			LoopInstancePtr pSelectedLoopInstance = pAllLoopInstances->GetLoopInstanceByPatternLoop(iPatternLoopID);
			pSelectedLoopInstances->push_back(pSelectedLoopInstance);
		}
	}

	return pSelectedLoopInstances;
}

pair<PatternListPtr, TextStructListPtr> CanvasGeometryEditor::GetSelectedPatterns() const
{
	pair<PatternListPtr, TextStructListPtr> selectedPatterns;

	// 设置初始值。
	{
		PatternListPtr pPatterns(new PatternList);
		TextStructListPtr pTextStructs(new TextStructList);
		selectedPatterns.first = pPatterns;
		selectedPatterns.second = pTextStructs;
	}

	if (const CanvasPatternSelectAction* pCanvasPatternSelectAction = dynamic_cast<const CanvasPatternSelectAction*>(m_pAction.get()))
	{
		selectedPatterns = pCanvasPatternSelectAction->GetSelectedPatterns();
	}

	return selectedPatterns;
}

void CanvasGeometryEditor::HideAllActionBars()
{
	if (m_pSelectPatternOrLoopActionBar->IsWindowVisible())
		m_pSelectPatternOrLoopActionBar->ShowWindow(SW_HIDE);

	if (m_pTextParamActionBar->IsWindowVisible())
		m_pTextParamActionBar->ShowWindow(SW_HIDE);

	if (m_pCreateGeometryGridActionBar->IsWindowVisible())
		m_pCreateGeometryGridActionBar->ShowWindow(SW_HIDE);

	if (m_pLoopPositionDetectActionBar->IsWindowVisible())
		m_pLoopPositionDetectActionBar->ShowWindow(SW_HIDE);
}

void CanvasGeometryEditor::HideAllPanels()
{
	CanvasPatternPropertyBar::GetInstance()->ShowBar(FALSE);
}

END_CUTLEADER_NAMESPACE()

#include "StdAfx.h"
#include "CanvasPatternSelectAction.h"

#include "GlViewPort.h"
#include "CommandManager.h"
#include "CursorDrawer.h"
#include "ValueInputManager.h"
#include "ClWindowFinder.h"
#include "LogMgr.h"
#include "Polygon2DList.h"
#include "Text2ContourHelper.h"
#include "clBaseDataResource.h"

#include "DisplayParam.h"
#include "CLOptionInfo.h"
#include "SnapPtParam.h"
#include "PartEditParam.h"
#include "SheetEditParam.h"

#include "SnapPtDrawer.h"
#include "Part.h"
#include "PartPlacement.h"
#include "clGeometryFeatureResource.h"
#include "PatternList.h"
#include "PartCadData.h"
#include "PatternDrawer.h"
#include "CanvasPatternPropertyBar.h"
#include "CanvasLinePatternEditAction.h"
#include "CanvasArcPatternEditAction.h"
#include "CanvasCirclePatternEditAction.h"
#include "CanvasRectPatternEditAction.h"
#include "PatternLoopList.h"
#include "LoopDrawer.h"
#include "CanvasPatternDeleteCommand.h"
#include "IArrayPattern.h"
#include "clPartLayoutResource.h"
#include "GeometryFeatureHelper.h"
#include "TextStruct.h"
#include "TextStructList.h"
#include "TextParamActionBar.h"
#include "CanvasTextEditCommand.h"
#include "clGeometryFeatureUIResource.h"
#include "clCanvasGeometryUIResource.h"
#include "CanvasLoopCopyCommand.h"
#include "CanvasLoopMoveCommand.h"
#include "CanvasPatternMoveAction.h"
#include "CanvasPatternCopyAction.h"
#include "CanvasPatternRotateAction.h"
#include "LoopToolDataList.h"
#include "CanvasLoopRotateCommand.h"

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

CanvasPatternSelectAction::CanvasPatternSelectAction(PartPlacementPtr pPartPlacement, SelectPatternOrLoopActionBar* pSelectPatternOrLoopActionBar,
	TextParamActionBar* pTextParamActionBar, LoopPositionDetectActionBar* pLoopPositionDetectActionBar, GlViewPortPtr pViewPort)
    : CanvasGeometryEditAction(pPartPlacement, pViewPort), LoopPositionDetector(m_pViewPort, pLoopPositionDetectActionBar)
{
	m_pActivePatterns.reset(new PatternList);
	m_pActiveTextStructs.reset(new TextStructList);
	m_pSelectedPatterns.reset(new PatternList);
	m_pSelectedTextStructs.reset(new TextStructList);
	m_emActionState = None;
	m_iMsgID = IDS_OUTPUT_SelectCanvasGeometry2Edit;
	m_pSelectPatternOrLoopActionBar = pSelectPatternOrLoopActionBar;
	m_pTextParamActionBar = pTextParamActionBar;
	m_pTextParamActionBar->AddObserver(this);

	// 有关快捷拷贝。
	m_emQuickEditType = NoneQuickCopy;
	m_pQuickEditPatterns.reset(new PatternList);
	m_pQuickEditTextStructs.reset(new TextStructList);

	m_pTextParamActionBar->HideActionBar();
	m_pSelectPatternOrLoopActionBar->DisplayActionBar(SelectLoop);
}

CanvasPatternSelectAction::~CanvasPatternSelectAction(void)
{
	m_pTextParamActionBar->RemoveObserver(GetID());
}

BOOL CanvasPatternSelectAction::LButtonDown(double& dPtX, double& dPtY)
{
	if (m_emQuickEditType == Ctrl_C_Ctrl_V) { // 执行快捷拷贝。
		ExecuteQuickCopy(true);
		return TRUE;
	}
	else if (m_emQuickEditType == Drag || m_emQuickEditType == Ctrl_Drag) {
		return TRUE;
	}

	if (CheckWillEnterGripEditAction(Point2D(dPtX, dPtY))) { // 要进行夹点编辑。
		SwitchToGripEdit(m_pSelectedPatterns->front(), Point2D(dPtX, dPtY));
		return TRUE;
	}

	// 判断是否进入快捷编辑模式（ctrl+拖拽拷贝几何特征或者拖拽移动几何特征）。
	if (m_pSelectedPatterns->size() > 0)
	{
		double dCaptureRange = CLOptionInfo::GetInstance()->GetSnapPtParam()->GetSnapRange(); // 捕捉距离。
		m_pViewPort->DToW(dCaptureRange);
		IPatternPtr pClosestPattern = m_pSelectedPatterns->GetPatternClosestTo(Point2D(dPtX, dPtY), TRUE); // 先得到距离光标最近的几何特征。
		if (pClosestPattern->DistanceTo(Point2D(dPtX, dPtY)) < dCaptureRange)
		{
			if (ValueInputManager::GetInstance()->IsInputCtrl())
				m_emQuickEditType = Ctrl_Drag;
			else
				m_emQuickEditType = Drag;
			m_quickEditBasePt = Point2D(dPtX, dPtY);
			m_pQuickEditPatterns->clear();
			m_pQuickEditPatterns->insert(m_pQuickEditPatterns->end(), m_pSelectedPatterns->begin(), m_pSelectedPatterns->end());
			m_pQuickEditTextStructs->clear();
			m_pQuickEditTextStructs->insert(m_pQuickEditTextStructs->end(), m_pSelectedTextStructs->begin(), m_pSelectedTextStructs->end());

			// 启动轮廓距离检测。
			PatternLoopListPtr pSelectedPatLoops = GeometryFeatureHelper::FindAffectedPatternLoops(m_pPartPlacement->GetPart()->GetCadData().get(), m_pSelectedPatterns.get());
			LoopToolDataListPtr pToolDataOfMovingPatLoops(new LoopToolDataList);
			UpdateDataBeforeMove(m_emQuickEditType == Ctrl_Drag ? CopyExsitingLoop : MoveExsitingLoop, m_pPartPlacement.get(), pSelectedPatLoops, pToolDataOfMovingPatLoops, m_quickEditBasePt);

			// 更新界面。注意下面的顺序。
			CanvasPatternPropertyBar::GetInstance()->ShowBar(FALSE);
			m_pSelectPatternOrLoopActionBar->HideActionBar();
			m_pTextParamActionBar->HideActionBar();
			m_pLoopPositionDetectActionBar->Show_LoopPositionDetectActionBar();

			return TRUE;
		}
	}

	if (m_emActionState == None || m_emActionState == SecondPoint)
	{
		m_emActionState = FirstPoint;
		m_iMsgID = IDS_OUTPUT_SELECTPAT_NEXTPT;

		m_data[0] = dPtX;
		m_data[1] = dPtY;
		m_data[2] = dPtX;
		m_data[3] = dPtY;
	}

	return TRUE;
}

BOOL CanvasPatternSelectAction::LButtonUp(double& dPtX, double& dPtY)
{
	if (m_emQuickEditType == Ctrl_Drag) { // 执行快捷拷贝。
		ExecuteQuickCopy(false);
		return TRUE;
	}
	else if (m_emQuickEditType == Drag) { // 执行快捷移动。
		ExecuteQuickMove();
		return TRUE;
	}

	if (m_emActionState == FirstPoint)
	{
		m_emActionState = SecondPoint;
		m_iMsgID = IDS_OUTPUT_SelectCanvasGeometry2Edit;

		// 更新选择集。
		if (!ValueInputManager::GetInstance()->IsInputCtrl()) { // 没有按住ctrl键，清除现有选择集。
			m_pSelectedPatterns->clear();
			m_loopsOfSelectedPatterns.clear();
			m_pSelectedTextStructs->clear();
		}
		if (m_pActivePatterns->size() > 0) {
			m_pSelectedPatterns->insert(m_pSelectedPatterns->end(), m_pActivePatterns->begin(), m_pActivePatterns->end());
			m_pSelectedPatterns->SetModified(TRUE); // 这里会更新几何特征集合的时间戳，这样特征点捕捉中会因此更新缓存。
			m_loopsOfSelectedPatterns.insert(m_loopsOfActivePatterns.begin(), m_loopsOfActivePatterns.end());
		}
		if (m_pActiveTextStructs->size() > 0) {
			m_pSelectedTextStructs->insert(m_pSelectedTextStructs->end(), m_pActiveTextStructs->begin(), m_pActiveTextStructs->end());
		}

		// 显示对应的界面。
		if (m_pSelectedPatterns->size() > 0) {
			PartCadDataPtr pPartCadData = m_pPartPlacement->GetPart()->GetCadData();
			PartCamDataPtr pPartCamData = m_pPartPlacement->GetCamData();
			CanvasPatternPropertyBar::GetInstance()->DisplayPatProp(pPartCadData, pPartCamData, m_pSelectedPatterns, m_pSelectedTextStructs);

			if (m_pSelectedTextStructs->size() == 1) { // 当选中一个文字对象时，显示action bar来编辑文字参数。
				CRect existActionBarRect;
				m_pSelectPatternOrLoopActionBar->GetWindowRect(existActionBarRect);
				m_pTextParamActionBar->Show_TextParamActionBar(true, existActionBarRect);
				const TextStruct* pTextStruct = m_pSelectedTextStructs->front().get();
				TextParamPtr pNewTextParam(new TextParam(*pTextStruct->GetTextParam()));
				m_pTextParamActionBar->InitTextParam(pNewTextParam);
			}
			else {
				m_pTextParamActionBar->HideActionBar();
			}
		}
		else {
			m_pTextParamActionBar->HideActionBar();
			CanvasPatternPropertyBar::GetInstance()->ShowBar(FALSE);
		}
	}

	return TRUE;
}

BOOL CanvasPatternSelectAction::MovePoint(double& dPtX, double& dPtY)
{
	if (m_emQuickEditType == Ctrl_C_Ctrl_V || m_emQuickEditType == Ctrl_Drag || m_emQuickEditType == Drag) {
		UpdateDataWhenMove(Point2D(dPtX, dPtY));
		return TRUE;
	}

	m_pActivePatterns->clear();
	m_loopsOfActivePatterns.clear();
	m_pActiveTextStructs->clear();

	const PartCadData* pPartCadData = m_pPartPlacement->GetPart()->GetCadData().get();
	if (m_emActionState == FirstPoint) // 框选。
	{
		m_data[2] = dPtX;
		m_data[3] = dPtY;

		Rect2D rect(m_data[0] < m_data[2] ? m_data[0] : m_data[2], m_data[0] < m_data[2] ? m_data[2] : m_data[0],
			m_data[1] < m_data[3] ? m_data[1] : m_data[3], m_data[1] < m_data[3] ? m_data[3] : m_data[1]);
		TextStructListPtr pTextStructs(new TextStructList);
		if (m_pSelectPatternOrLoopActionBar->GetIsSelectPattern()) {
			m_pActivePatterns = GeometryFeatureHelper::GetPatterns_byRectSelection(pPartCadData, rect, pTextStructs);
			m_loopsOfActivePatterns = GeometryFeatureHelper::FindPatternLoopsOfPatterns(pPartCadData, m_pActivePatterns.get());
			if (pTextStructs->size() > 0) {
				m_pActiveTextStructs->insert(m_pActiveTextStructs->end(), pTextStructs->begin(), pTextStructs->end());
			}
		}
		else if (m_pSelectPatternOrLoopActionBar->GetIsSelectPatternLoop()) {
			PatternLoopListPtr pPatternLoops = GeometryFeatureHelper::GetPatternLoops_byRectSelection(pPartCadData, rect, pTextStructs);
			if (pPatternLoops->size() > 0) {
				m_pActivePatterns = pPatternLoops->GetPatList(m_loopsOfActivePatterns);
				if (pTextStructs->size() > 0) {
					m_pActiveTextStructs->insert(m_pActiveTextStructs->end(), pTextStructs->begin(), pTextStructs->end());
				}
			}
		}
	}
	else { // 点选。
		Point2D pt(dPtX, dPtY);
		TextStructPtr pTextStruct;
		if (m_pSelectPatternOrLoopActionBar->GetIsSelectPattern()) {
			m_pActivePatterns = GeometryFeatureHelper::GetPatterns_byClickSelection(pPartCadData, pt, pTextStruct);
			m_loopsOfActivePatterns = GeometryFeatureHelper::FindPatternLoopsOfPatterns(pPartCadData, m_pActivePatterns.get());
			if (pTextStruct) {
				m_pActiveTextStructs->push_back(pTextStruct);
			}
		}
		else if (m_pSelectPatternOrLoopActionBar->GetIsSelectPatternLoop()) {
			PatternListPtr pPatterns = GeometryFeatureHelper::GetPatterns_byClickSelection(pPartCadData, pt, pTextStruct);
			if (pPatterns->size() > 0) {
				if (pTextStruct) { // 选中了一个文字对象。
					m_pActivePatterns = pPatterns;
					m_pActiveTextStructs->push_back(pTextStruct);
				}
				else { // 此时应该选中了一个几何特征。
					IPatternLoopPtr pPatternLoop = pPartCadData->GetPatternLoopList()->FindLoopByPat(pPatterns->front().get());
					m_pActivePatterns = pPatternLoop->GetPatternList();
				}
				m_loopsOfActivePatterns = GeometryFeatureHelper::FindPatternLoopsOfPatterns(pPartCadData, m_pActivePatterns.get());
			}
		}
	}

	return TRUE;
}

SnapInputPtr CanvasPatternSelectAction::GetSnapInput() const
{
	SnapInputPtr pSnapInput = SnapInput::BuildSnapInput4SnapNone();

	if (m_emQuickEditType == Ctrl_C_Ctrl_V) {
		pSnapInput = SnapInput::BuildSnapInput4SnapNone(); // 不要开启捕捉功能。
	}
	else {
		if (m_emActionState == FirstPoint) { // 框选。
			pSnapInput = SnapInput::BuildSnapInput4SnapNone(); // 不要开启捕捉功能。
		}
		else {
			// 只有当选中了一个几何特征时，才开启控制点捕捉。
			if (m_pSelectedPatterns->size() == 1)
				pSnapInput.reset(new SnapInput(SnapControlPtOfPattern, m_pSelectedPatterns, Matrix2D()));
			else
				pSnapInput = SnapInput::BuildSnapInput4SnapNone(); // 不要开启捕捉功能。
		}
	}

	return pSnapInput;
}

BOOL CanvasPatternSelectAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CanvasGeometryEditAction::OnKeyDown(nChar, nRepCnt, nFlags);

	CString strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);
	const SheetEditParam* pSheetEditParam = CLOptionInfo::GetInstance()->GetSheetEditParam().get();

	if (ValueInputManager::GetInstance()->IsInputCtrl() && ValueInputManager::GetInstance()->IsInputA(nChar)) { // 选中全部图形。
		PartCadDataPtr pPartCadData = m_pPartPlacement->GetPart()->GetCadData();
		m_pSelectedPatterns->clear();
		m_pSelectedPatterns->insert(m_pSelectedPatterns->end(), pPartCadData->GetPatternList()->begin(), pPartCadData->GetPatternList()->end());
		m_loopsOfSelectedPatterns = GeometryFeatureHelper::FindPatternLoopsOfPatterns(pPartCadData.get(), m_pSelectedPatterns.get());
	}
	else if (ValueInputManager::GetInstance()->IsInputCancel(nChar)) { // 此时要是按了退出键，需要隐藏属性编辑界面。
		CanvasPatternPropertyBar::GetInstance()->ShowBar(FALSE);
	}
	else if (ValueInputManager::GetInstance()->IsInputDel(nChar)) {
		DeleteSelectedPatterns();
	}
	// 步进移动选中的轮廓。
	else if (ValueInputManager::GetInstance()->IsInputCtrl() && ValueInputManager::GetInstance()->IsInputUpArrow(nChar))
		StepMove(POSITION_UP);
	else if (ValueInputManager::GetInstance()->IsInputCtrl() && ValueInputManager::GetInstance()->IsInputDownArrow(nChar))
		StepMove(POSITION_DOWN);
	else if (ValueInputManager::GetInstance()->IsInputCtrl() && ValueInputManager::GetInstance()->IsInputLeftArrow(nChar))
		StepMove(POSITION_LEFT);
	else if (ValueInputManager::GetInstance()->IsInputCtrl() && ValueInputManager::GetInstance()->IsInputRightArrow(nChar))
		StepMove(POSITION_RIGHT);
	// 旋转选中的几何轮廓。
	else if (ValueInputManager::GetInstance()->IsInputAdd(nChar) && m_pSelectedPatterns->size() > 0) {
		if (m_emQuickEditType == NoneQuickCopy) { // 不处于快捷编辑阶段。
			RotateSelectedLoops(-pSheetEditParam->GetRotateStep()*D2R);
		}
		else {
			UpdateWhenRotate(-pSheetEditParam->GetRotateStep()*D2R);
		}
	}
	else if (ValueInputManager::GetInstance()->IsInputDes(nChar) && m_pSelectedPatterns->size() > 0) {
		if (m_emQuickEditType == NoneQuickCopy) { // 不处于快捷编辑阶段。
			RotateSelectedLoops(pSheetEditParam->GetRotateStep()*D2R);
		}
		else {
			UpdateWhenRotate(pSheetEditParam->GetRotateStep()*D2R);
		}
	}
	else if (!ValueInputManager::GetInstance()->IsInputCtrl() && ValueInputManager::GetInstance()->IsInputS(nChar) && m_pSelectedPatterns->size() > 0) { // 逆时针旋转90度。
		if (m_emQuickEditType == NoneQuickCopy) { // 不处于快捷编辑阶段。
			RotateSelectedLoops(PID2);
		}
		else {
			UpdateWhenRotate(PID2);
		}
	}
	else if (!ValueInputManager::GetInstance()->IsInputCtrl() && ValueInputManager::GetInstance()->IsInputW(nChar) && m_pSelectedPatterns->size() > 0) { // 顺时针旋转90度。
		if (m_emQuickEditType == NoneQuickCopy) { // 不处于快捷编辑阶段。
			RotateSelectedLoops(-PID2);
		}
		else {
			UpdateWhenRotate(-PID2);
		}
	}
	//
	else if (ValueInputManager::GetInstance()->IsInputCtrl() && ValueInputManager::GetInstance()->IsInputC(nChar)) {
		if (m_pSelectedPatterns->size() > 0) {
			m_pQuickEditPatterns->clear();
			m_pQuickEditPatterns->insert(m_pQuickEditPatterns->end(), m_pSelectedPatterns->begin(), m_pSelectedPatterns->end());
			m_pQuickEditTextStructs->clear();
			m_pQuickEditTextStructs->insert(m_pQuickEditTextStructs->end(), m_pSelectedTextStructs->begin(), m_pSelectedTextStructs->end());
		} else {
			CString str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_OUTPUT_SelectCanvasGeometry2Copy);
			MessageBox(m_pViewPort->GetWnd(), str, strProductName, MB_OK | MB_ICONWARNING);
		}
	}
	else if (ValueInputManager::GetInstance()->IsInputCtrl() && ValueInputManager::GetInstance()->IsInputV(nChar))
	{
		if (m_pQuickEditPatterns->size() > 0) {
			m_emQuickEditType = Ctrl_C_Ctrl_V;
			m_quickEditBasePt = m_pQuickEditPatterns->GetRectBox().GetCenterPt();

			// 启动轮廓距离检测。
			PatternLoopListPtr pQuickEditPatLoops = GeometryFeatureHelper::FindAffectedPatternLoops(m_pPartPlacement->GetPart()->GetCadData().get(), m_pQuickEditPatterns.get());
			LoopToolDataListPtr pToolDataOfMovingPatLoops(new LoopToolDataList);
			UpdateDataBeforeMove(CopyExsitingLoop, m_pPartPlacement.get(), pQuickEditPatLoops, pToolDataOfMovingPatLoops, pQuickEditPatLoops->GetRectBox().GetCenterPt());
			m_pLoopPositionDetectActionBar->Show_LoopPositionDetectActionBar();
			CanvasPatternPropertyBar::GetInstance()->ShowBar(FALSE);
			m_pSelectPatternOrLoopActionBar->HideActionBar();
		}
	}
	else if (ValueInputManager::GetInstance()->IsInputCtrl() && ValueInputManager::GetInstance()->IsInputQ(nChar)) { // 试图移动
		if (m_pSelectedPatterns->size() > 0) {
			CanvasPatternPropertyBar::GetInstance()->ShowBar(FALSE); // 要先隐藏这个面板。
			PatternLoopListPtr pSelectedPatLoops = GeometryFeatureHelper::FindAffectedPatternLoops(m_pPartPlacement->GetPart()->GetCadData().get(), m_pSelectedPatterns.get());
			Point2D referencePt = pSelectedPatLoops->GetRectBox().GetCenterPt();
			CanvasPatternMoveAction* pCanvasPatternMoveAction = new CanvasPatternMoveAction(m_pPartPlacement, m_pSelectedPatterns, m_pSelectedTextStructs, referencePt,
				m_pSelectPatternOrLoopActionBar, m_pLoopPositionDetectActionBar, m_pViewPort);
			m_pNextAction.reset(pCanvasPatternMoveAction);
		} else {
			CString str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_OUTPUT_SelectCanvasGeometry2Move);
			MessageBox(m_pViewPort->GetWnd(), str, strProductName, MB_OK | MB_ICONWARNING);
		}
	}
	else if (ValueInputManager::GetInstance()->IsInputCtrl() && ValueInputManager::GetInstance()->IsInputW(nChar)) { // 试图旋转
		if (m_pSelectedPatterns->size() > 0) {
			CanvasPatternPropertyBar::GetInstance()->ShowBar(FALSE); // 要先隐藏这个面板。
			CanvasPatternRotateAction* pCanvasPatternRotateAction = new CanvasPatternRotateAction(m_pPartPlacement, m_pSelectedPatterns, m_pSelectedTextStructs,
				m_pSelectPatternOrLoopActionBar, m_pViewPort);
			m_pNextAction.reset(pCanvasPatternRotateAction);
		} else {
			CString str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_OUTPUT_SelectCanvasGeometry2Rotate);
			MessageBox(m_pViewPort->GetWnd(), str, strProductName, MB_OK | MB_ICONWARNING);
		}
	}
	// 现在不用这样了。
	//else if (ValueInputManager::GetInstance()->IsInputTab(nChar))
	//{
	//	if (m_emQuickEditType == Drag) // 切换到CanvasPatternMoveAction动作。
	//	{
	//		m_pNextAction.reset(new CanvasPatternMoveAction(m_pPartPlacement, m_pSelectedPatterns, m_pSelectedTextStructs, m_quickEditBasePt,
	//			m_pSelectPatternOrLoopActionBar, GetLoopDistanceDetectActionBar(), m_pViewPort));
	//	}
	//	else if (m_emQuickEditType == Ctrl_Drag) // 切换到CanvasPatternCopyAction动作。
	//	{
	//		m_pNextAction.reset(new CanvasPatternCopyAction(m_pPartPlacement, m_pSelectedPatterns, m_pSelectedTextStructs, m_quickEditBasePt,
	//			m_pSelectPatternOrLoopActionBar, GetLoopDistanceDetectActionBar(), m_pViewPort));
	//	}
	//}

	return TRUE;
}

void CanvasPatternSelectAction::DeleteSelectedPatterns()
{
	if (m_pSelectedPatterns->size() > 0)
	{
		// 准备要删除的几何特征。
		PatternListPtr pDeletingPatterns(new PatternList());
		for (unsigned int i = 0; i < m_pSelectedPatterns->size(); i++) {
			IPatternPtr pPattern = m_pSelectedPatterns->at(i);
			pDeletingPatterns->push_back(pPattern);
			if (pPattern->IsArrayPattern()) { // when delete the grid pattern, the control pattern will also be deleted.
				IArrayPatternPtr pArrayPattern = boost::dynamic_pointer_cast<IArrayPattern>(pPattern);
				IPatternPtr pCtrlPat = pArrayPattern->GetControlPattern();
				pDeletingPatterns->push_back(pCtrlPat);
			}
		}

		// 要删除的文字。
		vector<LONGLONG> textStructs_willDelete;
		for (unsigned int i = 0; i < m_pSelectedTextStructs->size(); i++) {
			textStructs_willDelete.push_back(m_pSelectedTextStructs->operator[](i)->GetID());
		}

		// 执行命令。
		PartCadDataPtr pPartCadData = m_pPartPlacement->GetPart()->GetCadData();
		PartCamDataPtr pPartCamData = m_pPartPlacement->GetCamData();
		ICommandPtr pCommand(new CanvasPatternDeleteCommand(GetEditorData(), pPartCadData, pPartCamData, pDeletingPatterns, textStructs_willDelete));
		pCommand->Do();
		CommandManager::GetInstance()->AddCommand(pCommand);

		// 重置数据。
		m_pActivePatterns->clear();
		m_pSelectedPatterns->clear();
		m_loopsOfActivePatterns.clear();
		m_loopsOfSelectedPatterns.clear();

		CanvasPatternPropertyBar::GetInstance()->ShowBar(FALSE);
		m_pTextParamActionBar->HideActionBar();

		// 做一些更新。
		ClWindowFinder::GetCurrentView_1()->SendMessage(WM_updateSheetPreviewData, 0, 0);
		ClWindowFinder::GetCurrentView_2()->SendMessage(WM_redrawSheetThumbnailOnSheetManagementPanel, 0, 0);
	}
}

int CanvasPatternSelectAction::GetActionName() const
{
	if (m_pSelectPatternOrLoopActionBar->GetIsSelectPattern()) {
		return IDS_SELECT_GEOM;
	}

	return IDS_SelectLoopInstance;
}

BOOL CanvasPatternSelectAction::DrawBeforeFinish()
{
	const DisplayParam* pDisplayParam = CLOptionInfo::GetInstance()->GetDisplayParam().get();
	SnapPtDrawer snapPtDrawer(m_pViewPort);
	PatternDrawer patternDrawer(m_pViewPort);

	// 绘制选中的几何特征。
	DrawSelectedPatterns(m_pSelectedPatterns.get(), m_loopsOfSelectedPatterns);

	if (m_emQuickEditType == Ctrl_C_Ctrl_V || m_emQuickEditType == Ctrl_Drag || m_emQuickEditType == Drag) {
		// 绘制临时的几何特征。
		const PatternList* pTmpPatterns = this->GetMovingPatterns();
		patternDrawer.DrawPatList(pTmpPatterns);

		// 在光标右下偏一点显示提示信息。
		CString strExtraTip;
		if (m_emQuickEditType != Ctrl_C_Ctrl_V && ValueInputManager::GetInstance()->IsInputLeftMouseButton()) {
			strExtraTip = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PressKey_SetLoopDistance);
		}
		DrawAssistInfo(strExtraTip);

		// draw cursor.
		if (m_emQuickEditType == Ctrl_C_Ctrl_V) {
			CursorDrawer::DrawCrossLineCursor(m_pViewPort);
		}
		else {
			CursorDrawer::DrawAimSelCursor(m_pViewPort);
		}
	}
	else {
		// 绘制矩形选择框。
		if (m_emActionState == FirstPoint) {
			DrawSelectionBox(m_data[0], m_data[1], m_data[2], m_data[3]);
		}

		// 绘制高亮的几何特征。
		PatternListPtr pActivePatterns(new PatternList);
		for (unsigned int i = 0; i < m_pActivePatterns->size(); i++) {
			if (m_pSelectedPatterns->GetPatternFromID(m_pActivePatterns->operator[](i)->GetID()) == nullptr) { // 这个几何特征不在选择集中。
				pActivePatterns->push_back(m_pActivePatterns->operator[](i));
			}
		}
		DrawActivePatterns(pActivePatterns.get(), m_loopsOfActivePatterns);

		// 仅当只选中一个几何特征时，才显示其控制点。
		if (m_pSelectedPatterns->size() == 1) {
			snapPtDrawer.DrawPatListCtrlPt(m_pSelectedPatterns);
		}

		// draw cursor.
		CursorDrawer::DrawAimSelCursor(m_pViewPort);
	}

	return TRUE;
}

void CanvasPatternSelectAction::UpdateForUndoRedo()
{
	m_pSelectedPatterns->clear();
	m_pSelectedPatterns->SetModified(TRUE); // 这里会更新几何特征集合的时间戳，这样特征点捕捉中会因此更新缓存。
	m_pActivePatterns->clear();
	m_loopsOfActivePatterns.clear();
	m_loopsOfSelectedPatterns.clear();
	m_pActiveTextStructs->clear();
	m_pSelectedTextStructs->clear();
	CanvasPatternPropertyBar::GetInstance()->ShowBar(FALSE);
	m_pTextParamActionBar->HideActionBar();
	m_pSelectPatternOrLoopActionBar->DisplayActionBar(OnlyRepositionBar); // 需要调整一下位置。

	// 更新预览数据。
	ClWindowFinder::GetCurrentView_1()->SendMessage(WM_updateSheetPreviewData, 0, 0);
	ClWindowFinder::GetCurrentView_2()->SendMessage(WM_redrawSheetThumbnailOnSheetManagementPanel, 0, 0);
}

void CanvasPatternSelectAction::Update(int iEventID, const CComVariant& varEventContent)
{
	if (iEventID == ID_EVENT_TextParamChanged)
	{
		// 得到新的文字参数。
		const TextParam* pTextParam = m_pTextParamActionBar->GetTextParam();
		TextParamPtr pNewTextParam(new TextParam(*pTextParam));

		// 要编辑的文字对象。
		if (m_pSelectedTextStructs->size() != 1) {
			LogMgr::DebugWarn(_T("451875"));
			return;
		}
		LONGLONG iTextStructID = m_pSelectedTextStructs->front()->GetID();

		// 重新生成文字的轮廓。
		Polygon2DListPtr pNewTextContours(new Polygon2DList);
		{
			const CString& strTextContent = pNewTextParam->GetTextContent();
			const CString& strFontFileName = pNewTextParam->GetFontFileName();
			double dTextHeight = pNewTextParam->GetTextHeight();
			double dTextWidth = pNewTextParam->GetTextWidth();
			double dTextTiltAngle = pNewTextParam->GetTextTiltAngle();
			const CString& strDefaultFontFileName = CLOptionInfo::GetInstance()->GetPartEditParam()->GetDefaultFontFileName();
			TextContourGenerationErrorCode emErrorCode = Text2ContourHelper::GenerateTextContours(strTextContent, strFontFileName, dTextHeight, dTextWidth, dTextTiltAngle,
				strDefaultFontFileName, pNewTextContours);
			if (emErrorCode != Succeed)
			{
				CString strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);
				CString strMessage = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_CannotCreateText);
				MessageBox(m_pViewPort->GetWnd(), strMessage, strProductName, MB_OK | MB_ICONWARNING);
				return;
			}
		}

		// 执行命令。
		PartCadDataPtr pPartCadData = m_pPartPlacement->GetPart()->GetCadData();
		PartCamDataPtr pPartCamData = m_pPartPlacement->GetCamData();
		ICommandPtr pCommand(new CanvasTextEditCommand(GetEditorData(), pPartCadData, pPartCamData, iTextStructID, pNewTextParam, pNewTextContours));
		pCommand->Do();
		CommandManager::GetInstance()->AddCommand(pCommand);

		// 重置数据。
		{
			TextStructPtr pTextStruct = pPartCadData->GetTextStructs()->GetTextStructByID(iTextStructID);

			// 更新当前的文字和选中的文字对象。
			m_pActiveTextStructs->clear();
			m_pActiveTextStructs->push_back(pTextStruct);
			m_pSelectedTextStructs->clear();
			m_pSelectedTextStructs->push_back(pTextStruct);

			// 更新当前的几何特征和选中的几何特征。
			const PatternList* pInvolvedPatterns = pTextStruct->GetInvolvedPatterns().get();
			m_pActivePatterns->clear();
			m_pActivePatterns->insert(m_pActivePatterns->end(), pInvolvedPatterns->begin(), pInvolvedPatterns->end());
			m_pSelectedPatterns->clear();
			m_pSelectedPatterns->insert(m_pSelectedPatterns->end(), pInvolvedPatterns->begin(), pInvolvedPatterns->end());
			m_loopsOfActivePatterns = GeometryFeatureHelper::FindPatternLoopsOfPatterns(pPartCadData.get(), m_pActivePatterns.get());
			m_loopsOfSelectedPatterns = m_loopsOfActivePatterns;
		}

		CWnd::FromHandle(m_pViewPort->GetWnd())->Invalidate(); // 更新板材视图。
		CanvasPatternPropertyBar::GetInstance()->DisplayPatProp(pPartCadData, m_pPartPlacement->GetCamData(), m_pSelectedPatterns, m_pSelectedTextStructs); // 更新几何特征属性面板。
		ClWindowFinder::GetCurrentView_1()->SendMessage(WM_updateSheetPreviewData, 0, 0); // 更新板材预览图。
		ClWindowFinder::GetCurrentView_2()->SendMessage(WM_redrawSheetThumbnailOnSheetManagementPanel, 0, 0); // 更新板材管理器面板中的预览图。
	}
}

bool CanvasPatternSelectAction::CheckWillEnterGripEditAction(const Point2D& pt)
{
	if (m_pSelectedPatterns->size() != 1)
		return false;

	const IPattern* pPattern = m_pSelectedPatterns->front().get();
	Point2DList pts;
	pPattern->GetSnapPoints(pts);
	for (unsigned int i = 0; i < pts.size(); i++)
	{
		if (pt.DistanceTo(pts[i]) < GEOM_TOLERANCE)
			return true;
	}

	return false;
}

void CanvasPatternSelectAction::SwitchToGripEdit(IPatternPtr pPattern, const Point2D& gripEditPt)
{
	// 隐藏这些界面。
	m_pSelectPatternOrLoopActionBar->HideActionBar();
	CanvasPatternPropertyBar::GetInstance()->ShowBar(FALSE);

	PatternType emPatternType = pPattern->GetPatternType();
	if (emPatternType == PATTERN_LINE)
		m_pNextAction.reset(new CanvasLinePatternEditAction(m_pPartPlacement, m_pViewPort));
	else if (emPatternType == PATTERN_ARC)
		m_pNextAction.reset(new CanvasArcPatternEditAction(m_pPartPlacement, m_pViewPort));
	else if (emPatternType == PATTERN_ROUNDHOLE)
		m_pNextAction.reset(new CanvasCirclePatternEditAction(m_pPartPlacement, m_pViewPort));
	else if (emPatternType == PATTERN_RECT_HOLE)
		m_pNextAction.reset(new CanvasRectPatternEditAction(m_pPartPlacement, m_pViewPort));
	else {
		LogMgr::DebugWarn(_T("745364"));
	}

	// init the data for Grip-edit.
	ICanvasPatternGripEditAction* pCanvasPatternGripEditAction = dynamic_cast<ICanvasPatternGripEditAction*>(m_pNextAction.get());
	pCanvasPatternGripEditAction->SetGripEditPattern(pPattern);
	pCanvasPatternGripEditAction->SetGripEditPt(gripEditPt.X(), gripEditPt.Y());
}

void CanvasPatternSelectAction::SelectLoopAndText(PatternLoopListPtr pPatternLoops, TextStructListPtr pTextStructs)
{
	m_pSelectedPatterns = pPatternLoops->GetPatList();
	m_loopsOfSelectedPatterns = GeometryFeatureHelper::FindPatternLoopsOfPatterns(m_pPartPlacement->GetPart()->GetCadData().get(), m_pSelectedPatterns.get());
	m_pSelectedTextStructs = pTextStructs;
	m_emActionState = SecondPoint;

	// 显示属性面板，注意这里要先显示它，这样后续的action bar才能显示在正确的位置。
	CanvasPatternPropertyBar::GetInstance()->DisplayPatProp(m_pPartPlacement->GetPart()->GetCadData(), m_pPartPlacement->GetCamData(), m_pSelectedPatterns, m_pSelectedTextStructs);

	// 这里还要显示这个action bar。
	m_pSelectPatternOrLoopActionBar->DisplayActionBar(SelectLoop);

	// 当选中一个文字对象时，显示action bar来编辑文字参数。
	if (m_pSelectedTextStructs->size() == 1) {
		CRect existActionBarRect;
		m_pSelectPatternOrLoopActionBar->GetWindowRect(existActionBarRect);
		m_pTextParamActionBar->Show_TextParamActionBar(true, existActionBarRect);
		m_pTextParamActionBar->InitTextParam(TextParamPtr(new TextParam(*m_pSelectedTextStructs->front()->GetTextParam())));
	}
}

void CanvasPatternSelectAction::SelectOnePattern(IPatternPtr pPattern)
{
	m_pSelectedPatterns->push_back(pPattern);
	m_loopsOfSelectedPatterns = GeometryFeatureHelper::FindPatternLoopsOfPatterns(m_pPartPlacement->GetPart()->GetCadData().get(), m_pSelectedPatterns.get());
	m_emActionState = SecondPoint;

	// 显示属性面板，注意这里要先显示它，这样后续的action bar才能显示在正确的位置。
	CanvasPatternPropertyBar::GetInstance()->DisplayPatProp(m_pPartPlacement->GetPart()->GetCadData(), m_pPartPlacement->GetCamData(), m_pSelectedPatterns, m_pSelectedTextStructs);

	// 这里还要显示这个action bar。
	m_pSelectPatternOrLoopActionBar->DisplayActionBar(SelectPattern);
}

void CanvasPatternSelectAction::ExecuteQuickCopy(bool bCtrl_C_Ctrl_V)
{
	PartCadDataPtr pPartCadData = m_pPartPlacement->GetPart()->GetCadData();
	PartCamDataPtr pPartCamData = m_pPartPlacement->GetCamData();

	// 要拷贝的文字。
	vector<LONGLONG> textStructs_willCopy;
	for (unsigned int i = 0; i < m_pQuickEditTextStructs->size(); i++)
		textStructs_willCopy.push_back(m_pQuickEditTextStructs->operator[](i)->GetID());

	// 执行命令。
	PatternLoopListPtr pPatternLoops_willCopy = GeometryFeatureHelper::FindAffectedPatternLoops(pPartCadData.get(), m_pQuickEditPatterns.get()); // 找出所有涉及到的几何轮廓。
	CanvasLoopCopyCommandPtr pCanvasLoopCopyCommand(new CanvasLoopCopyCommand(GetEditorData(), pPartCadData, pPartCamData, pPatternLoops_willCopy, textStructs_willCopy, GetTransformMatrix()));
	pCanvasLoopCopyCommand->Do();
	CommandManager::GetInstance()->AddCommand(pCanvasLoopCopyCommand);
	tuple<PatternListPtr, PatternLoopListPtr, TextStructListPtr> newCadData = pCanvasLoopCopyCommand->GetNewCadData();

	// 更新成员变量。
	m_pSelectedPatterns = get<0>(newCadData);
	m_loopsOfSelectedPatterns = GeometryFeatureHelper::FindPatternLoopsOfPatterns(pPartCadData.get(), m_pSelectedPatterns.get());
	m_pSelectedTextStructs = get<2>(newCadData);
	m_emQuickEditType = NoneQuickCopy;
	if (!bCtrl_C_Ctrl_V) {
		m_pQuickEditPatterns->clear();
		m_pQuickEditTextStructs->clear();
	}

	// 更新界面。
	ClWindowFinder::GetCurrentView_1()->SendMessage(WM_updateSheetPreviewData, 0, 0);
	ClWindowFinder::GetCurrentView_2()->SendMessage(WM_redrawSheetThumbnailOnSheetManagementPanel, 0, 0);
	m_pLoopPositionDetectActionBar->HideActionBar();
	CanvasPatternPropertyBar::GetInstance()->DisplayPatProp(pPartCadData, pPartCamData, m_pSelectedPatterns, m_pSelectedTextStructs); // 在这里显示，这样后续的action bar才能显示在正确的位置。
	m_pSelectPatternOrLoopActionBar->DisplayActionBar(SelectLoop);
	if (m_pSelectedTextStructs->size() == 1) { // 当选中一个文字对象时，显示action bar来编辑文字参数。
		CRect existActionBarRect;
		m_pSelectPatternOrLoopActionBar->GetWindowRect(existActionBarRect);
		m_pTextParamActionBar->Show_TextParamActionBar(true, existActionBarRect);
		m_pTextParamActionBar->InitTextParam(TextParamPtr(new TextParam(*m_pSelectedTextStructs->front()->GetTextParam())));
	}
}

void CanvasPatternSelectAction::ExecuteQuickMove()
{
	PartCadDataPtr pPartCadData = m_pPartPlacement->GetPart()->GetCadData();
	PartCamDataPtr pPartCamData = m_pPartPlacement->GetCamData();

	// 要移动的几何轮廓。
	vector<LONGLONG> patternLoops_willMove;
	PatternLoopListPtr pPatternLoops_willMove = GeometryFeatureHelper::FindAffectedPatternLoops(pPartCadData.get(), m_pQuickEditPatterns.get()); // 所有涉及到的几何轮廓。
	for (unsigned int i = 0; i < pPatternLoops_willMove->size(); i++)
		patternLoops_willMove.push_back(pPatternLoops_willMove->operator[](i)->GetID());

	// 要移动的文字。
	vector<LONGLONG> textStructs_willMove;
	for (unsigned int i = 0; i < m_pQuickEditTextStructs->size(); i++)
		textStructs_willMove.push_back(m_pQuickEditTextStructs->operator[](i)->GetID());

	// 执行命令。
	ICommandPtr pCommand(new CanvasLoopMoveCommand(GetEditorData(), pPartCadData, pPartCamData, patternLoops_willMove, textStructs_willMove, GetTransformMatrix()));
	pCommand->Do();
	CommandManager::GetInstance()->AddCommand(pCommand);

	// 更新成员变量。
	m_pSelectedPatterns = pPatternLoops_willMove->GetPatList();
	m_loopsOfSelectedPatterns = GeometryFeatureHelper::FindPatternLoopsOfPatterns(pPartCadData.get(), m_pSelectedPatterns.get());
	//m_pSelectedTextStructs = pTextStructs; // 这个成员中的数据应该就是所需的，不要再设置。
	m_emQuickEditType = NoneQuickCopy;
	m_pQuickEditPatterns->clear();
	m_pQuickEditTextStructs->clear();

	// 更新界面。
	ClWindowFinder::GetCurrentView_1()->SendMessage(WM_updateSheetPreviewData, 0, 0);
	ClWindowFinder::GetCurrentView_2()->SendMessage(WM_redrawSheetThumbnailOnSheetManagementPanel, 0, 0);
	m_pLoopPositionDetectActionBar->HideActionBar();
	CanvasPatternPropertyBar::GetInstance()->DisplayPatProp(pPartCadData, pPartCamData, m_pSelectedPatterns, m_pSelectedTextStructs); // 在这里显示，这样后续的action bar才能显示在正确的位置。
	m_pSelectPatternOrLoopActionBar->DisplayActionBar(SelectLoop);
	if (m_pSelectedTextStructs->size() == 1) { // 当选中一个文字对象时，显示action bar来编辑文字参数。
		CRect existActionBarRect;
		m_pSelectPatternOrLoopActionBar->GetWindowRect(existActionBarRect);
		m_pTextParamActionBar->Show_TextParamActionBar(true, existActionBarRect);
		m_pTextParamActionBar->InitTextParam(TextParamPtr(new TextParam(*m_pSelectedTextStructs->front()->GetTextParam())));
	}
}

void CanvasPatternSelectAction::StepMove(POSITION_INFO emDir)
{
	if (m_pSelectedPatterns->size() == 0)
		return;

	const SheetEditParam* pSheetEditParam = CLOptionInfo::GetInstance()->GetSheetEditParam().get();
	PartCadDataPtr pPartCadData = m_pPartPlacement->GetPart()->GetCadData();
	PartCamDataPtr pPartCamData = m_pPartPlacement->GetCamData();

	// 确定矩阵。
	Matrix2D transformMatrix;
	if (emDir == POSITION_UP) {
		transformMatrix.SetTransfer(Vector2D(0, pSheetEditParam->GetMoveStep()));
	}
	else if (emDir == POSITION_DOWN) {
		transformMatrix.SetTransfer(Vector2D(0, -pSheetEditParam->GetMoveStep()));
	}
	else if (emDir == POSITION_LEFT) {
		transformMatrix.SetTransfer(Vector2D(-pSheetEditParam->GetMoveStep(), 0));
	}
	else if (emDir == POSITION_RIGHT) {
		transformMatrix.SetTransfer(Vector2D(pSheetEditParam->GetMoveStep(), 0));
	}

	// 要移动的几何轮廓。
	vector<LONGLONG> patternLoops_willMove;
	PatternLoopListPtr pPatternLoops_willMove = GeometryFeatureHelper::FindAffectedPatternLoops(pPartCadData.get(), m_pSelectedPatterns.get()); // 所有涉及到的几何轮廓。
	for (unsigned int i = 0; i < pPatternLoops_willMove->size(); i++)
		patternLoops_willMove.push_back(pPatternLoops_willMove->operator[](i)->GetID());

	// 要移动的文字。
	vector<LONGLONG> textStructs_willMove;
	for (unsigned int i = 0; i < m_pQuickEditTextStructs->size(); i++)
		textStructs_willMove.push_back(m_pQuickEditTextStructs->operator[](i)->GetID());

	// 执行命令。
	ICommandPtr pCommand(new CanvasLoopMoveCommand(GetEditorData(), pPartCadData, pPartCamData, patternLoops_willMove, textStructs_willMove, transformMatrix));
	pCommand->Do();
	CommandManager::GetInstance()->AddCommand(pCommand);

	ClWindowFinder::GetCurrentView_1()->SendMessage(WM_updateSheetPreviewData, 0, 0);
	ClWindowFinder::GetCurrentView_2()->SendMessage(WM_redrawSheetThumbnailOnSheetManagementPanel, 0, 0);
	if (CanvasPatternPropertyBar::GetInstance()->IsVisible()) {
		CanvasPatternPropertyBar::GetInstance()->UpdateProp();
	}
}

void CanvasPatternSelectAction::RotateSelectedLoops(double dRotateAngle)
{
	if (m_pSelectedPatterns->size() == 0) {
		LogMgr::DebugWarn(_T("499982"));
		return;
	}

	PartCadDataPtr pPartCadData = m_pPartPlacement->GetPart()->GetCadData();
	PartCamDataPtr pPartCamData = m_pPartPlacement->GetCamData();

	// 要旋转的几何轮廓。
	vector<LONGLONG> patternLoops_willRotate;
	map<LONGLONG, PatternListPtr> patternsInLoop; // 这个结构记录了要删除的几何特征分别在哪个几何轮廓中。
	PatternLoopListPtr pPatternLoops_willRotate = GeometryFeatureHelper::FindAffectedPatternLoops(pPartCadData.get(), m_pSelectedPatterns.get(), patternsInLoop);
	for (unsigned int i = 0; i < pPatternLoops_willRotate->size(); i++)
		patternLoops_willRotate.push_back(pPatternLoops_willRotate->operator[](i)->GetID());

	// 要旋转的文字。
	vector<LONGLONG> textStructs_willMove;
	for (unsigned int i = 0; i < m_pSelectedTextStructs->size(); i++)
		textStructs_willMove.push_back(m_pSelectedTextStructs->operator[](i)->GetID());

	// 执行命令。
	Matrix2D transformMatrix;
	transformMatrix.RotateBy(pPatternLoops_willRotate->GetRectBox().GetCenterPt(), dRotateAngle);
	ICommandPtr pCommand(new CanvasLoopRotateCommand(GetEditorData(), pPartCadData, pPartCamData, patternLoops_willRotate, textStructs_willMove, transformMatrix));
	pCommand->Do();
	CommandManager::GetInstance()->AddCommand(pCommand);

	ClWindowFinder::GetCurrentView_1()->SendMessage(WM_updateSheetPreviewData, 0, 0);
	ClWindowFinder::GetCurrentView_2()->SendMessage(WM_redrawSheetThumbnailOnSheetManagementPanel, 0, 0);
	if (CanvasPatternPropertyBar::GetInstance()->IsVisible()) {
		CanvasPatternPropertyBar::GetInstance()->UpdateProp();
	}
}

END_CUTLEADER_NAMESPACE()

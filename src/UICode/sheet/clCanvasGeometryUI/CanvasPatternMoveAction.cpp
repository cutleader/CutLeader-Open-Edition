#include "StdAfx.h"
#include "CanvasPatternMoveAction.h"

#include "GlViewPort.h"
#include "CommandManager.h"
#include "CursorDrawer.h"
#include "clUILibResource.h"
#include "ClWindowFinder.h"
#include "ValueInputManager.h"

#include "DisplayParam.h"
#include "CLOptionInfo.h"
#include "SheetEditParam.h"

#include "PartCamData.h"
#include "Part.h"
#include "PartPlacement.h"
#include "clPartLayoutResource.h"
#include "clGeometryFeatureResource.h"
#include "PatternList.h"
#include "PatternDrawer.h"
#include "CanvasLoopMoveCommand.h"
#include "CanvasGeometryHelper.h"
#include "PatternLoopList.h"
#include "clCanvasGeometryUIResource.h"
#include "TextStruct.h"
#include "TextStructList.h"
#include "GeometryFeatureHelper.h"
#include "PartCadData.h"
#include "SelectPatternOrLoopActionBar.h"
#include "CanvasZoneList.h"
#include "LoopPositionDetectActionBar.h"
#include "LoopToolDataList.h"

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

CanvasPatternMoveAction::CanvasPatternMoveAction(PartPlacementPtr pPartPlacement, PatternListPtr pSelectedPatterns, TextStructListPtr pSelectedTextStructs, const Point2D& referencePt,
	SelectPatternOrLoopActionBar* pSelectPatternOrLoopActionBar, LoopPositionDetectActionBar* pLoopPositionDetectActionBar, GlViewPortPtr pViewPort)
    : CanvasPatternPreSelectAction(pPartPlacement, pSelectPatternOrLoopActionBar, pViewPort), LoopPositionDetector(m_pViewPort, pLoopPositionDetectActionBar)
{
	m_pSelectedPatterns = pSelectedPatterns;
	m_loopsOfSelectedPatterns = GeometryFeatureHelper::FindPatternLoopsOfPatterns(m_pPartPlacement->GetPart()->GetCadData().get(), m_pSelectedPatterns.get());
	m_pSelectedTextStructs = pSelectedTextStructs;

	if (pSelectedPatterns->size() > 0) {
		m_iMsgID = IDS_OUTPUT_MOVEPAT_NEWLOCATION;
		PatternLoopListPtr pSelectedPatLoops = GeometryFeatureHelper::FindAffectedPatternLoops(m_pPartPlacement->GetPart()->GetCadData().get(), m_pSelectedPatterns.get());
		LoopToolDataListPtr pToolDataOfMovingPatLoops(new LoopToolDataList);
		UpdateDataBeforeMove(MoveExsitingLoop, m_pPartPlacement.get(), pSelectedPatLoops, pToolDataOfMovingPatLoops, referencePt);
		m_pSelectPatternOrLoopActionBar->HideActionBar();
		m_pLoopPositionDetectActionBar->Show_LoopPositionDetectActionBar();
	}
	else {
		m_iMsgID = IDS_OUTPUT_SelectCanvasGeometry2Move;
		m_pSelectPatternOrLoopActionBar->DisplayActionBar(OnlyEnableLoopSelect);
	}
}

CanvasPatternMoveAction::~CanvasPatternMoveAction(void)
{
}

BOOL CanvasPatternMoveAction::LButtonDown(double& dPtX, double& dPtY)
{
	if (m_pSelectedPatterns->size() == 0) {
		CanvasPatternPreSelectAction::LButtonDown(dPtX, dPtY);
	}
	else
	{
		// 要移动的几何轮廓。
		vector<LONGLONG> patternLoops_willMove;
		PartCadDataPtr pPartCadData = m_pPartPlacement->GetPart()->GetCadData();
		PatternLoopListPtr pPatternLoops_willMove = GeometryFeatureHelper::FindAffectedPatternLoops(pPartCadData.get(), m_pSelectedPatterns.get());
		for (unsigned int i = 0; i < pPatternLoops_willMove->size(); i++)
			patternLoops_willMove.push_back(pPatternLoops_willMove->operator[](i)->GetID());

		// 要移动的文字。
		vector<LONGLONG> textStructs_willMove;
		for (unsigned int i = 0; i < m_pSelectedTextStructs->size(); i++)
			textStructs_willMove.push_back(m_pSelectedTextStructs->operator[](i)->GetID());

		// 执行命令。
		PartCamDataPtr pPartCamData = m_pPartPlacement->GetCamData();
		ICommandPtr pCommand(new CanvasLoopMoveCommand(GetEditorData(), pPartCadData, pPartCamData, patternLoops_willMove, textStructs_willMove, GetTransformMatrix()));
		pCommand->Do();
		CommandManager::GetInstance()->AddCommand(pCommand);

		PostMessage2EnterCanvasLoopSelectAction_PreSelectLoopAndText(pPatternLoops_willMove.get(), m_pSelectedTextStructs.get()); // 选中编辑的几何轮廓。
		ClWindowFinder::GetCurrentView_1()->SendMessage(WM_updateSheetPreviewData, 0, 0);
		ClWindowFinder::GetCurrentView_2()->SendMessage(WM_redrawSheetThumbnailOnSheetManagementPanel, 0, 0);
	}

	return TRUE;
}

BOOL CanvasPatternMoveAction::LButtonUp(double& dPtX, double& dPtY)
{
	if (m_pSelectedPatterns->size() == 0)
	{
		CanvasPatternPreSelectAction::LButtonUp(dPtX, dPtY);

		if (m_pSelectedPatterns->size() > 0) {
			PatternLoopListPtr pSelectedPatLoops = GeometryFeatureHelper::FindAffectedPatternLoops(m_pPartPlacement->GetPart()->GetCadData().get(), m_pSelectedPatterns.get());
			Point2D referencePt = pSelectedPatLoops->GetRectBox().GetCenterPt();
			LoopToolDataListPtr pToolDataOfMovingPatLoops(new LoopToolDataList);
			UpdateDataBeforeMove(MoveExsitingLoop, m_pPartPlacement.get(), pSelectedPatLoops, pToolDataOfMovingPatLoops, referencePt);
			m_pSelectPatternOrLoopActionBar->HideActionBar();
			m_pLoopPositionDetectActionBar->Show_LoopPositionDetectActionBar();
		}
	}

	return TRUE;
}

BOOL CanvasPatternMoveAction::MovePoint(double& dPtX, double& dPtY)
{
	if (m_pSelectedPatterns->size() == 0) {
		__super::MovePoint(dPtX, dPtY);
	}
	else {
		UpdateDataWhenMove(Point2D(dPtX, dPtY));
	}

	return TRUE;
}

SnapInputPtr CanvasPatternMoveAction::GetSnapInput() const
{
	SnapInputPtr pSnapInput = SnapInput::BuildSnapInput4SnapNone();
	return pSnapInput;
}

BOOL CanvasPatternMoveAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	__super::OnKeyDown(nChar, nRepCnt, nFlags);

	if (m_pSelectedPatterns->size() > 0) {
		UpdateDataWhenRotate(nChar);
	}

	return TRUE;
}

BOOL CanvasPatternMoveAction::DrawBeforeFinish()
{
	const DisplayParam* pDisplayParam = CLOptionInfo::GetInstance()->GetDisplayParam().get();
	PatternDrawer patDrawer(m_pViewPort);

	if (m_pSelectedPatterns->size() == 0) {
		CanvasPatternPreSelectAction::DrawBeforeFinish();
	}
	else {
		// draw the selected patterns.
		DrawSelectedPatterns(m_pSelectedPatterns.get(), m_loopsOfSelectedPatterns);

		const PatternList* pTmpPatterns = this->GetMovingPatterns();

		// 绘制牵引线。
		COLORREF dwOldColor;
		m_pViewPort->GetDrawColor(dwOldColor);
		m_pViewPort->SetDrawColor(*pDisplayParam->GetDrawAssistColor());
		Point2D centerPtBeforeMove = m_pSelectedPatterns->GetRectBox().GetCenterPt();
		Point2D centerPtAfterMove = pTmpPatterns->GetRectBox().GetCenterPt();
		m_pViewPort->DrawLinePoint2D(centerPtBeforeMove.X(), centerPtBeforeMove.Y(), centerPtAfterMove.X(), centerPtAfterMove.Y());
		m_pViewPort->SetDrawColor(dwOldColor);

		// 绘制移动中的几何特征。
		patDrawer.DrawPatList(pTmpPatterns);

		// 绘制辅助信息。
		DrawAssistInfo(_T(""));

		// draw cursor.
		CursorDrawer::DrawCrossLineCursor(m_pViewPort);
	}

	return TRUE;
}

void CanvasPatternMoveAction::UpdateForUndoRedo()
{
}

END_CUTLEADER_NAMESPACE()

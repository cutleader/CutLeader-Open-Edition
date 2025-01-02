#include "StdAfx.h"
#include "CanvasPatternCopyAction.h"

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
#include "CanvasLoopCopyCommand.h"
#include "CanvasGeometryHelper.h"
#include "clCanvasGeometryUIResource.h"
#include "TextStructList.h"
#include "TextStruct.h"
#include "GeometryFeatureHelper.h"
#include "PartCadData.h"
#include "SelectPatternOrLoopActionBar.h"
#include "PatternLoopList.h"
#include "LoopToolDataList.h"

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

CanvasPatternCopyAction::CanvasPatternCopyAction(PartPlacementPtr pPartPlacement, PatternListPtr pSelectedPatterns, TextStructListPtr pSelectedTextStructs,
	PatternLoopListPtr pAffectedPatternLoops, LoopTopologyItemListPtr pLoopTopologyItems, const Point2D& referencePt,
	SelectPatternOrLoopActionBar* pSelectPatternOrLoopActionBar, LoopPositionDetectActionBar* pLoopPositionDetectActionBar, GlViewPortPtr pViewPort)
    : CanvasPatternPreSelectAction(pPartPlacement, pSelectPatternOrLoopActionBar, pViewPort), LoopPositionDetector(m_pViewPort, pLoopPositionDetectActionBar)
{
	m_pSelectedPatterns = pSelectedPatterns;
	m_loopsOfSelectedPatterns = GeometryFeatureHelper::FindPatternLoopsOfPatterns(m_pPartPlacement->GetPart()->GetCadData().get(), m_pSelectedPatterns.get());
	m_pSelectedTextStructs = pSelectedTextStructs;

	if (pSelectedPatterns->size() > 0) {
		m_iMsgID = IDS_OUTPUT_COPYPAT_NEWLOCATION;
		LoopToolDataListPtr pToolDataOfMovingPatLoops(new LoopToolDataList);
		UpdateDataBeforeMove_2(CopyExsitingLoop, m_pPartPlacement.get(), pAffectedPatternLoops, pLoopTopologyItems.get(), pToolDataOfMovingPatLoops, referencePt);
		m_pLoopPositionDetectActionBar->Show_LoopPositionDetectActionBar();
	}
	else {
		m_iMsgID = IDS_OUTPUT_SelectCanvasGeometry2Copy;
		m_pSelectPatternOrLoopActionBar->DisplayActionBar(OnlyEnableLoopSelect);
	}
}

CanvasPatternCopyAction::~CanvasPatternCopyAction(void)
{
}

BOOL CanvasPatternCopyAction::LButtonDown(double& dPtX, double& dPtY)
{
	if (m_pSelectedPatterns->size() == 0)
	{
		CanvasPatternPreSelectAction::LButtonDown(dPtX, dPtY);
	}
	else
	{
		// 要拷贝的文字。
		vector<LONGLONG> textStructs_willCopy;
		for (unsigned int i = 0; i < m_pSelectedTextStructs->size(); i++)
			textStructs_willCopy.push_back(m_pSelectedTextStructs->operator[](i)->GetID());

		// 执行命令。
		PartCadDataPtr pPartCadData = m_pPartPlacement->GetPart()->GetCadData();
		PartCamDataPtr pPartCamData = m_pPartPlacement->GetCamData();
		PatternLoopListPtr pPatternLoops_willCopy = GeometryFeatureHelper::FindAffectedPatternLoops(pPartCadData.get(), m_pSelectedPatterns.get());
		CanvasLoopCopyCommandPtr pCanvasLoopCopyCommand(new CanvasLoopCopyCommand(GetEditorData(), pPartCadData, pPartCamData, pPatternLoops_willCopy,
			textStructs_willCopy, GetTransformMatrix()));
		pCanvasLoopCopyCommand->Do();
		CommandManager::GetInstance()->AddCommand(pCanvasLoopCopyCommand);

		// 为后续的距离检测功能做准备。
		{
			tuple<PatternListPtr, PatternLoopListPtr, TextStructListPtr> newCadData = pCanvasLoopCopyCommand->GetNewCadData();

			// 重置一些变量。
			m_pSelectedPatterns = get<0>(newCadData);
			m_loopsOfSelectedPatterns = GeometryFeatureHelper::FindPatternLoopsOfPatterns(pPartCadData.get(), m_pSelectedPatterns.get());
			m_pSelectedTextStructs = get<2>(newCadData);

			// 初始化距离检测功能。
			LoopToolDataListPtr pToolDataOfMovingPatLoops(new LoopToolDataList);
			Point2D referencePt = get<1>(newCadData)->GetRectBox().GetCenterPt();
			UpdateDataBeforeMove(CopyExsitingLoop, m_pPartPlacement.get(), get<1>(newCadData), pToolDataOfMovingPatLoops, referencePt);
		}

		// 做一些更新。
		ClWindowFinder::GetCurrentView_1()->SendMessage(WM_updateSheetPreviewData, 0, 0);
		ClWindowFinder::GetCurrentView_2()->SendMessage(WM_redrawSheetThumbnailOnSheetManagementPanel, 0, 0);
	}

	return TRUE;
}

BOOL CanvasPatternCopyAction::LButtonUp(double& dPtX, double& dPtY)
{
	if (m_pSelectedPatterns->size() == 0)
	{
		CanvasPatternPreSelectAction::LButtonUp(dPtX, dPtY);
		if (m_pSelectedPatterns->size() > 0) {
			PatternLoopListPtr pSelectedPatLoops = GeometryFeatureHelper::FindAffectedPatternLoops(m_pPartPlacement->GetPart()->GetCadData().get(), m_pSelectedPatterns.get());
			Point2D referencePt = pSelectedPatLoops->GetRectBox().GetCenterPt();
			LoopToolDataListPtr pToolDataOfMovingPatLoops(new LoopToolDataList);
			UpdateDataBeforeMove(CopyExsitingLoop, m_pPartPlacement.get(), pSelectedPatLoops, pToolDataOfMovingPatLoops, referencePt);
			m_pSelectPatternOrLoopActionBar->HideActionBar();
			m_pLoopPositionDetectActionBar->Show_LoopPositionDetectActionBar();
		}
	}

	return TRUE;
}

BOOL CanvasPatternCopyAction::MovePoint(double& dPtX, double& dPtY)
{
	if (m_pSelectedPatterns->size() == 0) { // 还没选中几何特征
		__super::MovePoint(dPtX, dPtY);
	}
	else {
		UpdateDataWhenMove(Point2D(dPtX, dPtY));
	}

	return TRUE;
}

SnapInputPtr CanvasPatternCopyAction::GetSnapInput() const
{
	SnapInputPtr pSnapInput = SnapInput::BuildSnapInput4SnapNone();
	return pSnapInput;
}

BOOL CanvasPatternCopyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	__super::OnKeyDown(nChar, nRepCnt, nFlags);

	if (m_pSelectedPatterns->size() > 0) {
		UpdateDataWhenRotate(nChar);
	}

	return TRUE;
}

BOOL CanvasPatternCopyAction::DrawBeforeFinish()
{
	const DisplayParam* pDisplayParam = CLOptionInfo::GetInstance()->GetDisplayParam().get();
	PatternDrawer patDrawer(m_pViewPort);

	if (m_pSelectedPatterns->size() == 0) {
		CanvasPatternPreSelectAction::DrawBeforeFinish();
	}
	else {
		// draw the selected patterns.
		DrawSelectedPatterns(m_pSelectedPatterns.get(), m_loopsOfSelectedPatterns);

		// 绘制移动中的几何特征。
		const PatternList* pTmpPatterns = this->GetMovingPatterns();
		patDrawer.DrawPatList(pTmpPatterns);

		// 绘制辅助信息。
		DrawAssistInfo(_T(""));

		// draw cursor.
		CursorDrawer::DrawCrossLineCursor(m_pViewPort);
	}

	return TRUE;
}

void CanvasPatternCopyAction::UpdateForUndoRedo()
{
	// 回到选择轮廓的状态。
	m_iMsgID = IDS_OUTPUT_SelectCanvasGeometry2Copy;
	m_pSelectedPatterns->clear();
	m_loopsOfSelectedPatterns.clear();
	m_pLoopPositionDetectActionBar->HideActionBar();
	m_pSelectPatternOrLoopActionBar->DisplayActionBar(OnlyEnableLoopSelect);
}

END_CUTLEADER_NAMESPACE()

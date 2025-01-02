#include "StdAfx.h"
#include "AddPatternsFromDxfdwgAction.h"

#include "Point2D.h"
#include "GlViewPort.h"
#include "CommandManager.h"
#include "CursorDrawer.h"
#include "clBaseDataResource.h"
#include "DataBaseManager.h"
#include "ClWindowFinder.h"
#include "ValueInputManager.h"

#include "DisplayParam.h"
#include "CLOptionInfo.h"
#include "SheetEditParam.h"

#include "Part.h"
#include "clGeometryFeatureResource.h"
#include "clPartLayoutResource.h"
#include "CanvasLoopAddCommand.h"
#include "PatternDrawer.h"
#include "PatternList.h"
#include "PartPlacement.h"
#include "PatternLoopList.h"
#include "LoopProcessor.h"
#include "TextStructList.h"
#include "TextStruct.h"
#include "CutFeatureHelper.h"
#include "CanvasPatternCopyAction.h"

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

AddPatternsFromDxfdwgAction::AddPatternsFromDxfdwgAction(PartPlacementPtr pPartPlacement, PatternListPtr pNewPatterns, PatternLoopListPtr pNewPatternLoops,
	TextStructListPtr pNewTextStructs, LoopTopologyItemListPtr pNewLoopTopologyItems, const Point2D& referencePt, SelectPatternOrLoopActionBar* pSelectPatternOrLoopActionBar,
	LoopPositionDetectActionBar* pLoopPositionDetectActionBar, GlViewPortPtr pViewPort, const ToolAssignConfigItemPtr pToolAssignConfigItem)
	: CanvasGeometryEditAction(pPartPlacement, pViewPort), LoopPositionDetector(m_pViewPort, pLoopPositionDetectActionBar), m_pToolAssignConfigItem(pToolAssignConfigItem)
{
	m_pNewPatterns = pNewPatterns;
	m_pNewPatternLoops = pNewPatternLoops;
	m_pNewTextStructs = pNewTextStructs;
	m_pNewLoopTopologyItems = pNewLoopTopologyItems;
	m_iMsgID = IDS_OUTPUT_COPYPAT_NEWLOCATION;
	m_pSelectPatternOrLoopActionBar = pSelectPatternOrLoopActionBar;

	// 根据分配规则给零件的每个轮廓分配刀具。这里把pPatternLoops看成一个零件。
	LoopToolDataListPtr pLoopToolDataList = CutFeatureHelper::AssignLoopTools4Part(m_pNewPatternLoops.get(), m_pNewLoopTopologyItems.get(), m_pToolAssignConfigItem.get());
	UpdateDataBeforeMove_2(MoveNewLoop, m_pPartPlacement.get(), m_pNewPatternLoops, m_pNewLoopTopologyItems.get(), pLoopToolDataList, referencePt);
	m_pLoopPositionDetectActionBar->Show_LoopPositionDetectActionBar();
}

AddPatternsFromDxfdwgAction::~AddPatternsFromDxfdwgAction(void)
{
}

BOOL AddPatternsFromDxfdwgAction::LButtonDown(double& dPtX, double& dPtY)
{
	// 把数据移到指定位置。
	m_pNewPatterns->Transform(GetTransformMatrix());
	LoopProcessor::UpdateCacheEx(m_pNewPatternLoops);
	for (unsigned int i = 0; i < m_pNewTextStructs->size(); i++) {
		TextStructPtr pTextStruct = m_pNewTextStructs->operator[](i);
		Matrix2D newMatrix = pTextStruct->GetTransformMatrix();
		newMatrix *= GetTransformMatrix();
		pTextStruct->SetTransformMatrix(newMatrix);
	}

	// 把导入的数据加到底图上。
	PartCadDataPtr pPartCadData = m_pPartPlacement->GetPart()->GetCadData();
	PartCamDataPtr pPartCamData = m_pPartPlacement->GetCamData();
	ICommandPtr pCommand(new CanvasLoopAddCommand(GetEditorData(), pPartCadData, pPartCamData, m_pNewPatterns, m_pNewPatternLoops, m_pNewTextStructs));
	pCommand->Do();
	CommandManager::GetInstance()->AddCommand(pCommand);

	ClWindowFinder::GetCurrentView_1()->SendMessage(WM_updateSheetPreviewData, 0, 0);
	ClWindowFinder::GetCurrentView_2()->SendMessage(WM_redrawSheetThumbnailOnSheetManagementPanel, 0, 0);

	// 支持连续放置图元，所以这里启动了CanvasPatternCopyAction。
	Point2D referencePt = m_pNewPatterns->GetRectBox().GetCenterPt();
	CanvasPatternCopyActionPtr pCanvasPatternCopyAction(new CanvasPatternCopyAction(m_pPartPlacement, m_pNewPatterns, m_pNewTextStructs, m_pNewPatternLoops,
		m_pNewLoopTopologyItems, referencePt, m_pSelectPatternOrLoopActionBar, m_pLoopPositionDetectActionBar, m_pViewPort));
	m_pNextAction = pCanvasPatternCopyAction;

	return TRUE;
}

BOOL AddPatternsFromDxfdwgAction::MovePoint(double& dPtX, double& dPtY)
{
	UpdateDataWhenMove(Point2D(dPtX, dPtY));
	return TRUE;
}

BOOL AddPatternsFromDxfdwgAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CanvasGeometryEditAction::OnKeyDown(nChar, nRepCnt, nFlags);

	if (m_pNewPatterns->size() > 0) {
		UpdateDataWhenRotate(nChar);
	}

	return TRUE;
}

BOOL AddPatternsFromDxfdwgAction::DrawBeforeFinish()
{
	const DisplayParam* pDisplayParam = CLOptionInfo::GetInstance()->GetDisplayParam().get();
	PatternDrawer patDrawer(m_pViewPort);

	// 绘制移动中的几何特征。
	const PatternList* pTmpPatterns = this->GetMovingPatterns();
	patDrawer.DrawPatList(pTmpPatterns);

	// 绘制辅助信息。
	DrawAssistInfo(_T(""));

	CursorDrawer::DrawCrossLineCursor(m_pViewPort);
	return TRUE;
}

void AddPatternsFromDxfdwgAction::UpdateForUndoRedo()
{
}

END_CUTLEADER_NAMESPACE()

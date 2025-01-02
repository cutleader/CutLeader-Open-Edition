#include "StdAfx.h"
#include "StartCutPtChangeAction.h"

#include "MathEx.h"
#include "LineArc2DList.h"
#include "GlViewPort.h"
#include "CommandManager.h"
#include "CursorDrawer.h"
#include "ValueInputManager.h"
#include "CursorDrawer.h"
#include "clBaseDataResource.h"
#include "LogMgr.h"

#include "ParamConfig.h"
#include "CLOptionInfo.h"
#include "DisplayParam.h"
#include "LeadConfigItem.h"
#include "SnapPtParam.h"

#include "PatternList.h"
#include "IPatternLoop.h"
#include "PatternLoopList.h"
#include "LoopDrawer.h"
#include "LoopProcessor.h"
#include "GeometryStartCutPt.h"
#include "CornerStartCutPt.h"
#include "LoopStartCutCache.h"
#include "LoopStartCutData.h"
#include "clCutFeatureResource.h"
#include "LoopCutFeature.h"
#include "LoopCutFeatureList.h"
#include "LeadManager.h"
#include "StartCutPtModifyCommand.h"
#include "StartCutPtModifyCommandA.h"
#include "LoopStartCutProcessor.h"
#include "LoopToolDataList.h"
#include "LoopToolManager.h"

BEGIN_CUTLEADER_NAMESPACE()

StartCutPtChangeAction::StartCutPtChangeAction(PatternListPtr pPatList, PatternLoopListPtr pPatternLoopList, LoopTopologyItemListPtr pLoopTopologyItems, 
	LoopCutFeatureListPtr pLoopFeatureList, GlViewPortPtr pViewPort, LeadConfigItemPtr pLeadConfigItem)
     : CutFeatureEditBase(pPatList, pPatternLoopList, pLoopTopologyItems, pLoopFeatureList, pViewPort)
{
    m_iMsgID = IDS_MSG_AdjustStartCutPt;
	m_pLeadConfigItem = pLeadConfigItem;
}

StartCutPtChangeAction::~StartCutPtChangeAction(void)
{
}

BOOL StartCutPtChangeAction::LButtonDown(double& dPtX, double& dPtY)
{
	if (!m_pActivePatternLoop) {
		CString strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);
		MessageBox(m_pViewPort->GetWnd(), MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_AdjustStartCutPt), strProductName, MB_OK | MB_ICONWARNING);
		return FALSE;
	}

	// 得到新的起切点位置。
	int iLoopNodeIndex = INVALID_IDX;
	Point2D newStartCutPt = LoopProcessor::GetClosestPt(m_pActivePatternLoop.get(), Matrix2D(), Point2D(dPtX, dPtY), iLoopNodeIndex);

	// 如新的起切点坐标和老的位置一样，则不需要移动，也不提示客户。
	LoopCutFeaturePtr pLoopFeature = m_pLoopFeatureList->FindByPatternLoop(m_pActivePatternLoop);
	LoopStartCutDataPtr pLoopStartCutData = pLoopFeature->GetLoopStartCutData();
	if (newStartCutPt == pLoopStartCutData->GetCacheData()->GetStartCutPt()) {
		return FALSE;
	}

	IPatternPtr pPattern = m_pActivePatternLoop->GetPatternByLoopNodeIndex(iLoopNodeIndex);
	PatternPosition patternPosition = pPattern->GetPatPosByPoint(newStartCutPt, PatternPosition());
	if (LoopProcessor::PtAtLoopCorner(m_pActivePatternLoop.get(), newStartCutPt)) // 起切点移到了角上。
	{
		double dCornerAngle = LoopProcessor::GetAngAtLoopCorner(m_pActivePatternLoop.get(), newStartCutPt);
		if (!MathEx::Equal(dCornerAngle, PI, GEOM_TOLERANCE)) {
			// 得到角上相邻的几何特征。
			LONGLONG iNextPatID;
			PatternPosition nextPatPos;
			LoopProcessor::GetNextCornerGeomPos(m_pActivePatternLoop.get(), newStartCutPt, pPattern->GetID(), patternPosition, iNextPatID, nextPatPos);

			// 创建起切点数据。
			BOOL bSameDirWithLoop = pLoopStartCutData->SameDirWithLoop();
			CornerStartCutPtPtr pNewSECutPt(new CornerStartCutPt(pPattern->GetID(), patternPosition, iNextPatID, nextPatPos, bSameDirWithLoop));
            pNewSECutPt->SetOverCut(m_pLeadConfigItem->GetOverCutAtCorner());

			// 创建角上的引入引出线。
			PatternLoopCutOffsetType emLoopCutOffset = LoopStartCutProcessor::CalculateLoopCutOffset(pLoopStartCutData.get());
			double dLeadInLen = pLoopStartCutData->GetCacheData()->GetLeadInGeom()->GetTotalLength();
			double dLeadOutLen = pLoopStartCutData->GetCacheData()->GetLeadOutGeom()->GetTotalLength();
			ILeadPtr pLeadIn = LeadManager::BuildCornerLead_byLeadLength(m_pActivePatternLoop.get(), bSameDirWithLoop, emLoopCutOffset, LeadInOut_In, newStartCutPt, dLeadInLen);
			ILeadPtr pLeadOut = LeadManager::BuildCornerLead_byLeadLength(m_pActivePatternLoop.get(), bSameDirWithLoop, emLoopCutOffset, LeadInOut_Out, newStartCutPt, dLeadOutLen);

			// create the command to modify the start point of the loop.
			ICommandPtr pCommand(new StartCutPtModifyCommandA(GetEditorData(), pLoopStartCutData, pNewSECutPt, pLeadIn, pLeadOut));
			pCommand->Do();
			CommandManager::GetInstance()->AddCommand(pCommand);
		}
		else{ // 新的起切点落在图形端点上，该端点两侧的图形是相切的。
			StartPt2Geom(pLoopStartCutData, pPattern->GetID(), patternPosition); // 把起切点移到图形上。
		}
	}
	else { // 新起切点在图形上。
		StartPt2Geom(pLoopStartCutData, pPattern->GetID(), patternPosition);
	}

	return TRUE;
}

BOOL StartCutPtChangeAction::MovePoint(double& dPtX, double& dPtY)
{
	Point2D pt(dPtX, dPtY);

	// 得到距离光标最近的几何轮廓。
	IPatternLoopPtr pClosestPatternLoop;
	IPatternPtr pClosestPattern = m_pPatList->GetPatternClosestTo(pt);
	if (pClosestPattern) {
		pClosestPatternLoop = m_pPatternLoopList->FindLoopByPat(pClosestPattern.get());
	}

	// 得到当前捕获到的轮廓。
	m_pActivePatternLoop.reset();
	double dSnapRangeInThisView = CLOptionInfo::GetInstance()->GetSnapPtParam()->GetSnapRangeInThisView(m_pViewPort.get());
	if (pClosestPatternLoop && !pClosestPatternLoop->IsOpenPath() && pClosestPattern->DistanceTo(pt) < dSnapRangeInThisView) {
		m_pActivePatternLoop = pClosestPatternLoop;
	}

	return TRUE;
}

BOOL StartCutPtChangeAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CutFeatureEditBase::OnKeyDown(nChar, nRepCnt, nFlags);

	return TRUE;
}

SnapInputPtr StartCutPtChangeAction::GetSnapInput() const
{
	SnapInputPtr pSnapInput = SnapInput::BuildSnapInput4SnapNone();
	if (m_pActivePatternLoop) {
		PatternListPtr pPatternsInSelectedPatternLoop = m_pActivePatternLoop->GetPatternList();
		pSnapInput.reset(new SnapInput(SnapEndPtOf2DLineArc | SnapMiddlePtOf2DLineArc | SnapClosestPtTo2DLineArc, pPatternsInSelectedPatternLoop, Matrix2D()));
	}

	return pSnapInput;
}

BOOL StartCutPtChangeAction::DrawBeforeFinish()
{
	LoopDrawer loopDrawer(m_pViewPort);
	if (m_pActivePatternLoop) {
		const LoopToolData* pLoopToolData = m_pLoopFeatureList->GetLoopToolList()->GetLoopTool_by_loopID(m_pActivePatternLoop->GetID()).get();
		loopDrawer.DrawActiveLoop(m_pActivePatternLoop.get(), LoopToolManager::GetLoopDisplayColor(pLoopToolData));
	}

	// draw cursor.
	CursorDrawer::DrawCrossLineCursor(m_pViewPort);

	return TRUE;
}

void StartCutPtChangeAction::StartPt2Geom(LoopStartCutDataPtr pLoopStartCutData, LONGLONG iNewPatternID, const PatternPosition& newPatternPosition)
{
	if (pLoopStartCutData->GetLoopStartCutPt()->GetSECutPtType() == SECUT_PT_CORNER) { // 老起切点在角上。
		ILoopStartCutPtPtr pNewSECutPt(new GeometryStartCutPt(iNewPatternID, newPatternPosition, pLoopStartCutData->SameDirWithLoop()));
		const IPatternLoop* pPatternLoop = pLoopStartCutData->GetPatternLoop().get();
		PatternLoopCutOffsetType emLoopCutOffset = LoopStartCutProcessor::CalculateLoopCutOffset(pLoopStartCutData.get());
		ILeadPtr pLeadIn = LeadManager::BuildGeomLead_byLeadConfig(pPatternLoop, emLoopCutOffset, LeadInOut_In, m_pLeadConfigItem.get());
		ILeadPtr pLeadOut = LeadManager::BuildGeomLead_byLeadConfig(pPatternLoop, emLoopCutOffset, LeadInOut_Out, m_pLeadConfigItem.get());
		ICommandPtr pCommand(new StartCutPtModifyCommandA(GetEditorData(), pLoopStartCutData, pNewSECutPt, pLeadIn, pLeadOut));
		pCommand->Do();
		CommandManager::GetInstance()->AddCommand(pCommand);
	}
	else { // 老起切点在图形上。
		ICommandPtr pCommand(new StartCutPtModifyCommand(GetEditorData(), pLoopStartCutData, iNewPatternID, newPatternPosition));
		pCommand->Do();
		CommandManager::GetInstance()->AddCommand(pCommand);
	}
}

END_CUTLEADER_NAMESPACE()

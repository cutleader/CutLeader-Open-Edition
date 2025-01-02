#include "StdAfx.h"
#include "StartCutPtAdjustAction.h"

#include "MathEx.h"
#include "LineArc2DList.h"
#include "GlViewPort.h"
#include "CommandManager.h"
#include "CursorDrawer.h"
#include "ValueInputManager.h"
#include "DummyAction.h"
#include "CursorDrawer.h"
#include "clBaseDataResource.h"
#include "LogMgr.h"

#include "ParamConfig.h"
#include "CLOptionInfo.h"
#include "DisplayParam.h"
#include "LeadConfigItem.h"
#include "IMaterialSize.h"
#include "SnapPtParam.h"

#include "PatternList.h"
#include "PatternLoopNode.h"
#include "PatternLoopNodeList.h"
#include "IPatternLoop.h"
#include "PatternLoopList.h"
#include "IArrayPattern.h"
#include "LoopDrawer.h"
#include "LoopProcessor.h"
#include "ILead.h"
#include "GeometryStartCutPt.h"
#include "CornerStartCutPt.h"
#include "LoopStartCutCache.h"
#include "LoopStartCutData.h"
#include "LoopStartCutDataList.h"
#include "clCutFeatureResource.h"
#include "LoopCutFeatureDrawer.h"
#include "LeadManager.h"
#include "StartCutPtModifyCommand.h"
#include "StartCutPtModifyCommandA.h"
#include "StartCutPt2GeometryCommand.h"
#include "StartCutPt2CornerCommand.h"
#include "LoopStartCutProcessor.h"
#include "LoopToolManager.h"
#include "LoopInstanceCache.h"
#include "LoopInstance.h"
#include "PartInstanceList.h"
#include "LoopInstanceManager.h"
#include "LoopInstanceList.h"

BEGIN_CUTLEADER_NAMESPACE()

StartCutPtAdjustAction::StartCutPtAdjustAction(IMaterialSizePtr pMaterialSize, PartPlacementListPtr pPartPlacements, PartInstanceListPtr pPartInstances,
	PartGroupListPtr pPartGroups, GlViewPortPtr pViewPort, LeadConfigItemPtr pLeadConfigItem, SpreadSamePartActionBar* pSpreadSamePartActionBar)
    : PartPlacementEditAction(pMaterialSize, pPartPlacements, pPartInstances, pPartGroups, pViewPort)
{
	m_pLeadConfigItem = pLeadConfigItem;
	m_pSpreadSamePartActionBar = pSpreadSamePartActionBar;
	m_iMsgID = IDS_MSG_AdjustStartCutPt;
	m_pAllLoopInstances = LoopInstanceManager::GetLoopInstanceList(pPartInstances);

	if (m_pMatSize->GetType() != MAT_SIZE_Canvas) {
		pSpreadSamePartActionBar->Show_SpreadSamePartActionBar();
	}
}

StartCutPtAdjustAction::~StartCutPtAdjustAction(void)
{
}

BOOL StartCutPtAdjustAction::LButtonDown(double& dPtX, double& dPtY)
{
	if (!m_pActiveLoopInstance) {
		CString strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);
		MessageBox(m_pViewPort->GetWnd(), MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_AdjustStartCutPt), strProductName, MB_OK | MB_ICONWARNING);
		return FALSE;
	}

	LoopStartCutDataPtr pLoopStartCutData = m_pActiveLoopInstance->GetCacheData()->GetLoopStartCutData();
	const IPatternLoop* pPatternLoop = pLoopStartCutData->GetPatternLoop().get();

	// 得到新的起切点位置(零件坐标系下)。
	int iLoopNodeIndex = INVALID_IDX;
	Point2D newStartCutPt = LoopProcessor::GetClosestPt(pPatternLoop, m_pActiveLoopInstance->GetTransformMatrix(), Point2D(dPtX, dPtY), iLoopNodeIndex);
	newStartCutPt = newStartCutPt.InverseTransform(m_pActiveLoopInstance->GetTransformMatrix()); // 转到零件坐标系下面。

	// 如新的起切点坐标和老的位置一样，则不需要移动，也不提示客户。
	if (newStartCutPt == pLoopStartCutData->GetCacheData()->GetStartCutPt()) {
		return FALSE;
	}

	IPatternPtr pPattern = pPatternLoop->GetPatternByLoopNodeIndex(iLoopNodeIndex);
	PatternPosition patternPosition = pPattern->GetPatPosByPoint(newStartCutPt, PatternPosition());
	if (LoopProcessor::PtAtLoopCorner(pLoopStartCutData->GetPatternLoop().get(), newStartCutPt)) { // 新的起切点在角上。
		double dCornerAngle = LoopProcessor::GetAngAtLoopCorner(pLoopStartCutData->GetPatternLoop().get(), newStartCutPt);
		if (MathEx::Equal(dCornerAngle, PI, GEOM_TOLERANCE)) { // 新的起切点落在图形端点上，该端点两侧的图形是相切的。
			StartPt2Geom(pLoopStartCutData, pPattern->GetID(), patternPosition); // 把起切点移到图形上。
		}
		else {
			// 得到角上相邻的几何特征。
			LONGLONG iNextPatID;
			PatternPosition nextPatPos;
			LoopProcessor::GetNextCornerGeomPos(pPatternLoop, newStartCutPt, pPattern->GetID(), patternPosition, iNextPatID, nextPatPos);

			// 创建起切点数据。
			BOOL bSameDirWithLoop = pLoopStartCutData->SameDirWithLoop();
			CornerStartCutPtPtr pNewSECutPt(new CornerStartCutPt(pPattern->GetID(), patternPosition, iNextPatID, nextPatPos, bSameDirWithLoop));
			pNewSECutPt->SetOverCut(m_pLeadConfigItem->GetOverCutAtCorner());

			// 创建角上的引入引出线。
			PatternLoopCutOffsetType emLoopCutOffset = LoopStartCutProcessor::CalculateLoopCutOffset(pLoopStartCutData.get());
			double dLeadInLen = pLoopStartCutData->GetCacheData()->GetLeadInGeom()->GetTotalLength();
			double dLeadOutLen = pLoopStartCutData->GetCacheData()->GetLeadOutGeom()->GetTotalLength();
			ILeadPtr pLeadIn = LeadManager::BuildCornerLead_byLeadLength(pPatternLoop, bSameDirWithLoop, emLoopCutOffset, LeadInOut_In, newStartCutPt, dLeadInLen);
			ILeadPtr pLeadOut = LeadManager::BuildCornerLead_byLeadLength(pPatternLoop, bSameDirWithLoop, emLoopCutOffset, LeadInOut_Out, newStartCutPt, dLeadOutLen);

			// 把起切点移到角上。
			StartPt2Corner(pLoopStartCutData, pNewSECutPt, pLeadIn, pLeadOut);
		}
	}
	else { // 新的起切点在图形上。
		StartPt2Geom(pLoopStartCutData, pPattern->GetID(), patternPosition);
	}

	return TRUE;
}

BOOL StartCutPtAdjustAction::MovePoint(double& dPtX, double& dPtY)
{
	Point2D pt(dPtX, dPtY);

	// 得到当前捕获到的轮廓实例。
	m_pActiveLoopInstance.reset();
	double dSnapRangeInThisView = CLOptionInfo::GetInstance()->GetSnapPtParam()->GetSnapRangeInThisView(m_pViewPort.get());
	LoopInstancePtr pLoopInstance = LoopInstanceManager::GetClosestLoopInst(m_pAllLoopInstances, pt);
	if (pLoopInstance && !pLoopInstance->GetCacheData()->GetPatternLoop()->IsOpenPath() && pLoopInstance->DistanceTo(pt) < dSnapRangeInThisView) {
		m_pActiveLoopInstance = pLoopInstance;
	}

	return TRUE;
}

BOOL StartCutPtAdjustAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	return __super::OnKeyDown(nChar, nRepCnt, nFlags);
}

SnapInputPtr StartCutPtAdjustAction::GetSnapInput() const
{
	SnapInputPtr pSnapInput = SnapInput::BuildSnapInput4SnapNone();

	if (m_pActiveLoopInstance) {
		PatternListPtr pPatternsInSelectedLoopInstance = m_pActiveLoopInstance->GetCacheData()->GetPatternLoop()->GetPatternList();
		Matrix2D transformMatrix = m_pActiveLoopInstance->GetTransformMatrix();
		pSnapInput.reset(new SnapInput(SnapEndPtOf2DLineArc | SnapMiddlePtOf2DLineArc | SnapClosestPtTo2DLineArc, pPatternsInSelectedLoopInstance, transformMatrix));
	}

	return pSnapInput;
}

BOOL StartCutPtAdjustAction::DrawBeforeFinish()
{
	LoopDrawer loopDrawer(m_pViewPort);
	if (m_pActiveLoopInstance) {
		COLORREF iColor = LoopToolManager::GetLoopDisplayColor(m_pActiveLoopInstance->GetCacheData()->GetLoopTool().get());
		loopDrawer.DrawActiveLoop(m_pActiveLoopInstance->GetCacheData()->GetPatternLoop().get(), iColor, m_pActiveLoopInstance->GetTransformMatrix());
	}

	// draw cursor.
	CursorDrawer::DrawCrossLineCursor(m_pViewPort);

	return TRUE;
}

void StartCutPtAdjustAction::StartPt2Corner(LoopStartCutDataPtr pLoopStartCutData, ILoopStartCutPtPtr pLoopStartCutPt, ILeadPtr pLeadIn, ILeadPtr pLeadOut)
{
	if (m_pSpreadSamePartActionBar->IsSpread()) {
		LoopStartCutDataListPtr pLoopStaEndCuts(new LoopStartCutDataList);
		for (unsigned int i = 0; i < m_pAllLoopInstances->size(); i++) {
			LoopInstancePtr pLoopInstance =  m_pAllLoopInstances->at(i);
			if (pLoopInstance->GetCacheData()->GetPatternLoop()->GetID() == pLoopStartCutData->GetPatternLoop()->GetID()) {
				pLoopStaEndCuts->push_back(pLoopInstance->GetCacheData()->GetLoopStartCutData());
			}
		}

		ICommandPtr pCommand(new StartCutPt2CornerCommand(GetEditorData(), pLoopStaEndCuts, pLoopStartCutPt, pLeadIn, pLeadOut));
		pCommand->Do();
		CommandManager::GetInstance()->AddCommand(pCommand);
	}
	else {
		ICommandPtr pCommand(new StartCutPtModifyCommandA(GetEditorData(), pLoopStartCutData, pLoopStartCutPt, pLeadIn, pLeadOut));
		pCommand->Do();
		CommandManager::GetInstance()->AddCommand(pCommand);
	}
}

void StartCutPtAdjustAction::StartPt2Geom(LoopStartCutDataPtr pLoopStartCutData, LONGLONG iNewPatID, const PatternPosition& newPatPos)
{
	// 如果有起切点从角移到图形上，需要这些变量。
	ILoopStartCutPtPtr pNewSECutPt(new GeometryStartCutPt(iNewPatID, newPatPos, pLoopStartCutData->SameDirWithLoop()));
	IPatternLoopPtr pPatternLoop = pLoopStartCutData->GetPatternLoop();
	PatternLoopCutOffsetType emLoopCutOffset = LoopStartCutProcessor::CalculateLoopCutOffset(pLoopStartCutData.get());
	ILeadPtr pNewLeadIn = LeadManager::BuildGeomLead_byLeadConfig(pPatternLoop.get(), emLoopCutOffset, LeadInOut_In, m_pLeadConfigItem.get());
	ILeadPtr pNewLeadOut = LeadManager::BuildGeomLead_byLeadConfig(pPatternLoop.get(), emLoopCutOffset, LeadInOut_Out, m_pLeadConfigItem.get());

	if (!m_pSpreadSamePartActionBar->IsSpread()) {
		ILoopStartCutPtPtr pLoopStartCutPt = pLoopStartCutData->GetLoopStartCutPt();
		if (pLoopStartCutPt->GetSECutPtType() == SECUT_PT_CORNER) { // 起切点从角移到图形上。
			ICommandPtr pCommand(new StartCutPtModifyCommandA(GetEditorData(), pLoopStartCutData, pNewSECutPt, pNewLeadIn, pNewLeadOut));
			pCommand->Do();
			CommandManager::GetInstance()->AddCommand(pCommand);
		}
		else { // 起切点从图形移到图形上。
			ICommandPtr pCommand(new StartCutPtModifyCommand(GetEditorData(), pLoopStartCutData, iNewPatID, newPatPos));
			pCommand->Do();
			CommandManager::GetInstance()->AddCommand(pCommand);
		}
	}
	else {
		// 所有要调整的“回路起切特征”。
		LoopStartCutDataListPtr pGeomStaEndCuts(new LoopStartCutDataList); // 起切点在图形上的。
		LoopStartCutDataListPtr pCornerStaEndCuts(new LoopStartCutDataList); // 起切点在角上的。
		for (unsigned int i = 0; i < m_pAllLoopInstances->size(); i++) {
			LoopInstancePtr pLoopInstance =  m_pAllLoopInstances->at(i);
			if (pLoopInstance->GetCacheData()->GetPatternLoop()->GetID() == pLoopStartCutData->GetPatternLoop()->GetID()) {
				LoopStartCutDataPtr pTmpLoopStaEndCut = pLoopInstance->GetCacheData()->GetLoopStartCutData();
				if (pTmpLoopStaEndCut->GetLoopStartCutPt()->GetSECutPtType() == SECUT_PT_GEOM)
					pGeomStaEndCuts->push_back(pTmpLoopStaEndCut);
				else
					pCornerStaEndCuts->push_back(pTmpLoopStaEndCut);
			}
		}

		// 执行命令。
		ICommandPtr pCommand(new StartCutPt2GeometryCommand(GetEditorData(), pGeomStaEndCuts, iNewPatID, newPatPos, pCornerStaEndCuts, pNewSECutPt, pNewLeadIn, pNewLeadOut));
		pCommand->Do();
		CommandManager::GetInstance()->AddCommand(pCommand);
	}
}

END_CUTLEADER_NAMESPACE()

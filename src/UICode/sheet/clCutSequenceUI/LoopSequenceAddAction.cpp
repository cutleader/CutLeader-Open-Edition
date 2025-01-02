#include "StdAfx.h"
#include "LoopSequenceAddAction.h"

#include "GlViewPort.h"
#include "Line2D.h"
#include "LineArc2DList.h"
#include "CommandManager.h"
#include "DummyAction.h"
#include "CursorDrawer.h"
#include "ValueInputManager.h"
#include "clBaseDataResource.h"

#include "SequenceConfigItem.h"
#include "IMaterialSize.h"
#include "CLOptionInfo.h"
#include "DisplayParam.h"

#include "IPatternLoop.h"
#include "PatternLoopCache.h"
#include "LoopDrawer.h"
#include "PartPlacementList.h"
#include "PartInstance.h"
#include "PartInstanceList.h"
#include "LoopInstanceList.h"
#include "LoopInstanceCache.h"
#include "LoopInstance.h"
#include "LoopInstanceManager.h"
#include "PartPlacementDrawer.h"
#include "PartPlacementManager.h"
#include "LoopToolData.h"
#include "clCutSequenceResource.h"
#include "CutNode.h"
#include "CutNodeList.h"
#include "LoopSequence.h"
#include "LoopSequenceManager.h"
#include "CutSequenceList.h"
#include "CutSequenceDrawer.h"
#include "LoopSequenceAddCommand.h"
#include "PartLayoutData.h"
#include "CutNodeManager.h"
#include "CutSequenceSelectAction.h"
#include "ConfigData.h"
#include "LoopToolManager.h"
#include "SequenceData.h"

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

LoopSequenceAddAction::LoopSequenceAddAction(PartLayoutDataPtr pPartLayoutData, SequenceDataPtr pSequenceData, ConfigDataPtr pConfigData,
	GlViewPortPtr pViewPort, ApplyPartActionBar* pApplyPartActionBar)
	: SequenceEditActionBase(pPartLayoutData, pSequenceData, pConfigData, pViewPort)
{
	m_pApplyPartActionBar = pApplyPartActionBar;
	m_iMsgID = IDS_SEQ_ADD_LOOPSEQ;

	// 得到所有“回路实例”。
	m_pLoopInstanceList = LoopInstanceManager::GetLoopInstanceList(m_pPartLayoutData->GetPartInstances());

	// 计算零件实例中各个回路实例的外接矩形。
	m_partRectMap = m_pPartLayoutData->GetPartInstances()->GetLoopInstanceRect();

	if (m_pConfigData->GetMatSize()->GetType() != MAT_SIZE_Canvas) {
		m_pApplyPartActionBar->Show_ApplyPartActionBar();
	}
}

LoopSequenceAddAction::~LoopSequenceAddAction(void)
{
}

BOOL LoopSequenceAddAction::LButtonDown(double& dPtX, double& dPtY)
{
	CString strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);

	if (m_pConfigData->GetMatSize()->GetType() != MAT_SIZE_Canvas && m_pApplyPartActionBar->IsApplyPart())
	{
		if (m_pActivePartInst) {
            if (PartPlacementManager::AllLoopToolsDisabled(m_pActivePartInst.get())) { // 这个零件实例是“不切割”的。
                MessageBox(m_pViewPort->GetWnd(), MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_PartNoCutting), strProductName, MB_OK | MB_ICONWARNING);
            }
            else
            {
                // 检查这个零件是不是已经分配了工序。
                BOOL bPartSequenced = m_pSequenceData->GetCutSequences()->IsPartInstanceSequenced(m_pActivePartInst);
                if (bPartSequenced) {
                    MessageBox(m_pViewPort->GetWnd(), MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_PART_SEQUENCED), strProductName, MB_OK | MB_ICONWARNING);
                }
                else { // 给零件实例分配工序。
                    AssignPartSeq(m_pActivePartInst);
                }
            }
		}
		else {
			MessageBox(m_pViewPort->GetWnd(), MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_SELECT_PARTINST), strProductName, MB_OK | MB_ICONWARNING);
		}
	}
	else
	{
		if (m_pActiveLoopInstance) {
            if (m_pActiveLoopInstance->GetCacheData()->GetLoopTool()->GetIsToolDiabled()) { // 这个轮廓实例是“不切割”的。
                MessageBox(m_pViewPort->GetWnd(), MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_LoopNoCutting), strProductName, MB_OK | MB_ICONWARNING);
            }
            else {
                // check whether the active loop is sequenced.
                BOOL bLoopSequenced = m_pSequenceData->GetCutSequences()->LoopInstSeqed(m_pActiveLoopInstance.get());
                if (bLoopSequenced) {
                    MessageBox(m_pViewPort->GetWnd(), MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_LOOP_SEQUENCED), strProductName, MB_OK | MB_ICONWARNING);
                }
                else {
                    // 增加工序。
                    CutSequenceListPtr pNewCutSeqs(new CutSequenceList);
                    LoopSequencePtr pLoopSequence(new LoopSequence(m_pActiveLoopInstance));
                    pNewCutSeqs->push_back(pLoopSequence);
                    ICommandPtr pCommand(new LoopSequenceAddCommand(GetEditorData(), m_pSequenceData->GetCutSequences(), m_pConfigData, pNewCutSeqs));
                    pCommand->Do();
                    CommandManager::GetInstance()->AddCommand(pCommand);

                    // 更新“切割节点”缓存。
                    RefreshCutNodes();
                }
            }
		}
		else {
			MessageBox(m_pViewPort->GetWnd(), MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_SELECT_LOOPINST), strProductName, MB_OK | MB_ICONWARNING);
		}
	}

	return TRUE;
}

BOOL LoopSequenceAddAction::MovePoint(double& dPtX, double& dPtY)
{
	Point2D pt(dPtX, dPtY);

	if (m_pConfigData->GetMatSize()->GetType() != MAT_SIZE_Canvas && m_pApplyPartActionBar->IsApplyPart()) {
		m_pActivePartInst = m_pPartLayoutData->GetPartInstances()->GetClosestPartInstance(pt, m_partRectMap);
	}
	else {
		m_pActiveLoopInstance = LoopInstanceManager::GetClosestLoopInstEx(m_pLoopInstanceList, pt);
	}

	return TRUE;
}

BOOL LoopSequenceAddAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	return __super::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL LoopSequenceAddAction::DrawBeforeFinish()
{
	// 绘制当前的“回路实例/零件实例”。
	if (m_pConfigData->GetMatSize()->GetType() != MAT_SIZE_Canvas && m_pApplyPartActionBar->IsApplyPart()) {
		if (m_pActivePartInst) {
			PartPlacementDrawer partPlacementDrawer(m_pViewPort);
			partPlacementDrawer.DrawActivePartInstance(m_pActivePartInst.get());
		}
	}
	else {
		if (m_pActiveLoopInstance) {
            LoopDrawer loopDrawer(m_pViewPort);
			COLORREF iColor = LoopToolManager::GetLoopDisplayColor(m_pActiveLoopInstance->GetCacheData()->GetLoopTool().get());
            loopDrawer.DrawActiveLoop(m_pActiveLoopInstance->GetCacheData()->GetPatternLoop().get(), iColor, m_pActiveLoopInstance->GetTransformMatrix());
		}
	}

	// draw cursor.
	CursorDrawer::DrawAimSelCursor(m_pViewPort);

	return TRUE;
}

void LoopSequenceAddAction::UpdateForUndoRedo()
{
    m_pLoopInstanceList = LoopInstanceManager::GetLoopInstanceList(m_pPartLayoutData->GetPartInstances());
    m_partRectMap = m_pPartLayoutData->GetPartInstances()->GetLoopInstanceRect();

	// 更新“切割节点”缓存。
	RefreshCutNodes();
}

void LoopSequenceAddAction::AssignPartSeq(PartInstancePtr pPartInstance)
{
	/************************************************************************/
	// 给零件实例分配工序。

	const IMaterialSize* pMaterialSize = m_pConfigData->GetMatSize();
	const SequenceConfigItem* pSeqConfigItem = m_pConfigData->GetSeqConfigItem();

	// 当前的切割位置。
	Point2D lastCutPt;
	if (m_pSequenceData->GetCutSequences()->size() == 0)
		lastCutPt = pMaterialSize->GetOuterRect().GetCornerPt(pSeqConfigItem->GetStartCorner());
	else
		lastCutPt = m_pSequenceData->GetCutSequences()->at(m_pSequenceData->GetCutSequences()->size()-1)->GetLastCutPt();

	// 前一个加工的回路实例。
	LoopInstancePtr pPreLoopInst;
	LoopInstanceListPtr pSeqLoopInsts = m_pSequenceData->GetCutSequences()->GetSequencedLoopInstances();
	if (pSeqLoopInsts->size() > 0)
		pPreLoopInst = pSeqLoopInsts->at(pSeqLoopInsts->size()-1);

	// 这个零件实例中未分配工序的“回路实例”。
	LoopInstanceListPtr pAllLoopInst = LoopInstanceManager::GetLoopInstanceList(m_pActivePartInst);
	LoopInstanceListPtr pUnseqLoopInsts = m_pSequenceData->GetCutSequences()->GetUnSequencedLoopInstances(pAllLoopInst);

	// 当前“零件实例”的拓扑层。
	map<LONGLONG, PartInstanceTopologyItemPtr> partTopMap;
	PartInstanceListPtr pPartInstances = m_pPartLayoutData->GetPartInstances();
	PartInstanceTopologyItemListPtr pAllPartTopItems = PartPlacementManager::CalcPartTopStruct(pPartInstances, m_partRectMap, partTopMap);
	PartInstanceTopologyItemPtr pPartTopItem = partTopMap[m_pActivePartInst->GetID()];
	PartInstanceTopologyItemListPtr pPartTopItems = PartPlacementManager::GetPartTopLayer(pAllPartTopItems, m_pActivePartInst->GetID());

	// 得到所有“零件实例”的外围回路的基本图形。
	map<LONGLONG, LineArc2DListPtr> outerLoopGeom = PartPlacementManager::CalcOutLoopGeom(pPartInstances);

	// 分配工序。
	CutSequenceListPtr pLoopSequenceList = LoopSequenceManager::GenerateLoopSequences_4_partInstance(lastCutPt, pPreLoopInst, pPartTopItems.get(),
		pPartTopItem.get(), pUnseqLoopInsts, pSeqLoopInsts, outerLoopGeom, m_pConfigData.get());
	/************************************************************************/


	// 增加工序。
	ICommandPtr pCommand(new LoopSequenceAddCommand(GetEditorData(), m_pSequenceData->GetCutSequences(), m_pConfigData, pLoopSequenceList));
	pCommand->Do();
	CommandManager::GetInstance()->AddCommand(pCommand);

	// 更新“切割节点”缓存。
	RefreshCutNodes();
}

END_CUTLEADER_NAMESPACE()

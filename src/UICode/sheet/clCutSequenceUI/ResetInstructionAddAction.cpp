#include "StdAfx.h"
#include "ResetInstructionAddAction.h"

#include "baseConst.h"
#include "GlViewPort.h"
#include "CursorDrawer.h"
#include "Point2DList.h"
#include "Line2D.h"
#include "LineArc2DList.h"
#include "ValueInputManager.h"
#include "clBaseDataResource.h"

#include "IMaterialSize.h"
#include "SequenceConfigItem.h"
#include "CLOptionInfo.h"
#include "DisplayParam.h"

#include "LoopCutFeatureDrawer.h"
#include "PartLayoutData.h"
#include "ICutPartSequence.h"
#include "CutSequenceList.h"
#include "CutSequenceSelectAction.h"
#include "CutNodeManager.h"
#include "clCutSequenceResource.h"
#include "TraceDataMgr.h"
#include "CutSequenceDrawer.h"
#include "CutNodeList.h"
#include "CutLineNode.h"
#include "CutArcNode.h"
#include "ConfigData.h"
#include "ChainCutSequence.h"
#include "ChainCutSequenceManager.h"
#include "SequenceData.h"

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

ResetInstructionAddAction::ResetInstructionAddAction(PartLayoutDataPtr pPartLayoutData, SequenceDataPtr pSequenceData, ConfigDataPtr pConfigData, GlViewPortPtr pViewPort)
	: SequenceEditActionBase(pPartLayoutData, pSequenceData, pConfigData, pViewPort)
{
    // 所有的连割工序
    m_pAllChainSeqs.reset(new CutSequenceList);
    for (unsigned int i = 0; i < m_pSequenceData->GetCutSequences()->size(); i++)
    {
        ICutSequencePtr pCutSequence = m_pSequenceData->GetCutSequences()->at(i);
        if (pCutSequence->GetCutSequenceType() == CutSequence_ChainCut)
        {
            m_pAllChainSeqs->push_back(pCutSequence);
        }
    }

    // 更新连割工序的切割路径和复位点。
    Update_cutPath_and_resetPt();
}

ResetInstructionAddAction::~ResetInstructionAddAction(void)
{
}

BOOL ResetInstructionAddAction::LButtonDown(double& dPtX, double& dPtY)
{
	Point2D pt(dPtX, dPtY);

    // 最近点所在的工序
    ICutSequencePtr pCutSequence = m_pAllChainSeqs->GetCutSequenceByID(m_pCutNode_hasClosestPt->GetParentID());
    ChainCutSequencePtr pChainSeq = boost::dynamic_pointer_cast<ChainCutSequence>(pCutSequence);

    // 这个点不能和已有的复位点重复
    Point2DListPtr pResetPtList = pChainSeq->GetResetPtList();
    if (pResetPtList->IsExist(m_closestPt_onCutPath))
    {
        ; // 复位点重叠，暂不处理
    }
    else
    {
        pResetPtList->push_back(m_closestPt_onCutPath);
        pChainSeq->SetModified(TRUE);

        // 更新连割工序的切割路径和复位点。
        Update_cutPath_and_resetPt();
    }

	return TRUE;
}

BOOL ResetInstructionAddAction::MovePoint(double& dPtX, double& dPtY)
{
	Point2D pt(dPtX, dPtY);

    // 计算切割路径上距离鼠标最近的点
    double dDistance = INFINITE_VAL;
    for (unsigned int i = 0; i < m_pCutNode_4_chainSeq->size(); i++)
    {
        ICutNodePtr pCutNode = m_pCutNode_4_chainSeq->at(i);

        Point2D closePt;
        if (pCutNode->GetCutNodeType() == CutNode_CutLine)
        {
            CutLineNodePtr pCutLineNode = boost::dynamic_pointer_cast<CutLineNode>(pCutNode);
            closePt = pCutLineNode->GetLine()->GetClosestPt(pt);
        }
        else if (pCutNode->GetCutNodeType() == CutNode_CutArc)
        {
            CutArcNodePtr pCutArcNode = boost::dynamic_pointer_cast<CutArcNode>(pCutNode);
            closePt = pCutArcNode->GetArc()->GetClosestPt(pt);
        }

        double dDis = closePt.DistanceTo(pt);
        if (dDis < dDistance)
        {
            dDistance = dDis;
            m_closestPt_onCutPath = closePt;
            m_pCutNode_hasClosestPt = pCutNode;
        }
    }

	return TRUE;
}

BOOL ResetInstructionAddAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	SequenceEditActionBase::OnKeyDown(nChar, nRepCnt, nFlags);

	if (ValueInputManager::GetInstance()->IsInputCancel(nChar))
	{
		m_pNextAction.reset(new CutSequenceSelectAction(m_pPartLayoutData, m_pSequenceData, m_pConfigData, m_pViewPort));
	}

	return TRUE;
}

BOOL ResetInstructionAddAction::DrawBeforeFinish()
{
	// prepare some data.
	CLOptionInfoPtr pCLOptionInfo = CLOptionInfo::GetInstance();
	DisplayParamPtr pDisplayParam = pCLOptionInfo->GetDisplayParam();
	CutSequenceDrawer cutSequenceDrawer(m_pViewPort);

	// 老的设置。
	COLORREF dwOldColor;
	m_pViewPort->GetDrawColor(dwOldColor);
	int iOldLineWid = m_pViewPort->GetLineWidth();

	// 绘制已有的连割工序的切割路径，此时需要考虑偏移。
	m_pViewPort->SetDrawColor(*pDisplayParam->GetSequenceColor());
	SequenceSimulateShapeListPtr pSeqSimShapeList = CutNodeManager::CutNode2SimShape(m_pCutNode_4_chainSeq.get());
	cutSequenceDrawer.DrawSequenceSimulateShapes(pSeqSimShapeList.get());

    // 画出所有复位点
    LoopCutFeatureDrawer loopFeatureDrawer(m_pViewPort);
    for (unsigned int i = 0; i < m_pResetPtList->size(); i++)
    {
        loopFeatureDrawer.DrawFeatureLoc(m_pResetPtList->at(i), RGB(255, 0, 0));
    }

    // 绘制切割路径上到光标最近的点
    loopFeatureDrawer.DrawPosRing(m_closestPt_onCutPath);

    // draw cursor.
    m_pViewPort->SetDrawColor(*pDisplayParam->GetPatColor());
    CursorDrawer::DrawCrossLineCursor(m_pViewPort);

	// 恢复设置。
	m_pViewPort->SetDrawColor(dwOldColor);
	m_pViewPort->SetLineWidth(iOldLineWid);

	return TRUE;
}

void ResetInstructionAddAction::Update_cutPath_and_resetPt()
{
    // 生成切割路径，此时可能考虑了偏移。
    const SequenceConfigItem* pSeqConfigItem = m_pConfigData->GetSeqConfigItem();
    const LeadConfigItem* pLeadConfigItem = m_pConfigData->GetLeadConfigItem();
    const IMaterialSize* pMaterialSize = m_pConfigData->GetMatSize();
    Point2D startSeqPt = pMaterialSize->GetOuterRect().GetCornerPt(pSeqConfigItem->GetStartCorner());
    PartPlacementListPtr pPartPlacements = m_pPartLayoutData->GetPartPlacements();
    PartInstanceListPtr pPartInstances = m_pPartLayoutData->GetPartInstances();
    std::set<LONGLONG> patternLoops_failedToOffset;
    CutNodeListPtr pCutNodeList_4_simulate = CutNodeManager::CutSequences2CutNodes_4_simulate(startSeqPt, pPartPlacements.get(), pPartInstances.get(),
        m_pSequenceData->GetCutSequences().get(), pLeadConfigItem, pSeqConfigItem, patternLoops_failedToOffset);

    // 删除偏移了切割路径的复位点。
    ChainCutSequenceManager::DeleteResetPts_whichDeviateFromCutPath(m_pAllChainSeqs, pCutNodeList_4_simulate);

    // 这些连割工序对应的切割路径
    m_pCutNode_4_chainSeq.reset(new CutNodeList);
    for (unsigned int i = 0; i < pCutNodeList_4_simulate->size(); i++)
    {
        ICutNodePtr pCutNode = pCutNodeList_4_simulate->at(i);
        if (m_pAllChainSeqs->GetCutSequenceByID(pCutNode->GetParentID()))
        {
            m_pCutNode_4_chainSeq->push_back(pCutNode);
        }
    }

    // 这些连割工序对应的复位点
    m_pResetPtList.reset(new Point2DList);
    for (unsigned int i = 0; i < m_pAllChainSeqs->size(); i++)
    {
        ChainCutSequencePtr pChainSeq = boost::dynamic_pointer_cast<ChainCutSequence>(m_pAllChainSeqs->at(i));
        Point2DListPtr pResetPtList = pChainSeq->GetResetPtList();
        m_pResetPtList->insert(m_pResetPtList->end(), pResetPtList->begin(), pResetPtList->end());
    }
}

END_CUTLEADER_NAMESPACE()

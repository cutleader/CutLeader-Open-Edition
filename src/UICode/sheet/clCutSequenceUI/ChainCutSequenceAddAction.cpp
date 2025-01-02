#include "StdAfx.h"
#include "ChainCutSequenceAddAction.h"

#include "baseConst.h"
#include "GlViewPort.h"
#include "CommandManager.h"
#include "CursorDrawer.h"
#include "clBaseDataResource.h"
#include "ValueInputManager.h"
#include "Point2DList.h"

#include "SequenceConfigItem.h"
#include "IMaterialSize.h"
#include "CLOptionInfo.h"
#include "DisplayParam.h"
#include "SheetEditParam.h"

#include "IArrayPattern.h"
#include "IPatternLoop.h"
#include "PatternLoopNode.h"
#include "PatternLoopNodeList.h"
#include "LoopDrawer.h"
#include "LoopToolData.h"
#include "LoopCutFeatureDrawer.h"
#include "PartPlacementList.h"
#include "PartInstance.h"
#include "PartInstanceList.h"
#include "LoopInstanceList.h"
#include "LoopInstanceCache.h"
#include "LoopInstance.h"
#include "PartPlacementManager.h"
#include "LoopInstanceManager.h"
#include "clCutSequenceResource.h"
#include "CutNodeList.h"
#include "PtChainNode.h"
#include "LoopChainNode.h"
#include "ChainNodeList.h"
#include "ChainCutSequence.h"
#include "CutSequenceList.h"
#include "ChainCutSequenceAddCommand.h"
#include "PtChainNodeAddCommand.h"
#include "LoopChainNodeAddCommand.h"
#include "CutSequenceDrawer.h"
#include "PartLayoutData.h"
#include "CutNodeManager.h"
#include "CutSequenceSelectAction.h"
#include "ChainCutSequenceManager.h"
#include "ConfigData.h"
#include "PatternList.h"
#include "SequenceData.h"

BEGIN_CUTLEADER_NAMESPACE()

ChainCutSequenceAddAction::ChainCutSequenceAddAction(PartLayoutDataPtr pPartLayoutData, SequenceDataPtr pSequenceData, 
	ConfigDataPtr pConfigData, GlViewPortPtr pViewPort)
	: SequenceEditActionBase(pPartLayoutData, pSequenceData, pConfigData, pViewPort)
{
	m_iState = NONE;
	m_iMsgID = IDS_SEQ_SEL_PT_LOOP_4_CHAIN;

	m_pCutPts.reset(new Point2DList);
	m_pChainSeq.reset();

	// 得到所有外围“回路实例”。
	m_pOuterLoopInsts = LoopInstanceManager::GetOuterLoopInsts(m_pPartLayoutData->GetPartInstances());

	m_pPatternsInActiveLoopInstance.reset(new PatternList);
}

ChainCutSequenceAddAction::~ChainCutSequenceAddAction(void)
{
}

BOOL ChainCutSequenceAddAction::LButtonDown(double& dPtX, double& dPtY)
{
	CString strProductName, strMsg;
	strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);

	if (m_iState == NONE)
	{
		if (m_pActiveLoopInstance) // 当前选择了轮廓。
		{
            if (m_pActiveLoopInstance->GetCacheData()->GetLoopTool()->GetIsToolDiabled()) // 这个轮廓实例是“不切割”的。
            {
                strMsg = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_LoopNoCutting);
                MessageBox(m_pViewPort->GetWnd(), strMsg, strProductName, MB_OK | MB_ICONWARNING);
                return TRUE;
            }

			// 检查能否在这个“回路实例”上加连割。
			if (Check4CreateChain(m_pActiveLoopInstance))
			{
				// 创建“连割工序”。
				CreateChainSeq();

				m_iState = CHAIN_SEQ_CREATED;
			}
		}
		else
		{
			m_pCutPts->push_back(Point2D(dPtX, dPtY));
		}
	}
	else if (m_iState == CHAIN_SEQ_CREATED)
	{
		// 在现有“连割工序”后面加节点。
		AddChainNode();
	}

	return TRUE;
}

BOOL ChainCutSequenceAddAction::MovePoint(double& dPtX, double& dPtY)
{
	Point2D pt(dPtX, dPtY);

	// 更新当前光标点。
	m_currentPt = pt;

	// 得到捕获轮廓的距离。
	CLOptionInfoPtr pCLOptionInfo = CLOptionInfo::GetInstance();
	SheetEditParamPtr pSheetEditParam = pCLOptionInfo->GetSheetEditParam();
	double dCapLoopDis = pSheetEditParam->GetLoopDetectDis();
	m_pViewPort->DToW(dCapLoopDis);

	// 检查鼠标当前是否“套住”了轮廓。
	LoopInstancePtr pLoopInstance = LoopInstanceManager::GetClosestLoopInst(m_pOuterLoopInsts, pt);
	if (pLoopInstance && pLoopInstance->DistanceTo(pt) < dCapLoopDis)
	{
		m_pActiveLoopInstance = pLoopInstance;
		m_pPatternsInActiveLoopInstance = m_pActiveLoopInstance->GetCacheData()->GetPatternLoop()->GetPatternList();

		// 是否套住了轮廓的起切点。
		Point2D startCutPt = m_pActiveLoopInstance->GetStartCutPt();
		if (startCutPt.DistanceTo(pt) < dCapLoopDis)
			m_bOldStartPt = TRUE;
		else
		{
			m_bOldStartPt = FALSE;

			// 新的起切点。
			Point2D pt(dPtX, dPtY);
			IPatternLoopPtr pPatternLoop = m_pActiveLoopInstance->GetCacheData()->GetPatternLoop();
			Matrix2D mat = m_pActiveLoopInstance->GetTransformMatrix();
			if (pPatternLoop->GetPatternLoopType() == PatternLoop_Array)
			{
				PatternLoopNodePtr pPatternLoopNode = pPatternLoop->GetPatternLoopNodes()->at(0);
				IArrayPatternPtr pArrayPattern = boost::dynamic_pointer_cast<IArrayPattern>(pPatternLoopNode->GetPattern());
				IPatternPtr pFirstPat = pArrayPattern->GetFirstInstance();
				m_newPt = pFirstPat->GetClosestPt(pt, mat);
				m_iNodeIndex = 0;
			}
			else
				pPatternLoop->GetClosestPt(pt, m_newPt, m_iNodeIndex, mat);
		}
	}
	else
	{
		m_pActiveLoopInstance.reset();
		m_pPatternsInActiveLoopInstance->clear();
	}

	return TRUE;
}

BOOL ChainCutSequenceAddAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	SequenceEditActionBase::OnKeyDown(nChar, nRepCnt, nFlags);

	if (ValueInputManager::GetInstance()->IsInputCancel(nChar))
	{
		m_pNextAction.reset(new CutSequenceSelectAction(m_pPartLayoutData, m_pSequenceData, m_pConfigData, m_pViewPort));
	}

	return TRUE;
}

BOOL ChainCutSequenceAddAction::DrawBeforeFinish()
{
	const DisplayParam* pDisplayParam = CLOptionInfo::GetInstance()->GetDisplayParam().get();
	LoopCutFeatureDrawer loopFeatureDrawer(m_pViewPort);

	// 老的设置。
	COLORREF dwOldColor;
	m_pViewPort->GetDrawColor(dwOldColor);
	int iOldLineWid = m_pViewPort->GetLineWidth();

	if (m_iState == NONE)
	{
		// 绘制已选中点的连线。
		if (m_pCutPts->size() > 1)
		{
			for (unsigned int i = 0; i < m_pCutPts->size()-1; i++)
			{
				Point2D startPt = m_pCutPts->at(i);
				Point2D endPt = m_pCutPts->at(i+1);
				m_pViewPort->DrawLinePoint2D(startPt.X(), startPt.Y(), endPt.X(), endPt.Y());
			}
		}

		// 绘制橡皮筋和起切点。
		if (m_pActiveLoopInstance)
		{
			if (m_bOldStartPt)
			{
				// 橡皮筋。
				if (m_pCutPts->size() > 0)
				{
					Point2D piercePt = LoopInstanceManager::GetPiercePt(m_pActiveLoopInstance);
					m_pViewPort->DrawLinePoint2D(m_pCutPts->back().X(), m_pCutPts->back().Y(), piercePt.X(), piercePt.Y());
				}

				// 绘制起切点。
				loopFeatureDrawer.DrawFeatureLoc(m_pActiveLoopInstance->GetStartCutPt(), RGB(255,0,0));
			}
			else
			{
				// 橡皮筋。
				if (m_pCutPts->size() > 0)
					m_pViewPort->DrawLinePoint2D(m_pCutPts->back().X(), m_pCutPts->back().Y(), m_newPt.X(), m_newPt.Y());

				// 绘制环。
				loopFeatureDrawer.DrawPosRing(m_newPt);
			}
		}
		else
		{
			if (m_pCutPts->size() > 0)
				m_pViewPort->DrawLinePoint2D(m_pCutPts->back().X(), m_pCutPts->back().Y(), m_currentPt.X(), m_currentPt.Y());
		}
	}
	else if (m_iState == CHAIN_SEQ_CREATED)
	{
		// 绘制“连割工序”的结束点的橡皮筋。
		Point2D lastCutPt = m_pChainSeq->GetLastCutPt();
		if (m_pActiveLoopInstance)
		{
			if (m_bOldStartPt)
			{
				// 橡皮筋。
				if (m_pCutPts->size() > 0)
				{
					Point2D piercePt = LoopInstanceManager::GetPiercePt(m_pActiveLoopInstance);
					m_pViewPort->DrawLinePoint2D(lastCutPt.X(), lastCutPt.Y(), piercePt.X(), piercePt.Y());
				}

				// 绘制起切点。
				loopFeatureDrawer.DrawFeatureLoc(m_pActiveLoopInstance->GetStartCutPt(), RGB(255,0,0));
			}
			else
			{
				// 橡皮筋。
				if (m_pCutPts->size() > 0)
					m_pViewPort->DrawLinePoint2D(lastCutPt.X(), lastCutPt.Y(), m_newPt.X(), m_newPt.Y());

				// 绘制环。
				loopFeatureDrawer.DrawPosRing(m_newPt);
			}
		}
		else
		{
			if (m_pCutPts->size() > 0)
				m_pViewPort->DrawLinePoint2D(lastCutPt.X(), lastCutPt.Y(), m_currentPt.X(), m_currentPt.Y());
		}
	}

	// 恢复设置。
	m_pViewPort->SetDrawColor(dwOldColor);
	m_pViewPort->SetLineWidth(iOldLineWid);

	// draw cursor.
	CursorDrawer::DrawCrossLineCursor(m_pViewPort);

	return TRUE;
}

void ChainCutSequenceAddAction::UpdateForUndoRedo()
{
	// 更新“切割节点”缓存。
	RefreshCutNodes();

	if (m_pChainSeq)
	{
		// 这个动作创建的工序如果没了，回到创建工序的状态。
		if (m_pSequenceData->GetCutSequences()->GetCutSequenceIndex(m_pChainSeq.get()) == INVALID_IDX)
			m_iState = NONE;
		else
			m_iState = CHAIN_SEQ_CREATED;
	}
}

SnapInputPtr ChainCutSequenceAddAction::GetSnapInput() const
{
	SnapInputPtr pSnapInput = SnapInput::BuildSnapInput4SnapNone();

	if (m_pActiveLoopInstance)
	{
		Matrix2D transformMatrix = m_pActiveLoopInstance->GetTransformMatrix();
		pSnapInput.reset(new SnapInput(SnapEndPtOf2DLineArc | SnapMiddlePtOf2DLineArc, m_pPatternsInActiveLoopInstance, transformMatrix));
	}

	return pSnapInput;
}

BOOL ChainCutSequenceAddAction::Check4CreateChain(LoopInstancePtr pLoopInstance)
{
	CString strProductName, strMsg;
	strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);

	ICutPartSequencePtr pCutSeqUnit = m_pSequenceData->GetCutSequences()->GetCutSequenceByLoopInstance(pLoopInstance.get());
	if (pCutSeqUnit)
	{
		if (pCutSeqUnit->GetCutSequenceType() == CutSequence_CommonCut || pCutSeqUnit->GetCutSequenceType() == CutSequence_Bridge || pCutSeqUnit->GetCutSequenceType() == CutSequence_ChainCut)
		{
			if (pCutSeqUnit->GetCutSequenceType() == CutSequence_CommonCut)
				strMsg = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_LOOP_NOT_CHAIN_1);
			else if (pCutSeqUnit->GetCutSequenceType() == CutSequence_Bridge)
				strMsg = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_LOOP_NOT_CHAIN_2);
			else if (pCutSeqUnit->GetCutSequenceType() == CutSequence_ChainCut)
				strMsg = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_LOOP_NOT_CHAIN_3);
			MessageBox(m_pViewPort->GetWnd(), strMsg, strProductName, MB_OK|MB_ICONWARNING);
			return FALSE;
		}
	}

	return TRUE;
}

void ChainCutSequenceAddAction::CreateChainSeq()
{
	// 增加“连割工序”。
	m_pChainSeq = ChainCutSequenceManager::BuildChainSeq(m_pSequenceData->GetCutSequences(), m_pCutPts, m_pActiveLoopInstance);
	Point2D newStartPt = m_newPt.InverseTransform(m_pActiveLoopInstance->GetTransformMatrix());
	LoopPtPtr pLoopPt;
	if (!m_bOldStartPt)
		pLoopPt.reset(new LoopPt(newStartPt, m_iNodeIndex));
	ICommandPtr pCommand(new ChainCutSequenceAddCommand(GetEditorData(), m_pSequenceData->GetCutSequences(), m_pConfigData, m_pPartLayoutData->GetPartPlacements(), m_pPartLayoutData->GetPartInstances(), m_pOuterLoopInsts, m_pChainSeq, pLoopPt));
	pCommand->Do();
	CommandManager::GetInstance()->AddCommand(pCommand);

	// 更新“切割节点”缓存。
	RefreshCutNodes();
}

void ChainCutSequenceAddAction::AddChainNode()
{
    CString strProductName, strMsg;
    strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);

	// 如果选中轮廓，需检查能不能作为连割节点。
	if (m_pActiveLoopInstance)
	{
        if (m_pActiveLoopInstance->GetCacheData()->GetLoopTool()->GetIsToolDiabled()) // 这个轮廓实例是“不切割”的。
        {
            strMsg = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_LoopNoCutting);
            MessageBox(m_pViewPort->GetWnd(), strMsg, strProductName, MB_OK | MB_ICONWARNING);
            return;
        }

		ICutPartSequencePtr pCutSeqUnit = m_pSequenceData->GetCutSequences()->GetCutSequenceByLoopInstance(m_pActiveLoopInstance.get());
		if (pCutSeqUnit)
		{
			if (pCutSeqUnit->GetCutSequenceType() == CutSequence_CommonCut || pCutSeqUnit->GetCutSequenceType() == CutSequence_Bridge || pCutSeqUnit->GetCutSequenceType() == CutSequence_ChainCut)
			{
				if (pCutSeqUnit->GetCutSequenceType() == CutSequence_CommonCut)
					strMsg = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_LOOP_NOT_CHAIN_1);
				else if (pCutSeqUnit->GetCutSequenceType() == CutSequence_Bridge)
					strMsg = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_LOOP_NOT_CHAIN_2);
				else if (pCutSeqUnit->GetCutSequenceType() == CutSequence_ChainCut)
					strMsg = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_LOOP_NOT_CHAIN_3);
				MessageBox(m_pViewPort->GetWnd(), strMsg, strProductName, MB_OK|MB_ICONWARNING);
				return;
			}
		}
	}

    // 检查轮廓的刀具尺寸
	if (m_pActiveLoopInstance)
    {
        LoopInstancePtr pFstLoopInst_of_chainSeq = m_pChainSeq->GetFstLoopInst();
        if (!m_pActiveLoopInstance->GetCacheData()->GetLoopTool()->IsSameToolSize(pFstLoopInst_of_chainSeq->GetCacheData()->GetLoopTool().get()))
        {
            strMsg = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_CHAIN_LOOPTOOL_WARN);
            MessageBox(m_pViewPort->GetWnd(), strMsg, strProductName, MB_OK | MB_ICONWARNING);
            return;
        }
    }

	// 创建“连割节点”。
	ICommandPtr pCommand;
	IChainNodePtr pLastNode = m_pChainSeq->GetChainNodes()->back();
	if (m_pActiveLoopInstance)
	{
		LoopChainNodePtr pChainLoopNode(new LoopChainNode(m_pActiveLoopInstance));
		Point2D newStartPt = m_newPt.InverseTransform(m_pActiveLoopInstance->GetTransformMatrix());
		LoopPtPtr pLoopPt;
		if (!m_bOldStartPt)
			pLoopPt.reset(new LoopPt(newStartPt, m_iNodeIndex));
		pCommand.reset(new LoopChainNodeAddCommand(GetEditorData(), m_pSequenceData->GetCutSequences(), m_pConfigData, m_pPartLayoutData->GetPartPlacements(), m_pPartLayoutData->GetPartInstances(), m_pOuterLoopInsts, m_pChainSeq, pChainLoopNode, INVALID_ID, pLoopPt));
	}
	else
	{
		PtChainNodePtr pChainPtNode(new PtChainNode(m_currentPt));
		pCommand.reset(new PtChainNodeAddCommand(GetEditorData(), m_pSequenceData->GetCutSequences(), m_pConfigData, m_pChainSeq, pChainPtNode, INVALID_ID));
	}
	pCommand->Do();
	CommandManager::GetInstance()->AddCommand(pCommand);

	// 更新“切割节点”缓存。
	RefreshCutNodes();
}

END_CUTLEADER_NAMESPACE()

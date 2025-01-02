#include "StdAfx.h"
#include "ChainNodeInsertAction.h"

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

#include "LoopPt.h"
#include "IArrayPattern.h"
#include "IPatternLoop.h"
#include "PatternLoopNode.h"
#include "PatternLoopNodeList.h"
#include "LoopToolData.h"
#include "LoopDrawer.h"
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
#include "PtChainNodeAddCommand.h"
#include "LoopChainNodeAddCommand.h"
#include "CutSequenceDrawer.h"
#include "PartLayoutData.h"
#include "CutNodeManager.h"
#include "LoopSequenceManager.h"
#include "ChainCutSequenceManager.h"
#include "CutSequenceSelectAction.h"
#include "ConfigData.h"
#include "PatternList.h"
#include "SequenceData.h"

BEGIN_CUTLEADER_NAMESPACE()

ChainNodeInsertAction::ChainNodeInsertAction(PartLayoutDataPtr pPartLayoutData, SequenceDataPtr pSequenceData,
	ConfigDataPtr pConfigData, GlViewPortPtr pViewPort, InsertPositionActionBar* pInsertPositionActionBar)
	: SequenceEditActionBase(pPartLayoutData, pSequenceData, pConfigData, pViewPort)
{
	m_pInsertPositionActionBar = pInsertPositionActionBar;

	m_iState = NONE;
	m_iMsgID = IDS_SEQ_CHAIN_SEL_BASE_NODE;
	m_pBaseChainNode.reset();

	// 得到所有外围“回路实例”。
	m_pOuterLoopInsts = LoopInstanceManager::GetOuterLoopInsts(m_pPartLayoutData->GetPartInstances());

	m_pPatternsInActiveLoopInstance.reset(new PatternList);

	m_pInsertPositionActionBar->Show_InsertPositionActionBar();
}

ChainNodeInsertAction::~ChainNodeInsertAction(void)
{
}

BOOL ChainNodeInsertAction::LButtonDown(double& dPtX, double& dPtY)
{
	Point2D pt(dPtX, dPtY);

	if (m_iState == NONE)
	{
		if (m_pBaseChainNode)
		{
			m_iState = CHAIN_NODE_SELECTED;
		}
		else
		{
			CString strProductName, strMsg;
			strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);
			strMsg = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_CHAIN_NO_BASE_NODE_SEL);
			MessageBox(m_pViewPort->GetWnd(), strMsg, strProductName, MB_OK|MB_ICONWARNING);
		}
	}
	else if (m_iState == CHAIN_NODE_SELECTED)
	{
		InsertChainNode();
	}

	return TRUE;
}

BOOL ChainNodeInsertAction::MovePoint(double& dPtX, double& dPtY)
{
	Point2D pt(dPtX, dPtY);

	// 更新当前光标点。
	m_currentPt = pt;

	if (m_iState == NONE)
	{
		m_iMsgID = IDS_SEQ_CHAIN_SEL_BASE_NODE;

		// 得到最近的连割节点。
		ChainNodeListPtr pChainNodes = ChainCutSequenceManager::GetAllChainNode(m_pSequenceData->GetCutSequences());
		m_pBaseChainNode = pChainNodes->GetClosestNode(pt);
	}
	else if (m_iState == CHAIN_NODE_SELECTED)
	{
		m_iMsgID = IDS_SEQ_SEL_PT_LOOP_4_CHAIN;

		// 得到捕获轮廓的距离。
		CLOptionInfoPtr pCLOptionInfo = CLOptionInfo::GetInstance();
		SheetEditParamPtr pSheetEditParam = pCLOptionInfo->GetSheetEditParam();
		double dCapLoopDis = pSheetEditParam->GetLoopDetectDis();
		m_pViewPort->DToW(dCapLoopDis);

		// 检查鼠标当前是否“套住”了轮廓。
		LoopInstancePtr pLoopInstance = LoopInstanceManager::GetClosestLoopInst(m_pOuterLoopInsts, pt);
		if (pLoopInstance->DistanceTo(pt) < dCapLoopDis)
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
			
	}

	return TRUE;
}

BOOL ChainNodeInsertAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	SequenceEditActionBase::OnKeyDown(nChar, nRepCnt, nFlags);

	if (ValueInputManager::GetInstance()->IsInputCancel(nChar))
	{
		m_pNextAction.reset(new CutSequenceSelectAction(m_pPartLayoutData, m_pSequenceData, m_pConfigData, m_pViewPort));
	}

	return TRUE;
}

BOOL ChainNodeInsertAction::DrawBeforeFinish()
{
    const DisplayParam* pDisplayParam = CLOptionInfo::GetInstance()->GetDisplayParam().get();
    LoopCutFeatureDrawer loopFeatureDrawer(m_pViewPort);
    CutSequenceDrawer cutSeqDrawer(m_pViewPort);

	if (m_iState == NONE)
	{
		// 绘制当前的“连割节点”。
		if (m_pBaseChainNode)
		{
			cutSeqDrawer.DrawActiveChainNode(m_pBaseChainNode.get());
		}

		// draw cursor.
		CursorDrawer::DrawAimSelCursor(m_pViewPort);
	}
	else if (m_iState == CHAIN_NODE_SELECTED)
	{
		// 绘制基准节点。
		cutSeqDrawer.DrawSelectedChainNode(m_pBaseChainNode.get());

		// 当靠近轮廓时，绘制起切点。
		if (m_pActiveLoopInstance)
		{
			if (m_bOldStartPt)
				loopFeatureDrawer.DrawFeatureLoc(m_pActiveLoopInstance->GetStartCutPt(), RGB(255,0,0));
			else
				loopFeatureDrawer.DrawPosRing(m_newPt);
		}

		// draw cursor.
		CursorDrawer::DrawCrossLineCursor(m_pViewPort);
	}

	return TRUE;
}

void ChainNodeInsertAction::UpdateForUndoRedo()
{
	// 更新“切割节点”缓存。
	RefreshCutNodes();

	// 重置状态。
	m_iState = NONE;
	m_pBaseChainNode.reset();
}

SnapInputPtr ChainNodeInsertAction::GetSnapInput() const
{
	SnapInputPtr pSnapInput = SnapInput::BuildSnapInput4SnapNone();

	if (m_pActiveLoopInstance)
	{
		Matrix2D transformMatrix = m_pActiveLoopInstance->GetTransformMatrix();
		pSnapInput.reset(new SnapInput(SnapEndPtOf2DLineArc | SnapMiddlePtOf2DLineArc, m_pPatternsInActiveLoopInstance, transformMatrix));
	}

	return pSnapInput;
}

void ChainNodeInsertAction::InsertChainNode()
{
    CString strProductName, strMsg;
    strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);

    if (m_pActiveLoopInstance && m_pActiveLoopInstance->GetCacheData()->GetLoopTool()->GetIsToolDiabled()) // 这个轮廓实例是“不切割”的。
    {
        strMsg = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_LoopNoCutting);
        MessageBox(m_pViewPort->GetWnd(), strMsg, strProductName, MB_OK | MB_ICONWARNING);
        return;
    }

	// 如果选中轮廓，需检查能不能作为连割节点。
	if (m_pActiveLoopInstance)
	{
		ICutPartSequencePtr pCutSeqUnit = m_pSequenceData->GetCutSequences()->GetCutSequenceByLoopInstance(m_pActiveLoopInstance.get());
		if (pCutSeqUnit)
		{
			if (pCutSeqUnit->GetCutSequenceType() == CutSequence_CommonCut || pCutSeqUnit->GetCutSequenceType() == CutSequence_Bridge ||
				pCutSeqUnit->GetCutSequenceType() == CutSequence_ChainCut)
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
        ChainCutSequencePtr pChainSeq = ChainCutSequenceManager::SearchChainSeq(m_pSequenceData->GetCutSequences(), m_pBaseChainNode);
        LoopInstancePtr pFstLoopInst_of_chainSeq = pChainSeq->GetFstLoopInst();
        if (!m_pActiveLoopInstance->GetCacheData()->GetLoopTool()->IsSameToolSize(pFstLoopInst_of_chainSeq->GetCacheData()->GetLoopTool().get()))
        {
            strMsg = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_CHAIN_LOOPTOOL_WARN);
            MessageBox(m_pViewPort->GetWnd(), strMsg, strProductName, MB_OK | MB_ICONWARNING);
            return;
        }
    }


	/************************************************************************/
	// 插入连割节点。

	// 当前的连割工序
	ChainCutSequencePtr pChainSeq = ChainCutSequenceManager::SearchChainSeq(m_pSequenceData->GetCutSequences(), m_pBaseChainNode);

	// 确定插在哪个节点前面。
	LONGLONG iNextNodeID;
	if (!m_pInsertPositionActionBar->IsInsertBehind())
		iNextNodeID = m_pBaseChainNode->GetID();
	else
		iNextNodeID = pChainSeq->GetChainNodes()->GetNextNodeID(m_pBaseChainNode);

	// 创建“连割节点”。
	ICommandPtr pCommand;
	if (m_pActiveLoopInstance)
	{
		LoopChainNodePtr pChainLoopNode(new LoopChainNode(m_pActiveLoopInstance));
		Point2D newStartPt = m_newPt.InverseTransform(m_pActiveLoopInstance->GetTransformMatrix());
		LoopPtPtr pLoopPt;
		if (!m_bOldStartPt)
			pLoopPt.reset(new LoopPt(newStartPt, m_iNodeIndex));
		pCommand.reset(new LoopChainNodeAddCommand(GetEditorData(), m_pSequenceData->GetCutSequences(), m_pConfigData,
			m_pPartLayoutData->GetPartPlacements(), m_pPartLayoutData->GetPartInstances(), m_pOuterLoopInsts, pChainSeq, pChainLoopNode, iNextNodeID, pLoopPt));

		// 更新基准节点。
		m_pBaseChainNode = pChainLoopNode;
	}
	else
	{
		PtChainNodePtr pChainPtNode(new PtChainNode(m_currentPt));
		pCommand.reset(new PtChainNodeAddCommand(GetEditorData(), m_pSequenceData->GetCutSequences(), m_pConfigData, pChainSeq, pChainPtNode, iNextNodeID));

		// 更新基准节点。
		m_pBaseChainNode = pChainPtNode;
	}
	pCommand->Do();
	CommandManager::GetInstance()->AddCommand(pCommand);
	/************************************************************************/


	// 更新“切割节点”缓存。
	RefreshCutNodes();
}

END_CUTLEADER_NAMESPACE()

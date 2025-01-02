#include "StdAfx.h"
#include "BridgeSequenceAddAction.h"

#include "baseConst.h"
#include "GlViewPort.h"
#include "CommandManager.h"
#include "CursorDrawer.h"
#include "clBaseDataResource.h"
#include "ValueInputManager.h"

#include "BridgeCutParam.h"
#include "SequenceConfigItem.h"
#include "IMaterialSize.h"
#include "CLOptionInfo.h"
#include "DisplayParam.h"

#include "IPatternLoop.h"
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
#include "CutSequenceList.h"
#include "CutSequenceDrawer.h"
#include "BridgeNodeAddCommand.h"
#include "BridgeSequenceAddCommand.h"
#include "BridgeNodeInsertCommand.h"
#include "BridgeSequenceLinkCommand.h"
#include "PartLayoutData.h"
#include "CutNodeManager.h"
#include "BridgeSequence.h"
#include "BridgeSequenceManager.h"
#include "LoopSequenceManager.h"
#include "CutSequenceSelectAction.h"
#include "ConfigData.h"
#include "PatternList.h"
#include "SequenceData.h"

BEGIN_CUTLEADER_NAMESPACE()

BridgeSequenceAddAction::BridgeSequenceAddAction(PartLayoutDataPtr pPartLayoutData, SequenceDataPtr pSequenceData, ConfigDataPtr pConfigData, GlViewPortPtr pViewPort)
	: SequenceEditActionBase(pPartLayoutData, pSequenceData, pConfigData, pViewPort)
{
	m_iState = NONE;
	m_iMsgID = IDS_SEQ_SEL_BDGE_PT;

	// 得到所有外围“回路实例”。
	m_pOuterLoopInsts = LoopInstanceManager::GetOuterLoopInsts(m_pPartLayoutData->GetPartInstances());

	// 计算零件实例中各个回路实例的外接矩形。
	m_partRectMap = m_pPartLayoutData->GetPartInstances()->GetLoopInstanceRect();

	m_iFstLoopNodeIdx = INVALID_IDX;
	m_iSndLoopNodeIdx = INVALID_IDX;

	m_pPatternsInFirstLoopInstance.reset(new PatternList);
	m_pPatternsInSecondLoopInstance.reset(new PatternList);
}

BridgeSequenceAddAction::~BridgeSequenceAddAction(void)
{
}

BOOL BridgeSequenceAddAction::LButtonDown(double& dPtX, double& dPtY)
{
	CString strProductName, strMsg;
	strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);

	if (m_iState == NONE || m_iState == SECOND_BDGE_PT_SELECTED)
	{
		if (!m_pFstLoopInst)
			return TRUE;

        if (m_pFstLoopInst->GetCacheData()->GetLoopTool()->GetIsToolDiabled()) // 这个轮廓实例是“不切割”的。
        {
            strMsg = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_LoopNoCutting);
            MessageBox(m_pViewPort->GetWnd(), strMsg, strProductName, MB_OK | MB_ICONWARNING);
            return TRUE;
        }

		// 检查能否在这个“回路实例”上加桥接。
		if (LoopCanAddBdge(m_pFstLoopInst))
		{
			m_iState = FIRST_BDGE_PT_SELECTED;
			m_pSndLoopInst.reset();
			m_pPatternsInSecondLoopInstance->clear();
		}
	}
	else if (m_iState == FIRST_BDGE_PT_SELECTED)
	{
		if (!m_pSndLoopInst)
			return TRUE;

        if (m_pSndLoopInst->GetCacheData()->GetLoopTool()->GetIsToolDiabled()) // 这个轮廓实例是“不切割”的。
        {
            strMsg = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_LoopNoCutting);
            MessageBox(m_pViewPort->GetWnd(), strMsg, strProductName, MB_OK | MB_ICONWARNING);
            return TRUE;
        }

		// 不能是同一个轮廓。
		if ((m_pFstLoopInst->GetPartInstance()->GetID() == m_pSndLoopInst->GetPartInstance()->GetID()) && (m_pFstLoopInst->GetPatternLoopID() == m_pSndLoopInst->GetPatternLoopID()))
		{
			strMsg = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_BDGE_SAME_LOOP);
			MessageBox(m_pViewPort->GetWnd(), strMsg, strProductName, MB_OK | MB_ICONWARNING);
			return TRUE;
		}

        // 检查这两个轮廓的刀具尺寸
        if (!m_pFstLoopInst->GetCacheData()->GetLoopTool()->IsSameToolSize(m_pSndLoopInst->GetCacheData()->GetLoopTool().get()))
        {
            strMsg = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_BDGE_LOOPTOOL_WARN);
            MessageBox(m_pViewPort->GetWnd(), strMsg, strProductName, MB_OK | MB_ICONWARNING);
            return TRUE;
        }

		// 检查能否在这个“回路实例”上加桥接。
		if (!LoopCanAddBdge(m_pSndLoopInst))
			return TRUE;

		// 检查这2个回路间能不能桥接。
		if (!BridgeSequenceManager::TwoLoopCanBdge(m_pSequenceData->GetCutSequences(), m_pFstLoopInst.get(), m_pSndLoopInst.get()))
		{
			strMsg = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_LOOP_NOTALLOW_BDGE);
			MessageBox(m_pViewPort->GetWnd(), strMsg, strProductName, MB_OK | MB_ICONWARNING);
		}
		else
		{
			// 检查这2个点之间能不能桥接。
			BridgeCutParamPtr pBdgeCutParam = m_pConfigData->GetSeqConfigItem()->GetBdgeCutParam();
			double dWidth = pBdgeCutParam->GetBdgeWidth();
			if (!BridgeSequenceManager::TwoLoopCanBdge(m_pFstLoopInst, m_iFstLoopNodeIdx, m_fstBdgePt, m_pSndLoopInst, m_iSndLoopNodeIdx, m_sndBdgePt, dWidth))
			{
				strMsg = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_PT_NOTALLOW_BDGE);
				MessageBox(m_pViewPort->GetWnd(), strMsg, strProductName, MB_OK | MB_ICONWARNING);
			}
			else
			{
				// 在选中的2个“回路实例”间增加桥接。
				AddBdgeCut();

				// 重设数据。
				m_iState = NONE;
				m_pFstLoopInst.reset();
				m_pPatternsInFirstLoopInstance->clear();
			}
		}
	}

	return TRUE;
}

BOOL BridgeSequenceAddAction::MovePoint(double& dPtX, double& dPtY)
{
	Point2D pt(dPtX, dPtY);

	if (m_iState == NONE || m_iState == SECOND_BDGE_PT_SELECTED)
	{
		m_pFstLoopInst = LoopInstanceManager::GetClosestLoopInst(m_pOuterLoopInsts, pt);
		if (m_pFstLoopInst)
		{
			m_pPatternsInFirstLoopInstance = m_pFstLoopInst->GetCacheData()->GetPatternLoop()->GetPatternList();
			IPatternLoopPtr pPatternLoop = m_pFstLoopInst->GetCacheData()->GetPatternLoop();
			pPatternLoop->GetClosestPt(pt, m_fstBdgePt, m_iFstLoopNodeIdx, m_pFstLoopInst->GetTransformMatrix());
		}
	}
	else if (m_iState == FIRST_BDGE_PT_SELECTED)
	{
		m_pSndLoopInst = LoopInstanceManager::GetClosestLoopInst(m_pOuterLoopInsts, pt);
		if (m_pSndLoopInst)
		{
			m_pPatternsInSecondLoopInstance = m_pSndLoopInst->GetCacheData()->GetPatternLoop()->GetPatternList();
			IPatternLoopPtr pPatternLoop = m_pSndLoopInst->GetCacheData()->GetPatternLoop();
			pPatternLoop->GetClosestPt(pt, m_sndBdgePt, m_iSndLoopNodeIdx, m_pSndLoopInst->GetTransformMatrix());
		}
	}

	return TRUE;
}

BOOL BridgeSequenceAddAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	SequenceEditActionBase::OnKeyDown(nChar, nRepCnt, nFlags);

	if (ValueInputManager::GetInstance()->IsInputCancel(nChar))
	{
		m_pNextAction.reset(new CutSequenceSelectAction(m_pPartLayoutData, m_pSequenceData, m_pConfigData, m_pViewPort));
	}

	return TRUE;
}

BOOL BridgeSequenceAddAction::DrawBeforeFinish()
{
    const DisplayParam* pDisplayParam = CLOptionInfo::GetInstance()->GetDisplayParam().get();
    LoopCutFeatureDrawer loopFeatureDrawer(m_pViewPort);

	// 老的设置。
	COLORREF dwOldColor;
	m_pViewPort->GetDrawColor(dwOldColor);
	int iOldLineWid = m_pViewPort->GetLineWidth();

	m_pViewPort->SetDrawColor(*pDisplayParam->GetSelectObjColor());
	if (m_iState == NONE || m_iState == SECOND_BDGE_PT_SELECTED)
	{
		// 绘制第一个轮廓上面的“套圈”。
		if (m_pFstLoopInst)
			loopFeatureDrawer.DrawPosRing(m_fstBdgePt);
	}
	else if (m_iState == FIRST_BDGE_PT_SELECTED)
	{
		// 绘制前一个桥接点。
		loopFeatureDrawer.DrawFeatureLoc(m_fstBdgePt, RGB(255,0,0));

		// 绘制第二个轮廓上面的“套圈”。
		if (m_pSndLoopInst)
			loopFeatureDrawer.DrawPosRing(m_sndBdgePt);
	}

	// 恢复设置。
	m_pViewPort->SetDrawColor(dwOldColor);
	m_pViewPort->SetLineWidth(iOldLineWid);

	// draw cursor.
	CursorDrawer::DrawCrossLineCursor(m_pViewPort);

	return TRUE;
}

void BridgeSequenceAddAction::UpdateForUndoRedo()
{
	// 更新“切割节点”缓存。
	RefreshCutNodes();
}

SnapInputPtr BridgeSequenceAddAction::GetSnapInput() const
{
	SnapInputPtr pSnapInput = SnapInput::BuildSnapInput4SnapNone();

	if (m_iState == NONE)
	{
		if (!m_pFstLoopInst)
			return pSnapInput;

		Matrix2D transformMatrix = m_pFstLoopInst->GetTransformMatrix();
		pSnapInput.reset(new SnapInput(SnapEndPtOf2DLineArc | SnapMiddlePtOf2DLineArc, m_pPatternsInFirstLoopInstance, transformMatrix));
	}
	else if (m_iState == FIRST_BDGE_PT_SELECTED)
	{
		if (!m_pSndLoopInst)
			return pSnapInput;

		Matrix2D transformMatrix = m_pSndLoopInst->GetTransformMatrix();
		pSnapInput.reset(new SnapInput(SnapEndPtOf2DLineArc | SnapMiddlePtOf2DLineArc, m_pPatternsInSecondLoopInstance, transformMatrix));
	}

	return pSnapInput;
}

BOOL BridgeSequenceAddAction::LoopCanAddBdge(LoopInstancePtr pLoopInstance)
{
	CString strProductName, strMsg;
	strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);

	ICutPartSequencePtr pCutSeqUnit = m_pSequenceData->GetCutSequences()->GetCutSequenceByLoopInstance(pLoopInstance.get());
	if (pCutSeqUnit)
	{
		if (pCutSeqUnit->GetCutSequenceType() == CutSequence_CommonCut || pCutSeqUnit->GetCutSequenceType() == CutSequence_ChainCut)
		{
			if (pCutSeqUnit->GetCutSequenceType() == CutSequence_CommonCut)
				strMsg = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_LOOP_NOT_BDGE_1);
			else if (pCutSeqUnit->GetCutSequenceType() == CutSequence_ChainCut)
				strMsg = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_LOOP_NOT_BDGE_2);
			MessageBox(m_pViewPort->GetWnd(), strMsg, strProductName, MB_OK|MB_ICONWARNING);
			return FALSE;
		}
	}

	return TRUE;
}

void BridgeSequenceAddAction::AddBdgeCut()
{
	const IMaterialSize* pMaterialSize = m_pConfigData->GetMatSize();
	const SequenceConfigItem* pSeqConfigItem = m_pConfigData->GetSeqConfigItem();

	// 判断选中的2个“回路实例”上有没有“桥接工序”。
	BOOL bFstHasBdgeSeq = FALSE, bSndHasBdgeSeq = FALSE;
	ICutPartSequencePtr pFstSeqUnit = m_pSequenceData->GetCutSequences()->GetCutSequenceByLoopInstance(m_pFstLoopInst.get());
	if (pFstSeqUnit && pFstSeqUnit->GetCutSequenceType() == CutSequence_Bridge)
		bFstHasBdgeSeq = TRUE;
	ICutPartSequencePtr pSndSeqUnit = m_pSequenceData->GetCutSequences()->GetCutSequenceByLoopInstance(m_pSndLoopInst.get());
	if (pSndSeqUnit && pSndSeqUnit->GetCutSequenceType() == CutSequence_Bridge)
		bSndHasBdgeSeq = TRUE;

	ICommandPtr pCommand;
	if (!bFstHasBdgeSeq && !bSndHasBdgeSeq) // 这2个“回路实例”上都没有“桥接工序”。
	{
		// 增加“桥接工序”。
		BridgePtPtr pParentBridgePt = BridgeSequenceManager::BuildBdgePt(m_pFstLoopInst, m_iFstLoopNodeIdx, m_fstBdgePt);
		BridgePtPtr pChildBdgePt = BridgeSequenceManager::BuildBdgePt(m_pSndLoopInst, m_iSndLoopNodeIdx, m_sndBdgePt);
		pCommand.reset(new BridgeSequenceAddCommand(GetEditorData(), m_pSequenceData->GetCutSequences(), m_pConfigData, m_pPartLayoutData->GetPartInstances(),
			m_pFstLoopInst, pParentBridgePt, m_pSndLoopInst, pChildBdgePt));
	}
	else if (bFstHasBdgeSeq && !bSndHasBdgeSeq) // 只有第一个“回路实例”上有“桥接工序”。
	{
		BridgeSequencePtr pBridgeSequence = boost::dynamic_pointer_cast<BridgeSequence>(pFstSeqUnit);

		// 在已有“桥接节点”下新增“桥接节点”的命令。
		BridgeNodePtr pParentBdgeNode = pBridgeSequence->GetBdgeNodeByLoopInst(m_pFstLoopInst);
		BridgePtPtr pParentBridgePt = BridgeSequenceManager::BuildBdgePt(m_pFstLoopInst, m_iFstLoopNodeIdx, m_fstBdgePt);
		BridgePtPtr pChildBdgePt = BridgeSequenceManager::BuildBdgePt(m_pSndLoopInst, m_iSndLoopNodeIdx, m_sndBdgePt);
		pCommand.reset(new BridgeNodeAddCommand(GetEditorData(), m_pSequenceData->GetCutSequences(), m_pConfigData, m_pPartLayoutData->GetPartInstances(),
			pBridgeSequence, pParentBdgeNode, pParentBridgePt, m_pSndLoopInst, pChildBdgePt));
	}
	else if (!bFstHasBdgeSeq && bSndHasBdgeSeq) // 只有第二个“回路实例”上有“桥接工序”。
	{
		BridgeSequencePtr pBridgeSequence = boost::dynamic_pointer_cast<BridgeSequence>(pSndSeqUnit);

		// 在已有“桥接节点”前插入“桥接节点”的命令。
		BridgeNodePtr pChildBdgeNode = pBridgeSequence->GetBdgeNodeByLoopInst(m_pSndLoopInst);
		BridgePtPtr pChildBridgePt = BridgeSequenceManager::BuildBdgePt(m_pSndLoopInst, m_iSndLoopNodeIdx, m_sndBdgePt);
		BridgePtPtr pParentBdgePt = BridgeSequenceManager::BuildBdgePt(m_pFstLoopInst, m_iFstLoopNodeIdx, m_fstBdgePt);
		pCommand.reset(new BridgeNodeInsertCommand(GetEditorData(), m_pSequenceData->GetCutSequences(), m_pConfigData,
			m_pPartLayoutData->GetPartInstances(), pBridgeSequence, pChildBdgeNode, pChildBridgePt, m_pFstLoopInst, pParentBdgePt));
	}
	else if (bFstHasBdgeSeq && bSndHasBdgeSeq) // 2个“回路实例”都有“桥接工序”。
	{
		BridgeSequencePtr pFstBdgeSeqUnit = boost::dynamic_pointer_cast<BridgeSequence>(pFstSeqUnit);
		BridgeSequencePtr pSndBdgeSeqUnit = boost::dynamic_pointer_cast<BridgeSequence>(pSndSeqUnit);

		// 链接2个“桥接工序”。
		BridgeNodePtr pFstBdgeNode = pFstBdgeSeqUnit->GetBdgeNodeByLoopInst(m_pFstLoopInst);
		BridgePtPtr pFstBdgePt = BridgeSequenceManager::BuildBdgePt(m_pFstLoopInst, m_iFstLoopNodeIdx, m_fstBdgePt);
		BridgeNodePtr pSndBdgeNode = pSndBdgeSeqUnit->GetBdgeNodeByLoopInst(m_pSndLoopInst);
		BridgePtPtr pSndBdgePt = BridgeSequenceManager::BuildBdgePt(m_pSndLoopInst, m_iSndLoopNodeIdx, m_sndBdgePt);
		pCommand.reset(new BridgeSequenceLinkCommand(GetEditorData(), m_pSequenceData->GetCutSequences(), m_pConfigData,
			pFstBdgeSeqUnit, pFstBdgeNode, pFstBdgePt, pSndBdgeSeqUnit, pSndBdgeNode, pSndBdgePt));
	}
	pCommand->Do();
	CommandManager::GetInstance()->AddCommand(pCommand);

	// 更新“切割节点”缓存。
	RefreshCutNodes();
}

END_CUTLEADER_NAMESPACE()

#include "StdAfx.h"
#include "CutSequenceSelectAction.h"

#include "GlViewPort.h"
#include "Rect2D.h"
#include "CursorDrawer.h"
#include "DummyAction.h"
#include "ValueInputManager.h"
#include "LogMgr.h"

#include "SequenceConfigItem.h"
#include "IMaterialSize.h"
#include "CLOptionInfo.h"
#include "DisplayParam.h"

#include "LoopInstanceManager.h"
#include "ICutPartSequence.h"
#include "CutSequenceList.h"
#include "CutSequenceDrawer.h"
#include "CutNodeManager.h"
#include "PartLayoutData.h"
#include "clCutSequenceResource.h"
#include "SequenceData.h"

BEGIN_CUTLEADER_NAMESPACE()

CutSequenceSelectAction::CutSequenceSelectAction(PartLayoutDataPtr pPartLayoutData, SequenceDataPtr pSequenceData, ConfigDataPtr pConfigData, GlViewPortPtr pViewPort)
	: SequenceEditActionBase(pPartLayoutData, pSequenceData, pConfigData, pViewPort)
{
	m_pSelSeqUnitList.reset(new CutSequenceList);
	m_pCurCutSeq.reset();
	m_iMsgID = IDS_SEQ_SELECT_SEQ;

	// 缓存所有添加了工序的“回路实例”。
	m_pAllSequencedLoopInstances = m_pSequenceData->GetCutSequences()->GetAllLoopInstances();

	// 缓存工序模拟信息
	m_pSequenceSimulateShapeList = CutNodeManager::CutNode2SimShape(m_pCutNodeList.get());
}

CutSequenceSelectAction::~CutSequenceSelectAction(void)
{
}

BOOL CutSequenceSelectAction::LButtonDown(double& dPtX, double& dPtY)
{
	if (m_pCurCutSeq)
	{
		if (!ValueInputManager::GetInstance()->IsInputCtrl())
			m_pSelSeqUnitList->clear();

		m_pSelSeqUnitList->push_back(m_pCurCutSeq);

		m_pCurCutSeq.reset();
	}

	return TRUE;
}

BOOL CutSequenceSelectAction::MovePoint(double& dPtX, double& dPtY)
{
	// 得到“加了工序的回路实例”中离光标最近的。
	Point2D pt(dPtX, dPtY);
	LoopInstancePtr pCurrentLoopInstance = LoopInstanceManager::GetClosestLoopInst_hasValidTool(m_pAllSequencedLoopInstances, pt);

	// 得到加在这个“回路实例”上的工序。
	if (pCurrentLoopInstance)
	{
		m_pCurCutSeq = m_pSequenceData->GetCutSequences()->GetCutSequenceByLoopInstance(pCurrentLoopInstance.get());

		// 显示当前的工序类型。
		if (m_pCurCutSeq)
		{
			if (m_pCurCutSeq->GetCutSequenceType() == CutSequence_Loop || m_pCurCutSeq->GetCutSequenceType() == CutSequence_GridLoop)
			{
				m_iMsgID = IDS_SEQ_ACTIVE_IS_LOOP_SEQ;
			}
			else if (m_pCurCutSeq->GetCutSequenceType() == CutSequence_Bridge)
			{
				m_iMsgID = IDS_SEQ_ACTIVE_IS_BDGE_SEQ;
			}
			else if (m_pCurCutSeq->GetCutSequenceType() == CutSequence_CommonCut)
			{
				m_iMsgID = IDS_SEQ_ACTIVE_IS_COMCUT_SEQ;
			}
			else if (m_pCurCutSeq->GetCutSequenceType() == CutSequence_CanvasCommonCut)
			{
				m_iMsgID = IDS_SEQ_ACTIVE_IS_COMCUT_SEQ;
			}
            else if (m_pCurCutSeq->GetCutSequenceType() == CutSequence_GridCut)
            {
                m_iMsgID = IDS_SEQ_ACTIVE_IS_GridCut_SEQ;
            }
            else if (m_pCurCutSeq->GetCutSequenceType() == CutSequence_FlyCut)
            {
                m_iMsgID = IDS_SEQ_ACTIVE_IS_FlyCut_SEQ;
            }
            else if (m_pCurCutSeq->GetCutSequenceType() == CutSequence_ChainCut)
            {
                m_iMsgID = IDS_SEQ_ACTIVE_IS_ChainCut_SEQ;
            }
            else
            {
                LogMgr::DebugWarn(_T("741395"), CWnd::FromHandle(m_pViewPort->GetWnd()));
            }
		}
	}

	return TRUE;
}

BOOL CutSequenceSelectAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	return __super::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CutSequenceSelectAction::DrawBeforeFinish()
{
	CutSequenceDrawer cutSequenceDrawer(m_pViewPort);

	// 绘制当前的工序。
	if (m_pCurCutSeq)
    {
		cutSequenceDrawer.DrawActiveCutSequence(m_pCurCutSeq.get(), m_pCutNodeList.get());
    }

	// 绘制选中的工序。
	cutSequenceDrawer.DrawSelectedCutSequences(m_pSelSeqUnitList.get(), m_pCutNodeList.get());

	// draw cursor.
	CursorDrawer::DrawAimSelCursor(m_pViewPort);

	return TRUE;
}

void CutSequenceSelectAction::UpdateForUndoRedo()
{
	// 更新“切割节点”缓存。
	RefreshCutNodes();
}

END_CUTLEADER_NAMESPACE()

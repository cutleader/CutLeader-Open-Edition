#include "StdAfx.h"
#include "CutSequencePreSelectAction.h"

#include "GlViewPort.h"
#include "Rect2D.h"
#include "CursorDrawer.h"
#include "DummyAction.h"
#include "ValueInputManager.h"

#include "SequenceConfigItem.h"
#include "IMaterialSize.h"
#include "CLOptionInfo.h"
#include "DisplayParam.h"

#include "PartInstanceList.h"
#include "LoopInstance.h"
#include "LoopInstanceManager.h"
#include "ICutPartSequence.h"
#include "CutSequenceList.h"
#include "CutSequenceDrawer.h"
#include "CutNodeManager.h"
#include "PartLayoutData.h"
#include "SequenceData.h"

BEGIN_CUTLEADER_NAMESPACE()

CutSequencePreSelectAction::CutSequencePreSelectAction(PartLayoutDataPtr pPartLayoutData, SequenceDataPtr pSequenceData, 
	ConfigDataPtr pConfigData, GlViewPortPtr pViewPort, ApplyPartActionBar* pApplyPartActionBar)
	: SequenceEditActionBase(pPartLayoutData, pSequenceData, pConfigData, pViewPort)
{
	m_pApplyPartActionBar = pApplyPartActionBar;

	m_pCurrentCutSequences.reset(new CutSequenceList);
	m_pSelectCutSequences.reset(new CutSequenceList);

	m_iPreSelectSequenceActionState = PreSelectSequenceAction_active;

	// 缓存所有添加了工序的“回路实例”。
	m_pAllSequencedLoopInstances = m_pSequenceData->GetCutSequences()->GetAllLoopInstances();
}

CutSequencePreSelectAction::~CutSequencePreSelectAction(void)
{
}

BOOL CutSequencePreSelectAction::LButtonDown(double& dPtX, double& dPtY)
{
	if (m_pCurrentCutSequences->size() > 0)
		m_pSelectCutSequences->AppendCutSequences(m_pCurrentCutSequences);

	return TRUE;
}

BOOL CutSequencePreSelectAction::MovePoint(double& dPtX, double& dPtY)
{
	Point2D pt(dPtX, dPtY);

	m_pCurrentCutSequences->clear();

	// “加了工序的回路实例”中离光标最近的。
	LoopInstancePtr pCurrentLoopInstance = LoopInstanceManager::GetClosestLoopInst_hasValidTool(m_pAllSequencedLoopInstances, pt);
	if (pCurrentLoopInstance)
	{
		// 加在这个“回路实例”上的工序。
		ICutPartSequencePtr pCutPartSeq = m_pSequenceData->GetCutSequences()->GetCutSequenceByLoopInstance(pCurrentLoopInstance.get());
		if (m_pApplyPartActionBar->IsApplyPart())
		{
			if (pCutPartSeq->GetCutSequenceType() == CutSequence_Bridge || pCutPartSeq->GetCutSequenceType() == CutSequence_ChainCut || pCutPartSeq->GetCutSequenceType() == CutSequence_GridCut)
			{
				PartInstanceListPtr pPartInstances = pCutPartSeq->GetPartInstances(); // 工序涉及的零件实例。
				m_pCurrentCutSequences->AppendCutSequences(m_pSequenceData->GetCutSequences()->GetCutSequencesByPartInstances(pPartInstances)); // 得到加在这些“零件实例”上的工序。
			}
			else
			{
				// 这个工序所在的零件外轮廓是否分配了桥或者连割工序。
				LoopInstancePtr pOutLoopInst = LoopInstanceManager::GetOuterLoopInst(pCurrentLoopInstance->GetPartInstance());
				if (pOutLoopInst)
				{
					ICutPartSequencePtr pCutPartSeq1 = m_pSequenceData->GetCutSequences()->GetCutSequenceByLoopInstance(pOutLoopInst.get());
					if (pCutPartSeq1 && (pCutPartSeq1->GetCutSequenceType() == CutSequence_Bridge || pCutPartSeq1->GetCutSequenceType() == CutSequence_ChainCut || pCutPartSeq->GetCutSequenceType() == CutSequence_GridCut))
					{
						PartInstanceListPtr pPartInstances = pCutPartSeq1->GetPartInstances(); // 工序涉及的零件实例。
						m_pCurrentCutSequences->AppendCutSequences(m_pSequenceData->GetCutSequences()->GetCutSequencesByPartInstances(pPartInstances)); // 加在这些零件上的工序。
					}
					else
						m_pCurrentCutSequences->AppendCutSequences(m_pSequenceData->GetCutSequences()->GetCutSequencesByPartInstance(pCurrentLoopInstance->GetPartInstance()));
				}
				else
					m_pCurrentCutSequences->AppendCutSequences(m_pSequenceData->GetCutSequences()->GetCutSequencesByPartInstance(pCurrentLoopInstance->GetPartInstance()));
			}
		}
		else
			m_pCurrentCutSequences->push_back(pCutPartSeq);
	}

	return TRUE;
}

BOOL CutSequencePreSelectAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	SequenceEditActionBase::OnKeyDown(nChar, nRepCnt, nFlags);

	return TRUE;
}

BOOL CutSequencePreSelectAction::DrawBeforeFinish()
{
	CutSequenceDrawer cutSequenceDrawer(m_pViewPort);

	// 绘制当前的工序。
	if (m_pCurrentCutSequences->size() > 0)
    {
		cutSequenceDrawer.DrawActiveCutSequences(m_pCurrentCutSequences.get(), m_pCutNodeList.get());
    }

	// 绘制选中的工序。
	cutSequenceDrawer.DrawSelectedCutSequences(m_pSelectCutSequences.get(), m_pCutNodeList.get());

	// draw cursor.
	CursorDrawer::DrawAimSelCursor(m_pViewPort);

	return TRUE;
}

END_CUTLEADER_NAMESPACE()

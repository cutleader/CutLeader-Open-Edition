#include "StdAfx.h"
#include "CutSequenceOrderChangeActionA.h"

#include "boost/bind.hpp"
#include "baseConst.h"
#include "GlViewPort.h"
#include "CommandManager.h"
#include "CursorDrawer.h"
#include "clBaseDataResource.h"
#include "ValueInputManager.h"

#include "SequenceConfigItem.h"
#include "IMaterialSize.h"
#include "CLOptionInfo.h"
#include "DisplayParam.h"

#include "LoopInstance.h"
#include "LoopInstanceList.h"
#include "LoopInstanceManager.h"
#include "PartLayoutData.h"
#include "ICutPartSequence.h"
#include "CutSequenceList.h"
#include "CutSequenceDrawer.h"
#include "clCutSequenceResource.h"
#include "CutNodeManager.h"
#include "SequenceLocationMoveCommand.h"
#include "CutSequenceSelectAction.h"
#include "ConfigData.h"
#include "SequenceData.h"

BEGIN_CUTLEADER_NAMESPACE()

CutSequenceOrderChangeActionA::CutSequenceOrderChangeActionA(PartLayoutDataPtr pPartLayoutData, SequenceDataPtr pSequenceData,
	ConfigDataPtr pConfigData, GlViewPortPtr pViewPort, ApplyPartActionBar* pApplyPartActionBar)
	: SequenceEditActionBase(pPartLayoutData, pSequenceData, pConfigData, pViewPort)
{
	m_pApplyPartActionBar = pApplyPartActionBar;

	m_iMsgID = IDS_SEQ_SELECT_SEQ_1;

	m_pCurrentCutSequences.reset(new CutSequenceList);
	m_pBaseCutSequence.reset();

	// 缓存所有添加了工序的“回路实例”。
	m_pAllSequencedLoopInstances = m_pSequenceData->GetCutSequences()->GetAllLoopInstances();

	m_pApplyPartActionBar->Show_ApplyPartActionBar();
}

CutSequenceOrderChangeActionA::~CutSequenceOrderChangeActionA(void)
{
}

BOOL CutSequenceOrderChangeActionA::LButtonDown(double& dPtX, double& dPtY)
{
	CString strProductName, strMsg;
	strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);
	if (m_pCurrentCutSequences->size() > 0)
	{
		const IMaterialSize* pMaterialSize = m_pConfigData->GetMatSize();
		const SequenceConfigItem* pSeqConfigItem = m_pConfigData->GetSeqConfigItem();

		// 先把工序按照顺序排一下序。
		sort(m_pCurrentCutSequences->begin(), m_pCurrentCutSequences->end(), boost::bind(&IsSeqBefore, _1, _2, m_pSequenceData->GetCutSequences()));

		if (!m_pBaseCutSequence) // 把选择的工序放到最前面。
		{
			// 执行命令。
			ICutSequencePtr pBaseCutSeq;
			ICommandPtr pCommand(new SequenceLocationMoveCommand(GetEditorData(), m_pSequenceData->GetCutSequences(), m_pConfigData, m_pCurrentCutSequences, pBaseCutSeq, m_pPartLayoutData->GetPartInstances()));
			pCommand->Do();
			CommandManager::GetInstance()->AddCommand(pCommand);

			// 更新“切割节点”缓存。
			RefreshCutNodes();
		}
		else
		{
			// 要移动的工序不能包含“基准工序”。
			if (m_pCurrentCutSequences->GetCutSequenceIndex(m_pBaseCutSequence.get()) != INVALID_IDX)
				return FALSE;

			// 执行命令。
			ICommandPtr pCommand(new SequenceLocationMoveCommand(GetEditorData(), m_pSequenceData->GetCutSequences(), m_pConfigData, m_pCurrentCutSequences, m_pBaseCutSequence, m_pPartLayoutData->GetPartInstances()));
			pCommand->Do();
			CommandManager::GetInstance()->AddCommand(pCommand);

			// 更新“切割节点”缓存。
			RefreshCutNodes();
		}

		m_pBaseCutSequence = m_pCurrentCutSequences->at(m_pCurrentCutSequences->size()-1);
		m_pCurrentCutSequences->clear();
		m_iMsgID = IDS_SEQ_SELECT_SEQ_2;
	}
	else
	{
		strMsg = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_NO_SEQ_SELECT);
		MessageBox(m_pViewPort->GetWnd(), strMsg, strProductName, MB_OK | MB_ICONWARNING);
	}

	return TRUE;
}

BOOL CutSequenceOrderChangeActionA::MovePoint(double& dPtX, double& dPtY)
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
				// 这个工序所在的零件外轮廓是否分配了桥接/连割工序。
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

BOOL CutSequenceOrderChangeActionA::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	SequenceEditActionBase::OnKeyDown(nChar, nRepCnt, nFlags);

	if (ValueInputManager::GetInstance()->IsInputCancel(nChar))
	{
		m_pNextAction.reset(new CutSequenceSelectAction(m_pPartLayoutData, m_pSequenceData, m_pConfigData, m_pViewPort));
	}

	return TRUE;
}

BOOL CutSequenceOrderChangeActionA::DrawBeforeFinish()
{
	CutSequenceDrawer cutSequenceDrawer(m_pViewPort);

	// 绘制“基准工序”。
	if (m_pBaseCutSequence)
    {
		cutSequenceDrawer.DrawSelectedCutSequence(m_pBaseCutSequence.get(), m_pCutNodeList.get());
    }

	// 绘制当前的工序。
	if (m_pCurrentCutSequences->size() > 0)
    {
		cutSequenceDrawer.DrawActiveCutSequences(m_pCurrentCutSequences.get(), m_pCutNodeList.get());
    }

	// draw cursor.
	CursorDrawer::DrawAimSelCursor(m_pViewPort);

	return TRUE;
}

void CutSequenceOrderChangeActionA::UpdateForUndoRedo()
{
	// 更新“切割节点”缓存。
	RefreshCutNodes();
}

END_CUTLEADER_NAMESPACE()

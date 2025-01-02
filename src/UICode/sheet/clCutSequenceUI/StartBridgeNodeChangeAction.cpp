#include "StdAfx.h"
#include "StartBridgeNodeChangeAction.h"

#include "GlViewPort.h"
#include "CommandManager.h"
#include "CursorDrawer.h"
#include "clBaseDataResource.h"
#include "ValueInputManager.h"

#include "SequenceConfigItem.h"
#include "IMaterialSize.h"
#include "CLOptionInfo.h"
#include "DisplayParam.h"

#include "LoopDrawer.h"
#include "LoopInstanceCache.h"
#include "LoopInstance.h"
#include "LoopInstanceList.h"
#include "LoopInstanceManager.h"
#include "LoopToolManager.h"
#include "clCutSequenceResource.h"
#include "CutNodeList.h"
#include "CutSequenceList.h"
#include "CutSequenceDrawer.h"
#include "CutNodeManager.h"
#include "BridgeNode.h"
#include "BridgeSequence.h"
#include "BridgeSequenceManager.h"
#include "PartLayoutData.h"
#include "StartBridgeNodeChangeCommand.h"
#include "CutSequenceSelectAction.h"
#include "SequenceData.h"

BEGIN_CUTLEADER_NAMESPACE()

StartBridgeNodeChangeAction::StartBridgeNodeChangeAction(PartLayoutDataPtr pPartLayoutData, SequenceDataPtr pSequenceData, ConfigDataPtr pConfigData, GlViewPortPtr pViewPort)
	: SequenceEditActionBase(pPartLayoutData, pSequenceData, pConfigData, pViewPort)
{
	m_iMsgID = IDS_SEQ_BDGE_START_SELECT;

	// 缓存所有“桥接工序”中的“回路实例”。
	m_pBdgeLoopInsts.reset(new LoopInstanceList);
	CutSequenceListPtr pBdgeSeqUnits = m_pSequenceData->GetCutSequences()->GetCutSequencesByType(CutSequence_Bridge);
	for (unsigned int i = 0; i < pBdgeSeqUnits->size(); i++)
	{
		BridgeSequencePtr pBridgeSequence = boost::dynamic_pointer_cast<BridgeSequence>(pBdgeSeqUnits->at(i));
		pBridgeSequence->GetRootBdgeNode()->GetAllLoopInsts(m_pBdgeLoopInsts);
	}
}

StartBridgeNodeChangeAction::~StartBridgeNodeChangeAction(void)
{
}

BOOL StartBridgeNodeChangeAction::LButtonDown(double& dPtX, double& dPtY)
{
	if (m_pActiveLoopInstance)
	{
		// 得到当前“回路实例”所在的“桥接工序”和“工序节点”。
		BridgeSequencePtr pCurBdgeSeqUnit;
		BridgeNodePtr pCurBdgeNode;
		CutSequenceListPtr pBdgeSeqUnits = m_pSequenceData->GetCutSequences()->GetCutSequencesByType(CutSequence_Bridge);
		for (unsigned int i = 0; i < pBdgeSeqUnits->size(); i++)
		{
			BridgeSequencePtr pBridgeSequence = boost::dynamic_pointer_cast<BridgeSequence>(pBdgeSeqUnits->at(i));
			BridgeNodePtr pBridgeNode = pBridgeSequence->GetBdgeNodeByLoopInst(m_pActiveLoopInstance);
			if (pBridgeNode)
			{
				pCurBdgeSeqUnit = pBridgeSequence;
				pCurBdgeNode = pBridgeNode;
				break;
			}
		}

		// 修改“桥接工序”的起始回路。
		ICommandPtr pCommand(new StartBridgeNodeChangeCommand(GetEditorData(), m_pSequenceData->GetCutSequences(), m_pConfigData, pCurBdgeSeqUnit, pCurBdgeNode));
		pCommand->Do();
		CommandManager::GetInstance()->AddCommand(pCommand);

		// 更新“切割节点”缓存。
		RefreshCutNodes();
	}

	return TRUE;
}

BOOL StartBridgeNodeChangeAction::MovePoint(double& dPtX, double& dPtY)
{
	Point2D pt(dPtX, dPtY);
	m_pActiveLoopInstance = LoopInstanceManager::GetClosestLoopInst(m_pBdgeLoopInsts, pt);

	return TRUE;
}

BOOL StartBridgeNodeChangeAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	SequenceEditActionBase::OnKeyDown(nChar, nRepCnt, nFlags);

	if (ValueInputManager::GetInstance()->IsInputCancel(nChar))
	{
		m_pNextAction.reset(new CutSequenceSelectAction(m_pPartLayoutData, m_pSequenceData, m_pConfigData, m_pViewPort));
	}

	return TRUE;
}

BOOL StartBridgeNodeChangeAction::DrawBeforeFinish()
{
    LoopDrawer loopDrawer(m_pViewPort);

    // 绘制当前的“回路实例”。
    if (m_pActiveLoopInstance)
    {
        const LoopToolData* pLoopToolData = m_pActiveLoopInstance->GetCacheData()->GetLoopTool().get();
        loopDrawer.DrawActiveLoop(m_pActiveLoopInstance->GetCacheData()->GetPatternLoop().get(), LoopToolManager::GetLoopDisplayColor(pLoopToolData), m_pActiveLoopInstance->GetTransformMatrix());
    }

    // draw cursor.
    CursorDrawer::DrawCrossLineCursor(m_pViewPort);

	return TRUE;
}

void StartBridgeNodeChangeAction::UpdateForUndoRedo()
{
	// 更新“切割节点”缓存。
	RefreshCutNodes();
}

END_CUTLEADER_NAMESPACE()

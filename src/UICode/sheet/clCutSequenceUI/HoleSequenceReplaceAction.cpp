#include "StdAfx.h"
#include "HoleSequenceReplaceAction.h"

#include "GlViewPort.h"
#include "CursorDrawer.h"
#include "clBaseDataResource.h"
#include "ValueInputManager.h"
#include "CommandManager.h"

#include "SequenceConfigItem.h"
#include "IMaterialSize.h"
#include "CLOptionInfo.h"
#include "DisplayParam.h"

#include "IPatternLoop.h"
#include "PartItem.h"
#include "LoopInstanceCache.h"
#include "LoopInstance.h"
#include "LoopInstanceList.h"
#include "PartInstance.h"
#include "PartInstanceList.h"
#include "PartPlacementDrawer.h"
#include "LoopInstanceManager.h"
#include "PartLayoutData.h"
#include "clCutSequenceResource.h"
#include "LoopSequence.h"
#include "GridLoopSequence.h"
#include "CutSequenceList.h"
#include "CutNodeManager.h"
#include "CutSequenceDrawer.h"
#include "CutSequenceSelectAction.h"
#include "CutSequenceManager.h"
#include "ConfigData.h"
#include "HoleSequenceReplaceCommand.h"
#include "SequenceData.h"

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

HoleSequenceReplaceAction::HoleSequenceReplaceAction(PartLayoutDataPtr pPartLayoutData, SequenceDataPtr pSequenceData, ConfigDataPtr pConfigData, GlViewPortPtr pViewPort)
    : SequenceEditActionBase(pPartLayoutData, pSequenceData, pConfigData, pViewPort)
{
	m_iMsgID = IDS_SEQ_REPLACE_HOLESEQ_SELECT_PART;

	m_pBasePartInst.reset();

	// 得到所有“回路实例”。
	m_pLoopInstanceList = LoopInstanceManager::GetLoopInstanceList(m_pPartLayoutData->GetPartInstances());

	// 计算零件实例中各个回路实例的外接矩形。
	PartInstanceListPtr pPartInstances = m_pPartLayoutData->GetPartInstances();
	m_partRectMap = pPartInstances->GetLoopInstanceRect();
}

HoleSequenceReplaceAction::~HoleSequenceReplaceAction(void)
{
}

BOOL HoleSequenceReplaceAction::LButtonDown(double& dPtX, double& dPtY)
{
	CString strProductName, strMsg;
	strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);

	// 做一些检查。
	{
		// 有没有选中零件。
		if (!m_pBasePartInst)
		{
			strMsg = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_REPLACE_HOLESEQ_SELECT_PART);
			MessageBox(m_pViewPort->GetWnd(), strMsg, strProductName, MB_OK | MB_ICONWARNING);
			return TRUE;
		}

		// 零件内部轮廓个数要大于1。
		LoopInstanceListPtr pInnerLoopInsts_source = LoopInstanceManager::GetInnerLoopInsts(m_pBasePartInst);
		if (pInnerLoopInsts_source->size() == 0)
		{
			strMsg = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_REPLACE_HOLESEQ_2);
			MessageBox(m_pViewPort->GetWnd(), strMsg, strProductName, MB_OK | MB_ICONWARNING);
			return TRUE;
		}

		// 源零件内部轮廓是否分配了工序。
		map<int, ICutSequencePtr> innerLoopSeqs_source = m_pSequenceData->GetCutSequences()->GetCutSequencesByLoopInstances_2(pInnerLoopInsts_source.get());
		if (innerLoopSeqs_source.size() == 0)
		{
			strMsg = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_REPLACE_HOLESEQ_1);
			MessageBox(m_pViewPort->GetWnd(), strMsg, strProductName, MB_OK | MB_ICONWARNING);
			return TRUE;
		}

		// 源零件的内部轮廓的工序是不是连续的。
		if (!IsSeqLinked(innerLoopSeqs_source))
		{
			strMsg = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_REPLACE_HOLESEQ_3);
			MessageBox(m_pViewPort->GetWnd(), strMsg, strProductName, MB_OK | MB_ICONWARNING);
			return TRUE;
		}

		// 有没有兄弟零件。
		PartInstanceListPtr pPartInsts_target = m_pPartLayoutData->GetPartInstances()->GetPartInstances_brother(m_pBasePartInst);
		if (pPartInsts_target->size() == 0)
		{
			strMsg = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_REPLACE_HOLESEQ_5);
			MessageBox(m_pViewPort->GetWnd(), strMsg, strProductName, MB_OK | MB_ICONWARNING);
			return TRUE;
		}
	}

	// 执行替换。
	strMsg = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_REPLACE_HOLESEQ_4);
	if (MessageBox(m_pViewPort->GetWnd(), strMsg, strProductName, MB_YESNO | MB_ICONQUESTION) == IDYES)
	{
		// 增加工序。
		PartInstanceListPtr pPartInsts_target = m_pPartLayoutData->GetPartInstances()->GetPartInstances_brother(m_pBasePartInst);
		ICommandPtr pCommand(new HoleSequenceReplaceCommand(GetEditorData(), m_pSequenceData->GetCutSequences(), m_pConfigData,
			m_pPartLayoutData->GetPartInstances(), m_pBasePartInst, pPartInsts_target));
		pCommand->Do();
		CommandManager::GetInstance()->AddCommand(pCommand);

		// 更新“切割节点”缓存。
		RefreshCutNodes();

		// 重设变量。
		m_pBasePartInst.reset();
	}

	return TRUE;
}

BOOL HoleSequenceReplaceAction::MovePoint(double& dPtX, double& dPtY)
{
	Point2D pt(dPtX, dPtY);

	// 得到离光标最近的“零件实例”。
	PartInstanceListPtr pPartInstances = m_pPartLayoutData->GetPartInstances();
	m_pBasePartInst = pPartInstances->GetClosestPartInstance(pt, m_partRectMap);

	return TRUE;
}

BOOL HoleSequenceReplaceAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	SequenceEditActionBase::OnKeyDown(nChar, nRepCnt, nFlags);

	if (ValueInputManager::GetInstance()->IsInputCancel(nChar))
	{
		m_pNextAction.reset(new CutSequenceSelectAction(m_pPartLayoutData, m_pSequenceData, m_pConfigData, m_pViewPort));
	}

	return TRUE;
}

BOOL HoleSequenceReplaceAction::DrawBeforeFinish()
{
	// 绘制当前的“零件实例”。
	if (m_pBasePartInst)
	{
		PartPlacementDrawer partPlacementDrawer(m_pViewPort);
		partPlacementDrawer.DrawActivePartInstance(m_pBasePartInst.get());
	}

	// draw cursor.
	CursorDrawer::DrawAimSelCursor(m_pViewPort);

	return TRUE;
}

void HoleSequenceReplaceAction::UpdateForUndoRedo()
{
	// 更新“切割节点”缓存。
	RefreshCutNodes();
}

BOOL HoleSequenceReplaceAction::IsSeqLinked(map<int, ICutSequencePtr> seqMap)
{
	int iPreIndex = -1;
	for (map<int, ICutSequencePtr>::iterator iter = seqMap.begin(); iter != seqMap.end(); iter++)
	{
		LONGLONG iIndex = iter->first;
		if (iPreIndex == -1)
		{
			iPreIndex = iIndex;
		}
		else
		{
			if (iIndex == (iPreIndex+1))
				iPreIndex = iIndex;
			else
				return FALSE;
		}
	}

	return TRUE;
}

END_CUTLEADER_NAMESPACE()

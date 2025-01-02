#include "StdAfx.h"
#include "CutSequenceDeleteAction.h"

#include "boost/bind.hpp"
#include "GlViewPort.h"
#include "CommandManager.h"
#include "ValueInputManager.h"
#include "clBaseDataResource.h"

#include "clCutSequenceResource.h"
#include "CutSequenceDeleteCommand.h"
#include "CutSequenceList.h"
#include "CutSequenceSelectAction.h"
#include "SequenceData.h"

BEGIN_CUTLEADER_NAMESPACE()

CutSequenceDeleteAction::CutSequenceDeleteAction(PartLayoutDataPtr pPartLayoutData, SequenceDataPtr pSequenceData, ConfigDataPtr pConfigData,
	GlViewPortPtr pViewPort, ApplyPartActionBar* pApplyPartActionBar)
	: CutSequencePreSelectAction(pPartLayoutData, pSequenceData, pConfigData, pViewPort, pApplyPartActionBar)
{
	m_iMsgID = IDS_SEQ_DELETE_SEQUENCE;

	// if some sequence unit were pre-selected, disable CutSequencePreSelectAction.
	if (m_pSelectCutSequences->size() > 0) {
		m_iPreSelectSequenceActionState = PreSelectSequenceAction_inactive;
    }

	m_pApplyPartActionBar->Show_ApplyPartActionBar();
}

CutSequenceDeleteAction::~CutSequenceDeleteAction(void)
{
}

BOOL CutSequenceDeleteAction::LButtonDown(double& dPtX, double& dPtY)
{
	if (m_iPreSelectSequenceActionState == PreSelectSequenceAction_active)
	{
		CutSequencePreSelectAction::LButtonDown(dPtX, dPtY);
	}

	// 删除选中的工序。
	if (m_pSelectCutSequences->size() > 0)
	{
		// 先把工序按照顺序排一下序。
		sort(m_pSelectCutSequences->begin(), m_pSelectCutSequences->end(), boost::bind(&IsSeqBefore, _1, _2, m_pSequenceData->GetCutSequences()));

		// 执行命令。
		ICommandPtr pCommand(new CutSequenceDeleteCommand(GetEditorData(), m_pSequenceData->GetCutSequences(), m_pConfigData, m_pSelectCutSequences));
		pCommand->Do();
		CommandManager::GetInstance()->AddCommand(pCommand);

		// 清空选中的工序。
		m_pCurrentCutSequences->clear();
		m_pSelectCutSequences->clear();

		// 更新所有添加了工序的“回路实例”。
		m_pAllSequencedLoopInstances = m_pSequenceData->GetCutSequences()->GetAllLoopInstances();

		// 更新“切割节点”缓存。
		RefreshCutNodes();
	}
	else
	{
		CString strProductName, strMsg;
		strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);
		strMsg = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_NO_SEQ_SELECT);
		MessageBox(m_pViewPort->GetWnd(), strMsg, strProductName, MB_OK | MB_ICONWARNING);
	}

	return TRUE;
}

BOOL CutSequenceDeleteAction::MovePoint(double& dPtX, double& dPtY)
{
	CutSequencePreSelectAction::MovePoint(dPtX, dPtY);

	return TRUE;
}

BOOL CutSequenceDeleteAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	return __super::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CutSequenceDeleteAction::DrawBeforeFinish()
{
	return __super::DrawBeforeFinish();
}

void CutSequenceDeleteAction::UpdateForUndoRedo()
{
	// 缓存所有添加了工序的“回路实例”。
	m_pAllSequencedLoopInstances = m_pSequenceData->GetCutSequences()->GetAllLoopInstances();

	// 更新“切割节点”缓存。
	RefreshCutNodes();

	m_pCurrentCutSequences->clear();
	m_pSelectCutSequences->clear();
}

void CutSequenceDeleteAction::SelectSeqUnits(CutSequenceListPtr pCutSequences)
{
	ICommandPtr pCommand(new CutSequenceDeleteCommand(GetEditorData(), m_pSequenceData->GetCutSequences(), m_pConfigData, pCutSequences));
	pCommand->Do();
	CommandManager::GetInstance()->AddCommand(pCommand);

	// 更新所有添加了工序的“回路实例”。
	m_pAllSequencedLoopInstances = m_pSequenceData->GetCutSequences()->GetAllLoopInstances();

	// 更新“切割节点”缓存。
	RefreshCutNodes();
}

END_CUTLEADER_NAMESPACE()

#include "StdAfx.h"
#include "SequenceEditor.h"

#include "GlViewPort.h"
#include "IAction.h"
#include "DummyAction.h"
#include "clBaseDataResource.h"
#include "LogMgr.h"
#include "DataBaseManager.h"

#include "ParamConfigItem.h"
#include "LeadConfigItem.h"
#include "SequenceConfigItem.h"
#include "ParamConfig.h"
#include "IMaterialSize.h"
#include "ExpertLibItemLoader.h"
#include "MachineItemLoader.h"

#include "PartPlacementList.h"
#include "PartInstanceList.h"
#include "LoopInstanceList.h"
#include "LoopSequenceAddAction.h"
#include "BridgeSequenceAddAction.h"
#include "BridgeSequenceBreakAction.h"
#include "StartBridgeNodeChangeAction.h"
#include "LoopSequenceInsertAction.h"
#include "HoleSequenceReplaceAction.h"
#include "CutSequenceDeleteAction.h"
#include "CutSequenceSelectAction.h"
#include "SequenceViewAction.h"
#include "CutSequenceOrderChangeActionA.h"
#include "CutSequenceOrderChangeActionB.h"
#include "MovePtAddAction.h"
#include "MovePtDeleteAction.h"
#include "ChainCutSequenceAddAction.h"
#include "ChainNodeInsertAction.h"
#include "ChainNodeDeleteAction.h"
#include "ResetInstructionAddAction.h"
#include "ResetInstructionDeleteAction.h"
#include "GridCutSequenceAddAction.h"
#include "RemnantCutlineAutoAction.h"
#include "RemnantCutlineAddAction.h"
#include "RemnantCutlineDeleteAction.h"
#include "FlyCutSequenceAddAction.h"
#include "CutSequenceList.h"
#include "CutNodeSimulator.h"
#include "clCutSequenceResource.h"
#include "ConfigData.h"
#include "CanvasGeometryEditAction.h"
#include "PartPlacementEditAction.h"
#include "CutSequencePane.h"
#include "SequenceData.h"

BEGIN_CUTLEADER_NAMESPACE()

SequenceEditor::SequenceEditor(GlViewPortPtr pViewPort)
{
	m_pViewPort = pViewPort;
	m_pAction.reset(new DummyAction(m_pViewPort));

	{
		m_pApplyPartActionBar = new ApplyPartActionBar();
		m_pApplyPartActionBar->Create(ApplyPartActionBar::IDD);
		m_pApplyPartActionBar->ShowWindow(SW_HIDE);

		m_pInsertPositionActionBar = new InsertPositionActionBar();
		m_pInsertPositionActionBar->Create(InsertPositionActionBar::IDD);
		m_pInsertPositionActionBar->ShowWindow(SW_HIDE);

		m_pGenerateRemCutLineActionBar = new GenerateRemCutLineActionBar();
		m_pGenerateRemCutLineActionBar->Create(GenerateRemCutLineActionBar::IDD);
		m_pGenerateRemCutLineActionBar->ShowWindow(SW_HIDE);
	}
}

SequenceEditor::~SequenceEditor(void)
{
	// 这里先销毁action，因为有的action可能会监听m_pGenerateRemCutLineActionBar。
	m_pAction.reset();
	if (m_pGenerateRemCutLineActionBar) {
		delete m_pGenerateRemCutLineActionBar;
		m_pGenerateRemCutLineActionBar = NULL;
	}
}

BOOL SequenceEditor::LButtonDown(double& dPtX, double& dPtY)
{
	if (m_pAction)
	{
		m_pAction->LButtonDown(dPtX, dPtY);

		// check whether we will enter other action.
		IActionPtr pNextAction = m_pAction->GetNextAction();
		if (pNextAction)
			m_pAction = pNextAction;
	}

	return TRUE;
}

BOOL SequenceEditor::LButtonUp(double& dPtX, double& dPtY)
{
	if (m_pAction)
	{
		m_pAction->LButtonUp(dPtX, dPtY);

		// check whether we will enter other action.
		IActionPtr pNextAction = m_pAction->GetNextAction();
		if (pNextAction)
			m_pAction = pNextAction;
	}

	return TRUE;
}

BOOL SequenceEditor::LButtonDBClk(double& dPtX, double& dPtY)
{
	if (m_pAction)
	{
		m_pAction->LButtonDBClk(dPtX, dPtY);

		// check whether we will enter other action.
		IActionPtr pNextAction = m_pAction->GetNextAction();
		if (pNextAction)
			m_pAction = pNextAction;
	}

	return TRUE;
}

BOOL SequenceEditor::RButtonUp(double& dPtX, double& dPtY)
{
	if (m_pAction)
	{
		m_pAction->RButtonUp(dPtX, dPtY);

		// check whether we will enter other action.
		IActionPtr pNextAction = m_pAction->GetNextAction();
		if (pNextAction)
			m_pAction = pNextAction;
	}

	return TRUE;
}

BOOL SequenceEditor::MovePoint(double& dPtX, double& dPtY)
{
	if (m_pAction)
	{
		m_pAction->MovePoint(dPtX, dPtY);

		// check whether we will enter other action.
		IActionPtr pNextAction = m_pAction->GetNextAction();
		if (pNextAction)
			m_pAction = pNextAction;
	}

	return TRUE;
}

BOOL SequenceEditor::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (m_pAction)
	{
		m_pAction->OnKeyDown(nChar, nRepCnt, nFlags);

		// check whether we will enter other action.
		IActionPtr pNextAction = m_pAction->GetNextAction();
		if (pNextAction)
			m_pAction = pNextAction;
	}

	return TRUE;
}

BOOL SequenceEditor::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (m_pAction)
	{
		m_pAction->OnKeyUp(nChar, nRepCnt, nFlags);

		// check whether we will enter other action.
		IActionPtr pNextAction = m_pAction->GetNextAction();
		if (pNextAction)
			m_pAction = pNextAction;
	}

	return TRUE;
}

CString SequenceEditor::GetEditorName() const
{
	CString str;
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ);
	return str;
}

BOOL SequenceEditor::SetAction(SequenceActionType actionType)
{
	// 先隐藏所有action bar。
	HideAllActionBars();

	// prepare some data.
	LeadConfigItemPtr pLeadConfigItem = boost::dynamic_pointer_cast<LeadConfigItem>(m_pParamConfig->GetItemByType(PARAMCONFIG_ITEM_LEAD_ASSIGN));
	SequenceConfigItemPtr pSeqConfigItem = boost::dynamic_pointer_cast<SequenceConfigItem>(m_pParamConfig->GetItemByType(PARAMCONFIG_ITEM_GENERATE_SEQ));
	LONGLONG iMachineItemID = ExpertLibItemLoader::GetMacIDByExpLibItem(DataBaseManager::GetInstance()->GetExpLibDBConnect(), m_pParamConfig->GetParentID());
	MachineParamPtr pMachineParam = MachineItemLoader::LoadMacParam(DataBaseManager::GetInstance()->GetMacLibDBConnect(), iMachineItemID);
	ConfigDataPtr pConfigData(new ConfigData(pLeadConfigItem, pSeqConfigItem, m_pMatSize, pMachineParam));

	SequenceEditActionBase* pSequenceEditActionBase;
	if (actionType == SEQUENCE_SELECT)
	{
		pSequenceEditActionBase = new CutSequenceSelectAction(m_pPartLayoutData, m_pSequenceData, pConfigData, m_pViewPort);
		m_pAction.reset(pSequenceEditActionBase);
	}
	else if (actionType == SEQUENCE_SORT_A)
	{
		pSequenceEditActionBase = new CutSequenceOrderChangeActionA(m_pPartLayoutData, m_pSequenceData, pConfigData, m_pViewPort, m_pApplyPartActionBar);
		m_pAction.reset(pSequenceEditActionBase);
	}
	else if (actionType == SEQUENCE_SORT_B)
	{
		pSequenceEditActionBase = new CutSequenceOrderChangeActionB(m_pPartLayoutData, m_pSequenceData, pConfigData, m_pViewPort, m_pApplyPartActionBar);
		m_pAction.reset(pSequenceEditActionBase);
	}
	else if (actionType == SEQUENCE_ADD_LOOPSEQ)
	{
		pSequenceEditActionBase = new LoopSequenceAddAction(m_pPartLayoutData, m_pSequenceData, pConfigData, m_pViewPort, m_pApplyPartActionBar);
		m_pAction.reset(pSequenceEditActionBase);
	}
	else if (actionType == SEQUENCE_ADD_BDGESEQ)
	{
		pSequenceEditActionBase = new BridgeSequenceAddAction(m_pPartLayoutData, m_pSequenceData, pConfigData, m_pViewPort);
		m_pAction.reset(pSequenceEditActionBase);
	}
	else if (actionType == SEQUENCE_BREAK_BDGESEQ)
	{
		pSequenceEditActionBase = new BridgeSequenceBreakAction(m_pPartLayoutData, m_pSequenceData, pConfigData, m_pViewPort);
		m_pAction.reset(pSequenceEditActionBase);
	}
	else if (actionType == SEQUENCE_BDGE_START_CHANGE)
	{
		pSequenceEditActionBase = new StartBridgeNodeChangeAction(m_pPartLayoutData, m_pSequenceData, pConfigData, m_pViewPort);
		m_pAction.reset(pSequenceEditActionBase);
	}
	else if (actionType == SEQUENCE_ADD_CHAINSEQ)
	{
		pSequenceEditActionBase = new ChainCutSequenceAddAction(m_pPartLayoutData, m_pSequenceData, pConfigData, m_pViewPort);
		m_pAction.reset(pSequenceEditActionBase);
	}
	else if (actionType == SEQUENCE_INSERT_CHAINNODE)
	{
		pSequenceEditActionBase = new ChainNodeInsertAction(m_pPartLayoutData, m_pSequenceData, pConfigData, m_pViewPort, m_pInsertPositionActionBar);
		m_pAction.reset(pSequenceEditActionBase);
	}
    else if (actionType == SEQUENCE_ADD_RESET_INSTRUCT)
    {
        pSequenceEditActionBase = new ResetInstructionAddAction(m_pPartLayoutData, m_pSequenceData, pConfigData, m_pViewPort);
        m_pAction.reset(pSequenceEditActionBase);
    }
    else if (actionType == SEQUENCE_DEL_RESET_INSTRUCT)
    {
        pSequenceEditActionBase = new ResetInstructionDeleteAction(m_pPartLayoutData, m_pSequenceData, pConfigData, m_pViewPort);
        m_pAction.reset(pSequenceEditActionBase);
    }
	else if (actionType == SEQUENCE_DEL_CHAINNODE)
	{
		pSequenceEditActionBase = new ChainNodeDeleteAction(m_pPartLayoutData, m_pSequenceData, pConfigData, m_pViewPort);
		m_pAction.reset(pSequenceEditActionBase);
	}
	else if (actionType == SEQUENCE_ADD_GRIDSEQ)
	{
		pSequenceEditActionBase = new GridCutSequenceAddAction(m_pPartLayoutData, m_pSequenceData, pConfigData, m_pViewPort);
		m_pAction.reset(pSequenceEditActionBase);
	}
	else if (actionType == SEQUENCE_Add_FLYCUT)
	{
		pSequenceEditActionBase = new FlyCutSequenceAddAction(m_pPartLayoutData, m_pSequenceData, pConfigData, m_pViewPort);
		m_pAction.reset(pSequenceEditActionBase);
	}
	else if (actionType == SEQUENCE_ADD_MOV_PT)
	{
		pSequenceEditActionBase = new MovePtAddAction(m_pPartLayoutData, m_pSequenceData, pConfigData, m_pViewPort);
		m_pAction.reset(pSequenceEditActionBase);
	}
	else if (actionType == SEQUENCE_DEL_MOV_PT)
	{
		pSequenceEditActionBase = new MovePtDeleteAction(m_pPartLayoutData, m_pSequenceData, pConfigData, m_pViewPort);
		m_pAction.reset(pSequenceEditActionBase);
	}
	else if (actionType == SEQUENCE_AUTO_REM_CUTLINE)
	{
		pSequenceEditActionBase = new RemnantCutlineAutoAction(m_pPartLayoutData, m_pSequenceData, pConfigData, m_pViewPort, m_pGenerateRemCutLineActionBar);
		m_pAction.reset(pSequenceEditActionBase);
	}
	else if (actionType == SEQUENCE_ADD_REM_CUTLINE)
	{
		pSequenceEditActionBase = new RemnantCutlineAddAction(m_pPartLayoutData, m_pSequenceData, pConfigData, m_pViewPort);
		m_pAction.reset(pSequenceEditActionBase);
	}
	else if (actionType == SEQUENCE_DEL_REM_CUTLINE)
	{
		pSequenceEditActionBase = new RemnantCutlineDeleteAction(m_pPartLayoutData, m_pSequenceData, pConfigData, m_pViewPort);
		m_pAction.reset(pSequenceEditActionBase);
	}
	else if (actionType == SEQUENCE_INSERT_LOOPSEQ)
	{
		pSequenceEditActionBase = new LoopSequenceInsertAction(m_pPartLayoutData, m_pSequenceData, pConfigData, m_pViewPort);
		m_pAction.reset(pSequenceEditActionBase);
	}
	else if (actionType == SEQUENCE_REPLACE_HOLE_SEQ)
	{
		pSequenceEditActionBase = new HoleSequenceReplaceAction(m_pPartLayoutData, m_pSequenceData, pConfigData, m_pViewPort);
		m_pAction.reset(pSequenceEditActionBase);
	}
	else if (actionType == SEQUENCE_REMOVE)
	{
		// 看看当前有没有选中工序。
		CutSequenceListPtr pSelSeqUnitList;
		if (m_pAction->GetActionType() == SEQUENCE_SELECT) {
			CutSequenceSelectActionPtr pSeqUnitSelectAction = boost::dynamic_pointer_cast<CutSequenceSelectAction>(m_pAction);
			pSelSeqUnitList = pSeqUnitSelectAction->GetSelSeqUnits();
		}

		// 切换到工序删除动作。
		CutSequenceDeleteAction* pSequenceDelAction = new CutSequenceDeleteAction(m_pPartLayoutData, m_pSequenceData, pConfigData, m_pViewPort, m_pApplyPartActionBar);
		pSequenceEditActionBase = pSequenceDelAction;
		m_pAction.reset(pSequenceEditActionBase);

		// 要是有选中的工序，删除。
		if (pSelSeqUnitList && pSelSeqUnitList->size() > 0) {
			pSequenceDelAction->SelectSeqUnits(pSelSeqUnitList);
        }
	}
	else if (actionType == SEQUENCE_VIEW)
	{
		// if still running, we do nothing.
		if (!CutNodeSimulator::GetInstance()->IsFinished()) {
			CutNodeSimulator::GetInstance()->StopSimulation();
        }

		// then, check whether there are sequence unit exist.
		if (m_pSequenceData->GetCutSequences()->size() == 0) {
			CString strMsg = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_NO_SEQ_SIM);
			MessageBox(m_pViewPort->GetWnd(), strMsg, MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME), MB_OK | MB_ICONWARNING);
			if (m_pMatSize->GetType() == MAT_SIZE_Canvas) {
				CanvasGeometryEditAction::PostMessage2EnterCanvasLoopSelectAction();
			}
			else {
				PartPlacementEditAction::PostMessage2EnterPartPlacementSelectAction();
			}
			return FALSE;
		}

		// create the SequenceViewAction.
		SequenceViewAction* pSequenceViewAction = new SequenceViewAction(m_pPartLayoutData, m_pSequenceData, pConfigData, m_pViewPort);
		pSequenceViewAction->Start();

		pSequenceEditActionBase = pSequenceViewAction;
		m_pAction.reset(pSequenceEditActionBase);
	}

	return TRUE;
}

void SequenceEditor::HideAllActionBars()
{
	if (m_pApplyPartActionBar->IsWindowVisible()) {
		m_pApplyPartActionBar->ShowWindow(SW_HIDE);
	}

	if (m_pInsertPositionActionBar->IsWindowVisible()) {
		m_pInsertPositionActionBar->ShowWindow(SW_HIDE);
	}

	if (m_pGenerateRemCutLineActionBar->IsWindowVisible()) {
		m_pGenerateRemCutLineActionBar->ShowWindow(SW_HIDE);
	}
}

void SequenceEditor::HideAllPanels()
{
	CutSequencePane::GetInstance()->ShowBar(FALSE);
}

END_CUTLEADER_NAMESPACE()

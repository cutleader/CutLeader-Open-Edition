#include "StdAfx.h"
#include "PartLayoutEditor.h"

#include "GlViewPort.h"
#include "ZoomInAction.h"
#include "ZoomOutAction.h"
#include "DummyAction.h"
#include "clBaseDataResource.h"
#include "DataBaseManager.h"
#include "LogMgr.h"
#include "ClFolderHelper.h"
#include "ClWindowFinder.h"

#include "MachineItemLoader.h"
#include "ToolAssignConfigItem.h"
#include "LeadConfigItem.h"
#include "PierceConfigItem.h"
#include "MicroJointConfigItem.h"
#include "CornerConfigItem.h"
#include "ParamConfig.h"
#include "ExpertLibItemLoader.h"
#include "ClOptionInfo.h"
#include "SheetEditParam.h"
#include "ConfigItemLoader.h"

#include "MicroJointList.h"
#include "clCutFeatureResource.h"
#include "clPartLayoutResource.h"
#include "PartPlacementPropertyBar.h"
#include "LoopInstancePropertyBar.h"
#include "AddPartPlacementFromDatacenterAction.h"
#include "AddPartPlacementFromDxfdwgAction.h"
#include "PartPlacementList.h"
#include "PartPlacementSelectAction.h"
#include "PartInstanceSelectAction.h"
#include "PartPlacementCopyAction.h"
#include "PartPlacementRotateAction.h"
#include "PartPlacementMoveAction.h"
#include "PartPlacementGridAction.h"
#include "PartPlacementExplodeAction.h"
#include "PartPlacementDeleteAction.h"
#include "LoopToolEditAction2.h"
#include "CutDirectionAdjustAction.h"
#include "MicroJointAddAction.h"
#include "LoopLeadEditAction2.h"
#include "StartCutPtAdjustAction.h"
#include "MicroJointEditAction2.h"
#include "MicroJointDeleteAction2.h"
#include "CornerAddAction2.h"
#include "CornerEditAction2.h"
#include "CornerDeleteAction2.h"
#include "DistanceMeasureAction.h"
#include "PartLayoutHelper.h"
#include "PartGroupList.h"
#include "PartPlacementManager.h"
#include "LoopInstanceSelectAction.h"
#include "LoopInstanceList.h"
#include "LoopInstance.h"
#include "PartPlacement.h"
#include "PartInstance.h"
#include "PartInstanceList.h"
#include "LoopInstanceManager.h"
#include "LoopToolEditAction3.h"
#include "PartPlacementViewBar.h"
#include "CornerPropertyBar.h"
#include "PartPlacementAddPanel.h"
#include "MicroJointPropertyBar.h"
#include "LoopToolEditPanel.h"
#include "LoopLeadPropertyBar.h"
#include "CutSideAdjustAction.h"
#include "LoopInstancePropertyBar.h"

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

PartLayoutEditor::PartLayoutEditor(GlViewPortPtr pViewPort)
{
	m_pViewPort = pViewPort;
	m_pAction.reset(new DummyAction(m_pViewPort));

	{
		m_pCreatePartGridActionBar = new CreatePartGridActionBar();
		m_pCreatePartGridActionBar->Create(CreatePartGridActionBar::IDD);
		m_pCreatePartGridActionBar->ShowWindow(SW_HIDE);

		m_pSelectPartOrLoopActionBar = new SelectPartOrLoopActionBar();
		m_pSelectPartOrLoopActionBar->Create(SelectPartOrLoopActionBar::IDD);
		m_pSelectPartOrLoopActionBar->ShowWindow(SW_HIDE);

		m_pPartDistanceDetectActionBar = new PartDistanceDetectActionBar();
		m_pPartDistanceDetectActionBar->Create(PartDistanceDetectActionBar::IDD);
		m_pPartDistanceDetectActionBar->ShowWindow(SW_HIDE);

		m_pSpreadSamePartActionBar = new SpreadSamePartActionBar();
		m_pSpreadSamePartActionBar->Create(SpreadSamePartActionBar::IDD);
		m_pSpreadSamePartActionBar->ShowWindow(SW_HIDE);

		m_pChangeLoopToolActionBar = new ChangeLoopToolActionBar();
		m_pChangeLoopToolActionBar->Create(ChangeLoopToolActionBar::IDD);
		m_pChangeLoopToolActionBar->ShowWindow(SW_HIDE);
	}
}

PartLayoutEditor::~PartLayoutEditor(void)
{
    // 这里先销毁action，因为有的action可能会监听m_pSpreadSamePartActionBar。
    m_pAction.reset();
    if (m_pSpreadSamePartActionBar) {
        delete m_pSpreadSamePartActionBar;
        m_pSpreadSamePartActionBar = NULL;
    }
	if (m_pChangeLoopToolActionBar) {
		delete m_pChangeLoopToolActionBar;
		m_pChangeLoopToolActionBar = NULL;
	}
	if (m_pCreatePartGridActionBar) {
		delete m_pCreatePartGridActionBar;
		m_pCreatePartGridActionBar = NULL;
	}
}

BOOL PartLayoutEditor::LButtonDown(double& dPtX, double& dPtY)
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

BOOL PartLayoutEditor::LButtonUp(double& dPtX, double& dPtY)
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

BOOL PartLayoutEditor::LButtonDBClk(double& dPtX, double& dPtY)
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

BOOL PartLayoutEditor::RButtonUp(double& dPtX, double& dPtY)
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

BOOL PartLayoutEditor::MovePoint(double& dPtX, double& dPtY)
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

BOOL PartLayoutEditor::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
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

BOOL PartLayoutEditor::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (m_pAction)
	{
		m_pAction->OnKeyUp(nChar, nRepCnt, nFlags);
	}

	return TRUE;
}

CString PartLayoutEditor::GetEditorName() const
{
	CString str;
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PART_LAYOUT);
	return str;
}

PartPlacementListPtr PartLayoutEditor::GetSelectedPartPlacements() const
{
    PartPlacementListPtr pSelectedPartPlacements(new PartPlacementList);

    if (const PartPlacementSelectAction* pPartPlacementSelectAction = dynamic_cast<const PartPlacementSelectAction*>(m_pAction.get()))
    {
        pSelectedPartPlacements = pPartPlacementSelectAction->GetSelectedPrtPmts();
    }
    else if (const LoopInstanceSelectAction* pLoopInstanceSelectAction = dynamic_cast<const LoopInstanceSelectAction*>(m_pAction.get()))
    {
        LoopInstanceListPtr pSelectedLoopInstances = pLoopInstanceSelectAction->GetSelectedLoopInstances();
        for (unsigned int i = 0; i < pSelectedLoopInstances->size(); i++)
        {
            LoopInstancePtr pLoopInstance = pSelectedLoopInstances->operator [](i);
            PartPlacementPtr pPartPlacement = pLoopInstance->GetPartInstance()->GetPartPlacement();
            if (!pSelectedPartPlacements->GetPartPlacementByID(pPartPlacement->GetID()))
            {
                pSelectedPartPlacements->push_back(pPartPlacement);
            }
        }
    }

    return pSelectedPartPlacements;
}

LoopInstanceListPtr PartLayoutEditor::GetSelectedLoopInstances() const
{
    LoopInstanceListPtr pSelectedLoopInstances(new LoopInstanceList);

    if (const PartPlacementSelectAction* pPartPlacementSelectAction = dynamic_cast<const PartPlacementSelectAction*>(m_pAction.get()))
    {
        PartPlacementListPtr pSelectedPartPlacements = pPartPlacementSelectAction->GetSelectedPrtPmts();
        for (unsigned int i = 0; i < pSelectedPartPlacements->size(); i++)
        {
            PartPlacementPtr pPartPlacement = pSelectedPartPlacements->operator [](i);
            PartInstanceListPtr pPartInstances = m_pPartInstanceList->GetPartInstanceByPartPlacement(pPartPlacement.get());
            LoopInstanceListPtr pLoopInstances = LoopInstanceManager::GetLoopInstanceList(pPartInstances);
            pSelectedLoopInstances->insert(pSelectedLoopInstances->end(), pLoopInstances->begin(), pLoopInstances->end());
        }
    }
    else if (const LoopInstanceSelectAction* pLoopInstanceSelectAction = dynamic_cast<const LoopInstanceSelectAction*>(m_pAction.get()))
    {
        pSelectedLoopInstances = pLoopInstanceSelectAction->GetSelectedLoopInstances();
    }

    return pSelectedLoopInstances;
}

void PartLayoutEditor::StartPartPlacementSelectAction_andPreSelect(vector<LONGLONG>* pPartPlacementIDs_preSelect)
{
	if (pPartPlacementIDs_preSelect == 0) {
		LogMgr::DebugWarn(_T("246845"));
		return;
	}

	// 得到预选择的零件放置。
	PartPlacementListPtr pPartPlacements_preSelect(new PartPlacementList);
	for (unsigned int i = 0; i < pPartPlacementIDs_preSelect->size(); i++) {
		LONGLONG iPartPlacementID = pPartPlacementIDs_preSelect->operator [](i);
		PartPlacementPtr pPartPlacement = m_pPartPlacementList->GetPartPlacementByID(iPartPlacementID);
		pPartPlacements_preSelect->push_back(pPartPlacement);
	}
	if (pPartPlacements_preSelect->size() == 0) {
		LogMgr::DebugWarn(_T("789642"));
		return;
	}
	delete pPartPlacementIDs_preSelect;

	if (m_pAction->GetActionType() == EDIT_SELECTPRTPMT) { // 当前就是零件放置选择action，不需要隐藏所有action bar和面板。
		;
	}
	else { // 先隐藏所有action bar和面板。
		HideAllActionBars();
		HideAllPanels();
	}

	PartPlacementSelectActionPtr pPartPlacementSelectAction(new PartPlacementSelectAction(m_pMatSize, m_pPartPlacementList, m_pPartInstanceList, m_pPartGroups,
		m_pViewPort, m_pPartDistanceDetectActionBar, m_pSelectPartOrLoopActionBar));
	pPartPlacementSelectAction->SelectPartPlacements(pPartPlacements_preSelect);
	m_pAction = pPartPlacementSelectAction;
}

void PartLayoutEditor::StartLoopInstanceSelectAction_andPreSelect(LoopInstanceListPtr pLoopInstances_preSelect)
{
	if (m_pAction->GetActionType() == EDIT_SelectLoopInstance) { // 当前就是轮廓实例选择action，不需要隐藏所有action bar和面板。
		;
	}
	else { // 先隐藏所有action bar和面板。
		HideAllActionBars();
		HideAllPanels();
	}

	LoopInstanceSelectActionPtr pLoopInstanceSelectAction(new LoopInstanceSelectAction(m_pMatSize, m_pPartPlacementList, m_pPartInstanceList, m_pPartGroups,
		m_pViewPort, m_pSelectPartOrLoopActionBar));
	pLoopInstanceSelectAction->SelectLoopInstances(pLoopInstances_preSelect);
	m_pAction = pLoopInstanceSelectAction;
}

void PartLayoutEditor::StartLoopInstanceSelectAction_andPreSelect(vector<pair<LONGLONG, LONGLONG>>* pIdPairs)
{
	if (pIdPairs == 0) {
		LogMgr::DebugWarn(_T("889932"));
		return;
	}

	// 得到预选择的轮廓实例。
	LoopInstanceListPtr pLoopInstances_preSelect(new LoopInstanceList);
	LoopInstanceListPtr pAllLoopInstances = LoopInstanceManager::GetLoopInstanceList(m_pPartInstanceList);
	for (unsigned int i = 0; i < pIdPairs->size(); i++) {
		const pair<LONGLONG, LONGLONG>& idPair = pIdPairs->operator [](i);
		LoopInstancePtr pLoopInstance = pAllLoopInstances->GetLoopInstance_ByPartInstanceAndPatternLoop(idPair.first, idPair.second);
		pLoopInstances_preSelect->push_back(pLoopInstance);
	}
	if (pLoopInstances_preSelect->size() == 0) {
		LogMgr::DebugWarn(_T("789642"));
		return;
	}
	delete pIdPairs;

	if (m_pAction->GetActionType() == EDIT_SelectLoopInstance) { // 当前就是轮廓实例选择action，不需要隐藏所有action bar和面板。
		;
	}
	else { // 先隐藏所有action bar和面板。
		HideAllActionBars();
		HideAllPanels();
	}

	LoopInstanceSelectActionPtr pLoopInstanceSelectAction(new LoopInstanceSelectAction(m_pMatSize, m_pPartPlacementList, m_pPartInstanceList, m_pPartGroups,
		m_pViewPort, m_pSelectPartOrLoopActionBar));
	pLoopInstanceSelectAction->SelectLoopInstances(pLoopInstances_preSelect);
	m_pAction = pLoopInstanceSelectAction;
}

void PartLayoutEditor::ChangeToolOfSelectedLoopInstances(int iToolIndex)
{
	// 先隐藏所有action bar和面板。
	HideAllActionBars();
	HideAllPanels();

    // 准备加工工艺数据
    LONGLONG iExpertLibItemID = m_pParamConfig->GetParentID();
    DBConnectPtr pMachineLibConnect = DataBaseManager::GetInstance()->GetMacLibDBConnect();
    DBConnectPtr pExpertLibConnect = DataBaseManager::GetInstance()->GetExpLibDBConnect();
    LONGLONG iMacItemID = ExpertLibItemLoader::GetMacIDByExpLibItem(pExpertLibConnect, iExpertLibItemID);
    ConditionTypeListPtr pConditionTypeList = MachineItemLoader::LoadMacConds(pMachineLibConnect, iMacItemID);
    ToolAssignConfigItemPtr pToolAssignConfigItem = ConfigItemLoader::LoadToolAssignConfigItem(pExpertLibConnect, m_pParamConfig->GetID(), pConditionTypeList);
    LeadConfigItemPtr pLeadConfigItem = boost::dynamic_pointer_cast<LeadConfigItem>(m_pParamConfig->GetItemByType(PARAMCONFIG_ITEM_LEAD_ASSIGN));

    // 启动动作
    LoopToolEditAction3* pLoopToolEditAction3 = new LoopToolEditAction3(m_pMatSize, m_pPartPlacementList, m_pPartInstanceList, m_pPartGroups, iToolIndex,
        m_pViewPort, pToolAssignConfigItem, pLeadConfigItem, pConditionTypeList, m_pChangeLoopToolActionBar);
    LoopInstanceListPtr pSelectedLoopInstances = GetSelectedLoopInstances();
    if (pSelectedLoopInstances->size() > 0)
    {
        pLoopToolEditAction3->SetTool_toSelectedLoopInstances(pSelectedLoopInstances);
    }

    m_pAction.reset(pLoopToolEditAction3);
}

BOOL PartLayoutEditor::SetEditAction(EDIT_SHEET_ACTION_TYPE actionType)
{
	if (m_pAction->GetActionType() == actionType) { // 不需要隐藏所有action bar和面板。
		;
	}
	else { // 先隐藏所有action bar和所有面板。
		HideAllActionBars();
		HideAllPanels();
	}

	// load the condition type under the expert library item.
	LONGLONG iExpLibItemID = m_pParamConfig->GetParentID();
	DBConnectPtr pMacLibConnect = DataBaseManager::GetInstance()->GetMacLibDBConnect();
	DBConnectPtr pExpLibConnect = DataBaseManager::GetInstance()->GetExpLibDBConnect();
	LONGLONG iMacItemID = ExpertLibItemLoader::GetMacIDByExpLibItem(pExpLibConnect, iExpLibItemID);
	ConditionTypeListPtr pCondTypeList = MachineItemLoader::LoadMacConds(pMacLibConnect, iMacItemID);

	//
	ToolAssignConfigItemPtr pToolAssignConfigItem = boost::dynamic_pointer_cast<ToolAssignConfigItem>(m_pParamConfig->GetItemByType(PARAMCONFIG_ITEM_TOOL_ASSIGN));
	LeadConfigItemPtr pLeadConfigItem = boost::dynamic_pointer_cast<LeadConfigItem>(m_pParamConfig->GetItemByType(PARAMCONFIG_ITEM_LEAD_ASSIGN));
	PierceConfigItemPtr pPierceConfigItem = boost::dynamic_pointer_cast<PierceConfigItem>(m_pParamConfig->GetItemByType(PARAMCONFIG_ITEM_PIERCE_ASSIGN));
	CornerConfigItemPtr pCornerConfigItem = boost::dynamic_pointer_cast<CornerConfigItem>(m_pParamConfig->GetItemByType(PARAMCONFIG_ITEM_CORNER_ASSIGN));

	if (actionType == EDIT_GRIDPRTPMT)
	{
        PartPlacementListPtr pSelectedPartPlacements = GetSelectedPartPlacements();
        if (pSelectedPartPlacements->size() == 1 && !pSelectedPartPlacements->operator [](0)->IsGrid()) { // 选中了一个非阵列零件。
			;
        }
        else { // 此时传一个空的集合给action。
			pSelectedPartPlacements->clear();
        }
		m_pAction.reset(new PartPlacementGridAction(m_pMatSize, m_pPartPlacementList, m_pPartInstanceList, m_pPartGroups,
			pSelectedPartPlacements, m_pViewPort, m_pCreatePartGridActionBar));
	}
	else if (actionType == EDIT_EXPLODE_GRID)
	{
        PartPlacementListPtr pSelectedPartPlacements = GetSelectedPartPlacements();
        if (pSelectedPartPlacements->size() == 1 && pSelectedPartPlacements->operator [](0)->IsGrid()) { // 选中了一个阵列零件
            PartPlacementExplodeAction* pPartPlacementExplodeAction = new PartPlacementExplodeAction(m_pMatSize, m_pPartPlacementList, m_pPartInstanceList, m_pPartGroups, m_pViewPort);
            pPartPlacementExplodeAction->ExplodeGridPartPlacement(pSelectedPartPlacements->operator [](0));
            m_pAction.reset(pPartPlacementExplodeAction);
        }
        else {
            m_pAction.reset(new PartPlacementExplodeAction(m_pMatSize, m_pPartPlacementList, m_pPartInstanceList, m_pPartGroups, m_pViewPort));
        }
	}
    else if (actionType == EDIT_SelectPartOrLoopInstance)
    {
        if (m_pSelectPartOrLoopActionBar->GetIsSelectingLoopInstance()) {
            m_pAction.reset(new LoopInstanceSelectAction(m_pMatSize, m_pPartPlacementList, m_pPartInstanceList, m_pPartGroups, m_pViewPort, m_pSelectPartOrLoopActionBar));
        }
        else {
            m_pAction.reset(new PartPlacementSelectAction(m_pMatSize, m_pPartPlacementList, m_pPartInstanceList, m_pPartGroups, m_pViewPort, m_pPartDistanceDetectActionBar,
				m_pSelectPartOrLoopActionBar));
        }
    }
	else if (actionType == EDIT_SELECTPRTPMT)
    {
		m_pAction.reset(new PartPlacementSelectAction(m_pMatSize, m_pPartPlacementList, m_pPartInstanceList, m_pPartGroups, m_pViewPort,
			m_pPartDistanceDetectActionBar, m_pSelectPartOrLoopActionBar));
    }
    else if (actionType == EDIT_SelectLoopInstance)
    {
        m_pAction.reset(new LoopInstanceSelectAction(m_pMatSize, m_pPartPlacementList, m_pPartInstanceList, m_pPartGroups, m_pViewPort, m_pSelectPartOrLoopActionBar));
    }
	else if (actionType == EDIT_SELECTPRTINST)
    {
		m_pAction.reset(new PartInstanceSelectAction(m_pViewPort));
    }
	else if (actionType == EDIT_DELETEPARTPMT)
	{
        PartPlacementListPtr pSelectedPartPlacements = GetSelectedPartPlacements();
		PartPlacementDeleteActionPtr pPartPlacementDeleteAction(new PartPlacementDeleteAction(m_pMatSize, m_pPartPlacementList, m_pPartInstanceList, m_pPartGroups,
			pSelectedPartPlacements, m_pViewPort));
		if (pSelectedPartPlacements->size() > 0) { // 删除选中的零件放置，之后不需要进入删除action。
			pPartPlacementDeleteAction->DeleteSelectedPartPlacements();
		}
		else {
			m_pAction = pPartPlacementDeleteAction;
		}
	}
	else if(actionType == EDIT_ROTATEPARTPMT)
	{
        PartPlacementListPtr pSelectedPartPlacements = GetSelectedPartPlacements();
		m_pAction.reset(new PartPlacementRotateAction(m_pMatSize, m_pPartPlacementList, m_pPartInstanceList, m_pPartGroups, pSelectedPartPlacements, m_pViewPort));
	}
	else if(actionType == EDIT_MOVEPRTPMT)
	{
        PartPlacementListPtr pSelectedPartPlacements = GetSelectedPartPlacements();
		Point2D referencePt;
		if (pSelectedPartPlacements->size() > 0)
			referencePt = pSelectedPartPlacements->GetRectBox().GetCenterPt();
		m_pAction.reset(new PartPlacementMoveAction(m_pMatSize, m_pPartPlacementList, m_pPartInstanceList, m_pPartGroups, pSelectedPartPlacements, referencePt,
			m_pViewPort, m_pPartDistanceDetectActionBar));
	}
	else if(actionType == EDIT_COPYPRTPMT)
	{
        PartPlacementListPtr pSelectedPartPlacements = GetSelectedPartPlacements();
		Point2D referencePt;
		if (pSelectedPartPlacements->size() > 0)
			referencePt = pSelectedPartPlacements->GetRectBox().GetCenterPt();
		m_pAction.reset(new PartPlacementCopyAction(m_pMatSize, m_pPartPlacementList, m_pPartInstanceList, m_pPartGroups, pSelectedPartPlacements, referencePt,
			m_pViewPort, m_pPartDistanceDetectActionBar));
	}
	else if (actionType == LOOPTOOL_ADJUST)
	{
		LoopToolEditAction2* pLoopToolEditAction = new LoopToolEditAction2(m_pMatSize, m_pPartPlacementList, m_pPartInstanceList, m_pPartGroups,
			m_pViewPort, pToolAssignConfigItem, pLeadConfigItem, pCondTypeList, m_pSpreadSamePartActionBar);
        LoopInstanceListPtr pSelectedLoopInstances = GetSelectedLoopInstances();
        pLoopToolEditAction->EditToolsOfSelectedLoopInstances(pSelectedLoopInstances);
		m_pAction.reset(pLoopToolEditAction);
	}
	else if (actionType == LEAD_ADJUST) // 应该调用其他函数来进入这个action。
	{
		LogMgr::DebugWarn(_T("735892"));
	}
	else if (actionType == START_PT_ADJUST) {
		m_pAction.reset(new StartCutPtAdjustAction(m_pMatSize, m_pPartPlacementList, m_pPartInstanceList, m_pPartGroups, m_pViewPort, pLeadConfigItem, m_pSpreadSamePartActionBar));
	}
	else if (actionType == CUTSIDE_ADJUST) { // 应该调用其他函数来进入这个action。
		LogMgr::DebugWarn(_T("409428"));
	}
	else if (actionType == DIRECTION_ADJUST) { // 应该调用其他函数来进入这个action。
		LogMgr::DebugWarn(_T("999044"));
	}
	else if (actionType == PMT_MJ_ADD) {
		m_pAction.reset(new MicroJointAddAction(m_pMatSize, m_pPartPlacementList, m_pPartInstanceList, m_pPartGroups, m_pViewPort, m_pParamConfig, m_pSpreadSamePartActionBar));
	}
	else if (actionType == PMT_MJ_EDIT)
	{
		m_pAction.reset(new MicroJointEditAction2(m_pMatSize, m_pPartPlacementList, m_pPartInstanceList, m_pPartGroups, m_pViewPort, m_pSpreadSamePartActionBar));
	}
	else if (actionType == PMT_MJ_DELETE)
	{
		// 看看当前有没有选中的微连接特征。
		MicroJointListPtr pMicroJointList;
		if (m_pAction->GetActionType() == MicroJoint_EDIT)
		{
			MicroJointEditAction2* pMJEditAction = dynamic_cast<MicroJointEditAction2*>(m_pAction.get());
			pMicroJointList = pMJEditAction->GetSelectMJList();
		}

		// 切换到微连接删除动作。
		MicroJointDeleteAction2* pMJDelAction = new MicroJointDeleteAction2(m_pMatSize, m_pPartPlacementList, m_pPartInstanceList, m_pPartGroups, m_pViewPort, m_pSpreadSamePartActionBar);
		m_pAction.reset(pMJDelAction);

		// 要是有选中的微连接特征，删除。
		if (pMicroJointList && pMicroJointList->size() > 0)
		{
			MicroJointListPtr pDelMJList(new MicroJointList);
			pDelMJList->insert(pDelMJList->end(), pMicroJointList->begin(), pMicroJointList->end());
			pMJDelAction->SelectMJList(pDelMJList);
		}
	}
	else if (actionType == PMT_CORNER_ADD)
	{
		m_pAction.reset(new CornerAddAction2(m_pMatSize, m_pPartPlacementList, m_pPartInstanceList, m_pPartGroups, m_pViewPort, m_pSpreadSamePartActionBar));
	}
	else if (actionType == PMT_CORNER_EDIT)
	{
		m_pAction.reset(new CornerEditAction2(m_pMatSize, m_pPartPlacementList, m_pPartInstanceList, m_pPartGroups, m_pViewPort, m_pSpreadSamePartActionBar, pCornerConfigItem));
	}
	else if (actionType == PMT_CORNER_DELETE)
	{
		m_pAction.reset(new CornerDeleteAction2(m_pMatSize, m_pPartPlacementList, m_pPartInstanceList, m_pPartGroups, m_pViewPort, m_pSpreadSamePartActionBar));
	}
	else if (actionType == MEASURE_PT_DISTANCE)
	{
		m_pAction.reset(new DistanceMeasureAction(m_pMatSize, m_pPartPlacementList, m_pPartInstanceList, m_pPartGroups, m_pViewPort));
	}
    else if (actionType == EDIT_CreatePartGroup)
    {
        PartPlacementListPtr pSelectedPartPlacements = GetSelectedPartPlacements();
        if (pSelectedPartPlacements->size() > 0)
        {
            // 从零件放置集合中过滤出零件组
            PartGroupListPtr pPartGroups_return(new PartGroupList);
            PartPlacementListPtr pPartPlacements_return(new PartPlacementList);
            PartPlacementManager::FilterOutPartGroups_fromSelectedPartPlacements(pSelectedPartPlacements.get(), m_pPartGroups.get(), pPartGroups_return, pPartPlacements_return);

            // 执行命令，创建零件组。
            CWnd* pView = CWnd::FromHandle(m_pViewPort->GetWnd());
            PartLayoutHelper::ExecuteCommand2CreatePartGroup(pView, m_pPartGroups, pPartGroups_return, pPartPlacements_return);

            // 进入零件选择action。
            PartPlacementSelectActionPtr pPartPlacementSelectAction(new PartPlacementSelectAction(m_pMatSize, m_pPartPlacementList, m_pPartInstanceList, m_pPartGroups,
                m_pViewPort, m_pPartDistanceDetectActionBar, m_pSelectPartOrLoopActionBar));
            pPartPlacementSelectAction->SelectPartPlacements(pSelectedPartPlacements);
            m_pAction = pPartPlacementSelectAction;
        }
        else
        {
            CString strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);
            CString str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_PartGroup_1);
            MessageBox(m_pViewPort->GetWnd(), str, strProductName, MB_OK | MB_ICONWARNING);

            // 进入零件选择action。
            PartPlacementSelectActionPtr pPartPlacementSelectAction(new PartPlacementSelectAction(m_pMatSize, m_pPartPlacementList, m_pPartInstanceList, m_pPartGroups,
                m_pViewPort, m_pPartDistanceDetectActionBar, m_pSelectPartOrLoopActionBar));
            m_pAction = pPartPlacementSelectAction;
        }
    }
    else if (actionType == EDIT_ExplodePartGroup)
    {
        PartPlacementListPtr pSelectedPartPlacements = GetSelectedPartPlacements();

        // 从零件放置集合中过滤出零件组
        PartGroupListPtr pPartGroups_return(new PartGroupList);
        PartPlacementListPtr pPartPlacements_return(new PartPlacementList);
        PartPlacementManager::FilterOutPartGroups_fromSelectedPartPlacements(pSelectedPartPlacements.get(), m_pPartGroups.get(), pPartGroups_return, pPartPlacements_return);

        if (pPartPlacements_return->size() == 0 && pPartGroups_return->size() == 1)
        {
            // 执行命令，创建零件组。
            CWnd* pView = CWnd::FromHandle(m_pViewPort->GetWnd());
            PartLayoutHelper::ExecuteCommand2ExplodePartGroup(pView, m_pPartGroups, pPartGroups_return->operator [](0));

            // 进入零件选择action。
            PartPlacementSelectActionPtr pPartPlacementSelectAction(new PartPlacementSelectAction(m_pMatSize, m_pPartPlacementList, m_pPartInstanceList, m_pPartGroups,
                m_pViewPort, m_pPartDistanceDetectActionBar, m_pSelectPartOrLoopActionBar));
            pPartPlacementSelectAction->SelectPartPlacements(pSelectedPartPlacements);
            m_pAction = pPartPlacementSelectAction;
        }
        else
        {
            CString strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);
            CString str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_PartGroup_2);
            MessageBox(m_pViewPort->GetWnd(), str, strProductName, MB_OK | MB_ICONWARNING);

            // 进入零件选择action。
            PartPlacementSelectActionPtr pPartPlacementSelectAction(new PartPlacementSelectAction(m_pMatSize, m_pPartPlacementList, m_pPartInstanceList, m_pPartGroups,
                m_pViewPort, m_pPartDistanceDetectActionBar, m_pSelectPartOrLoopActionBar));
            m_pAction = pPartPlacementSelectAction;
        }
    }

	return TRUE;
}

void PartLayoutEditor::EditLoopLeadLine(LoopInstanceListPtr pSelectedLoopInstances)
{
	if (m_pAction->GetActionType() == LEAD_ADJUST) { // 不需要隐藏所有action bar和面板。
		;
	}
	else { // 先隐藏所有action bar和所有面板。
		HideAllActionBars();
		HideAllPanels();
	}

	LONGLONG iMacItemID = ExpertLibItemLoader::GetMacIDByExpLibItem(DataBaseManager::GetInstance()->GetExpLibDBConnect(), m_pParamConfig->GetParentID());
	ConditionTypeListPtr pCondTypeList = MachineItemLoader::LoadMacConds(DataBaseManager::GetInstance()->GetMacLibDBConnect(), iMacItemID);
	LeadConfigItemPtr pLeadConfigItem = boost::dynamic_pointer_cast<LeadConfigItem>(m_pParamConfig->GetItemByType(PARAMCONFIG_ITEM_LEAD_ASSIGN));

	LoopLeadEditAction2* pLoopLeadEditAction2 = new LoopLeadEditAction2(m_pMatSize, m_pPartPlacementList, m_pPartInstanceList, m_pPartGroups, m_pViewPort,
		pLeadConfigItem, pCondTypeList, m_pSpreadSamePartActionBar);
	m_pAction.reset(pLoopLeadEditAction2);
	if (pSelectedLoopInstances->size() > 0) {
		pLoopLeadEditAction2->EditLeadsOfSelectedLoopInstances(pSelectedLoopInstances);
		ClWindowFinder::GetCurrentView_1()->Invalidate();
	}
}

void PartLayoutEditor::ChangeLoopCutOffset_ByAction(PatternLoopCutOffsetType emCutOffsetType)
{
	if (m_pAction->GetActionType() == CUTSIDE_ADJUST) { // 不需要隐藏所有action bar和面板。
		;
	}
	else { // 先隐藏所有action bar和所有面板。
		HideAllActionBars();
		HideAllPanels();
	}

	LeadConfigItemPtr pLeadConfigItem = boost::dynamic_pointer_cast<LeadConfigItem>(m_pParamConfig->GetItemByType(PARAMCONFIG_ITEM_LEAD_ASSIGN));
	m_pAction.reset(new CutSideAdjustAction(m_pMatSize, m_pPartPlacementList, m_pPartInstanceList, m_pPartGroups, emCutOffsetType, m_pViewPort, pLeadConfigItem, m_pSpreadSamePartActionBar));
}

void PartLayoutEditor::ChangeLoopCutDirection_ByAction(CutDirectionChangeType emCutDirectionChangeType)
{
	if (m_pAction->GetActionType() == DIRECTION_ADJUST) { // 不需要隐藏所有action bar和面板。
		;
	}
	else { // 先隐藏所有action bar和所有面板。
		HideAllActionBars();
		HideAllPanels();
	}

	m_pAction.reset(new CutDirectionAdjustAction(m_pMatSize, m_pPartPlacementList, m_pPartInstanceList, m_pPartGroups, emCutDirectionChangeType, m_pViewPort, m_pSpreadSamePartActionBar));
}

BOOL PartLayoutEditor::SetViewAction(VIEW_ACTION_TYPE viewType)
{
	if (viewType == ACTION_ZOOM_IN)
		m_pAction.reset(new ZoomInAction(m_pViewPort));
	else if(viewType == ACTION_ZOOM_OUT)
		m_pAction.reset(new ZoomOutAction(m_pViewPort));

	return TRUE;
}

void PartLayoutEditor::AddPartPmtFromDataCenter(LONGLONG iPartID, LONGLONG iShtParamConfigID)
{
	if (m_pAction->GetActionType() == EDIT_ADDPARTPMT) { // 不需要隐藏所有action bar和面板。
		;
	}
	else { // 先隐藏所有action bar和面板。
		HideAllActionBars();
		HideAllPanels();
	}

	// 新建action。
	AddPartPlacementFromDatacenterAction* pPartPmtAddAction = new AddPartPlacementFromDatacenterAction(m_pMatSize, m_pPartPlacementList, m_pPartInstanceList,
		m_pPartGroups, m_pViewPort, iPartID, iShtParamConfigID, m_pPartDistanceDetectActionBar);
	m_pAction.reset(pPartPmtAddAction);

	// 设置焦点到当前视图。
	CWnd::FromHandle(m_pViewPort->GetWnd())->SetFocus();
}

void PartLayoutEditor::AddPartPmtFromDxfdwg(PartItemListPtr pPartItemList, const map<LONGLONG, vector<pair<LONGLONG, Matrix2D>>>& matrixData_of_sameShapePart, FolderNodePtr pSaveFolder)
{
	// 先隐藏所有action bar和面板。
	HideAllActionBars();
	HideAllPanels();

	// 新建action。
	AddPartPlacementFromDxfdwgAction* pPartPmtAddAction = new AddPartPlacementFromDxfdwgAction(m_pMatSize, m_pPartPlacementList, m_pPartInstanceList, m_pPartGroups,
		m_pViewPort, pPartItemList, matrixData_of_sameShapePart, pSaveFolder, m_pPartDistanceDetectActionBar);
	m_pAction.reset(pPartPmtAddAction);

	// 设置焦点到当前视图。
	CWnd::FromHandle(m_pViewPort->GetWnd())->SetFocus();
}

void PartLayoutEditor::HideAllActionBars()
{
	if (m_pPartDistanceDetectActionBar->IsWindowVisible())
		m_pPartDistanceDetectActionBar->ShowWindow(SW_HIDE);

	if (m_pSpreadSamePartActionBar->IsWindowVisible())
		m_pSpreadSamePartActionBar->ShowWindow(SW_HIDE);

	if (m_pCreatePartGridActionBar->IsWindowVisible())
		m_pCreatePartGridActionBar->ShowWindow(SW_HIDE);

	if (m_pSelectPartOrLoopActionBar->IsWindowVisible())
		m_pSelectPartOrLoopActionBar->ShowWindow(SW_HIDE);

	if (m_pChangeLoopToolActionBar->IsWindowVisible())
		m_pChangeLoopToolActionBar->ShowWindow(SW_HIDE);
}

void PartLayoutEditor::HideAllPanels()
{
	PartPlacementViewBar::GetInstance()->ShowBar(FALSE);
	PartPlacementPropertyBar::GetInstance()->ShowBar(FALSE);
	LoopInstancePropertyBar::GetInstance()->ShowBar(FALSE);
	//PartPlacementAddPanel::GetInstance()->ShowBar(FALSE); // 不隐藏这个面板，这个面板需要手动关闭。

	CornerPropertyBar::GetInstance()->ShowBar(FALSE);
	LoopToolEditPanel::GetInstance()->ShowBar(FALSE);
	LoopLeadPropertyBar::GetInstance()->ShowBar(FALSE);
	MicroJointPropertyBar::GetInstance()->ShowBar(FALSE);
}

END_CUTLEADER_NAMESPACE()

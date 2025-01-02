#include "StdAfx.h"
#include "PartLayoutHelper.h"

#include <LoopInstanceList.h>

#include "LogMgr.h"
#include "ICommand.h"
#include "CommandManager.h"
#include "LineArc2DList.h"

#include "CLOptionInfo.h"
#include "SheetEditParam.h"
#include "ParamConfig.h"
#include "LeadConfigItem.h"
#include "ToolAssignConfigItem.h"
#include "ToolInstanceList.h"

#include "PartPlacementList.h"
#include "PartPlacement.h"
#include "PartGroup.h"
#include "PartGroupList.h"
#include "CreatePartGroupCommand.h"
#include "clPartLayoutResource.h"
#include "DeletePartGroupCommand.h"
#include "LoopInstance.h"
#include "IPatternLoop.h"
#include "LoopProcessor.h"
#include "LoopInstanceCache.h"
#include "LoopStartCutProcessor.h"
#include "LoopCornerData.h"
#include "PatternList.h"
#include "PatternLoopNode.h"
#include "PatternLoopNodeList.h"
#include "Part.h"
#include "PartCadData.h"
#include "LoopCutFeature.h"
#include "PartCamData.h"
#include "LoopToolData.h"
#include "PatternLoopList.h"
#include "LoopInstCutSideChangeCommand.h"
#include "LoopToolDataList.h"
#include "LoopStartCutDataList.h"
#include "clCutFeatureResource.h"
#include "DisableToolOnLoopCommand.h"
#include "LoopToolChangeCommand_2.h"
#include "PartInstanceList.h"
#include "PartInstance.h"
#include "LoopStartCutData.h"
#include "CutDirectionsChangeCommand.h"

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

PartGridParam::PartGridParam()
{
	double dPartDis = CLOptionInfo::GetInstance()->GetSheetEditParam()->GetPartDis();

	m_dRowSpacing = dPartDis;
	m_dColumnSpacing = dPartDis;
	m_iRowNum = 2;
	m_iColumnNum = 2;
	m_emPartGridDir = PART_GRID_RIGHT_TOP;

	m_bUsePartDistance = false;
	m_dPartDistance = dPartDis;
	m_bCommonCut = false;
}

PartGridParam::~PartGridParam(void)
{
}

PartPlacementListPtr PartLayoutHelper::GetPartPlacements_byRectSelection(const Rect2D& rect, const PartPlacementList* pAllPartPlacements, const PartGroupList* pAllPartGroups,
                                                                         const std::map<LONGLONG, Rect2D>& m_rects_of_partPmt)
{
    PartPlacementListPtr pPartPlacements_return(new PartPlacementList);

    if (!pAllPartPlacements || !pAllPartGroups)
    {
        LogMgr::DebugWarn(_T("32012"));
        return pPartPlacements_return;
    }

    // 在框框中的所有零件放置。
    PartPlacementListPtr pPartPlacements_inRect;
    if (m_rects_of_partPmt.size() > 0)
    {
        pPartPlacements_inRect = pAllPartPlacements->GetPartPlacementInRect(rect, m_rects_of_partPmt);
    }
    else
    {
        pPartPlacements_inRect = pAllPartPlacements->GetPartPlacementInRect(rect);
    }

    // 搜集出那些在分组中和不在分组中的所有零件放置。
    PartGroupListPtr pPartGroupList_tmp(new PartGroupList); // 临时记录选中的分组，防止重复
    for (unsigned int i = 0; i < pPartPlacements_inRect->size(); i++)
    {
        PartPlacementPtr pPartPlacement_inRect = pPartPlacements_inRect->operator [](i);
        PartGroupPtr pPartGroup = pAllPartGroups->GetPartGroupIncludePartPlacement(pPartPlacement_inRect.get());
        if (pPartGroup)
        {
            PartGroupPtr pPartGroup_topLevel = pAllPartGroups->GetTopLevelPartGroup(pPartGroup);
            if (!pPartGroupList_tmp->GetPartGroupByID(pPartGroup_topLevel->GetID()))
            {
                pPartGroupList_tmp->push_back(pPartGroup_topLevel);
                PartPlacementListPtr pPartPlacements = PartGroup::GetAllPartPlacementsInThisGroup(pPartGroup_topLevel.get());
                pPartPlacements_return->insert(pPartPlacements_return->end(), pPartPlacements->begin(), pPartPlacements->end());
            }
        }
        else
        {
            pPartPlacements_return->push_back(pPartPlacement_inRect);
        }
    }

    return pPartPlacements_return;
}

PartPlacementListPtr PartLayoutHelper::GetPartPlacements_byClickSelection(const Point2D& pt, const PartPlacementList* pAllPartPlacements, const PartGroupList* pAllPartGroups,
                                                                          const std::map<LONGLONG, Rect2D>& m_rects_of_partPmt)
{
    PartPlacementListPtr pPartPlacements_return(new PartPlacementList);

    if (!pAllPartPlacements || !pAllPartGroups)
    {
        LogMgr::DebugWarn(_T("65742"));
        return pPartPlacements_return;
    }

    PartPlacementPtr pClosestPartPlacement;
    if (m_rects_of_partPmt.size() > 0)
    {
        pClosestPartPlacement = pAllPartPlacements->GetPartPlacementClosestTo(pt, m_rects_of_partPmt);
    }
    else
    {
        pClosestPartPlacement = pAllPartPlacements->GetPartPlacementClosestTo(pt);
    }

    if (pClosestPartPlacement)
    {
        PartGroupPtr pPartGroup = pAllPartGroups->GetPartGroupIncludePartPlacement(pClosestPartPlacement.get());
        if (pPartGroup) // 距离鼠标最近的零件放置属于一个零件组。
        {
            PartGroupPtr pPartGroup_topLevel = pAllPartGroups->GetTopLevelPartGroup(pPartGroup);
            PartPlacementListPtr pPartPlacementsInGroup = PartGroup::GetAllPartPlacementsInThisGroup(pPartGroup_topLevel.get());
            pPartPlacements_return->insert(pPartPlacements_return->end(), pPartPlacementsInGroup->begin(), pPartPlacementsInGroup->end());
        }
        else
        {
            pPartPlacements_return->push_back(pClosestPartPlacement);
        }
    }

    return pPartPlacements_return;
}

void PartLayoutHelper::ExecuteCommand2CreatePartGroup(CWnd* pView, PartGroupListPtr pAllPartGroups, PartGroupListPtr pSubPartGroups, PartPlacementListPtr pPartPlacements)
{
    if (!pAllPartGroups)
    {
        LogMgr::DebugWarn(_T("21795"));
        return;
    }

    CString str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PART_LAYOUT);
    PartGroupPtr pNewPartGroup(new PartGroup(pSubPartGroups, pPartPlacements));
    ICommandPtr pCommand(new CreatePartGroupCommand(EditorData(pView, str), pAllPartGroups, pNewPartGroup));
    pCommand->Do();
    CommandManager::GetInstance()->AddCommand(pCommand);
}

void PartLayoutHelper::ExecuteCommand2ExplodePartGroup(CWnd* pView, PartGroupListPtr pAllPartGroups, PartGroupPtr pPartGroup_willBeDeleted)
{
    if (!pAllPartGroups)
    {
        LogMgr::DebugWarn(_T("71394"));
        return;
    }

    CString str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PART_LAYOUT);
    ICommandPtr pCommand(new DeletePartGroupCommand(EditorData(pView, str), pAllPartGroups, pPartGroup_willBeDeleted));
    pCommand->Do();
    CommandManager::GetInstance()->AddCommand(pCommand);
}

bool PartLayoutHelper::CheckToolSize_4_LoopInstance(const LoopInstance* pLoopInstance, double dToolSize)
{
    if (fabs(dToolSize) < GEOM_TOLERANCE)
    {
        return true;
    }

    if (pLoopInstance->GetCacheData()->GetPatternLoop()->IsOpenPath())
    {
        return true;
    }

    // 克隆一份临时的几何轮廓用于测试偏移。
    IPatternLoopPtr pPatternLoop_copy;
    {
        // 克隆几何图形
        const IPatternLoop* pPatternLoop = pLoopInstance->GetCacheData()->GetPatternLoop().get();
        PatternListPtr pPatternList = pPatternLoop->GetPatternList();
        PatternListPtr pPatternList_copy = boost::dynamic_pointer_cast<PatternList>(pPatternList->Clone());

        // 克隆几何回路。
        pPatternLoop_copy = boost::dynamic_pointer_cast<IPatternLoop>(pPatternLoop->Clone());

        // 更新几何轮廓节点中引用的几何图形。
        PatternLoopNodeListPtr pNewLoopNodeList = pPatternLoop_copy->GetPatternLoopNodes();
        for (unsigned int j = 0; j < pNewLoopNodeList->size(); j++)
        {
            PatternLoopNodePtr pNewLoopNode = pNewLoopNodeList->operator [](j);
            const IPattern* pOldPat = pNewLoopNode->GetPattern().get();
            int iPatIdx = pPatternList->GetPatternIndex(pOldPat);
            pNewLoopNode->SetPattern(pPatternList_copy->operator [](iPatIdx));
        }

        // 更新缓存。
        LoopProcessor::UpdateCacheEx(pPatternLoop_copy);
    }

    // 偏移测试
    {
        PatternLoopCutOffsetType emLoopCutOffset = LoopStartCutProcessor::CalculateLoopCutOffset(pLoopInstance->GetCacheData()->GetLoopStartCutData().get());
        if (emLoopCutOffset == PatternLoopCutOffset_On)
        {
            return true;
        }
        else
        {
            BOOL bCutInside = TRUE;
            if (emLoopCutOffset == PatternLoopCutOffset_Outside)
            {
                bCutInside = FALSE;
            }
            vector<pair<LONGLONG, LONGLONG>> cornerPats = pLoopInstance->GetCacheData()->GetLoopCorner()->GetAllCornerPats();
            return LoopProcessor::OffsetPolyLoop(pPatternLoop_copy, bCutInside, dToolSize / 2.0, cornerPats);
        }
    }

    return true;
}

PartPlacementPtr PartLayoutHelper::CreatePartGrid(const PartPlacement* pNotGridPartPlacement, const PartGridParam* pPartGridParam)
{
	PartPlacementPtr pGridPartPlacement;

	if (pNotGridPartPlacement->IsGrid()) {
		LogMgr::DebugWarn(_T("368409"));
		return pGridPartPlacement;
	}

	if (pPartGridParam->GetIsUsePartDistance()) { // 按照零件间距来创建阵列。
		double dPartDistance = .0;
		if (pPartGridParam->GetIsCommonCut()) {
			const LoopTopologyItemList* pLoopTopologyItems = pNotGridPartPlacement->GetPart()->GetCadData()->GetLoopTopologyItems();
			LoopCutFeaturePtr pBoundaryLoopFeature = pNotGridPartPlacement->GetCamData()->GetBoundaryLoopFeature(pLoopTopologyItems);
			dPartDistance = pBoundaryLoopFeature->GetLoopTool()->GetToolWidth();
		}
		else {
			dPartDistance = pPartGridParam->GetPartDistance();
		}
		pGridPartPlacement = PartPlacementManager::BuildGridPartPmt(pNotGridPartPlacement, dPartDistance, pPartGridParam->GetRowNum(),
			pPartGridParam->GetColumnNum(), pPartGridParam->GetPartGridDir());
	}
	else // 按照行间距和列间距的方式来创建阵列。
	{
		// 零件放置的行偏移和列偏移。
		pair<double, double> offsetVal;
		{
			PatternLoopListPtr pPatternLoops = pNotGridPartPlacement->GetPart()->GetCadData()->GetPatternLoopList();
			LineArc2DListPtr pGeom2Ds = pPatternLoops->Calculate2DLineArcs()->Clone();
			pGeom2Ds->Transform(pNotGridPartPlacement->GetMatrix());
			Rect2D outerRect = pGeom2Ds->GetRectBox();
			offsetVal.first = outerRect.GetWidth() + pPartGridParam->GetColumnSpacing();
			offsetVal.second = outerRect.GetHeight() + pPartGridParam->GetRowSpacing();
		}

		pGridPartPlacement.reset(new PartPlacement(pNotGridPartPlacement->GetPart(), pNotGridPartPlacement->GetCamData(), pNotGridPartPlacement->GetMatrix()));
		pGridPartPlacement->IsGrid(TRUE);
		pGridPartPlacement->SetRowCount(pPartGridParam->GetRowNum());
		pGridPartPlacement->SetColCount(pPartGridParam->GetColumnNum());

		// 设置零件的阵列偏移。
		if (pPartGridParam->GetPartGridDir() == PART_GRID_RIGHT_TOP) {
			pGridPartPlacement->SetSpacingX(offsetVal.first);
			pGridPartPlacement->SetSpacingY(offsetVal.second);
		}
		else if (pPartGridParam->GetPartGridDir() == PART_GRID_RIGHT_BOTTOM) {
			pGridPartPlacement->SetSpacingX(offsetVal.first);
			pGridPartPlacement->SetSpacingY(-offsetVal.second);
		}
		else if (pPartGridParam->GetPartGridDir() == PART_GRID_LEFT_TOP) {
			pGridPartPlacement->SetSpacingX(-offsetVal.first);
			pGridPartPlacement->SetSpacingY(offsetVal.second);
		}
		else if (pPartGridParam->GetPartGridDir() == PART_GRID_LEFT_BOTTOM) {
			pGridPartPlacement->SetSpacingX(-offsetVal.first);
			pGridPartPlacement->SetSpacingY(-offsetVal.second);
		}
	}

	return pGridPartPlacement;
}

void PartLayoutHelper::ChangeLoopCutOffset(const CWnd* pView, const CString& strEditorName, LoopInstanceListPtr pLoopInstances,
	PatternLoopCutOffsetType emCutOffsetType, LeadConfigItemPtr pLeadConfigItem)
{
	if (pLoopInstances->size() == 0) {
		LogMgr::DebugWarn(_T("460972"));
		return;
	}

	LoopInstanceListPtr pLoopInstances_picked(new LoopInstanceList);
	for (unsigned int i = 0; i < pLoopInstances->size(); i++)
	{
		LoopInstancePtr pLoopInstance = pLoopInstances->operator[](i);
		if (pLoopInstance->GetCacheData()->GetPatternLoop()->IsOpenPath()) { // 开放轮廓没必要设置。
			continue;
		}

		const LoopStartCutData* pLoopStartCutData = pLoopInstance->GetCacheData()->GetLoopStartCutData().get();
		PatternLoopCutOffsetType emOldCutOffsetType = LoopStartCutProcessor::CalculateLoopCutOffset(pLoopStartCutData);
		if (emOldCutOffsetType == emCutOffsetType) { // 这种情况没必要进行设置。
			continue;
		}

		// 这里检查是不是选了零件阵列，对于零件阵列中的相同轮廓只需设置一个。
		bool bIgnore = false;
		for (unsigned int j = 0; j < pLoopInstances_picked->size(); j++) {
			const LoopStartCutData* pLoopStartCutData_tmp = pLoopInstances_picked->operator [](j)->GetCacheData()->GetLoopStartCutData().get();
			if (pLoopStartCutData->GetID() == pLoopStartCutData_tmp->GetID()) {
				bIgnore = true;
				break;
			}
		}
		if (bIgnore) {
			continue;
		}

		pLoopInstances_picked->push_back(pLoopInstance);
	}

	// 执行命令。
	if (pLoopInstances_picked->size() > 0) {
		ICommandPtr pCommand(new LoopInstCutSideChangeCommand(EditorData(pView, strEditorName), pLoopInstances_picked, emCutOffsetType, pLeadConfigItem));
		pCommand->Do();
		CommandManager::GetInstance()->AddCommand(pCommand);
	}
}

void PartLayoutHelper::ChangeLoopCutDirection(const CWnd* pView, const CString& strEditorName, LoopInstanceListPtr pLoopInstances, CutDirectionChangeType emCutDirectionChangeType)
{
	if (pLoopInstances->size() == 0) {
		LogMgr::DebugWarn(_T("909532"));
		return;
	}

	LoopStartCutDataListPtr pLoopStartCutDatas_WillChange(new LoopStartCutDataList);
	for (unsigned int i = 0; i < pLoopInstances->size(); i++)
	{
		const LoopInstance* pLoopInstance = pLoopInstances->operator[](i).get();
		LoopStartCutDataPtr pLoopStartCutData = pLoopInstance->GetCacheData()->GetLoopStartCutData();

		// 这里检查是不是选了零件阵列，对于零件阵列中的相同轮廓只需设置一个。
		if (pLoopStartCutDatas_WillChange->GetItemByID(pLoopStartCutData->GetID())) {
			continue;
		}

		if (pLoopInstance->GetCacheData()->GetPatternLoop()->IsOpenPath()) { // 这个轮廓是开放轮廓。
			if (emCutDirectionChangeType == CutDirectionChange_Switch) {
				pLoopStartCutDatas_WillChange->push_back(pLoopStartCutData);
			}
		}
		else { // 这个轮廓是封闭轮廓。
			DIRECTION emCutDirection = LoopStartCutProcessor::GetClosedLoopCutDirection(pLoopStartCutData.get());
			if (emCutDirectionChangeType == CutDirectionChange_Switch) {
				pLoopStartCutDatas_WillChange->push_back(pLoopStartCutData);
			}
			else if (emCutDirectionChangeType == CutDirectionChange_CCW && emCutDirection != CCW) {
				pLoopStartCutDatas_WillChange->push_back(pLoopStartCutData);
			}
			else if (emCutDirectionChangeType == CutDirectionChange_CW && emCutDirection != CW) {
				pLoopStartCutDatas_WillChange->push_back(pLoopStartCutData);
			}
		}
	}

	// 执行命令。
	if (pLoopStartCutDatas_WillChange->size() > 0) {
		ICommandPtr pCommand(new CutDirectionsChangeCommand(EditorData(pView, strEditorName), pLoopStartCutDatas_WillChange));
		pCommand->Do();
		CommandManager::GetInstance()->AddCommand(pCommand);
	}
}

vector<LoopTopologyItemListPtr> PartLayoutHelper::GetLoopTopologyItemDataOfPartInstances(const PartInstanceList* pPartInstances)
{
	vector<LoopTopologyItemListPtr> loopTopologyItemData;

	set<long long> partIDs;
	for (unsigned int i = 0; i < pPartInstances->size(); i++) {
		const PartInstance* pPartInstance = pPartInstances->operator[](i).get();
		if (partIDs.find(pPartInstance->GetPartPlacement()->GetPart()->GetID()) == partIDs.end()) {
			loopTopologyItemData.push_back(pPartInstance->GetLoopTopologyItems_FW());
			partIDs.insert(pPartInstance->GetPartPlacement()->GetPart()->GetID());
		}
	}

	return loopTopologyItemData;
}

void PartLayoutHelper::SetToolToLoopInstances(const CWnd* pView, const CString& strEditorName, LoopInstanceListPtr pLoopInstances, int iToolIndex,
	const vector<LoopTopologyItemListPtr>& loopTopologyItemData, const ToolAssignConfigItem* pToolAssignConfigItem, LeadConfigItemPtr pLeadConfigItem)
{
	if (pLoopInstances->size() == 0) {
		LogMgr::DebugWarn(_T("301865"), pView);
		return;
	}

	// get the loop tool data list which will change tool.
	LoopToolDataListPtr pLoopToolList_4_edit(new LoopToolDataList);
	LoopStartCutDataListPtr pLoopStaEndCutList_4_edit(new LoopStartCutDataList);
	for (unsigned int i = 0; i < pLoopInstances->size(); i++) {
		pLoopToolList_4_edit->push_back(pLoopInstances->at(i)->GetCacheData()->GetLoopTool());
		pLoopStaEndCutList_4_edit->push_back(pLoopInstances->at(i)->GetCacheData()->GetLoopStartCutData());
	}

	// run the command.
	ICommandPtr pCommand;
	EditorData editorData(pView, strEditorName);
	if (iToolIndex == -1) { // -1 means no-cutting selected loops
		// 发消息给板材视图，问问是不是还要把其他的轮廓设置为“不加工”。
		LoopToolDataListPtr pLoopToolDataList_other(new LoopToolDataList);
		pView->SendMessage(WM_QueryOtherLoopTools, (WPARAM)pLoopToolList_4_edit.get(), (LPARAM)pLoopToolDataList_other.get());
		LoopToolDataListPtr pLoopToolDataList(new LoopToolDataList);
		pLoopToolDataList->insert(pLoopToolDataList->end(), pLoopToolList_4_edit->begin(), pLoopToolList_4_edit->end());
		pLoopToolDataList->insert(pLoopToolDataList->end(), pLoopToolDataList_other->begin(), pLoopToolDataList_other->end());
		pCommand.reset(new DisableToolOnLoopCommand(editorData, pLoopToolDataList, true));
	}
	else if (iToolIndex == -2) { // -2 means cutting selected loops
		// 发消息给板材视图，问问是不是还要把其他的轮廓设置为“不加工”。
		LoopToolDataListPtr pLoopToolDataList_other(new LoopToolDataList);
		pView->SendMessage(WM_QueryOtherLoopTools, (WPARAM)pLoopToolList_4_edit.get(), (LPARAM)pLoopToolDataList_other.get());
		LoopToolDataListPtr pLoopToolDataList(new LoopToolDataList);
		pLoopToolDataList->insert(pLoopToolDataList->end(), pLoopToolList_4_edit->begin(), pLoopToolList_4_edit->end());
		pLoopToolDataList->insert(pLoopToolDataList->end(), pLoopToolDataList_other->begin(), pLoopToolDataList_other->end());
		pCommand.reset(new DisableToolOnLoopCommand(editorData, pLoopToolDataList, false));
	}
	else {
		ToolInstancePtr pToolInstance = pToolAssignConfigItem->GetToolInstanceList()->operator [](iToolIndex);
		if (!pToolInstance) {
			LogMgr::DebugWarn(_T("974352"), pView);
			return;
		}
		pCommand.reset(new LoopToolChangeCommand_2(editorData, loopTopologyItemData, pLoopToolList_4_edit, pToolInstance, pLoopStaEndCutList_4_edit, pLeadConfigItem));
	}
	pCommand->Do();
	CommandManager::GetInstance()->AddCommand(pCommand);
}

END_CUTLEADER_NAMESPACE()

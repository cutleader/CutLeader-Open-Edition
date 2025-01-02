#include "StdAfx.h"
#include "PartPlacementCopyAction.h"

#include "baseConst.h"
#include "Point2D.h"
#include "GlViewPort.h"
#include "CommandManager.h"
#include "ICommand.h"
#include "ValueInputManager.h"
#include "CursorDrawer.h"
#include "clUILibResource.h"
#include "ClWindowFinder.h"

#include "DisplayParam.h"
#include "SheetEditParam.h"
#include "CLOptionInfo.h"

#include "SnapPtDrawer.h"
#include "PatternDrawer.h"
#include "LoopStartCutDataList.h"
#include "LoopCutFeature.h"
#include "LoopCutFeatureList.h"
#include "LoopStartCutProcessor.h"
#include "PartCamData.h"
#include "PartItem.h"
#include "Part.h"
#include "PartPlacement.h"
#include "PartPlacementList.h"
#include "PartPlacementDrawer.h"
#include "PartPlacementCopyCommand.h"
#include "PartPlacementSelectAction.h"
#include "clPartLayoutResource.h"
#include "PartPlacementManager.h"
#include "PartLayoutHelper.h"
#include "PartGroupList.h"
#include "PartGroup.h"

BEGIN_CUTLEADER_NAMESPACE()

PartPlacementCopyAction::PartPlacementCopyAction(IMaterialSizePtr pMaterialSize, PartPlacementListPtr pPartPlacements, PartInstanceListPtr pPartInstances, PartGroupListPtr pPartGroups,
	PartPlacementListPtr pSelectedPartPlacements, const Point2D& referencePt, GlViewPortPtr pViewPort, PartDistanceDetectActionBar* pPartDistanceDetectActionBar)
    : PartPlacementPreSelectAction(pMaterialSize, pPartPlacements, pPartInstances, pPartGroups, pViewPort), PartPlacementMoveBase(m_pViewPort, pPartDistanceDetectActionBar)
{
	m_pSelectedPartPlacements = pSelectedPartPlacements;

	if (pSelectedPartPlacements->size() > 0)
	{
		m_referencePt = referencePt;

		// 为移动零件准备数据。
		PartPlacementListPtr pMovePartPlacements(new PartPlacementList);
		for (unsigned int i = 0; i < m_pSelectedPartPlacements->size(); i++)
			pMovePartPlacements->push_back(PartPlacementManager::ClonePartPmt(m_pSelectedPartPlacements->at(i).get()));
		GetPartDistanceDetectActionBar()->Show_PartDistanceDetectActionBar();
		UpdateBeforeMov(pMovePartPlacements, m_referencePt, pMaterialSize, m_pPartInstanceList);

		m_iMsgID = IDS_OUTPUT_COPYPARTPMT_NEWLOCATION;
	}
	else
		m_iMsgID = IDS_OUTPUT_SELECT_PRTPMT_COPY;
}

PartPlacementCopyAction::~PartPlacementCopyAction(void)
{
}

BOOL PartPlacementCopyAction::LButtonDown(double& dPtX, double& dPtY)
{
	if (m_pSelectedPartPlacements->size() == 0)
	{
		PartPlacementPreSelectAction::LButtonDown(dPtX, dPtY);
	}
	else
	{
		PartPlacementListPtr pMovePartPlacements = GetMovePartPlacements();

        // 克隆零件放置后，也克隆相关的零件组，不过只需克隆顶层零件组。
        PartGroupListPtr pPartGroupList_new = PartPlacementManager::ClonePartGroups_whenClonePartPlacements_2(m_pPartGroupList.get(),
            m_pSelectedPartPlacements.get(), pMovePartPlacements.get());

        // 所有要新建的零件分组
        PartGroupListPtr pPartGroups_willAdded(new PartGroupList);
        for (unsigned int i = 0; i < pPartGroupList_new->size(); i++)
        {
            PartGroupPtr pPartGroup = pPartGroupList_new->operator [](i);
            pPartGroups_willAdded->push_back(pPartGroup);
            PartGroupListPtr pPartGroups_tmp = pPartGroup->GetSubPartGroupsRecursively();
            pPartGroups_willAdded->insert(pPartGroups_willAdded->end(), pPartGroups_tmp->begin(), pPartGroups_tmp->end());
        }

		// 拷贝零件放置。
		ICommandPtr pCommand(new PartPlacementCopyCommand(GetEditorData(), m_pPartPlacementList, m_pPartInstanceList, m_pPartGroupList, pMovePartPlacements, pPartGroups_willAdded));
        pCommand->Do();
		CommandManager::GetInstance()->AddCommand(pCommand);

		// 更新任务
		{
			// 板材相关
			ClWindowFinder::GetCurrentView_1()->SendMessage(WM_updateSheetPreviewData, 0, 0);
			ClWindowFinder::GetCurrentView_2()->SendMessage(WM_redrawSheetThumbnailOnSheetManagementPanel, 0, 0);

			// 零件相关
			ClWindowFinder::GetCurrentView_2()->SendMessage(WM_ReDisplayNestedCountOfEachPart, 0, 0);
		}

        // 选中拷贝后的零件放置。
        PostMessage2EnterPartPlacementSelectAction(pMovePartPlacements.get());
	}

	return TRUE;
}

BOOL PartPlacementCopyAction::LButtonUp(double& dPtX, double& dPtY)
{
	if (m_pSelectedPartPlacements->size() == 0)
	{
		PartPlacementPreSelectAction::LButtonUp(dPtX, dPtY);

		if (m_pSelectedPartPlacements->size() > 0)
		{
			// 以零件放置的中心点为参考点。
			m_referencePt = m_pSelectedPartPlacements->GetRectBox().GetCenterPt();

			// 为移动零件准备数据。
			PartPlacementListPtr pMovePartPlacements(new PartPlacementList);
			for (unsigned int i = 0; i < m_pSelectedPartPlacements->size(); i++)
				pMovePartPlacements->push_back(PartPlacementManager::ClonePartPmt(m_pSelectedPartPlacements->at(i).get()));
			UpdateBeforeMov(pMovePartPlacements, m_referencePt, m_pMatSize, m_pPartInstanceList);

			m_iMsgID = IDS_OUTPUT_COPYPARTPMT_NEWLOCATION;
		}
	}

	return TRUE;
}

BOOL PartPlacementCopyAction::MovePoint(double& dPtX, double& dPtY)
{
	Point2D currentPt(dPtX, dPtY);

	if (m_pSelectedPartPlacements->size() == 0)
	{
		PartPlacementPreSelectAction::MovePoint(dPtX, dPtY);
	}
	else
	{
		// 移动的过程中更新一些数据。
		UpdateWhenMov(currentPt);
	}

	return TRUE;
}

BOOL PartPlacementCopyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	__super::OnKeyDown(nChar, nRepCnt, nFlags);
	if (m_pSelectedPartPlacements->size() > 0) {
		UpdateDataWhenRotate(nChar);
	}

	return TRUE;
}

BOOL PartPlacementCopyAction::DrawBeforeFinish()
{
	DisplayParamPtr pColorInfo = CLOptionInfo::GetInstance()->GetDisplayParam();

	if (m_pSelectedPartPlacements->size() == 0)
	{
		PartPlacementPreSelectAction::DrawBeforeFinish();
	}
	else
	{
		// 绘制选中的“零件放置”。
		PartPlacementDrawer partPlacementDrawer(m_pViewPort);
		partPlacementDrawer.DrawSelectedPartPlacements(m_pSelectedPartPlacements.get());

		// 绘制移动中的“零件放置”。
		const PartPlacementList* pMovePartPlacements = GetMovePartPlacements().get();
		partPlacementDrawer.DrawPartPlacements(pMovePartPlacements);

		// 绘制辅助信息。
		DrawAssistInfo(_T(""));

		// draw cursor.
		CursorDrawer::DrawCrossLineCursor(m_pViewPort);
	}

	return TRUE;
}

END_CUTLEADER_NAMESPACE()

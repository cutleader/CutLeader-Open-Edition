#include "StdAfx.h"
#include "AddPartPlacementFromDxfdwgAction.h"

#include "DateHelper.h"
#include "CommandManager.h"
#include "ICommand.h"
#include "ValueInputManager.h"
#include "GlViewPort.h"
#include "DummyAction.h"
#include "CursorDrawer.h"
#include "ClFolderHelper.h"
#include "DataBaseManager.h"
#include "ClWindowFinder.h"

#include "DisplayParam.h"
#include "SheetEditParam.h"
#include "CLOptionInfo.h"

#include "LoopStartCutProcessor.h"
#include "LoopCutFeatureList.h"
#include "LoopCutFeature.h"
#include "PartCadData.h"
#include "PartCamData.h"
#include "Part.h"
#include "PartItem.h"
#include "PartItemList.h"
#include "PartLoader.h"
#include "PartManager.h"
#include "clPartLayoutResource.h"
#include "PartLayoutEditor.h"
#include "PartPlacement.h"
#include "PartPlacementList.h"
#include "PartPlacementAddCommand.h"
#include "PartPlacementManager.h"
#include "PartPlacementDrawer.h"

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

AddPartPlacementFromDxfdwgAction::AddPartPlacementFromDxfdwgAction(IMaterialSizePtr pMaterialSize, PartPlacementListPtr pPartPlacements,
	PartInstanceListPtr pPartInstances, PartGroupListPtr pPartGroups, GlViewPortPtr pViewPort, PartItemListPtr pPartItemList,
	const map<LONGLONG, vector<pair<LONGLONG, Matrix2D>>>& matrixData_of_sameShapePart, FolderNodePtr pSaveFolder, PartDistanceDetectActionBar* pPartDistanceDetectActionBar)
    : PartPlacementEditAction(pMaterialSize, pPartPlacements, pPartInstances, pPartGroups, pViewPort), PartPlacementMoveBase(m_pViewPort, pPartDistanceDetectActionBar)
{
	m_pPartItemList = pPartItemList;
	m_bPartItemSaved = FALSE;
	m_pSaveFolder = pSaveFolder;
	m_iMsgID = IDS_OUTPUT_PARTPMT_BASEPT;

	// 逐个处理零件项。
	PartPlacementListPtr pMovePartPlacements(new PartPlacementList);
	for (unsigned int i = 0; i < m_pPartItemList->size(); i++)
	{
		PartItemPtr pPartItem = m_pPartItemList->at(i);
        PartPlacementPtr pPartPlacement = PartPlacementManager::BuildPartPmt(pPartItem.get(), Matrix2D(), true);
		pMovePartPlacements->push_back(pPartPlacement);

        // 看看这个零件有没有“相同零件”
		map<LONGLONG, vector<pair<LONGLONG, Matrix2D>>>::const_iterator iter = matrixData_of_sameShapePart.find(pPartItem->GetPart()->GetID());
		if (iter != matrixData_of_sameShapePart.end())
		{
			vector<pair<LONGLONG, Matrix2D>> matList = iter->second;
			for (unsigned int j = 0; j < matList.size(); j++)
			{
				Matrix2D mat = matList.at(j).second;
				PartPlacementPtr pPartPmt_samePart = PartPlacementManager::BuildPartPmt(pPartItem.get(), mat, true);
				pMovePartPlacements->push_back(pPartPmt_samePart);
			}
		}
	}

	// 为移动零件准备数据。
	GetPartDistanceDetectActionBar()->Show_PartDistanceDetectActionBar();
	UpdateBeforeMov(pMovePartPlacements, pMovePartPlacements->GetRectBox().GetCenterPt(), m_pMatSize, m_pPartInstanceList);
}

AddPartPlacementFromDxfdwgAction::~AddPartPlacementFromDxfdwgAction(void)
{
}

BOOL AddPartPlacementFromDxfdwgAction::LButtonDown(double& dPtX, double& dPtY)
{
	// 保存零件项到数据库。
	if (!m_bPartItemSaved)
	{
		for (unsigned int i = 0; i < m_pPartItemList->size(); i++)
		{
			PartItemPtr pPartItem = m_pPartItemList->at(i);
			PartPtr pPart = pPartItem->GetPart();
			pPart->SetCreateTime(DateHelper::GetCurrentTime());
			pPart->SetModifyTime(DateHelper::GetCurrentTime());
			PartManager::SaveNewPart(pPart, m_pSaveFolder);
		}

		m_bPartItemSaved = TRUE;
	}

	// 把“零件放置”放到板材上。
	PartPlacementListPtr pPartPlacements_willBeSelected(new PartPlacementList);
	const PartPlacementList* pMovePartPlacements = GetMovePartPlacements().get();
	for (unsigned int i = 0; i < pMovePartPlacements->size(); i++)
	{
		PartPlacementPtr pPartPlacement = pMovePartPlacements->at(i);
		ICommandPtr pCommand(new PartPlacementAddCommand(GetEditorData(), m_pPartPlacementList, pPartPlacement, m_pPartInstanceList));
		pCommand->Do();
		CommandManager::GetInstance()->AddCommand(pCommand);
		pPartPlacements_willBeSelected->push_back(pPartPlacement);
	}

	// 当添加一个零件时可以连续操作。
	if (pMovePartPlacements->size() > 1)
	{
		PostMessage2EnterPartPlacementSelectAction(pPartPlacements_willBeSelected.get());
	}
	else // 准备下一次操作的数据。
	{
		PartPlacementListPtr pPartPlacements(new PartPlacementList); // 这一次要放的零件放置。
		for (unsigned int i = 0; i < pMovePartPlacements->size(); i++)
			pPartPlacements->push_back(PartPlacementManager::ClonePartPmt(pMovePartPlacements->at(i).get()));
		UpdateBeforeMov(pPartPlacements, pPartPlacements->GetRectBox().GetCenterPt(), m_pMatSize, m_pPartInstanceList);
	}

	// 更新任务
	{
		// 板材相关
		ClWindowFinder::GetCurrentView_1()->SendMessage(WM_updateSheetPreviewData, 0, 0);
		ClWindowFinder::GetCurrentView_2()->SendMessage(WM_redrawSheetThumbnailOnSheetManagementPanel, 0, 0);

		// 零件相关
		ClWindowFinder::GetCurrentView_2()->SendMessage(WM_PartAddedOnSheet, 0, 0);
	}

	return TRUE;
}

BOOL AddPartPlacementFromDxfdwgAction::MovePoint(double& dPtX, double& dPtY)
{
	Point2D currentPt(dPtX, dPtY);

	// 移动的过程中更新一些数据。
	UpdateWhenMov(currentPt);

	return TRUE;
}

BOOL AddPartPlacementFromDxfdwgAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	PartPlacementEditAction::OnKeyDown(nChar, nRepCnt, nFlags);
	UpdateDataWhenRotate(nChar);

	return TRUE;
}

BOOL AddPartPlacementFromDxfdwgAction::DrawBeforeFinish()
{
	// 绘制移动中的“零件放置”。
	PartPlacementDrawer partPlacementDrawer(m_pViewPort);
	const PartPlacementList* pMovePartPlacements = GetMovePartPlacements().get();
	partPlacementDrawer.DrawPartPlacements(pMovePartPlacements);

	// draw cursor.
	CursorDrawer::DrawCrossLineCursor(m_pViewPort);

	return TRUE;
}

void AddPartPlacementFromDxfdwgAction::UpdateForUndoRedo()
{
	// 为零件吸附准备数据。
	PartPlacementListPtr pMovePartPlacements(new PartPlacementList);
	PartPlacementPtr pPartPlacement = PartPlacementManager::ClonePartPmt(GetMovePartPlacements()->front().get());
	pMovePartPlacements->push_back(pPartPlacement);
	UpdateBeforeMov(pMovePartPlacements, pMovePartPlacements->GetRectBox().GetCenterPt(), m_pMatSize, m_pPartInstanceList);
}

END_CUTLEADER_NAMESPACE()

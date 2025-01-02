#include "StdAfx.h"
#include "PartPlacementDeleteAction.h"

#include "GlViewPort.h"
#include "CommandManager.h"
#include "clUILibResource.h"
#include "CursorDrawer.h"
#include "ValueInputManager.h"
#include "ClWindowFinder.h"

#include "CLOptionInfo.h"
#include "DisplayParam.h"

#include "PartPlacement.h"
#include "PartPlacementList.h"
#include "PartPlacementSelectAction.h"
#include "PartPlacementDeleteCommand.h"
#include "clPartLayoutResource.h"

BEGIN_CUTLEADER_NAMESPACE()

PartPlacementDeleteAction::PartPlacementDeleteAction(IMaterialSizePtr pMaterialSize, PartPlacementListPtr pPartPlacements, PartInstanceListPtr pPartInstances,
	PartGroupListPtr pPartGroups, PartPlacementListPtr pSelectedPartPlacements, GlViewPortPtr pViewPort)
    : PartPlacementPreSelectAction(pMaterialSize, pPartPlacements, pPartInstances, pPartGroups, pViewPort)
{
	m_pSelectedPartPlacements = pSelectedPartPlacements;
	m_iMsgID = IDS_OUTPUT_SELECT_PRTPMT_DEL;
}

PartPlacementDeleteAction::~PartPlacementDeleteAction(void)
{
}

BOOL PartPlacementDeleteAction::LButtonDown(double& dPtX, double& dPtY)
{
	if (m_pSelectedPartPlacements->size() == 0)
	{
		PartPlacementPreSelectAction::LButtonDown(dPtX, dPtY);
	}

	return TRUE;
}

BOOL PartPlacementDeleteAction::LButtonUp(double& dPtX, double& dPtY)
{
	if (m_pSelectedPartPlacements->size() == 0)
	{
		PartPlacementPreSelectAction::LButtonUp(dPtX, dPtY);
	}

	// 删除选中的“零件放置”。
	DeleteSelectedPartPlacements();

	return TRUE;
}

BOOL PartPlacementDeleteAction::MovePoint(double& dPtX, double& dPtY)
{
	if (m_pSelectedPartPlacements->size() == 0)
	{
		PartPlacementPreSelectAction::MovePoint(dPtX, dPtY);
	}

	return TRUE;
}

BOOL PartPlacementDeleteAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (m_pSelectedPartPlacements->size() == 0)
	{
		PartPlacementPreSelectAction::OnKeyDown(nChar, nRepCnt, nFlags);
	}
	else
	{
		PartPlacementEditAction::OnKeyDown(nChar, nRepCnt, nFlags);
	}

	return TRUE;
}

BOOL PartPlacementDeleteAction::DrawBeforeFinish()
{
	if (m_pSelectedPartPlacements->size() == 0)
	{
		PartPlacementPreSelectAction::DrawBeforeFinish();
	}
	else
	{
		// draw cursor.
		CursorDrawer::DrawAimSelCursor(m_pViewPort);
	}

	return TRUE;
}

void PartPlacementDeleteAction::UpdateForUndoRedo()
{
	for (unsigned int i = 0; i < m_pSelectedPartPlacements->size(); i++)
	{
		PartPlacementPtr pPartPlacement = m_pSelectedPartPlacements->at(i);
		if (!m_pSelectedPartPlacements->GetPartPlacementByID(pPartPlacement->GetID()))
		{
			// reset the action status.
			m_pSelectedPartPlacements->clear();
			m_pActivePartPlacements->clear();

			break;
		}
	}
}

void PartPlacementDeleteAction::DeleteSelectedPartPlacements()
{
	if (m_pSelectedPartPlacements->size() > 0)
	{
		// 执行命令。
		ICommandPtr pCommand(new PartPlacementDeleteCommand(GetEditorData(), m_pPartPlacementList, m_pPartInstanceList, m_pPartGroupList, m_pSelectedPartPlacements));
		pCommand->Do();
		CommandManager::GetInstance()->AddCommand(pCommand);

		// 重置数据。
		m_pActivePartPlacements->clear();
		m_pSelectedPartPlacements->clear();

		// 做一些更新。
		ClWindowFinder::GetCurrentView_1()->SendMessage(WM_updateSheetPreviewData, 0, 0);
		ClWindowFinder::GetCurrentView_2()->SendMessage(WM_redrawSheetThumbnailOnSheetManagementPanel, 0, 0);
		ClWindowFinder::GetCurrentView_2()->SendMessage(WM_ReDisplayNestedCountOfEachPart, 0, 0);
	}
}

END_CUTLEADER_NAMESPACE()

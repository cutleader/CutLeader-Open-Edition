#include "StdAfx.h"
#include "PartPlacementMoveAction.h"

#include "baseConst.h"
#include "GlViewPort.h"
#include "CommandManager.h"
#include "ValueInputManager.h"
#include "CursorDrawer.h"
#include "clUILibResource.h"
#include "ClWindowFinder.h"

#include "DisplayParam.h"
#include "SheetEditParam.h"
#include "CLOptionInfo.h"

#include "IPatternLoop.h"
#include "PatternLoopCache.h"
#include "PartPlacement.h"
#include "PartPlacementList.h"
#include "PartInstance.h"
#include "PartInstanceList.h"
#include "LoopInstance.h"
#include "LoopInstanceCache.h"
#include "LoopInstanceList.h"
#include "LoopInstanceManager.h"
#include "PartPlacementDrawer.h"
#include "PartPlacementTransformCommand.h"
#include "PartPlacementSelectAction.h"
#include "clPartLayoutResource.h"
#include "PartPlacementManager.h"

BEGIN_CUTLEADER_NAMESPACE()

PartPlacementMoveAction::PartPlacementMoveAction(IMaterialSizePtr pMaterialSize, PartPlacementListPtr pPartPlacements, PartInstanceListPtr pPartInstances, PartGroupListPtr pPartGroups,
	PartPlacementListPtr pSelectedPartPlacements, const Point2D& referencePt, GlViewPortPtr pViewPort, PartDistanceDetectActionBar* pPartDistanceDetectActionBar)
    : PartPlacementPreSelectAction(pMaterialSize, pPartPlacements, pPartInstances, pPartGroups, pViewPort), PartPlacementMoveBase(m_pViewPort, pPartDistanceDetectActionBar)
{
	m_pSelectedPartPlacements = pSelectedPartPlacements;

	if (m_pSelectedPartPlacements->size() > 0)
	{
		m_referencePt = referencePt;
		m_iMsgID = IDS_OUTPUT_MOVEPARTPMT_NEWLOCATION;

		// 为移动零件准备数据。
		PartPlacementListPtr pMovePartPlacements = boost::dynamic_pointer_cast<PartPlacementList>(m_pSelectedPartPlacements->Clone()); // 克隆一份“零件放置”用于移动。
		PartInstanceListPtr pFixPartInsts(new PartInstanceList);
		for (unsigned int i = 0; i < m_pPartPlacementList->size(); i++) {
			const PartPlacement* pPartPlacement = m_pPartPlacementList->operator [](i).get();
			PartInstanceListPtr pPartInstances = m_pPartInstanceList->GetPartInstanceByPartPlacement(pPartPlacement);
			if (!m_pSelectedPartPlacements->GetPartPlacementByID(pPartPlacement->GetID()))
				pFixPartInsts->insert(pFixPartInsts->end(), pPartInstances->begin(), pPartInstances->end());
		}
		UpdateBeforeMov(pMovePartPlacements, m_referencePt, pMaterialSize, pFixPartInsts);
		GetPartDistanceDetectActionBar()->Show_PartDistanceDetectActionBar();
	}
	else {
		m_iMsgID = IDS_OUTPUT_SELECT_PRTPMT_MOVE;
	}
}

PartPlacementMoveAction::~PartPlacementMoveAction(void)
{
}

BOOL PartPlacementMoveAction::LButtonDown(double& dPtX, double& dPtY)
{
	if (m_pSelectedPartPlacements->size() == 0)
	{
		PartPlacementPreSelectAction::LButtonDown(dPtX, dPtY);
	}
	else
	{
		// 移动零件放置。
		CString str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_COMMAND_MOVE_PARTPMT);
		PartPlacementListPtr pSelPartPmts(new PartPlacementList);
		pSelPartPmts->insert(pSelPartPmts->end(), m_pSelectedPartPlacements->begin(), m_pSelectedPartPlacements->end());
		ICommandPtr pCommand(new PartPlacementTransformCommand(str, GetEditorData(), m_pPartPlacementList, pSelPartPmts, GetMovePartPlacements()));
		pCommand->Do();
		CommandManager::GetInstance()->AddCommand(pCommand);

		ClWindowFinder::GetCurrentView_1()->SendMessage(WM_updateSheetPreviewData, 0, 0);
		ClWindowFinder::GetCurrentView_2()->SendMessage(WM_redrawSheetThumbnailOnSheetManagementPanel, 0, 0);

        // 选中移动后的零件放置。
        PostMessage2EnterPartPlacementSelectAction(m_pSelectedPartPlacements.get());
	}

	return TRUE;
}

BOOL PartPlacementMoveAction::LButtonUp(double& dPtX, double& dPtY)
{
	if (m_pSelectedPartPlacements->size() == 0)
	{
		PartPlacementPreSelectAction::LButtonUp(dPtX, dPtY);

		if (m_pSelectedPartPlacements->size() > 0)
		{
			// 以零件放置的中心点为参考点。
			m_referencePt = m_pSelectedPartPlacements->GetRectBox().GetCenterPt();

			// 为移动零件准备数据。
			PartPlacementListPtr pMovePartPlacements = boost::dynamic_pointer_cast<PartPlacementList>(m_pSelectedPartPlacements->Clone());
			PartInstanceListPtr pFixPartInsts(new PartInstanceList);
			for (unsigned int i = 0; i < m_pPartPlacementList->size(); i++)
			{
				const PartPlacement* pPartPlacement = m_pPartPlacementList->operator [](i).get();
				PartInstanceListPtr pPartInstances = m_pPartInstanceList->GetPartInstanceByPartPlacement(pPartPlacement);
				if (!m_pSelectedPartPlacements->GetPartPlacementByID(pPartPlacement->GetID()))
					pFixPartInsts->insert(pFixPartInsts->end(), pPartInstances->begin(), pPartInstances->end());
			}
			UpdateBeforeMov(pMovePartPlacements, m_referencePt, m_pMatSize, pFixPartInsts);

			m_iMsgID = IDS_OUTPUT_MOVEPARTPMT_NEWLOCATION;
		}
	}

	return TRUE;
}

BOOL PartPlacementMoveAction::MovePoint(double& dPtX, double& dPtY)
{
	Point2D currentPt(dPtX, dPtY);
	if (m_pSelectedPartPlacements->size() == 0) {
		PartPlacementPreSelectAction::MovePoint(dPtX, dPtY);
	}
	else {
		// 移动的过程中更新一些数据。
		UpdateWhenMov(currentPt);
	}

	return TRUE;
}

BOOL PartPlacementMoveAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	__super::OnKeyDown(nChar, nRepCnt, nFlags);
	if (m_pSelectedPartPlacements->size() > 0) {
		UpdateDataWhenRotate(nChar);
	}

	return TRUE;
}

BOOL PartPlacementMoveAction::DrawBeforeFinish()
{
	if (m_pSelectedPartPlacements->size() == 0)
	{
		PartPlacementPreSelectAction::DrawBeforeFinish();
	}
	else
	{
		const DisplayParam* pDisplayParam = CLOptionInfo::GetInstance()->GetDisplayParam().get();

		// 绘制选中的“零件放置”。
		PartPlacementDrawer partPlacementDrawer(m_pViewPort);
		partPlacementDrawer.DrawSelectedPartPlacements(m_pSelectedPartPlacements.get());

		const PartPlacementList* pMovePartPlacements = GetMovePartPlacements().get();
		if (pMovePartPlacements->size() > 0)
		{
			COLORREF dwOldColor;
			m_pViewPort->GetDrawColor(dwOldColor);

			// 绘制牵引线。
			m_pViewPort->SetDrawColor(*pDisplayParam->GetDrawAssistColor());
			m_pViewPort->DrawLinePoint2D(m_referencePt.X(), m_referencePt.Y(), GetRefPtOfMovingPartPmts().X(), GetRefPtOfMovingPartPmts().Y());

			// 绘制移动中的“零件放置”。
			partPlacementDrawer.DrawPartPlacements(pMovePartPlacements);

			m_pViewPort->SetDrawColor(dwOldColor);

			// 绘制辅助信息。
			DrawAssistInfo(_T(""));
		}

		// draw cursor.
		CursorDrawer::DrawCrossLineCursor(m_pViewPort);
	}

	return TRUE;
}

END_CUTLEADER_NAMESPACE()

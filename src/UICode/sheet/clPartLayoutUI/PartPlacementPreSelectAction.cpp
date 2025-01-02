#include "StdAfx.h"
#include "PartPlacementPreSelectAction.h"

#include "Point2D.h"
#include "MathEx.h"
#include "GlViewPort.h"
#include "CommandManager.h"
#include "ValueInputManager.h"
#include "DummyAction.h"
#include "CursorDrawer.h"
#include "clBaseDataResource.h"

#include "DisplayParam.h"
#include "CLOptionInfo.h"

#include "PartPlacementList.h"
#include "PartPlacement.h"
#include "PartPlacementDrawer.h"
#include "clPartLayoutResource.h"
#include "PartLayoutHelper.h"
#include "SelectPartOrLoopActionBar.h"

BEGIN_CUTLEADER_NAMESPACE()

PartPlacementPreSelectAction::PartPlacementPreSelectAction(IMaterialSizePtr pMaterialSize, PartPlacementListPtr pPartPlacementList, PartInstanceListPtr pPartInstanceList,
	PartGroupListPtr pPartGroups, GlViewPortPtr pViewPort)
    : PartPlacementEditAction(pMaterialSize, pPartPlacementList, pPartInstanceList, pPartGroups, pViewPort)
{
	m_pActivePartPlacements.reset(new PartPlacementList);
    m_pSelectedPartPlacements.reset(new PartPlacementList);
	m_preSelectState = PRE_SELECT_NO_INPUT;
}

PartPlacementPreSelectAction::~PartPlacementPreSelectAction(void)
{
}

BOOL PartPlacementPreSelectAction::LButtonDown(double& dPtX, double& dPtY)
{
	if (m_preSelectState == PRE_SELECT_NO_INPUT || m_preSelectState == PRE_SELECT_LEFT_BTN_UP)
	{
		m_preSelectState = PRE_SELECT_LEFT_BTN_DOWN;

		m_data[0] = dPtX;
		m_data[1] = dPtY;
		m_data[2] = dPtX;
		m_data[3] = dPtY;
	}

	return TRUE;
}

BOOL PartPlacementPreSelectAction::LButtonUp(double& dPtX, double& dPtY)
{
	if (m_preSelectState == PRE_SELECT_LEFT_BTN_DOWN)
	{
		m_preSelectState = PRE_SELECT_LEFT_BTN_UP;

		// 要是没有按住ctrl键，就需要清除现有选择集。
		if (!ValueInputManager::GetInstance()->IsInputCtrl())
        {
			m_pSelectedPartPlacements->clear();
        }

        if (m_pActivePartPlacements->size() > 0)
        {
            m_pSelectedPartPlacements->insert(m_pSelectedPartPlacements->end(), m_pActivePartPlacements->begin(), m_pActivePartPlacements->end());
        }
	}

	return TRUE;
}

BOOL PartPlacementPreSelectAction::MovePoint(double& dPtX, double& dPtY)
{
	if (m_preSelectState == PRE_SELECT_LEFT_BTN_DOWN)
	{
		m_data[2] = dPtX;
		m_data[3] = dPtY;

		Rect2D rect;
		rect.SetXMin(m_data[0] < m_data[2] ? m_data[0] : m_data[2]);
		rect.SetXMax(m_data[0] < m_data[2] ? m_data[2] : m_data[0]);
		rect.SetYMin(m_data[1] < m_data[3] ? m_data[1] : m_data[3]);
		rect.SetYMax(m_data[1] < m_data[3] ? m_data[3] : m_data[1]);
        std::map<LONGLONG, Rect2D> rects_of_partPmt;
        m_pActivePartPlacements = PartLayoutHelper::GetPartPlacements_byRectSelection(rect, m_pPartPlacementList.get(), m_pPartGroupList.get(), rects_of_partPmt);
	}
	else
	{
		Point2D pt(dPtX, dPtY);
        std::map<LONGLONG, Rect2D> rects_of_partPmt;
        m_pActivePartPlacements = PartLayoutHelper::GetPartPlacements_byClickSelection(pt, m_pPartPlacementList.get(), m_pPartGroupList.get(), rects_of_partPmt);
	}

	return TRUE;
}

BOOL PartPlacementPreSelectAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	PartPlacementEditAction::OnKeyDown(nChar, nRepCnt, nFlags);

	if (ValueInputManager::GetInstance()->IsInputCtrl() && ValueInputManager::GetInstance()->IsInputA(nChar))
	{
		m_pSelectedPartPlacements->clear();
		m_pSelectedPartPlacements->insert(m_pSelectedPartPlacements->end(), m_pPartPlacementList->begin(), m_pPartPlacementList->end());
	}

	return TRUE;
}

BOOL PartPlacementPreSelectAction::DrawBeforeFinish()
{
	// prepare some data.
	CLOptionInfoPtr pCLOptionInfo = CLOptionInfo::GetInstance();
	DisplayParamPtr pColorInfo = pCLOptionInfo->GetDisplayParam();
	PartPlacementDrawer partPlacementDrawer(m_pViewPort);

	// draw the selected part placement.
	partPlacementDrawer.DrawSelectedPartPlacements(m_pSelectedPartPlacements.get());

	if (m_preSelectState == PRE_SELECT_LEFT_BTN_DOWN) { // 框选
		DrawSelectionBox(m_data[0], m_data[1], m_data[2], m_data[3]);
	}

    // 高亮零件放置
    for (unsigned int i = 0; i < m_pActivePartPlacements->size(); i++)
    {
        partPlacementDrawer.DrawActivePartPlacement(m_pActivePartPlacements->at(i).get());
    }

	// draw cursor.
	CursorDrawer::DrawAimSelCursor(m_pViewPort);

	return TRUE;
}

END_CUTLEADER_NAMESPACE()

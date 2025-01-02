#include "StdAfx.h"
#include "PartPlacementTransformAction.h"

#include "baseConst.h"
#include "ValueInputManager.h"
#include "CommandManager.h"
#include "GlViewPort.h"
#include "clUILibResource.h"
#include "ClWindowFinder.h"

#include "DisplayParam.h"
#include "CLOptionInfo.h"

#include "Part.h"
#include "PatternDrawer.h"
#include "PartPlacement.h"
#include "PartPlacementList.h"
#include "PartPlacementDrawer.h"
#include "PartPlacementTransformCommand.h"
#include "PartPlacementSelectAction.h"
#include "clPartLayoutResource.h"

BEGIN_CUTLEADER_NAMESPACE()

PartPlacementTransformAction::PartPlacementTransformAction(IMaterialSizePtr pMaterialSize, PartPlacementListPtr pPartPlacementList, PartInstanceListPtr pPartInstanceList,
    PartGroupListPtr pPartGroups, GlViewPortPtr pViewPort)
    : PartPlacementPreSelectAction(pMaterialSize, pPartPlacementList, pPartInstanceList, pPartGroups, pViewPort)
{
	m_actionState = TRANSFORM_PARTPMT_NOTSTART;
	m_pNewPartPlacements.reset(new PartPlacementList());
}

PartPlacementTransformAction::~PartPlacementTransformAction(void)
{
}

BOOL PartPlacementTransformAction::LButtonDown(double& dPtX, double& dPtY)
{
	if (m_pSelectedPartPlacements->size() == 0)
	{
		PartPlacementPreSelectAction::LButtonDown(dPtX, dPtY);
	}
	else
	{
		if (m_actionState == TRANSFORM_PARTPMT_NOTSTART || m_actionState == TRANSFORM_PARTPMT_SECOND_PT_SELECTED)
		{
			m_actionState = TRANSFORM_PARTPMT_BASE_PT_SELECTED;
			m_iMsgID = GetMsgID();

			// set the transform matrix.
			m_dBasePtX = dPtX;
			m_dBasePtY = dPtY;
			m_dFirstPtX = dPtX;
			m_dFirstPtY = dPtY;
			m_dSecondPtX = dPtX;
			m_dSecondPtY = dPtY;
			SetMatrixValue();

			// create a new copy of selected part pmt.
			for (unsigned int i = 0; i < m_pSelectedPartPlacements->size(); i++)
			{
				PartPlacementPtr pPartPlacement;
				m_pSelectedPartPlacements->at(i)->CopyTo(pPartPlacement);
				m_pNewPartPlacements->push_back(pPartPlacement);
			}
		}
		else if (m_actionState == TRANSFORM_PARTPMT_BASE_PT_SELECTED)
		{
			m_actionState = TRANSFORM_PARTPMT_SECOND_PT_SELECTED;
			m_iMsgID = GetMsgID();

			// set the transform matrix.
			m_dSecondPtX = dPtX;
			m_dSecondPtY = dPtY;
			m_dFirstPtX = m_dBasePtX;
			m_dFirstPtY = m_dBasePtY;
			SetMatrixValue();

			// create command to transform the part pmts.
			ICommandPtr pCommand(new PartPlacementTransformCommand(GetActionName(), GetEditorData(), m_pPartPlacementList, m_pSelectedPartPlacements, m_pNewPartPlacements));
			pCommand->Do();
			CommandManager::GetInstance()->AddCommand(pCommand);

			// 更新任务
			{
				// 板材相关
				ClWindowFinder::GetCurrentView_1()->SendMessage(WM_updateSheetPreviewData, 0, 0);
				ClWindowFinder::GetCurrentView_2()->SendMessage(WM_redrawSheetThumbnailOnSheetManagementPanel, 0, 0);
			}

			// enter PartPlacementSelectAction.
			PostMessage2EnterPartPlacementSelectAction(m_pSelectedPartPlacements.get());
		}
	}

	return TRUE;
}

BOOL PartPlacementTransformAction::LButtonUp(double& dPtX, double& dPtY)
{
	if (m_pSelectedPartPlacements->size() == 0)
	{
		PartPlacementPreSelectAction::LButtonUp(dPtX, dPtY);

		if (m_pSelectedPartPlacements->size() > 0)
		{
			m_iMsgID = GetMsgID();
		}
	}

	return TRUE;
}

BOOL PartPlacementTransformAction::MovePoint(double& dPtX, double& dPtY)
{
	if (m_pSelectedPartPlacements->size() == 0)
	{
		PartPlacementPreSelectAction::MovePoint(dPtX, dPtY);
	}
	else
	{
		if (m_actionState == TRANSFORM_PARTPMT_NOTSTART)
		{
			m_iMsgID = GetMsgID();
		}
		else if (m_actionState == TRANSFORM_PARTPMT_BASE_PT_SELECTED)
		{
			// set the transform matrix.
			m_dFirstPtX = m_dSecondPtX;
			m_dFirstPtY = m_dSecondPtY;
			m_dSecondPtX = dPtX;
			m_dSecondPtY = dPtY;
			SetMatrixValue();

			// update the new copy of the selected part pmts.
			for (unsigned int i = 0; i < m_pNewPartPlacements->size(); i++)
				m_pNewPartPlacements->at(i)->Transform(m_modifyMatrix);
		}
	}

	return TRUE;
}

BOOL PartPlacementTransformAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	return __super::OnKeyDown(nChar, nRepCnt, nFlags);
}

void PartPlacementTransformAction::UpdateForUndoRedo()
{
	for (unsigned int i = 0; i < m_pSelectedPartPlacements->size(); i++)
	{
		PartPlacementPtr pPartPlacement = m_pSelectedPartPlacements->at(i);
		if (!m_pPartPlacementList->GetPartPlacementByID(pPartPlacement->GetID()))
		{
			m_pSelectedPartPlacements->clear();
			m_pNewPartPlacements->clear();
			m_pActivePartPlacements->clear();

			m_actionState = TRANSFORM_PARTPMT_NOTSTART;
			m_iMsgID = IDS_OUTPUT_SELECTPRTPMT;

			break;
		}
	}
}

END_CUTLEADER_NAMESPACE()

#include "StdAfx.h"
#include "PartPlacementRotateAction.h"

#include "baseConst.h"
#include "GlViewPort.h"
#include "CommandManager.h"
#include "ValueInputManager.h"
#include "CursorDrawer.h"
#include "clUILibResource.h"

#include "Part.h"
#include "PatternDrawer.h"

#include "PartPlacement.h"
#include "PartPlacementList.h"
#include "PartPlacementDrawer.h"
#include "PartPlacementTransformCommand.h"
#include "PartPlacementSelectAction.h"
#include "clPartLayoutResource.h"

#include "DisplayParam.h"
#include "CLOptionInfo.h"

BEGIN_CUTLEADER_NAMESPACE()

PartPlacementRotateAction::PartPlacementRotateAction(IMaterialSizePtr pMaterialSize, PartPlacementListPtr pPartPlacements, PartInstanceListPtr pPartInstances,
	PartGroupListPtr pPartGroups, PartPlacementListPtr pSelectedPartPlacements, GlViewPortPtr pViewPort)
    : PartPlacementTransformAction(pMaterialSize, pPartPlacements, pPartInstances, pPartGroups, pViewPort)
{
	m_pSelectedPartPlacements = pSelectedPartPlacements;

	if (pSelectedPartPlacements->size() > 0) {
		m_iMsgID = IDS_OUTPUT_ROTATEPARTPMT_BASEPT;
	}
	else {
		m_iMsgID = IDS_OUTPUT_SELECT_PRTPMT_ROTATE;
	}
}

PartPlacementRotateAction::~PartPlacementRotateAction(void)
{
}

CString PartPlacementRotateAction::GetActionName()
{
	CString str;
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_COMMAND_ROTATE_PARTPMT);

	return str;
}

void PartPlacementRotateAction::SetMatrixValue()
{
	m_modifyMatrix.Reset();

	Point2D basePoint(m_dBasePtX, m_dBasePtY);
	double dAngle = atan2((m_dSecondPtY - m_dBasePtY), (m_dSecondPtX - m_dBasePtX)) - 
				    atan2((m_dFirstPtY - m_dBasePtY), (m_dFirstPtX - m_dBasePtX));

	m_modifyMatrix.RotateBy(basePoint, dAngle);
}

void PartPlacementRotateAction::ConvertKBInput(double& dVal1, double& dVal2)
{
	if (m_actionState == TRANSFORM_PARTPMT_NOTSTART || m_actionState == TRANSFORM_PARTPMT_SECOND_PT_SELECTED)
	{
		;
	}
	else if (m_actionState == TRANSFORM_PARTPMT_BASE_PT_SELECTED)
	{
		Point2D basePoint(m_dBasePtX, m_dBasePtY);

		// "dVal1" holds the rotate angle.
		Vector2D v1;
		v1.SetAngle(dVal1*D2R);
		basePoint.Transfer(v1);

		dVal1 = basePoint.X();
		dVal2 = basePoint.Y();
	}
}

int PartPlacementRotateAction::GetMsgID()
{
	int iMsgID = INVALID_ID;

	if (m_actionState == TRANSFORM_PARTPMT_NOTSTART || m_actionState == TRANSFORM_PARTPMT_SECOND_PT_SELECTED)
	{
		iMsgID = IDS_OUTPUT_ROTATEPARTPMT_BASEPT;
	}
	else if (m_actionState == TRANSFORM_PARTPMT_BASE_PT_SELECTED)
	{
		iMsgID = IDS_OUTPUT_ROTATEPARTPMT_ANGLE;
	}

	return iMsgID;
}

BOOL PartPlacementRotateAction::DrawBeforeFinish()
{
	CLOptionInfoPtr pCLOptionInfo = CLOptionInfo::GetInstance();
	DisplayParamPtr pColorInfo = pCLOptionInfo->GetDisplayParam();

	if (m_pSelectedPartPlacements->size() == 0)
	{
		PartPlacementPreSelectAction::DrawBeforeFinish();
	}
	else
	{
		// draw the selected part pmt.
		PartPlacementDrawer partPlacementDrawer(m_pViewPort);
		partPlacementDrawer.DrawSelectedPartPlacements(m_pSelectedPartPlacements.get());

		// 绘制引线和旋转中的“零件放置”。
		if (m_actionState == TRANSFORM_PARTPMT_BASE_PT_SELECTED) 
		{
			COLORREF dwOldColor;
			m_pViewPort->GetDrawColor(dwOldColor);

			// draw the trace line.
			m_pViewPort->SetDrawColor(*pColorInfo->GetDrawAssistColor());
			m_pViewPort->DrawLinePoint2D(m_dBasePtX, m_dBasePtY, m_dSecondPtX, m_dSecondPtY);

			// restore the draw color
			m_pViewPort->SetDrawColor(dwOldColor);

			// draw the new copy of the selected part pmts.
			partPlacementDrawer.DrawPartPlacements(m_pNewPartPlacements.get());
		}

		// draw cursor.
		CursorDrawer::DrawCrossLineCursor(m_pViewPort);
	}

	return TRUE;
}

END_CUTLEADER_NAMESPACE()

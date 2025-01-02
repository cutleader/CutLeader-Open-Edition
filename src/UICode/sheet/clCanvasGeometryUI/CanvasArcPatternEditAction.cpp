#include "StdAfx.h"
#include "CanvasArcPatternEditAction.h"

#include "boost/bind.hpp"
#include "Point2D.h"
#include "MathEx.h"
#include "baseConst.h"
#include "GlViewPort.h"
#include "CommandManager.h"
#include "DummyAction.h"
#include "CursorDrawer.h"
#include "clBaseDataResource.h"
#include "clUILibResource.h"
#include "DataBaseManager.h"
#include "ValueInputManager.h"
#include "ClWindowFinder.h"

#include "ConfigItemLoader.h"
#include "SequenceConfigItem.h"
#include "DisplayParam.h"
#include "SheetEditParam.h"
#include "CLOptionInfo.h"

#include "SnapPtDrawer.h"
#include "LoopToolData.h"
#include "LoopCutFeature.h"
#include "PartCamData.h"
#include "PartItem.h"
#include "Part.h"
#include "PartPlacementList.h"
#include "PartPlacement.h"
#include "PartInstanceList.h"
#include "clPartLayoutResource.h"
#include "PartPlacementManager.h"
#include "LoopInstanceList.h"
#include "LoopInstance.h"
#include "PartInstance.h"
#include "clGeometryFeatureResource.h"
#include "ArcPattern.h"
#include "CanvasPatternAddCommand.h"
#include "CanvasPatternGripEditCommand.h"

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

CanvasArcPatternEditAction::CanvasArcPatternEditAction(PartPlacementPtr pPartPlacement, GlViewPortPtr pViewPort)
    : CanvasPatternGripEditActionBase(pPartPlacement, pViewPort)
{
	m_iStatus = DRAW_ARC3P_NOTSTART;
	m_editState = EDIT_ARC_NOTSTART;
	memset(m_data, 0, sizeof(double) * 5);
	m_iMsgID = IDS_OUTPUT_PATARC3PT_FIRSTPT;
}

CanvasArcPatternEditAction::~CanvasArcPatternEditAction(void)
{
}

BOOL CanvasArcPatternEditAction::LButtonDown(double& dPtX, double& dPtY)
{
	if (m_editState != EDIT_ARC_NOTSTART) // 夹点编辑
	{
		// create the new arc pattern with the new data.
		Point2D centerPt(m_data[0], m_data[1]);
		IPatternPtr pNewPat(new ArcPattern(centerPt, m_data[2], m_data[3], m_data[4]));

		// 执行命令。
		PartCadDataPtr pPartCadData = m_pPartPlacement->GetPart()->GetCadData();
		PartCamDataPtr pPartCamData = m_pPartPlacement->GetCamData();
		ICommandPtr pCommand(new CanvasPatternGripEditCommand(GetEditorData(), pPartCadData, pPartCamData, m_pGripEditPat->GetID(), pNewPat));
		pCommand->Do();
		CommandManager::GetInstance()->AddCommand(pCommand);

		// 选中刚刚编辑的几何特征。
		PostMessage2EnterCanvasPatternSelectAction_PreSelectOnePattern(m_pGripEditPat.get());

		// 做一些更新。
		ClWindowFinder::GetCurrentView_1()->SendMessage(WM_updateSheetPreviewData, 0, 0);
		ClWindowFinder::GetCurrentView_2()->SendMessage(WM_redrawSheetThumbnailOnSheetManagementPanel, 0, 0);
	}
	else
	{
		if (m_iStatus == DRAW_ARC3P_NOTSTART || m_iStatus == DRAW_ARC3P_END_POINT)
		{
			m_iStatus = DRAW_ARC3P_START_POINT;
			m_iMsgID = IDS_OUTPUT_PATARC3PT_SECONDPT;

			m_data[0] = dPtX;
			m_data[1] = dPtY;
			m_data[2] = dPtX;
			m_data[3] = dPtY;
			m_data[4] = dPtX;
			m_data[5] = dPtY;
		}
		else if (m_iStatus == DRAW_ARC3P_START_POINT)
		{
			// 检查能不能进入下一状态。
			if (Point2D(m_data[0], m_data[1]) == Point2D(dPtX, dPtY)) {
				return FALSE;
			}

			m_data[2] = dPtX;
			m_data[3] = dPtY;
			m_data[4] = dPtX;
			m_data[5] = dPtY;

			m_iStatus = DRAW_ARC3P_MID_POINT;
			m_iMsgID = IDS_OUTPUT_PATARC3PT_THIRDPT;
		}
		else if (m_iStatus == DRAW_ARC3P_MID_POINT)
		{
			m_data[4] = dPtX;
			m_data[5] = dPtY;

			// calculate arc form 3 points
			double dCenterX = 0, dCenterY = 0, dRadius = 0, dStartAngle = 0, dMoveAngle = 0;
			MathEx::GetArcFrom3Point(m_data[0], m_data[1], m_data[2], m_data[3], m_data[4], m_data[5], dCenterX, dCenterY, dRadius, dStartAngle, dMoveAngle);

			// check whether 3 pts are on a line.
			if (dRadius < GEOM_TOLERANCE)
				return FALSE;

			m_iStatus = DRAW_ARC3P_END_POINT;
			m_iMsgID = IDS_OUTPUT_PATARC3PT_FIRSTPT;

			// create arc pattern.
			Point2D centerPt(dCenterX, dCenterY);
			IPatternPtr pArcPat(new ArcPattern(centerPt, dRadius, dStartAngle, dMoveAngle));
			pArcPat->UpdateCache();

			// 执行命令。
			PartCadDataPtr pPartCadData = m_pPartPlacement->GetPart()->GetCadData();
			PartCamDataPtr pPartCamData = m_pPartPlacement->GetCamData();
			ICommandPtr pCommand(new CanvasPatternAddCommand(GetEditorData(), pPartCadData, pPartCamData, pArcPat));
			pCommand->Do();
			CommandManager::GetInstance()->AddCommand(pCommand);

			// 做一些更新。
			ClWindowFinder::GetCurrentView_1()->SendMessage(WM_updateSheetPreviewData, 0, 0);
			ClWindowFinder::GetCurrentView_2()->SendMessage(WM_redrawSheetThumbnailOnSheetManagementPanel, 0, 0);
		}
	}

	return TRUE;
}

BOOL CanvasArcPatternEditAction::MovePoint(double& dPtX, double& dPtY)
{
	if (m_editState != EDIT_ARC_NOTSTART) // 夹点编辑
	{
		double dStartX = m_data[0] + m_data[2] * cos(m_data[3]);
		double dStartY = m_data[1] + m_data[2] * sin(m_data[3]);
		double dStartAngle = atan2((dStartY - m_data[1]), (dStartX - m_data[0]));
		double dEndX = m_data[0] + m_data[2] * cos(m_data[3] + m_data[4]);
		double dEndY = m_data[1] + m_data[2] * sin(m_data[3] + m_data[4]);
		double dEndAngle = atan2((dEndY - m_data[1]), (dEndX - m_data[0]));
		double dPointAngle = atan2((dPtY - m_data[1]), (dPtX - m_data[0])); // angle of current mouse position to center point line.

		if (m_editState == EDIT_ARC_CENTER_POINT)
		{
			m_data[0] = dPtX;
			m_data[1] = dPtY;
		}
		else if (m_editState == EDIT_ARC_START_POINT)
		{
			double dAngle = dPointAngle - dStartAngle;
			m_data[3] += dAngle;
			m_data[4] -= dAngle;
		}
		else if (m_editState == EDIT_ARC_MID_POINT)
		{
			ChangeMidPoint(dPtX, dPtY);
		}
		else if (m_editState == EDIT_ARC_END_POINT)
		{
			double dAngle = dPointAngle - dEndAngle;
			m_data[4] += dAngle;
		}
	}
	else
	{
		if (m_iStatus == DRAW_ARC3P_START_POINT)
		{
			m_data[2] = dPtX;
			m_data[3] = dPtY;
			m_data[4] = dPtX;
			m_data[5] = dPtY;
		}
		else if (m_iStatus == DRAW_ARC3P_MID_POINT)
		{
			m_data[4] = dPtX;
			m_data[5] = dPtY;
		}
	}

	return TRUE;
}

BOOL CanvasArcPatternEditAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CanvasGeometryEditAction::OnKeyDown(nChar, nRepCnt, nFlags);

	return TRUE;
}

BOOL CanvasArcPatternEditAction::DrawBeforeFinish()
{
	const DisplayParam* pDisplayParam = CLOptionInfo::GetInstance()->GetDisplayParam().get();

	// 老的设置
	COLORREF dwOldColor;
	m_pViewPort->GetDrawColor(dwOldColor);

	// 绘制圆弧。
	m_pViewPort->SetDrawColor(*pDisplayParam->GetPatColor());
	if (m_editState != EDIT_ARC_NOTSTART)
		m_pViewPort->DrawArc2D(m_data[0], m_data[1], m_data[2], m_data[3], m_data[4]);
	else
	{
		if (m_iStatus == DRAW_ARC3P_START_POINT)
			m_pViewPort->DrawLinePoint2D(m_data[0], m_data[1], m_data[2], m_data[3]);
		else if (m_iStatus == DRAW_ARC3P_MID_POINT) // we should draw arc after second click.
		{
			double centerX = 0, centerY = 0, radius = 0, startAngle = 0, moveAngle = 0;
			MathEx::GetArcFrom3Point(m_data[0], m_data[1], m_data[2], m_data[3], m_data[4], m_data[5], centerX, centerY, radius, startAngle, moveAngle);
			m_pViewPort->DrawArc2D(centerX, centerY, radius, startAngle, moveAngle);
		}
	}

	// 绘制光标。
	CursorDrawer::DrawCrossLineCursor(m_pViewPort);

	// 恢复设置
	m_pViewPort->SetDrawColor(dwOldColor);

	return TRUE;
}

void CanvasArcPatternEditAction::UpdateForUndoRedo()
{

}

void CanvasArcPatternEditAction::SetGripEditPattern(IPatternPtr pPattern)
{
	m_pGripEditPat = pPattern;

	ArcPattern* pArcPat = dynamic_cast<ArcPattern*>(pPattern.get());
	Point2D centerPt = pArcPat->GetBasePoint();
	m_data[0] = centerPt.X();
	m_data[1] = centerPt.Y();
	m_data[2] = pArcPat->GetRadius();
	m_data[3] = pArcPat->GetStartAngle();
	m_data[4] = pArcPat->GetMoveAngle();
}

void CanvasArcPatternEditAction::SetGripEditPt(double dPtX, double dPtY)
{
	// get snap points of the pattern
	Point2DList snapPts;
	m_pGripEditPat->GetSnapPoints(snapPts);

	Point2D tmpPt(dPtX, dPtY);
	for (unsigned int i = 0; i < snapPts.size(); i++)
	{
		Point2D snapPt = (Point2D)snapPts[i];
		if (snapPt.IsEqual(tmpPt))
		{
			if (i == 0)
			{
				m_editState = EDIT_ARC_CENTER_POINT;
				m_iMsgID = IDS_OUTPUT_PATARC_CENTERPT;
			}
			else if (i == 1)
			{
				m_editState = EDIT_ARC_START_POINT;
				m_iMsgID = IDS_OUTPUT_PATARC_STARTANGLE;
			}
			else if (i == 2)
			{
				m_editState = EDIT_ARC_MID_POINT;
				m_iMsgID = IDS_OUTPUT_PATARC_CENTERPT_RAD;
			}
			else if (i == 3)
			{
				m_editState = EDIT_ARC_END_POINT;
				m_iMsgID = IDS_OUTPUT_PATARC_MOVEANGLE;
			}

			return;
		}
	}
}

void CanvasArcPatternEditAction::ChangeMidPoint(double midPtX, double midPtY)
{
	double dX1 = m_data[0] + m_data[2] * cos(m_data[3]);
	double dY1 = m_data[1] + m_data[2] * sin(m_data[3]);

	double dX2 = midPtX;
	double dY2 = midPtY;

	double dX3 = m_data[0] + m_data[2] * cos(m_data[3] + m_data[4]);
	double dY3 = m_data[1] + m_data[2] * sin(m_data[3] + m_data[4]);

	double dArea = fabs(((dX1 - dX2) * (dY3 - dY1) + (dY1 - dY2) * (dX1 - dX3)) / 2);
	if (dArea != 0)
	{
		double dX32 = (dX3 - dX2);
		double dX12 = (dX1 - dX2);
		double dY12 = (dY1 - dY2);
		double dY32 = (dY3 - dY2);

		double dC = dY12 * dX32 - dY32 * dX12;
		double dA = (dY1 * dY1 - dY2 * dY2) + (dX1 * dX1 - dX2 * dX2);
		double dB = (dY3 * dY3 - dY2 * dY2) + (dX3 * dX3 - dX2 * dX2);

		double dCenterX, dCenterY;
		dCenterY = (dA * dX32 - dB * dX12) / dC / 2;
		if (dX32 != 0)
			dCenterX = (-2 * dY32 * dCenterY + dB) / dX32 / 2;
		else
			dCenterX = (-2 * dY12 * dCenterY + dA) / dX12 / 2;

		m_data[0] = dCenterX;
		m_data[1] = dCenterY;
		m_data[2] = sqrt((dCenterX - dX1) * (dCenterX - dX1) + (dCenterY - dY1) * (dCenterY - dY1));

		double dStartAngle = atan2(dY1 - dCenterY, dX1 - dCenterX);
		double dMidAngle = atan2(dY2 - dCenterY, dX2 - dCenterX);
		double dEndAngle = atan2(dY3 - dCenterY, dX3 - dCenterX);
		double dMoveAngle = dEndAngle - dStartAngle;
		if (dStartAngle < dEndAngle)
		{
			if ((dMidAngle < dStartAngle) || (dMidAngle > dEndAngle))
				dMoveAngle -= TWOPI;
		}
		else
		{
			if ((dMidAngle < dEndAngle) || (dMidAngle > dStartAngle))
				dMoveAngle += TWOPI;
		}

		m_data[3] = dStartAngle;
		m_data[4] = dMoveAngle;
	}
}

END_CUTLEADER_NAMESPACE()

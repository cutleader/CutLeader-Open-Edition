#include "StdAfx.h"
#include "ArcPatternEditAction.h"

#include "baseConst.h"
#include "GlViewPort.h"
#include "Point2D.h"
#include "ICommand.h"
#include "ValueInputManager.h"
#include "clUILibResource.h"
#include "CommandManager.h"
#include "CursorDrawer.h"
#include "Point2DList.h"

#include "CLOptionInfo.h"
#include "DisplayParam.h"

#include "ArcPattern.h"
#include "PatternSelectAction.h"
#include "PatternAddCommand.h"
#include "GripEditCommand.h"
#include "PatternList.h"
#include "clGeometryFeatureResource.h"
#include "PatternPropertyBar.h"
#include "PartCadData.h"

BEGIN_CUTLEADER_NAMESPACE()

ArcPatternEditAction::ArcPatternEditAction(PartCadDataPtr pPartCadData, GlViewPortPtr pViewPort)
{
	m_iStatus = DRAW_ARC_NOTSTART;
	m_editState = EDIT_ARC_NOTSTART;
	memset(m_data, 0, sizeof(double)*5);

	m_pPartCadData = pPartCadData;
	m_pViewPort = pViewPort;

	m_iMsgID = IDS_OUTPUT_PATARC_CENTERPT;
}

ArcPatternEditAction::~ArcPatternEditAction(void)
{
}

BOOL ArcPatternEditAction::LButtonDown(double& dPtX, double& dPtY)
{
	PatternListPtr pPatList = m_pPartCadData->GetPatternList();
	PatternLoopListPtr pPatternLoopList = m_pPartCadData->GetPatternLoopList();
	LoopTopologyItemListPtr pLoopTopologyItems = m_pPartCadData->GetLoopTopologyItems_FW();

	if (m_editState != EDIT_ARC_NOTSTART)
	{
		// create the new arc pattern with the new data.
		Point2D centerPt(m_data[0], m_data[1]);
		IPatternPtr pNewPat(new ArcPattern(centerPt, m_data[2], m_data[3], m_data[4]));

		// create command
		EditorData editorData = GetEditorData();
		ICommandPtr pCommand(new GripEditCommand(editorData, pNewPat, pPatternLoopList, pLoopTopologyItems, m_pGripEditPat));
		pCommand->Do();
		CommandManager::GetInstance()->AddCommand(pCommand);

		// enter the selecting pattern action.
		PostMessage2EnterPatternSelectAction(m_pGripEditPat.get());
	}
	else
	{
		if (m_iStatus == DRAW_ARC_NOTSTART || m_iStatus == DRAW_ARC_END_POINT)
		{
			m_iStatus = DRAW_ARC_CENTER_POINT;
			m_iMsgID = IDS_OUTPUT_PATARC_RAD_STARTANGLE;

			m_data[0] = dPtX;
			m_data[1] = dPtY;
			m_data[2] = 0;
			m_data[3] = 0;
			m_data[4] = 0;
		}
		else if(m_iStatus == DRAW_ARC_CENTER_POINT)
		{
			m_iStatus = DRAW_ARC_START_POINT;
			m_iMsgID = IDS_OUTPUT_PATARC_MOVEANGLE;

			m_data[2] = sqrt((dPtX - m_data[0])*(dPtX - m_data[0])+(dPtY - m_data[1])*(dPtY - m_data[1]));
			m_data[3] = atan2((dPtY - m_data[1]), (dPtX - m_data[0]));
		}
		else if(m_iStatus == DRAW_ARC_START_POINT)
		{
			m_iStatus = DRAW_ARC_END_POINT;
			m_iMsgID = IDS_OUTPUT_PATARC_CENTERPT;
			
			// create arc pattern.
			Point2D centerPt(m_data[0], m_data[1]);
			IPatternPtr pArcPat(new ArcPattern(centerPt, m_data[2], m_data[3], m_data[4]));

			// create command
			EditorData editorData = GetEditorData();
			ICommandPtr pCommand(new PatternAddCommand(editorData, pPatList, pPatternLoopList, pLoopTopologyItems, pArcPat));
			pCommand->Do();
			CommandManager::GetInstance()->AddCommand(pCommand);

			// enter the selecting pattern action.
			PostMessage2EnterPatternSelectAction(pArcPat.get());
		}
	}

	return TRUE;
}

BOOL ArcPatternEditAction::MovePoint(double& dPtX, double& dPtY)
{
	double dStartX, dStartY, dStartAngle;
	dStartX = m_data[0] + m_data[2] * cos(m_data[3]);
	dStartY = m_data[1] + m_data[2] * sin(m_data[3]);
	dStartAngle = atan2((dStartY - m_data[1]), (dStartX - m_data[0]));

	double dEndX, dEndY, dEndAngle;
	dEndX = m_data[0] + m_data[2] *	cos(m_data[3]+m_data[4]);
	dEndY = m_data[1] + m_data[2] * sin(m_data[3]+m_data[4]);
	dEndAngle = atan2((dEndY - m_data[1]), (dEndX - m_data[0]));

	// angle of current mouse position to center point line
	double dPointAngle = atan2((dPtY - m_data[1]), (dPtX - m_data[0]));

	if (m_editState != EDIT_ARC_NOTSTART) //EDIT mode
	{
		if (m_editState == EDIT_ARC_CENTER_POINT)
		{
			m_data[0] = dPtX;
			m_data[1] = dPtY;
		}
		else if(m_editState == EDIT_ARC_START_POINT)
		{
			double dAngle = dPointAngle - dStartAngle;
			m_data[3] += dAngle;
			m_data[4] -= dAngle;
		}
		else if(m_editState == EDIT_ARC_MID_POINT)
		{
			ChangeMidPoint(dPtX, dPtY);
		}
		else if(m_editState == EDIT_ARC_END_POINT)
		{
			double dAngle = dPointAngle - dEndAngle;
			m_data[4] += dAngle;
		}
	}
	else
	{
		if (m_iStatus == DRAW_ARC_CENTER_POINT)
		{
			m_data[2] = sqrt((dPtX-m_data[0])*(dPtX-m_data[0]) + (dPtY - m_data[1])*(dPtY-m_data[1]));
			m_data[3] = dPointAngle;
		}
		else if(m_iStatus == DRAW_ARC_START_POINT)
		{
			double dAngle = dPointAngle - dEndAngle;
			m_data[4] += dAngle;
		}
	}

	return TRUE;
}

void ArcPatternEditAction::ConvertKBInput(double& dVal1, double& dVal2)
{
	if (m_editState != EDIT_ARC_NOTSTART)
	{
		// under "edit" action.

		if (m_editState == EDIT_ARC_CENTER_POINT ||
			m_editState == EDIT_ARC_MID_POINT)
		{
			// now, do not need to convert.
			;
		}
		else if(m_editState == EDIT_ARC_START_POINT)
		{
			// "dVal1" holds the start angle.

			// the vector from center point to start point.
			Vector2D v1;
			v1.Magnitude(m_data[2]);
			v1.SetAngle(dVal1);

			// the start point.
			Point2D centerPt(m_data[0], m_data[1]);
			Point2D startPt = centerPt;
			startPt.Transfer(v1);

			dVal1 = startPt.X();
			dVal2 = startPt.Y();
		}
		else if(m_editState == EDIT_ARC_END_POINT)
		{
			// "dVal1" holds the sweep angle, so we
			// should figure out the end point.

			/************************************************************************/		
			// get the end point.

			// the vector from center point to start point.
			Vector2D v1;
			v1.Magnitude(m_data[2]);
			v1.SetAngle(m_data[2]);

			// the start point.
			Point2D centerPt(m_data[0], m_data[1]);
			Point2D startPt = centerPt;
			startPt.Transfer(v1);

			// rotate the start point around the center point,
			// then can get end point.
			startPt.RotateBasedPt(centerPt, dVal1);
			/************************************************************************/

			dVal1 = startPt.X();
			dVal2 = startPt.Y();
		}
	}
	else
	{
		// under "draw" action.

		if (m_iStatus == DRAW_ARC_NOTSTART || m_iStatus == DRAW_ARC_END_POINT ||
			m_iStatus == DRAW_ARC_CENTER_POINT)
		{
			// now, do not need to convert.
			;
		}
		else if(m_iStatus == DRAW_ARC_START_POINT)
		{
			// "dVal1" holds the sweep angle, so we
			// should figure out the end point.

			/************************************************************************/		
			// get the end point.

			// the vector from center point to start point.
			Vector2D v1;
			v1.Magnitude(m_data[2]);
			v1.SetAngle(m_data[2]);

			// the start point.
			Point2D centerPt(m_data[0], m_data[1]);
			Point2D startPt = centerPt;
			startPt.Transfer(v1);

			// rotate the start point around the center point,
			// then can get end point.
			startPt.RotateBasedPt(centerPt, dVal1);
			/************************************************************************/

			dVal1 = startPt.X();
			dVal2 = startPt.Y();
		}
	}
}

BOOL ArcPatternEditAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	PatternEditActionBase::OnKeyDown(nChar, nRepCnt, nFlags);

	return TRUE;
}

BOOL ArcPatternEditAction::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	return TRUE;
}

BOOL ArcPatternEditAction::DrawBeforeFinish()
{
	CLOptionInfoPtr pCLOptionInfo = CLOptionInfo::GetInstance();
	DisplayParamPtr pColorInfo = pCLOptionInfo->GetDisplayParam();

	// keep the old color.
	COLORREF dwOldColor;
	m_pViewPort->GetDrawColor(dwOldColor);

	//
	m_pViewPort->SetDrawColor(*pColorInfo->GetPatColor());

	if (m_iStatus == DRAW_ARC_CENTER_POINT)  
	{
		m_pViewPort->DrawCircle2D(m_data[0], m_data[1], m_data[2]);

		// get start point of arc
		double dStartX = m_data[0] + m_data[2]*cos(m_data[3]);
		double dStartY = m_data[1] + m_data[2]*sin(m_data[3]);

		// draw line from start point to center point
		m_pViewPort->DrawLinePoint2D(dStartX, dStartY, m_data[0], m_data[1]);
	}
	// draw arc and line from current point to center point
	else if(m_iStatus == DRAW_ARC_START_POINT || m_editState != EDIT_ARC_NOTSTART)  
	{
		m_pViewPort->DrawArc2D(m_data[0], m_data[1], m_data[2], m_data[3], m_data[4]);

		// get end point of arc
		double dEndX = m_data[0] + m_data[2] * cos(m_data[3] + m_data[4]);
		double dEndY = m_data[1] + m_data[2] * sin(m_data[3] + m_data[4]);

		// draw line from end point to center point
		m_pViewPort->DrawLinePoint2D(dEndX, dEndY, m_data[0], m_data[1]);
	}

	// draw cursor.
	CursorDrawer::DrawCrossLineCursor(m_pViewPort);

	// restore the draw color
	m_pViewPort->SetDrawColor(dwOldColor);

	return TRUE;
}

void ArcPatternEditAction::UpdateForUndoRedo()
{
	PatternListPtr pPatList = m_pPartCadData->GetPatternList();
	if (m_editState != EDIT_ARC_NOTSTART && !pPatList->GetPatternFromID(m_pGripEditPat->GetID()))
	{
		PostMessage2EnterPatternSelectAction();
	}
}

void ArcPatternEditAction::SetGripEditPattern(IPatternPtr pPattern)
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

void ArcPatternEditAction::SetGripEditPt(double dPtX, double dPtY)
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

void ArcPatternEditAction::ChangeMidPoint(double midPtX, double midPtY)
{
	double dX1 = m_data[0] + m_data[2]*cos(m_data[3]);
	double dY1 = m_data[1] + m_data[2]*sin(m_data[3]);

	double dX2 = midPtX;
	double dY2 = midPtY;

	double dX3 = m_data[0] + m_data[2]*cos(m_data[3] + m_data[4]);
	double dY3 = m_data[1] + m_data[2]*sin(m_data[3] + m_data[4]);

	double dArea = fabs(((dX1-dX2)*(dY3-dY1)+(dY1-dY2)*(dX1-dX3))/2);
	if (dArea != 0)
	{
		double dX32 = (dX3 - dX2);
		double dX12 = (dX1 - dX2);
		double dY12 = (dY1 - dY2);
		double dY32 = (dY3 - dY2);

		double dC = dY12*dX32 - dY32*dX12;
		double dA = (dY1*dY1 - dY2*dY2) + (dX1*dX1 - dX2*dX2);
		double dB = (dY3*dY3 - dY2*dY2) + (dX3*dX3 - dX2*dX2);

		double dCenterX, dCenterY;
		dCenterY = (dA*dX32 - dB*dX12) / dC / 2;
		if (dX32 != 0)
			dCenterX = (-2*dY32*dCenterY + dB) / dX32 / 2;
		else
			dCenterX = (-2*dY12*dCenterY + dA) / dX12 / 2;

		m_data[0] = dCenterX;
		m_data[1] = dCenterY;
		m_data[2] = sqrt((dCenterX - dX1)*(dCenterX - dX1) + (dCenterY - dY1)*(dCenterY - dY1));

		double dStartAngle = atan2(dY1 - dCenterY,dX1 - dCenterX);
		double dMidAngle = atan2(dY2 - dCenterY,dX2 - dCenterX);
		double dEndAngle = atan2(dY3 - dCenterY,dX3 - dCenterX);
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

#include "StdAfx.h"
#include "RoundCornerHolePatternEditAction.h"

#include "Point2D.h"
#include "GlViewPort.h"
#include "ICommand.h"
#include "CommandManager.h"
#include "ValueInputManager.h"
#include "clUILibResource.h"
#include "MathEx.h"
#include "CursorDrawer.h"
#include "Point2DList.h"

#include "CLOptionInfo.h"
#include "DisplayParam.h"

#include "PatternSelectAction.h"
#include "PatternList.h"
#include "PatternAddCommand.h"
#include "GripEditCommand.h"
#include "RoundCornerHolePattern.h"
#include "clGeometryFeatureResource.h"
#include "PatternPropertyBar.h"
#include "PartCadData.h"

BEGIN_CUTLEADER_NAMESPACE()

RoundCornerHolePatternEditAction::RoundCornerHolePatternEditAction(PartCadDataPtr pPartCadData, GlViewPortPtr pViewPort)
{
	m_pPartCadData = pPartCadData;
	m_pViewPort = pViewPort;

	m_drawState = DRAW_RCRECT_NOTSTART;
	m_editState = EDIT_RCRECT_NOTSTART;
	memset(m_data, 0, sizeof(double)*6);

	m_iMsgID = IDS_OUTPUT_RCHOLE_CENTERPT;
}

RoundCornerHolePatternEditAction::~RoundCornerHolePatternEditAction(void)
{
}

BOOL RoundCornerHolePatternEditAction::LButtonDown(double& dPtX, double& dPtY)
{
	PatternListPtr pPatList = m_pPartCadData->GetPatternList();
	PatternLoopListPtr pPatternLoopList = m_pPartCadData->GetPatternLoopList();
	LoopTopologyItemListPtr pLoopTopologyItems = m_pPartCadData->GetLoopTopologyItems_FW();

	if (m_editState != EDIT_RCRECT_NOTSTART) //EDIT mode
	{
		if (m_editState == EDIT_RCRECT_CENTER_POINT)
		{
			m_data[0] = dPtX;
			m_data[1] = dPtY;
		}
		else if (m_editState == EDIT_RCRECT_TOP_POINT || m_editState == EDIT_RCRECT_BOTTOM_POINT)
		{
			m_data[3] = 2*sqrt((dPtY - m_data[1])*(dPtY - m_data[1]) + (dPtX - m_data[0])*(dPtX - m_data[0]));
			if (m_data[3] < 2*m_data[4])
				m_data[3] = 2*m_data[4];
		}
		else if (m_editState == EDIT_RCRECT_LEFT_POINT || m_editState == EDIT_RCRECT_RIGHT_POINT)
		{
			m_data[2] = 2 * sqrt((dPtY - m_data[1])*(dPtY - m_data[1]) + (dPtX - m_data[0])*(dPtX - m_data[0]));
			if (m_data[2] < 2 * m_data[4])
				m_data[2] = 2 * m_data[4];
		}
		else if (m_editState == EDIT_RCRECT_CORNER_POINT)
		{
			double dPtX2 = m_data[0] - (m_data[2] / 2);
			double dPtY2 = m_data[1] + (m_data[3] / 2);

			double dNewPtX = 0.0, dNewPtY = 0.0;
			MathEx::RotatePoint(m_data[0], m_data[1], -m_data[5], dPtX, dPtY, dNewPtX, dNewPtY);

			double dRadiusX = dNewPtX - dPtX2;
			double dRadiusY = dPtY2 - dNewPtY;
			double dRadius = 0.0001;

			if ((dNewPtX <= dPtX2) || (dPtY2 <= dNewPtY))
			{
				m_data[4] = dRadius;
			}
			else
			{
				if (dRadiusX < dRadiusY)
					m_data[4] = dRadiusX;
				else
					m_data[4] = dRadiusY;

				if ((m_data[3]/2) < m_data[4])
					m_data[4] = m_data[3]/2;

				if ((m_data[2]/2) < m_data[4])
					m_data[4] = m_data[2]/2;
			}    
		}

		// create the new round corner hole pattern with the new data.
		Point2D basePt(m_data[0], m_data[1]);
		IPatternPtr pRCRectPat(new RoundCornerHolePattern(basePt, m_data[2], m_data[3], m_data[4],  m_data[5]));

		// create command
		EditorData editorData = GetEditorData();
		ICommandPtr pCommand(new GripEditCommand(editorData, pRCRectPat, pPatternLoopList, pLoopTopologyItems, m_pGripEditPat));
		pCommand->Do();
		CommandManager::GetInstance()->AddCommand(pCommand);

		// enter the selecting pattern action.
		PostMessage2EnterPatternSelectAction(m_pGripEditPat.get());
	}
	else
	{
		if (m_drawState == DRAW_RCRECT_NOTSTART || 
			m_drawState == DRAW_RCRECT_THIRD_POINT) //first click
		{
			m_drawState = DRAW_RCRECT_FIRST_POINT;
			m_iMsgID = IDS_OUTPUT_RCHOLE_LENGTH_WIDTH;

			m_data[0] = dPtX;
			m_data[1] = dPtY;
			m_data[2] = 0;
			m_data[3] = 0;
			m_data[4] = 0;
			m_data[5] = 0;
		}
		else if(m_drawState == DRAW_RCRECT_FIRST_POINT)
		{
			m_drawState = DRAW_RCRECT_SECOND_POINT;
			m_iMsgID = IDS_OUTPUT_RCHOLE_RADIUS;

			m_data[2] = 2 * fabs(dPtX - m_data[0]);
			m_data[3] = 2 * fabs(dPtY - m_data[1]);
		}
		else if(m_drawState == DRAW_RCRECT_SECOND_POINT)
		{
			m_drawState = DRAW_RCRECT_THIRD_POINT;
			m_iMsgID = IDS_OUTPUT_RCHOLE_CENTERPT;

			double dRadius = sqrt((dPtX - m_data[0])*(dPtX - m_data[0]) + (dPtY - m_data[1])*(dPtY - m_data[1]));
			double dMinSize = (m_data[3] < m_data[2]) ? m_data[3] : m_data[2];
			if (dRadius > dMinSize/2.0 ) 
				dRadius = dMinSize/2.0;

			m_data[4] = dRadius;

			// create round corner hole pattern.
			Point2D basePt(m_data[0], m_data[1]);
			IPatternPtr pRCRectPat(new RoundCornerHolePattern(basePt, m_data[2], m_data[3], m_data[4], m_data[5]));

			// create command
			EditorData editorData = GetEditorData();
			ICommandPtr pCommand(new PatternAddCommand(editorData, pPatList, pPatternLoopList, pLoopTopologyItems, pRCRectPat));
			pCommand->Do();
			CommandManager::GetInstance()->AddCommand(pCommand);

			// enter the selecting pattern action.
			PostMessage2EnterPatternSelectAction(pRCRectPat.get());
		}
	}

	return TRUE;
}

BOOL RoundCornerHolePatternEditAction::MovePoint(double& dPtX, double& dPtY)
{
	if (m_editState != EDIT_RCRECT_NOTSTART) //EDIT mode
	{
		switch(m_editState) 
		{
		case EDIT_RCRECT_CENTER_POINT:
			{
				m_data[0] = dPtX;
				m_data[1] = dPtY;
			}
			break;
		case EDIT_RCRECT_TOP_POINT:
		case EDIT_RCRECT_BOTTOM_POINT :
			{
				m_data[3] = 2*sqrt((dPtY - m_data[1])*(dPtY - m_data[1]) + 
								   (dPtX - m_data[0])*(dPtX - m_data[0]));
				if (m_data[3] < 2*m_data[4])
				{
					m_data[3] = 2*m_data[4];
				}
			}
			break;
		case EDIT_RCRECT_LEFT_POINT:
		case EDIT_RCRECT_RIGHT_POINT :
			{
				m_data[2] = 2 * sqrt((dPtY - m_data[1])*(dPtY - m_data[1]) + 
									 (dPtX - m_data[0])*(dPtX - m_data[0]));
				if (m_data[2] < 2 * m_data[4])
					m_data[2] = 2 * m_data[4];
			}
			break;
		case EDIT_RCRECT_CORNER_POINT :
			{
				double dPtX2 = m_data[0] - (m_data[2] / 2);
				double dPtY2 = m_data[1] + (m_data[3] / 2);

				double dNewPtX = 0.0;
				double dNewPtY = 0.0;

				MathEx::RotatePoint(m_data[0], m_data[1],
									-m_data[5], dPtX, dPtY,
									dNewPtX, dNewPtY);

				double dRadiusX = dNewPtX - dPtX2;
				double dRadiusY = dPtY2 - dNewPtY;
				double dRadius = 0.0001;

				if ((dNewPtX <= dPtX2) || (dPtY2 <= dNewPtY))
				{
					m_data[4] = dRadius;
				}
				else
				{
					if (dRadiusX < dRadiusY)
						m_data[4] = dRadiusX;
					else
						m_data[4] = dRadiusY;

					if ((m_data[3]/2) < m_data[4])
						m_data[4] = m_data[3]/2;

					if ((m_data[2]/2) < m_data[4])
						m_data[4] = m_data[2]/2;
				}            
			}
			break;

		default:
			break;
		}
	}
	else
	{
		if (m_drawState == DRAW_RCRECT_FIRST_POINT)
		{
			m_data[2] = 2 * fabs(dPtX - m_data[0]);
			m_data[3] = 2 * fabs(dPtY - m_data[1]);
		}
		else if(m_drawState == DRAW_RCRECT_SECOND_POINT)
		{
			double dRadius = sqrt((dPtX - m_data[0])*(dPtX - m_data[0])+
								 (dPtY - m_data[1])*(dPtY - m_data[1]));

			double dMinSize = (m_data[3] < m_data[2]) ? m_data[3] : m_data[2];
			if (dRadius > dMinSize/2.0 ) 
				dRadius = dMinSize/2.0;

			m_data[4] = dRadius;
		}
	}

	return TRUE;
}

void RoundCornerHolePatternEditAction::ConvertKBInput(double& dVal1, double& dVal2)
{
	if (m_editState != EDIT_RCRECT_NOTSTART)
	{
		// under "edit" action.
		
		if (m_editState == EDIT_RCRECT_CENTER_POINT)
		{
			// now, do not need to convert.
			;
		}
		else if (m_editState == EDIT_RCRECT_TOP_POINT ||
				 m_editState == EDIT_RCRECT_BOTTOM_POINT)
		{
			// now, "dVal1" holds the width.

			Point2D centerPt(m_data[0], m_data[1]);
			centerPt.Y(centerPt.Y()+dVal1/2);

			dVal1 = centerPt.X();
			dVal2 = centerPt.Y();
		}
		else if (m_editState == EDIT_RCRECT_LEFT_POINT ||
				 m_editState == EDIT_RCRECT_RIGHT_POINT)
		{
			// now, "dVal1" holds the length.

			Point2D centerPt(m_data[0], m_data[1]);
			centerPt.X(centerPt.X()+dVal1/2);

			dVal1 = centerPt.X();
			dVal2 = centerPt.Y();
		}
	}
	else
	{
		// under "draw" action.

		if (m_drawState == DRAW_RCRECT_NOTSTART || 
			m_drawState == DRAW_RCRECT_THIRD_POINT)
		{
			// now, do not need to convert.
			;
		}
		else if(m_drawState == DRAW_RCRECT_FIRST_POINT)
		{
			// now, "dVal1" holds the length and
			// the "dVal2" holds the width.
			
			Point2D centerPt(m_data[0], m_data[1]);
			centerPt.X(centerPt.X()+dVal1/2);
			centerPt.Y(centerPt.Y()+dVal2/2);

			dVal1 = centerPt.X();
			dVal2 = centerPt.Y();
		}
		else if(m_drawState == DRAW_RCRECT_SECOND_POINT)
		{
			// now, "dVal1" holds the corner dRadius

			Point2D centerPt(m_data[0], m_data[1]);
			centerPt.X(centerPt.X()+dVal1);

			dVal1 = centerPt.X();
			dVal2 = centerPt.Y();
		}
	}
}

BOOL RoundCornerHolePatternEditAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	PatternEditActionBase::OnKeyDown(nChar, nRepCnt, nFlags);

	return TRUE;
}

BOOL RoundCornerHolePatternEditAction::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	return TRUE;
}

BOOL RoundCornerHolePatternEditAction::DrawBeforeFinish()
{
	CLOptionInfoPtr pCLOptionInfo = CLOptionInfo::GetInstance();
	DisplayParamPtr pColorInfo = pCLOptionInfo->GetDisplayParam();

	// keep the old color.
	COLORREF dwOldColor;
	m_pViewPort->GetDrawColor(dwOldColor);

	//
	m_pViewPort->SetDrawColor(*pColorInfo->GetPatColor());

	if (m_drawState == DRAW_RCRECT_FIRST_POINT)
	{
		// draw rectangle
		double dAngle1 = atan2(m_data[3]/2.0, m_data[2]/2.0);
		double dAngle = dAngle1 + m_data[5];
		double distance = sqrt(pow(m_data[3]/2.0, 2.0) + pow(m_data[2]/2.0, 2.0));

		// first corner
		double dPtX1 = m_data[0] + distance*cos(dAngle);
		double dPtY1 = m_data[1] + distance*sin(dAngle);

		// second corner
		dAngle = PI - dAngle1 + m_data[5];
		double dPtX2 = m_data[0] + distance*cos(dAngle);
		double dPtY2 = m_data[1] + distance*sin(dAngle);

		// third corner
		dAngle = dAngle1 + m_data[5] + PI;
		double dPtX3 = m_data[0] + distance*cos(dAngle);
		double dPtY3 = m_data[1] + distance*sin(dAngle);

		// fourth corner
		dAngle = -dAngle1 + m_data[5];
		double dPtX4 = m_data[0] + distance*cos(dAngle);
		double dPtY4 = m_data[1] + distance*sin(dAngle);

		m_pViewPort->DrawLinePoint2D(dPtX1, dPtY1, dPtX2, dPtY2);
		m_pViewPort->DrawLinePoint2D(dPtX2, dPtY2, dPtX3, dPtY3);
		m_pViewPort->DrawLinePoint2D(dPtX3, dPtY3, dPtX4, dPtY4);
		m_pViewPort->DrawLinePoint2D(dPtX4, dPtY4, dPtX1, dPtY1);
	}
	else if (m_drawState == DRAW_RCRECT_SECOND_POINT || m_editState != EDIT_RCRECT_NOTSTART)
	{
		// first corner
		double dAngle1 = atan2((m_data[3]/2.0 - m_data[4]), (m_data[2]/2.0 - m_data[4]));
		double dAngle = dAngle1 + m_data[5];
		double dDis = sqrt(pow(m_data[3]/2.0 - m_data[4], 2.0) + pow(m_data[2]/2.0 - m_data[4], 2.0));
		double dArcCenPtX1 = m_data[0] + dDis*cos(dAngle);
		double dArcCenPtY1 = m_data[1] + dDis*sin(dAngle);
		m_pViewPort->DrawArc2D(dArcCenPtX1, dArcCenPtY1, m_data[4], m_data[5], PID2);

		double dArcStartX1 = dArcCenPtX1 + m_data[4] * cos(m_data[5]);
		double dArcStartY1 = dArcCenPtY1 + m_data[4] * sin(m_data[5]);
		double dArcEndX1 = dArcCenPtX1 + m_data[4] * cos(m_data[5] + PID2);
		double dArcEndY1 = dArcCenPtY1 + m_data[4] * sin(m_data[5] + PID2);

		// second Corner
		dAngle = PI - dAngle1 + m_data[5];
		double dArcCenPtX2 = m_data[0] + dDis*cos(dAngle);
		double dArcCenPtY2 = m_data[1] + dDis*sin(dAngle);
		m_pViewPort->DrawArc2D(dArcCenPtX2, dArcCenPtY2, m_data[4], m_data[5] + PID2, PID2);

		double dArcStartX2 = dArcCenPtX2 + m_data[4] * cos(m_data[5] + PID2);
		double dArcStartY2 = dArcCenPtY2 + m_data[4] * sin(m_data[5] + PID2);
		double dArcEndX2 = dArcCenPtX2 + m_data[4] * cos(m_data[5] + PI);
		double dArcEndY2 = dArcCenPtY2 + m_data[4] * sin(m_data[5] + PI);

		// third corner
		dAngle = dAngle1 + m_data[5] + PI;
		double dArcCenPtX3 = m_data[0] + dDis*cos(dAngle);
		double dArcCenPtY3 = m_data[1] + dDis*sin(dAngle);
		m_pViewPort->DrawArc2D(dArcCenPtX3, dArcCenPtY3, m_data[4], m_data[5] + PI, PID2);

		double dArcStartX3 = dArcCenPtX3 + m_data[4] * cos(m_data[5] + PI);
		double dArcStartY3 = dArcCenPtY3 + m_data[4] * sin(m_data[5] + PI);
		double dArcEndX3 = dArcCenPtX3 + m_data[4] * cos(m_data[5] - PID2);
		double dArcEndY3 = dArcCenPtY3 + m_data[4] * sin(m_data[5] - PID2);

		// fourth corner
		dAngle = -dAngle1 + m_data[5];
		double dArcCenPtX4 = m_data[0] + dDis*cos(dAngle);
		double dArcCenPtY4 = m_data[1] + dDis*sin(dAngle);
		m_pViewPort->DrawArc2D(dArcCenPtX4, dArcCenPtY4, m_data[4], m_data[5] - PID2, PID2);

		double dArcStartX4 = dArcCenPtX4 + m_data[4] * cos(m_data[5] - PID2);
		double dArcStartY4 = dArcCenPtY4 + m_data[4] * sin(m_data[5] - PID2);
		double dArcEndX4 = dArcCenPtX4 + m_data[4] * cos(m_data[5]);
		double dArcEndY4 = dArcCenPtY4 + m_data[4] * sin(m_data[5]);

		// draw four lines
		m_pViewPort->DrawLinePoint2D(dArcEndX1, dArcEndY1, dArcStartX2, dArcStartY2);
		m_pViewPort->DrawLinePoint2D(dArcEndX2, dArcEndY2, dArcStartX3, dArcStartY3);
		m_pViewPort->DrawLinePoint2D(dArcEndX3, dArcEndY3, dArcStartX4, dArcStartY4);
		m_pViewPort->DrawLinePoint2D(dArcEndX4, dArcEndY4, dArcStartX1, dArcStartY1);
	}

	// restore the draw color
	m_pViewPort->SetDrawColor(dwOldColor);

	// draw cursor.
	CursorDrawer::DrawCrossLineCursor(m_pViewPort);

	return TRUE;
}

void RoundCornerHolePatternEditAction::UpdateForUndoRedo()
{
	PostMessage2EnterPatternSelectAction();
}

void RoundCornerHolePatternEditAction::SetGripEditPattern(IPatternPtr pPattern)
{
	m_pGripEditPat = pPattern;
	RoundCornerHolePatternPtr pRCRectDPat = boost::dynamic_pointer_cast<RoundCornerHolePattern>(pPattern);

	// fill the m_data with the data in round corner hole pattern.
	Point2D basePt = pRCRectDPat->GetBasePoint();
	m_data[0] = basePt.X();
	m_data[1] = basePt.Y();

	m_data[2] =	pRCRectDPat->GetLength();
	m_data[3] =	pRCRectDPat->GetWidth();
	m_data[4] =	pRCRectDPat->GetArcRadius();
	m_data[5] =	pRCRectDPat->GetAngle();
}

void RoundCornerHolePatternEditAction::SetGripEditPt(double dPtX, double dPtY)
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
				// Modify center point
				m_editState = EDIT_RCRECT_CENTER_POINT;
				m_iMsgID = IDS_OUTPUT_RCHOLE_CENTERPT;
			}
			else if (i == 1)
			{
				m_editState = EDIT_RCRECT_LEFT_POINT;
				m_iMsgID = IDS_OUTPUT_RCHOLE_LENGTH;
			}
			else if (i == 2)
			{
				m_editState = EDIT_RCRECT_BOTTOM_POINT;
				m_iMsgID = IDS_OUTPUT_RCHOLE_WIDTH;
			}
			else if (i == 3)
			{
				m_editState = EDIT_RCRECT_RIGHT_POINT;
				m_iMsgID = IDS_OUTPUT_RCHOLE_LENGTH;
			}
			else if (i == 4)
			{
				m_editState = EDIT_RCRECT_TOP_POINT;
				m_iMsgID = IDS_OUTPUT_RCHOLE_WIDTH;
			}
			else if (i == 5)
			{
				// dRadius center of top left
				m_editState = EDIT_RCRECT_CORNER_POINT;
				m_iMsgID = IDS_OUTPUT_RCHOLE_RADIUS;
			}

			return;
		}
	}
}

END_CUTLEADER_NAMESPACE()

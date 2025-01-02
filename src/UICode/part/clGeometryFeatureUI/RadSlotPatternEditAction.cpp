#include "StdAfx.h"
#include "RadSlotPatternEditAction.h"

#include "baseConst.h"
#include "GlViewPort.h"
#include "ValueInputManager.h"
#include "clUILibResource.h"
#include "CommandManager.h"
#include "ICommand.h"
#include "CursorDrawer.h"
#include "Point2DList.h"

#include "CLOptionInfo.h"
#include "DisplayParam.h"

#include "PatternSelectAction.h"
#include "PatternAddCommand.h"
#include "GripEditCommand.h"
#include "PatternList.h"
#include "RadSlotPattern.h"
#include "clGeometryFeatureResource.h"
#include "PatternPropertyBar.h"
#include "PartCadData.h"

BEGIN_CUTLEADER_NAMESPACE()

RadSlotPatternEditAction::RadSlotPatternEditAction(PartCadDataPtr pPartCadData, GlViewPortPtr pViewPort)
{
	m_drawStatus = DRAW_RADSLOT_NOTSTART;
	m_editState = EDIT_RADSLOT_NOTSTART;
	memset(m_data, 0, sizeof(double)*6);

	m_pPartCadData = pPartCadData;
	m_pViewPort = pViewPort;

	m_iMsgID = IDS_OUTPUT_RADSLOT_CENTERPT;
}

RadSlotPatternEditAction::~RadSlotPatternEditAction(void)
{
}

void RadSlotPatternEditAction::CheckArcDir(double& startAng, double& dEndAng, double& lastAng)
{
	if (lastAng < startAng) 
	{
		if (fabs(dEndAng - lastAng) >= fabs(dEndAng- lastAng - 2*PI))
			dEndAng -= 2*PI;
	}
	else
		if (fabs(dEndAng - lastAng) >= fabs(dEndAng- lastAng + 2*PI))
			dEndAng += 2*PI;

	if ((dEndAng - startAng) >= 2*PI)
		dEndAng -= 2*PI;
	else
		if ((startAng - dEndAng) >= 2*PI)
			dEndAng += 2*PI;

	if (fabs(dEndAng - lastAng) >= 2*PI) 
	{
		if (lastAng > 0 && dEndAng < 0)
			dEndAng += 2*PI;
		else
			if (lastAng < 0 && dEndAng > 0)
				dEndAng -= 2*PI;
	}

	lastAng = dEndAng;
}

BOOL RadSlotPatternEditAction::LButtonDown(double& dPtX, double& dPtY)
{
	PatternListPtr pPatList = m_pPartCadData->GetPatternList();
	PatternLoopListPtr pPatternLoopList = m_pPartCadData->GetPatternLoopList();
	LoopTopologyItemListPtr pLoopTopologyItems = m_pPartCadData->GetLoopTopologyItems_FW();

	if (m_editState != EDIT_RADSLOT_NOTSTART)
	{
		// create the new radius slot pattern with the new data.
		Point2D centerPt(m_data[0], m_data[1]);
		IPatternPtr pRadSlotPat(new RadSlotPattern(centerPt, m_data[2], m_data[4], m_data[5], m_data[3]));

		// create command
		EditorData editorData = GetEditorData();
		ICommandPtr pCommand(new GripEditCommand(editorData, pRadSlotPat, pPatternLoopList, pLoopTopologyItems, m_pGripEditPat));
		pCommand->Do();
		CommandManager::GetInstance()->AddCommand(pCommand);

		// enter the selecting pattern action.
		PostMessage2EnterPatternSelectAction(m_pGripEditPat.get());
	}
	else
	{
		if (m_drawStatus == DRAW_RADSLOT_NOTSTART || m_drawStatus == DRAW_RADSLOT_FOURTH_POINT)
		{
			m_drawStatus = DRAW_RADSLOT_FIRST_POINT;
			m_iMsgID = IDS_OUTPUT_RADSLOT_RAD_ANGLE;

			m_data[0] = dPtX;
			m_data[1] = dPtY;
			m_data[2] = 0;
			m_data[3] = 0;
			m_data[4] = 0;
			m_data[5] = 0;
		}
		else if (m_drawStatus == DRAW_RADSLOT_FIRST_POINT)
		{
			m_drawStatus = DRAW_RADSLOT_SECOND_POINT;
			m_iMsgID = IDS_OUTPUT_RADSLOT_ENDANGLE;

			m_data[2] = sqrt((dPtX - m_data[0])*(dPtX - m_data[0]) + (dPtY - m_data[1])*(dPtY - m_data[1]));
			m_data[4] = atan2(dPtY - m_data[1], dPtX - m_data[0]);

			m_dLastAng = m_data[4];
		}
		else if (m_drawStatus == DRAW_RADSLOT_SECOND_POINT)
		{
			m_drawStatus = DRAW_RADSLOT_THIRD_POINT;
			m_iMsgID = IDS_OUTPUT_RADSLOT_WIDTH;

			double dEndAng = atan2(dPtY - m_data[1], dPtX - m_data[0]);
			CheckArcDir(m_data[4], dEndAng, m_dLastAng);

			double dMoveAng = dEndAng - m_data[4];
			m_data[5] = dMoveAng;
		}
		else if (m_drawStatus == DRAW_RADSLOT_THIRD_POINT)
		{
			m_drawStatus = DRAW_RADSLOT_FOURTH_POINT;
			m_iMsgID = IDS_OUTPUT_RADSLOT_CENTERPT;

			double dDis = sqrt((dPtX - m_data[0])*(dPtX - m_data[0]) + (dPtY - m_data[1])*(dPtY - m_data[1]));

			double dWidth = 0.0;
			if (dDis > 2.0 * m_data[2])
				dWidth = m_data[2] * 2.0;
			else
				dWidth = fabs(dDis - m_data[2]) * 2.0;

			m_data[3] = dWidth;

			// create radius slot pattern.
			Point2D centerPt(m_data[0], m_data[1]);
			IPatternPtr pRadSlotPat(new RadSlotPattern(centerPt, m_data[2], m_data[4], m_data[5], m_data[3]));

			// create command
			EditorData editorData = GetEditorData();
			ICommandPtr pCommand(new PatternAddCommand(editorData, pPatList, pPatternLoopList, pLoopTopologyItems, pRadSlotPat));
			pCommand->Do();
			CommandManager::GetInstance()->AddCommand(pCommand);

			// enter the selecting pattern action.
			PostMessage2EnterPatternSelectAction(pRadSlotPat.get());
		}
	}

	return TRUE;
}

BOOL RadSlotPatternEditAction::MovePoint(double& dPtX, double& dPtY)
{
	if (m_editState != EDIT_RADSLOT_NOTSTART) //EDIT mode
	{
		if (m_editState == EDIT_RADSLOT_CENTER_POINT)
		{
			m_data[0] = dPtX;
			m_data[1] = dPtY;
		}
		else if (m_editState == EDIT_RADSLOT_START_POINT)
		{
			// calculate the start angle
			double dStartAngle = atan2(dPtY - m_data[1], dPtX - m_data[0]);

			m_data[4] = dStartAngle;
		}
		else if (m_editState == EDIT_RADSLOT_MID_POINT)
		{
			// calculate the radius
			m_data[2] = sqrt((dPtX-m_data[0])*(dPtX-m_data[0]) + 
							 (dPtY-m_data[1])*(dPtY-m_data[1]));
		}
		else if (m_editState == EDIT_RADSLOT_END_POINT)
		{
			// calculate the end angle
			double dEndAng = atan2(dPtY - m_data[1], dPtX - m_data[0]);
			CheckArcDir(m_data[4], dEndAng, m_dLastAng);
			m_data[5] = dEndAng - m_data[4];
		}
		else if (m_editState == EDIT_RADSLOT_WIDTH_POINT)
		{
			// change the dWidth of the radius slot.

			double dDis = sqrt((dPtX - m_data[0])*(dPtX - m_data[0])+
							   (dPtY - m_data[1])*(dPtY - m_data[1]));

			if (dDis > 2.0 * m_data[2])
				m_data[3] = m_data[2] * 2.0;
			else
				m_data[3] = fabs(dDis - m_data[2]) * 2.0;
		}
	}
	else
	{
		if (m_drawStatus == DRAW_RADSLOT_FIRST_POINT)
		{
			m_data[2] = sqrt((dPtX - m_data[0])*(dPtX - m_data[0])+
							 (dPtY - m_data[1])*(dPtY - m_data[1]));

			m_data[4] = atan2(dPtY - m_data[1], dPtX - m_data[0]);
		}
		else if (m_drawStatus == DRAW_RADSLOT_SECOND_POINT)
		{
			double dEndAng = atan2(dPtY - m_data[1], dPtX - m_data[0]);
			CheckArcDir(m_data[4], dEndAng, m_dLastAng);
			m_data[5] = dEndAng - m_data[4];
		}
		else if (m_drawStatus == DRAW_RADSLOT_THIRD_POINT)
		{
			double dDis = sqrt((dPtX - m_data[0])*(dPtX - m_data[0])+
							   (dPtY - m_data[1])*(dPtY - m_data[1]));

			double dWidth = 0.0;
			if (dDis > 2.0 * m_data[2])
				dWidth = m_data[2] * 2.0;
			else
				dWidth = fabs(dDis - m_data[2]) * 2.0;                    

			m_data[3] = dWidth;
		}
	}

	return TRUE;
}

void RadSlotPatternEditAction::ConvertKBInput(double& dVal1, double& dVal2)
{
	if (m_editState != EDIT_RADSLOT_NOTSTART)
	{
		// under "edit" action.

		if (m_editState == EDIT_RADSLOT_CENTER_POINT)
		{
			// now, do not need to convert.
			;
		}
		else if(m_editState == EDIT_RADSLOT_START_POINT)
		{
			// now, "dVal1" holds the start angle.

			Vector2D v1;
			v1.SetAngle(dVal1);

			Point2D centerPt(m_data[0], m_data[1]);
			centerPt.Transfer(v1);

			dVal1 = centerPt.X();
			dVal2 = centerPt.Y();
		}
		else if(m_editState == EDIT_RADSLOT_MID_POINT)
		{
			// now, "dVal1" holds the radius.

			Vector2D v1;
			v1.Magnitude(dVal1);

			Point2D centerPt(m_data[0], m_data[1]);
			centerPt.Transfer(v1);

			dVal1 = centerPt.X();
			dVal2 = centerPt.Y();
		}
		else if(m_editState == EDIT_RADSLOT_END_POINT)
		{
			// now, "dVal1" holds the move angle.

			Vector2D v1;
			v1.SetAngle(m_data[4]);
			v1.Rotate(dVal1);

			Point2D centerPt(m_data[0], m_data[1]);
			centerPt.Transfer(v1);

			dVal1 = centerPt.X();
			dVal2 = centerPt.Y();
		}
		else if (m_editState == EDIT_RADSLOT_WIDTH_POINT)
		{
			// now, "dVal1" holds the dWidth.

			double d1 = dVal1/2;

			Vector2D v1;
			v1.Magnitude(m_data[2] - d1);

			Point2D centerPt(m_data[0], m_data[1]);
			centerPt.Transfer(v1);

			dVal1 = centerPt.X();
			dVal2 = centerPt.Y();
		}
	}
	else
	{
		// under "draw" action.

		if (m_drawStatus == DRAW_RADSLOT_NOTSTART || 
			m_drawStatus == DRAW_RADSLOT_FOURTH_POINT)
		{
			// now, do not need to convert.
			;
		}
		else if(m_drawStatus == DRAW_RADSLOT_FIRST_POINT)
		{
			// now, "dVal1" holds the radius
			// now, "dVal2" holds the start angle

			Vector2D v1;
			v1.SetAngle(dVal2);
			v1.Magnitude(dVal1);

			Point2D centerPt(m_data[0], m_data[1]);
			centerPt.Transfer(v1);

			dVal1 = centerPt.X();
			dVal2 = centerPt.Y();
		}
		else if(m_drawStatus == DRAW_RADSLOT_SECOND_POINT)
		{
			// now, "dVal1" holds the move angle.

			Vector2D v1;
			v1.SetAngle(m_data[4]);
			v1.Rotate(dVal1);

			Point2D centerPt(m_data[0], m_data[1]);
			centerPt.Transfer(v1);

			dVal1 = centerPt.X();
			dVal2 = centerPt.Y();
		}
		else if(m_drawStatus == DRAW_RADSLOT_THIRD_POINT)
		{
			// now, "dVal1" holds the dWidth.

			double d1 = dVal1/2;

			Vector2D v1;
			v1.Magnitude(m_data[2] - d1);

			Point2D centerPt(m_data[0], m_data[1]);
			centerPt.Transfer(v1);

			dVal1 = centerPt.X();
			dVal2 = centerPt.Y();
		}
	}
}

BOOL RadSlotPatternEditAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	PatternEditActionBase::OnKeyDown(nChar, nRepCnt, nFlags);

	return TRUE;
}

BOOL RadSlotPatternEditAction::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	return TRUE;
}

BOOL RadSlotPatternEditAction::DrawBeforeFinish()
{
	CLOptionInfoPtr pCLOptionInfo = CLOptionInfo::GetInstance();
	DisplayParamPtr pColorInfo = pCLOptionInfo->GetDisplayParam();

	// keep the old color.
	COLORREF dwOldColor;
	m_pViewPort->GetDrawColor(dwOldColor);

	//
	m_pViewPort->SetDrawColor(*pColorInfo->GetPatColor());

	// under this condition, we can not draw.
	if ((m_drawStatus == DRAW_RADSLOT_NOTSTART || m_drawStatus == DRAW_RADSLOT_FOURTH_POINT) &&
		m_editState == EDIT_RADSLOT_NOTSTART)
	{
		;
	}
	else if ((m_data[5] == 0) && (m_editState == EDIT_RADSLOT_NOTSTART))
	{
		// Draw circle and line
		double dStartPtX = m_data[0] + m_data[2] * cos(m_data[4]);
		double dStartPtY = m_data[1] + m_data[2] * sin(m_data[4]);

		m_pViewPort->DrawLinePoint2D(m_data[0], m_data[1], dStartPtX, dStartPtY);
		m_pViewPort->DrawCircle2D(m_data[0], m_data[1], m_data[2]);
	}
	else
	{
		double dStartAng = 0, dMoveAng = 0;
		double dEndAng = m_data[4] +	m_data[5];

		double dWidth = m_data[3] /2.0;
		if (m_data[5] < 0)
		{
			dStartAng = dEndAng;
			dMoveAng = -m_data[5];
		}
		else
		{
			dStartAng = m_data[4];
			dMoveAng = m_data[5];
		}

		if (m_data[3] == 0)
		{
			// draw arc and line
			double dEndAng = m_data[4] + m_data[5];
			double dEndPtX = m_data[0] + m_data[2] * cos(dEndAng);
			double dEndPtY = m_data[1] + m_data[2] * sin(dEndAng);

			m_pViewPort->DrawLinePoint2D(m_data[0], m_data[1], dEndPtX, dEndPtY);
			m_pViewPort->DrawArc2D(m_data[0], m_data[1], m_data[2], dStartAng, dMoveAng);
		}
		else
		{
			double dStartAng1 = m_data[4]; 
			double dMoveAng1 = m_data[5];
			double dEndAng1 = dStartAng1 + dMoveAng1;
			double dWidth1 = m_data[3];
			double dCenterX = m_data[0];
			double dCenterY = m_data[1];
			double dRadius = m_data[2];

			m_pViewPort->DrawArc2D(dCenterX, dCenterY, dRadius - dWidth1/2.0, dStartAng1, dMoveAng1);
			m_pViewPort->DrawArc2D(dCenterX, dCenterY, dRadius + dWidth1/2.0, dStartAng1, dMoveAng1);

			double dTempPtX = dCenterX + dRadius * cos(dStartAng1);
			double dTempPtY = dCenterY + dRadius * sin(dStartAng1);
			double dStartAngle2 = (dMoveAng1 > 0) ? dStartAng1 - PI : dStartAng1;
			m_pViewPort->DrawArc2D(dTempPtX, dTempPtY, dWidth1/2.0, dStartAngle2, PI);

			dTempPtX = dCenterX + dRadius * cos(dEndAng1);
			dTempPtY = dCenterY + dRadius * sin(dEndAng1);

			double dStartAng3 = (dMoveAng1 < 0) ? dEndAng1 + PI : dEndAng1;
			m_pViewPort->DrawArc2D(dTempPtX, dTempPtY, dWidth1/2.0, dStartAng3, PI);
		}
	}

	// restore the draw color
	m_pViewPort->SetDrawColor(dwOldColor);

	// draw cursor.
	CursorDrawer::DrawCrossLineCursor(m_pViewPort);

	return TRUE;
}

void RadSlotPatternEditAction::UpdateForUndoRedo()
{
	PatternListPtr pPatList = m_pPartCadData->GetPatternList();
	if (m_editState != EDIT_RADSLOT_NOTSTART && !pPatList->GetPatternFromID(m_pGripEditPat->GetID()))
	{
		PostMessage2EnterPatternSelectAction();
	}
}

void RadSlotPatternEditAction::SetGripEditPattern(IPatternPtr pPattern)
{
	m_pGripEditPat = pPattern;

	RadSlotPattern* pRadSlotPat = dynamic_cast<RadSlotPattern*>(pPattern.get());
	Point2D centerPt = pRadSlotPat->GetBasePoint();
	m_data[0] = centerPt.X();
	m_data[1] = centerPt.Y();
	m_data[2] = pRadSlotPat->GetRadius();
	m_data[3] = pRadSlotPat->GetRadius() * pRadSlotPat->GetWidthRatio();
	m_data[4] = pRadSlotPat->GetStartAngle();
	m_data[5] = pRadSlotPat->GetMoveAngle();
}

void RadSlotPatternEditAction::SetGripEditPt(double dPtX, double dPtY)
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
				m_editState = EDIT_RADSLOT_CENTER_POINT;
				m_iMsgID = IDS_OUTPUT_RADSLOT_CENTERPT;
			}
			else if (i == 1)
			{
				m_editState = EDIT_RADSLOT_START_POINT;
				m_iMsgID = IDS_OUTPUT_RADSLOT_STARTANGLE;
			}
			else if (i == 2)
			{
				m_editState = EDIT_RADSLOT_END_POINT;
				m_iMsgID = IDS_OUTPUT_RADSLOT_ENDANGLE;
			}
			else if (i == 3)
			{
				m_editState = EDIT_RADSLOT_WIDTH_POINT;
				m_iMsgID = IDS_OUTPUT_RADSLOT_WIDTH;
			}
			else if (i == 4)
			{
				m_editState = EDIT_RADSLOT_MID_POINT;
				m_iMsgID = IDS_OUTPUT_RADSLOT_RADIUS;
			}

			return;
		}
	}
}

END_CUTLEADER_NAMESPACE()

#include "StdAfx.h"
#include "OBroundPatternEditAction.h"

#include "MathEx.h"
#include "ICommand.h"
#include "GlViewPort.h"
#include "ValueInputManager.h"
#include "clUILibResource.h"
#include "CommandManager.h"
#include "CursorDrawer.h"

#include "CLOptionInfo.h"
#include "DisplayParam.h"

#include "PatternSelectAction.h"
#include "PatternAddCommand.h"
#include "GripEditCommand.h"
#include "OBroundPattern.h"
#include "PatternList.h"
#include "clGeometryFeatureResource.h"
#include "PatternPropertyBar.h"
#include "PartCadData.h"

BEGIN_CUTLEADER_NAMESPACE()

OBroundPatternEditAction::OBroundPatternEditAction(PartCadDataPtr pPartCadData, GlViewPortPtr pViewPort)
{
	m_drawStatus = DRAW_OBROUND_NOTSTART;
	m_editState = EDIT_OBROUND_NOTSTART;
	memset(m_data, 0, sizeof(double)*5);

	m_pPartCadData = pPartCadData;
	m_pViewPort = pViewPort;

	m_iMsgID = IDS_OUTPUT_OBROUND_CENTERPT;
}

OBroundPatternEditAction::~OBroundPatternEditAction(void)
{
}

BOOL OBroundPatternEditAction::LButtonDown(double& dPtX, double& dPtY)
{
	PatternListPtr pPatList = m_pPartCadData->GetPatternList();
	PatternLoopListPtr pPatternLoopList = m_pPartCadData->GetPatternLoopList();
	LoopTopologyItemListPtr pLoopTopologyItems = m_pPartCadData->GetLoopTopologyItems_FW();

	if (m_editState != EDIT_OBROUND_NOTSTART)
	{
		// create the new OBround pattern with the new data.
		Point2D centerPt(m_data[0], m_data[1]);
		IPatternPtr pOBroundPat(new OBroundPattern(centerPt, m_data[4], m_data[2], m_data[3]));

		// create command
		EditorData editorData = GetEditorData();
		ICommandPtr pCommand(new GripEditCommand(editorData, pOBroundPat, pPatternLoopList, pLoopTopologyItems, m_pGripEditPat));
		pCommand->Do();
		CommandManager::GetInstance()->AddCommand(pCommand);

		// enter the selecting pattern action.
		PostMessage2EnterPatternSelectAction(m_pGripEditPat.get());
	}
	else
	{
		if (m_drawStatus == DRAW_OBROUND_NOTSTART || m_drawStatus == DRAW_OBROUND_THIRD_POINT)
		{
			m_drawStatus = DRAW_OBROUND_FIRST_POINT;
			m_iMsgID = IDS_OUTPUT_OBROUND_ANGLE_LENGTH;

			m_data[0] = dPtX;
			m_data[1] = dPtY;
			m_data[2] = 0;
			m_data[3] = 0;
			m_data[4] = 0;
		}
		else if(m_drawStatus == DRAW_OBROUND_FIRST_POINT)
		{
			m_drawStatus = DRAW_OBROUND_SECOND_POINT;
			m_iMsgID = IDS_OUTPUT_OBROUND_WIDTH;

			// calculate the dLength
			m_data[2] = 2 * sqrt((dPtX-m_data[0])*(dPtX-m_data[0]) + (dPtY-m_data[1])*(dPtY-m_data[1]));

			// calculate the angle
			m_data[4] = atan2((dPtY-m_data[1]),	(dPtX-m_data[0]));

			// calculate the dWidth
			m_data[3] = m_data[2]/2;

		}
		else if(m_drawStatus == DRAW_OBROUND_SECOND_POINT)
		{
			m_drawStatus = DRAW_OBROUND_THIRD_POINT;
			m_iMsgID = IDS_OUTPUT_OBROUND_CENTERPT;

			// calculate the dWidth
			m_data[3] = sqrt((dPtX-m_data[0])*(dPtX-m_data[0]) + (dPtY-m_data[1])*(dPtY-m_data[1]));

			// create OBround pattern.
			Point2D centerPt(m_data[0], m_data[1]);
			IPatternPtr pOBroundPat(new OBroundPattern(centerPt, m_data[4], m_data[2], m_data[3]));

			// create command
			EditorData editorData = GetEditorData();
			ICommandPtr pCommand(new PatternAddCommand(editorData, pPatList, pPatternLoopList, pLoopTopologyItems, pOBroundPat));
			pCommand->Do();
			CommandManager::GetInstance()->AddCommand(pCommand);

			// enter the selecting pattern action.
			PostMessage2EnterPatternSelectAction(pOBroundPat.get());
		}
	}

	return TRUE;
}

BOOL OBroundPatternEditAction::MovePoint(double& dPtX, double& dPtY)
{
	if (m_editState != EDIT_OBROUND_NOTSTART) //EDIT mode
	{
		if (m_editState == EDIT_OBROUND_CENTER_POINT)
		{
			m_data[0] = dPtX;
			m_data[1] = dPtY;
		}
		else if(m_editState == EDIT_OBROUND_TOP_POINT ||
				m_editState == EDIT_OBROUND_BOTTOM_POINT)
		{
			// calculate the short radius
			m_data[3] = sqrt((dPtX-m_data[0])*(dPtX-m_data[0]) + (dPtY-m_data[1])*(dPtY-m_data[1]));

		}
		else if(m_editState == EDIT_OBROUND_LEFT_POINT || 
				m_editState == EDIT_OBROUND_RIGHT_POINT)
		{
			// calculate the long radius
			m_data[2] = sqrt((dPtX-m_data[0])*(dPtX-m_data[0]) + (dPtY-m_data[1])*(dPtY-m_data[1]));
		}
	}
	else
	{
		if (m_drawStatus == DRAW_OBROUND_FIRST_POINT)
		{
			// calculate the dLength.
			m_data[2] = 2 * sqrt((dPtX-m_data[0])*(dPtX-m_data[0]) + (dPtY-m_data[1])*(dPtY-m_data[1]));

			// calculate the angle
			m_data[4] = atan2((dPtY-m_data[1]),	(dPtX-m_data[0]));

		}
		else if(m_drawStatus == DRAW_OBROUND_SECOND_POINT)
		{
			// calculate the dWidth.
			m_data[3] = sqrt((dPtX-m_data[0])*(dPtX-m_data[0]) + (dPtY-m_data[1])*(dPtY-m_data[1]));
		}
	}

	return TRUE;
}

void OBroundPatternEditAction::ConvertKBInput(double& dVal1, double& dVal2)
{
	if (m_editState != EDIT_OBROUND_NOTSTART)
	{
		// under "edit" action.

		if (m_editState == EDIT_OBROUND_CENTER_POINT)
		{
			// now, do not need to convert.
			;
		}
		else if(m_editState == EDIT_OBROUND_TOP_POINT ||
				m_editState == EDIT_OBROUND_BOTTOM_POINT)
		{
			// now, "dVal1" holds the dWidth of the OBround
			
			Point2D centerPt(m_data[0], m_data[1]);

			// get the vector along the dWidth direction
			Vector2D v1;
			v1.SetAngle(m_data[4]);
			v1.Orthogonal();
			v1.Magnitude(dVal1);

			centerPt.Transfer(v1);

			dVal1 = centerPt.X();
			dVal2 = centerPt.Y();
		}
		else if(m_editState == EDIT_OBROUND_LEFT_POINT || 
				m_editState == EDIT_OBROUND_RIGHT_POINT)
		{
			// now, "dVal1" holds the angle of the OBround
			// now, "dVal2" holds the dLength of the OBround

			double d1 = dVal2/2;

			// the vector from the center point to the "right" snap point.
			Vector2D v1;
			v1.SetAngle(dVal1);
			v1.Magnitude(d1);

			Point2D centerPt(m_data[0], m_data[1]);
			centerPt.Transfer(v1);

			dVal1 = centerPt.X();
			dVal2 = centerPt.Y();
		}
	}
	else
	{
		// under "draw" action.

		if (m_drawStatus == DRAW_OBROUND_NOTSTART || 
			m_drawStatus == DRAW_OBROUND_THIRD_POINT)
		{
			// now, do not need to convert.
			;
		}
		else if(m_drawStatus == DRAW_OBROUND_FIRST_POINT)
		{
			// now, "dVal1" holds the angle of the OBround
			// now, "dVal2" holds the dLength of the OBround

			double d1 = dVal2/2;

			// the vector fron the center point to the "right" snap point.
			Vector2D v1;
			v1.SetAngle(dVal1);
			v1.Magnitude(d1);

			Point2D centerPt(m_data[0], m_data[1]);
			centerPt.Transfer(v1);

			dVal1 = centerPt.X();
			dVal2 = centerPt.Y();
		}
		else if(m_drawStatus == DRAW_OBROUND_SECOND_POINT)
		{
			// now, "dVal1" holds the dWidth of the OBround

			Point2D centerPt(m_data[0], m_data[1]);

			// get the vector along the dWidth direction
			Vector2D v1;
			v1.SetAngle(m_data[4]);
			v1.Orthogonal();
			v1.Magnitude(dVal1);

			centerPt.Transfer(v1);

			dVal1 = centerPt.X();
			dVal2 = centerPt.Y();
		}
	}
}

BOOL OBroundPatternEditAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	PatternEditActionBase::OnKeyDown(nChar, nRepCnt, nFlags);

	return TRUE;
}

BOOL OBroundPatternEditAction::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	return TRUE;
}

BOOL OBroundPatternEditAction::DrawBeforeFinish()
{
	CLOptionInfoPtr pCLOptionInfo = CLOptionInfo::GetInstance();
	DisplayParamPtr pColorInfo = pCLOptionInfo->GetDisplayParam();

	// keep the old color.
	COLORREF dwOldColor;
	m_pViewPort->GetDrawColor(dwOldColor);

	//
	m_pViewPort->SetDrawColor(*pColorInfo->GetPatColor());

	if (m_data[3] == 0)
	{
		double dStartPtX = m_data[0] + m_data[2] / 2.0 * cos(m_data[4]);
		double dStartPtY = m_data[1] + m_data[2] / 2.0 * sin(m_data[4]);
		double dEndPtX = m_data[0] + m_data[2] / 2.0 * cos(m_data[4] + PI);
		double dEndPtY = m_data[1] + m_data[2] / 2.0 * sin(m_data[4] + PI);

		m_pViewPort->DrawLinePoint2D(dStartPtX, dStartPtY, dEndPtX, dEndPtY);
	}
	else if (MathEx::Equal(m_data[3], m_data[2]))
	{
		m_pViewPort->DrawCircle2D(m_data[0], m_data[1], m_data[3]/2.0);
	}
	else
	{
		// Draw two ares and two lines which form obRound slot
		double dLength = m_data[2] / 2.0;
		double dWidth  = m_data[3] / 2.0;
		double dStartAng = m_data[4] - PI/2.0;
		double dEndAng = m_data[4] + PI/2.0;
		if (dStartAng > dEndAng)
			dEndAng += 2*PI;

		double dArcCenterPtX1 = m_data[0] + (dLength)*cos(m_data[4]);
		double dArcCenterPtY1 = m_data[1] + (dLength)*sin(m_data[4]);
		double dLinePtX1 = dArcCenterPtX1 + dWidth * cos(dStartAng);
		double dLinePtY1 = dArcCenterPtY1 + dWidth * sin(dStartAng);
		double dLinePtX2 = dArcCenterPtX1+ dWidth * cos(dEndAng);
		double dLinePtY2 = dArcCenterPtY1 + dWidth * sin(dEndAng);

		double dArcCenterPtX2 = m_data[0] + (dLength)*cos(m_data[4] + PI);
		double dArcCenterPtY2 = m_data[1] + (dLength)*sin(m_data[4] + PI);
		double dLinePtX3 = dArcCenterPtX2 + dWidth * cos(dStartAng + PI);
		double dLinePtY3 = dArcCenterPtY2 + dWidth * sin(dStartAng + PI);
		double dLinePtX4 = dArcCenterPtX2 + dWidth * cos(dEndAng + PI);
		double dLinePtY4 = dArcCenterPtY2 + dWidth * sin(dEndAng + PI);

		m_pViewPort->DrawArc2D(dArcCenterPtX1, dArcCenterPtY1, dWidth, dStartAng, dEndAng - dStartAng);
		m_pViewPort->DrawArc2D(dArcCenterPtX2, dArcCenterPtY2, dWidth, dStartAng + PI, dEndAng - dStartAng);
		m_pViewPort->DrawLinePoint2D(dLinePtX1, dLinePtY1, dLinePtX4, dLinePtY4);
		m_pViewPort->DrawLinePoint2D(dLinePtX2, dLinePtY2, dLinePtX3, dLinePtY3);
	}

	// restore the draw color
	m_pViewPort->SetDrawColor(dwOldColor);

	// draw cursor.
	CursorDrawer::DrawCrossLineCursor(m_pViewPort);

	return TRUE;
}

void OBroundPatternEditAction::UpdateForUndoRedo()
{
	PatternEditActionBase::UpdateForUndoRedo();

	PatternListPtr pPatList = m_pPartCadData->GetPatternList();
	if (m_editState != EDIT_OBROUND_NOTSTART && !pPatList->GetPatternFromID(m_pGripEditPat->GetID()))
	{
		PostMessage2EnterPatternSelectAction();
	}
}

void OBroundPatternEditAction::SetGripEditPattern(IPatternPtr pPattern)
{
	m_pGripEditPat = pPattern;

	OBroundPattern* pOBroundPat = dynamic_cast<OBroundPattern*>(pPattern.get());
	Point2D centerPt = pOBroundPat->GetBasePoint();
	m_data[0] = centerPt.X();
	m_data[1] = centerPt.Y();
	m_data[2] = pOBroundPat->GetLength();
	m_data[3] = pOBroundPat->GetWidth();
	m_data[4] = pOBroundPat->GetBaseVector().Angle();
}

void OBroundPatternEditAction::SetGripEditPt(double dPtX, double dPtY)
{
	double dLength = m_data[2] / 2.0;
	double dWidth  = m_data[3] / 2.0;
	double dStartAng = m_data[4] - PI/2.0;
	double dEndAng = m_data[4] + PI/2.0;
	if (dStartAng > dEndAng)
		dEndAng += 2*PI;

	double dArcCenterPtX1 = m_data[0] + (dLength - dWidth)*cos(m_data[4]);
	double dArcCenterPtY1 = m_data[1] + (dLength - dWidth)*sin(m_data[4]);
	double dLinePtX1 = dArcCenterPtX1 + dWidth * cos(dStartAng);
	double dLinePtY1 = dArcCenterPtY1 + dWidth * sin(dStartAng);
	double dLinePtX2 = dArcCenterPtX1+ dWidth * cos(dEndAng);
	double dLinePtY2 = dArcCenterPtY1 + dWidth * sin(dEndAng);

	double dArcCenterPtX2 = m_data[0] + (dLength - dWidth)*cos(m_data[4] + PI);
	double dArcCenterPtY2 = m_data[1] + (dLength - dWidth)*sin(m_data[4] + PI);
	double dLinePtX3 = dArcCenterPtX2 + dWidth * cos(dStartAng + PI);
	double dLinePtY3 = dArcCenterPtY2 + dWidth * sin(dStartAng + PI);
	double dLinePtX4 = dArcCenterPtX2 + dWidth * cos(dEndAng + PI);
	double dLinePtY4 = dArcCenterPtY2 + dWidth * sin(dEndAng + PI);

	double EndPtX1 = dArcCenterPtX1 + dWidth * cos(m_data[4]);
	double EndPtY1 = dArcCenterPtY1 + dWidth * sin(m_data[4]);
	double EndPtX2 = dArcCenterPtX2 + dWidth * cos(m_data[4] + PI);
	double EndPtY2 = dArcCenterPtY2 + dWidth * sin(m_data[4] + PI);

	if (MathEx::PointEqual(dPtX, dPtY, (dLinePtX1+dLinePtX4)/2.0, (dLinePtY1+dLinePtY4)/2.0))
	{
		// Modify dWidth
		m_editState = EDIT_OBROUND_BOTTOM_POINT;
		m_iMsgID = IDS_OUTPUT_OBROUND_WIDTH;
	}
	else if(MathEx::PointEqual(dPtX, dPtY, (dLinePtX2+dLinePtX3)/2.0, (dLinePtY2+dLinePtY3)/2.0))
	{
		// Modify dWidth
		m_editState = EDIT_OBROUND_TOP_POINT;
		m_iMsgID = IDS_OUTPUT_OBROUND_WIDTH;
	}
	else if(MathEx::PointEqual(dPtX, dPtY, EndPtX1, EndPtY1))
	{
		// Modify angle and dLength
		m_editState = EDIT_OBROUND_LEFT_POINT;
		m_iMsgID = IDS_OUTPUT_OBROUND_ANGLE_LENGTH;
	}
	else if(MathEx::PointEqual(dPtX, dPtY, EndPtX2, EndPtY2))
	{
		// Modify angle and dLength
		m_editState = EDIT_OBROUND_RIGHT_POINT;
		m_iMsgID = IDS_OUTPUT_OBROUND_ANGLE_LENGTH;
	}
	else if( MathEx::PointEqual(dPtX, dPtY, m_data[0], m_data[1]))
	{
		// Modify center point
		m_editState = EDIT_OBROUND_CENTER_POINT;
		m_iMsgID = IDS_OUTPUT_OBROUND_CENTERPT;
	}
}

END_CUTLEADER_NAMESPACE()

#include "StdAfx.h"
#include "DoubleDPatternEditAction.h"

#include "GlViewPort.h"
#include "MathEx.h"
#include "ICommand.h"
#include "CommandManager.h"
#include "ValueInputManager.h"
#include "clUILibResource.h"
#include "Point2D.h"
#include "CursorDrawer.h"

#include "CLOptionInfo.h"
#include "DisplayParam.h"

#include "PatternSelectAction.h"
#include "PatternList.h"
#include "DoubleDPattern.h"
#include "PatternAddCommand.h"
#include "GripEditCommand.h"
#include "clGeometryFeatureResource.h"
#include "PartCadData.h"

BEGIN_CUTLEADER_NAMESPACE()

DoubleDPatternEditAction::DoubleDPatternEditAction(PartCadDataPtr pPartCadData, GlViewPortPtr pViewPort)
{
	m_drawState = DRAW_DOUBLED_NOTSTART;
	m_editState = EDIT_DOUBLED_NOTSTART;

	memset(m_data, 0, sizeof(double)*5);

	m_pPartCadData = pPartCadData;
	m_pViewPort = pViewPort;

	m_iMsgID = IDS_OUTPUT_PATDOUBLED_CENTERPT;
}

DoubleDPatternEditAction::~DoubleDPatternEditAction(void)
{

}

BOOL DoubleDPatternEditAction::LButtonDown(double& dPtX, double& dPtY)
{
	PatternListPtr pPatList = m_pPartCadData->GetPatternList();
	PatternLoopListPtr pPatternLoopList = m_pPartCadData->GetPatternLoopList();
	LoopTopologyItemListPtr pLoopTopologyItems = m_pPartCadData->GetLoopTopologyItems_FW();

	if (m_editState != EDIT_DOUBLED_NOTSTART) //EDIT mode
	{
		if (m_editState == EDIT_DOUBLED_CENTER_POINT)
		{
			m_data[0] = dPtX;
			m_data[1] = dPtY;
		}
		else if(m_editState == EDIT_DOUBLED_TOP_POINT)
		{
			m_data[3] = sqrt((dPtX - m_data[0])*(dPtX - m_data[0])+(dPtY - m_data[1])*(dPtY - m_data[1]));
		}
		else if(m_editState == EDIT_DOUBLED_BOTTOM_POINT)
		{
			m_data[2] = sqrt((dPtX - m_data[0])*(dPtX - m_data[0])+(dPtY - m_data[1])*(dPtY - m_data[1]));
		}

		// create the new DoubleD pattern with the new data.
		Point2D basePt(m_data[0], m_data[1]);
		IPatternPtr pNewPat(new DoubleDPattern(basePt, m_data[2], m_data[3], m_data[4]));

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
		if (m_drawState == DRAW_DOUBLED_NOTSTART || 
			m_drawState == DRAW_DOUBLED_THIRD_POINT) //first click
		{
			m_drawState = DRAW_DOUBLED_FIRST_POINT;
			m_iMsgID = IDS_OUTPUT_PATDOUBLED_RADIUS;

			m_data[0] = dPtX;
			m_data[1] = dPtY;
			m_data[2] = 0;
			m_data[3] = 0;
			m_data[4] = 0;
		}
		else if(m_drawState == DRAW_DOUBLED_FIRST_POINT) //second click, we can create a line
		{
			m_drawState = DRAW_DOUBLED_SECOND_POINT;
			m_iMsgID = IDS_OUTPUT_PATDOUBLED_WIDTH_ANGLE;

			m_data[2] = sqrt((dPtX - m_data[0])*(dPtX - m_data[0])+(dPtY - m_data[1])*(dPtY - m_data[1]));
		}
		else if(m_drawState == DRAW_DOUBLED_SECOND_POINT) //second click, we can create a line
		{
			m_drawState = DRAW_DOUBLED_THIRD_POINT;
			m_iMsgID = IDS_OUTPUT_PATDOUBLED_CENTERPT;

			m_data[3] = sqrt((dPtX - m_data[0])*(dPtX - m_data[0])+(dPtY - m_data[1])*(dPtY - m_data[1]));
			m_data[4] = MathEx::GetSignedAng(atan2(dPtY-m_data[1], dPtX-m_data[0]));

			// create double D pattern.
			Point2D basePt(m_data[0], m_data[1]);
			IPatternPtr pDoubleDPat(new DoubleDPattern(basePt, m_data[2], m_data[3], m_data[4]));

			// create command
			EditorData editorData = GetEditorData();
			ICommandPtr pCommand(new PatternAddCommand(editorData, pPatList, pPatternLoopList, pLoopTopologyItems, pDoubleDPat));
			pCommand->Do();
			CommandManager::GetInstance()->AddCommand(pCommand);

			// enter the selecting pattern action.
			PostMessage2EnterPatternSelectAction(pDoubleDPat.get());
		}
	}

	return TRUE;
}

BOOL DoubleDPatternEditAction::MovePoint(double& dPtX, double& dPtY)
{
	if (m_editState != EDIT_DOUBLED_NOTSTART) //EDIT mode
	{
		if (m_editState == EDIT_DOUBLED_CENTER_POINT)
		{
			m_data[0] = dPtX;
			m_data[1] = dPtY;
		}
		else if(m_editState == EDIT_DOUBLED_TOP_POINT ||
				m_editState == EDIT_DOUBLED_BOTTOM_POINT)
		{
			m_data[3] = sqrt((dPtX - m_data[0])*(dPtX - m_data[0])+
							 (dPtY - m_data[1])*(dPtY - m_data[1]));
		}
		else if(m_editState == EDIT_DOUBLED_RIGHT_POINT ||
				m_editState == EDIT_DOUBLED_LEFT_POINT)
		{
			m_data[2] = sqrt((dPtX - m_data[0])*(dPtX - m_data[0]) + (dPtY - m_data[1])*(dPtY - m_data[1]));
		}
	}
	else
	{
		if (m_drawState == DRAW_DOUBLED_FIRST_POINT)
		{
			m_data[2] = sqrt((dPtX - m_data[0])*(dPtX - m_data[0]) + (dPtY - m_data[1])*(dPtY - m_data[1]));
		}
		else if(m_drawState == DRAW_DOUBLED_SECOND_POINT)
		{
			m_data[3] = sqrt((dPtX - m_data[0])*(dPtX - m_data[0]) + (dPtY - m_data[1])*(dPtY - m_data[1]));
			m_data[4] = MathEx::GetSignedAng(atan2(dPtY-m_data[1], dPtX-m_data[0]));
		}
	}

	return TRUE;
}

void DoubleDPatternEditAction::ConvertKBInput(double& dVal1, double& dVal2)
{
	if (m_editState != EDIT_DOUBLED_NOTSTART)
	{
		// under "edit" action.

		if (m_editState == EDIT_DOUBLED_CENTER_POINT)
		{
			// now, do not need to convert.
			;
		}
		else if(m_editState == EDIT_DOUBLED_TOP_POINT ||
				m_editState == EDIT_DOUBLED_BOTTOM_POINT)
		{
			// "dVal1" holds the "width" of the Double-D
			
			/************************************************************************/
			// get the vector from center point to chord.
			
			// get the vector along the Double D angle.
			Vector2D v1;
			v1.SetAngle(m_data[4]);

			v1.Orthogonal();
			v1.Magnitude(dVal1);
			/************************************************************************/

			// get the point on the chord.
			Point2D centerPt(m_data[0], m_data[1]);
			centerPt.Transfer(v1);

			dVal1 = centerPt.X();
			dVal2 = centerPt.Y();
		}
		else if(m_editState == EDIT_DOUBLED_RIGHT_POINT ||
				m_editState == EDIT_DOUBLED_LEFT_POINT)
		{
			// "dVal1" holds the "radius" of the Double-D

			// get the vector along the Double D angle.
			Vector2D v1;
			v1.SetAngle(m_data[4]);
			v1.Magnitude(dVal1);

			// get the point on the arc.
			Point2D centerPt(m_data[0], m_data[1]);
			centerPt.Transfer(v1);

			dVal1 = centerPt.X();
			dVal2 = centerPt.Y();
		}
	}
	else
	{
		// under "draw" action.

		if (m_drawState == DRAW_DOUBLED_NOTSTART || 
			m_drawState == DRAW_DOUBLED_THIRD_POINT)
		{
			// now, do not need to convert.
			;
		}
		else if(m_drawState == DRAW_DOUBLED_FIRST_POINT)
		{
			// "dVal1" holds the "radius" of the Double-D

			// get the vector along the Double D angle.
			Vector2D v1;
			v1.Magnitude(dVal1);

			// get the point on the arc.
			Point2D centerPt(m_data[0], m_data[1]);
			centerPt.Transfer(v1);

			dVal1 = centerPt.X();
			dVal2 = centerPt.Y();
		}
		else if(m_drawState == DRAW_DOUBLED_SECOND_POINT)
		{
			// "dVal1" holds the "width" of the Double-D
			// "dVal2" holds the "angle" of the Double-D
			// notes: we should return a point and can input to "LButtonDown".

			/************************************************************************/
			// get the vector along the Double D angle.

			Vector2D v1;
			v1.SetAngle(dVal2);

			v1.Magnitude(dVal1);
			/************************************************************************/

			// get the point on the chord.
			Point2D centerPt(m_data[0], m_data[1]);
			centerPt.Transfer(v1);

			dVal1 = centerPt.X();
			dVal2 = centerPt.Y();
		}
	}
}

BOOL DoubleDPatternEditAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	PatternEditActionBase::OnKeyDown(nChar, nRepCnt, nFlags);

	return TRUE;
}

BOOL DoubleDPatternEditAction::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	return TRUE;
}

BOOL DoubleDPatternEditAction::DrawBeforeFinish()
{
	CLOptionInfoPtr pCLOptionInfo = CLOptionInfo::GetInstance();
	DisplayParamPtr pColorInfo = pCLOptionInfo->GetDisplayParam();

	// keep the old color.
	COLORREF dwOldColor;
	m_pViewPort->GetDrawColor(dwOldColor);

	//
	m_pViewPort->SetDrawColor(*pColorInfo->GetPatColor());

	if (m_drawState == DRAW_DOUBLED_FIRST_POINT)
	{
		m_pViewPort->DrawCircle2D(m_data[0], m_data[1], m_data[2]);		 
	}
	else if(m_drawState == DRAW_DOUBLED_SECOND_POINT || m_editState != EDIT_DOUBLED_NOTSTART)
	{
		/************************************************************************/		
		// Draw two arcs and lines.

		//
		double dTang = asin(m_data[3]/m_data[2]);
		double dStartAng = m_data[4] - dTang;
		double dEndAng = m_data[4] + dTang;
		m_pViewPort->DrawArc2D(m_data[0], m_data[1], m_data[2], dStartAng, dEndAng - dStartAng);

		//
		double dChordStartX = m_data[0] + m_data[2]*cos(dEndAng);
		double dChordStartY = m_data[1] + m_data[2]*sin(dEndAng);
		double dChordEndX = m_data[0] + m_data[2]*cos(dStartAng + PI);
		double dChordEndY = m_data[1] + m_data[2]*sin(dStartAng + PI);
		m_pViewPort->DrawLinePoint2D(dChordStartX, dChordStartY, dChordEndX, dChordEndY);

		//
		dStartAng += PI;
		dEndAng += PI;
		m_pViewPort->DrawArc2D(m_data[0], m_data[1], m_data[2], dStartAng, dEndAng - dStartAng);

		//
		dChordStartX = m_data[0] + m_data[2]*cos(dEndAng);
		dChordStartY = m_data[1] + m_data[2]*sin(dEndAng);
		dChordEndX = m_data[0] + m_data[2]*cos(dStartAng - PI);
		dChordEndY = m_data[1] + m_data[2]*sin(dStartAng - PI);
		m_pViewPort->DrawLinePoint2D(dChordStartX, dChordStartY, dChordEndX, dChordEndY);
		/************************************************************************/
	}

	// restore the draw color
	m_pViewPort->SetDrawColor(dwOldColor);

	// draw cursor.
	CursorDrawer::DrawCrossLineCursor(m_pViewPort);

	return TRUE;
}

void DoubleDPatternEditAction::UpdateForUndoRedo()
{
	PatternListPtr pPatList = m_pPartCadData->GetPatternList();
	if (m_editState != EDIT_DOUBLED_NOTSTART && !pPatList->GetPatternFromID(m_pGripEditPat->GetID()))
	{
		PostMessage2EnterPatternSelectAction();
	}
}

void DoubleDPatternEditAction::SetGripEditPattern(IPatternPtr pPattern)
{
	m_pGripEditPat = pPattern;

	DoubleDPattern* pDoubleDPat = dynamic_cast<DoubleDPattern*>(pPattern.get());
	Point2D centerPt = pDoubleDPat->GetBasePoint();
	m_data[0] = centerPt.X();
	m_data[1] = centerPt.Y();

	m_data[2] = pDoubleDPat->GetRadius();
	m_data[3] = pDoubleDPat->GetToChord();
	m_data[4] = pDoubleDPat->GetAngle();
}

void DoubleDPatternEditAction::SetGripEditPt(double dPtX, double dPtY)
{
	double dTang = asin(m_data[3] / m_data[2]);
	double dStartAng = m_data[4] - dTang;
	double dEndAng = m_data[4] + dTang;

	double chordStartX1 = m_data[0] + m_data[2] * cos(dEndAng);
	double chordStartY1 = m_data[1] + m_data[2] * sin(dEndAng);
	double chordEndX1 = m_data[0] + m_data[2] * cos(dStartAng + PI);
	double chordEndY1 = m_data[1] + m_data[2] * sin(dStartAng + PI);

	dStartAng += PI;
	dEndAng += PI;

	double chordStartX2 = m_data[0] + m_data[2]*cos(dEndAng);
	double chordStartY2 = m_data[1] + m_data[2]*sin(dEndAng);
	double chordEndX2 = m_data[0] + m_data[2]*cos(dStartAng - PI);
	double chordEndY2 = m_data[1] + m_data[2]*sin(dStartAng - PI);

	double endPtX1 = m_data[0] + m_data[2]*cos(m_data[4]);
	double endPtY1 = m_data[1] + m_data[2]*sin(m_data[4]);
	double endPtX2 = m_data[0] + m_data[2]*cos(m_data[4] + PI);
	double endPtY2 = m_data[1] + m_data[2]*sin(m_data[4] + PI);

	if (MathEx::PointEqual(dPtX, dPtY, chordStartX1, chordStartY1) ||
	    MathEx::PointEqual(dPtX, dPtY, chordEndX1, chordEndY1) || 
		MathEx::PointEqual(dPtX, dPtY, chordStartX2, chordStartY2) ||
		MathEx::PointEqual(dPtX, dPtY, chordEndX2, chordEndY2) ||
		MathEx::PointEqual(dPtX, dPtY, endPtX1, endPtY1) ||
		MathEx::PointEqual(dPtX, dPtY, endPtX2, endPtY2))
	{
		// Modify radius
		m_editState = EDIT_DOUBLED_LEFT_POINT;
		m_iMsgID = IDS_OUTPUT_PATDOUBLED_RADIUS;
	}
	else if(MathEx::PointEqual(dPtX, dPtY, (chordStartX1+chordEndX1)/2.0, (chordStartY1+chordEndY1)/2.0))
	{
		m_editState = EDIT_DOUBLED_TOP_POINT;
		m_iMsgID = IDS_OUTPUT_PATDOUBLED_WIDTH;

	}
	else if(MathEx::PointEqual(dPtX, dPtY, (chordStartX2+chordEndX2)/2.0, (chordStartY2+chordEndY2)/2.0))
	{
		// modify chord
		m_editState = EDIT_DOUBLED_BOTTOM_POINT;
		m_iMsgID = IDS_OUTPUT_PATDOUBLED_WIDTH;
	}
	else if(MathEx::PointEqual(dPtX, dPtY, m_data[0], m_data[1]))
	{
		// modify center point
		m_editState = EDIT_DOUBLED_CENTER_POINT;
		m_iMsgID = IDS_OUTPUT_PATDOUBLED_CENTERPT;
	}
}

END_CUTLEADER_NAMESPACE()

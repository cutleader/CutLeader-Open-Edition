#include "StdAfx.h"
#include "EllipsePatternEditAction.h"

#include "GlViewPort.h"
#include "MathEx.h"
#include "CommandManager.h"
#include "ICommand.h"
#include "ValueInputManager.h"
#include "clUILibResource.h"
#include "CursorDrawer.h"
#include "Point2DList.h"

#include "CLOptionInfo.h"
#include "DisplayParam.h"

#include "PatternSelectAction.h"
#include "EllipsePattern.h"
#include "PatternAddCommand.h"
#include "GripEditCommand.h"
#include "clGeometryFeatureResource.h"
#include "PatternList.h"
#include "PatternPropertyBar.h"
#include "PartCadData.h"

BEGIN_CUTLEADER_NAMESPACE()

EllipsePatternEditAction::EllipsePatternEditAction(PartCadDataPtr pPartCadData, GlViewPortPtr pViewPort)
{
	m_drawStatus = DRAW_ELLIPSE_NOTSTART;
	m_editState = EDIT_ELLIPSE_NOTSTART;
	memset(m_data, 0, sizeof(double)*5);

	m_pPartCadData = pPartCadData;
	m_pViewPort = pViewPort;

	m_iMsgID = IDS_OUTPUT_PATELLIPSE_CENTERPT;
}

EllipsePatternEditAction::~EllipsePatternEditAction(void)
{
}

BOOL EllipsePatternEditAction::LButtonDown(double& dPtX, double& dPtY)
{
	PatternListPtr pPatList = m_pPartCadData->GetPatternList();
	PatternLoopListPtr pPatternLoopList = m_pPartCadData->GetPatternLoopList();
	LoopTopologyItemListPtr pLoopTopologyItems = m_pPartCadData->GetLoopTopologyItems_FW();

	if (m_editState != EDIT_ELLIPSE_NOTSTART)
	{
		// create the new ellipse pattern with the new data.
		Point2D centerPt(m_data[0], m_data[1]);
		IPatternPtr pEllipsePat(new EllipsePattern(centerPt, m_data[4], m_data[2]*2, m_data[3]*2));

		// create command
		EditorData editorData = GetEditorData();
		ICommandPtr pCommand(new GripEditCommand(editorData, pEllipsePat, pPatternLoopList, pLoopTopologyItems, m_pGripEditPat));
		pCommand->Do();
		CommandManager::GetInstance()->AddCommand(pCommand);

		// enter the selecting pattern action.
		PostMessage2EnterPatternSelectAction(m_pGripEditPat.get());
	}
	else
	{
		if (m_drawStatus == DRAW_ELLIPSE_NOTSTART || m_drawStatus == DRAW_ELLIPSE_THIRD_POINT)
		{
			m_drawStatus = DRAW_ELLIPSE_FIRST_POINT;
			m_iMsgID = IDS_OUTPUT_PATELLIPSE_LENGTH_ANGLE;

			m_data[0] = dPtX;
			m_data[1] = dPtY;
			m_data[2] = 0;
			m_data[3] = 0;
			m_data[4] = 0;
		}
		else if(m_drawStatus == DRAW_ELLIPSE_FIRST_POINT)
		{
			m_drawStatus = DRAW_ELLIPSE_SECOND_POINT;
			m_iMsgID = IDS_OUTPUT_PATELLIPSE_WIDTH;

			// calculate the long radius
			m_data[2] = sqrt((dPtX-m_data[0])*(dPtX-m_data[0]) + (dPtY-m_data[1])*(dPtY-m_data[1]));
			// calculate the short radius
			m_data[3] = m_data[2]/2;
			// calculate the angle
			m_data[4] = atan2((dPtY-m_data[1]),	(dPtX-m_data[0]));

		}
		else if(m_drawStatus == DRAW_ELLIPSE_SECOND_POINT)
		{
			m_drawStatus = DRAW_ELLIPSE_THIRD_POINT;
			m_iMsgID = IDS_OUTPUT_PATELLIPSE_CENTERPT;

			// calculate the short radius
			m_data[3] = sqrt((dPtX-m_data[0])*(dPtX-m_data[0]) + (dPtY-m_data[1])*(dPtY-m_data[1])) / 2;

			// create ellipse pattern.
			Point2D centerPt(m_data[0], m_data[1]);
			IPatternPtr pEllipsePat(new EllipsePattern(centerPt, m_data[4], m_data[2]*2, m_data[3]*2));

			// create command
			EditorData editorData = GetEditorData();
			ICommandPtr pCommand(new PatternAddCommand(editorData, pPatList, pPatternLoopList, pLoopTopologyItems, pEllipsePat));
			pCommand->Do();
			CommandManager::GetInstance()->AddCommand(pCommand);

			// enter the selecting pattern action.
			PostMessage2EnterPatternSelectAction(pEllipsePat.get());
		}
	}

	return TRUE;
}

BOOL EllipsePatternEditAction::MovePoint(double& dPtX, double& dPtY)
{
	if (m_editState != EDIT_ELLIPSE_NOTSTART) //EDIT mode
	{
		if (m_editState == EDIT_ELLIPSE_CENTER_POINT)
		{
			m_data[0] = dPtX;
			m_data[1] = dPtY;
		}
		else if (m_editState == EDIT_ELLIPSE_TOP_POINT || m_editState == EDIT_ELLIPSE_BOTTOM_POINT)
		{
			// calculate the short radius
			m_data[3] = sqrt((dPtX-m_data[0])*(dPtX-m_data[0]) + (dPtY-m_data[1])*(dPtY-m_data[1]));

		}
		else if (m_editState == EDIT_ELLIPSE_LEFT_POINT || m_editState == EDIT_ELLIPSE_RIGHT_POINT)
		{
			// calculate the long radius
			m_data[2] = sqrt((dPtX-m_data[0])*(dPtX-m_data[0]) + (dPtY-m_data[1])*(dPtY-m_data[1]));
		}
	}
	else
	{
		if (m_drawStatus == DRAW_ELLIPSE_FIRST_POINT)
		{
			// calculate the long radius
			m_data[2] = sqrt((dPtX-m_data[0])*(dPtX-m_data[0]) + (dPtY-m_data[1])*(dPtY-m_data[1]));
			// calculate the short radius
			m_data[3] = m_data[2]/2;
			// calculate the angle
			m_data[4] = atan2((dPtY-m_data[1]),	(dPtX-m_data[0]));
		}
		else if(m_drawStatus == DRAW_ELLIPSE_SECOND_POINT)
		{
			// calculate the short radius
			m_data[3] = sqrt((dPtX-m_data[0])*(dPtX-m_data[0]) + (dPtY-m_data[1])*(dPtY-m_data[1])) / 2;
		}
	}

	return TRUE;
}

void EllipsePatternEditAction::ConvertKBInput(double& dVal1, double& dVal2)
{
	if (m_editState != EDIT_ELLIPSE_NOTSTART)
	{
		// under "edit" action.

		if (m_editState == EDIT_ELLIPSE_CENTER_POINT)
		{
			// now, do not need to convert.
			;
		}
		else if (m_editState == EDIT_ELLIPSE_TOP_POINT || m_editState == EDIT_ELLIPSE_BOTTOM_POINT)
		{
			// now, "dVal1" holds the width of the ellipse.
			// so we only need to specify a point,
			// the distance from this point to center point is "dVal1"

			Point2D centerPt(m_data[0], m_data[1]);

			Vector2D v1;
			v1.Magnitude(dVal1);
			centerPt.Transfer(v1);

			dVal1 = centerPt.X();
			dVal2 = centerPt.Y();
		}
		else if (m_editState == EDIT_ELLIPSE_LEFT_POINT || m_editState == EDIT_ELLIPSE_RIGHT_POINT)
		{
			// now, "dVal1" holds the length of the ellipse.
			// so we only need to specify a point,
			// the distance from this point to center point is "dVal1"

			Point2D centerPt(m_data[0], m_data[1]);

			Vector2D v1;
			v1.Magnitude(dVal1);
			centerPt.Transfer(v1);

			dVal1 = centerPt.X();
			dVal2 = centerPt.Y();
		}
	}
	else
	{
		// under "draw" action.

		if (m_drawStatus == DRAW_ELLIPSE_NOTSTART || m_drawStatus == DRAW_ELLIPSE_THIRD_POINT)
		{
			// now, do not need to convert.
			;
		}
		else if(m_drawStatus == DRAW_ELLIPSE_FIRST_POINT)
		{
			// now, "dVal1" holds the length of the ellipse.
			// "dVal2" holds the angle of the ellipse. so we only need to specify a point,
			// the distance from this point to center point is "dVal1"
			// the vector from this point to center point has the angle: "dVal2"

			Point2D centerPt(m_data[0], m_data[1]);

			Vector2D v1;
			v1.SetAngle(dVal2);
			v1.Magnitude(dVal1);
			centerPt.Transfer(v1);

			dVal1 = centerPt.X();
			dVal2 = centerPt.Y();
		}
		else if(m_drawStatus == DRAW_ELLIPSE_SECOND_POINT)
		{
			// now, "dVal1" holds the width of the ellipse.
			// so we only need to specify a point, the distance from this point to center point is "dVal1"

			Point2D centerPt(m_data[0], m_data[1]);

			Vector2D v1;
			v1.Magnitude(dVal1);
			centerPt.Transfer(v1);

			dVal1 = centerPt.X();
			dVal2 = centerPt.Y();
		}
	}
}

BOOL EllipsePatternEditAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	PatternEditActionBase::OnKeyDown(nChar, nRepCnt, nFlags);

	return TRUE;
}

BOOL EllipsePatternEditAction::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	return TRUE;
}

BOOL EllipsePatternEditAction::DrawBeforeFinish()
{
	CLOptionInfoPtr pCLOptionInfo = CLOptionInfo::GetInstance();
	DisplayParamPtr pColorInfo = pCLOptionInfo->GetDisplayParam();

	// keep the old config.
	COLORREF dwOldColor;
	m_pViewPort->GetDrawColor(dwOldColor);
	int iOldLineWidth = m_pViewPort->GetLineWidth();

	//
	m_pViewPort->SetDrawColor(*pColorInfo->GetPatColor());
	m_pViewPort->SetLineWidth(1);

	// only under this condition we can draw line.
	if (m_drawStatus == DRAW_ELLIPSE_FIRST_POINT)
	{
		double dX0 = m_data[0];
		double dY0 = m_data[1];
		double dAngle = m_data[4];
		double dLongRadius = m_data[2];

		double dXr1 = dX0 + dLongRadius;
		double dYr1 = dY0;
		double dX3r = dX0 - dLongRadius;
		double dY3r = dY0;

		//
		double dX1, dY1, dX3, dY3;
		MathEx::RotatePoint(dX0, dY0, dAngle, dXr1, dYr1, dX1, dY1);
		MathEx::RotatePoint(dX0, dY0, dAngle, dX3r, dY3r, dX3, dY3);

		m_pViewPort->DrawLinePoint2D(dX1, dY1, dX3, dY3);
	}
	// under this condition, we can not draw.
	else if ((m_drawStatus == DRAW_ELLIPSE_NOTSTART || m_drawStatus == DRAW_ELLIPSE_THIRD_POINT) &&
		     m_editState == EDIT_ELLIPSE_NOTSTART)
	{
		;
	}
	else
	{
		m_pViewPort->DrawEllipse2D(m_data[0], m_data[1], m_data[2], m_data[3], m_data[4]);
	}

	// restore the draw color
	m_pViewPort->SetDrawColor(dwOldColor);
	m_pViewPort->SetLineWidth(iOldLineWidth);

	// draw cursor.
	CursorDrawer::DrawCrossLineCursor(m_pViewPort);

	return TRUE;
}

void EllipsePatternEditAction::UpdateForUndoRedo()
{
	PatternListPtr pPatList = m_pPartCadData->GetPatternList();
	if (m_editState != EDIT_ELLIPSE_NOTSTART && !pPatList->GetPatternFromID(m_pGripEditPat->GetID()))
	{
		PostMessage2EnterPatternSelectAction();
	}
}

void EllipsePatternEditAction::SetGripEditPattern(IPatternPtr pPattern)
{
	m_pGripEditPat = pPattern;

	EllipsePattern* pEllipsePat = dynamic_cast<EllipsePattern*>(pPattern.get());
	Point2D centerPt = pEllipsePat->GetBasePoint();
	m_data[0] = centerPt.X();
	m_data[1] = centerPt.Y();
	m_data[2] = pEllipsePat->GetLength()/2;
	m_data[3] = pEllipsePat->GetWidth()/2;
	m_data[4] = pEllipsePat->GetBaseVector().Angle();
}

void EllipsePatternEditAction::SetGripEditPt(double dPtX, double dPtY)
{
	// get snap points of the pattern
	Point2DList snapPts;
	m_pGripEditPat->GetSnapPoints(snapPts);

	Point2D tmpPt(dPtX, dPtY);
	for(unsigned int i = 0; i < snapPts.size(); i++)
	{
		Point2D snapPt = (Point2D)snapPts[i];
		if(snapPt.IsEqual(tmpPt))
		{
			if (i == 0)
			{
				m_editState = EDIT_ELLIPSE_CENTER_POINT;
				m_iMsgID = IDS_OUTPUT_PATELLIPSE_CENTERPT;
			}
			// along "length" direction
			else if (i == 1)
			{
				m_editState = EDIT_ELLIPSE_RIGHT_POINT;
				m_iMsgID = IDS_OUTPUT_PATELLIPSE_LENGTH;
			}
			else if (i == 2)
			{
				m_editState = EDIT_ELLIPSE_LEFT_POINT;
				m_iMsgID = IDS_OUTPUT_PATELLIPSE_LENGTH;
			}
			// along "width" direction
			else if (i == 3)
			{
				m_editState = EDIT_ELLIPSE_TOP_POINT;
				m_iMsgID = IDS_OUTPUT_PATELLIPSE_WIDTH;
			}
			else if (i == 4)
			{
				m_editState = EDIT_ELLIPSE_BOTTOM_POINT;
				m_iMsgID = IDS_OUTPUT_PATELLIPSE_WIDTH;
			}

			return;
		}
	}
}

END_CUTLEADER_NAMESPACE()

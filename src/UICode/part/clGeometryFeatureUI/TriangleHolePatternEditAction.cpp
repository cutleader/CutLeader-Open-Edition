#include "StdAfx.h"
#include "TriangleHolePatternEditAction.h"

#include "MathEx.h"
#include "GlViewPort.h"
#include "ValueInputManager.h"
#include "clUILibResource.h"
#include "CommandManager.h"
#include "ICommand.h"
#include "CursorDrawer.h"
#include "Point2DList.h"

#include "CLOptionInfo.h"
#include "DisplayParam.h"

#include "PatternAddCommand.h"
#include "GripEditCommand.h"
#include "TriangleHolePattern.h"
#include "PatternList.h"
#include "clGeometryFeatureResource.h"
#include "PatternSelectAction.h"
#include "PatternPropertyBar.h"
#include "PartCadData.h"

BEGIN_CUTLEADER_NAMESPACE()

TriangleHolePatternEditAction::TriangleHolePatternEditAction(PartCadDataPtr pPartCadData, GlViewPortPtr pViewPort)
{
	m_drawState = DRAW_TRIANGLE_NOTSTART;
	m_editState = EDIT_TRIANGLE_NOTSTART;

	memset(m_data, 0, sizeof(double)*7);

	m_pPartCadData = pPartCadData;
	m_pViewPort = pViewPort;

	m_iMsgID = IDS_OUTPUT_TRIHOLE_BASEPT;
}

TriangleHolePatternEditAction::~TriangleHolePatternEditAction(void)
{
}

BOOL TriangleHolePatternEditAction::LButtonDown(double& dPtX, double& dPtY)
{
	PatternListPtr pPatList = m_pPartCadData->GetPatternList();
	PatternLoopListPtr pPatternLoopList = m_pPartCadData->GetPatternLoopList();
	LoopTopologyItemListPtr pLoopTopologyItems = m_pPartCadData->GetLoopTopologyItems_FW();

	if (m_editState != EDIT_TRIANGLE_NOTSTART)
	{
		/************************************************************************/		
		// create the new TriangleHolePattern with the new data.

		// calculate the length(width)
		double dTmpEndPtx,dTmpEndPtY;
		MathEx::RotatePoint(m_data[0], m_data[1], -m_data[6], m_data[2], m_data[3], dTmpEndPtx, dTmpEndPtY);
		double dWidth = (dTmpEndPtx - m_data[0])*2;

		// calculate the height
		double dTmpTopPtX,dTmpTopPtY;
		MathEx::RotatePoint(m_data[0], m_data[1], -m_data[6], m_data[4], m_data[5], dTmpTopPtX, dTmpTopPtY);
		double dHeight = dTmpTopPtY - m_data[1];

		// create TriangleHolePattern.
		Point2D centerPt(m_data[0], m_data[1]);
		IPatternPtr pTRHolePat(new TriangleHolePattern(centerPt, m_data[6], dWidth, dHeight));
		/************************************************************************/

		// create command
		EditorData editorData = GetEditorData();
		ICommandPtr pCommand(new GripEditCommand(editorData, pTRHolePat, pPatternLoopList, pLoopTopologyItems, m_pGripEditPat));
		pCommand->Do();
		CommandManager::GetInstance()->AddCommand(pCommand);

		// enter the selecting pattern action.
		PostMessage2EnterPatternSelectAction(m_pGripEditPat.get());
	}
	else
	{
		if (m_drawState == DRAW_TRIANGLE_NOTSTART || m_drawState == DRAW_TRIANGLE_THIRD_POINT)
		{
			m_drawState = DRAW_TRIANGLE_FIRST_POINT;
			m_iMsgID = IDS_OUTPUT_TRIHOLE_ANGLE_WIDTH;

			m_data[0] = dPtX;
			m_data[1] = dPtY;
			m_data[2] = dPtX;
			m_data[3] = dPtY;
			m_data[4] = dPtX;
			m_data[5] = dPtY;
		}
		else if(m_drawState == DRAW_TRIANGLE_FIRST_POINT)
		{
			m_drawState = DRAW_TRIANGLE_SECOND_POINT;
			m_iMsgID = IDS_OUTPUT_TRIHOLE_HEIGHT;

			m_data[2] = dPtX;
			m_data[3] = dPtY;
			m_data[6] = atan2((dPtY - m_data[1]), (dPtX - m_data[0]));

			double dX1 = m_data[0];
			double dY1 = m_data[1];
			double dAngle = m_data[6];

			double dTmpPtX, dTmpPtY;
			MathEx::RotatePoint(dX1, dY1, -dAngle, dPtX, dPtY, dTmpPtX, dTmpPtY);

			double dTmp = sqrt((dTmpPtX-dX1)*(dTmpPtX-dX1)+(dTmpPtY-dY1)*(dTmpPtY-dY1));
			if ((dTmpPtY - dY1) < 0)
			{
				dTmp = -dTmp;
			}

			double dTmpX2 = dX1;
			double dTmpY2 = dY1 + dTmp;
			double dX2, dY2;
			MathEx::RotatePoint(dX1, dY1, dAngle, dTmpX2, dTmpY2, dX2, dY2);

			m_data[4] = dX2;
			m_data[5] = dY2;
		}
		else if(m_drawState == DRAW_TRIANGLE_SECOND_POINT)
		{
			m_drawState = DRAW_TRIANGLE_THIRD_POINT;
			m_iMsgID = IDS_OUTPUT_TRIHOLE_BASEPT;

			double dX1 = m_data[0];
			double dY1 = m_data[1];
			double dAngle = m_data[6];

			double dTmpPtX, dTmpPtY;
			MathEx::RotatePoint(dX1, dY1, -dAngle, dPtX, dPtY, dTmpPtX, dTmpPtY);

			double dTmp = sqrt((dTmpPtX-dX1)*(dTmpPtX-dX1)+(dTmpPtY-dY1)*(dTmpPtY-dY1));
			if ((dTmpPtY - dY1) < 0)
			{
				dTmp = -dTmp;
			}

			double dTmpX2 = dX1;
			double dTmpY2 = dY1 + dTmp;
			double dX2, dY2;
			MathEx::RotatePoint(dX1, dY1, dAngle, dTmpX2, dTmpY2, dX2, dY2);

			m_data[4] = dX2;
			m_data[5] = dY2;

			// calculate the length(width)
			double dTmpEndPtx,dTmpEndPtY;
			MathEx::RotatePoint(m_data[0], m_data[1], -m_data[6], m_data[2], m_data[3], dTmpEndPtx, dTmpEndPtY);
			double dWidth = (dTmpEndPtx - m_data[0])*2;

			// calculate the height
			double dTmpTopPtX, dTmpTopPtY;
			MathEx::RotatePoint(m_data[0], m_data[1], -m_data[6], m_data[4], m_data[5], dTmpTopPtX, dTmpTopPtY);
			double dHeight = dTmpTopPtY - m_data[1];

			// create TriangleHolePattern.
			Point2D centerPt(m_data[0], m_data[1]);
			IPatternPtr pTRHolePat(new TriangleHolePattern(centerPt, m_data[6], dWidth, dHeight));

			// create command
			EditorData editorData = GetEditorData();
			ICommandPtr pCommand(new PatternAddCommand(editorData, pPatList, pPatternLoopList, pLoopTopologyItems, pTRHolePat));
			pCommand->Do();
			CommandManager::GetInstance()->AddCommand(pCommand);

			// enter the selecting pattern action.
			PostMessage2EnterPatternSelectAction(pTRHolePat.get());
		}
	}

	return TRUE;
}

BOOL TriangleHolePatternEditAction::MovePoint(double& dPtX, double& dPtY)
{
	if (m_editState != EDIT_TRIANGLE_NOTSTART) //EDIT mode
	{
		if (m_editState == EDIT_TRIANGLE_BASE_POINT)
		{
			double dX = dPtX - m_data[0];
			double dY = dPtY - m_data[1];

			// get start point and end point after move
			m_data[0] += dX;
			m_data[1] += dY;
			m_data[2] += dX;
			m_data[3] += dY;
			m_data[4] += dX;
			m_data[5] += dY;  

		}
		else if(m_editState == EDIT_TRIANGLE_LEFT_POINT || m_editState == EDIT_TRIANGLE_RIGHT_POINT)
		{
			double dX1 = m_data[0];
			double dY1 = m_data[1];

			double dAngle = m_data[6];
			double dTmpPtX, dTmpPtY;
			MathEx::RotatePoint(dX1, dY1, -dAngle, dPtX, dPtY, dTmpPtX, dTmpPtY);

			double dTmpX3, dTmpY3, dX3, dY3;
			dTmpY3 = m_data[1];
			if (dTmpPtX > m_data[0])
				dTmpX3 = dTmpPtX;
			else
				dTmpX3 = 2*m_data[0] - dTmpPtX;
			MathEx::RotatePoint(dX1, dY1, dAngle, dTmpX3, dTmpY3, dX3, dY3);

			m_data[2] = dX3;
			m_data[3] = dY3;
		}
		else if (m_editState == EDIT_TRIANGLE_TOP_POINT)
		{
			double dX1 = m_data[0];
			double dY1 = m_data[1];
			double dAngle = m_data[6];

			double dTmpPtX, dTmpPtY;
			MathEx::RotatePoint(dX1, dY1, -dAngle, dPtX, dPtY, dTmpPtX, dTmpPtY);

			double dTmp = sqrt((dTmpPtX-dX1)*(dTmpPtX-dX1)+(dTmpPtY-dY1)*(dTmpPtY-dY1));
			if ((dTmpPtY - dY1) < 0)
			{
				dTmp = -dTmp;
			}

			double dTmpX2 = dX1;
			double dTmpY2 = dY1 + dTmp;
			double dX2, dY2;
			MathEx::RotatePoint(dX1, dY1, dAngle, dTmpX2, dTmpY2, dX2, dY2);

			m_data[4] = dX2;
			m_data[5] = dY2;
		}
	}
	else
	{
		if (m_drawState == DRAW_TRIANGLE_FIRST_POINT)
		{
			m_data[2] = dPtX;
			m_data[3] = dPtY;
			m_data[6] = atan2((dPtY - m_data[1]), (dPtX - m_data[0]));
		}
		else if(m_drawState == DRAW_TRIANGLE_SECOND_POINT)
		{
			double dX1 = m_data[0];
			double dY1 = m_data[1];
			double dAngle = m_data[6];

			double dTmpPtX, dTmpPtY;
			MathEx::RotatePoint(dX1, dY1, -dAngle, dPtX, dPtY, dTmpPtX, dTmpPtY);

			double dTmp = sqrt((dTmpPtX-dX1)*(dTmpPtX-dX1)+(dTmpPtY-dY1)*(dTmpPtY-dY1));
			if ((dTmpPtY - dY1) < 0)
			{
				dTmp = -dTmp;
			}

			double dTmpX2 = dX1;
			double dTmpY2 = dY1 + dTmp;
			double dX2, dY2;
			MathEx::RotatePoint(dX1, dY1, dAngle, dTmpX2, dTmpY2, dX2, dY2);

			m_data[4] = dX2;
			m_data[5] = dY2;
		}
	}

	return TRUE;
}

void TriangleHolePatternEditAction::ConvertKBInput(double& dVal1, double& dVal2)
{
	if (m_editState != EDIT_TRIANGLE_NOTSTART)
	{
		// under "edit" action.

		if (m_editState == EDIT_TRIANGLE_BASE_POINT)
		{
			// now, do not need to convert.
			;
		}
		else if(m_editState == EDIT_TRIANGLE_LEFT_POINT ||
			m_editState == EDIT_TRIANGLE_RIGHT_POINT)
		{
			// now, "dVal1" holds the width

			Vector2D v1;
			v1.SetAngle(m_data[6]);
			v1.Magnitude(dVal1/2);

			Point2D basePt(m_data[0], m_data[1]);
			basePt.Transfer(v1);

			dVal1 = basePt.X();
			dVal2 = basePt.Y();
		}
		else if(m_editState == EDIT_TRIANGLE_TOP_POINT)
		{
			// now, "dVal1" holds the height

			// get the vector along the height of the triangle.
			// notes: we should check whether the "height" is less than zero.
			Vector2D v1;
			v1.SetAngle(m_data[6]);
			if (dVal1 < 0)
				v1.Rotate(-PID2);
			else
				v1.Rotate(PID2);
			v1.Magnitude(dVal1);

			Point2D basePt(m_data[0], m_data[1]);
			basePt.Transfer(v1);

			dVal1 = basePt.X();
			dVal2 = basePt.Y();
		}
	}
	else
	{
		// under "draw" action.

		if (m_drawState == DRAW_TRIANGLE_NOTSTART || m_drawState == DRAW_TRIANGLE_THIRD_POINT)
		{
			// now, do not need to convert.
			;
		}
		else if(m_drawState == DRAW_TRIANGLE_FIRST_POINT)
		{
			// now, "dVal1" "dVal2" hold the angle and width.
			
			Vector2D v1;
			v1.SetAngle(dVal1);
			v1.Magnitude(dVal2/2);

			Point2D basePt(m_data[0], m_data[1]);
			basePt.Transfer(v1);

			dVal1 = basePt.X();
			dVal2 = basePt.Y();
		}
		else if(m_drawState == DRAW_TRIANGLE_SECOND_POINT)
		{
			// now, "dVal1" holds the height
			
			// get the vector along the height of the triangle.
			// notes: we should check whether the "height" is less than zero.
			Vector2D v1;
			v1.SetAngle(m_data[6]);
			if (dVal1 < 0)
				v1.Rotate(-PID2);
			else
				v1.Rotate(PID2);
			v1.Magnitude(dVal1);

			Point2D basePt(m_data[0], m_data[1]);
			basePt.Transfer(v1);

			dVal1 = basePt.X();
			dVal2 = basePt.Y();
		}
	}
}

BOOL TriangleHolePatternEditAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	PatternEditActionBase::OnKeyDown(nChar, nRepCnt, nFlags);

	return TRUE;
}

BOOL TriangleHolePatternEditAction::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	return TRUE;
}

BOOL TriangleHolePatternEditAction::DrawBeforeFinish()
{
	CLOptionInfoPtr pCLOptionInfo = CLOptionInfo::GetInstance();
	DisplayParamPtr pColorInfo = pCLOptionInfo->GetDisplayParam();

	// keep the old color.
	COLORREF dwOldColor;
	m_pViewPort->GetDrawColor(dwOldColor);

	//
	m_pViewPort->SetDrawColor(*pColorInfo->GetPatColor());

	double dX1 = m_data[0];
	double dY1 = m_data[1];
	double dX2 = m_data[4];
	double dY2 = m_data[5];
	double dX3 = m_data[2];
	double dY3 = m_data[3];
	double dAngle = m_data[6];

	double dX4, dY4, dX3r, dY3r, dX4r, dY4r;
	MathEx::RotatePoint(dX1, dY1, -dAngle, dX3, dY3, dX3r, dY3r);

	dX4r = 2*dX1 - dX3r;
	dY4r = dY3r;

	MathEx::RotatePoint(dX1, dY1, dAngle, dX4r, dY4r, dX4, dY4);

	if (m_drawState == DRAW_TRIANGLE_FIRST_POINT)
	{
		m_pViewPort->DrawLinePoint2D(dX4, dY4, dX3, dY3);
	}
	else if ((m_drawState == DRAW_TRIANGLE_SECOND_POINT) || (m_editState != EDIT_TRIANGLE_NOTSTART))
	{
		m_pViewPort->DrawLinePoint2D(dX4, dY4, dX3, dY3);
		m_pViewPort->DrawLinePoint2D(dX3, dY3, dX2, dY2);
		m_pViewPort->DrawLinePoint2D(dX2, dY2, dX4, dY4);
	}

	// restore the draw color
	m_pViewPort->SetDrawColor(dwOldColor);

	// draw cursor.
	CursorDrawer::DrawCrossLineCursor(m_pViewPort);

	return TRUE;
}

void TriangleHolePatternEditAction::UpdateForUndoRedo()
{
	PatternListPtr pPatList = m_pPartCadData->GetPatternList();
	if (m_editState != EDIT_TRIANGLE_NOTSTART && !pPatList->GetPatternFromID(m_pGripEditPat->GetID()))
	{
		PostMessage2EnterPatternSelectAction();
	}
}

void TriangleHolePatternEditAction::SetGripEditPattern(IPatternPtr pPattern)
{
	m_pGripEditPat = pPattern;

	TriangleHolePattern* pTRHolePat = dynamic_cast<TriangleHolePattern*>(pPattern.get());
	Point2D centerPt = pTRHolePat->GetBasePoint();

	// center point
	m_data[0] = centerPt.X();
	m_data[1] = centerPt.Y();

	// end point
	Point2D endPt = centerPt;
	Vector2D v1 = pTRHolePat->GetBaseVector();
	endPt.Transfer(v1/2);
	m_data[2] = endPt.X();
	m_data[3] = endPt.Y();

	// top point
	Point2D topPt = centerPt;
	topPt.Transfer(pTRHolePat->GetHeightVector());
	m_data[4] = topPt.X();
	m_data[5] = topPt.Y();

	// angle
	m_data[6] = pTRHolePat->GetBaseVector().Angle();
}

void TriangleHolePatternEditAction::SetGripEditPt(double dPtX, double dPtY)
{
	// get snap points of the pattern
	Point2DList snapPts;
	m_pGripEditPat->GetSnapPoints(snapPts);

	Point2D tmpPt(dPtX, dPtY);
	for(unsigned int i = 0; i < snapPts.size(); i++)
	{
		Point2D snapPt = (Point2D)snapPts[i];
		if (snapPt.IsEqual(tmpPt))
		{
			if (i == 0)
			{
				m_editState = EDIT_TRIANGLE_BASE_POINT;
				m_iMsgID = IDS_OUTPUT_TRIHOLE_BASEPT;
			}
			else if (i == 1)
			{
				m_editState = EDIT_TRIANGLE_RIGHT_POINT;
				m_iMsgID = IDS_OUTPUT_TRIHOLE_WIDTH;
			}
			else if (i == 2)
			{
				m_editState = EDIT_TRIANGLE_LEFT_POINT;
				m_iMsgID = IDS_OUTPUT_TRIHOLE_WIDTH;
			}
			else if (i == 3)
			{
				m_editState = EDIT_TRIANGLE_TOP_POINT;
				m_iMsgID = IDS_OUTPUT_TRIHOLE_HEIGHT;
			}

			return;
		}
	}
}

END_CUTLEADER_NAMESPACE()

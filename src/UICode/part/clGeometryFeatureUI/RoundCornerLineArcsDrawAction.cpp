#include "StdAfx.h"
#include "RoundCornerLineArcsDrawAction.h"

#include "GlViewPort.h"
#include "CommandManager.h"
#include "ICommand.h"
#include "MathEx.h"
#include "ValueInputManager.h"
#include "clUILibResource.h"
#include "CursorDrawer.h"

#include "CLOptionInfo.h"
#include "DisplayParam.h"

#include "PatternList.h"
#include "LinePattern.h"
#include "ArcPattern.h"
#include "PatternAddCommand.h"
#include "PatternSelectAction.h"
#include "clGeometryFeatureResource.h"
#include "PatternPropertyBar.h"
#include "PartCadData.h"
#include "TextStructList.h"

BEGIN_CUTLEADER_NAMESPACE()

RoundCornerLineArcsDrawAction::RoundCornerLineArcsDrawAction(PartCadDataPtr pPartCadData, GlViewPortPtr pViewPort)
{
	m_pPartCadData = pPartCadData;
	m_pViewPort = pViewPort;

	m_drawState = DRAW_RCLINE_NOTSTART;
	memset(m_data, 0, sizeof(double)*6);

	m_iMsgID = IDS_OUTPUT_RCLINE_CENTERPT;
}

RoundCornerLineArcsDrawAction::~RoundCornerLineArcsDrawAction(void)
{
}

BOOL RoundCornerLineArcsDrawAction::LButtonDown(double& dPtX, double& dPtY)
{
	if (m_drawState == DRAW_RCLINE_NOTSTART || 
		m_drawState == DRAW_RCLINE_THIRD_POINT) //first click
	{
		m_drawState = DRAW_RCLINE_FIRST_POINT;
		m_iMsgID = IDS_OUTPUT_RCLINE_LENGTH_WIDTH;

		m_data[0] = dPtX;
		m_data[1] = dPtY;
		m_data[2] = 0;
		m_data[3] = 0;
		m_data[4] = 0;
		m_data[5] = 0;
	}
	else if(m_drawState == DRAW_RCLINE_FIRST_POINT)
	{
		m_drawState = DRAW_RCLINE_SECOND_POINT;
		m_iMsgID = IDS_OUTPUT_RCLINE_RADIUS;

		m_data[2] = 2 * fabs(dPtX - m_data[0]);
		m_data[3] = 2 * fabs(dPtY - m_data[1]);
	}
	else if(m_drawState == DRAW_RCLINE_SECOND_POINT)
	{
		m_drawState = DRAW_RCLINE_THIRD_POINT;
		m_iMsgID = IDS_OUTPUT_RCLINE_CENTERPT;

		double radius = sqrt((dPtX - m_data[0])*(dPtX - m_data[0])+
							 (dPtY - m_data[1])*(dPtY - m_data[1]));

		double minSize = (m_data[3] < m_data[2]) ? m_data[3] : m_data[2];
		if (radius > minSize/2.0 ) 
			radius = minSize/2.0;

		m_data[4] = radius;

		// create 4 lines and 4 arcs patterns.
		CreateRCLinePat();
	}

	return TRUE;
}

void RoundCornerLineArcsDrawAction::CreateRCLinePat()
{
	EditorData editorData = GetEditorData();
	PatternListPtr pPatList = m_pPartCadData->GetPatternList();
	PatternLoopListPtr pPatternLoopList = m_pPartCadData->GetPatternLoopList();
	LoopTopologyItemListPtr pLoopTopologyItems = m_pPartCadData->GetLoopTopologyItems_FW();

	int iSign = 1;
	double dAngle = 0;

	PatternListPtr pSelPatList(new PatternList);

	/************************************************************************/
	// create first arc pattern.

	// First corner
	double dAngle1 = atan2((m_data[3]/2.0 - m_data[4] * iSign), (m_data[2]/2.0 - m_data[4] * iSign));
	dAngle = dAngle1 + m_data[5];
	double dDistance = sqrt(pow(m_data[3]/2.0 - m_data[4] * iSign, 2.0) + pow(m_data[2]/2.0 - m_data[4] * iSign, 2.0));
	double ArcCenPtX1 = m_data[0] + dDistance*cos(dAngle);
	double ArcCenPtY1 = m_data[1] + dDistance*sin(dAngle);

	Point2D arcCenter1(ArcCenPtX1, ArcCenPtY1);
	IPatternPtr pArcPat1(new ArcPattern(arcCenter1, m_data[4], m_data[5], PID2));
	pSelPatList->push_back(pArcPat1);

	// create command
	ICommandPtr pCommand1(new PatternAddCommand(editorData, pPatList, pPatternLoopList, pLoopTopologyItems, pArcPat1));
	pCommand1->Do();
	CommandManager::GetInstance()->AddCommand(pCommand1);
	/************************************************************************/

	/************************************************************************/
	// create second arc pattern.

	double dArcStartX1 = ArcCenPtX1 + m_data[4] * cos(m_data[5]);
	double dArcStartY1 = ArcCenPtY1 + m_data[4] * sin(m_data[5]);
	double dArcEndX1 = ArcCenPtX1 + m_data[4] * cos(m_data[5] + PID2);
	double dArcEndY1 = ArcCenPtY1 + m_data[4] * sin(m_data[5] + PID2);

	// Second Corner
	dAngle = PI - dAngle1 + m_data[5];
	double dArcCenPtX2 = m_data[0] + dDistance*cos(dAngle);
	double dArcCenPtY2 = m_data[1] + dDistance*sin(dAngle);

	Point2D arcCenter2(dArcCenPtX2, dArcCenPtY2);
	IPatternPtr pArcPat2(new ArcPattern(arcCenter2, m_data[4], m_data[5]+PID2, PID2));
	pSelPatList->push_back(pArcPat2);

	// create command
	ICommandPtr pCommand2(new PatternAddCommand(editorData, pPatList, pPatternLoopList, pLoopTopologyItems, pArcPat2));
	pCommand2->Do();
	CommandManager::GetInstance()->AddCommand(pCommand2);
	/************************************************************************/

	/************************************************************************/
	// create third arc pattern.

	double dArcStartX2 = dArcCenPtX2 + m_data[4] * cos(m_data[5] + PID2);
	double dArcStartY2 = dArcCenPtY2 + m_data[4] * sin(m_data[5] + PID2);
	double dArcEndX2 = dArcCenPtX2 + m_data[4] * cos(m_data[5] + PI);
	double dArcEndY2 = dArcCenPtY2 + m_data[4] * sin(m_data[5] + PI);

	// Third corner
	dAngle = dAngle1 + m_data[5] + PI;
	double ArcCenPtX3 = m_data[0] + dDistance*cos(dAngle);
	double ArcCenPtY3 = m_data[1] + dDistance*sin(dAngle);

	Point2D arcCenter3(ArcCenPtX3, ArcCenPtY3);
	IPatternPtr pArcPat3(new ArcPattern(arcCenter3, m_data[4], m_data[5]+PI, PID2));
	pSelPatList->push_back(pArcPat3);

	// create command
	ICommandPtr pCommand3(new PatternAddCommand(editorData, pPatList, pPatternLoopList, pLoopTopologyItems, pArcPat3));
	pCommand3->Do();
	CommandManager::GetInstance()->AddCommand(pCommand3);
	/************************************************************************/

	/************************************************************************/
	// create fourth arc pattern.

	double dArcStartX3 = ArcCenPtX3 + m_data[4] * cos(m_data[5] + PI);
	double dArcStartY3 = ArcCenPtY3 + m_data[4] * sin(m_data[5] + PI);
	double dArcEndX3 = ArcCenPtX3 + m_data[4] * cos(m_data[5] - PID2);
	double dArcEndY3 = ArcCenPtY3 + m_data[4] * sin(m_data[5] - PID2);

	// Fourth corner
	dAngle = -dAngle1 + m_data[5];
	double dArcCenPtX4 = m_data[0] + dDistance*cos(dAngle);
	double dArcCenPtY4 = m_data[1] + dDistance*sin(dAngle);

	Point2D arcCenter4(dArcCenPtX4, dArcCenPtY4);
	IPatternPtr pArcPat4(new ArcPattern(arcCenter4, m_data[4], m_data[5]-PID2, PID2));
	pSelPatList->push_back(pArcPat4);

	// create command
	ICommandPtr pCommand4(new PatternAddCommand(editorData, pPatList, pPatternLoopList, pLoopTopologyItems, pArcPat4));
	pCommand4->Do();
	CommandManager::GetInstance()->AddCommand(pCommand4);
	/************************************************************************/

	/************************************************************************/
	// create four line patterns.

	double dArcStartX4 = dArcCenPtX4 + m_data[4] * cos(m_data[5] - PID2);
	double dArcStartY4 = dArcCenPtY4 + m_data[4] * sin(m_data[5] - PID2);
	double dArcEndX4 = dArcCenPtX4 + m_data[4] * cos(m_data[5]);
	double dArcEndY4 = dArcCenPtY4 + m_data[4] * sin(m_data[5]);

	// create line pattern.
	LinePatternPtr pLinePat1(new LinePattern(Point2D(dArcEndX1, dArcEndY1), Point2D(dArcStartX2, dArcStartY2)));
	if (pLinePat1->GetLength() > GEOM_TOLERANCE)
	{
		pSelPatList->push_back(pLinePat1);

		// create command
		ICommandPtr pCommand5(new PatternAddCommand(editorData, pPatList, pPatternLoopList, pLoopTopologyItems, pLinePat1));
		pCommand5->Do();
		CommandManager::GetInstance()->AddCommand(pCommand5);
	}

	// create line pattern.
	LinePatternPtr pLinePat2(new LinePattern(Point2D(dArcEndX2, dArcEndY2), Point2D(dArcStartX3, dArcStartY3)));
	if (pLinePat2->GetLength() > GEOM_TOLERANCE)
	{
		pSelPatList->push_back(pLinePat2);

		// create command
		ICommandPtr pCommand6(new PatternAddCommand(editorData, pPatList, pPatternLoopList, pLoopTopologyItems, pLinePat2));
		pCommand6->Do();
		CommandManager::GetInstance()->AddCommand(pCommand6);
	}

	// create line pattern.
	LinePatternPtr pLinePat3(new LinePattern(Point2D(dArcEndX3, dArcEndY3), Point2D(dArcStartX4, dArcStartY4)));
	if (pLinePat3->GetLength() > GEOM_TOLERANCE)
	{
		pSelPatList->push_back(pLinePat3);

		// create command
		ICommandPtr pCommand7(new PatternAddCommand(editorData, pPatList, pPatternLoopList, pLoopTopologyItems, pLinePat3));
		pCommand7->Do();
		CommandManager::GetInstance()->AddCommand(pCommand7);
	}

	// create line pattern.
	LinePatternPtr pLinePat4(new LinePattern(Point2D(dArcEndX4, dArcEndY4), Point2D(dArcStartX1, dArcStartY1)));
	if (pLinePat4->GetLength() > GEOM_TOLERANCE)
	{
		pSelPatList->push_back(pLinePat4);

		// create command
		ICommandPtr pCommand8(new PatternAddCommand(editorData, pPatList, pPatternLoopList, pLoopTopologyItems, pLinePat4));
		pCommand8->Do();
		CommandManager::GetInstance()->AddCommand(pCommand8);
	}
	/************************************************************************/

	// enter the selecting pattern action.
	TextStructListPtr pTextStructs(new TextStructList);
	PostMessage2EnterPatternSelectAction(pSelPatList.get(), pTextStructs.get());
}

BOOL RoundCornerLineArcsDrawAction::MovePoint(double& dPtX, double& dPtY)
{
	if (m_drawState == DRAW_RCLINE_FIRST_POINT)
	{
		m_data[2] = 2 * fabs(dPtX - m_data[0]);
		m_data[3] = 2 * fabs(dPtY - m_data[1]);
	}
	else if(m_drawState == DRAW_RCLINE_SECOND_POINT)
	{
		double dRadius = sqrt((dPtX - m_data[0])*(dPtX - m_data[0])+
							 (dPtY - m_data[1])*(dPtY - m_data[1]));

		double dMinSize = (m_data[3] < m_data[2]) ? m_data[3] : m_data[2];
		if (dRadius > dMinSize/2.0 ) 
			dRadius = dMinSize/2.0;

		m_data[4] = dRadius;
	}

	return TRUE;
}

void RoundCornerLineArcsDrawAction::ConvertKBInput(double& dVal1, double& dVal2)
{
	if (m_drawState == DRAW_RCLINE_NOTSTART || 
		m_drawState == DRAW_RCLINE_THIRD_POINT)
	{
		// now, do not need to convert.
		;
	}
	else if(m_drawState == DRAW_RCLINE_FIRST_POINT)
	{
		// now, "dVal1" holds the length and
		// the "dVal2" holds the width.

		Point2D centerPt(m_data[0], m_data[1]);
		centerPt.X(centerPt.X()+dVal1/2);
		centerPt.Y(centerPt.Y()+dVal2/2);

		dVal1 = centerPt.X();
		dVal2 = centerPt.Y();
	}
	else if(m_drawState == DRAW_RCLINE_SECOND_POINT)
	{
		// now, "dVal1" holds the corner radius

		Point2D centerPt(m_data[0], m_data[1]);
		centerPt.X(centerPt.X()+dVal1);

		dVal1 = centerPt.X();
		dVal2 = centerPt.Y();
	}
}

BOOL RoundCornerLineArcsDrawAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	PatternEditActionBase::OnKeyDown(nChar, nRepCnt, nFlags);

	return TRUE;
}

BOOL RoundCornerLineArcsDrawAction::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	return TRUE;
}

BOOL RoundCornerLineArcsDrawAction::DrawBeforeFinish()
{
	CLOptionInfoPtr pCLOptionInfo = CLOptionInfo::GetInstance();
	DisplayParamPtr pColorInfo = pCLOptionInfo->GetDisplayParam();

	// keep the old color.
	COLORREF dwOldColor;
	m_pViewPort->GetDrawColor(dwOldColor);

	//
	m_pViewPort->SetDrawColor(*pColorInfo->GetPatColor());

	if (m_drawState == DRAW_RCLINE_FIRST_POINT)
	{
		// Draw rectangle
		double dAngle = 0;
		double dAngle1 = atan2(m_data[3]/2.0,m_data[2]/2.0);
		dAngle = dAngle1 + m_data[5];
		double dDistance = sqrt(pow(m_data[3]/2.0, 2.0) + pow(m_data[2]/2.0, 2.0));

		// First corner
		double dPtX1 = m_data[0] + dDistance*cos(dAngle);
		double dPtY1 = m_data[1] + dDistance*sin(dAngle);

		// Second corner
		dAngle = PI - dAngle1 + m_data[5];
		double dPtX2 = m_data[0] + dDistance*cos(dAngle);
		double dPtY2 = m_data[1] + dDistance*sin(dAngle);

		// Third corner
		dAngle = dAngle1 + m_data[5] + PI;
		double dPtX3 = m_data[0] + dDistance*cos(dAngle);
		double dPtY3 = m_data[1] + dDistance*sin(dAngle);

		// Fourth corner
		dAngle = -dAngle1 + m_data[5];
		double dPtX4 = m_data[0] + dDistance*cos(dAngle);
		double dPtY4 = m_data[1] + dDistance*sin(dAngle);

		m_pViewPort->DrawLinePoint2D(dPtX1, dPtY1, dPtX2, dPtY2);
		m_pViewPort->DrawLinePoint2D(dPtX2, dPtY2, dPtX3, dPtY3);
		m_pViewPort->DrawLinePoint2D(dPtX3, dPtY3, dPtX4, dPtY4);
		m_pViewPort->DrawLinePoint2D(dPtX4, dPtY4, dPtX1, dPtY1);
	}
	else if(m_drawState == DRAW_RCLINE_SECOND_POINT)
	{
		int iSign = 1;
		double dAngle = 0;

		// First corner
		double dAngle1 = atan2((m_data[3]/2.0 - m_data[4] * iSign), (m_data[2]/2.0 - m_data[4] * iSign));
		dAngle = dAngle1 + m_data[5];
		double dDistance = sqrt(pow(m_data[3]/2 - m_data[4] * iSign, 2) + pow(m_data[2]/2 - m_data[4] * iSign, 2));
		double dArcCenPtX1 = m_data[0] + dDistance*cos(dAngle);
		double dArcCenPtY1 = m_data[1] + dDistance*sin(dAngle);
		m_pViewPort->DrawArc2D(dArcCenPtX1, dArcCenPtY1, m_data[4], m_data[5], PID2);

		double dArcStartX1 = dArcCenPtX1 + m_data[4] * cos(m_data[5]);
		double dArcStartY1 = dArcCenPtY1 + m_data[4] * sin(m_data[5]);
		double dArcEndX1 = dArcCenPtX1 + m_data[4] * cos(m_data[5] + PID2);
		double dArcEndY1 = dArcCenPtY1 + m_data[4] * sin(m_data[5] + PID2);

		// Second Corner
		dAngle = PI - dAngle1 + m_data[5];
		double dArcCenPtX2 = m_data[0] + dDistance*cos(dAngle);
		double dArcCenPtY2 = m_data[1] + dDistance*sin(dAngle);
		m_pViewPort->DrawArc2D(dArcCenPtX2, dArcCenPtY2, m_data[4], m_data[5] + PID2, PID2);

		double dArcStartX2 = dArcCenPtX2 + m_data[4] * cos(m_data[5] + PID2);
		double dArcStartY2 = dArcCenPtY2 + m_data[4] * sin(m_data[5] + PID2);
		double dArcEndX2 = dArcCenPtX2 + m_data[4] * cos(m_data[5] + PI);
		double dArcEndY2 = dArcCenPtY2 + m_data[4] * sin(m_data[5] + PI);

		// Third corner
		dAngle = dAngle1 + m_data[5] + PI;
		double dArcCenPtX3 = m_data[0] + dDistance*cos(dAngle);
		double dArcCenPtY3 = m_data[1] + dDistance*sin(dAngle);
		m_pViewPort->DrawArc2D(dArcCenPtX3, dArcCenPtY3, m_data[4], m_data[5] + PI, PID2);

		double dArcStartX3 = dArcCenPtX3 + m_data[4] * cos(m_data[5] + PI);
		double dArcStartY3 = dArcCenPtY3 + m_data[4] * sin(m_data[5] + PI);
		double dArcEndX3 = dArcCenPtX3 + m_data[4] * cos(m_data[5] - PID2);
		double dArcEndY3 = dArcCenPtY3 + m_data[4] * sin(m_data[5] - PID2);

		// Fourth corner
		dAngle = -dAngle1 + m_data[5];
		double dArcCenPtX4 = m_data[0] + dDistance*cos(dAngle);
		double dArcCenPtY4 = m_data[1] + dDistance*sin(dAngle);
		m_pViewPort->DrawArc2D(dArcCenPtX4, dArcCenPtY4, m_data[4], m_data[5] - PID2, PID2);

		double dArcStartX4 = dArcCenPtX4 + m_data[4] * cos(m_data[5] - PID2);
		double dArcStartY4 = dArcCenPtY4 + m_data[4] * sin(m_data[5] - PID2);
		double dArcEndX4 = dArcCenPtX4 + m_data[4] * cos(m_data[5]);
		double dArcEndY4 = dArcCenPtY4 + m_data[4] * sin(m_data[5]);

		// Draw four lines
		m_pViewPort->DrawLinePoint2D(dArcEndX1, dArcEndY1, dArcStartX2, dArcStartY2);
		m_pViewPort->DrawLinePoint2D(dArcEndX2, dArcEndY2, dArcStartX3, dArcStartY3);
		m_pViewPort->DrawLinePoint2D(dArcEndX3, dArcEndY3, dArcStartX4, dArcStartY4);
		m_pViewPort->DrawLinePoint2D(dArcEndX4, dArcEndY4, dArcStartX1, dArcStartY1);
	}

	// draw cursor.
	CursorDrawer::DrawCrossLineCursor(m_pViewPort);

	// restore the draw color
	m_pViewPort->SetDrawColor(dwOldColor);

	return TRUE;
}

END_CUTLEADER_NAMESPACE()

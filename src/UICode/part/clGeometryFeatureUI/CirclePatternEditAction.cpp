#include "StdAfx.h"
#include "CirclePatternEditAction.h"

#include "Point2D.h"
#include "GlViewPort.h"
#include "ICommand.h"
#include "CommandManager.h"
#include "ValueInputManager.h"
#include "clUILibResource.h"
#include "CursorDrawer.h"
#include "Point2DList.h"

#include "CLOptionInfo.h"
#include "DisplayParam.h"

#include "PatternSelectAction.h"
#include "CirclePattern.h"
#include "PatternList.h"
#include "PatternAddCommand.h"
#include "GripEditCommand.h"
#include "clGeometryFeatureResource.h"
#include "PatternPropertyBar.h"
#include "PartCadData.h"

BEGIN_CUTLEADER_NAMESPACE()

CirclePatternEditAction::CirclePatternEditAction(PartCadDataPtr pPartCadData, GlViewPortPtr pViewPort)
{
	m_drawState = DRAW_CIRCLE_NOTSTART;
	m_editState = EDIT_CIRCLE_NOTSTART;
	memset(m_data, 0, sizeof(double)*3);

	m_pPartCadData = pPartCadData;
	m_pViewPort = pViewPort;

	m_iMsgID = IDS_OUTPUT_PATCIRCLE_CENTERPT;
}

CirclePatternEditAction::~CirclePatternEditAction(void)
{
}

BOOL CirclePatternEditAction::LButtonDown(double& dPtX, double& dPtY)
{
	PatternListPtr pPatList = m_pPartCadData->GetPatternList();
	PatternLoopListPtr pPatternLoopList = m_pPartCadData->GetPatternLoopList();
	LoopTopologyItemListPtr pLoopTopologyItems = m_pPartCadData->GetLoopTopologyItems_FW();

	if (m_editState != EDIT_CIRCLE_NOTSTART)
	{
		if (m_editState == EDIT_CIRCLE_CENTER_POINT) {
			m_data[0] = dPtX;
			m_data[1] = dPtY;
		}
		else {
			m_data[2] = sqrt((dPtX - m_data[0])*(dPtX - m_data[0])+(dPtY - m_data[1])*(dPtY - m_data[1]));
		}

		// create the new circle pattern with the new data.
		Point2D basePt(m_data[0], m_data[1]);	
		IPatternPtr pNewPat(new CirclePattern(basePt, m_data[2]));

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
		if (m_drawState == DRAW_CIRCLE_NOTSTART || m_drawState == DRAW_CIRCLE_END_POINT) //first click
		{
			m_drawState = DRAW_CIRCLE_CENTER_POINT;
			m_iMsgID = IDS_OUTPUT_PATCIRCLE_RADIUS;

			m_data[0] = dPtX;
			m_data[1] = dPtY;
			m_data[2] = 0;
		}
		else if(m_drawState == DRAW_CIRCLE_CENTER_POINT) //second click, we can create a circle
		{
			// 检查能不能创建圆。
			double dRadius = sqrt((dPtX - m_data[0])*(dPtX - m_data[0]) + (dPtY - m_data[1])*(dPtY - m_data[1]));
			if (dRadius < GEOM_TOLERANCE) {
				return FALSE;
			}

			m_drawState = DRAW_CIRCLE_END_POINT;
			m_iMsgID = IDS_OUTPUT_PATCIRCLE_CENTERPT;
			m_data[2] = dRadius;

			// create circle pattern.
			Point2D basePt(m_data[0], m_data[1]);
			IPatternPtr pCirclePat(new CirclePattern(basePt, m_data[2]));

			// create command
			EditorData editorData = GetEditorData();
			ICommandPtr pCommand(new PatternAddCommand(editorData, pPatList, pPatternLoopList, pLoopTopologyItems, pCirclePat));
			pCommand->Do();
			CommandManager::GetInstance()->AddCommand(pCommand);
		}
	}

	return TRUE;
}

BOOL CirclePatternEditAction::MovePoint(double& dPtX, double& dPtY)
{
	if (m_editState != EDIT_CIRCLE_NOTSTART) //EDIT mode
	{
		if (m_editState == EDIT_CIRCLE_CENTER_POINT) {
			m_data[0] = dPtX;
			m_data[1] = dPtY;
		}
		else {
			m_data[2] = sqrt((dPtX - m_data[0])*(dPtX - m_data[0])+(dPtY - m_data[1])*(dPtY - m_data[1]));
		}
	}
	else
	{
		if(m_drawState == DRAW_CIRCLE_CENTER_POINT) {
			m_data[2] = sqrt((dPtX - m_data[0])*(dPtX - m_data[0])+(dPtY - m_data[1])*(dPtY - m_data[1]));
		}
	}

	return TRUE;
}

BOOL CirclePatternEditAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	PatternEditActionBase::OnKeyDown(nChar, nRepCnt, nFlags);

	return TRUE;
}

BOOL CirclePatternEditAction::DrawBeforeFinish()
{
	const DisplayParam* pDisplayParam = CLOptionInfo::GetInstance()->GetDisplayParam().get();

	// keep the old color.
	COLORREF dwOldColor;
	m_pViewPort->GetDrawColor(dwOldColor);

	//
	m_pViewPort->SetDrawColor(*pDisplayParam->GetPatColor());
	if (m_drawState == DRAW_CIRCLE_CENTER_POINT || m_editState != EDIT_CIRCLE_NOTSTART) {
		m_pViewPort->DrawCircle2D(m_data[0], m_data[1], m_data[2]);
	}

	// restore the draw color
	m_pViewPort->SetDrawColor(dwOldColor);

	// draw cursor.
	CursorDrawer::DrawCrossLineCursor(m_pViewPort);

	return TRUE;
}

void CirclePatternEditAction::UpdateForUndoRedo()
{
}

void CirclePatternEditAction::SetGripEditPattern(IPatternPtr pPattern)
{
	m_pGripEditPat = pPattern;

	CirclePattern* pCirclePat = dynamic_cast<CirclePattern*>(pPattern.get());

	// fill the m_data with the data in line pattern.
	Point2D startPt = pCirclePat->GetBasePoint();
	m_data[0] = startPt.X();
	m_data[1] = startPt.Y();

	m_data[2] =	pCirclePat->GetRadius();
}

void CirclePatternEditAction::SetGripEditPt(double dPtX, double dPtY)
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
				m_editState = EDIT_CIRCLE_CENTER_POINT;
				m_iMsgID = IDS_OUTPUT_PATCIRCLE_CENTERPT;
			}
			else if (i == 1)
			{
				m_editState = EDIT_CIRCLE_TOP_POINT;
				m_iMsgID = IDS_OUTPUT_PATCIRCLE_RADIUS;
			}
			else if (i == 2)
			{
				m_editState = EDIT_CIRCLE_LEFT_POINT;
				m_iMsgID = IDS_OUTPUT_PATCIRCLE_RADIUS;
			}
			else if (i == 3)
			{
				m_editState = EDIT_CIRCLE_BOTTOM_POINT;
				m_iMsgID = IDS_OUTPUT_PATCIRCLE_RADIUS;
			}
			else if (i == 4)
			{
				m_editState = EDIT_CIRCLE_RIGHT_POINT;
				m_iMsgID = IDS_OUTPUT_PATCIRCLE_RADIUS;
			}

			return;
		}
	}
}

END_CUTLEADER_NAMESPACE()

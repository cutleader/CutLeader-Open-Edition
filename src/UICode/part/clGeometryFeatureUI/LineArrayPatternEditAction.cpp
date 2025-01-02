#include "StdAfx.h"
#include "LineArrayPatternEditAction.h"

#include "MathEx.h"
#include "GlViewPort.h"
#include "ICommand.h"
#include "CommandManager.h"
#include "ValueInputManager.h"
#include "clUILibResource.h"

#include "CLOptionInfo.h"
#include "DisplayParam.h"

#include "PatternSelectAction.h"
#include "PatternList.h"
#include "LineArrayPattern.h"
#include "PatternLoopList.h"
#include "PatternDrawer.h"
#include "ArrayPatternAddCommand.h"
#include "GripEditCommand.h"
#include "clGeometryFeatureResource.h"
#include "PatternPropertyBar.h"
#include "PatternProcessor.h"
#include "PartCadData.h"

BEGIN_CUTLEADER_NAMESPACE()

LineArrayPatternEditAction::LineArrayPatternEditAction(PartCadDataPtr pPartCadData, GlViewPortPtr pViewPort)
{
	m_pPartCadData = pPartCadData;
	m_pViewPort = pViewPort;

	m_drawState = DRAW_ARRAY_NOTSTART;
	m_editState = EDIT_PATTERNLINE_NOTSTART;

	memset(m_data, 0, sizeof(double)*5);

	m_iMsgID = IDS_OUTPUT_PATLINE_PAT;
}

LineArrayPatternEditAction::~LineArrayPatternEditAction(void)
{
}

BOOL LineArrayPatternEditAction::LButtonDown(double& dPtX, double& dPtY)
{
	PatternListPtr pPatList = m_pPartCadData->GetPatternList();
	PatternLoopListPtr pPatternLoopList = m_pPartCadData->GetPatternLoopList();
	LoopTopologyItemListPtr pLoopTopologyItems = m_pPartCadData->GetLoopTopologyItems_FW();

	// edit mode
	if (m_editState != EDIT_PATTERNLINE_NOTSTART)
	{
		// create Pattern-line pattern.
		Point2D basePt(m_data[0], m_data[1]);

		// get the pattern that edited currently.
		LineArrayPattern* pPatLinePat = dynamic_cast<LineArrayPattern*>(m_pGripEditPat.get());

		// create the new pattern
		LineArrayPattern* pTmpPat = new LineArrayPattern(pPatLinePat->GetPatternRelationList(), basePt, m_data[2], m_data[3], (int)m_data[4]);
		IPatternPtr pNewPat(pTmpPat);

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
		// first click
		if (m_drawState == DRAW_ARRAY_NOTSTART || m_drawState == DRAW_ARRAY_THIRD_POINT)
		{
			if (!ArrayPatternEditActionBase::LButtonDown(dPtX, dPtY))
				return FALSE;

			m_drawState = DRAW_ARRAY_FIRST_POINT;
			m_iMsgID = IDS_OUTPUT_PATLINE_XYSPACE;

			// init some data.
			Point2D basePt = m_pControlPat->GetBasePoint();
			m_data[0] = basePt.X();
			m_data[1] = basePt.Y();
			m_data[2] = 0;
			m_data[3] = 0;
			m_data[4] = 1;
		}
		else if(m_drawState == DRAW_ARRAY_FIRST_POINT)
		{
			m_drawState = DRAW_ARRAY_SECOND_POINT;
			m_iMsgID = IDS_OUTPUT_PATLINE_NUM;

			CalcSpaceAngle(dPtX, dPtY);
		}
		else if(m_drawState == DRAW_ARRAY_SECOND_POINT)
		{
			if (!CalcRepeatCount(dPtX, dPtY))
				return FALSE;

			m_drawState = DRAW_ARRAY_THIRD_POINT;

			// create Pattern-line pattern.
			Point2D basePt(m_data[0], m_data[1]);
			ArrayPatternBasePtr pPatLine(new LineArrayPattern(basePt, m_data[2], m_data[3], (int)m_data[4]));

			// create command
			EditorData editorData = GetEditorData();
			ICommandPtr pCommand(new ArrayPatternAddCommand(editorData, pPatList, pPatternLoopList, pLoopTopologyItems, pPatLine, m_pControlPat));
			pCommand->Do();
			CommandManager::GetInstance()->AddCommand(pCommand);

			// remember to reset some variables.
			m_pControlPat.reset();
			m_drawState = DRAW_ARRAY_NOTSTART;

			// enter the selecting pattern action.
			PostMessage2EnterPatternSelectAction(pPatLine.get());
		}
	}

	return TRUE;
}

BOOL LineArrayPatternEditAction::MovePoint(double& dPtX, double& dPtY)
{
	if (m_editState != EDIT_PATTERNLINE_NOTSTART) //EDIT mode
	{
		if (m_editState == EDIT_PATTERNLINE_BASEPOINT)
		{
			m_data[0] = dPtX;
			m_data[1] = dPtY;
		}
		else if (m_editState == EDIT_PATTERNLINE_SPACEANGLE)
		{
			CalcSpaceAngle(dPtX, dPtY);
		}
		else if (m_editState == EDIT_PATTERNLINE_REPEATCOUNT)
		{
			CalcRepeatCount(dPtX, dPtY);
		}
	}
	else
	{
		if (m_drawState == DRAW_ARRAY_NOTSTART)
		{
			// here, we use the input point to select a pattern.
			Point2D pt(dPtX, dPtY);
			PatternListPtr pPatList = m_pPartCadData->GetPatternList();
			m_pActivePat = pPatList->GetPatternClosestTo(pt);
		}
		else if (m_drawState == DRAW_ARRAY_FIRST_POINT)
		{
			CalcSpaceAngle(dPtX, dPtY);
		}
		else if(m_drawState == DRAW_ARRAY_SECOND_POINT)
		{
			CalcRepeatCount(dPtX, dPtY);
		}
	}

	return TRUE;
}

BOOL LineArrayPatternEditAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	PatternEditActionBase::OnKeyDown(nChar, nRepCnt, nFlags);

	return TRUE;
}

BOOL LineArrayPatternEditAction::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	return TRUE;
}

BOOL LineArrayPatternEditAction::DrawBeforeFinish()
{
	ArrayPatternEditActionBase::DrawBeforeFinish();

	PatternDrawer patDrawer(m_pViewPort);

	if (m_drawState == DRAW_ARRAY_FIRST_POINT || m_drawState == DRAW_ARRAY_SECOND_POINT ||
	    m_editState != EDIT_PATTERNLINE_NOTSTART)
	{
		// get the repeat count.
		int iCount = (m_data[4] == 1) ? 2 : (int)m_data[4];

		int iStartIndex = 1;

		// in grip-edit case, we should draw the control pattern.
		if (m_editState == EDIT_PATTERNLINE_BASEPOINT)
			iStartIndex = 0;

		// get the position of each instance, and draw them.
		for (int i = iStartIndex; i < iCount; i++)
		{
			Point2D basePt = m_pControlPat->GetBasePoint();

			/************************************************************************/
			// get the offset vector to the control pattern.
			
			double dOffsetX = 0.0, dOffsetY = 0.0;
			dOffsetX += (m_data[0] - basePt.X());
			dOffsetY += (m_data[1] - basePt.Y());
			dOffsetX += m_data[2] * i * cos(m_data[3]);
			dOffsetY += m_data[2] * i * sin(m_data[3]);

			Vector2D offsetVect;
			offsetVect.X(dOffsetX);
			offsetVect.Y(dOffsetY);
			/************************************************************************/

			/************************************************************************/
			// create a tmp pattern and draw it.

			IDataPtr pTmpData = m_pControlPat->Clone();
			IPatternPtr pTmpPat = boost::dynamic_pointer_cast<IPattern>(pTmpData);
			IPatternPtr pNullPat;
			pNullPat.reset();
			pTmpPat->SetBeControlPat(pNullPat);

			// transform the pattern.
			Matrix2D mat;
			mat.SetTransfer(offsetVect);
			pTmpPat->Transform(mat);

			// update the cache data of the pattern object.
			pTmpPat->UpdateCache();

			// draw the pattern.
			patDrawer.DrawPat(pTmpPat.get());
			/************************************************************************/
		}
	}

	return TRUE;
}

void LineArrayPatternEditAction::UpdateForUndoRedo()
{
	PatternListPtr pPatList = m_pPartCadData->GetPatternList();
	if (m_editState != EDIT_PATTERNLINE_NOTSTART && !pPatList->GetPatternFromID(m_pGripEditPat->GetID()))
	{
		PostMessage2EnterPatternSelectAction();
	}
	else if (m_editState == EDIT_PATTERNLINE_NOTSTART)
	{
		if (m_pControlPat && !pPatList->GetPatternFromID(m_pControlPat->GetID()))
		{
			m_pControlPat.reset();
			m_drawState = DRAW_ARRAY_NOTSTART;
			m_iMsgID = IDS_OUTPUT_PATLINE_PAT;
		}
	}
}

void LineArrayPatternEditAction::CalcSpaceAngle(double dPtX, double dPtY)
{
	double dSpace = sqrt(pow((dPtX - m_data[0]),2)+pow((dPtY - m_data[1]),2));
	if (dSpace != 0.0)
	{
		if (m_data[2] != dSpace)
			m_data[2] = dSpace;

		double dAngle = atan2((dPtY - m_data[1]), (dPtX - m_data[0]));
		if (m_data[3] != dAngle)
			m_data[3] = dAngle;
	}
}

BOOL LineArrayPatternEditAction::CalcRepeatCount(double dPtX, double dPtY)
{
	double dNewPtX = 0.0, dNewPtY = 0.0;
	MathEx::RotatePoint(m_data[0], m_data[1], -m_data[3], dPtX, dPtY, dNewPtX, dNewPtY);

	double dDistance = dNewPtX - m_data[0];
	long iRepeatCount = (long)floor(dDistance/m_data[2]);
	if (iRepeatCount >= 1)
	{
		iRepeatCount++;
		if (m_data[4] != iRepeatCount)
			m_data[4] = iRepeatCount;

		return TRUE;
	}

	return FALSE;
}

void LineArrayPatternEditAction::SetGripEditPattern(IPatternPtr pPattern)
{
	// keep the LineArrayPattern that will be edited.
	m_pGripEditPat = pPattern;

	LineArrayPatternPtr pPatLinePat = boost::dynamic_pointer_cast<LineArrayPattern>(pPattern);

	// fill the m_data with the data in Pat-Line pattern.
	Point2D basePt = pPatLinePat->GetBasePoint();
	m_data[0] = basePt.X();
	m_data[1] = basePt.Y();

	m_data[2] =	pPatLinePat->GetSpace();
	m_data[3] =	pPatLinePat->GetAngle();
	m_data[4] =	pPatLinePat->GetRepeatNum();

	// also keep the control pattern.
	m_pControlPat = pPatLinePat->GetControlPattern();
}

void LineArrayPatternEditAction::SetGripEditPt(double dPtX, double dPtY)
{
	double dSecondPtX = m_data[0] + m_data[2] * cos(m_data[3]);
	double dSecondPtY = m_data[1] + m_data[2] * sin(m_data[3]);

	double dLastPtX = m_data[0] + m_data[2] * (m_data[4] -1) * cos(m_data[3]);
	double dLastPtY = m_data[1] + m_data[2] * (m_data[4] -1) * sin(m_data[3]);

	if (MathEx::PointEqual(dPtX, dPtY, m_data[0], m_data[1]))
	{
		m_editState = EDIT_PATTERNLINE_BASEPOINT;
		m_iMsgID = IDS_OUTPUT_NEWPOSITION;
	}
	else if(MathEx::PointEqual(dPtX, dPtY, dSecondPtX, dSecondPtY))
	{
		m_editState = EDIT_PATTERNLINE_SPACEANGLE;
		m_iMsgID = IDS_OUTPUT_PATLINE_XYSPACE;
	}
	else if(MathEx::PointEqual(dPtX, dPtY, dLastPtX, dLastPtY))
	{
		m_editState = EDIT_PATTERNLINE_REPEATCOUNT;
		m_iMsgID = IDS_OUTPUT_PATLINE_NUM;
	}
}

END_CUTLEADER_NAMESPACE()

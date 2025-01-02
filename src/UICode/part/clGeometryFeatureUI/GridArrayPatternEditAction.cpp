#include "StdAfx.h"
#include "GridArrayPatternEditAction.h"

#include "MathEx.h"
#include "GlViewPort.h"
#include "ICommand.h"
#include "ValueInputManager.h"
#include "clUILibResource.h"
#include "CommandManager.h"

#include "PatternSelectAction.h"
#include "PatternList.h"
#include "PatternDrawer.h"
#include "GridArrayPattern.h"
#include "PatternLoopList.h"
#include "ArrayPatternAddCommand.h"
#include "GripEditCommand.h"
#include "clGeometryFeatureResource.h"
#include "PatternPropertyBar.h"
#include "PartCadData.h"

BEGIN_CUTLEADER_NAMESPACE()

GridArrayPatternEditAction::GridArrayPatternEditAction(PartCadDataPtr pPartCadData, GlViewPortPtr pViewPort)
{
	m_pPartCadData = pPartCadData;
	m_pViewPort = pViewPort;

	m_drawState = DRAW_ARRAY_NOTSTART;
	m_editState = EDIT_PATTERNGRID_NOTSTART;

	memset(m_data, 0, sizeof(double) * 6);

	m_iMsgID = IDS_OUTPUT_PATGRID_PAT;
}

GridArrayPatternEditAction::~GridArrayPatternEditAction(void)
{
}

BOOL GridArrayPatternEditAction::LButtonDown(double& dPtX, double& dPtY)
{
	PatternListPtr pPatList = m_pPartCadData->GetPatternList();
	PatternLoopListPtr pPatternLoopList = m_pPartCadData->GetPatternLoopList();
	LoopTopologyItemListPtr pLoopTopologyItems = m_pPartCadData->GetLoopTopologyItems_FW();

	if (m_editState != EDIT_PATTERNGRID_NOTSTART) // EDIT mode
	{
		Point2D basePt(m_data[0], m_data[1]);

		// get the pattern that edited currently.
		GridArrayPattern* pPatGridPat = dynamic_cast<GridArrayPattern*>(m_pGripEditPat.get());

		// create Pattern-grid pattern.
		IPatternPtr pNewPat(new GridArrayPattern(pPatGridPat->GetPatternRelationList(), basePt, m_data[3], (int)m_data[5], m_data[2], (int)m_data[4]));

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
		if (m_drawState == DRAW_ARRAY_NOTSTART || 
			m_drawState == DRAW_ARRAY_THIRD_POINT) //first click
		{
			if (!ArrayPatternEditActionBase::LButtonDown(dPtX, dPtY))
				return FALSE;

			m_drawState = DRAW_ARRAY_FIRST_POINT;
			m_iMsgID = IDS_OUTPUT_PATGRID_XYSPACE;

			Point2D basePt = m_pControlPat->GetBasePoint();
			m_data[0] = basePt.X();
			m_data[1] = basePt.Y();
			m_data[2] = 0;
			m_data[3] = 0;

			// only set repeat num to 1.
			m_data[4] = 1;
			m_data[5] = 1;
		}
		else if(m_drawState == DRAW_ARRAY_FIRST_POINT)
		{
			m_drawState = DRAW_ARRAY_SECOND_POINT;
			m_iMsgID = IDS_OUTPUT_PATGRID_NUM;

			CalcXSpacing(dPtX, dPtY);
			CalcYSpacing(dPtX, dPtY);

		}
		else if(m_drawState == DRAW_ARRAY_SECOND_POINT)
		{
			if (!CalcXRepeatCount(dPtX, dPtY))
				return FALSE;
			if (!CalcYRepeatCount(dPtX, dPtY))
				return FALSE;

			m_drawState = DRAW_ARRAY_THIRD_POINT;
			m_iMsgID = IDS_OUTPUT_PATGRID_PAT;

			// create Pattern-grid pattern.
			Point2D basePt(m_data[0], m_data[1]);
			ArrayPatternBasePtr pPatGridPat(new GridArrayPattern(basePt, m_data[3], (int)m_data[5], m_data[2], (int)m_data[4]));

			// create command
			EditorData editorData = GetEditorData();
			ICommandPtr pCommand(new ArrayPatternAddCommand(editorData, pPatList, pPatternLoopList, pLoopTopologyItems, pPatGridPat, m_pControlPat));
			pCommand->Do();
			CommandManager::GetInstance()->AddCommand(pCommand);

			// remember to reset some variables.
			m_pControlPat.reset();
			m_drawState = DRAW_ARRAY_NOTSTART;

			// enter the selecting pattern action.
			PostMessage2EnterPatternSelectAction(pPatGridPat.get());
		}
	}

	return TRUE;
}

BOOL GridArrayPatternEditAction::MovePoint(double& dPtX, double& dPtY)
{
	if (m_editState != EDIT_PATTERNGRID_NOTSTART) //EDIT mode
	{
		if (m_editState == EDIT_PATTERNGRID_BASEPOINT)
		{
			m_data[0] = dPtX;
			m_data[1] = dPtY;
		}
		else if(m_editState == EDIT_PATTERNGRID_SPACING)
		{
			CalcXSpacing(dPtX, dPtY);
			CalcYSpacing(dPtX, dPtY);
		}
		else if (m_editState == EDIT_PATTERNGRID_SPACINTX)
		{
			CalcYSpacing(dPtX, dPtY);
		}
		else if (m_editState == EDIT_PATTERNGRID_SPACINTY)
		{
			CalcYSpacing(dPtX, dPtY);
		}
		else if (m_editState == EDIT_PATTERNGRID_REPEATCOUNT)
		{
			CalcXRepeatCount(dPtX, dPtY);
			CalcYRepeatCount(dPtX, dPtY);
		}
		else if (m_editState == EDIT_PATTERNGRID_REPEATCOUNTX)
		{
			CalcXRepeatCount(dPtX, dPtY);
		}
		else if (m_editState == EDIT_PATTERNGRID_REPEATCOUNTY)
		{
			CalcYRepeatCount(dPtX, dPtY);
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
		else if(m_drawState == DRAW_ARRAY_FIRST_POINT)
		{
			CalcXSpacing(dPtX, dPtY);
			CalcYSpacing(dPtX, dPtY);
		}
		else if(m_drawState == DRAW_ARRAY_SECOND_POINT)
		{
			CalcXRepeatCount(dPtX, dPtY);
			CalcYRepeatCount(dPtX, dPtY);
		}
	}

	return TRUE;
}

BOOL GridArrayPatternEditAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	PatternEditActionBase::OnKeyDown(nChar, nRepCnt, nFlags);

	return TRUE;
}

BOOL GridArrayPatternEditAction::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	return TRUE;
}

BOOL GridArrayPatternEditAction::DrawBeforeFinish()
{
	ArrayPatternEditActionBase::DrawBeforeFinish();

	PatternDrawer patDrawer(m_pViewPort);

	if (m_drawState == DRAW_ARRAY_FIRST_POINT || m_drawState == DRAW_ARRAY_SECOND_POINT ||
	    m_editState != EDIT_PATTERNGRID_NOTSTART)
	{
		int iXCount = (m_data[4] == 1) ? 2 : (int)m_data[4];
		int iYCount = (m_data[5] == 1) ? 2 : (int)m_data[5];

		// get the position of instance, and draw these instances.
		int iStartIndexX = 0, iStartIndexY = 0;
		for (int i = iStartIndexX; i < iXCount; i++)
		{
			for (int j = iStartIndexY; j < iYCount; j++)
			{
				if (m_editState != EDIT_PATTERNGRID_BASEPOINT)
				{
					if (i == 0 && j == 0)
						continue;
				}

				Point2D basePt = m_pControlPat->GetBasePoint();

				// get the offset vector to the control pattern.
				double dOffsetX = (m_data[0] - basePt.X());
				double dOffsetY = (m_data[1] - basePt.Y());
				dOffsetX += m_data[2] * i;
				dOffsetY += m_data[3] * j;

				/************************************************************************/
				// create a tmp pattern and draw it.

				IDataPtr pTmpData = m_pControlPat->Clone();
				IPatternPtr pTmpPat = boost::dynamic_pointer_cast<IPattern>(pTmpData);
				IPatternPtr pNullPat;
				pNullPat.reset();
				pTmpPat->SetBeControlPat(pNullPat);

				// transform the pattern.
				Matrix2D mat;
				mat.SetTransfer(Vector2D(dOffsetX, dOffsetY));
				pTmpPat->Transform(mat);

				// update the cache data of the pattern object.
				pTmpPat->UpdateCache();

				// draw the patterns.
				patDrawer.DrawPat(pTmpPat.get());
				/************************************************************************/
			}
		}
	}

	return TRUE;
}

void GridArrayPatternEditAction::UpdateForUndoRedo()
{
	PatternListPtr pPatList = m_pPartCadData->GetPatternList();
	if (m_editState != EDIT_PATTERNGRID_NOTSTART && !pPatList->GetPatternFromID(m_pGripEditPat->GetID()))
	{
		PostMessage2EnterPatternSelectAction();
	}
	else if (m_editState == EDIT_PATTERNGRID_NOTSTART)
	{
		if (m_pControlPat && !pPatList->GetPatternFromID(m_pControlPat->GetID()))
		{
			m_pControlPat.reset();
			m_drawState = DRAW_ARRAY_NOTSTART;
			m_iMsgID = IDS_OUTPUT_PATGRID_PAT;
		}
	}
}

void GridArrayPatternEditAction::SetGripEditPattern(IPatternPtr pPattern)
{
	// keep the LineArrayPattern that will be edited.
	m_pGripEditPat = pPattern;

	GridArrayPattern* pPatGridPat = dynamic_cast<GridArrayPattern*>(pPattern.get());

	// fill the m_data with the data in Pat-Line pattern.
	Point2D basePt = pPatGridPat->GetBasePoint();
	m_data[0] = basePt.X();
	m_data[1] = basePt.Y();

	m_data[2] =	pPatGridPat->GetColumnSpace();
	m_data[3] =	pPatGridPat->GetRowSpace();
	m_data[4] =	pPatGridPat->GetColumnCount();
	m_data[5] =	pPatGridPat->GetRowCount();

	// also keep the control pattern.
	m_pControlPat = pPatGridPat->GetControlPattern();
}

void GridArrayPatternEditAction::SetGripEditPt(double dPtX, double dPtY)
{
	double StartPointX = m_data[0];
	double StartPointY = m_data[1];
	double XSpacing = m_data[2];
	double YSpacing = m_data[3];
	double XRepeatCount = m_data[4];
	double YRepeatCount = m_data[5];

	double RotatePointX = 0.0, RotatePointY = 0.0;
	MathEx::RotatePoint(StartPointX, StartPointY, 0, dPtX, dPtY, RotatePointX, RotatePointY);

	double SecondVerPointX = StartPointX;
	double SecondVerPointY = StartPointY + YSpacing;

	double SecondHorPointX = StartPointX + XSpacing;
	double SecondHorPointY = StartPointY;

	double SecondCrossPointX = StartPointX + XSpacing;
	double SecondCrossPointY = StartPointY + YSpacing;

	double LastVerPointX = StartPointX;
	double LastVerPointY = StartPointY + YSpacing * (YRepeatCount - 1);

	double LastHorPointX = StartPointX + XSpacing * (XRepeatCount - 1);
	double LastHorPointY = StartPointY;

	double LastCrossPointX = StartPointX + XSpacing * (XRepeatCount - 1);
	double LastCrossPointY = StartPointY + YSpacing * (YRepeatCount - 1);

	if (MathEx::PointEqual(RotatePointX, RotatePointY,StartPointX,StartPointY))
	{
		m_editState = EDIT_PATTERNGRID_BASEPOINT;
		m_iMsgID = IDS_OUTPUT_NEWPOSITION;
	}
	else if (MathEx::PointEqual(dPtX,dPtY,SecondVerPointX,SecondVerPointY))
	{
		m_editState = EDIT_PATTERNGRID_SPACINTY;
		m_iMsgID = IDS_OUTPUT_PATGRID_XYSPACE;
	}
	else if (MathEx::PointEqual(dPtX, dPtY, SecondHorPointX, SecondHorPointY))
	{
		m_editState = EDIT_PATTERNGRID_SPACINTX;
		m_iMsgID = IDS_OUTPUT_PATGRID_XYSPACE;
	}
	else if (MathEx::PointEqual(dPtX, dPtY, SecondCrossPointX, SecondCrossPointY))
	{
		m_editState = EDIT_PATTERNGRID_SPACING;
		m_iMsgID = IDS_OUTPUT_PATGRID_XYSPACE;
	}
	else if (MathEx::PointEqual(dPtX, dPtY, LastVerPointX, LastVerPointY))
	{
		m_editState = EDIT_PATTERNGRID_REPEATCOUNTY;
		m_iMsgID = IDS_OUTPUT_PATGRID_YNUM;
	}
	else if (MathEx::PointEqual(dPtX, dPtY, LastHorPointX, LastHorPointY))
	{
		m_editState = EDIT_PATTERNGRID_REPEATCOUNTX;
		m_iMsgID = IDS_OUTPUT_PATGRID_XNUM;
	}
	else if (MathEx::PointEqual(dPtX, dPtY, LastCrossPointX, LastCrossPointY))
	{
		m_editState = EDIT_PATTERNGRID_REPEATCOUNT;
		m_iMsgID = IDS_OUTPUT_PATGRID_NUM;
	}
}

void GridArrayPatternEditAction::CalcXSpacing(double dPtX, double dPtY)
{
	double dStartPtX = m_data[0];
	double dStartPtY = m_data[1];

	double dRotatePtX = 0.0, dRotatePtY = 0.0;
	MathEx::RotatePoint(dStartPtX, dStartPtY, 0, dPtX, dPtY, dRotatePtX, dRotatePtY);

	double dSpaceX = dRotatePtX - dStartPtX;
	m_data[2] = dSpaceX;
}

void GridArrayPatternEditAction::CalcYSpacing(double dPtX, double dPtY)
{
	double dStartPtX = m_data[0];
	double dStartPtY = m_data[1];

	double dRotatePtX = 0.0, dRotatePtY = 0.0;
	MathEx::RotatePoint(dStartPtX, dStartPtY, 0, dPtX, dPtY, dRotatePtX, dRotatePtY);

	double dSpaceY = dRotatePtY - dStartPtY;
	m_data[3] = dSpaceY;
}

BOOL GridArrayPatternEditAction::CalcXRepeatCount(double dPtX, double dPtY)
{
	double dStartPtX = m_data[0];
	double dStartPtY = m_data[1];

	double dRotatePtX = 0.0, dRotatePtY = 0.0;
	MathEx::RotatePoint(dStartPtX, dStartPtY, 0, dPtX, dPtY, dRotatePtX, dRotatePtY);

	double dDisX = dRotatePtX - dStartPtX;
	int iRepeatCountX = (int)floor(dDisX / m_data[2]);
	if (iRepeatCountX >= 1)
	{
		iRepeatCountX++;
		m_data[4] = iRepeatCountX;

		return TRUE;
	}

	return FALSE;
}

BOOL GridArrayPatternEditAction::CalcYRepeatCount(double dPtX, double dPtY)
{
	double dStartPtX = m_data[0];
	double dStartPtY = m_data[1];

	double dRotatePtX = 0.0, dRotatePtY = 0.0;
	MathEx::RotatePoint(dStartPtX, dStartPtY, 0, dPtX, dPtY, dRotatePtX, dRotatePtY);

	double dDisY = dRotatePtY - dStartPtY;

	int iRepeatCountY = (int)floor(dDisY / m_data[3]);
	if (iRepeatCountY >= 1)
	{
		iRepeatCountY++;
		m_data[5] = iRepeatCountY;

		return TRUE;
	}

	return FALSE;
}

END_CUTLEADER_NAMESPACE()

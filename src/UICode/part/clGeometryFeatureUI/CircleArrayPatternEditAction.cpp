#include "StdAfx.h"
#include "CircleArrayPatternEditAction.h"

#include "MathEx.h"
#include "GlViewPort.h"
#include "ICommand.h"
#include "ValueInputManager.h"
#include "clUILibResource.h"
#include "CommandManager.h"

#include "PatternSelectAction.h"
#include "PatternList.h"
#include "CircleArrayPattern.h"
#include "PatternLoopList.h"
#include "PatternDrawer.h"
#include "ArrayPatternAddCommand.h"
#include "GripEditCommand.h"
#include "clGeometryFeatureResource.h"
#include "PatternPropertyBar.h"
#include "PartCadData.h"

BEGIN_CUTLEADER_NAMESPACE()

CircleArrayPatternEditAction::CircleArrayPatternEditAction(PartCadDataPtr pPartCadData, GlViewPortPtr pViewPort)
{
	m_pPartCadData = pPartCadData;
	m_pViewPort = pViewPort;

	m_drawState = DRAW_ARRAY_NOTSTART;
	m_editState = EDIT_PATTERNCIRCLE_NOTSTART;

	memset(m_data, 0, sizeof(double)*6);

	m_iMsgID = IDS_OUTPUT_PATCIRCLE_PAT;
}

CircleArrayPatternEditAction::~CircleArrayPatternEditAction(void)
{
}

void CircleArrayPatternEditAction::CalcRadiusAngle(double dPtX, double dPtY)
{
	double dCenterPtX = m_data[0];
	double dCenterPtY = m_data[1];

	double dRadius = sqrt(pow((dPtX - dCenterPtX),2)+pow((dPtY - dCenterPtY),2));
	m_data[3] = dRadius;

	double dAngle = atan2((dPtY - dCenterPtY), (dPtX - dCenterPtX));
	m_data[2] = dAngle;
}

void CircleArrayPatternEditAction::CalcSpaceAndRepeatCount(double dPtX, double dPtY)
{
	double dStartAngle = m_data[2];
	double dCenterPtX = m_data[0];
	double dCenterPtY = m_data[1];

	double dAngle = atan2((dPtY - dCenterPtY), (dPtX - dCenterPtX));
	double dSpaceAngle = dAngle - dStartAngle;

	int iRepeatCount = (int)fabs(floor(TWOPI / dSpaceAngle));
	if (iRepeatCount >= 1)
	{
		if (dSpaceAngle >= 0.0)
			dSpaceAngle = TWOPI / iRepeatCount;
		else
			dSpaceAngle = - TWOPI / iRepeatCount;

		m_data[4] = dSpaceAngle;
		m_data[5] = iRepeatCount;
	}
}

BOOL CircleArrayPatternEditAction::LButtonDown(double& dPtX, double& dPtY)
{
	PatternListPtr pPatList = m_pPartCadData->GetPatternList();
	PatternLoopListPtr pPatternLoopList = m_pPartCadData->GetPatternLoopList();
	LoopTopologyItemListPtr pLoopTopologyItems = m_pPartCadData->GetLoopTopologyItems_FW();

	if (m_editState != EDIT_PATTERNCIRCLE_NOTSTART) //EDIT mode
	{
		Point2D basePt(m_data[0], m_data[1]);

		// get the pattern that edited currently.
		CircleArrayPattern* pPatCirclePat = dynamic_cast<CircleArrayPattern*>(m_pGripEditPat.get());

		// create Pattern-circle pattern.
		IPatternPtr pNewPat(new CircleArrayPattern(pPatCirclePat->GetPatternRelationList(), basePt, m_data[3], m_data[2], (int)m_data[5]));

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
			{
				return FALSE;
			}

			m_drawState = DRAW_ARRAY_FIRST_POINT;
			m_iMsgID = IDS_OUTPUT_PATCIRCLE_RAD_ANGLE;

			Point2D basePt = m_pControlPat->GetBasePoint();
			m_data[0] = basePt.X();
			m_data[1] = basePt.Y();
			m_data[2] = 0;
			m_data[3] = 0;
			m_data[4] = 0;

			// only set repeat num to 1.
			m_data[5] = 1;
		}
		else if(m_drawState == DRAW_ARRAY_FIRST_POINT)
		{
			m_drawState = DRAW_ARRAY_SECOND_POINT;
			m_iMsgID = IDS_OUTPUT_PATCIRCLE_NUM;

			CalcRadiusAngle(dPtX, dPtY);
		}
		else if(m_drawState == DRAW_ARRAY_SECOND_POINT)
		{
			m_drawState = DRAW_ARRAY_THIRD_POINT;
			m_iMsgID = IDS_OUTPUT_PATCIRCLE_PAT;

			CalcSpaceAndRepeatCount(dPtX, dPtY);

			// create Pattern-circle pattern.
			Point2D basePt(m_data[0], m_data[1]);
			ArrayPatternBasePtr pPatCirclePat(new CircleArrayPattern(basePt, m_data[3], m_data[2], (int)m_data[5]));

			// create command
			EditorData editorData = GetEditorData();
			ICommandPtr pCommand(new ArrayPatternAddCommand(editorData, pPatList, pPatternLoopList, pLoopTopologyItems, pPatCirclePat, m_pControlPat));
			pCommand->Do();
			CommandManager::GetInstance()->AddCommand(pCommand);

			// remember to reset some variables.
			m_pControlPat.reset();
			m_drawState = DRAW_ARRAY_NOTSTART;

			// enter the selecting pattern action.
			PostMessage2EnterPatternSelectAction(pPatCirclePat.get());
		}
	}

	return TRUE;
}

BOOL CircleArrayPatternEditAction::MovePoint(double& dPtX, double& dPtY)
{
	if (m_editState != EDIT_PATTERNCIRCLE_NOTSTART) //EDIT mode
	{
		if (m_editState == EDIT_PATTERNCIRCLE_BASEPOINT)
		{
			m_data[0] = dPtX;
			m_data[1] = dPtY;
		}
		else if (m_editState == EDIT_PATTERNCIRCLE_STARTANGLE)
		{
			CalcRadiusAngle(dPtX, dPtY);
		}
		else if (m_editState == EDIT_PATTERNCIRCLE_SPACE)
		{
			CalcSpaceAndRepeatCount(dPtX, dPtY);
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
			CalcRadiusAngle(dPtX, dPtY);
		}
		else if (m_drawState == DRAW_ARRAY_SECOND_POINT)
		{
			CalcSpaceAndRepeatCount(dPtX, dPtY);
		}
	}

	return TRUE;
}

BOOL CircleArrayPatternEditAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	PatternEditActionBase::OnKeyDown(nChar, nRepCnt, nFlags);

	return TRUE;
}

BOOL CircleArrayPatternEditAction::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	return TRUE;
}

BOOL CircleArrayPatternEditAction::DrawBeforeFinish()
{
	ArrayPatternEditActionBase::DrawBeforeFinish();

	if (m_drawState == DRAW_ARRAY_FIRST_POINT || m_drawState == DRAW_ARRAY_SECOND_POINT ||
		m_editState != EDIT_PATTERNCIRCLE_NOTSTART)
	{
		int iCount = ((m_drawState == DRAW_ARRAY_SECOND_POINT) && (m_editState == EDIT_PATTERNCIRCLE_NOTSTART)) ? 
					  2 : (int)m_data[5];

		int iStartIndex = 0;

		// get the position of instance, and draw these instances.
		PatternDrawer patDrawer(m_pViewPort);
		for (int i = iStartIndex; i < iCount; i++)
		{
			Point2D basePt = m_pControlPat->GetBasePoint();

			/************************************************************************/
			// get the offset vector to the control pattern.

			double dOffsetX = 0.0, dOffsetY = 0.0;
			dOffsetX += (m_data[0] - basePt.X());
			dOffsetY += (m_data[1] - basePt.Y());
			dOffsetX += m_data[3] * cos(m_data[2] + i*m_data[4]);
			dOffsetY += m_data[3] * sin(m_data[2] + i*m_data[4]);

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

			// draw the patterns.
			patDrawer.DrawPat(pTmpPat.get());
			/************************************************************************/
		}
	}

	return TRUE;
}

void CircleArrayPatternEditAction::UpdateForUndoRedo()
{
	PatternListPtr pPatList = m_pPartCadData->GetPatternList();
	if (m_editState != EDIT_PATTERNCIRCLE_NOTSTART && !pPatList->GetPatternFromID(m_pGripEditPat->GetID()))
	{
		PostMessage2EnterPatternSelectAction();
	}
	else if (m_editState == EDIT_PATTERNCIRCLE_NOTSTART)
	{
		if (m_pControlPat && !pPatList->GetPatternFromID(m_pControlPat->GetID()))
		{
			m_pControlPat.reset();
			m_drawState = DRAW_ARRAY_NOTSTART;
			m_iMsgID = IDS_OUTPUT_PATCIRCLE_PAT;
		}
	}
}

void CircleArrayPatternEditAction::SetGripEditPattern(IPatternPtr pPattern)
{
	// keep the LineArrayPattern that will be edited.
	m_pGripEditPat = pPattern;

	CircleArrayPattern* pPatCirclePat = dynamic_cast<CircleArrayPattern*>(pPattern.get());

	// fill the m_data with the data in Pat-Line pattern.
	Point2D basePt = pPatCirclePat->GetBasePoint();
	m_data[0] = basePt.X();
	m_data[1] = basePt.Y();

	m_data[2] =	pPatCirclePat->GetStartAngle();
	m_data[3] =	pPatCirclePat->GetRadius();
	m_data[4] =	pPatCirclePat->GetRepeatAngle();
	m_data[5] =	pPatCirclePat->GetRepeatNum();

	// also keep the control pattern.
	m_pControlPat = pPatCirclePat->GetControlPattern();
}

void CircleArrayPatternEditAction::SetGripEditPt(double dPtX, double dPtY)
{
	double dCenterPtX = m_data[0];
	double dCenterPtY = m_data[1];
	double dStartAngle = m_data[2];
	double dSpaceAngle = m_data[4];
	double dRadius = m_data[3];

	double dFirstPtX = dCenterPtX + dRadius*cos(dStartAngle);
	double dFirstPtY = dCenterPtY + dRadius*sin(dStartAngle);

	double dSecondPtX = dCenterPtX + dRadius*cos(dStartAngle+dSpaceAngle);
	double dSecondPtY = dCenterPtY + dRadius*sin(dStartAngle+dSpaceAngle);

	if (MathEx::PointEqual(dPtX, dPtY, dCenterPtX, dCenterPtY))
	{
		m_editState = EDIT_PATTERNCIRCLE_BASEPOINT;
		m_iMsgID = IDS_OUTPUT_NEWPOSITION;
	}
	else if (MathEx::PointEqual(dPtX, dPtY, dSecondPtX, dSecondPtY))
	{
		m_editState = EDIT_PATTERNCIRCLE_SPACE;
		m_iMsgID = IDS_OUTPUT_PATCIRCLE_NUM;
	}
	else if (MathEx::PointEqual(dPtX, dPtY, dFirstPtX, dFirstPtY))
	{
		m_editState = EDIT_PATTERNCIRCLE_STARTANGLE;
		m_iMsgID = IDS_OUTPUT_PATCIRCLE_RAD_ANGLE;
	}
}

END_CUTLEADER_NAMESPACE()

#include "StdAfx.h"
#include "ArcArrayPatternEditAction.h"

#include "baseConst.h"
#include "GlViewPort.h"
#include "ICommand.h"
#include "CommandManager.h"
#include "ValueInputManager.h"
#include "clUILibResource.h"
#include "Point2DList.h"

#include "PatternSelectAction.h"
#include "PatternList.h"
#include "ArcArrayPattern.h"
#include "PatternLoopList.h"
#include "PatternDrawer.h"
#include "ArrayPatternAddCommand.h"
#include "GripEditCommand.h"
#include "clGeometryFeatureResource.h"
#include "PatternPropertyBar.h"
#include "PartCadData.h"

BEGIN_CUTLEADER_NAMESPACE()

ArcArrayPatternEditAction::ArcArrayPatternEditAction(PartCadDataPtr pPartCadData, GlViewPortPtr pViewPort)
{
	m_pPartCadData = pPartCadData;
	m_pViewPort = pViewPort;

	m_drawState = DRAW_ARRAY_NOTSTART;
	m_editState = EDIT_PATTERNARC_NOTSTART;

	memset(m_data, 0, sizeof(double)*7);

	m_iMsgID = IDS_OUTPUT_PATARC_PAT;
}

ArcArrayPatternEditAction::~ArcArrayPatternEditAction(void)
{
}

void ArcArrayPatternEditAction::CalcRadiusAngle(double dPtX, double dPtY)
{
	double dCenterPtX = m_data[0];
	double dCenterPtY = m_data[1];

	m_data[3] = sqrt(pow((dPtX - dCenterPtX),2)+pow((dPtY - dCenterPtY),2));
	m_data[2] = atan2((dPtY - dCenterPtY),(dPtX - dCenterPtX));
}

void ArcArrayPatternEditAction::CalcSpaceAngle(double dPtX, double dPtY)
{
	double dStartAngle = m_data[2];
	double dCenterPtX = m_data[0];
	double dCenterPtY = m_data[1];
	double RepeatCount = m_data[6];
	double dTmpAngle = atan2((dPtY - dCenterPtY), (dPtX - dCenterPtX));
	double dSpaceAngle = dTmpAngle - dStartAngle;

	if ((dSpaceAngle*(RepeatCount-1)) < (PI*2) && (dSpaceAngle*(RepeatCount-1)) > (-PI*2))
	{
		m_data[5] = dSpaceAngle;
	}
}

void ArcArrayPatternEditAction::CalcRepeatCount(double dPtX, double dPtY)
{
	double dStartAngle = m_data[2];
	double dCenterPtX = m_data[0];
	double dCenterPtY = m_data[1];
	double dSpaceAngle = m_data[5];
	double dMoveAngle = m_data[4];

	double dTmpAngle = atan2((dPtY - dCenterPtY), (dPtX - dCenterPtX));
	double dAngle = dTmpAngle - (dStartAngle + dMoveAngle);
	if (abs(dAngle) > PI)
	{
		if (dAngle > 0)
			dAngle -= 2*PI;
		else
			dAngle += 2*PI;
	}            

	dMoveAngle += dAngle;

	// if move angle >= 2pi or <= 2pi, change it
	if (dMoveAngle >= 2*PI)
		dMoveAngle -= 2*PI;
	else if(dMoveAngle <= -2*PI)
		dMoveAngle += 2*PI;

	int iRepeatCount = (int)fabs(floor(dMoveAngle / dSpaceAngle));

	if (iRepeatCount >= 1)
	{
		m_data[4] = dMoveAngle;

		iRepeatCount++;
		m_data[6] = iRepeatCount;
	}
}

BOOL ArcArrayPatternEditAction::LButtonDown(double& dPtX, double& dPtY)
{
	PatternListPtr pPatList = m_pPartCadData->GetPatternList();
	PatternLoopListPtr pPatternLoopList = m_pPartCadData->GetPatternLoopList();
	LoopTopologyItemListPtr pLoopTopologyItems = m_pPartCadData->GetLoopTopologyItems_FW();

	if (m_editState != EDIT_PATTERNARC_NOTSTART) //EDIT mode
	{
		Point2D basePt(m_data[0], m_data[1]);

		// get the pattern that edited currently.
		ArcArrayPattern* pPatArcPat = dynamic_cast<ArcArrayPattern*>(m_pGripEditPat.get());

		// create Pattern-arc pattern.
		IPatternPtr pNewPat(new ArcArrayPattern(pPatArcPat->GetPatternRelationList(), basePt, m_data[3], m_data[2], m_data[5], (int)m_data[6]));

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
		if (m_drawState == DRAW_ARRAY_NOTSTART) //first click
		{
			if (!ArrayPatternEditActionBase::LButtonDown(dPtX, dPtY))
				return FALSE;

			m_drawState = DRAW_ARRAY_FIRST_POINT;
			m_iMsgID = IDS_OUTPUT_PATARC_RAD_ANGLE;

			Point2D basePt = m_pControlPat->GetBasePoint();
			m_data[0] = basePt.X();
			m_data[1] = basePt.Y();
			m_data[2] = 0;
			m_data[3] = 0;
			m_data[4] = 0;
			m_data[5] = 0;
			// only set repeat num to 1.
			m_data[6] = 1;
		}
		else if(m_drawState == DRAW_ARRAY_FIRST_POINT)
		{
			m_drawState = DRAW_ARRAY_SECOND_POINT;
			m_iMsgID = IDS_OUTPUT_PATARC_REPEAT_ANGLE;

			CalcRadiusAngle(dPtX, dPtY);
		}
		else if(m_drawState == DRAW_ARRAY_SECOND_POINT)
		{
			m_drawState = DRAW_ARRAY_THIRD_POINT;
			m_iMsgID = IDS_OUTPUT_PATARC_NUM;

			CalcSpaceAngle(dPtX, dPtY);
		}
		else if(m_drawState == DRAW_ARRAY_THIRD_POINT)
		{
			m_drawState = DRAW_ARRAY_NOTSTART;
			m_iMsgID = IDS_OUTPUT_PATARC_PAT;

			// create Pattern-arc pattern.
			Point2D basePt(m_data[0], m_data[1]);
			ArrayPatternBasePtr pPatArcPat(new ArcArrayPattern(basePt, m_data[3], m_data[2], m_data[5], (int)m_data[6]));

			// create command
			EditorData editorData = GetEditorData();
			ICommandPtr pCommand(new ArrayPatternAddCommand(editorData, pPatList, pPatternLoopList, pLoopTopologyItems, pPatArcPat, m_pControlPat));
			pCommand->Do();
			CommandManager::GetInstance()->AddCommand(pCommand);

			// remember to reset some variables.
			m_pControlPat.reset();

			// enter the selecting pattern action.
			PostMessage2EnterPatternSelectAction(pPatArcPat.get());
		}
	}

	return TRUE;
}

BOOL ArcArrayPatternEditAction::MovePoint(double& dPtX, double& dPtY)
{
	if (m_editState != EDIT_PATTERNARC_NOTSTART) //EDIT mode
	{
		if (m_editState == EDIT_PATTERNARC_BASEPOINT)
		{
			m_data[0] = dPtX;
			m_data[1] = dPtY;
		}
		else if (m_editState == EDIT_PATTERNARC_STARTANGLE)
		{
			CalcRadiusAngle(dPtX, dPtY);
		}
		else if (m_editState == EDIT_PATTERNARC_SPACING)
		{
			CalcSpaceAngle(dPtX, dPtY);
		}
		else if (m_editState == EDIT_PATTERNARC_REPEATCOUNT)
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
			CalcRadiusAngle(dPtX, dPtY);
		}
		else if(m_drawState == DRAW_ARRAY_SECOND_POINT)
		{
			CalcSpaceAngle(dPtX, dPtY);
		}
		else if(m_drawState == DRAW_ARRAY_THIRD_POINT)
		{
			CalcRepeatCount(dPtX, dPtY);
		}
	}

	return TRUE;
}

BOOL ArcArrayPatternEditAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	PatternEditActionBase::OnKeyDown(nChar, nRepCnt, nFlags);

	return TRUE;
}

BOOL ArcArrayPatternEditAction::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	return TRUE;
}

BOOL ArcArrayPatternEditAction::DrawBeforeFinish()
{
	ArrayPatternEditActionBase::DrawBeforeFinish();

	if (m_drawState != DRAW_ARRAY_NOTSTART || m_editState != EDIT_PATTERNARC_NOTSTART)
	{
		int iCount = ((m_drawState == DRAW_ARRAY_SECOND_POINT) && (m_editState == EDIT_PATTERNARC_NOTSTART)) ? 
					   2 : (int)m_data[6];

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
			dOffsetX += m_data[3] * cos(m_data[2] + i*m_data[5]);
			dOffsetY += m_data[3] * sin(m_data[2] + i*m_data[5]);

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

void ArcArrayPatternEditAction::UpdateForUndoRedo()
{
	PatternListPtr pPatList = m_pPartCadData->GetPatternList();
	if (m_editState != EDIT_PATTERNARC_NOTSTART && !pPatList->GetPatternFromID(m_pGripEditPat->GetID()))
	{
		PostMessage2EnterPatternSelectAction();
	}
	else if (m_editState == EDIT_PATTERNARC_NOTSTART)
	{
		if (m_pControlPat && !pPatList->GetPatternFromID(m_pControlPat->GetID()))
		{
			m_pControlPat.reset();
			m_drawState = DRAW_ARRAY_NOTSTART;
			m_iMsgID = IDS_OUTPUT_PATARC_PAT;
		}
	}
}

void ArcArrayPatternEditAction::SetGripEditPattern(IPatternPtr pPattern)
{
	// keep the LineArrayPattern that will be edited.
	m_pGripEditPat = pPattern;

	ArcArrayPattern* pPatArcPat = dynamic_cast<ArcArrayPattern*>(pPattern.get());

	// fill the m_data with the data in Pat-Line pattern.
	Point2D basePt = pPatArcPat->GetBasePoint();
	m_data[0] = basePt.X();
	m_data[1] = basePt.Y();

	m_data[2] =	pPatArcPat->GetStartAngle();
	m_data[3] =	pPatArcPat->GetRadius();
	m_data[4] =	pPatArcPat->GetMoveAngle();
	m_data[5] =	pPatArcPat->GetRepeatAngle();
	m_data[6] =	pPatArcPat->GetRepeatNum();

	// also keep the control pattern.
	m_pControlPat = pPatArcPat->GetControlPattern();
}

void ArcArrayPatternEditAction::SetGripEditPt(double dPtX, double dPtY)
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
				m_editState = EDIT_PATTERNARC_BASEPOINT;
				m_iMsgID = IDS_OUTPUT_NEWPOSITION;
			}
			else if (i == 1)
			{
				m_editState = EDIT_PATTERNARC_STARTANGLE;
				m_iMsgID = IDS_OUTPUT_PATARC_RAD_ANGLE;
			}
			else if (i == 2)
			{
				m_editState = EDIT_PATTERNARC_SPACING;
				m_iMsgID = IDS_OUTPUT_PATARC_REPEAT_ANGLE;
			}
			else if (i == 3)
			{
				m_editState = EDIT_PATTERNARC_REPEATCOUNT;
				m_iMsgID = IDS_OUTPUT_PATARC_NUM;
			}

			return;
		}
	}
}

END_CUTLEADER_NAMESPACE()

#include "StdAfx.h"
#include "ArcPattern3PtEditAction.h"

#include "MathEx.h"
#include "GlViewPort.h"
#include "Point2D.h"
#include "CommandManager.h"
#include "ICommand.h"
#include "ValueInputManager.h"
#include "clUILibResource.h"
#include "CursorDrawer.h"

#include "CLOptionInfo.h"
#include "DisplayParam.h"

#include "ArcPattern.h"
#include "ArcPatternEditAction.h"
#include "PatternAddCommand.h"
#include "PatternSelectAction.h"
#include "clGeometryFeatureResource.h"
#include "PartCadData.h"

BEGIN_CUTLEADER_NAMESPACE()

ArcPattern3PtEditAction::ArcPattern3PtEditAction(PartCadDataPtr pPartCadData, GlViewPortPtr pViewPort)
{
	m_iStatus = DRAW_ARC3P_NOTSTART;
	memset(m_data, 0, sizeof(double)*5);

	m_pPartCadData = pPartCadData;
	m_pViewPort = pViewPort;

	m_iMsgID = IDS_OUTPUT_PATARC3PT_FIRSTPT;
}

ArcPattern3PtEditAction::~ArcPattern3PtEditAction(void)
{
}

BOOL ArcPattern3PtEditAction::LButtonDown(double& dPtX, double& dPtY)
{
	PatternListPtr pPatList = m_pPartCadData->GetPatternList();
	PatternLoopListPtr pPatternLoopList = m_pPartCadData->GetPatternLoopList();
	LoopTopologyItemListPtr pLoopTopologyItems = m_pPartCadData->GetLoopTopologyItems_FW();

	if (m_iStatus == DRAW_ARC3P_NOTSTART || m_iStatus == DRAW_ARC3P_END_POINT)
	{
		m_iStatus = DRAW_ARC3P_START_POINT;
		m_iMsgID = IDS_OUTPUT_PATARC3PT_SECONDPT;

		m_data[0] = dPtX;
		m_data[1] = dPtY;
		m_data[2] = dPtX;
		m_data[3] = dPtY;
		m_data[4] = dPtX;
		m_data[5] = dPtY;
	}
	else if(m_iStatus == DRAW_ARC3P_START_POINT)
	{
		// 检查能不能进入下一状态。
		if (Point2D(m_data[0], m_data[1]) == Point2D(dPtX, dPtY)) {
			return FALSE;
		}

		m_data[2] = dPtX;
		m_data[3] = dPtY;
		m_data[4] = dPtX;
		m_data[5] = dPtY;

		m_iStatus = DRAW_ARC3P_MID_POINT;
		m_iMsgID = IDS_OUTPUT_PATARC3PT_THIRDPT;
	}
	else if(m_iStatus == DRAW_ARC3P_MID_POINT)
	{
		m_data[4] = dPtX;
		m_data[5] = dPtY;

		// calculate arc form 3 points
		double dCenterX = 0, dCenterY = 0, dRadius = 0, dStartAngle = 0, dMoveAngle = 0;
		MathEx::GetArcFrom3Point(m_data[0], m_data[1], m_data[2], m_data[3], m_data[4], m_data[5], dCenterX, dCenterY, dRadius, dStartAngle, dMoveAngle);

		// check whether 3 pts are on a line.
		if (dRadius < GEOM_TOLERANCE)
			return FALSE;

		m_iStatus = DRAW_ARC3P_END_POINT;
		m_iMsgID = IDS_OUTPUT_PATARC3PT_FIRSTPT;

		// create arc pattern.
		Point2D centerPt(dCenterX, dCenterY);
		IPatternPtr pArcPat(new ArcPattern(centerPt, dRadius, dStartAngle, dMoveAngle));

		// create command.
		EditorData editorData = GetEditorData();
		ICommandPtr pCommand(new PatternAddCommand(editorData, pPatList, pPatternLoopList, pLoopTopologyItems, pArcPat));
		pCommand->Do();
		CommandManager::GetInstance()->AddCommand(pCommand);
	}

	return TRUE;
}

BOOL ArcPattern3PtEditAction::MovePoint(double& dPtX, double& dPtY)
{
	if (m_iStatus == DRAW_ARC3P_START_POINT)  
	{
		m_data[2] = dPtX;
		m_data[3] = dPtY;
		m_data[4] = dPtX;
		m_data[5] = dPtY;
	}
	else if(m_iStatus == DRAW_ARC3P_MID_POINT)  
	{
		m_data[4] = dPtX;
		m_data[5] = dPtY;
	}

	return TRUE;
}

BOOL ArcPattern3PtEditAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	ValueInputManager* pValInputMgr = ValueInputManager::GetInstance();

	if (pValInputMgr->IsInputCancel(nChar))
	{
		PostMessage2EnterPatternSelectAction();
	}

	return TRUE;
}

BOOL ArcPattern3PtEditAction::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	return TRUE;
}

BOOL ArcPattern3PtEditAction::DrawBeforeFinish()
{
	CLOptionInfoPtr pCLOptionInfo = CLOptionInfo::GetInstance();
	DisplayParamPtr pColorInfo = pCLOptionInfo->GetDisplayParam();

	// keep the old color.
	COLORREF dwOldColor;
	m_pViewPort->GetDrawColor(dwOldColor);

	//
	m_pViewPort->SetDrawColor(*pColorInfo->GetPatColor());

	// we should draw line after the first click
	if (m_iStatus == DRAW_ARC3P_START_POINT)  
	{
		m_pViewPort->DrawLinePoint2D(m_data[0], m_data[1], m_data[2], m_data[3]);
	}
	// we should draw arc after second click.
	else if(m_iStatus == DRAW_ARC3P_MID_POINT)  
	{
		double centerX = 0;
		double centerY = 0; 
		double radius = 0;
		double startAngle = 0;
		double moveAngle = 0;

		// calculate arc form 3 points
		MathEx::GetArcFrom3Point(m_data[0], m_data[1], m_data[2], m_data[3], m_data[4], m_data[5],
								 centerX, centerY, radius, startAngle, moveAngle);

		m_pViewPort->DrawArc2D(centerX, centerY, radius, startAngle, moveAngle);
	}

	// draw cursor.
	CursorDrawer::DrawCrossLineCursor(m_pViewPort);

	// restore the draw color
	m_pViewPort->SetDrawColor(dwOldColor);

	return TRUE;
}

END_CUTLEADER_NAMESPACE()

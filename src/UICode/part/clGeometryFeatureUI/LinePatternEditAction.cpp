#include "StdAfx.h"
#include "LinePatternEditAction.h"

#include "Point2D.h"
#include "GlViewPort.h"
#include "ICommand.h"
#include "ValueInputManager.h"
#include "clUILibResource.h"
#include "CommandManager.h"
#include "Point2DList.h"
#include "CursorDrawer.h"

#include "CLOptionInfo.h"
#include "DisplayParam.h"

#include "LinePattern.h"
#include "PatternList.h"
#include "PatternLoopList.h"
#include "PatternSelectAction.h"
#include "PatternAddCommand.h"
#include "GripEditCommand.h"
#include "clGeometryFeatureResource.h"
#include "PatternPropertyBar.h"
#include "PartCadData.h"

BEGIN_CUTLEADER_NAMESPACE()

LinePatternEditAction::LinePatternEditAction(PartCadDataPtr pPartCadData, GlViewPortPtr pViewPort)
{
	m_pPartCadData = pPartCadData;
	m_pViewPort = pViewPort;

	m_drawState = DRAW_LINE_NOTSTART;
	m_editState = EDIT_LINE_NOTSTART;
	memset(m_data, 0, sizeof(double)*4);

	m_iMsgID = IDS_OUTPUT_DRAWLINE_STARTPT;
}

LinePatternEditAction::~LinePatternEditAction(void)
{
}

BOOL LinePatternEditAction::LButtonDown(double& dPtX, double& dPtY)
{
	PatternListPtr pPatList = m_pPartCadData->GetPatternList();
	PatternLoopListPtr pPatternLoopList = m_pPartCadData->GetPatternLoopList();
	LoopTopologyItemListPtr pLoopTopologyItems = m_pPartCadData->GetLoopTopologyItems_FW();

	if (m_editState != EDIT_LINE_NOTSTART) // EDIT mode
	{
		if (m_editState == EDIT_LINE_START_POINT)
		{
			m_data[0] = dPtX;
			m_data[1] = dPtY;
		}
		else if(m_editState == EDIT_LINE_CENTER_POINT)
		{
			double dOriCenterX = (m_data[0]+m_data[2])/2;
			double dOriCenterY = (m_data[1]+m_data[3])/2;

			m_data[0] += dPtX - dOriCenterX;
			m_data[1] += dPtY - dOriCenterY;
			m_data[2] += dPtX - dOriCenterX;
			m_data[3] += dPtY - dOriCenterY;
		}
		else if(m_editState == EDIT_LINE_END_POINT)
		{
			m_data[2] = dPtX;
			m_data[3] = dPtY;
		}

		// create the new line pattern with the new data.
		Point2D basePt(m_data[0], m_data[1]);
		Vector2D v(m_data[2] - m_data[0], m_data[3] - m_data[1]);	
		IPatternPtr pNewPat(new LinePattern(basePt, v));

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
		if (m_drawState == DRAW_LINE_NOTSTART || m_drawState == DRAW_LINE_END_POINT)
		{
			m_drawState = DRAW_LINE_START_POINT;
			m_iMsgID = IDS_OUTPUT_DRAWLINE_ENDPT;

			m_data[0] = dPtX;
			m_data[1] = dPtY;
			m_data[2] = dPtX;
			m_data[3] = dPtY;

			// add the first input point to "m_pInputPtList"
			m_pInputPtList->push_back(Point2D(dPtX, dPtY));
		}
		else if (m_drawState == DRAW_LINE_START_POINT) // second click, we can create a line
		{
			// 检查能不能创建线段。
			if (Point2D(m_data[0], m_data[1]) == Point2D(dPtX, dPtY)) {
				return FALSE;
			}

			m_drawState = DRAW_LINE_END_POINT;
			m_iMsgID = IDS_OUTPUT_DRAWLINE_STARTPT;

			m_data[2] = dPtX;
			m_data[3] = dPtY;

			// create line pattern.
			Point2D basePt(m_data[0], m_data[1]);
			Vector2D v(m_data[2] - m_data[0], m_data[3] - m_data[1]);
			IPatternPtr pLinePat(new LinePattern(basePt, v));

			// create command
			ICommandPtr pCommand(new PatternAddCommand(GetEditorData(), pPatList, pPatternLoopList, pLoopTopologyItems, pLinePat));
			pCommand->Do();
			CommandManager::GetInstance()->AddCommand(pCommand);

			// finished drawing line, clear "m_pInputPtList".
			m_pInputPtList->clear();
		}
	}

	return TRUE;
}

BOOL LinePatternEditAction::MovePoint(double& dPtX, double& dPtY)
{
	if (m_editState != EDIT_LINE_NOTSTART) // EDIT mode
	{
		if (m_editState == EDIT_LINE_START_POINT)
		{
			m_data[0] = dPtX;
			m_data[1] = dPtY;
		}
		else if(m_editState == EDIT_LINE_CENTER_POINT)
		{
			double dOriCenterX = (m_data[0]+m_data[2])/2;
			double dOriCenterY = (m_data[1]+m_data[3])/2;

			m_data[0] += dPtX - dOriCenterX;
			m_data[1] += dPtY - dOriCenterY;
			m_data[2] += dPtX - dOriCenterX;
			m_data[3] += dPtY - dOriCenterY;
		}
		else if(m_editState == EDIT_LINE_END_POINT)
		{
			m_data[2] = dPtX;
			m_data[3] = dPtY;
		}
	}
	else
	{
		if (m_drawState == DRAW_LINE_START_POINT)
		{
			m_data[2] = dPtX;
			m_data[3] = dPtY;
		}
	}

	return TRUE;
}

BOOL LinePatternEditAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	PatternEditActionBase::OnKeyDown(nChar, nRepCnt, nFlags);

	return TRUE;
}

BOOL LinePatternEditAction::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	return TRUE;
}

SnapInputPtr LinePatternEditAction::GetSnapInput() const
{
	SnapInputPtr pSnapInput = SnapInput::BuildSnapInput4SnapNone();

	DWORD dwSnapStyle = 0;
	if (m_editState != EDIT_LINE_NOTSTART) // EDIT mode
	{
		dwSnapStyle = SnapEndPtOf2DLineArc | SnapMiddlePtOf2DLineArc | SnapCenterPtOf2DArc | SnapClosestPtTo2DLineArc |
			SnapClosestPtToOneAssistLine | SnapCrossPtOfTwo2DLineArc | SnapCrossPtOfTwoAssistLines;
		pSnapInput.reset(new SnapInput(dwSnapStyle, m_pPartCadData->GetPatternList(), Matrix2D()));
	}
	else // 绘制线段。
	{
		if (m_drawState == DRAW_LINE_NOTSTART || m_drawState == DRAW_LINE_END_POINT) // 线段的起点还没确定。
			dwSnapStyle = SnapEndPtOf2DLineArc | SnapMiddlePtOf2DLineArc | SnapCenterPtOf2DArc | SnapClosestPtTo2DLineArc | SnapClosestPtToOneAssistLine |
			SnapCrossPtOfTwo2DLineArc | SnapCrossPtOfTwoAssistLines;
		else if (m_drawState == DRAW_LINE_START_POINT) // 已经确定了线段的起点。
			dwSnapStyle = SnapEndPtOf2DLineArc | SnapMiddlePtOf2DLineArc | SnapCenterPtOf2DArc | SnapClosestPtTo2DLineArc | SnapClosestPtToOneAssistLine |
			SnapFootPtTo2DLine | SnapTangencyPtTo2DArc | SnapCrossPtOfTwo2DLineArc | SnapCrossPtOfTwoAssistLines;
		pSnapInput.reset(new SnapInput(dwSnapStyle, m_pPartCadData->GetPatternList(), Matrix2D()));
	}

	return pSnapInput;
}

BOOL LinePatternEditAction::DrawBeforeFinish()
{
	CLOptionInfoPtr pCLOptionInfo = CLOptionInfo::GetInstance();
	DisplayParamPtr pColorInfo = pCLOptionInfo->GetDisplayParam();

	// keep the old color.
	COLORREF dwOldColor;
	m_pViewPort->GetDrawColor(dwOldColor);

	//
	m_pViewPort->SetDrawColor(*pColorInfo->GetPatColor());

	if (m_drawState == DRAW_LINE_START_POINT || m_editState != EDIT_LINE_NOTSTART)
		m_pViewPort->DrawLinePoint2D(m_data[0], m_data[1], m_data[2], m_data[3]);

	// draw cursor.
	CursorDrawer::DrawCrossLineCursor(m_pViewPort);

	// restore the draw color
	m_pViewPort->SetDrawColor(dwOldColor);

	return TRUE;
}

void LinePatternEditAction::UpdateForUndoRedo()
{
}

void LinePatternEditAction::SetGripEditPattern(IPatternPtr pPattern)
{
	m_pGripEditPat = pPattern;

	LinePattern* pLinePat = dynamic_cast<LinePattern*>(pPattern.get());

	// fill the m_data with the data in line pattern.
	Point2D startPt = pLinePat->GetBasePoint();
	m_data[0] = startPt.X();
	m_data[1] = startPt.Y();

	Point2D endPt =	pLinePat->GetEndPoint();
	m_data[2] = endPt.X();
	m_data[3] = endPt.Y();
}

void LinePatternEditAction::SetGripEditPt(double dPtX, double dPtY)
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
				m_editState = EDIT_LINE_START_POINT;
				m_iMsgID = IDS_OUTPUT_DRAWLINE_STARTPT;
			}
			else if (i == 1)
			{
				m_editState = EDIT_LINE_CENTER_POINT;
				m_iMsgID = IDS_OUTPUT_NEWPOSITION;
			}
			else if (i == 2)
			{
				m_editState = EDIT_LINE_END_POINT;
				m_iMsgID = IDS_OUTPUT_DRAWLINE_ENDPT;
			}
			return;
		}
	}
}

END_CUTLEADER_NAMESPACE()

#include "StdAfx.h"
#include "MultiLinePatternEditAction.h"

#include "ICommand.h"
#include "CommandManager.h"
#include "Point2DList.h"

#include "LinePattern.h"
#include "PatternList.h"
#include "PatternAddCommand.h"
#include "GripEditCommand.h"
#include "clGeometryFeatureResource.h"
#include "PartCadData.h"

BEGIN_CUTLEADER_NAMESPACE()

MultiLinePatternEditAction::MultiLinePatternEditAction(PartCadDataPtr pPartCadData, GlViewPortPtr pViewPort)
	: LinePatternEditAction(pPartCadData, pViewPort)
{
	m_pNewPatList.reset(new PatternList);
}

MultiLinePatternEditAction::~MultiLinePatternEditAction(void)
{
}

BOOL MultiLinePatternEditAction::LButtonDown(double& dPtX, double& dPtY)
{
	PatternListPtr pPatList = m_pPartCadData->GetPatternList();
	PatternLoopListPtr pPatternLoopList = m_pPartCadData->GetPatternLoopList();
	LoopTopologyItemListPtr pLoopTopologyItems = m_pPartCadData->GetLoopTopologyItems_FW();

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
	else if (m_drawState == DRAW_LINE_START_POINT)
	{
		// 检查能不能创建线。
		if (Point2D(m_data[0], m_data[1]) == Point2D(dPtX, dPtY)) {
			return FALSE;
		}

		// create line pattern.
		Point2D basePt(m_data[0], m_data[1]);
		m_data[2] = dPtX;
		m_data[3] = dPtY;
		Vector2D v(m_data[2] - m_data[0], m_data[3] - m_data[1]);
		IPatternPtr pLinePat(new LinePattern(basePt, v));
		m_pNewPatList->push_back(pLinePat);

		// create command
		EditorData editorData = GetEditorData();
		ICommandPtr pCommand(new PatternAddCommand(editorData, pPatList, pPatternLoopList, pLoopTopologyItems, pLinePat));
		pCommand->Do();
		CommandManager::GetInstance()->AddCommand(pCommand);

		// reset the state.
		m_drawState = DRAW_LINE_START_POINT;
		m_iMsgID = IDS_OUTPUT_DRAWLINE_ENDPT;
		m_data[0] = dPtX;
		m_data[1] = dPtY;
		m_data[2] = dPtX;
		m_data[3] = dPtY;
		m_pInputPtList->clear();
		m_pInputPtList->push_back(Point2D(dPtX, dPtY));
	}

	return TRUE;
}

BOOL MultiLinePatternEditAction::MovePoint(double& dPtX, double& dPtY)
{
	LinePatternEditAction::MovePoint(dPtX, dPtY);

	return TRUE;
}

END_CUTLEADER_NAMESPACE()

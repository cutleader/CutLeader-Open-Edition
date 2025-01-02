#include "StdAfx.h"
#include "RectLinesDrawAction.h"

#include "GlViewPort.h"
#include "ICommand.h"
#include "CommandManager.h"
#include "ValueInputManager.h"
#include "clUILibResource.h"
#include "CursorDrawer.h"

#include "CLOptionInfo.h"
#include "DisplayParam.h"

#include "PatternList.h"
#include "LinePattern.h"
#include "PatternAddCommand.h"
#include "GripEditCommand.h"
#include "clGeometryFeatureResource.h"
#include "PatternSelectAction.h"
#include "PatternPropertyBar.h"
#include "PatternProcessor.h"
#include "PartCadData.h"
#include "TextStructList.h"

BEGIN_CUTLEADER_NAMESPACE()

RectLinesDrawAction::RectLinesDrawAction(PartCadDataPtr pPartCadData, GlViewPortPtr pViewPort)
{
	m_pPartCadData = pPartCadData;
	m_pViewPort = pViewPort;

	m_drawState = DRAW_RECTLINE_NOTSTART;
	memset(m_data, 0, sizeof(double)*4);

	m_iMsgID = IDS_OUTPUT_RECTLINE_STARTCORNER;
}

RectLinesDrawAction::~RectLinesDrawAction(void)
{
}

BOOL RectLinesDrawAction::LButtonDown(double& dPtX, double& dPtY)
{
	EditorData editorData = GetEditorData();
	PatternListPtr pPatList = m_pPartCadData->GetPatternList();
	PatternLoopListPtr pPatternLoopList = m_pPartCadData->GetPatternLoopList();
	LoopTopologyItemListPtr pLoopTopologyItems = m_pPartCadData->GetLoopTopologyItems_FW();

	if (m_drawState == DRAW_RECTLINE_NOTSTART || m_drawState == DRAW_RECTLINE_END_POINT)
	{
		m_drawState = DRAW_RECTLINE_START_POINT;
		m_iMsgID = IDS_OUTPUT_RECTLINE_ENDCORNER;

		m_data[0] = dPtX;
		m_data[1] = dPtY;
		m_data[2] = dPtX;
		m_data[3] = dPtY;
	}
	else if (m_drawState == DRAW_RECTLINE_START_POINT) // 生成四条线。
	{
		// 检查能不能创建矩形线。
		if (fabs(m_data[0] - dPtX) < GEOM_TOLERANCE || fabs(m_data[1] - dPtY) < GEOM_TOLERANCE) {
			return FALSE;
		}

		m_drawState = DRAW_RECTLINE_END_POINT;
		m_iMsgID = IDS_OUTPUT_RECTLINE_STARTCORNER;
		m_data[2] = dPtX;
		m_data[3] = dPtY;


		/************************************************************************/
		// create four line patterns four this rectangle line.

		PatternListPtr pNewPatList(new PatternList);

		// line pattern1.
		Point2D basePt1(m_data[0], m_data[1]);
		Vector2D vect1(m_data[2] - m_data[0], m_data[1] - m_data[1]);
		IPatternPtr pLinePat1(new LinePattern(basePt1, vect1));
		pNewPatList->push_back(pLinePat1);

		// line pattern2.
		Point2D basePt2(m_data[0], m_data[1]);
		Vector2D vect2(m_data[0] - m_data[0], m_data[3] - m_data[1]);
		IPatternPtr pLinePat2(new LinePattern(basePt2, vect2));
		pNewPatList->push_back(pLinePat2);

		// line pattern3.
		Point2D basePt3(m_data[0], m_data[3]);
		Vector2D vect3(m_data[2] - m_data[0], m_data[3] - m_data[3]);
		IPatternPtr pLinePat3(new LinePattern(basePt3, vect3));
		pNewPatList->push_back(pLinePat3);

		// line pattern4.
		Point2D basePt4(m_data[2], m_data[1]);
		Vector2D vect4(m_data[2] - m_data[2], m_data[3] - m_data[1]);
		IPatternPtr pLinePat4(new LinePattern(basePt4, vect4));
		pNewPatList->push_back(pLinePat4);

		// update the cache data of the pattern object.
		pNewPatList->UpdateCache();
		/************************************************************************/


		// create four commands
		ICommandPtr pCommand1(new PatternAddCommand(editorData, pPatList, pPatternLoopList, pLoopTopologyItems, pLinePat1));
		ICommandPtr pCommand2(new PatternAddCommand(editorData, pPatList, pPatternLoopList, pLoopTopologyItems, pLinePat2));
		ICommandPtr pCommand3(new PatternAddCommand(editorData, pPatList, pPatternLoopList, pLoopTopologyItems, pLinePat3));
		ICommandPtr pCommand4(new PatternAddCommand(editorData, pPatList, pPatternLoopList, pLoopTopologyItems, pLinePat4));
		pCommand1->Do();
		pCommand2->Do();
		pCommand3->Do();
		pCommand4->Do();
		CommandManager::GetInstance()->AddCommand(pCommand1);
		CommandManager::GetInstance()->AddCommand(pCommand2);
		CommandManager::GetInstance()->AddCommand(pCommand3);
		CommandManager::GetInstance()->AddCommand(pCommand4);
	}

	return TRUE;
}

BOOL RectLinesDrawAction::MovePoint(double& dPtX, double& dPtY)
{
	if (m_drawState == DRAW_RECTLINE_START_POINT)
	{
		m_data[2] = dPtX;
		m_data[3] = dPtY;
	}

	return TRUE;
}

BOOL RectLinesDrawAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	PatternEditActionBase::OnKeyDown(nChar, nRepCnt, nFlags);

	return TRUE;
}

BOOL RectLinesDrawAction::DrawBeforeFinish()
{
	CLOptionInfoPtr pCLOptionInfo = CLOptionInfo::GetInstance();
	DisplayParamPtr pColorInfo = pCLOptionInfo->GetDisplayParam();

	// keep the old color.
	COLORREF dwOldColor;
	m_pViewPort->GetDrawColor(dwOldColor);

	//
	m_pViewPort->SetDrawColor(*pColorInfo->GetPatColor());

	if (m_drawState == DRAW_RECTLINE_START_POINT)
	{
		m_pViewPort->DrawLinePoint2D(m_data[0], m_data[1], m_data[2], m_data[1]);
		m_pViewPort->DrawLinePoint2D(m_data[0], m_data[1], m_data[0], m_data[3]);
		m_pViewPort->DrawLinePoint2D(m_data[0], m_data[3], m_data[2], m_data[3]);
		m_pViewPort->DrawLinePoint2D(m_data[2], m_data[1], m_data[2], m_data[3]);
	}

	// draw cursor.
	CursorDrawer::DrawCrossLineCursor(m_pViewPort);

	// restore the draw color
	m_pViewPort->SetDrawColor(dwOldColor);

	return TRUE;
}

END_CUTLEADER_NAMESPACE()

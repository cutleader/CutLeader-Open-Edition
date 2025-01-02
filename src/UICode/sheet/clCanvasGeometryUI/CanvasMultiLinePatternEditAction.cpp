#include "StdAfx.h"
#include "CanvasMultiLinePatternEditAction.h"

#include "boost/bind.hpp"
#include "Point2D.h"
#include "MathEx.h"
#include "baseConst.h"
#include "GlViewPort.h"
#include "CommandManager.h"
#include "DummyAction.h"
#include "CursorDrawer.h"
#include "clBaseDataResource.h"
#include "clUILibResource.h"
#include "DataBaseManager.h"
#include "ValueInputManager.h"
#include "ClWindowFinder.h"

#include "ConfigItemLoader.h"
#include "SequenceConfigItem.h"
#include "DisplayParam.h"
#include "SheetEditParam.h"
#include "CLOptionInfo.h"

#include "SnapPtDrawer.h"
#include "LoopToolData.h"
#include "LoopCutFeature.h"
#include "PartCamData.h"
#include "PartItem.h"
#include "Part.h"
#include "PartPlacementList.h"
#include "PartPlacement.h"
#include "PartInstanceList.h"
#include "clPartLayoutResource.h"
#include "PartPlacementManager.h"
#include "LoopInstanceList.h"
#include "LoopInstance.h"
#include "PartInstance.h"
#include "clGeometryFeatureResource.h"
#include "LinePattern.h"
#include "CanvasPatternAddCommand.h"
#include "CanvasPatternGripEditCommand.h"

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

CanvasMultiLinePatternEditAction::CanvasMultiLinePatternEditAction(PartPlacementPtr pPartPlacement, GlViewPortPtr pViewPort)
    : CanvasLinePatternEditAction(pPartPlacement, pViewPort)
{
}

CanvasMultiLinePatternEditAction::~CanvasMultiLinePatternEditAction(void)
{
}

BOOL CanvasMultiLinePatternEditAction::LButtonDown(double& dPtX, double& dPtY)
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
	else if (m_drawState == DRAW_LINE_START_POINT) // second click, we can create a line.
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
		pLinePat->UpdateCache();

		// 执行命令。
		PartCadDataPtr pPartCadData = m_pPartPlacement->GetPart()->GetCadData();
		PartCamDataPtr pPartCamData = m_pPartPlacement->GetCamData();
		ICommandPtr pCommand(new CanvasPatternAddCommand(GetEditorData(), pPartCadData, pPartCamData, pLinePat));
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

		// 做一些更新。
		ClWindowFinder::GetCurrentView_1()->SendMessage(WM_updateSheetPreviewData, 0, 0);
		ClWindowFinder::GetCurrentView_2()->SendMessage(WM_redrawSheetThumbnailOnSheetManagementPanel, 0, 0);
	}

	return TRUE;
}

BOOL CanvasMultiLinePatternEditAction::MovePoint(double& dPtX, double& dPtY)
{
	CanvasLinePatternEditAction::MovePoint(dPtX, dPtY);

	return TRUE;
}

END_CUTLEADER_NAMESPACE()

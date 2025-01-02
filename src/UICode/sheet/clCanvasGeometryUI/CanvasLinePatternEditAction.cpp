#include "StdAfx.h"
#include "CanvasLinePatternEditAction.h"

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
#include "PartCadData.h"

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

CanvasLinePatternEditAction::CanvasLinePatternEditAction(PartPlacementPtr pPartPlacement, GlViewPortPtr pViewPort)
    : CanvasPatternGripEditActionBase(pPartPlacement, pViewPort)
{
	m_drawState = DRAW_LINE_NOTSTART;
	m_editState = EDIT_LINE_NOTSTART;
	memset(m_data, 0, sizeof(double) * 4);
	m_iMsgID = IDS_OUTPUT_DRAWLINE_STARTPT;
}

CanvasLinePatternEditAction::~CanvasLinePatternEditAction(void)
{
}

BOOL CanvasLinePatternEditAction::LButtonDown(double& dPtX, double& dPtY)
{
	if (m_editState != EDIT_LINE_NOTSTART) // 夹点编辑
	{
		if (m_editState == EDIT_LINE_START_POINT)
		{
			m_data[0] = dPtX;
			m_data[1] = dPtY;
		}
		else if (m_editState == EDIT_LINE_CENTER_POINT)
		{
			double dOriCenterX = (m_data[0] + m_data[2]) / 2;
			double dOriCenterY = (m_data[1] + m_data[3]) / 2;

			m_data[0] += dPtX - dOriCenterX;
			m_data[1] += dPtY - dOriCenterY;
			m_data[2] += dPtX - dOriCenterX;
			m_data[3] += dPtY - dOriCenterY;
		}
		else if (m_editState == EDIT_LINE_END_POINT)
		{
			m_data[2] = dPtX;
			m_data[3] = dPtY;
		}

		// create the new line pattern with the new data.
		Point2D basePt(m_data[0], m_data[1]);
		Vector2D v(m_data[2] - m_data[0], m_data[3] - m_data[1]);
		IPatternPtr pNewPat(new LinePattern(basePt, v));

		// 执行命令。
		PartCadDataPtr pPartCadData = m_pPartPlacement->GetPart()->GetCadData();
		PartCamDataPtr pPartCamData = m_pPartPlacement->GetCamData();
		ICommandPtr pCommand(new CanvasPatternGripEditCommand(GetEditorData(), pPartCadData, pPartCamData, m_pGripEditPat->GetID(), pNewPat));
		pCommand->Do();
		CommandManager::GetInstance()->AddCommand(pCommand);

		// 选中刚刚编辑的几何特征。
		PostMessage2EnterCanvasPatternSelectAction_PreSelectOnePattern(m_pGripEditPat.get());

		// 做一些更新。
		ClWindowFinder::GetCurrentView_1()->SendMessage(WM_updateSheetPreviewData, 0, 0);
		ClWindowFinder::GetCurrentView_2()->SendMessage(WM_redrawSheetThumbnailOnSheetManagementPanel, 0, 0);
	}
	else // 绘制线段
	{
		if (m_drawState == DRAW_LINE_NOTSTART || m_drawState == DRAW_LINE_END_POINT) //first click
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
		else if (m_drawState == DRAW_LINE_START_POINT) //second click, we can create a line
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
			pLinePat->UpdateCache();

			// 执行命令。
			PartCadDataPtr pPartCadData = m_pPartPlacement->GetPart()->GetCadData();
			PartCamDataPtr pPartCamData = m_pPartPlacement->GetCamData();
			ICommandPtr pCommand(new CanvasPatternAddCommand(GetEditorData(), pPartCadData, pPartCamData, pLinePat));
			pCommand->Do();
			CommandManager::GetInstance()->AddCommand(pCommand);

			// finished drawing line, clear "m_pInputPtList".
			m_pInputPtList->clear();

			// 做一些更新。
			ClWindowFinder::GetCurrentView_1()->SendMessage(WM_updateSheetPreviewData, 0, 0);
			ClWindowFinder::GetCurrentView_2()->SendMessage(WM_redrawSheetThumbnailOnSheetManagementPanel, 0, 0);
		}
	}

	return TRUE;
}

BOOL CanvasLinePatternEditAction::MovePoint(double& dPtX, double& dPtY)
{
	if (m_editState != EDIT_LINE_NOTSTART) // 夹点编辑
	{
		if (m_editState == EDIT_LINE_START_POINT)
		{
			m_data[0] = dPtX;
			m_data[1] = dPtY;
		}
		else if (m_editState == EDIT_LINE_CENTER_POINT)
		{
			double dOriCenterX = (m_data[0] + m_data[2]) / 2;
			double dOriCenterY = (m_data[1] + m_data[3]) / 2;

			m_data[0] += dPtX - dOriCenterX;
			m_data[1] += dPtY - dOriCenterY;
			m_data[2] += dPtX - dOriCenterX;
			m_data[3] += dPtY - dOriCenterY;
		}
		else if (m_editState == EDIT_LINE_END_POINT)
		{
			m_data[2] = dPtX;
			m_data[3] = dPtY;
		}
	}
	else
	{
		if (m_drawState == DRAW_LINE_NOTSTART || m_drawState == DRAW_LINE_END_POINT) // 线段的起点还没确定。
		{
			;
		}
		else if (m_drawState == DRAW_LINE_START_POINT) // 已经确定了线段的起点。
		{
			m_data[2] = dPtX;
			m_data[3] = dPtY;
		}
	}

	return TRUE;
}

SnapInputPtr CanvasLinePatternEditAction::GetSnapInput() const
{
	SnapInputPtr pSnapInput = SnapInput::BuildSnapInput4SnapNone();

	PartCadDataPtr pPartCadData = m_pPartPlacement->GetPart()->GetCadData();
	DWORD dwSnapStyle = 0;
	if (m_editState != EDIT_LINE_NOTSTART) // 夹点编辑
	{
		dwSnapStyle = SnapEndPtOf2DLineArc | SnapMiddlePtOf2DLineArc | SnapCenterPtOf2DArc | SnapClosestPtTo2DLineArc | SnapClosestPtToOneAssistLine |
			SnapCrossPtOfTwo2DLineArc | SnapCrossPtOfTwoAssistLines;
		pSnapInput.reset(new SnapInput(dwSnapStyle, pPartCadData->GetPatternList(), Matrix2D()));
	}
	else // 绘制线段。
	{
		if (m_drawState == DRAW_LINE_NOTSTART || m_drawState == DRAW_LINE_END_POINT) // 线段的起点还没确定。
		{
			dwSnapStyle = SnapEndPtOf2DLineArc | SnapMiddlePtOf2DLineArc | SnapCenterPtOf2DArc | SnapClosestPtTo2DLineArc | SnapClosestPtToOneAssistLine |
				SnapCrossPtOfTwo2DLineArc | SnapCrossPtOfTwoAssistLines;
		}
		else if (m_drawState == DRAW_LINE_START_POINT) // 已经确定了线段的起点。
		{
			dwSnapStyle = SnapEndPtOf2DLineArc | SnapMiddlePtOf2DLineArc | SnapCenterPtOf2DArc | SnapClosestPtTo2DLineArc | SnapClosestPtToOneAssistLine |
				SnapFootPtTo2DLine | SnapTangencyPtTo2DArc | SnapCrossPtOfTwo2DLineArc | SnapCrossPtOfTwoAssistLines;
		}
		pSnapInput.reset(new SnapInput(dwSnapStyle, pPartCadData->GetPatternList(), Matrix2D()));
	}

	return pSnapInput;
}

BOOL CanvasLinePatternEditAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CanvasGeometryEditAction::OnKeyDown(nChar, nRepCnt, nFlags);

	return TRUE;
}

BOOL CanvasLinePatternEditAction::DrawBeforeFinish()
{
	const DisplayParam* pDisplayParam = CLOptionInfo::GetInstance()->GetDisplayParam().get();

	// 老的设置
	COLORREF dwOldColor;
	m_pViewPort->GetDrawColor(dwOldColor);
	int iOldLineWid = m_pViewPort->GetLineWidth();

	// 绘制线段。
	m_pViewPort->SetDrawColor(*pDisplayParam->GetPatColor());
	if (m_drawState == DRAW_LINE_START_POINT || m_editState != EDIT_LINE_NOTSTART)
		m_pViewPort->DrawLinePoint2D(m_data[0], m_data[1], m_data[2], m_data[3]);

	// 恢复设置
	m_pViewPort->SetDrawColor(dwOldColor);
	m_pViewPort->SetLineWidth(iOldLineWid);

	// 绘制光标。
	CursorDrawer::DrawCrossLineCursor(m_pViewPort);

	return TRUE;
}

void CanvasLinePatternEditAction::UpdateForUndoRedo()
{

}

void CanvasLinePatternEditAction::SetGripEditPattern(IPatternPtr pPattern)
{
	m_pGripEditPat = pPattern;

	LinePattern* pLinePattern = dynamic_cast<LinePattern*>(pPattern.get());

	// fill the m_data with the data in line pattern.
	Point2D startPt = pLinePattern->GetBasePoint();
	m_data[0] = startPt.X();
	m_data[1] = startPt.Y();

	Point2D endPt = pLinePattern->GetEndPoint();
	m_data[2] = endPt.X();
	m_data[3] = endPt.Y();
}

void CanvasLinePatternEditAction::SetGripEditPt(double dPtX, double dPtY)
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

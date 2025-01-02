#include "StdAfx.h"
#include "CanvasCirclePatternEditAction.h"

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
#include "CirclePattern.h"
#include "CanvasPatternAddCommand.h"
#include "CanvasPatternGripEditCommand.h"
#include "PartCadData.h"

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

CanvasCirclePatternEditAction::CanvasCirclePatternEditAction(PartPlacementPtr pPartPlacement, GlViewPortPtr pViewPort)
    : CanvasPatternGripEditActionBase(pPartPlacement, pViewPort)
{
	m_drawState = DRAW_CIRCLE_NOTSTART;
	m_editState = EDIT_CIRCLE_NOTSTART;
	memset(m_data, 0, sizeof(double) * 3);
	m_iMsgID = IDS_OUTPUT_PATCIRCLE_CENTERPT;
}

CanvasCirclePatternEditAction::~CanvasCirclePatternEditAction(void)
{
}

BOOL CanvasCirclePatternEditAction::LButtonDown(double& dPtX, double& dPtY)
{
	if (m_editState != EDIT_CIRCLE_NOTSTART) // 夹点编辑
	{
		if (m_editState == EDIT_CIRCLE_CENTER_POINT)
		{
			m_data[0] = dPtX;
			m_data[1] = dPtY;
		}
		else
		{
			m_data[2] = sqrt((dPtX - m_data[0]) * (dPtX - m_data[0]) + (dPtY - m_data[1]) * (dPtY - m_data[1]));
		}

		// create the new circle pattern with the new data.
		Point2D basePt(m_data[0], m_data[1]);
		IPatternPtr pNewPat(new CirclePattern(basePt, m_data[2]));

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
	else // 绘制圆
	{
		if (m_drawState == DRAW_CIRCLE_NOTSTART || m_drawState == DRAW_CIRCLE_END_POINT) //first click
		{
			m_drawState = DRAW_CIRCLE_CENTER_POINT;
			m_iMsgID = IDS_OUTPUT_PATCIRCLE_RADIUS;

			m_data[0] = dPtX;
			m_data[1] = dPtY;
			m_data[2] = 0;
		}
		else if (m_drawState == DRAW_CIRCLE_CENTER_POINT) //second click, we can create a circle
		{
			// 检查能不能创建圆。
			double dRadius = sqrt((dPtX - m_data[0]) * (dPtX - m_data[0]) + (dPtY - m_data[1]) * (dPtY - m_data[1]));
			if (dRadius < GEOM_TOLERANCE) {
				return FALSE;
			}

			m_drawState = DRAW_CIRCLE_END_POINT;
			m_iMsgID = IDS_OUTPUT_PATCIRCLE_CENTERPT;
			m_data[2] = dRadius;

			// 创建几何特征。
			IPatternPtr pCirclePat(new CirclePattern(Point2D(m_data[0], m_data[1]), m_data[2]));
			pCirclePat->UpdateCache();

			// 执行命令。
			PartCadDataPtr pPartCadData = m_pPartPlacement->GetPart()->GetCadData();
			PartCamDataPtr pPartCamData = m_pPartPlacement->GetCamData();
			ICommandPtr pCommand(new CanvasPatternAddCommand(GetEditorData(), pPartCadData, pPartCamData, pCirclePat));
			pCommand->Do();
			CommandManager::GetInstance()->AddCommand(pCommand);

			// 做一些更新。
			ClWindowFinder::GetCurrentView_1()->SendMessage(WM_updateSheetPreviewData, 0, 0);
			ClWindowFinder::GetCurrentView_2()->SendMessage(WM_redrawSheetThumbnailOnSheetManagementPanel, 0, 0);
		}
	}

	return TRUE;
}

BOOL CanvasCirclePatternEditAction::MovePoint(double& dPtX, double& dPtY)
{
	if (m_editState != EDIT_CIRCLE_NOTSTART) //EDIT mode
	{
		if (m_editState == EDIT_CIRCLE_CENTER_POINT)
		{
			m_data[0] = dPtX;
			m_data[1] = dPtY;
		}
		else
		{
			m_data[2] = sqrt((dPtX - m_data[0]) * (dPtX - m_data[0]) + (dPtY - m_data[1]) * (dPtY - m_data[1]));
		}
	}
	else
	{
		if (m_drawState == DRAW_CIRCLE_CENTER_POINT)
		{
			m_data[2] = sqrt((dPtX - m_data[0]) * (dPtX - m_data[0]) + (dPtY - m_data[1]) * (dPtY - m_data[1]));
		}
	}

	return TRUE;
}

BOOL CanvasCirclePatternEditAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CanvasGeometryEditAction::OnKeyDown(nChar, nRepCnt, nFlags);

	return TRUE;
}

BOOL CanvasCirclePatternEditAction::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	return TRUE;
}

BOOL CanvasCirclePatternEditAction::DrawBeforeFinish()
{
	const DisplayParam* pDisplayParam = CLOptionInfo::GetInstance()->GetDisplayParam().get();

	// 老的设置
	COLORREF dwOldColor;
	m_pViewPort->GetDrawColor(dwOldColor);
	int iOldLineWid = m_pViewPort->GetLineWidth();

	// 绘制圆。
	m_pViewPort->SetDrawColor(*pDisplayParam->GetPatColor());
	if (m_drawState == DRAW_CIRCLE_CENTER_POINT || m_editState != EDIT_CIRCLE_NOTSTART)
		m_pViewPort->DrawCircle2D(m_data[0], m_data[1], m_data[2]);

	// 恢复设置
	m_pViewPort->SetDrawColor(dwOldColor);
	m_pViewPort->SetLineWidth(iOldLineWid);

	// 绘制光标。
	CursorDrawer::DrawCrossLineCursor(m_pViewPort);

	return TRUE;
}

void CanvasCirclePatternEditAction::UpdateForUndoRedo()
{
}

void CanvasCirclePatternEditAction::SetGripEditPattern(IPatternPtr pPattern)
{
	m_pGripEditPat = pPattern;

	CirclePattern* pCirclePat = dynamic_cast<CirclePattern*>(pPattern.get());

	// fill the m_data with the data in line pattern.
	Point2D startPt = pCirclePat->GetBasePoint();
	m_data[0] = startPt.X();
	m_data[1] = startPt.Y();

	m_data[2] = pCirclePat->GetRadius();
}

void CanvasCirclePatternEditAction::SetGripEditPt(double dPtX, double dPtY)
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
				m_editState = EDIT_CIRCLE_CENTER_POINT;
				m_iMsgID = IDS_OUTPUT_PATCIRCLE_CENTERPT;
			}
			else if (i == 1)
			{
				m_editState = EDIT_CIRCLE_TOP_POINT;
				m_iMsgID = IDS_OUTPUT_PATCIRCLE_RADIUS;
			}
			else if (i == 2)
			{
				m_editState = EDIT_CIRCLE_LEFT_POINT;
				m_iMsgID = IDS_OUTPUT_PATCIRCLE_RADIUS;
			}
			else if (i == 3)
			{
				m_editState = EDIT_CIRCLE_BOTTOM_POINT;
				m_iMsgID = IDS_OUTPUT_PATCIRCLE_RADIUS;
			}
			else if (i == 4)
			{
				m_editState = EDIT_CIRCLE_RIGHT_POINT;
				m_iMsgID = IDS_OUTPUT_PATCIRCLE_RADIUS;
			}

			return;
		}
	}
}

END_CUTLEADER_NAMESPACE()

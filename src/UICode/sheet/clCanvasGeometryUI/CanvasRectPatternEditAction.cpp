#include "StdAfx.h"
#include "CanvasRectPatternEditAction.h"

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
#include "RectHolePattern.h"
#include "CanvasPatternAddCommand.h"
#include "CanvasPatternGripEditCommand.h"

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

CanvasRectPatternEditAction::CanvasRectPatternEditAction(PartPlacementPtr pPartPlacement, GlViewPortPtr pViewPort)
    : CanvasPatternGripEditActionBase(pPartPlacement, pViewPort)
{
	m_drawState = DRAW_RECTHOLE_NOTSTART;
	m_editState = EDIT_RECTHOLE_NOTSTART;
	memset(m_data, 0, sizeof(double) * 5);
	m_iMsgID = IDS_OUTPUT_RECTHOLE_CENTERPT;
}

CanvasRectPatternEditAction::~CanvasRectPatternEditAction(void)
{
}

BOOL CanvasRectPatternEditAction::LButtonDown(double& dPtX, double& dPtY)
{
	if (m_editState != EDIT_RECTHOLE_NOTSTART) //EDIT mode
	{
		// create the new RECTHOLE pattern with the new data.
		Point2D startPt(m_data[0], m_data[1]);
		Point2D endPt(m_data[2], m_data[3]);
		IPatternPtr pNewPat(new RectHolePattern(startPt, endPt, m_data[4]));

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
	else
	{
		if (m_drawState == DRAW_RECTHOLE_NOTSTART || m_drawState == DRAW_RECTHOLE_END_POINT) //first click
		{
			m_drawState = DRAW_RECTHOLE_START_POINT;
			m_iMsgID = IDS_OUTPUT_RECTHOLE_LENGTH_WIDTH;

			m_data[0] = dPtX;
			m_data[1] = dPtY;
			m_data[2] = dPtX;
			m_data[3] = dPtY;
		}
		else if (m_drawState == DRAW_RECTHOLE_START_POINT) //second click, we can create a rect hole.
		{
			// 检查能不能创建矩形孔。
			if (fabs(m_data[0] - dPtX) < GEOM_TOLERANCE || fabs(m_data[1] - dPtY) < GEOM_TOLERANCE) {
				return FALSE;
			}

			m_drawState = DRAW_RECTHOLE_END_POINT;
			m_iMsgID = IDS_OUTPUT_RECTHOLE_CENTERPT;

			m_data[2] = dPtX;
			m_data[3] = dPtY;

			// create rect hole pattern.
			Point2D startPt(m_data[0], m_data[1]);
			Point2D endPt(m_data[2], m_data[3]);
			IPatternPtr pRectHolePat(new RectHolePattern(startPt, endPt, m_data[4]));
			pRectHolePat->UpdateCache();

			// 执行命令。
			PartCadDataPtr pPartCadData = m_pPartPlacement->GetPart()->GetCadData();
			PartCamDataPtr pPartCamData = m_pPartPlacement->GetCamData();
			ICommandPtr pCommand(new CanvasPatternAddCommand(GetEditorData(), pPartCadData, pPartCamData, pRectHolePat));
			pCommand->Do();
			CommandManager::GetInstance()->AddCommand(pCommand);

			// 做一些更新。
			ClWindowFinder::GetCurrentView_1()->SendMessage(WM_updateSheetPreviewData, 0, 0);
			ClWindowFinder::GetCurrentView_2()->SendMessage(WM_redrawSheetThumbnailOnSheetManagementPanel, 0, 0);
		}
	}

	return TRUE;
}

BOOL CanvasRectPatternEditAction::MovePoint(double& dPtX, double& dPtY)
{
	if (m_editState != EDIT_RECTHOLE_NOTSTART) //EDIT mode
	{
		if (m_editState == EDIT_RECTHOLE_CENTER_POINT)
		{
			//center point move dX, dY in horizontal and vertical 
			double dX = dPtX - (m_data[0] + m_data[2]) / 2;
			double dY = dPtY - (m_data[1] + m_data[3]) / 2;

			//get start point and end point after move
			m_data[0] += dX;
			m_data[1] += dY;
			m_data[2] += dX;
			m_data[3] += dY;
		}
		else if (m_editState == EDIT_RECTHOLE_START_POINT)
		{
			m_data[0] = dPtX;
			m_data[1] = dPtY;
		}
		else if (m_editState == EDIT_RECTHOLE_END_POINT)
		{
			m_data[2] = dPtX;
			m_data[3] = dPtY;
		}
		else if (m_editState == EDIT_RECTHOLE_THIRD_POINT || m_editState == EDIT_RECTHOLE_FOUR_POINT)
		{
			ScalePattern(dPtX, dPtY);
		}
	}
	else
	{
		if (m_drawState == DRAW_RECTHOLE_START_POINT)
		{
			m_data[2] = dPtX;
			m_data[3] = dPtY;
		}
	}

	return TRUE;
}

BOOL CanvasRectPatternEditAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CanvasGeometryEditAction::OnKeyDown(nChar, nRepCnt, nFlags);

	return TRUE;
}

BOOL CanvasRectPatternEditAction::DrawBeforeFinish()
{
	const DisplayParam* pDisplayParam = CLOptionInfo::GetInstance()->GetDisplayParam().get();

	// 老的设置
	COLORREF dwOldColor;
	m_pViewPort->GetDrawColor(dwOldColor);
	int iOldLineWid = m_pViewPort->GetLineWidth();

	// 绘制矩形。
	m_pViewPort->SetDrawColor(*pDisplayParam->GetPatColor());
	if (m_drawState == DRAW_RECTHOLE_START_POINT || m_editState != EDIT_RECTHOLE_NOTSTART)
	{
		/*
			(dX1, dY1)        (dX2, dY2)
			 ______________
			|			   |
			|			   |
			|______________|

			(dX4, dY4)        (dX3, dY3) (new)
		*/

		double dAngle = m_data[4];
		double dX1 = m_data[0];
		double dY1 = m_data[1];
		double dX3 = m_data[2];
		double dY3 = m_data[3];

		// if dAngle not equal zero, rotate rectangle (-dAngle) with center start point
		// get new x y of point 2,point 3 and point 4, dX1 dY1 not changed
		double dX3new, dY3new;
		MathEx::RotatePoint(dX1, dY1, -dAngle, dX3, dY3, dX3new, dY3new);

		// rotate back dAngle also use start point as center,
		double dX2 = .0, dY2 = .0, dX4 = .0, dY4 = .0;
		MathEx::RotatePoint(dX1, dY1, dAngle, dX3new, dY1, dX2, dY2);
		MathEx::RotatePoint(dX1, dY1, dAngle, dX1, dY3new, dX4, dY4);
		m_pViewPort->DrawLinePoint2D(dX1, dY1, dX2, dY2);
		m_pViewPort->DrawLinePoint2D(dX2, dY2, dX3, dY3);
		m_pViewPort->DrawLinePoint2D(dX3, dY3, dX4, dY4);
		m_pViewPort->DrawLinePoint2D(dX4, dY4, dX1, dY1);
	}

	// 恢复设置
	m_pViewPort->SetDrawColor(dwOldColor);
	m_pViewPort->SetLineWidth(iOldLineWid);

	// 绘制光标。
	CursorDrawer::DrawCrossLineCursor(m_pViewPort);

	return TRUE;
}

void CanvasRectPatternEditAction::UpdateForUndoRedo()
{

}

void CanvasRectPatternEditAction::SetGripEditPattern(IPatternPtr pPattern)
{
	m_pGripEditPat = pPattern;

	RectHolePattern* pRectHolePat = dynamic_cast<RectHolePattern*>(pPattern.get());

	// fill the m_data with the data in line pattern.
	Point2D startPt = pRectHolePat->GetStartPoint();
	m_data[0] = startPt.X();
	m_data[1] = startPt.Y();

	Point2D endPt = pRectHolePat->GetEndPoint();
	m_data[2] = endPt.X();
	m_data[3] = endPt.Y();

	m_data[4] = pRectHolePat->GetBaseVector().Angle();
}

void CanvasRectPatternEditAction::SetGripEditPt(double dPtX, double dPtY)
{
	double dX2 = .0, dY2 = .0, dX4 = .0, dY4 = .0;
	double dAngle = m_data[4];

	double dX1 = m_data[0];
	double dY1 = m_data[1];
	double dX3 = m_data[2];
	double dY3 = m_data[3];

	// if dAngle not equal zero, rotate rectangle (-dAngle) with center start point
	// get new x y of point 2,point 3 and point 4, dX1 dY1 not changed
	double dX3new, dY3new;
	MathEx::RotatePoint(dX1, dY1, -dAngle, dX3, dY3, dX3new, dY3new);

	// rotate back dAngle also use start point as center,
	MathEx::RotatePoint(dX1, dY1, dAngle, dX3new, dY1, dX2, dY2);
	MathEx::RotatePoint(dX1, dY1, dAngle, dX1, dY3new, dX4, dY4);

	if (MathEx::PointEqual(dPtX, dPtY, m_data[2], m_data[3]))
	{
		m_editState = EDIT_RECTHOLE_END_POINT;
		m_iMsgID = IDS_OUTPUT_RECTHOLE_LENGTH_WIDTH;
	}
	else if (MathEx::PointEqual(dPtX, dPtY, m_data[0], m_data[1]))
	{
		m_editState = EDIT_RECTHOLE_START_POINT;
		m_iMsgID = IDS_OUTPUT_RECTHOLE_LENGTH_WIDTH;
	}
	else if (MathEx::PointEqual(dPtX, dPtY, dX2, dY2))
	{
		m_editState = EDIT_RECTHOLE_THIRD_POINT;
		m_iMsgID = IDS_OUTPUT_RECTHOLE_LENGTH_WIDTH;
	}
	else if (MathEx::PointEqual(dPtX, dPtY, dX4, dY4))
	{
		m_editState = EDIT_RECTHOLE_FOUR_POINT;
		m_iMsgID = IDS_OUTPUT_RECTHOLE_LENGTH_WIDTH;
	}
	else if (MathEx::PointEqual(dPtX, dPtY, (m_data[0] + m_data[2]) / 2, (m_data[1] + m_data[3]) / 2))
	{
		m_editState = EDIT_RECTHOLE_CENTER_POINT;
		m_iMsgID = IDS_OUTPUT_RECTHOLE_CENTERPT;
	}
}

void CanvasRectPatternEditAction::ScalePattern(double PointX, double PointY)
{
	double dAngle = m_data[4];

	double dX1, dY1, dX3, dY3;
	dX1 = m_data[0];
	dY1 = m_data[1];
	dX3 = m_data[2];
	dY3 = m_data[3];

	double x1new, y1new, dX3new, dY3new;
	double x3r, y3r, x3newr, y3newr, x1newr, y1newr, x2newr, y2newr;//four points after rotate

	// rotate -dAngle
	MathEx::RotatePoint(dX1, dY1, -dAngle, dX3, dY3, x3r, y3r);
	MathEx::RotatePoint(dX1, dY1, -dAngle, PointX, PointY, x2newr, y2newr);

	if (m_editState == EDIT_RECTHOLE_THIRD_POINT)
	{
		x1newr = dX1;
		y1newr = y2newr;

		x3newr = x2newr;
		y3newr = y3r;
	}
	else if (m_editState == EDIT_RECTHOLE_FOUR_POINT)
	{
		x1newr = x2newr;
		y1newr = dY1;

		x3newr = x3r;
		y3newr = y2newr;
	}

	// rotate back
	MathEx::RotatePoint(dX1, dY1, dAngle, x1newr, y1newr, x1new, y1new);
	MathEx::RotatePoint(dX1, dY1, dAngle, x3newr, y3newr, dX3new, dY3new);

	m_data[0] = x1new;
	m_data[1] = y1new;
	m_data[2] = dX3new;
	m_data[3] = dY3new;
}

END_CUTLEADER_NAMESPACE()

#include "StdAfx.h"

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

#include "CanvasRectLinePatternEditAction.h"

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

CanvasRectLinePatternEditAction::CanvasRectLinePatternEditAction(PartPlacementPtr pPartPlacement, GlViewPortPtr pViewPort)
    : CanvasPatternGripEditActionBase(pPartPlacement, pViewPort)
{
	m_drawState = DRAW_RECTLINE_NOTSTART;
	memset(m_data, 0, sizeof(double) * 4);
	m_iMsgID = IDS_OUTPUT_RECTLINE_STARTCORNER;
}

CanvasRectLinePatternEditAction::~CanvasRectLinePatternEditAction(void)
{
}

BOOL CanvasRectLinePatternEditAction::LButtonDown( double &dPtX, double &dPtY )
{
	switch (m_drawState)
	{
	case DRAW_RECTLINE_NOTSTART:
	case DRAW_RECTLINE_END_POINT:
	{
		m_drawState = DRAW_RECTLINE_START_POINT;
		m_iMsgID = IDS_OUTPUT_RECTLINE_ENDCORNER;

		m_data[0] = dPtX;
		m_data[1] = dPtY;
		m_data[2] = dPtX;
		m_data[3] = dPtY;

		break;
	}
	case DRAW_RECTLINE_START_POINT:
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
		Vector2D vect1(m_data[2] - m_data[0], 0);
		IPatternPtr pLinePat1(new LinePattern(basePt1, vect1));
		pNewPatList->push_back(pLinePat1);

		// line pattern2.
		Point2D basePt2(m_data[0], m_data[1]);
		Vector2D vect2(0, m_data[3] - m_data[1]);
		IPatternPtr pLinePat2(new LinePattern(basePt2, vect2));
		pNewPatList->push_back(pLinePat2);

		// line pattern3.
		Point2D basePt3(m_data[0], m_data[3]);
		Vector2D vect3(m_data[2] - m_data[0], 0);
		IPatternPtr pLinePat3(new LinePattern(basePt3, vect3));
		pNewPatList->push_back(pLinePat3);

		// line pattern4.
		Point2D basePt4(m_data[2], m_data[1]);
		Vector2D vect4(0, m_data[3] - m_data[1]);
		IPatternPtr pLinePat4(new LinePattern(basePt4, vect4));
		pNewPatList->push_back(pLinePat4);

		// update the cache data of the pattern object.
		pNewPatList->UpdateCache();
		/************************************************************************/


		// create four commands
		PartCadDataPtr pPartCadData = m_pPartPlacement->GetPart()->GetCadData();
		PartCamDataPtr pPartCamData = m_pPartPlacement->GetCamData();
		ICommandPtr pCommand1(new CanvasPatternAddCommand(GetEditorData(), pPartCadData, pPartCamData, pLinePat1));
		ICommandPtr pCommand2(new CanvasPatternAddCommand(GetEditorData(), pPartCadData, pPartCamData, pLinePat2));
		ICommandPtr pCommand3(new CanvasPatternAddCommand(GetEditorData(), pPartCadData, pPartCamData, pLinePat3));
		ICommandPtr pCommand4(new CanvasPatternAddCommand(GetEditorData(), pPartCadData, pPartCamData, pLinePat4));
		pCommand1->Do();
		pCommand2->Do();
		pCommand3->Do();
		pCommand4->Do();
		CommandManager::GetInstance()->AddCommand(pCommand1);
		CommandManager::GetInstance()->AddCommand(pCommand2);
		CommandManager::GetInstance()->AddCommand(pCommand3);
		CommandManager::GetInstance()->AddCommand(pCommand4);

		// 做一些更新。
		ClWindowFinder::GetCurrentView_1()->SendMessage(WM_updateSheetPreviewData, 0, 0);
		ClWindowFinder::GetCurrentView_2()->SendMessage(WM_redrawSheetThumbnailOnSheetManagementPanel, 0, 0);

		break;
	}
	default:
		break;
	}

	return TRUE;
}

BOOL CanvasRectLinePatternEditAction::MovePoint( double &dPtX, double &dPtY )
{
	if( m_drawState == DRAW_RECTLINE_START_POINT ) {
		m_data[ 2 ] = dPtX;
		m_data[ 3 ] = dPtY;
	}

	return TRUE;
}

void CanvasRectLinePatternEditAction::ConvertKBInput( double &dVal1, double &dVal2 )
{
	// now, do not need to convert.
}

BOOL CanvasRectLinePatternEditAction::OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags )
{
	CanvasGeometryEditAction::OnKeyDown( nChar, nRepCnt, nFlags );

	return TRUE;
}

BOOL CanvasRectLinePatternEditAction::OnKeyUp( UINT nChar, UINT nRepCnt, UINT nFlags )
{
	return TRUE;
}

BOOL CanvasRectLinePatternEditAction::DrawBeforeFinish()
{
	CLOptionInfoPtr pCLOptionInfo = CLOptionInfo::GetInstance();
	DisplayParamPtr pColorInfo = pCLOptionInfo->GetDisplayParam();

	// keep the old color.
	COLORREF dwOldColor;
	m_pViewPort->GetDrawColor( dwOldColor );

	//
	m_pViewPort->SetDrawColor( *pColorInfo->GetPatColor() );

	if( m_drawState == DRAW_RECTLINE_START_POINT ) {
		m_pViewPort->DrawLinePoint2D( m_data[ 0 ], m_data[ 1 ], m_data[ 2 ], m_data[ 1 ] );
		m_pViewPort->DrawLinePoint2D( m_data[ 0 ], m_data[ 1 ], m_data[ 0 ], m_data[ 3 ] );
		m_pViewPort->DrawLinePoint2D( m_data[ 0 ], m_data[ 3 ], m_data[ 2 ], m_data[ 3 ] );
		m_pViewPort->DrawLinePoint2D( m_data[ 2 ], m_data[ 1 ], m_data[ 2 ], m_data[ 3 ] );
	}

	// draw cursor.
	CursorDrawer::DrawCrossLineCursor( m_pViewPort );

	// restore the draw color
	m_pViewPort->SetDrawColor( dwOldColor );

	return TRUE;
}

void CanvasRectLinePatternEditAction::SetGripEditPattern( IPatternPtr pPattern )
{
	// do nothing
}

void CanvasRectLinePatternEditAction::SetGripEditPt( double dPtX, double dPtY )
{
	// do nothing
}

SnapInputPtr CanvasRectLinePatternEditAction::GetSnapInput() const
{
	SnapInputPtr pSnapInput = SnapInput::BuildSnapInput4SnapNone();

	PartCadDataPtr pPartCadData = m_pPartPlacement->GetPart()->GetCadData();
	DWORD dwSnapStyle = SnapEndPtOf2DLineArc | SnapMiddlePtOf2DLineArc | SnapCenterPtOf2DArc | SnapClosestPtTo2DLineArc | SnapClosestPtToOneAssistLine |
		SnapFootPtTo2DLine | SnapTangencyPtTo2DArc | SnapCrossPtOfTwo2DLineArc | SnapCrossPtOfTwoAssistLines;;
	pSnapInput.reset( new SnapInput( dwSnapStyle, pPartCadData->GetPatternList(), Matrix2D() ) );
	return pSnapInput;
}

END_CUTLEADER_NAMESPACE()

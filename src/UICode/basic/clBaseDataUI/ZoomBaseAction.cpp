#include "StdAfx.h"
#include "ZoomBaseAction.h"

#include "math.h"
#include "GlViewPort.h"
#include "CursorDrawer.h"

#include "IEditor.h"
#include "clUILibResource.h"
#include "DummyAction.h"

BEGIN_CUTLEADER_NAMESPACE()

ZoomBaseAction::ZoomBaseAction(void)
{
	m_state = ZOOM_STATE_NONE;

	m_basePtX = 0.0;
	m_basePtY = 0.0;
	m_secondPtX = 0.0;
	m_secondPtY = 0.0;

	m_iMsgID = IDS_OUTPUT_ZOOM_STARTCORNER;
}

ZoomBaseAction::~ZoomBaseAction(void)
{
}

BOOL ZoomBaseAction::LButtonDown(double& dPtX, double& dPtY)
{
	switch (m_state) 
	{
	case ZOOM_STATE_NONE:
	case ZOOM_STATE_SELECT_SECONDPOINT:
		{
			m_basePtX = dPtX;
			m_basePtY = dPtY;
			m_secondPtX = dPtX;
			m_secondPtY = dPtY;

			m_state = ZOOM_STATE_SELECT_BASEPOINT;
			m_iMsgID = IDS_OUTPUT_ZOOM_ENDCORNER;
		}
		break;
	case ZOOM_STATE_SELECT_BASEPOINT:
		{
			if ((dPtX != m_basePtX) && (dPtY != m_basePtY))
			{
				// adjust the second point.
				AdjustSecondPt();

				m_state = ZOOM_STATE_SELECT_SECONDPOINT;
				m_iMsgID = IDS_OUTPUT_ZOOM_STARTCORNER;
			}

			// here, we should change the view port.
			double dWorldRange, dScreenLength, dScreenHeight;
			double dLeftBottomX, dLeftBottomY;
			GetZoomPort(dWorldRange, dScreenLength, dScreenHeight, dLeftBottomX, dLeftBottomY);
			m_pViewPort->SetDrawingArea(dWorldRange, dScreenLength, dScreenHeight, dLeftBottomX, dLeftBottomY);
		}
		break;
	default:
		break;
	}

	return TRUE;
}

BOOL ZoomBaseAction::MovePoint(double& dPtX, double& dPtY)
{
	switch (m_state) 
	{
	case ZOOM_STATE_NONE:
		break;
	case ZOOM_STATE_SELECT_BASEPOINT:
		{
			if ((dPtX != m_basePtX) && (dPtY != m_basePtY))
			{
				m_secondPtX = dPtX;
				m_secondPtY = dPtY;
			}
		}
		break;
	case ZOOM_STATE_SELECT_SECONDPOINT:
		break;
	default:
		break;
	}

	return TRUE;
}

BOOL ZoomBaseAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// switch to dummy action.
	m_pNextAction.reset(new DummyAction(m_pViewPort));

	return TRUE;
}

BOOL ZoomBaseAction::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	return TRUE;
}

BOOL ZoomBaseAction::DrawBeforeFinish()
{
	// draw cursor.
	CursorDrawer::DrawCrossLineCursor(m_pViewPort);

	if (m_state != ZOOM_STATE_SELECT_BASEPOINT)
		return FALSE;

	// adjust the second point.
	AdjustSecondPt();

	// get the old drawer config.
	DWORD dwOldColor;
	int iOldLineWid = m_pViewPort->GetLineWidth();
	m_pViewPort->GetDrawColor(dwOldColor);

	// draw rectangle.
	m_pViewPort->SetLineWidth(1);
	m_pViewPort->SetDrawColor(RGB(255,0,0));
	m_pViewPort->DrawLinePoint2D(m_basePtX, m_basePtY, m_secondPtX, m_basePtY);
	m_pViewPort->DrawLinePoint2D(m_secondPtX, m_basePtY, m_secondPtX, m_secondPtY);
	m_pViewPort->DrawLinePoint2D(m_secondPtX, m_secondPtY, m_basePtX, m_secondPtY);
	m_pViewPort->DrawLinePoint2D(m_basePtX, m_secondPtY, m_basePtX, m_basePtY);

	// restore the old drawer config.
	m_pViewPort->SetLineWidth(iOldLineWid);
	m_pViewPort->SetDrawColor(dwOldColor);

	return TRUE;
}

BOOL ZoomBaseAction::AdjustSecondPt()
{
	double dWorldRange, dScreenLength, dScreenHeight, dLeftBottomX, dLeftBottomY;
	m_pViewPort->GetDrawingArea(dWorldRange, dScreenLength, dScreenHeight, dLeftBottomX, dLeftBottomY);

	// get length / height of screen and cur two point
	if (dScreenHeight == 0.0)
		return FALSE;

	double dScreenLHRatio = dScreenLength/dScreenHeight;
	double dCurPointLHRatio = dScreenLHRatio;

	if ((m_secondPtX - m_basePtX) != 0.0)
	{
		dCurPointLHRatio = (m_secondPtX - m_basePtX) / (m_secondPtY - m_basePtY);
	}

	// change second point's position to make sure two rect was same length / height ratio
	if (fabs(dCurPointLHRatio) >= dScreenLHRatio)
	{
		m_secondPtY = m_basePtY + (((m_secondPtX - m_basePtX)/dScreenLHRatio) *
								   (dCurPointLHRatio/fabs(dCurPointLHRatio)));
	}
	else
	{
		m_secondPtX = m_basePtX + (((m_secondPtY - m_basePtY)*dScreenLHRatio) *
								   (dCurPointLHRatio/fabs(dCurPointLHRatio)));
	}

	return TRUE;
}

END_CUTLEADER_NAMESPACE()

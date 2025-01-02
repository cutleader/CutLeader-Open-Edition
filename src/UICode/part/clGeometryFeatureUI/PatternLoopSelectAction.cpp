#include "StdAfx.h"
#include "PatternLoopSelectAction.h"

#include "GlViewPort.h"
#include "DummyAction.h"
#include "CursorDrawer.h"

#include "CLOptionInfo.h"
#include "DisplayParam.h"

#include "LoopDrawer.h"
#include "PatternDrawer.h"
#include "IPatternLoop.h"

BEGIN_CUTLEADER_NAMESPACE()

PatternLoopSelectAction::PatternLoopSelectAction(PartCadDataPtr pPartCadData, GlViewPortPtr pViewPort)
{
	m_pPartCadData = pPartCadData;
	m_pViewPort = pViewPort;

	m_loopDrawerPtr.reset(new LoopDrawer(pViewPort));
}

PatternLoopSelectAction::~PatternLoopSelectAction(void)
{
}

BOOL PatternLoopSelectAction::LButtonDown(double& dPtX, double& dPtY)
{
	return TRUE;
}

BOOL PatternLoopSelectAction::LButtonUp(double& dPtX, double& dPtY)
{
	return TRUE;
}

BOOL PatternLoopSelectAction::MovePoint(double& dPtX, double& dPtY)
{
	return TRUE;
}

BOOL PatternLoopSelectAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar) 
	{
	case VK_ESCAPE:
		{
			// switch to dummy action.
			m_pNextAction.reset(new DummyAction(m_pViewPort));
		}
		break;
	default:
		break;
	}

	return TRUE;
}

BOOL PatternLoopSelectAction::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	return TRUE;
}

BOOL PatternLoopSelectAction::DrawBeforeFinish()
{
	CLOptionInfoPtr pCLOptionInfo = CLOptionInfo::GetInstance();
	DisplayParamPtr pColorInfo = pCLOptionInfo->GetDisplayParam();

	COLORREF dwOldColor;
	m_pViewPort->GetDrawColor(dwOldColor);
	ROP_MODE ropMode;
	m_pViewPort->GetROP(ropMode);

	/************************************************************************/
	// draw the selected loop in highlight mode.

	if (m_pSelectedLoop)
	{
		m_pViewPort->SetROP(ROP_COPY);
		m_pViewPort->SetLineWidth(2);
		m_pViewPort->SetDrawColor(*pColorInfo->GetSelectObjColor());

		m_loopDrawerPtr->DrawPatternLoop(m_pSelectedLoop.get());

		m_pViewPort->SetLineWidth(1);
	}
	/************************************************************************/

	// restore the drawer
	m_pViewPort->SetROP(ropMode);
	m_pViewPort->SetDrawColor(dwOldColor);

	// draw cursor.
	CursorDrawer::DrawCrossLineCursor(m_pViewPort);

	return TRUE;
}

END_CUTLEADER_NAMESPACE()

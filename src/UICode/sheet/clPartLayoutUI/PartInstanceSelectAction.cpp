#include "StdAfx.h"
#include "PartInstanceSelectAction.h"

#include "DummyAction.h"
#include "CursorDrawer.h"

#include "PartPlacementList.h"
#include "PartPlacementDrawer.h"

BEGIN_CUTLEADER_NAMESPACE()

PartInstanceSelectAction::PartInstanceSelectAction(GlViewPortPtr pViewPort)
{
	m_pViewPort = pViewPort;
}

PartInstanceSelectAction::~PartInstanceSelectAction(void)
{
}

BOOL PartInstanceSelectAction::LButtonDown(double& dPtX, double& dPtY)
{
	return TRUE;
}

BOOL PartInstanceSelectAction::LButtonUp(double& dPtX, double& dPtY)
{
	return TRUE;
}

BOOL PartInstanceSelectAction::MovePoint(double& dPtX, double& dPtY)
{
	return TRUE;
}

BOOL PartInstanceSelectAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
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

BOOL PartInstanceSelectAction::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	return TRUE;
}

BOOL PartInstanceSelectAction::DrawBeforeFinish()
{
	PartPlacementDrawer partPlacementDrawer(m_pViewPort);
	partPlacementDrawer.DrawSelectedPartInstances(m_pSelPartInstanceList.get());

	// draw cursor.
	CursorDrawer::DrawDefaultCursor(m_pViewPort);

	return TRUE;
}

END_CUTLEADER_NAMESPACE()

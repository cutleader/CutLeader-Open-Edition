#include "StdAfx.h"
#include "AddPartFromCanvasAction.h"

#include "GlViewPort.h"
#include "CommandManager.h"
#include "CursorDrawer.h"
#include "clUILibResource.h"
#include "ClWindowFinder.h"

#include "DisplayParam.h"
#include "CLOptionInfo.h"

#include "PartCamData.h"
#include "Part.h"
#include "PartPlacement.h"
#include "clCanvasGeometryUIResource.h"
#include "PatternList.h"
#include "PatternDrawer.h"
#include "GeometryFeatureHelper.h"

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

AddPartFromCanvasAction::AddPartFromCanvasAction(PartPlacementPtr pPartPlacement, PatternListPtr pSelectedPatterns,
	SelectPatternOrLoopActionBar* pSelectPatternOrLoopActionBar, GlViewPortPtr pViewPort)
    : CanvasPatternPreSelectAction(pPartPlacement, pSelectPatternOrLoopActionBar, pViewPort)
{
	m_pSelectedPatterns = pSelectedPatterns;
	m_loopsOfSelectedPatterns = GeometryFeatureHelper::FindPatternLoopsOfPatterns(m_pPartPlacement->GetPart()->GetCadData().get(), m_pSelectedPatterns.get());
	m_iMsgID = IDS_SelectPatternAdding2Task;
	if (m_pSelectedPatterns->size() == 0) {
		m_pSelectPatternOrLoopActionBar->DisplayActionBar(SelectLoop);
	}
}

AddPartFromCanvasAction::~AddPartFromCanvasAction(void)
{
}

BOOL AddPartFromCanvasAction::LButtonDown(double& dPtX, double& dPtY)
{
	if (m_pSelectedPatterns->size() == 0)
	{
		CanvasPatternPreSelectAction::LButtonDown(dPtX, dPtY);
	}

	return TRUE;
}

BOOL AddPartFromCanvasAction::LButtonUp(double& dPtX, double& dPtY)
{
	if (m_pSelectedPatterns->size() == 0)
	{
		CanvasPatternPreSelectAction::LButtonUp(dPtX, dPtY);

		if (m_pSelectedPatterns->size() > 0)
		{
			AddSelectedPattern2PartLibrary();
		}
	}

	return TRUE;
}

BOOL AddPartFromCanvasAction::MovePoint(double& dPtX, double& dPtY)
{
	if (m_pSelectedPatterns->size() == 0) {
		__super::MovePoint(dPtX, dPtY);
	}
	else {
		m_pSelectPatternOrLoopActionBar->HideActionBar();
	}

	return TRUE;
}

BOOL AddPartFromCanvasAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	__super::OnKeyDown(nChar, nRepCnt, nFlags);

	return TRUE;
}

BOOL AddPartFromCanvasAction::DrawBeforeFinish()
{
	const DisplayParam* pDisplayParam = CLOptionInfo::GetInstance()->GetDisplayParam().get();
	PatternDrawer patDrawer(m_pViewPort);

	if (m_pSelectedPatterns->size() == 0)
	{
		CanvasPatternPreSelectAction::DrawBeforeFinish();
	}
	else
	{
		// draw the selected patterns.
		DrawSelectedPatterns(m_pSelectedPatterns.get(), m_loopsOfSelectedPatterns);

		// draw cursor.
		CursorDrawer::DrawAimSelCursor(m_pViewPort);
	}

	return TRUE;
}

void AddPartFromCanvasAction::UpdateForUndoRedo()
{
}

void AddPartFromCanvasAction::AddSelectedPattern2PartLibrary()
{
	// 发消息让任务视图来完成这个操作。
	ClWindowFinder::GetCurrentView_2()->SendMessage(WM_AddPattern2CurrentTask, (WPARAM)m_pSelectedPatterns.get(), (LPARAM)m_pSelectedTextStructs.get());

	// 清空当前选择集。
	m_pSelectedPatterns->clear();
}

END_CUTLEADER_NAMESPACE()

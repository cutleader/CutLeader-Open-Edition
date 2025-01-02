#include "StdAfx.h"
#include "CanvasPatternDeleteAction.h"

#include "GlViewPort.h"
#include "CommandManager.h"
#include "clUILibResource.h"
#include "CursorDrawer.h"
#include "ValueInputManager.h"
#include "ClWindowFinder.h"

#include "CLOptionInfo.h"
#include "DisplayParam.h"

#include "IArrayPattern.h"
#include "PatternList.h"
#include "PatternDrawer.h"
#include "clGeometryFeatureResource.h"
#include "PatternSelectAction.h"
#include "CanvasPatternDeleteCommand.h"
#include "PartPlacement.h"
#include "Part.h"
#include "clCanvasGeometryUIResource.h"
#include "clPartLayoutResource.h"
#include "TextStruct.h"
#include "TextStructList.h"
#include "GeometryFeatureHelper.h"

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

CanvasPatternDeleteAction::CanvasPatternDeleteAction(PartPlacementPtr pPartPlacement, PatternListPtr pSelectedPatterns, TextStructListPtr pSelectedTextStructs,
	SelectPatternOrLoopActionBar* pSelectPatternOrLoopActionBar, GlViewPortPtr pViewPort)
	: CanvasPatternPreSelectAction(pPartPlacement, pSelectPatternOrLoopActionBar, pViewPort)
{
	m_pSelectedPatterns = pSelectedPatterns;
	m_loopsOfSelectedPatterns = GeometryFeatureHelper::FindPatternLoopsOfPatterns(m_pPartPlacement->GetPart()->GetCadData().get(), m_pSelectedPatterns.get());
	m_pSelectedTextStructs = pSelectedTextStructs;
	m_iMsgID = IDS_OUTPUT_SelectCanvasGeometry2Delete;
	m_pSelectPatternOrLoopActionBar->DisplayActionBar(SelectLoop);
}

CanvasPatternDeleteAction::~CanvasPatternDeleteAction(void)
{
}

BOOL CanvasPatternDeleteAction::LButtonDown(double& dPtX, double& dPtY)
{
	if (m_pSelectedPatterns->size() == 0) {
		CanvasPatternPreSelectAction::LButtonDown(dPtX, dPtY);
	}

	return TRUE;
}

BOOL CanvasPatternDeleteAction::LButtonUp(double& dPtX, double& dPtY)
{
	if (m_pSelectedPatterns->size() == 0) {
		CanvasPatternPreSelectAction::LButtonUp(dPtX, dPtY);
	}

	// 删除选中的几何特征。
	DeleteSelectedPatterns();

	return TRUE;
}

BOOL CanvasPatternDeleteAction::RButtonUp(double& dPtX, double& dPtY)
{
	return TRUE;
}

BOOL CanvasPatternDeleteAction::MovePoint(double& dPtX, double& dPtY)
{
	if (m_pSelectedPatterns->size() == 0) {
		CanvasPatternPreSelectAction::MovePoint(dPtX, dPtY);
	}

	return TRUE;
}

BOOL CanvasPatternDeleteAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (m_pSelectedPatterns->size() == 0) {
		CanvasPatternPreSelectAction::OnKeyDown(nChar, nRepCnt, nFlags);
	}
	else {
		CanvasGeometryEditAction::OnKeyDown(nChar, nRepCnt, nFlags);
	}

	return TRUE;
}

BOOL CanvasPatternDeleteAction::DrawBeforeFinish()
{
	if (m_pSelectedPatterns->size() == 0) {
		CanvasPatternPreSelectAction::DrawBeforeFinish();
	}
	else {
		CursorDrawer::DrawAimSelCursor(m_pViewPort);
	}

	return TRUE;
}

void CanvasPatternDeleteAction::UpdateForUndoRedo()
{
}

SnapInputPtr CanvasPatternDeleteAction::GetSnapInput() const
{
	SnapInputPtr pSnapInput = SnapInput::BuildSnapInput4SnapNone();
	return pSnapInput;
}

void CanvasPatternDeleteAction::DeleteSelectedPatterns()
{
	if (m_pSelectedPatterns->size() > 0)
	{
		// 准备要删除的几何特征。
		PatternListPtr pDeletingPatterns(new PatternList());
		for (unsigned int i = 0; i < m_pSelectedPatterns->size(); i++)
		{
			IPatternPtr pPattern = m_pSelectedPatterns->at(i);
			pDeletingPatterns->push_back(pPattern);
			if (pPattern->IsArrayPattern()) // when delete the grid pattern, the control pattern will also be deleted.
			{
				IArrayPatternPtr pArrayPattern = boost::dynamic_pointer_cast<IArrayPattern>(pPattern);
				IPatternPtr pCtrlPat = pArrayPattern->GetControlPattern();
				pDeletingPatterns->push_back(pCtrlPat);
			}
		}

		// 要删除的文字。
		vector<LONGLONG> textStructs_willDelete;
		for (unsigned int i = 0; i < m_pSelectedTextStructs->size(); i++)
			textStructs_willDelete.push_back(m_pSelectedTextStructs->operator[](i)->GetID());

		// 执行命令。
		PartCadDataPtr pPartCadData = m_pPartPlacement->GetPart()->GetCadData();
		PartCamDataPtr pPartCamData = m_pPartPlacement->GetCamData();
		ICommandPtr pCommand(new CanvasPatternDeleteCommand(GetEditorData(), pPartCadData, pPartCamData, pDeletingPatterns, textStructs_willDelete));
		pCommand->Do();
		CommandManager::GetInstance()->AddCommand(pCommand);

		// 重置数据。
		m_pActivePatterns->clear();
		m_pSelectedPatterns->clear();
		m_pActiveTextStructs->clear();
		m_pSelectedTextStructs->clear();

		// 做一些更新。
		ClWindowFinder::GetCurrentView_1()->SendMessage(WM_updateSheetPreviewData, 0, 0);
		ClWindowFinder::GetCurrentView_2()->SendMessage(WM_redrawSheetThumbnailOnSheetManagementPanel, 0, 0);
	}
}

END_CUTLEADER_NAMESPACE()

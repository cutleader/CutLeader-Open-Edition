#include "StdAfx.h"
#include "CanvasLoopAlignAction.h"

#include "GlViewPort.h"
#include "CommandManager.h"
#include "clUILibResource.h"
#include "CursorDrawer.h"
#include "ValueInputManager.h"
#include "ClWindowFinder.h"
#include "LogMgr.h"
#include "clBaseDataResource.h"

#include "CLOptionInfo.h"
#include "DisplayParam.h"

#include "IArrayPattern.h"
#include "PatternList.h"
#include "PatternDrawer.h"
#include "clGeometryFeatureResource.h"
#include "PatternSelectAction.h"
#include "CanvasLoopAlignCommand.h"
#include "PartPlacement.h"
#include "Part.h"
#include "clCanvasGeometryUIResource.h"
#include "clPartLayoutResource.h"
#include "TextStruct.h"
#include "TextStructList.h"
#include "GeometryFeatureHelper.h"
#include "PatternLoopList.h"
#include "CanvasGeometryHelper.h"

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

CanvasLoopAlignAction::CanvasLoopAlignAction(PartPlacementPtr pPartPlacement, AlignLoopStyle emAlignLoopStyle,
	SelectPatternOrLoopActionBar* pSelectPatternOrLoopActionBar, GlViewPortPtr pViewPort)
	: CanvasPatternPreSelectAction(pPartPlacement, pSelectPatternOrLoopActionBar, pViewPort)
{
	m_emAlignLoopStyle = emAlignLoopStyle;
	m_iMsgID = IDS_OUTPUT_SelectMultiLoop2Align;
	m_pSelectPatternOrLoopActionBar->DisplayActionBar(SelectLoop);
}

CanvasLoopAlignAction::~CanvasLoopAlignAction(void)
{
}

BOOL CanvasLoopAlignAction::LButtonDown(double& dPtX, double& dPtY)
{
	if (m_pSelectedPatterns->size() == 0) {
		CanvasPatternPreSelectAction::LButtonDown(dPtX, dPtY);
	}

	return TRUE;
}

BOOL CanvasLoopAlignAction::LButtonUp(double& dPtX, double& dPtY)
{
	if (m_pSelectedPatterns->size() == 0) {
		CanvasPatternPreSelectAction::LButtonUp(dPtX, dPtY);
	}

	// 如果没有选择几何特征，需要报警。
	if (m_pSelectedPatterns->size() == 0) {
		CString str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_OUTPUT_SelectMultiLoop2Align);
		MessageBox(m_pViewPort->GetWnd(), str, MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME), MB_OK | MB_ICONWARNING);
		return FALSE;
	}

	// 对齐选中的几何轮廓。
	const EditorData& editorData = GetEditorData();
	PatternLoopListPtr pPatternLoops = GeometryFeatureHelper::FindAffectedPatternLoops(m_pPartPlacement->GetPart()->GetCadData().get(), m_pSelectedPatterns.get());
	if (CanvasGeometryHelper::AlignCanvasLoop(editorData.GetView(), editorData.GetEditorName(), pPatternLoops.get(), m_pSelectedTextStructs.get(), m_emAlignLoopStyle, m_pPartPlacement)) {
		ClWindowFinder::GetCurrentView_1()->SendMessage(WM_updateSheetPreviewData, 0, 0);
		ClWindowFinder::GetCurrentView_2()->SendMessage(WM_redrawSheetThumbnailOnSheetManagementPanel, 0, 0);
		PostMessage2EnterCanvasLoopSelectAction_PreSelectLoopAndText(pPatternLoops.get(), m_pSelectedTextStructs.get());
	}

	return TRUE;
}

BOOL CanvasLoopAlignAction::MovePoint(double& dPtX, double& dPtY)
{
	if (m_pSelectedPatterns->size() == 0) {
		CanvasPatternPreSelectAction::MovePoint(dPtX, dPtY);
	}

	return TRUE;
}

BOOL CanvasLoopAlignAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (m_pSelectedPatterns->size() == 0) {
		CanvasPatternPreSelectAction::OnKeyDown(nChar, nRepCnt, nFlags);
	}
	else {
		CanvasGeometryEditAction::OnKeyDown(nChar, nRepCnt, nFlags);
	}

	return TRUE;
}

BOOL CanvasLoopAlignAction::DrawBeforeFinish()
{
	if (m_pSelectedPatterns->size() == 0) {
		CanvasPatternPreSelectAction::DrawBeforeFinish();
	}
	else {
		CursorDrawer::DrawAimSelCursor(m_pViewPort);
	}

	return TRUE;
}

void CanvasLoopAlignAction::UpdateForUndoRedo()
{
}

SnapInputPtr CanvasLoopAlignAction::GetSnapInput() const
{
	SnapInputPtr pSnapInput = SnapInput::BuildSnapInput4SnapNone();
	return pSnapInput;
}

END_CUTLEADER_NAMESPACE()

#include "StdAfx.h"
#include "PatternLoopAlignAction.h"

#include "GlViewPort.h"
#include "CommandManager.h"
#include "clUILibResource.h"
#include "CursorDrawer.h"
#include "ValueInputManager.h"
#include "LogMgr.h"
#include "clBaseDataResource.h"

#include "CLOptionInfo.h"
#include "DisplayParam.h"

#include "IArrayPattern.h"
#include "PatternList.h"
#include "PatternDrawer.h"
#include "clGeometryFeatureResource.h"
#include "PatternSelectAction.h"
#include "PatternDeleteCommand.h"
#include "PartCadData.h"
#include "TextStructList.h"
#include "TextStruct.h"
#include "GeometryFeatureHelper.h"
#include "PatternLoopList.h"
#include "PatternLoopAlignCommand.h"

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

PatternLoopAlignAction::PatternLoopAlignAction(PartCadDataPtr pPartCadData, GlViewPortPtr pViewPort, PatternListPtr pSelectedPatterns,
	TextStructListPtr pSelectedTextStructs, AlignLoopStyle emAlignLoopStyle)
{
	m_pPartCadData = pPartCadData;
	m_pViewPort = pViewPort;
	m_pSelectedPats = pSelectedPatterns;
	m_pSelectedTextStructs = pSelectedTextStructs;
	m_emAlignLoopStyle = emAlignLoopStyle;
}

PatternLoopAlignAction::~PatternLoopAlignAction(void)
{
}

BOOL PatternLoopAlignAction::LButtonDown(double& dPtX, double& dPtY)
{
	if (m_pSelectedPats->size() == 0) {
		PatternPreSelectAction::LButtonDown(dPtX, dPtY);
	}

	return TRUE;
}

BOOL PatternLoopAlignAction::LButtonUp(double& dPtX, double& dPtY)
{
	if (m_pSelectedPats->size() == 0) {
		PatternPreSelectAction::LButtonUp(dPtX, dPtY);
	}

	// 如果没有选择几何特征，需要报警。
	if (m_pSelectedPats->size() == 0) {
		CString str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_OUTPUT_SelectMultiLoop2Align);
		MessageBox(m_pViewPort->GetWnd(), str, MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME), MB_OK | MB_ICONWARNING);
		return FALSE;
	}

	// 对齐选中的几何轮廓。
	if (AlignLoop()) {
		PostMessage2EnterPatternSelectAction(m_pSelectedPats.get(), m_pSelectedTextStructs.get());
	}

	return TRUE;
}

BOOL PatternLoopAlignAction::MovePoint(double& dPtX, double& dPtY)
{
	if (m_pSelectedPats->size() == 0) {
		PatternPreSelectAction::MovePoint(dPtX, dPtY);
	}

	return TRUE;
}

BOOL PatternLoopAlignAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (m_pSelectedPats->size() == 0) {
		PatternPreSelectAction::OnKeyDown(nChar, nRepCnt, nFlags);
	}
	else {
		PatternEditActionBase::OnKeyDown(nChar, nRepCnt, nFlags);
	}

	return TRUE;
}

BOOL PatternLoopAlignAction::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	return TRUE;
}

BOOL PatternLoopAlignAction::DrawBeforeFinish()
{
	if (m_pSelectedPats->size() == 0) {
		PatternPreSelectAction::DrawBeforeFinish();
	}
	else {
		CursorDrawer::DrawAimSelCursor(m_pViewPort);
	}

	return TRUE;
}

SnapInputPtr PatternLoopAlignAction::GetSnapInput() const
{
	SnapInputPtr pSnapInput = SnapInput::BuildSnapInput4SnapNone();
	return pSnapInput;
}

void PatternLoopAlignAction::UpdateForUndoRedo()
{
}

bool PatternLoopAlignAction::AlignLoop()
{
	if (m_pSelectedPats->size() == 0) {
		LogMgr::DebugWarn(_T("334412"));
		return false;
	}

	// 要对齐的几何轮廓。
	vector<LONGLONG> patternLoops_willAlign;
	{
		// 找出所有涉及到的几何轮廓。
		map<LONGLONG, PatternListPtr> patternsInLoop; // 这个结构记录了要删除的几何特征分别在哪个几何轮廓中。
		PatternLoopListPtr pPatternLoops_willAlign = GeometryFeatureHelper::FindAffectedPatternLoops(m_pPartCadData.get(), m_pSelectedPats.get(), patternsInLoop);
		if (pPatternLoops_willAlign->size() < 2) { // 选中的轮廓数量小于2个，需提示用户重新选择。
			CString str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_OUTPUT_SelectMultiLoop2Align);
			MessageBox(m_pViewPort->GetWnd(), str, MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME), MB_OK | MB_ICONWARNING);
			m_pSelectedPats->clear();
			m_pSelectedTextStructs->clear();
			return false;
		}
		for (unsigned int i = 0; i < pPatternLoops_willAlign->size(); i++) {
			patternLoops_willAlign.push_back(pPatternLoops_willAlign->operator[](i)->GetID());
		}
	}

	// 要对齐的文字。
	vector<LONGLONG> textStructs_willAlign;
	for (unsigned int i = 0; i < m_pSelectedTextStructs->size(); i++) {
		textStructs_willAlign.push_back(m_pSelectedTextStructs->operator[](i)->GetID());
	}

	// 执行命令。
	ICommandPtr pCommand(new PatternLoopAlignCommand(GetEditorData(), m_pPartCadData, patternLoops_willAlign, textStructs_willAlign, m_emAlignLoopStyle));
	pCommand->Do();
	CommandManager::GetInstance()->AddCommand(pCommand);

	return true;
}

END_CUTLEADER_NAMESPACE()

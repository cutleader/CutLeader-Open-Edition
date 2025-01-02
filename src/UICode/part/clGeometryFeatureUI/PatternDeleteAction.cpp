#include "StdAfx.h"
#include "PatternDeleteAction.h"

#include "GlViewPort.h"
#include "CommandManager.h"
#include "clUILibResource.h"
#include "CursorDrawer.h"
#include "ValueInputManager.h"

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

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

PatternDeleteAction::PatternDeleteAction(PartCadDataPtr pPartCadData, GlViewPortPtr pViewPort, PatternListPtr pSelectedPatterns, TextStructListPtr pSelectedTextStructs)
{
	m_pPartCadData = pPartCadData;
	m_pViewPort = pViewPort;
	m_pSelectedPats = pSelectedPatterns;
	m_pSelectedTextStructs = pSelectedTextStructs;
}

PatternDeleteAction::~PatternDeleteAction(void)
{
}

BOOL PatternDeleteAction::LButtonDown(double& dPtX, double& dPtY)
{
	if (m_pSelectedPats->size() == 0) {
		PatternPreSelectAction::LButtonDown(dPtX, dPtY);
	}

	return TRUE;
}

BOOL PatternDeleteAction::LButtonUp(double& dPtX, double& dPtY)
{
	if (m_pSelectedPats->size() == 0) {
		PatternPreSelectAction::LButtonUp(dPtX, dPtY);
	}

	// delete the selected patterns.
	DeleteSelectedPats();

	return TRUE;
}

BOOL PatternDeleteAction::RButtonUp(double& dPtX, double& dPtY)
{
	return TRUE;
}

BOOL PatternDeleteAction::MovePoint(double& dPtX, double& dPtY)
{
	if (m_pSelectedPats->size() == 0) {
		PatternPreSelectAction::MovePoint(dPtX, dPtY);
	}

	return TRUE;
}

BOOL PatternDeleteAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (m_pSelectedPats->size() == 0) {
		PatternPreSelectAction::OnKeyDown(nChar, nRepCnt, nFlags);
	}
	else {
		PatternEditActionBase::OnKeyDown(nChar, nRepCnt, nFlags);
	}

	return TRUE;
}

BOOL PatternDeleteAction::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	return TRUE;
}

BOOL PatternDeleteAction::DrawBeforeFinish()
{
	if (m_pSelectedPats->size() == 0) {
		PatternPreSelectAction::DrawBeforeFinish();
	}
	else {
		CursorDrawer::DrawAimSelCursor(m_pViewPort);
	}

	return TRUE;
}

SnapInputPtr PatternDeleteAction::GetSnapInput() const
{
	SnapInputPtr pSnapInput = SnapInput::BuildSnapInput4SnapNone();
	return pSnapInput;
}

void PatternDeleteAction::UpdateForUndoRedo()
{
	PatternListPtr pPatList = m_pPartCadData->GetPatternList();
	for (unsigned int i = 0; i < m_pSelectedPats->size(); i++)
	{
		IPatternPtr pPattern = m_pSelectedPats->at(i);
		if (!pPatList->GetPatternFromID(pPattern->GetID()))
		{
			// reset the action status.
			m_pSelectedPats->clear();
			m_pActivePats->clear();

			break;
		}
	}
}

void PatternDeleteAction::DeleteSelectedPats()
{
	if (m_pSelectedPats->size() > 0)
	{
		// when delete the grid pattern, the control pattern will also be deleted.
		PatternListPtr pSelPatList(new PatternList());
		for (unsigned int i = 0; i < m_pSelectedPats->size(); i++)
		{
			IPatternPtr pPattern = m_pSelectedPats->at(i);
			pSelPatList->push_back(pPattern);
			if (pPattern->IsArrayPattern()) { // 该几何特征是阵列类型的。
				IArrayPatternPtr pArrayPattern = boost::dynamic_pointer_cast<IArrayPattern>(pPattern);
				pSelPatList->push_back(pArrayPattern->GetControlPattern());
			}
		}

		// 要删除的文字。
		vector<LONGLONG> textStructs_willDelete;
		for (unsigned int i = 0; i < m_pSelectedTextStructs->size(); i++) {
			textStructs_willDelete.push_back(m_pSelectedTextStructs->operator[](i)->GetID());
		}

		// 执行命令。
		ICommandPtr pCommand(new PatternDeleteCommand(GetEditorData(), m_pPartCadData, pSelPatList, textStructs_willDelete));
		pCommand->Do();
		CommandManager::GetInstance()->AddCommand(pCommand);

		// 重置数据。
		m_pActivePats->clear();
		m_pSelectedPats->clear();
		m_pActiveTextStructs->clear();
		m_pSelectedTextStructs->clear();
	}
}

END_CUTLEADER_NAMESPACE()

#include "StdAfx.h"
#include "CornerDeleteAction.h"

#include "ValueInputManager.h"
#include "CommandManager.h"
#include "Point2DList.h"
#include "CursorDrawer.h"

#include "IPatternLoop.h"
#include "PatternLoopList.h"

#include "LoopCutFeatureList.h"
#include "LoopCutFeature.h"
#include "LoopCornerDataList.h"
#include "LoopCornerData.h"
#include "CornerList.h"
#include "CornerDeleteCommand.h"
#include "clCutFeatureResource.h"

#include "CornerConfigItem.h"

BEGIN_CUTLEADER_NAMESPACE()

CornerDeleteAction::CornerDeleteAction(PatternListPtr pPatList, PatternLoopListPtr pPatternLoopList, LoopTopologyItemListPtr pLoopTopologyItems, 
	LoopCutFeatureListPtr pLoopFeatureList, GlViewPortPtr pViewPort)
    : CornerPreSelectAction(pPatList, pPatternLoopList, pLoopTopologyItems, pLoopFeatureList, pViewPort)
{
    m_iMsgID = IDS_CUTFEATURE_DELETE_CORNER;
}

CornerDeleteAction::~CornerDeleteAction(void)
{
}

BOOL CornerDeleteAction::LButtonDown(double& dPtX, double& dPtY)
{
	CornerPreSelectAction::LButtonDown(dPtX, dPtY);

	return TRUE;
}

BOOL CornerDeleteAction::LButtonUp(double& dPtX, double& dPtY)
{
	CornerPreSelectAction::LButtonUp(dPtX, dPtY);

	// 删除选中的“角特征”。
	if (m_selCornerPtPairs.size() > 0)
	{
		CornerListPtr pCornerList(new CornerList);
		for (unsigned int i = 0; i < m_selCornerPtPairs.size(); i++)
			pCornerList->push_back(m_selCornerPtPairs.at(i).first);
		ICommandPtr pCommand(new CornerDeleteCommand(GetEditorData(), m_pLoopFeatureList->GetLoopCornerList(), pCornerList));
		pCommand->Do();
		CommandManager::GetInstance()->AddCommand(pCommand);

		// 清除选中的“角特征”。
		m_pActiveCornerPt.reset();
		m_selCornerPtPairs.clear();
	}

	return TRUE;
}

BOOL CornerDeleteAction::MovePoint(double& dPtX, double& dPtY)
{
	CornerPreSelectAction::MovePoint(dPtX, dPtY);

	return TRUE;
}

BOOL CornerDeleteAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CornerPreSelectAction::OnKeyDown(nChar, nRepCnt, nFlags);

	// 删除选中的“角特征”。
	if (ValueInputManager::GetInstance()->IsInputCtrl() && ValueInputManager::GetInstance()->IsInputA(nChar))
	{
		if (m_selCornerPtPairs.size() > 0)
		{
			CornerListPtr pCornerList(new CornerList);
			for (unsigned int i = 0; i < m_selCornerPtPairs.size(); i++)
				pCornerList->push_back(m_selCornerPtPairs.at(i).first);
			ICommandPtr pCommand(new CornerDeleteCommand(GetEditorData(), m_pLoopFeatureList->GetLoopCornerList(), pCornerList));
			pCommand->Do();
			CommandManager::GetInstance()->AddCommand(pCommand);

			// 清除选中的“角特征”。
			m_selCornerPtPairs.clear();
		}
	}

	return TRUE;
}

BOOL CornerDeleteAction::DrawBeforeFinish()
{
	CornerPreSelectAction::DrawBeforeFinish();

	return TRUE;
}

void CornerDeleteAction::SelectCornerList(CornerListPtr pCornerList)
{
	ICommandPtr pCommand(new CornerDeleteCommand(GetEditorData(), m_pLoopFeatureList->GetLoopCornerList(), pCornerList));
	pCommand->Do();
	CommandManager::GetInstance()->AddCommand(pCommand);
}

END_CUTLEADER_NAMESPACE()

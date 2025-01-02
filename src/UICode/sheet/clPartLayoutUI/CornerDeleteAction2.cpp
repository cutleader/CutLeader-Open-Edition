#include "StdAfx.h"
#include "CornerDeleteAction2.h"

#include "GlViewPort.h"
#include "ValueInputManager.h"
#include "CommandManager.h"
#include "Point2DList.h"
#include "DummyAction.h"
#include "CursorDrawer.h"

#include "CornerConfigItem.h"
#include "IMaterialSize.h"

#include "IPatternLoop.h"
#include "PatternLoopList.h"
#include "PatternList.h"
#include "LoopCutFeatureList.h"
#include "LoopCutFeature.h"
#include "LoopCornerDataList.h"
#include "ICorner.h"
#include "LoopCornerData.h"
#include "CornerList.h"
#include "CornerDeleteCommand.h"
#include "clCutFeatureResource.h"
#include "LoopInstanceList.h"
#include "PartInstanceList.h"
#include "PartPlacementList.h"
#include "LoopInstanceManager.h"

BEGIN_CUTLEADER_NAMESPACE()

CornerDeleteAction2::CornerDeleteAction2(IMaterialSizePtr pMaterialSize, PartPlacementListPtr pPartPlacements, PartInstanceListPtr pPartInstances,
	PartGroupListPtr pPartGroups, GlViewPortPtr pViewPort, SpreadSamePartActionBar* pSpreadSamePartActionBar)
    : CornerPreSelectAction2(pMaterialSize, pPartPlacements, pPartInstances, pPartGroups, pViewPort)
{
	m_pSpreadSamePartActionBar = pSpreadSamePartActionBar;
	m_pAllLoopInstanceList = LoopInstanceManager::GetLoopInstanceList(pPartInstances);
	m_iMsgID = IDS_CUTFEATURE_DELETE_CORNER;

	if (m_pMatSize->GetType() != MAT_SIZE_Canvas)
		pSpreadSamePartActionBar->Show_SpreadSamePartActionBar();
}

CornerDeleteAction2::~CornerDeleteAction2(void)
{
}

BOOL CornerDeleteAction2::LButtonDown(double& dPtX, double& dPtY)
{
	CornerPreSelectAction2::LButtonDown(dPtX, dPtY);

	return TRUE;
}

BOOL CornerDeleteAction2::LButtonUp(double& dPtX, double& dPtY)
{
	CornerPreSelectAction2::LButtonUp(dPtX, dPtY);

	// 删除选中的“角特征”。
	if (m_selCornerPtPairs.size() > 0)
	{
		LoopCornerDataListPtr pLoopCornerList = m_pPartPlacementList->GetAllLoopCornerList();

		// 选中的角特征
		CornerListPtr pCornerList_selected(new CornerList);
		for (unsigned int i = 0; i < m_selCornerPtPairs.size(); i++)
		{
			pCornerList_selected->push_back(m_selCornerPtPairs.at(i).first);
		}

		if (m_pSpreadSamePartActionBar->IsSpread())
		{
			CornerListPtr pCornerList_involveSameParts(new CornerList);
			for (unsigned int i = 0; i < pCornerList_selected->size(); i++)
			{
				ICornerPtr pCorner = pCornerList_selected->at(i);
				for (unsigned int j = 0; j < pLoopCornerList->size(); j++)
				{
					LoopCornerDataPtr pLoopCorner = pLoopCornerList->at(j);
					ICornerPtr pCorner_involveSameParts = pLoopCorner->GetCornerByPat(pCorner->GetFirstPatternID(), pCorner->GetSecondPatternID());
					if (pCorner_involveSameParts)
					{
						pCornerList_involveSameParts->push_back(pCorner_involveSameParts);
					}
				}
			}

			ICommandPtr pCommand(new CornerDeleteCommand(GetEditorData(), pLoopCornerList, pCornerList_involveSameParts));
			pCommand->Do();
			CommandManager::GetInstance()->AddCommand(pCommand);
		}
		else
		{
			ICommandPtr pCommand(new CornerDeleteCommand(GetEditorData(), pLoopCornerList, pCornerList_selected));
			pCommand->Do();
			CommandManager::GetInstance()->AddCommand(pCommand);
		}

		// 清除选中的“角特征”。
		m_pActiveCornerPt.reset();
		m_selCornerPtPairs.clear();
	}

	return TRUE;
}

BOOL CornerDeleteAction2::MovePoint(double& dPtX, double& dPtY)
{
	CornerPreSelectAction2::MovePoint(dPtX, dPtY);

	return TRUE;
}

BOOL CornerDeleteAction2::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CornerPreSelectAction2::OnKeyDown(nChar, nRepCnt, nFlags);

	// 删除选中的“角特征”。
	if (ValueInputManager::GetInstance()->IsInputCtrl() && ValueInputManager::GetInstance()->IsInputA(nChar))
	{
		if (m_selCornerPtPairs.size() > 0)
		{
			LoopCornerDataListPtr pLoopCornerList = m_pPartPlacementList->GetAllLoopCornerList();
			CornerListPtr pCornerList(new CornerList);
			for (unsigned int i = 0; i < m_selCornerPtPairs.size(); i++)
				pCornerList->push_back(m_selCornerPtPairs.at(i).first);
			ICommandPtr pCommand(new CornerDeleteCommand(GetEditorData(), pLoopCornerList, pCornerList));
			pCommand->Do();
			CommandManager::GetInstance()->AddCommand(pCommand);

			// 清除选中的“角特征”。
			m_selCornerPtPairs.clear();
		}
	}

	return TRUE;
}

BOOL CornerDeleteAction2::DrawBeforeFinish()
{
	CornerPreSelectAction2::DrawBeforeFinish();

	return TRUE;
}

void CornerDeleteAction2::SelectCornerList(CornerListPtr pCornerList)
{
	LoopCornerDataListPtr pLoopCornerList = m_pPartPlacementList->GetAllLoopCornerList();
	ICommandPtr pCommand(new CornerDeleteCommand(GetEditorData(), pLoopCornerList, pCornerList));
	pCommand->Do();
	CommandManager::GetInstance()->AddCommand(pCommand);
}

END_CUTLEADER_NAMESPACE()

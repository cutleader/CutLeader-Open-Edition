#include "StdAfx.h"
#include "CornerEditAction2.h"

#include "CommandManager.h"
#include "GlViewPort.h"
#include "DummyAction.h"
#include "Point2DList.h"
#include "ValueInputManager.h"
#include "CursorDrawer.h"

#include "CLOptionInfo.h"
#include "DisplayParam.h"
#include "IMaterialSize.h"

#include "PatternLoopList.h"
#include "PatternList.h"
#include "PolygonPatternLoop.h"
#include "RadiusCorner.h"
#include "CornerModifyCommand.h"
#include "CornerDeleteCommand.h"
#include "LoopCutFeatureList.h"
#include "LoopCutFeature.h"
#include "LoopCornerDataList.h"
#include "LoopCornerData.h"
#include "CornerList.h"
#include "CutFeatureEditor.h"
#include "CornerPropertyBar.h"
#include "LoopCutFeatureDrawer.h"
#include "LoopInstanceList.h"
#include "PartInstanceList.h"
#include "PartPlacementList.h"
#include "LoopInstanceManager.h"

BEGIN_CUTLEADER_NAMESPACE()

CornerEditAction2::CornerEditAction2(IMaterialSizePtr pMaterialSize, PartPlacementListPtr pPartPlacements, PartInstanceListPtr pPartInstances,
	PartGroupListPtr pPartGroups, GlViewPortPtr pViewPort, SpreadSamePartActionBar* pSpreadSamePartActionBar, CornerConfigItemPtr pCornerConfigItem)
    : CornerPreSelectAction2(pMaterialSize, pPartPlacements, pPartInstances, pPartGroups, pViewPort)
{
	m_pSpreadSamePartActionBar = pSpreadSamePartActionBar;
	m_pSpreadSamePartActionBar->AddObserver(this);
	m_pAllLoopInstanceList = LoopInstanceManager::GetLoopInstanceList(pPartInstances);
    m_pCornerConfigItem = pCornerConfigItem;

	if (m_pMatSize->GetType() != MAT_SIZE_Canvas)
		pSpreadSamePartActionBar->Show_SpreadSamePartActionBar();
}

CornerEditAction2::~CornerEditAction2(void)
{
	m_pSpreadSamePartActionBar->RemoveObserver(GetID());
}

BOOL CornerEditAction2::LButtonDown(double& dPtX, double& dPtY)
{
	CornerPreSelectAction2::LButtonDown(dPtX, dPtY);

	return TRUE;
}

BOOL CornerEditAction2::LButtonUp(double& dPtX, double& dPtY)
{
	CornerPreSelectAction2::LButtonUp(dPtX, dPtY);

	DisplayCornerData();

	return TRUE;
}

BOOL CornerEditAction2::MovePoint(double& dPtX, double& dPtY)
{
	CornerPreSelectAction2::MovePoint(dPtX, dPtY);

	return TRUE;
}

BOOL CornerEditAction2::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CornerPreSelectAction2::OnKeyDown(nChar, nRepCnt, nFlags);

	if (ValueInputManager::GetInstance()->IsInputCancel(nChar))
	{
		CornerPropertyBar::GetInstance()->ShowPane(FALSE, FALSE, TRUE);
	}
	else if (ValueInputManager::GetInstance()->IsInputCtrl() && ValueInputManager::GetInstance()->IsInputA(nChar))
	{
		// 显示选中的“角特征”属性。
		if (m_selCornerPtPairs.size() > 0)
		{
			LoopCornerDataListPtr pLoopCornerList = m_pPartPlacementList->GetAllLoopCornerList();
			CornerListPtr pCornerList(new CornerList);
			for (unsigned int i = 0; i < m_selCornerPtPairs.size(); i++)
				pCornerList->push_back(m_selCornerPtPairs.at(i).first);
			CornerPropertyBar::GetInstance()->DisplayCornerData_4_edit(GetEditorData(), pLoopCornerList, pCornerList, m_pCornerConfigItem);
		}
	}
	else if (ValueInputManager::GetInstance()->IsInputDel(nChar))
	{
		// 删除选中的“角特征”。
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

			// 隐藏角特征属性界面。
			CornerPropertyBar::GetInstance()->ShowBar(FALSE);
		}
	}

	return TRUE;
}

BOOL CornerEditAction2::DrawBeforeFinish()
{
	CornerPreSelectAction2::DrawBeforeFinish();

	return TRUE;
}

void CornerEditAction2::UpdateForUndoRedo()
{
	// 不显示“当前的”和“选中的”角特征点。
	m_pActiveCornerPt.reset();
	m_selCornerPtPairs.clear();

	// 隐藏角特征属性界面。
	CornerPropertyBar::GetInstance()->ShowBar(FALSE);
}

void CornerEditAction2::Update(int iEventID, const CComVariant& varEventContent)
{
	// SpreadSamePartActionBar有变化，这里需要重新显示一遍
	DisplayCornerData();
}

void CornerEditAction2::DisplayCornerData()
{
	// 显示角特征编辑界面。
	if (m_selCornerPtPairs.size() > 0)
	{
		LoopCornerDataListPtr pLoopCornerList = m_pPartPlacementList->GetAllLoopCornerList();
		CornerListPtr pCornerList(new CornerList);
		for (unsigned int i = 0; i < m_selCornerPtPairs.size(); i++)
		{
			pCornerList->push_back(m_selCornerPtPairs.at(i).first);
		}
		if (m_pSpreadSamePartActionBar->IsSpread())
		{
			CornerListPtr pCornerList_involveSameParts(new CornerList);
			for (unsigned int i = 0; i < pCornerList->size(); i++)
			{
				ICornerPtr pCorner = pCornerList->at(i);
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
			CornerPropertyBar::GetInstance()->DisplayCornerData_4_edit(GetEditorData(), pLoopCornerList, pCornerList_involveSameParts, m_pCornerConfigItem);
		}
		else
		{
			CornerPropertyBar::GetInstance()->DisplayCornerData_4_edit(GetEditorData(), pLoopCornerList, pCornerList, m_pCornerConfigItem);
		}
	}
}

END_CUTLEADER_NAMESPACE()

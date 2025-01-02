#include "StdAfx.h"
#include "LoopLeadEditAction2.h"

#include "baseConst.h"
#include "MathEx.h"
#include "ValueInputManager.h"
#include "DummyAction.h"
#include "GlViewPort.h"
#include "CommandManager.h"
#include "Point2DList.h"
#include "LineArc2DList.h"
#include "CursorDrawer.h"
#include "clBaseDataResource.h"
#include "LogMgr.h"

#include "CLOptionInfo.h"
#include "DisplayParam.h"
#include "IMaterialSize.h"

#include "IArrayPattern.h"
#include "PatternLoopNode.h"
#include "PatternLoopNodeList.h"
#include "IPatternLoop.h"
#include "LoopProcessor.h"
#include "LoopStartCutData.h"
#include "LoopStartCutDataList.h"
#include "LoopLeadPropertyBar.h"
#include "LoopCutFeatureDrawer.h"
#include "clCutFeatureResource.h"
#include "LoopInstanceCache.h"
#include "LoopInstance.h"
#include "LoopInstanceList.h"
#include "PartInstanceList.h"
#include "LoopInstanceManager.h"

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

LoopLeadEditAction2::LoopLeadEditAction2(IMaterialSizePtr pMaterialSize, PartPlacementListPtr pPartPlacements, PartInstanceListPtr pPartInstances, PartGroupListPtr pPartGroups,
	GlViewPortPtr pViewPort, LeadConfigItemPtr pLeadConfigItem, ConditionTypeListPtr pCondTypeList, SpreadSamePartActionBar* pSpreadSamePartActionBar)
    : StartCutPtPreSelectAction2(pMaterialSize, pPartPlacements, pPartInstances, pPartGroups, pViewPort)
{
	m_pSpreadSamePartActionBar = pSpreadSamePartActionBar;
	m_pSpreadSamePartActionBar->AddObserver(this);
	m_pLeadConfigItem = pLeadConfigItem;
	m_pCondTypeList = pCondTypeList;
	m_pAllLoopInstanceList = LoopInstanceManager::GetLoopInstanceList(pPartInstances);

	if (m_pMatSize->GetType() != MAT_SIZE_Canvas)
		pSpreadSamePartActionBar->Show_SpreadSamePartActionBar();
}

LoopLeadEditAction2::~LoopLeadEditAction2(void)
{
	m_pSpreadSamePartActionBar->RemoveObserver(GetID());
}

BOOL LoopLeadEditAction2::LButtonDown(double& dPtX, double& dPtY)
{
	StartCutPtPreSelectAction2::LButtonDown(dPtX, dPtY);

	return TRUE;
}

BOOL LoopLeadEditAction2::LButtonUp(double& dPtX, double& dPtY)
{
	StartCutPtPreSelectAction2::LButtonUp(dPtX, dPtY);

	DisplayLoopLead();

	return TRUE;
}

BOOL LoopLeadEditAction2::MovePoint(double& dPtX, double& dPtY)
{
	StartCutPtPreSelectAction2::MovePoint(dPtX, dPtY);

	return TRUE;
}

BOOL LoopLeadEditAction2::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (ValueInputManager::GetInstance()->IsInputCtrl() && ValueInputManager::GetInstance()->IsInputA(nChar)) // 选中全部“回路起切”特征。
	{
		// 显示“回路引入引出线和起切点”界面。
		if (m_selStaEndPtPairs.size() > 0)
		{
			LoopStartCutDataListPtr pLoopStaEndCutList(new LoopStartCutDataList);
			vector<Point2D> startPtList;
			for (unsigned int i = 0; i < m_selStaEndPtPairs.size(); i++)
			{
				pLoopStaEndCutList->push_back(m_selStaEndPtPairs.at(i).first);
				startPtList.push_back(m_selStaEndPtPairs.at(i).second);
			}
			LoopLeadPropertyBar::GetInstance()->DisplayLoopLead(GetEditorData(), pLoopStaEndCutList, startPtList, m_pLeadConfigItem);
		}
	}
	else
	{
		__super::OnKeyDown(nChar, nRepCnt, nFlags);
	}

	return TRUE;
}

BOOL LoopLeadEditAction2::DrawBeforeFinish()
{
	StartCutPtPreSelectAction2::DrawBeforeFinish();

	return TRUE;
}

void LoopLeadEditAction2::UpdateForUndoRedo()
{
	LoopLeadPropertyBar::GetInstance()->ShowBar(FALSE);
}

void LoopLeadEditAction2::Update(int iEventID, const CComVariant& varEventContent)
{
	// SpreadSamePartActionBar有变化，这里需要重新显示一遍
	DisplayLoopLead();
}

void LoopLeadEditAction2::EditLeadsOfSelectedLoopInstances(LoopInstanceListPtr pSelectedLoopInstances)
{
    if (!pSelectedLoopInstances)
    {
        LogMgr::DebugWarn(_T("640218"), CWnd::FromHandle(m_pViewPort->GetWnd()));
        return;
    }

    m_selStaEndPtPairs = pSelectedLoopInstances->GetAllStartCutPtPairs();
    DisplayLoopLead();
}

void LoopLeadEditAction2::DisplayLoopLead()
{
	if (m_selStaEndPtPairs.size() > 0)
	{
		// 显示“回路引入引出线和起切点”界面。
		LoopStartCutDataListPtr pLoopStaEndCutList(new LoopStartCutDataList);
		vector<Point2D> startPtList;
		for (unsigned int i = 0; i < m_selStaEndPtPairs.size(); i++)
		{
			pLoopStaEndCutList->push_back(m_selStaEndPtPairs.at(i).first);
			startPtList.push_back(m_selStaEndPtPairs.at(i).second);
		}
		if (m_pSpreadSamePartActionBar->IsSpread())
		{
			LoopStartCutDataListPtr pLoopStaEndCutList_involveSameParts(new LoopStartCutDataList);
			vector<Point2D> startPtList_involveSameParts;
			for (unsigned int i = 0; i < pLoopStaEndCutList->size(); i++)
			{
				IPatternLoopPtr pPatternLoop = pLoopStaEndCutList->at(i)->GetPatternLoop();
				for (unsigned int j = 0; j < m_pAllLoopInstanceList->size(); j++)
				{
					LoopInstancePtr pLoopInstance = m_pAllLoopInstanceList->at(j);
					if (pLoopInstance->GetPatternLoopID() == pPatternLoop->GetID())
					{
						pLoopStaEndCutList_involveSameParts->push_back(pLoopInstance->GetCacheData()->GetLoopStartCutData());
						startPtList_involveSameParts.push_back(pLoopInstance->GetStartCutPt());
					}
				}
			}
			LoopLeadPropertyBar::GetInstance()->DisplayLoopLead(GetEditorData(), pLoopStaEndCutList_involveSameParts, startPtList_involveSameParts, m_pLeadConfigItem);
		}
		else
		{
			LoopLeadPropertyBar::GetInstance()->DisplayLoopLead(GetEditorData(), pLoopStaEndCutList, startPtList, m_pLeadConfigItem);
		}
	}
}

END_CUTLEADER_NAMESPACE()

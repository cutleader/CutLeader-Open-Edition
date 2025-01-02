#include "StdAfx.h"
#include "LoopInstanceSelectAction.h"

#include "GlViewPort.h"
#include "ValueInputManager.h"
#include "CursorDrawer.h"
#include "clBaseDataResource.h"

#include "CLOptionInfo.h"
#include "DisplayParam.h"

#include "LoopDrawer.h"
#include "clPartLayoutResource.h"
#include "LoopToolManager.h"
#include "PartInstance.h"
#include "LoopInstanceManager.h"
#include "LoopInstanceCache.h"
#include "LoopInstance.h"
#include "LoopInstanceList.h"
#include "LoopInstancePropertyBar.h"

BEGIN_CUTLEADER_NAMESPACE()

LoopInstanceSelectAction::LoopInstanceSelectAction(IMaterialSizePtr pMaterialSize, PartPlacementListPtr pPartPlacements, PartInstanceListPtr pPartInstances,
    PartGroupListPtr pPartGroups, GlViewPortPtr pViewPort, SelectPartOrLoopActionBar* pSelectPartOrLoopActionBar)
    : PartPlacementEditAction(pMaterialSize, pPartPlacements, pPartInstances, pPartGroups, pViewPort)
{
	m_iMsgID = IDS_OUTPUT_SelectLoopInstance;

    m_iState = NONE;
    m_pActiveLoopInsts_inRect.reset(new LoopInstanceList);
    m_pSelectedLoopInsts.reset(new LoopInstanceList);

	// 得到所有“回路实例”。
	m_pAllLoopInstances = LoopInstanceManager::GetLoopInstanceList(m_pPartInstanceList);

    m_pSelectPartOrLoopActionBar = pSelectPartOrLoopActionBar;
	m_pSelectPartOrLoopActionBar->Show_SelectPartOrLoopActionBar(false);
}

LoopInstanceSelectAction::~LoopInstanceSelectAction(void)
{
}

BOOL LoopInstanceSelectAction::LButtonDown(double& dPtX, double& dPtY)
{
    if (m_iState == NONE || m_iState == LOOPINST_SELECT_SECOND_POINT)
    {
        m_iState = LOOPINST_SELECT_FIRST_POINT;

        m_boxData[0] = dPtX;
        m_boxData[1] = dPtY;
        m_boxData[2] = dPtX;
        m_boxData[3] = dPtY;
    }

	return TRUE;
}

BOOL LoopInstanceSelectAction::LButtonUp(double& dPtX, double& dPtY)
{
    CString strProductName, strMsg;
    strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);

    if (m_iState == LOOPINST_SELECT_FIRST_POINT)
    {
        m_iState = LOOPINST_SELECT_SECOND_POINT;

        // 得到选中的轮廓
		if (!ValueInputManager::GetInstance()->IsInputCtrl()) {
			m_pSelectedLoopInsts->clear();
		}
        if (m_pActiveLoopInsts_inRect->size() > 0) {
            m_pSelectedLoopInsts->insert(m_pSelectedLoopInsts->end(), m_pActiveLoopInsts_inRect->begin(), m_pActiveLoopInsts_inRect->end());
        }
        else if (m_pActiveLoopInstance) {
            m_pSelectedLoopInsts->push_back(m_pActiveLoopInstance);
        }

        // 看看要不要选中相同轮廓。
        if (m_pSelectPartOrLoopActionBar->GetIsAutoSelectSamePartsOrLoops()) {
            for (unsigned int i = 0; i < m_pSelectedLoopInsts->size(); i++) {
                const LoopInstance* pLoopInstance = m_pSelectedLoopInsts->operator [](i).get();
                LoopInstanceListPtr pLoopInstances_sameLoop = m_pAllLoopInstances->GetLoopInstances_ByPatternLoop(pLoopInstance->GetPatternLoopID());
                for (unsigned int j = 0; j < pLoopInstances_sameLoop->size(); j++) {
                    LoopInstancePtr pLoopInstance_sameLoop = pLoopInstances_sameLoop->operator [](j);
                    if (!m_pSelectedLoopInsts->GetLoopInstance_ByPartInstanceAndPatternLoop(pLoopInstance_sameLoop->GetPartInstance()->GetID(),
                        pLoopInstance_sameLoop->GetPatternLoopID()))
                    {
                        m_pSelectedLoopInsts->push_back(pLoopInstance_sameLoop);
                    }
                }
            }
        }

		if (m_pSelectedLoopInsts->size() > 0) {
			LoopInstancePropertyBar::GetInstance()->DisplayProperty(m_pSelectedLoopInsts.get());
		}
		else {
			LoopInstancePropertyBar::GetInstance()->ShowBar(FALSE);
		}
    }

    return TRUE;
}

BOOL LoopInstanceSelectAction::MovePoint(double& dPtX, double& dPtY)
{
	Point2D pt(dPtX, dPtY);

    if (m_iState == LOOPINST_SELECT_FIRST_POINT)
    {
        m_boxData[2] = dPtX;
        m_boxData[3] = dPtY;

        Rect2D rect;
        rect.SetXMin(m_boxData[0] < m_boxData[2] ? m_boxData[0] : m_boxData[2]);
        rect.SetXMax(m_boxData[0] < m_boxData[2] ? m_boxData[2] : m_boxData[0]);
        rect.SetYMin(m_boxData[1] < m_boxData[3] ? m_boxData[1] : m_boxData[3]);
        rect.SetYMax(m_boxData[1] < m_boxData[3] ? m_boxData[3] : m_boxData[1]);
		m_pActiveLoopInsts_inRect = LoopInstanceManager::GetLoopInstsInRect(m_pAllLoopInstances, rect);
        m_pActiveLoopInstance.reset();
    }
    else
    {
		m_pActiveLoopInstance = LoopInstanceManager::GetClosestLoopInst(m_pAllLoopInstances, pt);
        m_pActiveLoopInsts_inRect->clear();
    }

	return TRUE;
}

BOOL LoopInstanceSelectAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	PartPlacementEditAction::OnKeyDown(nChar, nRepCnt, nFlags);

	return TRUE;
}

BOOL LoopInstanceSelectAction::DrawBeforeFinish()
{
    LoopDrawer loopDrawer(m_pViewPort);

    // 绘制选中的轮廓。
    for (unsigned int i = 0; i < m_pSelectedLoopInsts->size(); i++) {
		LoopInstancePtr pSelectedLoopInst = m_pSelectedLoopInsts->at(i);
        COLORREF iColor = LoopToolManager::GetLoopDisplayColor(pSelectedLoopInst->GetCacheData()->GetLoopTool().get());
        loopDrawer.DrawSelectedLoop(pSelectedLoopInst->GetCacheData()->GetPatternLoop().get(), iColor, pSelectedLoopInst->GetTransformMatrix());
    }

    if (m_iState == LOOPINST_SELECT_FIRST_POINT) // 框选。
    {
        // 绘制框框。
    	DrawSelectionBox(m_boxData[0], m_boxData[1], m_boxData[2], m_boxData[3]);

        // 绘制候选轮廓。
        for (unsigned int i = 0; i < m_pActiveLoopInsts_inRect->size(); i++) {
            LoopInstancePtr pActiveLoopInst = m_pActiveLoopInsts_inRect->operator[](i);
            if (!m_pSelectedLoopInsts->GetLoopInstance_ByPartInstanceAndPatternLoop(pActiveLoopInst->GetPartInstance()->GetID(), pActiveLoopInst->GetPatternLoopID())) {
                COLORREF iColor = LoopToolManager::GetLoopDisplayColor(pActiveLoopInst->GetCacheData()->GetLoopTool().get());
                loopDrawer.DrawActiveLoop(pActiveLoopInst->GetCacheData()->GetPatternLoop().get(), iColor, pActiveLoopInst->GetTransformMatrix());
            }
        }
    }
    else
    {
        // 绘制候选轮廓
        if (m_pActiveLoopInstance) {
			LONGLONG iPartInstanceID = m_pActiveLoopInstance->GetPartInstance()->GetID();
			LONGLONG iPatternLoopID = m_pActiveLoopInstance->GetPatternLoopID();
			if (!m_pSelectedLoopInsts->GetLoopInstance_ByPartInstanceAndPatternLoop(iPartInstanceID, iPatternLoopID)) {
				COLORREF iColor = LoopToolManager::GetLoopDisplayColor(m_pActiveLoopInstance->GetCacheData()->GetLoopTool().get());
				loopDrawer.DrawActiveLoop(m_pActiveLoopInstance->GetCacheData()->GetPatternLoop().get(), iColor, m_pActiveLoopInstance->GetTransformMatrix());
			}
        }
    }

	// draw cursor.
	CursorDrawer::DrawAimSelCursor(m_pViewPort);

	return TRUE;
}

void LoopInstanceSelectAction::SelectLoopInstances(LoopInstanceListPtr pSelectedLoopInsts)
{
	m_pSelectedLoopInsts = pSelectedLoopInsts;
	m_iState = LOOPINST_SELECT_SECOND_POINT;

	// 显示属性面板，注意这里要先显示它，这样后续的action bar才能显示在正确的位置。
	LoopInstancePropertyBar::GetInstance()->DisplayProperty(pSelectedLoopInsts.get());

	// 这里还要显示这个action bar。
	m_pSelectPartOrLoopActionBar->Show_SelectPartOrLoopActionBar(false);
}

END_CUTLEADER_NAMESPACE()

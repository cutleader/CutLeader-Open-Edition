#include "StdAfx.h"
#include "CutSideAdjustAction.h"

#include "GlViewPort.h"
#include "CursorDrawer.h"
#include "clBaseDataResource.h"

#include "IMaterialSize.h"

#include "LoopDrawer.h"
#include "clCutFeatureResource.h"
#include "LoopInstanceCache.h"
#include "LoopInstance.h"
#include "LoopInstanceList.h"
#include "PartInstance.h"
#include "LoopInstanceManager.h"
#include "LoopToolManager.h"
#include "PartLayoutHelper.h"
#include "CanvasGeometryEditAction.h"

using namespace std;
using namespace std::tr1;

BEGIN_CUTLEADER_NAMESPACE()

CutSideAdjustAction::CutSideAdjustAction(IMaterialSizePtr pMaterialSize, PartPlacementListPtr pPartPlacements, PartInstanceListPtr pPartInstances,
    PartGroupListPtr pPartGroups, PatternLoopCutOffsetType emCutOffsetType, GlViewPortPtr pViewPort, LeadConfigItemPtr pLeadConfigItem, SpreadSamePartActionBar* pSpreadSamePartActionBar)
    : PartPlacementEditAction(pMaterialSize, pPartPlacements, pPartInstances, pPartGroups, pViewPort)
{
	m_emState = CutSideAdjust_None;
	m_emCutOffsetType = emCutOffsetType;
    m_pSpreadSamePartActionBar = pSpreadSamePartActionBar;
	m_pLeadConfigItem = pLeadConfigItem;
	m_iMsgID = IDS_CHANGE_CUTSIDE;
	m_pAllLoopInstances = LoopInstanceManager::GetLoopInstanceList(pPartInstances);
	m_pRectSelectLoopInstances.reset(new LoopInstanceList);

	if (m_pMatSize->GetType() != MAT_SIZE_Canvas) {
		pSpreadSamePartActionBar->Show_SpreadSamePartActionBar();
	}
}

CutSideAdjustAction::~CutSideAdjustAction(void)
{
}

BOOL CutSideAdjustAction::LButtonDown(double& dPtX, double& dPtY)
{
	if (m_emState == CutSideAdjust_None || m_emState == CutSideAdjust_SecondPt)
	{
		m_emState = CutSideAdjust_FirstPt;
		m_data[0] = dPtX;
		m_data[1] = dPtY;
		m_data[2] = dPtX;
		m_data[3] = dPtY;
	}

	return TRUE;
}

BOOL CutSideAdjustAction::LButtonUp(double& dPtX, double& dPtY)
{
	CString strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);

	if (m_emState == CutSideAdjust_FirstPt)
	{
		m_emState = CutSideAdjust_SecondPt;

		// 得到选中的轮廓实例。
		LoopInstanceListPtr pSelectedLoopInstances(new LoopInstanceList);
		if (m_pRectSelectLoopInstances->size() > 0) {
			pSelectedLoopInstances->insert(pSelectedLoopInstances->end(), m_pRectSelectLoopInstances->begin(), m_pRectSelectLoopInstances->end());
		}
		else
		{
			if (m_pActiveLoopInstance) {
				pSelectedLoopInstances->push_back(m_pActiveLoopInstance);
			}
			else {
				MessageBox(m_pViewPort->GetWnd(), MultiLanguageMgr::GetInstance()->TranslateString(IDS_CHANGE_CUTSIDE), strProductName, MB_OK | MB_ICONWARNING);
				return TRUE;
			}
		}
		// 能运行到这里，肯定有轮廓被选中了。

		// 看看要不要涉及到相同零件的轮廓。
		LoopInstanceListPtr pLoopInstances_ChangeCutSide(new LoopInstanceList);
		pLoopInstances_ChangeCutSide->insert(pLoopInstances_ChangeCutSide->end(), pSelectedLoopInstances->begin(), pSelectedLoopInstances->end());
		if (m_pMatSize->GetType() != MAT_SIZE_Canvas && m_pSpreadSamePartActionBar->IsSpread())
		{
			for (unsigned int i = 0; i < pSelectedLoopInstances->size(); i++) {
				const LoopInstance* pSelectedLoopInstance = pSelectedLoopInstances->operator[](i).get();
				for (unsigned int j = 0; j < m_pAllLoopInstances->size(); j++) {
					LoopInstancePtr pLoopInstance = m_pAllLoopInstances->operator[](j);
					if (pLoopInstance->GetPatternLoopID() == pSelectedLoopInstance->GetPatternLoopID()) {
						if (!pLoopInstances_ChangeCutSide->GetLoopInstance_ByPartInstanceAndPatternLoop(pLoopInstance->GetPartInstance()->GetID(), pLoopInstance->GetPatternLoopID())) {
							pLoopInstances_ChangeCutSide->push_back(pLoopInstance);
						}
					}
				}
			}
		}

		// 设置这些轮廓实例的阴切阳切方式。
		const EditorData& editorData = GetEditorData();
		PartLayoutHelper::ChangeLoopCutOffset(editorData.GetView(), editorData.GetEditorName(), pLoopInstances_ChangeCutSide, m_emCutOffsetType, m_pLeadConfigItem);

		// 选中这些轮廓实例.
		if (m_pMatSize->GetType() == MAT_SIZE_Canvas) {
			CanvasGeometryEditAction::PostMessage2EnterCanvasLoopSelectAction_PreSelectLoopAndText(pLoopInstances_ChangeCutSide.get());
		}
		else {
			PartPlacementEditAction::PostMessage2EnterLoopInstanceSelectAction(pLoopInstances_ChangeCutSide.get());
		}
	}

	return TRUE;
}

BOOL CutSideAdjustAction::MovePoint(double& dPtX, double& dPtY)
{
	Point2D pt(dPtX, dPtY);

	if (m_emState == CutSideAdjust_FirstPt) // 在框选。
	{
		m_data[2] = dPtX;
		m_data[3] = dPtY;
		Rect2D rect(m_data[0] < m_data[2] ? m_data[0] : m_data[2], m_data[0] < m_data[2] ? m_data[2] : m_data[0],
			m_data[1] < m_data[3] ? m_data[1] : m_data[3], m_data[1] < m_data[3] ? m_data[3] : m_data[1]);
		m_pRectSelectLoopInstances = LoopInstanceManager::GetLoopInstsInRect(m_pAllLoopInstances, rect);
		m_pActiveLoopInstance.reset();
	}
	else { // 在点选。
		m_pActiveLoopInstance = LoopInstanceManager::GetClosestLoopInstEx(m_pAllLoopInstances, pt);
		m_pRectSelectLoopInstances->clear();
	}

	return TRUE;
}

BOOL CutSideAdjustAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	__super::OnKeyDown(nChar, nRepCnt, nFlags);
	return TRUE;
}

BOOL CutSideAdjustAction::DrawBeforeFinish()
{
	LoopDrawer loopDrawer(m_pViewPort);

	// 绘制高亮的轮廓实例。
	if (m_emState == CutSideAdjust_FirstPt) { // 框选。
		// 绘制框框。
		DrawSelectionBox(m_data[0], m_data[1], m_data[2], m_data[3]);

		// 绘制框选到的轮廓实例。
		for (unsigned int i = 0; i < m_pRectSelectLoopInstances->size(); i++) {
			const LoopInstance* pLoopInstance = m_pRectSelectLoopInstances->at(i).get();
			COLORREF iColor = LoopToolManager::GetLoopDisplayColor(pLoopInstance->GetCacheData()->GetLoopTool().get());
			loopDrawer.DrawActiveLoop(pLoopInstance->GetCacheData()->GetPatternLoop().get(), iColor, pLoopInstance->GetTransformMatrix());
		}
	}
	else { // 点选
		if (m_pActiveLoopInstance) { // 绘制当前的轮廓实例。
			COLORREF iColor = LoopToolManager::GetLoopDisplayColor(m_pActiveLoopInstance->GetCacheData()->GetLoopTool().get());
			loopDrawer.DrawActiveLoop(m_pActiveLoopInstance->GetCacheData()->GetPatternLoop().get(), iColor, m_pActiveLoopInstance->GetTransformMatrix());
		}
	}

	// draw cursor.
	CursorDrawer::DrawAimSelCursor(m_pViewPort);

	return TRUE;
}

END_CUTLEADER_NAMESPACE()

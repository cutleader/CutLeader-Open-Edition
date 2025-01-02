#include "StdAfx.h"
#include "CutDirectionAdjustAction.h"

#include "GlViewPort.h"
#include "CommandManager.h"
#include "DummyAction.h"
#include "ValueInputManager.h"
#include "CursorDrawer.h"
#include "clBaseDataResource.h"

#include "CLOptionInfo.h"
#include "DisplayParam.h"
#include "IMaterialSize.h"

#include "PatternList.h"
#include "IPatternLoop.h"
#include "PatternLoopList.h"
#include "LoopDrawer.h"
#include "LoopCutFeature.h"
#include "LoopCutFeatureList.h"
#include "LoopStartCutData.h"
#include "LoopStartCutDataList.h"
#include "clCutFeatureResource.h"
#include "LoopInstanceCache.h"
#include "LoopInstance.h"
#include "LoopInstanceList.h"
#include "PartInstanceList.h"
#include "LoopInstanceManager.h"
#include "CutDirectionsChangeCommand.h"
#include "PartLayoutHelper.h"
#include "PartInstance.h"
#include "CanvasGeometryEditAction.h"
#include "LoopToolManager.h"

BEGIN_CUTLEADER_NAMESPACE()

CutDirectionAdjustAction::CutDirectionAdjustAction(IMaterialSizePtr pMaterialSize, PartPlacementListPtr pPartPlacements, PartInstanceListPtr pPartInstances,
	PartGroupListPtr pPartGroups, CutDirectionChangeType emCutDirectionChangeType, GlViewPortPtr pViewPort, SpreadSamePartActionBar* pSpreadSamePartActionBar)
    : PartPlacementEditAction(pMaterialSize, pPartPlacements, pPartInstances, pPartGroups, pViewPort)
{
	m_emState = CutDirectionAdjust_None;
	m_pSpreadSamePartActionBar = pSpreadSamePartActionBar;
	m_iMsgID = IDS_CHANGE_DIRECTION;
	m_pAllLoopInstances = LoopInstanceManager::GetLoopInstanceList(pPartInstances);
	m_emCutDirectionChangeType = emCutDirectionChangeType;
	m_pRectSelectLoopInstances.reset(new LoopInstanceList);

	if (m_pMatSize->GetType() != MAT_SIZE_Canvas) {
		pSpreadSamePartActionBar->Show_SpreadSamePartActionBar();
	}
}

CutDirectionAdjustAction::~CutDirectionAdjustAction(void)
{
}

BOOL CutDirectionAdjustAction::LButtonDown(double& dPtX, double& dPtY)
{
	if (m_emState == CutDirectionAdjust_None || m_emState == CutDirectionAdjust_SecondPt)
	{
		m_emState = CutDirectionAdjust_FirstPt;
		m_data[0] = dPtX;
		m_data[1] = dPtY;
		m_data[2] = dPtX;
		m_data[3] = dPtY;
	}

	return TRUE;
}

BOOL CutDirectionAdjustAction::LButtonUp(double& dPtX, double& dPtY)
{
	CString strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);

	if (m_emState == CutDirectionAdjust_FirstPt)
	{
		m_emState = CutDirectionAdjust_SecondPt;

		// 得到选中的轮廓实例。
		LoopInstanceListPtr pSelectedLoopInstances(new LoopInstanceList);
		if (m_pRectSelectLoopInstances->size() > 0) {
			pSelectedLoopInstances->insert(pSelectedLoopInstances->end(), m_pRectSelectLoopInstances->begin(), m_pRectSelectLoopInstances->end());
		}
		else {
			if (m_pActiveLoopInstance) {
				pSelectedLoopInstances->push_back(m_pActiveLoopInstance);
			}
			else {
				MessageBox(m_pViewPort->GetWnd(), MultiLanguageMgr::GetInstance()->TranslateString(IDS_CHANGE_DIRECTION), strProductName, MB_OK | MB_ICONWARNING);
				return TRUE;
			}
		}
		// 能运行到这里，肯定有轮廓被选中了。

		// 看看要不要涉及到相同零件的轮廓。
		LoopInstanceListPtr pLoopInstances_ChangeCutDirection(new LoopInstanceList);
		pLoopInstances_ChangeCutDirection->insert(pLoopInstances_ChangeCutDirection->end(), pSelectedLoopInstances->begin(), pSelectedLoopInstances->end());
		if (m_pMatSize->GetType() != MAT_SIZE_Canvas && m_pSpreadSamePartActionBar->IsSpread())
		{
			for (unsigned int i = 0; i < pSelectedLoopInstances->size(); i++) {
				const LoopInstance* pSelectedLoopInstance = pSelectedLoopInstances->operator[](i).get();
				for (unsigned int j = 0; j < m_pAllLoopInstances->size(); j++) {
					LoopInstancePtr pLoopInstance = m_pAllLoopInstances->operator[](j);
					if (pLoopInstance->GetPatternLoopID() == pSelectedLoopInstance->GetPatternLoopID()) {
						if (!pLoopInstances_ChangeCutDirection->GetLoopInstance_ByPartInstanceAndPatternLoop(pLoopInstance->GetPartInstance()->GetID(), pLoopInstance->GetPatternLoopID())) {
							pLoopInstances_ChangeCutDirection->push_back(pLoopInstance);
						}
					}
				}
			}
		}

		// 设置这些轮廓实例的阴切阳切方式。
		const EditorData& editorData = GetEditorData();
		PartLayoutHelper::ChangeLoopCutDirection(editorData.GetView(), editorData.GetEditorName(), pLoopInstances_ChangeCutDirection, m_emCutDirectionChangeType);

		// 选中这些轮廓实例.
		if (m_pMatSize->GetType() == MAT_SIZE_Canvas) {
			CanvasGeometryEditAction::PostMessage2EnterCanvasLoopSelectAction_PreSelectLoopAndText(pLoopInstances_ChangeCutDirection.get());
		}
		else {
			PartPlacementEditAction::PostMessage2EnterLoopInstanceSelectAction(pLoopInstances_ChangeCutDirection.get());
		}
	}

	return TRUE;
}

BOOL CutDirectionAdjustAction::MovePoint(double& dPtX, double& dPtY)
{
	Point2D pt(dPtX, dPtY);

	if (m_emState == CutDirectionAdjust_FirstPt) {
		m_data[2] = dPtX;
		m_data[3] = dPtY;
		Rect2D rect(m_data[0] < m_data[2] ? m_data[0] : m_data[2], m_data[0] < m_data[2] ? m_data[2] : m_data[0],
			m_data[1] < m_data[3] ? m_data[1] : m_data[3], m_data[1] < m_data[3] ? m_data[3] : m_data[1]);
		m_pRectSelectLoopInstances = LoopInstanceManager::GetLoopInstsInRect(m_pAllLoopInstances, rect);
		m_pActiveLoopInstance.reset();
	}
	else {
		m_pActiveLoopInstance = LoopInstanceManager::GetClosestLoopInstEx(m_pAllLoopInstances, pt);
		m_pRectSelectLoopInstances->clear();
	}

	return TRUE;
}

BOOL CutDirectionAdjustAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	return __super::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CutDirectionAdjustAction::DrawBeforeFinish()
{
	LoopDrawer loopDrawer(m_pViewPort);

	// 绘制高亮的轮廓实例。
	if (m_emState == CutDirectionAdjust_FirstPt) { // 框选。
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

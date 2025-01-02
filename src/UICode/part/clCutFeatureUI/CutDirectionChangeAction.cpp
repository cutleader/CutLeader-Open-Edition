#include "StdAfx.h"
#include "CutDirectionChangeAction.h"

#include "GlViewPort.h"
#include "CommandManager.h"
#include "clBaseDataResource.h"
#include "ValueInputManager.h"
#include "CursorDrawer.h"

#include "CLOptionInfo.h"
#include "DisplayParam.h"

#include "PatternList.h"
#include "IPatternLoop.h"
#include "PatternLoopList.h"
#include "LoopDrawer.h"
#include "LoopCutFeature.h"
#include "LoopCutFeatureList.h"
#include "LoopStartCutData.h"
#include "clCutFeatureResource.h"
#include "CutDirectionsChangeCommand.h"
#include "LoopToolDataList.h"
#include "LoopToolManager.h"
#include "LoopStartCutDataList.h"
#include "LoopStartCutProcessor.h"

BEGIN_CUTLEADER_NAMESPACE()

CutDirectionChangeAction::CutDirectionChangeAction(PatternListPtr pPatList, PatternLoopListPtr pPatternLoopList, LoopTopologyItemListPtr pLoopTopologyItems, 
	LoopCutFeatureListPtr pLoopFeatureList, CutDirectionChangeType emCutDirectionChangeType, GlViewPortPtr pViewPort)
    : CutFeatureEditBase(pPatList, pPatternLoopList, pLoopTopologyItems, pLoopFeatureList, pViewPort)
{
	m_iMsgID = IDS_CHANGE_DIRECTION;
	m_emCutDirectionChangeType = emCutDirectionChangeType;
	m_emState = CutDirectionChange_None;
	m_pActivePatternLoops_inRect.reset(new PatternLoopList);
}

CutDirectionChangeAction::~CutDirectionChangeAction(void)
{
}

BOOL CutDirectionChangeAction::LButtonDown(double& dPtX, double& dPtY)
{
	if (m_emState == CutDirectionChange_None || m_emState == CutDirectionChange_SecondPt) {
		m_emState = CutDirectionChange_FirstPt;
		m_boxData[0] = dPtX;
		m_boxData[1] = dPtY;
		m_boxData[2] = dPtX;
		m_boxData[3] = dPtY;
	}

	return TRUE;
}

BOOL CutDirectionChangeAction::LButtonUp(double& dPtX, double& dPtY)
{
	CString strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);
	if (m_emState == CutDirectionChange_FirstPt)
	{
		m_emState = CutDirectionChange_SecondPt;

		// 得到选中的轮廓。
		PatternLoopListPtr pSelectedPatternLoops(new PatternLoopList);
		if (m_pActivePatternLoops_inRect->size() > 0) {
			pSelectedPatternLoops->insert(pSelectedPatternLoops->end(), m_pActivePatternLoops_inRect->begin(), m_pActivePatternLoops_inRect->end());
		}
		else if (m_pActivePatternLoop) {
			pSelectedPatternLoops->push_back(m_pActivePatternLoop);
		}
		if (pSelectedPatternLoops->size() == 0) {
			MessageBox(m_pViewPort->GetWnd(), MultiLanguageMgr::GetInstance()->TranslateString(IDS_CHANGE_DIRECTION), strProductName, MB_OK | MB_ICONWARNING);
			return TRUE;
		}

		// 挑选出可以改变切割方向的几何轮廓。
		LoopStartCutDataListPtr pLoopStartCutDatas_WillChange(new LoopStartCutDataList);
		for (unsigned int i = 0; i < pSelectedPatternLoops->size(); i++)
		{
			const IPatternLoop* pPatternLoop = pSelectedPatternLoops->operator[](i).get();
			LoopCutFeaturePtr pLoopCutFeature = m_pLoopFeatureList->FindByPatternLoopID(pPatternLoop->GetID());
			LoopStartCutDataPtr pLoopStartCutData = pLoopCutFeature->GetLoopStartCutData();
			if (pPatternLoop->IsOpenPath()) { // 这个轮廓是开放轮廓。
				if (m_emCutDirectionChangeType == CutDirectionChange_Switch) {
					pLoopStartCutDatas_WillChange->push_back(pLoopStartCutData);
				}
			}
			else { // 这个轮廓是封闭轮廓。
				DIRECTION emCutDirection = LoopStartCutProcessor::GetClosedLoopCutDirection(pLoopStartCutData.get());
				if (m_emCutDirectionChangeType == CutDirectionChange_Switch) {
					pLoopStartCutDatas_WillChange->push_back(pLoopStartCutData);
				}
				else if (m_emCutDirectionChangeType == CutDirectionChange_CCW && emCutDirection != CCW) {
					pLoopStartCutDatas_WillChange->push_back(pLoopStartCutData);
				}
				else if (m_emCutDirectionChangeType == CutDirectionChange_CW && emCutDirection != CW) {
					pLoopStartCutDatas_WillChange->push_back(pLoopStartCutData);
				}
			}
		}

		if (pLoopStartCutDatas_WillChange->size() > 0) {
			ICommandPtr pCommand(new CutDirectionsChangeCommand(GetEditorData(), pLoopStartCutDatas_WillChange));
			pCommand->Do();
			CommandManager::GetInstance()->AddCommand(pCommand);
		}
	}

	return TRUE;
}

BOOL CutDirectionChangeAction::MovePoint(double& dPtX, double& dPtY)
{
	Point2D pt(dPtX, dPtY);

	if (m_emState == CutDirectionChange_FirstPt) {
		m_boxData[2] = dPtX;
		m_boxData[3] = dPtY;
		Rect2D rect(m_boxData[0] < m_boxData[2] ? m_boxData[0] : m_boxData[2], m_boxData[0] < m_boxData[2] ? m_boxData[2] : m_boxData[0],
			m_boxData[1] < m_boxData[3] ? m_boxData[1] : m_boxData[3], m_boxData[1] < m_boxData[3] ? m_boxData[3] : m_boxData[1]);
		m_pActivePatternLoops_inRect = m_pPatternLoopList->GetPatternLoopsInRect(rect);
		m_pActivePatternLoop.reset();
	}
	else {
		IPatternPtr pPattern = m_pPatList->GetPatternClosestTo(pt);
		if (pPattern) {
			m_pActivePatternLoop = m_pPatternLoopList->FindLoopByPat(pPattern.get());
		}
		m_pActivePatternLoops_inRect->clear();
	}

	return TRUE;
}

BOOL CutDirectionChangeAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CutFeatureEditBase::OnKeyDown(nChar, nRepCnt, nFlags);

	return TRUE;
}

BOOL CutDirectionChangeAction::DrawBeforeFinish()
{
	LoopDrawer loopDrawer(m_pViewPort);
	if (m_emState == CutDirectionChange_FirstPt) { // 框选。
		DrawSelectionBox(m_boxData[0], m_boxData[1], m_boxData[2], m_boxData[3]); // 绘制框框。
		for (unsigned int i = 0; i < m_pActivePatternLoops_inRect->size(); i++) {
			IPatternLoopPtr pActivePatternLoop_inRect = m_pActivePatternLoops_inRect->at(i);
			const LoopToolData* pLoopToolData = m_pLoopFeatureList->GetLoopToolList()->GetLoopTool_by_loopID(pActivePatternLoop_inRect->GetID()).get();
			loopDrawer.DrawActiveLoop(pActivePatternLoop_inRect.get(), LoopToolManager::GetLoopDisplayColor(pLoopToolData));
		}
	}
	else { // 点选
		if (m_pActivePatternLoop) {
			const LoopToolData* pLoopToolData = m_pLoopFeatureList->GetLoopToolList()->GetLoopTool_by_loopID(m_pActivePatternLoop->GetID()).get();
			loopDrawer.DrawActiveLoop(m_pActivePatternLoop.get(), LoopToolManager::GetLoopDisplayColor(pLoopToolData));
		}
	}

	CursorDrawer::DrawAimSelCursor(m_pViewPort);
	return TRUE;
}

END_CUTLEADER_NAMESPACE()

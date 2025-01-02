#include "StdAfx.h"
#include "CutSideChangeAction.h"

#include <LoopStartCutDataList.h>

#include "GlViewPort.h"
#include "CommandManager.h"
#include "DummyAction.h"
#include "ValueInputManager.h"
#include "CursorDrawer.h"
#include "clBaseDataResource.h"
#include "LineArc2DList.h"

#include "PatternList.h"
#include "IPatternLoop.h"
#include "PatternLoopList.h"
#include "LoopDrawer.h"
#include "LoopCutFeature.h"
#include "LoopCutFeatureList.h"
#include "LoopStartCutData.h"
#include "LoopStartCutCache.h"
#include "clCutFeatureResource.h"
#include "CutSideChangeCommand.h"
#include "LoopStartCutProcessor.h"
#include "LoopToolDataList.h"
#include "LoopToolManager.h"

using namespace std;
using namespace std::tr1;

BEGIN_CUTLEADER_NAMESPACE()

CutSideChangeAction::CutSideChangeAction(PatternListPtr pPatList, PatternLoopListPtr pPatternLoopList, LoopTopologyItemListPtr pLoopTopologyItems, 
	LoopCutFeatureListPtr pLoopFeatureList, PatternLoopCutOffsetType emCutOffsetType, GlViewPortPtr pViewPort, LeadConfigItemPtr pLeadConfigItem)
    : CutFeatureEditBase(pPatList, pPatternLoopList, pLoopTopologyItems, pLoopFeatureList, pViewPort)
{
	m_emCutOffsetType = emCutOffsetType;
	m_pLeadConfigItem = pLeadConfigItem;
	m_emState = CutSideChange_None;
	m_iMsgID = IDS_CHANGE_CUTSIDE;
	m_pActivePatternLoops_inRect.reset(new PatternLoopList);
}

CutSideChangeAction::~CutSideChangeAction(void)
{
}

BOOL CutSideChangeAction::LButtonDown(double& dPtX, double& dPtY)
{
	if (m_emState == CutSideChange_None || m_emState == CutSideChange_SecondPt)
	{
		m_emState = CutSideChange_FirstPt;

		m_boxData[0] = dPtX;
		m_boxData[1] = dPtY;
		m_boxData[2] = dPtX;
		m_boxData[3] = dPtY;
	}

	return TRUE;
}

BOOL CutSideChangeAction::LButtonUp(double& dPtX, double& dPtY)
{
	CString strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);

	if (m_emState == CutSideChange_FirstPt)
	{
		m_emState = CutSideChange_SecondPt;

		// 得到选中的轮廓。
		PatternLoopListPtr pSelectedPatternLoops(new PatternLoopList);
		if (m_pActivePatternLoops_inRect->size() > 0) {
			pSelectedPatternLoops->insert(pSelectedPatternLoops->end(), m_pActivePatternLoops_inRect->begin(), m_pActivePatternLoops_inRect->end());
		}
		else if (m_pActivePatternLoop) {
			pSelectedPatternLoops->push_back(m_pActivePatternLoop);
		}
		if (pSelectedPatternLoops->size() == 0) {
			MessageBox(m_pViewPort->GetWnd(), MultiLanguageMgr::GetInstance()->TranslateString(IDS_CHANGE_CUTSIDE), strProductName, MB_OK | MB_ICONWARNING);
			return TRUE;
		}

		// 挑选出可以切换阴阳切的几何轮廓。
		LoopStartCutDataListPtr pLoopStartCutDataList(new LoopStartCutDataList);
		for (unsigned int i = 0; i < pSelectedPatternLoops->size(); i++)
		{
			const IPatternLoop* pPatternLoop = pSelectedPatternLoops->operator[](i).get();
			if (pPatternLoop->IsOpenPath()) { // 开放轮廓没必要设置。
				continue;
			}

			LoopCutFeaturePtr pLoopCutFeature = m_pLoopFeatureList->FindByPatternLoopID(pPatternLoop->GetID());
			LoopStartCutDataPtr pLoopStartCutData = pLoopCutFeature->GetLoopStartCutData();
			PatternLoopCutOffsetType emOldCutOffsetType = LoopStartCutProcessor::CalculateLoopCutOffset(pLoopStartCutData.get());
			if (emOldCutOffsetType == m_emCutOffsetType) { // 这种情况没必要进行设置。
				continue;
			}

			pLoopStartCutDataList->push_back(pLoopStartCutData);
		}

		if (pLoopStartCutDataList->size() > 0) {
			ICommandPtr pCommand(new CutSideChangeCommand(GetEditorData(), pLoopStartCutDataList, m_emCutOffsetType, m_pLeadConfigItem));
			pCommand->Do();
			CommandManager::GetInstance()->AddCommand(pCommand);
		}
	}

	return TRUE;
}

BOOL CutSideChangeAction::MovePoint(double& dPtX, double& dPtY)
{
	Point2D pt(dPtX, dPtY);

	if (m_emState == CutSideChange_FirstPt) {
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

BOOL CutSideChangeAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CutFeatureEditBase::OnKeyDown(nChar, nRepCnt, nFlags);

	return TRUE;
}

BOOL CutSideChangeAction::DrawBeforeFinish()
{
	LoopDrawer loopDrawer(m_pViewPort);
	if (m_emState == CutSideChange_FirstPt) { // 框选。
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

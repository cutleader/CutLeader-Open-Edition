#include "StdAfx.h"
#include "StartCutPtPreSelectAction2.h"

#include "MathEx.h"
#include "Rect2D.h"
#include "Point2DList.h"
#include "GlViewPort.h"
#include "CursorDrawer.h"
#include "ValueInputManager.h"
#include "clBaseDataResource.h"

#include "DisplayParam.h"
#include "CLOptionInfo.h"

#include "clGeometryFeatureResource.h"
#include "clCutFeatureResource.h"
#include "LoopStartCutDataList.h"
#include "LoopCutFeatureList.h"
#include "LoopCutFeatureDrawer.h"
#include "LoopInstanceList.h"
#include "PartInstanceList.h"

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

StartCutPtPreSelectAction2::StartCutPtPreSelectAction2(IMaterialSizePtr pMaterialSize, PartPlacementListPtr pPartPlacementList,
	PartInstanceListPtr pPartInstanceList, PartGroupListPtr pPartGroups, GlViewPortPtr pViewPort)
    : PartPlacementEditAction(pMaterialSize, pPartPlacementList, pPartInstanceList, pPartGroups, pViewPort)
{
	m_preSelectState = PRE_SELECT_NO_INPUT;
	m_iMsgID = IDS_CUTFEATURE_SELECT_STARTCUT;
	m_pRectSelectStartCutPts.reset(new Point2DList);
}

StartCutPtPreSelectAction2::~StartCutPtPreSelectAction2(void)
{
}

BOOL StartCutPtPreSelectAction2::LButtonDown(double& dPtX, double& dPtY)
{
	if (m_preSelectState == PRE_SELECT_NO_INPUT || m_preSelectState == PRE_SELECT_LEFT_BTN_UP)
	{
		m_preSelectState = PRE_SELECT_LEFT_BTN_DOWN;
		m_iMsgID = IDS_OUTPUT_SELECTPAT_NEXTPT;

		m_data[0] = dPtX;
		m_data[1] = dPtY;
		m_data[2] = dPtX;
		m_data[3] = dPtY;
	}

	return TRUE;
}

BOOL StartCutPtPreSelectAction2::LButtonUp(double& dPtX, double& dPtY)
{
	CString strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);

	if (m_preSelectState == PRE_SELECT_LEFT_BTN_DOWN)
	{
		m_preSelectState = PRE_SELECT_LEFT_BTN_UP;

		// 得到这次选中的起切点及轮廓起切特征。
		vector<pair<LoopStartCutDataPtr, Point2D>> allStartCutPtPairs = m_pAllLoopInstanceList->GetAllStartCutPtPairs();
		vector<pair<LoopStartCutDataPtr, Point2D>> selectedStartCutPtPairs;
		if (m_pRectSelectStartCutPts->size() > 0) { // 框选到了起切点。
			for (unsigned int i = 0; i < m_pRectSelectStartCutPts->size(); i++) {
				const Point2D& startCutPt = m_pRectSelectStartCutPts->operator [](i);
				for (unsigned int j = 0; j < allStartCutPtPairs.size(); j++)
				{
					const pair<LoopStartCutDataPtr, Point2D>& startCutPtPair = allStartCutPtPairs.at(j);
					if (startCutPtPair.second == startCutPt) {
						selectedStartCutPtPairs.push_back(startCutPtPair);
						break;
					}
				}
			}
		}
		else if (m_pActiveStartCutPt) { // 点选到了起切点。
			for (unsigned int j = 0; j < allStartCutPtPairs.size(); j++) {
				const pair<LoopStartCutDataPtr, Point2D>& startCutPtPair = allStartCutPtPairs.at(j);
				if (startCutPtPair.second == *m_pActiveStartCutPt) {
					selectedStartCutPtPairs.push_back(startCutPtPair);
					break;
				}
			}
		}

		// 确定最终选中的起切点及轮廓起切特征。
		if (selectedStartCutPtPairs.size() > 0) {
			if (!ValueInputManager::GetInstance()->IsInputCtrl()) { // 没有按下ctrl键，要清除之前的选择。
				m_selStaEndPtPairs.clear();
			}
			m_selStaEndPtPairs.insert(m_selStaEndPtPairs.end(), selectedStartCutPtPairs.begin(), selectedStartCutPtPairs.end());
		}

		// 确定提示信息。
		if (m_selStaEndPtPairs.size() == 0) {
			m_iMsgID = IDS_CUTFEATURE_NO_STARTPT_SELECT;
			MessageBox(m_pViewPort->GetWnd(), MultiLanguageMgr::GetInstance()->TranslateString(IDS_CUTFEATURE_NO_STARTPT_SELECT), strProductName, MB_OK | MB_ICONWARNING);
		}
		else {
			m_iMsgID = IDS_CUTFEATURE_STARTPT_SELECTED;
		}
	}

	return TRUE;
}

BOOL StartCutPtPreSelectAction2::MovePoint(double& dPtX, double& dPtY)
{
	Point2D pt(dPtX, dPtY);

	Point2DListPtr pAllStartPts = m_pAllLoopInstanceList->GetAllStartCutPts();
	if (m_preSelectState == PRE_SELECT_LEFT_BTN_DOWN) // 在框选。
	{
		m_data[2] = dPtX;
		m_data[3] = dPtY;
		Rect2D rect(m_data[0] < m_data[2] ? m_data[0] : m_data[2], m_data[0] < m_data[2] ? m_data[2] : m_data[0],
			m_data[1] < m_data[3] ? m_data[1] : m_data[3], m_data[1] < m_data[3] ? m_data[3] : m_data[1]);

		m_pRectSelectStartCutPts->clear();
		for (unsigned int i = 0; i < pAllStartPts->size(); i++) {
			const Point2D& pt = pAllStartPts->operator [](i);
			if (rect.ContainPt(pt)) {
				m_pRectSelectStartCutPts->push_back(pt);
			}
		}
		m_pActiveStartCutPt.reset();
	}
	else { // 在点选，得到离光标最近的起切点。
		if (pAllStartPts->size() > 0) {
			m_pActiveStartCutPt.reset(new Point2D(pAllStartPts->GetPtClosestTo(pt)));
		}
		else {
			m_pActiveStartCutPt.reset();
		}
		m_pRectSelectStartCutPts->clear();
	}

	return TRUE;
}

BOOL StartCutPtPreSelectAction2::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// 选中全部“回路起切特征”。
	if (ValueInputManager::GetInstance()->IsInputCtrl() && ValueInputManager::GetInstance()->IsInputA(nChar))
	{
		m_selStaEndPtPairs.clear();
		vector< pair<LoopStartCutDataPtr, Point2D> > staEndPtPairs = m_pAllLoopInstanceList->GetAllStartCutPtPairs();
		m_selStaEndPtPairs.insert(m_selStaEndPtPairs.end(), staEndPtPairs.begin(), staEndPtPairs.end());
	}

	return __super::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL StartCutPtPreSelectAction2::DrawBeforeFinish()
{
	const DisplayParam* pDisplayParam = CLOptionInfo::GetInstance()->GetDisplayParam().get();
	LoopCutFeatureDrawer loopFeatureDrawer(m_pViewPort);

	// 绘制选中的起切点。
	for (unsigned int i = 0; i < m_selStaEndPtPairs.size(); i++) {
		loopFeatureDrawer.DrawFeatureLoc(m_selStaEndPtPairs.at(i).second, *pDisplayParam->GetSelectObjColor());
	}

	if (m_preSelectState == PRE_SELECT_LEFT_BTN_DOWN) { // 在框选。
		// 绘制选择框。
		DrawSelectionBox(m_data[0], m_data[1], m_data[2], m_data[3]);

		// 绘制框选到的轮廓实例。
		for (unsigned int i = 0; i < m_pRectSelectStartCutPts->size(); i++) {
			const Point2D& pt = m_pRectSelectStartCutPts->operator [](i);
			loopFeatureDrawer.DrawFeatureLoc(pt, *pDisplayParam->GetActiveObjColor());
		}
	}
	else { // 点选
		if (m_pActiveStartCutPt) { // 绘制当前的起切点。
			loopFeatureDrawer.DrawFeatureLoc(*m_pActiveStartCutPt, *pDisplayParam->GetActiveObjColor());
		}
	}

	// 绘制光标。
	CursorDrawer::DrawAimSelCursor(m_pViewPort);

	return TRUE;
}

END_CUTLEADER_NAMESPACE()

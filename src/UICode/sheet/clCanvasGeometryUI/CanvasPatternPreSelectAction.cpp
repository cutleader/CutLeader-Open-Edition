#include "StdAfx.h"
#include "CanvasPatternPreSelectAction.h"

#include "MathEx.h"
#include "GlViewPort.h"
#include "Rect2D.h"
#include "CommandManager.h"
#include "ValueInputManager.h"
#include "clUILibResource.h"
#include "DummyAction.h"
#include "CursorDrawer.h"
#include "clBaseDataResource.h"
#include "LogMgr.h"

#include "CLOptionInfo.h"
#include "DisplayParam.h"

#include "PatternDrawer.h"
#include "PatternList.h"
#include "clGeometryFeatureResource.h"
#include "PartPlacement.h"
#include "Part.h"
#include "PartCadData.h"
#include "PatternLoopList.h"
#include "GeometryFeatureHelper.h"
#include "TextStruct.h"
#include "TextStructList.h"

BEGIN_CUTLEADER_NAMESPACE()

CanvasPatternPreSelectAction::CanvasPatternPreSelectAction(PartPlacementPtr pPartPlacement, SelectPatternOrLoopActionBar* pSelectPatternOrLoopActionBar, GlViewPortPtr pViewPort)
	: CanvasGeometryEditAction(pPartPlacement, pViewPort)
{
	m_pActivePatterns.reset(new PatternList);
	m_pSelectedPatterns.reset(new PatternList);
	m_pActiveTextStructs.reset(new TextStructList);
	m_pSelectedTextStructs.reset(new TextStructList);

	m_preSelectState = PRE_SELECT_NO_INPUT;
	m_pSelectPatternOrLoopActionBar = pSelectPatternOrLoopActionBar;
}

CanvasPatternPreSelectAction::~CanvasPatternPreSelectAction(void)
{
}

BOOL CanvasPatternPreSelectAction::LButtonDown(double& dPtX, double& dPtY)
{
	if (m_preSelectState == PRE_SELECT_NO_INPUT || m_preSelectState == PRE_SELECT_LEFT_BTN_UP) {
		m_preSelectState = PRE_SELECT_LEFT_BTN_DOWN;
		m_data[0] = dPtX;
		m_data[1] = dPtY;
		m_data[2] = dPtX;
		m_data[3] = dPtY;
	}

	return TRUE;
}

BOOL CanvasPatternPreSelectAction::LButtonUp(double& dPtX, double& dPtY)
{
	if (m_preSelectState == PRE_SELECT_LEFT_BTN_DOWN)
	{
		m_preSelectState = PRE_SELECT_LEFT_BTN_UP;

		// 更新选择集。
		if (!ValueInputManager::GetInstance()->IsInputCtrl()) { // 要是没有按住ctrl键，就需要清除现有选择集。
			m_pSelectedPatterns->clear();
			m_loopsOfSelectedPatterns.clear();
			m_pSelectedTextStructs->clear();
		}
		if (m_pActivePatterns->size() > 0) {
			m_pSelectedPatterns->insert(m_pSelectedPatterns->end(), m_pActivePatterns->begin(), m_pActivePatterns->end());
			m_loopsOfSelectedPatterns = m_loopsOfActivePatterns;
		}
		if (m_pActiveTextStructs->size() > 0) {
			m_pSelectedTextStructs->insert(m_pSelectedTextStructs->end(), m_pActiveTextStructs->begin(), m_pActiveTextStructs->end());
		}
	}

	return TRUE;
}

BOOL CanvasPatternPreSelectAction::MovePoint(double& dPtX, double& dPtY)
{
	m_pActivePatterns->clear();
	m_pActiveTextStructs->clear();

	const PartCadData* pPartCadData = m_pPartPlacement->GetPart()->GetCadData().get();
	if (m_preSelectState == PRE_SELECT_LEFT_BTN_DOWN) // 框选。
	{
		m_data[2] = dPtX;
		m_data[3] = dPtY;

		Rect2D rect(m_data[0] < m_data[2] ? m_data[0] : m_data[2], m_data[0] < m_data[2] ? m_data[2] : m_data[0],
			m_data[1] < m_data[3] ? m_data[1] : m_data[3], m_data[1] < m_data[3] ? m_data[3] : m_data[1]);
		TextStructListPtr pTextStructs(new TextStructList);
		if (m_pSelectPatternOrLoopActionBar->GetIsSelectPattern()) {
			m_pActivePatterns = GeometryFeatureHelper::GetPatterns_byRectSelection(pPartCadData, rect, pTextStructs);
			m_loopsOfActivePatterns = GeometryFeatureHelper::FindPatternLoopsOfPatterns(pPartCadData, m_pActivePatterns.get());
			if (pTextStructs->size() > 0) {
				m_pActiveTextStructs->insert(m_pActiveTextStructs->end(), pTextStructs->begin(), pTextStructs->end());
			}
		}
		else if (m_pSelectPatternOrLoopActionBar->GetIsSelectPatternLoop()) {
			PatternLoopListPtr pPatternLoops = GeometryFeatureHelper::GetPatternLoops_byRectSelection(pPartCadData, rect, pTextStructs);
			if (pPatternLoops->size() > 0) {
				m_pActivePatterns = pPatternLoops->GetPatList(m_loopsOfActivePatterns);
				if (pTextStructs->size() > 0) {
					m_pActiveTextStructs->insert(m_pActiveTextStructs->end(), pTextStructs->begin(), pTextStructs->end());
				}
			}
		}
	}
	else { // 点选。
		Point2D pt(dPtX, dPtY);
		TextStructPtr pTextStruct;
		if (m_pSelectPatternOrLoopActionBar->GetIsSelectPattern()) {
			m_pActivePatterns = GeometryFeatureHelper::GetPatterns_byClickSelection(pPartCadData, pt, pTextStruct);
			m_loopsOfActivePatterns = GeometryFeatureHelper::FindPatternLoopsOfPatterns(pPartCadData, m_pActivePatterns.get());
			if (pTextStruct) {
				m_pActiveTextStructs->push_back(pTextStruct);
			}
		}
		else if (m_pSelectPatternOrLoopActionBar->GetIsSelectPatternLoop()) {
			PatternListPtr pPatterns = GeometryFeatureHelper::GetPatterns_byClickSelection(pPartCadData, pt, pTextStruct);
			if (pPatterns->size() > 0) {
				if (pTextStruct) { // 选中了一个文字对象。
					m_pActivePatterns = pPatterns;
					m_pActiveTextStructs->push_back(pTextStruct);
				}
				else { // 此时应该选中了一个几何特征。
					IPatternLoopPtr pPatternLoop = pPartCadData->GetPatternLoopList()->FindLoopByPat(pPatterns->front().get());
					m_pActivePatterns = pPatternLoop->GetPatternList();
				}
				m_loopsOfActivePatterns = GeometryFeatureHelper::FindPatternLoopsOfPatterns(pPartCadData, m_pActivePatterns.get());
			}
		}
	}

	return TRUE;
}

BOOL CanvasPatternPreSelectAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CanvasGeometryEditAction::OnKeyDown(nChar, nRepCnt, nFlags);

	// 选中全部图形。
	PartCadDataPtr pPartCadData = m_pPartPlacement->GetPart()->GetCadData();
	if (ValueInputManager::GetInstance()->IsInputCtrl() && ValueInputManager::GetInstance()->IsInputA(nChar))
	{
		m_pSelectedPatterns->clear();
		m_pSelectedPatterns->insert(m_pSelectedPatterns->end(), pPartCadData->GetPatternList()->begin(), pPartCadData->GetPatternList()->end());
		m_loopsOfSelectedPatterns = GeometryFeatureHelper::FindPatternLoopsOfPatterns(pPartCadData.get(), m_pSelectedPatterns.get());
	}

	return TRUE;
}

BOOL CanvasPatternPreSelectAction::DrawBeforeFinish()
{
	const DisplayParam* pDisplayParam = CLOptionInfo::GetInstance()->GetDisplayParam().get();
	PatternDrawer patternDrawer(m_pViewPort);

	// 绘制选择框。
	if (m_preSelectState == PRE_SELECT_LEFT_BTN_DOWN) {
		DrawSelectionBox(m_data[0], m_data[1], m_data[2], m_data[3]);
	}

	// 绘制当前几何特征。
	DrawActivePatterns(m_pActivePatterns.get(), m_loopsOfActivePatterns);

	// 绘制选中的几何特征。
	DrawSelectedPatterns(m_pSelectedPatterns.get(), m_loopsOfSelectedPatterns);

	// draw cursor.
	CursorDrawer::DrawAimSelCursor(m_pViewPort);

	return TRUE;
}

END_CUTLEADER_NAMESPACE()

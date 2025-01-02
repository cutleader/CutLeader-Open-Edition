#include "StdAfx.h"
#include "PatternPreSelectAction.h"

#include "MathEx.h"
#include "GlViewPort.h"
#include "Rect2D.h"
#include "CommandManager.h"
#include "ValueInputManager.h"
#include "clUILibResource.h"
#include "DummyAction.h"
#include "CursorDrawer.h"
#include "clBaseDataResource.h"

#include "CLOptionInfo.h"
#include "DisplayParam.h"

#include "PatternDrawer.h"
#include "PatternList.h"
#include "clGeometryFeatureResource.h"
#include "PartCadData.h"
#include "TextStruct.h"
#include "TextStructList.h"
#include "GeometryFeatureHelper.h"

BEGIN_CUTLEADER_NAMESPACE()

PatternPreSelectAction::PatternPreSelectAction()
{
	m_pActivePats.reset(new PatternList);
	m_pSelectedPats.reset(new PatternList);
	m_pActiveTextStructs.reset(new TextStructList);
	m_pSelectedTextStructs.reset(new TextStructList);

	m_preSelectState = PRE_SELECT_NO_INPUT;
	m_iMsgID = IDS_OUTPUT_SELECTPATTERN;
}

PatternPreSelectAction::~PatternPreSelectAction(void)
{
}

BOOL PatternPreSelectAction::LButtonDown(double& dPtX, double& dPtY)
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

BOOL PatternPreSelectAction::LButtonUp(double& dPtX, double& dPtY)
{
	if (m_preSelectState == PRE_SELECT_LEFT_BTN_DOWN)
	{
		m_preSelectState = PRE_SELECT_LEFT_BTN_UP;
		m_data[2] = dPtX;
		m_data[3] = dPtY;

		// 更新选择集。
		if (!ValueInputManager::GetInstance()->IsInputCtrl()) // 如果没有按下control键，那么就换掉选择集。
		{
			m_pSelectedPats->clear();
			m_pSelectedPats->AddPatterns(m_pActivePats);
			m_pSelectedTextStructs->clear();
			m_pSelectedTextStructs->insert(m_pSelectedTextStructs->end(), m_pActiveTextStructs->begin(), m_pActiveTextStructs->end());
		}
		else // 如果按下了control键，那么需要更新现有选择集。
		{
			for (unsigned int i = 0; i < m_pActivePats->size(); i++)
			{
				IPatternPtr pActivePat = m_pActivePats->operator[](i);
				if (m_pSelectedPats->GetPatternFromID(pActivePat->GetID()) == nullptr) {
					m_pSelectedPats->push_back(pActivePat);
				}
			}
			for (unsigned int i = 0; i < m_pActiveTextStructs->size(); i++)
			{
				TextStructPtr pActiveTextStruct = m_pActiveTextStructs->operator [](i);
				if (m_pSelectedTextStructs->GetTextStructByID(pActiveTextStruct->GetID()) == nullptr) {
					m_pSelectedTextStructs->push_back(pActiveTextStruct);
				}
			}
		}

		// 确定提示信息。
		if (m_pSelectedPats->size() == 0)
			m_iMsgID = IDS_OUTPUT_NOPAT_SELECT;
		else
			m_iMsgID = IDS_OUTPUT_PAT_SELECT;
	}

	return TRUE;
}

BOOL PatternPreSelectAction::MovePoint(double& dPtX, double& dPtY)
{
	m_pActivePats->clear();
	m_pActiveTextStructs->clear();

	if (m_preSelectState == PRE_SELECT_LEFT_BTN_DOWN) // 框选。
	{
		m_data[2] = dPtX;
		m_data[3] = dPtY;

		Rect2D rect(m_data[0] < m_data[2] ? m_data[0] : m_data[2], m_data[0] < m_data[2] ? m_data[2] : m_data[0],
			m_data[1] < m_data[3] ? m_data[1] : m_data[3], m_data[1] < m_data[3] ? m_data[3] : m_data[1]);
		TextStructListPtr pTextStructs(new TextStructList);
		m_pActivePats = GeometryFeatureHelper::GetPatterns_byRectSelection(m_pPartCadData.get(), rect, pTextStructs);
		if (pTextStructs->size() > 0) {
			m_pActiveTextStructs->insert(m_pActiveTextStructs->end(), pTextStructs->begin(), pTextStructs->end());
		}
	}
	else // 点选。
	{
		TextStructPtr pTextStruct;
		m_pActivePats = GeometryFeatureHelper::GetPatterns_byClickSelection(m_pPartCadData.get(), Point2D(dPtX, dPtY), pTextStruct);
		if (pTextStruct) {
			m_pActiveTextStructs->push_back(pTextStruct);
		}
	}

	return TRUE;
}

BOOL PatternPreSelectAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	PatternEditActionBase::OnKeyDown(nChar, nRepCnt, nFlags);

	// 选中全部图形。
	PatternListPtr pPatList = m_pPartCadData->GetPatternList();
	if (ValueInputManager::GetInstance()->IsInputCtrl() && ValueInputManager::GetInstance()->IsInputA(nChar)) {
		m_pSelectedPats->insert(m_pSelectedPats->end(), pPatList->begin(), pPatList->end());
	}

	return TRUE;
}

BOOL PatternPreSelectAction::DrawBeforeFinish()
{
	const DisplayParam* pDisplayParam = CLOptionInfo::GetInstance()->GetDisplayParam().get();
	PatternDrawer patternDrawer(m_pViewPort);

	// 绘制选择框。
	if (m_preSelectState == PRE_SELECT_LEFT_BTN_DOWN)
	{
		DrawSelectionBox(m_data[0], m_data[1], m_data[2], m_data[3]);
	}

	// 绘制高亮的和选中的几何特征。
	for (unsigned int i = 0; i < m_pActivePats->size(); i++) {
		patternDrawer.DrawActivePat(m_pActivePats->operator[](i).get());
	}
	patternDrawer.DrawSelPatList(m_pSelectedPats.get());

	// draw cursor.
	CursorDrawer::DrawAimSelCursor(m_pViewPort);

	return TRUE;
}

END_CUTLEADER_NAMESPACE()

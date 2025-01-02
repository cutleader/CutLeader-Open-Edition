#include "StdAfx.h"
#include "CornerPreSelectAction.h"

#include "MathEx.h"
#include "Rect2D.h"
#include "Point2DList.h"
#include "GlViewPort.h"
#include "CursorDrawer.h"
#include "ValueInputManager.h"

#include "clGeometryFeatureResource.h"

#include "clCutFeatureResource.h"
#include "LoopCutFeatureDrawer.h"
#include "CornerList.h"
#include "LoopCornerDataList.h"
#include "LoopCutFeatureList.h"

#include "DisplayParam.h"
#include "CLOptionInfo.h"

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

CornerPreSelectAction::CornerPreSelectAction(PatternListPtr pPatList, PatternLoopListPtr pPatternLoopList, LoopTopologyItemListPtr pLoopTopologyItems,
	LoopCutFeatureListPtr pLoopFeatureList, GlViewPortPtr pViewPort)
    : CutFeatureEditBase(pPatList, pPatternLoopList, pLoopTopologyItems, pLoopFeatureList, pViewPort)
{
    m_iMsgID = IDS_CUTFEATURE_SELECT_CORNER;
    m_preSelectState = PRE_SELECT_NO_INPUT;
}

CornerPreSelectAction::~CornerPreSelectAction(void)
{
}

BOOL CornerPreSelectAction::LButtonDown(double& dPtX, double& dPtY)
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

BOOL CornerPreSelectAction::LButtonUp(double& dPtX, double& dPtY)
{
	if (m_preSelectState == PRE_SELECT_LEFT_BTN_DOWN)
	{
		m_data[2] = dPtX;
		m_data[3] = dPtY;

		// 确定选中的角特征。
		vector< pair<ICornerPtr, Point2D> > selCornerPtPairs = CalcSelectCornerPt();
		if (selCornerPtPairs.size() > 0)
		{
			// 要是没有按下ctrl键，要清除之前的选择。
			if (!ValueInputManager::GetInstance()->IsInputCtrl())
				m_selCornerPtPairs.clear();

			// 更新选中的“角特征”信息。
			m_selCornerPtPairs.insert(m_selCornerPtPairs.end(), selCornerPtPairs.begin(), selCornerPtPairs.end());
		}

		// 确定提示信息。
		if (m_selCornerPtPairs.size() == 0)
			m_iMsgID = IDS_CUTFEATURE_NO_CORNER_SELECT;
		else
			m_iMsgID = IDS_CUTFEATURE_CORNER_SELECTED;

		// 当前的鼠标状态。
		m_preSelectState = PRE_SELECT_LEFT_BTN_UP;
	}

	return TRUE;
}

BOOL CornerPreSelectAction::MovePoint(double& dPtX, double& dPtY)
{
	Point2D pt(dPtX, dPtY);

	// 得到离光标最近的角特征点。
	LoopCornerDataListPtr pLoopCornerList = m_pLoopFeatureList->GetLoopCornerList();
	Point2DListPtr pCornerPtList = pLoopCornerList->GetAllCornerPts();
	if (pCornerPtList->size() > 0)
		m_pActiveCornerPt.reset(new Point2D(pCornerPtList->GetPtClosestTo(pt)));
	else
		m_pActiveCornerPt.reset();

	// if left button down, we should draw a rectangle in stipple mode.
	if (m_preSelectState == PRE_SELECT_LEFT_BTN_DOWN)
	{
		m_data[2] = dPtX;
		m_data[3] = dPtY;
	}

	return TRUE;
}

BOOL CornerPreSelectAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CutFeatureEditBase::OnKeyDown(nChar, nRepCnt, nFlags);

	// 选中全部“角特征”。
	if (ValueInputManager::GetInstance()->IsInputCtrl() && ValueInputManager::GetInstance()->IsInputA(nChar))
	{
		m_selCornerPtPairs.clear();
		LoopCornerDataListPtr pLoopCornerList = m_pLoopFeatureList->GetLoopCornerList();
		vector< pair<ICornerPtr, Point2D> > cornerPtPairs = pLoopCornerList->GetAllCornerPtPairs();
		m_selCornerPtPairs.insert(m_selCornerPtPairs.end(), cornerPtPairs.begin(), cornerPtPairs.end());
	}

	return TRUE;
}

BOOL CornerPreSelectAction::DrawBeforeFinish()
{
	// prepare some data.
	CLOptionInfoPtr pCLOptionInfo = CLOptionInfo::GetInstance();
	DisplayParamPtr pColorInfo = pCLOptionInfo->GetDisplayParam();
	LoopCutFeatureDrawer loopFeatureDrawer(m_pViewPort);

	// 绘制选择框。
	if (m_preSelectState == PRE_SELECT_LEFT_BTN_DOWN) {
		DrawSelectionBox(m_data[0], m_data[1], m_data[2], m_data[3]);
	}

	// 绘制当前微连接点。
	if (m_pActiveCornerPt)
		loopFeatureDrawer.DrawFeatureLoc(*m_pActiveCornerPt, *pColorInfo->GetActiveObjColor());

	// 绘制选中的微连接点。
	for (unsigned int i = 0; i < m_selCornerPtPairs.size(); i++)
		loopFeatureDrawer.DrawFeatureLoc(m_selCornerPtPairs.at(i).second, *pColorInfo->GetSelectObjColor());

	// 绘制光标。
	CursorDrawer::DrawAimSelCursor(m_pViewPort);

	return TRUE;
}

CornerListPtr CornerPreSelectAction::GetSelectCornerList()
{
	CornerListPtr pCornerList(new CornerList);

	for (unsigned int i = 0; i < m_selCornerPtPairs.size(); i++)
	{
		pair<ICornerPtr, Point2D> cornerPtPair = m_selCornerPtPairs.at(i);
		pCornerList->push_back(cornerPtPair.first);
	}

	return pCornerList;
}

vector< pair<ICornerPtr, Point2D> > CornerPreSelectAction::CalcSelectCornerPt()
{
	vector< pair<ICornerPtr, Point2D> > selCornerPtPairs;

	// 得到所有“角特征点”及对应的“角特征”对象。
	LoopCornerDataListPtr pLoopCornerList = m_pLoopFeatureList->GetLoopCornerList();
	vector< pair<ICornerPtr, Point2D> > cornerPtPairs = pLoopCornerList->GetAllCornerPtPairs();

	if (MathEx::Equal(m_data[0], m_data[2]) && MathEx::Equal(m_data[1], m_data[3]))
	{
		Point2D pt(m_data[0], m_data[1]);

		// 得到离这个点最近的角特征。
		double dDis = INFINITE_VAL;
		int iIndex = INVALID_IDX;
		for (unsigned int i = 0; i < cornerPtPairs.size(); i++)
		{
			pair<ICornerPtr, Point2D> cornerPtPair = cornerPtPairs.at(i);
			double dTmpDis = pt.DistanceTo(cornerPtPair.second);
			if (dTmpDis < dDis)
			{
				dDis = dTmpDis;
				iIndex = i;
			}
		}
		if (iIndex != INVALID_IDX)
			selCornerPtPairs.push_back(cornerPtPairs[iIndex]);
	}
	else
	{
		Rect2D rect;
		rect.SetXMin(m_data[0] < m_data[2] ? m_data[0] : m_data[2]);
		rect.SetXMax(m_data[0] < m_data[2] ? m_data[2] : m_data[0]);
		rect.SetYMin(m_data[1] < m_data[3] ? m_data[1] : m_data[3]);
		rect.SetYMax(m_data[1] < m_data[3] ? m_data[3] : m_data[1]);

		// 得到在选择框中的角特征。
		for (unsigned int i = 0; i < cornerPtPairs.size(); i++)
		{
			pair<ICornerPtr, Point2D> cornerPtPair = cornerPtPairs.at(i);
			if (rect.ContainPt(cornerPtPair.second))
				selCornerPtPairs.push_back(cornerPtPairs[i]);
		}
	}

	return selCornerPtPairs;
}

END_CUTLEADER_NAMESPACE()

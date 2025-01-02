#include "StdAfx.h"
#include "StartCutPtPreSelectAction.h"

#include "MathEx.h"
#include "Rect2D.h"
#include "Point2DList.h"
#include "GlViewPort.h"
#include "CursorDrawer.h"
#include "ValueInputManager.h"

#include "clGeometryFeatureResource.h"

#include "clCutFeatureResource.h"
#include "LoopStartCutDataList.h"
#include "LoopCutFeatureList.h"
#include "LoopCutFeatureDrawer.h"

#include "DisplayParam.h"
#include "CLOptionInfo.h"

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

StartCutPtPreSelectAction::StartCutPtPreSelectAction(PatternListPtr pPatList, PatternLoopListPtr pPatternLoopList, LoopTopologyItemListPtr pLoopTopologyItems, 
	LoopCutFeatureListPtr pLoopFeatureList, GlViewPortPtr pViewPort)
    : CutFeatureEditBase(pPatList, pPatternLoopList, pLoopTopologyItems, pLoopFeatureList, pViewPort)
{
    m_preSelectState = PRE_SELECT_NO_INPUT;
    m_iMsgID = IDS_CUTFEATURE_SELECT_STARTCUT;
}

StartCutPtPreSelectAction::~StartCutPtPreSelectAction(void)
{
}

BOOL StartCutPtPreSelectAction::LButtonDown(double& dPtX, double& dPtY)
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

BOOL StartCutPtPreSelectAction::LButtonUp(double& dPtX, double& dPtY)
{
	if (m_preSelectState == PRE_SELECT_LEFT_BTN_DOWN)
	{
		m_data[2] = dPtX;
		m_data[3] = dPtY;

		// 确定选中的“回路起切”特征。
		vector< pair<LoopStartCutDataPtr, Point2D> > selStaEndPtPairs = CalcSelLoopStaEndCut();
		if (selStaEndPtPairs.size() > 0)
		{
			// 要是没有按下ctrl键，要清除之前的选择。
			if (!ValueInputManager::GetInstance()->IsInputCtrl())
				m_selStaEndPtPairs.clear();

			// 更新选中的“回路起切”信息。
			m_selStaEndPtPairs.insert(m_selStaEndPtPairs.end(), selStaEndPtPairs.begin(), selStaEndPtPairs.end());
		}

		// 确定提示信息。
		if (m_selStaEndPtPairs.size() == 0)
			m_iMsgID = IDS_CUTFEATURE_NO_STARTPT_SELECT;
		else
			m_iMsgID = IDS_CUTFEATURE_STARTPT_SELECTED;

		// 当前的鼠标状态。
		m_preSelectState = PRE_SELECT_LEFT_BTN_UP;
	}

	return TRUE;
}

BOOL StartCutPtPreSelectAction::MovePoint(double& dPtX, double& dPtY)
{
	Point2D pt(dPtX, dPtY);

	// 得到离光标最近的起切点。
	LoopStartCutDataListPtr pLoopStaEndCutList = m_pLoopFeatureList->GetLoopStaEndCutList();
	Point2DListPtr pStartPtList = pLoopStaEndCutList->GetAllStartCutPts();
	if (pStartPtList->size() > 0)
		m_pActiveStartPt.reset(new Point2D(pStartPtList->GetPtClosestTo(pt)));
	else
		m_pActiveStartPt.reset();

	// if left button down, we should draw a rectangle in stipple mode.
	if (m_preSelectState == PRE_SELECT_LEFT_BTN_DOWN)
	{
		m_data[2] = dPtX;
		m_data[3] = dPtY;
	}

	return TRUE;
}

BOOL StartCutPtPreSelectAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CutFeatureEditBase::OnKeyDown(nChar, nRepCnt, nFlags);

	// 选中全部“回路起切特征”。
	if (ValueInputManager::GetInstance()->IsInputCtrl() && ValueInputManager::GetInstance()->IsInputA(nChar))
	{
		m_selStaEndPtPairs.clear();
		LoopStartCutDataListPtr pLoopStaEndCutList = m_pLoopFeatureList->GetLoopStaEndCutList();
		vector< pair<LoopStartCutDataPtr, Point2D> > staEndPtPairs = pLoopStaEndCutList->GetAllStartCutPtPairs();
		m_selStaEndPtPairs.insert(m_selStaEndPtPairs.end(), staEndPtPairs.begin(), staEndPtPairs.end());
	}

	return TRUE;
}

BOOL StartCutPtPreSelectAction::DrawBeforeFinish()
{
	// prepare some data.
	CLOptionInfoPtr pCLOptionInfo = CLOptionInfo::GetInstance();
	DisplayParamPtr pColorInfo = pCLOptionInfo->GetDisplayParam();
	LoopCutFeatureDrawer loopFeatureDrawer(m_pViewPort);

	// 绘制选择框。
	if (m_preSelectState == PRE_SELECT_LEFT_BTN_DOWN)
	{
		DrawSelectionBox(m_data[0], m_data[1], m_data[2], m_data[3]);
	}

	// 绘制当前微连接点。
	if (m_pActiveStartPt)
		loopFeatureDrawer.DrawFeatureLoc(*m_pActiveStartPt, *pColorInfo->GetActiveObjColor());

	// 绘制选中的微连接点。
	for (unsigned int i = 0; i < m_selStaEndPtPairs.size(); i++)
		loopFeatureDrawer.DrawFeatureLoc(m_selStaEndPtPairs.at(i).second, *pColorInfo->GetSelectObjColor());

	// 绘制光标。
	CursorDrawer::DrawAimSelCursor(m_pViewPort);

	return TRUE;
}

vector< pair<LoopStartCutDataPtr, Point2D> > StartCutPtPreSelectAction::CalcSelLoopStaEndCut()
{
	vector< pair<LoopStartCutDataPtr, Point2D> > selStaEndPtPairs;

	// 得到所有“回路起切特征”和“起切点”配对。
	LoopStartCutDataListPtr pLoopStaEndCutList = m_pLoopFeatureList->GetLoopStaEndCutList();
	vector< pair<LoopStartCutDataPtr, Point2D> > staEndPtPairs = pLoopStaEndCutList->GetAllStartCutPtPairs();

	if (MathEx::Equal(m_data[0], m_data[2]) && MathEx::Equal(m_data[1], m_data[3]))
	{
		Point2D pt(m_data[0], m_data[1]);

		// 得到离这个点最近的微连接特征。
		double dDis = INFINITE_VAL;
		int iIndex = INVALID_IDX;
		for (unsigned int i = 0; i < staEndPtPairs.size(); i++)
		{
			pair<LoopStartCutDataPtr, Point2D> staEndPtPair = staEndPtPairs.at(i);
			double dTmpDis = pt.DistanceTo(staEndPtPair.second);
			if (dTmpDis < dDis)
			{
				dDis = dTmpDis;
				iIndex = i;
			}
		}
		if (iIndex != INVALID_IDX)
			selStaEndPtPairs.push_back(staEndPtPairs[iIndex]);
	}
	else
	{
		Rect2D rect;
		rect.SetXMin(m_data[0] < m_data[2] ? m_data[0] : m_data[2]);
		rect.SetXMax(m_data[0] < m_data[2] ? m_data[2] : m_data[0]);
		rect.SetYMin(m_data[1] < m_data[3] ? m_data[1] : m_data[3]);
		rect.SetYMax(m_data[1] < m_data[3] ? m_data[3] : m_data[1]);

		// 得到在选择框中的“回路起切”特征。
		for (unsigned int i = 0; i < staEndPtPairs.size(); i++)
		{
			pair<LoopStartCutDataPtr, Point2D> staEndPtPair = staEndPtPairs.at(i);
			if (rect.ContainPt(staEndPtPair.second))
				selStaEndPtPairs.push_back(staEndPtPairs[i]);
		}
	}

	return selStaEndPtPairs;
}

END_CUTLEADER_NAMESPACE()

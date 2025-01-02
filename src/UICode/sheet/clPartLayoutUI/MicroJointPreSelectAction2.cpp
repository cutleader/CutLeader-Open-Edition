#include "StdAfx.h"
#include "MicroJointPreSelectAction2.h"

#include "MathEx.h"
#include "Rect2D.h"
#include "Point2DList.h"
#include "GlViewPort.h"
#include "CursorDrawer.h"
#include "ValueInputManager.h"

#include "clGeometryFeatureResource.h"

#include "clCutFeatureResource.h"
#include "LoopCutFeatureDrawer.h"
#include "MicroJointList.h"
#include "LoopMicroJointList.h"
#include "LoopCutFeatureList.h"

#include "LoopInstanceList.h"

#include "DisplayParam.h"
#include "CLOptionInfo.h"

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

MicroJointPreSelectAction2::MicroJointPreSelectAction2(IMaterialSizePtr pMaterialSize, PartPlacementListPtr pPartPlacementList, PartInstanceListPtr pPartInstanceList, PartGroupListPtr pPartGroups, GlViewPortPtr pViewPort)
    : PartPlacementEditAction(pMaterialSize, pPartPlacementList, pPartInstanceList, pPartGroups, pViewPort)
{
	m_preSelectState = PRE_SELECT_NO_INPUT;
	m_iMsgID = IDS_CUTFEATURE_SELECT_MicroJoint;
}

MicroJointPreSelectAction2::~MicroJointPreSelectAction2(void)
{
}

BOOL MicroJointPreSelectAction2::LButtonDown(double& dPtX, double& dPtY)
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

BOOL MicroJointPreSelectAction2::LButtonUp(double& dPtX, double& dPtY)
{
	if (m_preSelectState == PRE_SELECT_LEFT_BTN_DOWN)
	{
		m_data[2] = dPtX;
		m_data[3] = dPtY;

		// 确定选中的微连接特征。
		vector< pair<MicroJointPtr, Point2D> > selMJPtPairs = CalcSelectMJPt();
		if (selMJPtPairs.size() > 0)
		{
			// 要是没有按下ctrl键，要清除之前的选择。
			if (!ValueInputManager::GetInstance()->IsInputCtrl())
				m_selMjPtPairs.clear();

			// 更新选中的微连接信息。
			m_selMjPtPairs.insert(m_selMjPtPairs.end(), selMJPtPairs.begin(), selMJPtPairs.end());
		}

		// 确定提示信息。
		if (m_selMjPtPairs.size() == 0)
			m_iMsgID = IDS_CUTFEATURE_NO_MJ_SELECT;
		else
			m_iMsgID = IDS_CUTFEATURE_MJ_SELECTED;

		// 当前的鼠标状态。
		m_preSelectState = PRE_SELECT_LEFT_BTN_UP;
	}

	return TRUE;
}

BOOL MicroJointPreSelectAction2::MovePoint(double& dPtX, double& dPtY)
{
	Point2D pt(dPtX, dPtY);

	// 得到离光标最近的微连接点。
	Point2DListPtr pMJPtList = m_pAllLoopInstanceList->GetAllMicroJointPts();
	if (pMJPtList->size() > 0)
		m_pActiveMJPt.reset(new Point2D(pMJPtList->GetPtClosestTo(pt)));
	else
		m_pActiveMJPt.reset();

	// if left button down, we should draw a rectangle in stipple mode.
	if (m_preSelectState == PRE_SELECT_LEFT_BTN_DOWN)
	{
		m_data[2] = dPtX;
		m_data[3] = dPtY;
	}

	return TRUE;
}

BOOL MicroJointPreSelectAction2::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	PartPlacementEditAction::OnKeyDown(nChar, nRepCnt, nFlags);

	// 选中全部“微连接特征”。
	if (ValueInputManager::GetInstance()->IsInputCtrl() && ValueInputManager::GetInstance()->IsInputA(nChar))
	{
		m_selMjPtPairs.clear();
		vector< pair<MicroJointPtr, Point2D> > mjPtPairs = m_pAllLoopInstanceList->GetAllMicroJointPtPairs();
		m_selMjPtPairs.insert(m_selMjPtPairs.end(), mjPtPairs.begin(), mjPtPairs.end());
	}

	return TRUE;
}

BOOL MicroJointPreSelectAction2::DrawBeforeFinish()
{
	CLOptionInfoPtr pCLOptionInfo = CLOptionInfo::GetInstance();
	DisplayParamPtr pColorInfo = pCLOptionInfo->GetDisplayParam();
	LoopCutFeatureDrawer loopFeatureDrawer(m_pViewPort);

	// 绘制选择框。
	if (m_preSelectState == PRE_SELECT_LEFT_BTN_DOWN) {
		DrawSelectionBox(m_data[0], m_data[1], m_data[2], m_data[3]);
	}

	// 绘制当前微连接点。
	if (m_pActiveMJPt)
		loopFeatureDrawer.DrawFeatureLoc(*m_pActiveMJPt, *pColorInfo->GetActiveObjColor());

	// 绘制选中的微连接点。
	for (unsigned int i = 0; i < m_selMjPtPairs.size(); i++)
		loopFeatureDrawer.DrawFeatureLoc(m_selMjPtPairs.at(i).second, *pColorInfo->GetSelectObjColor());

	// 绘制光标。
	CursorDrawer::DrawAimSelCursor(m_pViewPort);

	return TRUE;
}

MicroJointListPtr MicroJointPreSelectAction2::GetSelectMJList()
{
	MicroJointListPtr pMicroJointList(new MicroJointList);

	for (unsigned int i = 0; i < m_selMjPtPairs.size(); i++)
		pMicroJointList->push_back(m_selMjPtPairs.at(i).first);

	return pMicroJointList;
}

vector< pair<MicroJointPtr, Point2D> > MicroJointPreSelectAction2::CalcSelectMJPt()
{
	vector< pair<MicroJointPtr, Point2D> > selMjPtPairs;

	// 得到所有“微连接点”对应的“微连接特征”。
	vector< pair<MicroJointPtr, Point2D> > mjPtPairs = m_pAllLoopInstanceList->GetAllMicroJointPtPairs();

	if (MathEx::Equal(m_data[0], m_data[2]) && MathEx::Equal(m_data[1], m_data[3]))
	{
		Point2D pt(m_data[0], m_data[1]);

		// 得到离这个点最近的微连接特征。
		double dDis = INFINITE_VAL;
		int iIndex = INVALID_IDX;
		for (unsigned int i = 0; i < mjPtPairs.size(); i++)
		{
			pair<MicroJointPtr, Point2D> mjPtPair = mjPtPairs.at(i);
			double dTmpDis = pt.DistanceTo(mjPtPair.second);
			if (dTmpDis < dDis)
			{
				dDis = dTmpDis;
				iIndex = i;
			}
		}
		if (iIndex != INVALID_IDX)
			selMjPtPairs.push_back(mjPtPairs[iIndex]);
	}
	else
	{
		Rect2D rect;
		rect.SetXMin(m_data[0] < m_data[2] ? m_data[0] : m_data[2]);
		rect.SetXMax(m_data[0] < m_data[2] ? m_data[2] : m_data[0]);
		rect.SetYMin(m_data[1] < m_data[3] ? m_data[1] : m_data[3]);
		rect.SetYMax(m_data[1] < m_data[3] ? m_data[3] : m_data[1]);

		// 得到在选择框中的微连接特征。
		for (unsigned int i = 0; i < mjPtPairs.size(); i++)
		{
			pair<MicroJointPtr, Point2D> mjPtPair = mjPtPairs.at(i);
			if (rect.ContainPt(mjPtPair.second))
				selMjPtPairs.push_back(mjPtPairs[i]);
		}
	}

	return selMjPtPairs;
}

END_CUTLEADER_NAMESPACE()

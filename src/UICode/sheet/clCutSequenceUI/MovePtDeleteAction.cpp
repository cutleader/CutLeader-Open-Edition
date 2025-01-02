#include "StdAfx.h"
#include "MovePtDeleteAction.h"

#include "baseConst.h"
#include "GlViewPort.h"
#include "CursorDrawer.h"
#include "Point2DList.h"
#include "Line2D.h"
#include "LineArc2DList.h"
#include "ValueInputManager.h"
#include "clBaseDataResource.h"

#include "IMaterialSize.h"
#include "SequenceConfigItem.h"
#include "CLOptionInfo.h"
#include "DisplayParam.h"

#include "ICutPartSequence.h"
#include "CutSequenceList.h"
#include "CutSequenceSelectAction.h"
#include "CutNodeManager.h"
#include "clCutSequenceResource.h"
#include "TraceDataMgr.h"
#include "CutSequenceDrawer.h"
#include "ConfigData.h"
#include "SequenceData.h"

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

MovePtDeleteAction::MovePtDeleteAction(PartLayoutDataPtr pPartLayoutData, SequenceDataPtr pSequenceData, ConfigDataPtr pConfigData, GlViewPortPtr pViewPort)
	: SequenceEditActionBase(pPartLayoutData, pSequenceData, pConfigData, pViewPort)
{
	m_iMsgID = IDS_SEQ_DELETE_MOV_PT;

	// 缓存所有工序之间的快速移动点。
	const IMaterialSize* pMaterialSize = m_pConfigData->GetMatSize();
	const SequenceConfigItem* pSeqConfigItem = m_pConfigData->GetSeqConfigItem();
	Point2D startSeqPt = pMaterialSize->GetOuterRect().GetCornerPt(pSeqConfigItem->GetStartCorner());
	m_movePtData = TraceDataMgr::GetTracePtData(startSeqPt, pSequenceData->GetCutSequences());

	m_iCurrentSeq = INVALID_ID;
}

MovePtDeleteAction::~MovePtDeleteAction(void)
{
}

BOOL MovePtDeleteAction::LButtonDown(double& dPtX, double& dPtY)
{
	if (m_iCurrentSeq != INVALID_ID)
	{
		// 删除快速移动点。
		Point2DListPtr pSelMovPts = m_movePtData[m_iCurrentSeq];
		pSelMovPts->DeletePt(m_curMovPt);

		// 修改工序的快速移动点。
		ICutSequencePtr pCutSequence = m_pSequenceData->GetCutSequences()->GetCutSequenceByID(m_iCurrentSeq);
		Point2DListPtr pPoint2DList = pCutSequence->GetTracePtList();
		pPoint2DList->DeletePt(m_curMovPt);
		pCutSequence->SetModified(TRUE);

		m_iCurrentSeq = INVALID_ID;

		// 更新“切割节点”缓存。
		RefreshCutNodes();
	}
	else
	{
		CString strProductName, strMsg;
		strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);
		strMsg = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_DEL_MOVPT_NO_SEL);
		MessageBox(m_pViewPort->GetWnd(), strMsg, strProductName, MB_OK|MB_ICONWARNING);
	}

	return TRUE;
}

BOOL MovePtDeleteAction::MovePoint(double& dPtX, double& dPtY)
{
	Point2D pt(dPtX, dPtY);

	// 找到最近的快速移动线。
	FindClosestMovPt(pt);

	return TRUE;
}

BOOL MovePtDeleteAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	return __super::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL MovePtDeleteAction::DrawBeforeFinish()
{
	const DisplayParam* pDisplayParam = CLOptionInfo::GetInstance()->GetDisplayParam().get();

	// 老的设置。
	COLORREF dwOldColor;
	m_pViewPort->GetDrawColor(dwOldColor);
	int iOldLineWid = m_pViewPort->GetLineWidth();

	// 绘制当前的快速移动点。
	if (m_iCurrentSeq != INVALID_ID)
	{
		m_pViewPort->SetDrawColor(*pDisplayParam->GetActiveObjColor());
		m_pViewPort->SetLineWidth(2);
		m_pViewPort->DrawCrossPoint2DEx(m_curMovPt.X(), m_curMovPt.Y(), 12, 0);
	}

	// 恢复设置。
	m_pViewPort->SetDrawColor(dwOldColor);
	m_pViewPort->SetLineWidth(iOldLineWid);

	// draw cursor.
	CursorDrawer::DrawAimSelCursor(m_pViewPort);

	return TRUE;
}

void MovePtDeleteAction::FindClosestMovPt(const Point2D& pt)
{
	double dDis = INVALID_VALUE;
	for (map<LONGLONG, Point2DListPtr>::iterator iter = m_movePtData.begin(); iter != m_movePtData.end(); iter++)
	{
		Point2DListPtr pPtList = iter->second;
		if (pPtList->size() > 2)
		{
			for (unsigned int i = 1; i < pPtList->size()-1; i++)
			{
				if (dDis == INVALID_VALUE)
				{
					dDis = pPtList->at(i).DistanceTo(pt);
					m_iCurrentSeq = iter->first;
					m_curMovPt = pPtList->at(i);
				}
				else
				{
					Point2D tmpPt = pPtList->at(i);
					double dTmpDis = tmpPt.DistanceTo(pt);
					if (dTmpDis < dDis)
					{
						dDis = dTmpDis;
						m_iCurrentSeq = iter->first;
						m_curMovPt = tmpPt;
					}
				}
			}
		}
	}
}

END_CUTLEADER_NAMESPACE()

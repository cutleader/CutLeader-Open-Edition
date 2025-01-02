#include "StdAfx.h"
#include "RemnantCutlineAddAction.h"

#include "baseConst.h"
#include "GlViewPort.h"
#include "CursorDrawer.h"
#include "Point2DList.h"
#include "Line2D.h"
#include "Line2DList.h"
#include "LineArc2DList.h"
#include "ValueInputManager.h"
#include "clBaseDataResource.h"

#include "IMaterialSize.h"
#include "SequenceConfigItem.h"
#include "CLOptionInfo.h"
#include "DisplayParam.h"

#include "RemnantLineSeq.h"
#include "CutSequenceList.h"
#include "CutSequenceSelectAction.h"
#include "CutNodeManager.h"
#include "clCutSequenceResource.h"
#include "ConfigData.h"
#include "CutSequenceManager.h"
#include "CutSequenceDrawer.h"
#include "SequenceData.h"

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

RemnantCutlineAddAction::RemnantCutlineAddAction(PartLayoutDataPtr pPartLayoutData, SequenceDataPtr pSequenceData, ConfigDataPtr pConfigData, GlViewPortPtr pViewPort)
	: SequenceEditActionBase(pPartLayoutData, pSequenceData, pConfigData, pViewPort)
{
	m_iMsgID = IDS_SEQ_ADD_REM_CUTLINE_ACTION;
	m_pEndPts_of_remCutLine.reset(new Point2DList);
}

RemnantCutlineAddAction::~RemnantCutlineAddAction(void)
{
}

BOOL RemnantCutlineAddAction::LButtonDown(double& dPtX, double& dPtY)
{
	Point2D pt(dPtX, dPtY);
	m_pEndPts_of_remCutLine->push_back(pt);

	return TRUE;
}

BOOL RemnantCutlineAddAction::RButtonUp(double& dPtX, double& dPtY)
{
	if (m_pEndPts_of_remCutLine->size() > 1)
	{
		RemnantLineSeqPtr pRemnantLineSeq(new RemnantLineSeq);
		Line2DListPtr pLines = pRemnantLineSeq->GetCutLines();
		for (unsigned int i = 0; i < m_pEndPts_of_remCutLine->size() - 1; i++)
		{
			Point2D startPt = m_pEndPts_of_remCutLine->at(i);
			Point2D endPt = m_pEndPts_of_remCutLine->at(i+1);
			Line2DPtr pLine(new Line2D(startPt, endPt));
			pLines->push_back(pLine);
		}
		CutSequenceManager::InsertCutSeq(m_pSequenceData->GetCutSequences(), -1, pRemnantLineSeq, m_pConfigData->GetMatSize(), m_pConfigData->GetSeqConfigItem());
		m_pSequenceData->GetCutSequences()->SetModified(TRUE);
		RefreshCutNodes(); // 更新“切割节点”缓存。
	}

	// 重新开始动作
	m_pEndPts_of_remCutLine->clear();

	return TRUE;
}

BOOL RemnantCutlineAddAction::MovePoint(double& dPtX, double& dPtY)
{
	Point2D pt(dPtX, dPtY);
	m_currentPos = pt;
	return TRUE;
}

BOOL RemnantCutlineAddAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	return __super::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL RemnantCutlineAddAction::DrawBeforeFinish()
{
	const DisplayParam* pDisplayParam = CLOptionInfo::GetInstance()->GetDisplayParam().get();

	// 老的设置。
	COLORREF dwOldColor;
	m_pViewPort->GetDrawColor(dwOldColor);
	int iOldLineWid = m_pViewPort->GetLineWidth();

	// 设置绘制样式
	m_pViewPort->SetDrawColor(*pDisplayParam->GetSequenceColor());
	m_pViewPort->SetLineWidth(1);

	// 绘制
	if (m_pEndPts_of_remCutLine->size() > 1)
	{
		for (unsigned int i = 0; i < m_pEndPts_of_remCutLine->size() - 1; i++)
		{
			Point2D startPt = m_pEndPts_of_remCutLine->at(i);
			Point2D endPt = m_pEndPts_of_remCutLine->at(i+1);
			m_pViewPort->DrawLinePoint2D(startPt.X(), startPt.Y(), endPt.X(), endPt.Y());
		}
	}
	if (m_pEndPts_of_remCutLine->size() > 0)
	{
		Point2D startPt = m_pEndPts_of_remCutLine->back();
		Point2D endPt = m_currentPos;
		m_pViewPort->DrawLinePoint2D(startPt.X(), startPt.Y(), m_currentPos.X(), m_currentPos.Y());
	}

	// 恢复设置。
	m_pViewPort->SetDrawColor(dwOldColor);
	m_pViewPort->SetLineWidth(iOldLineWid);

	// draw cursor.
	CursorDrawer::DrawCrossLineCursor(m_pViewPort);

	return TRUE;
}

END_CUTLEADER_NAMESPACE()

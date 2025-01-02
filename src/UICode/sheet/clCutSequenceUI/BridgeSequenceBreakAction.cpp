#include "StdAfx.h"
#include "BridgeSequenceBreakAction.h"

#include "baseConst.h"
#include "GlViewPort.h"
#include "CommandManager.h"
#include "CursorDrawer.h"
#include "clBaseDataResource.h"
#include "LineArc2DList.h"
#include "BasicGeometryDrawer.h"
#include "ValueInputManager.h"

#include "SequenceConfigItem.h"
#include "IMaterialSize.h"
#include "CLOptionInfo.h"
#include "DisplayParam.h"

#include "LoopInstanceList.h"
#include "clCutSequenceResource.h"
#include "CutNodeList.h"
#include "CutSequenceList.h"
#include "CutSequenceDrawer.h"
#include "CutNodeManager.h"
#include "BridgeSequence.h"
#include "BridgeSequenceBreakCommand.h"
#include "BridgeSequenceManager.h"
#include "PartLayoutData.h"
#include "CutSequenceSelectAction.h"
#include "SequenceData.h"

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

BridgeSequenceBreakAction::BridgeSequenceBreakAction(PartLayoutDataPtr pPartLayoutData, SequenceDataPtr pSequenceData, ConfigDataPtr pConfigData, GlViewPortPtr pViewPort)
	: SequenceEditActionBase(pPartLayoutData, pSequenceData, pConfigData, pViewPort)
{
	m_iMsgID = IDS_SEQ_BREAK_BDGE;

	m_pCurBdgeGeom.reset();
	m_pCurBdgeNode.reset();
}

BridgeSequenceBreakAction::~BridgeSequenceBreakAction(void)
{
}

BOOL BridgeSequenceBreakAction::LButtonDown(double& dPtX, double& dPtY)
{
	if (m_pCurBdgeNode)
	{
		// 得到对应的“桥接工序”。
		BridgeSequencePtr pModBdgeSeqUnit;
		CutSequenceListPtr pBdgeSeqUnits = m_pSequenceData->GetCutSequences()->GetCutSequencesByType(CutSequence_Bridge);
		for (unsigned int i = 0; i < pBdgeSeqUnits->size(); i++)
		{
			BridgeSequencePtr pBridgeSequence = boost::dynamic_pointer_cast<BridgeSequence>(pBdgeSeqUnits->at(i));
			if (pBridgeSequence->IncludeBdgeNode(m_pCurBdgeNode))
			{
				pModBdgeSeqUnit = pBridgeSequence;
				break;
			}
		}

		// 在“入口桥接点”处断开“桥接节点”。
		ICommandPtr pCommand(new BridgeSequenceBreakCommand(GetEditorData(), m_pSequenceData->GetCutSequences(), m_pConfigData,
											    pModBdgeSeqUnit, m_pCurBdgeNode));
		pCommand->Do();
		CommandManager::GetInstance()->AddCommand(pCommand);

		// 更新“切割节点”缓存。
		RefreshCutNodes();

		// 更新当前的“入口桥”基本图形及对应的“桥接节点”。
		m_pCurBdgeGeom.reset();
		m_pCurBdgeNode.reset();
	}
	else
	{
		CString strProductName, strMsg;
		strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);
		strMsg = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_BREAK_BDGE_NO_SEL);
		MessageBox(m_pViewPort->GetWnd(), strMsg, strProductName, MB_OK|MB_ICONWARNING);
	}

	return TRUE;
}

BOOL BridgeSequenceBreakAction::MovePoint(double& dPtX, double& dPtY)
{
	// 计算距离光标最近的“入口桥”基本图形及对应的“桥接节点”。
	Point2D pt(dPtX, dPtY);
	CalcCurBdgeGeomInfo(pt);

	return TRUE;
}

BOOL BridgeSequenceBreakAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	SequenceEditActionBase::OnKeyDown(nChar, nRepCnt, nFlags);

	if (ValueInputManager::GetInstance()->IsInputCancel(nChar))
	{
		// figure out the next action.
		m_pNextAction.reset(new CutSequenceSelectAction(m_pPartLayoutData, m_pSequenceData, m_pConfigData, m_pViewPort));
	}

	return TRUE;
}

BOOL BridgeSequenceBreakAction::DrawBeforeFinish()
{
	CLOptionInfoPtr pCLOptionInfo = CLOptionInfo::GetInstance();
	DisplayParamPtr pDisplayParam = pCLOptionInfo->GetDisplayParam();
	BasicGeometryDrawer geomBaseDrawer(m_pViewPort);
	CutSequenceDrawer cutSequenceDrawer(m_pViewPort);

	// 老的设置。
	COLORREF dwOldColor;
	m_pViewPort->GetDrawColor(dwOldColor);

	// 绘制已有的“桥接工序”，此时不考虑偏移。
	m_pViewPort->SetDrawColor(*pDisplayParam->GetSequenceColor());
	CutSequenceListPtr pBdgeSeqUnits = m_pSequenceData->GetCutSequences()->GetCutSequencesByType(CutSequence_Bridge);
	cutSequenceDrawer.DrawCutSequences(pBdgeSeqUnits.get(), m_pCutNodeList.get());

	// 绘制当前“桥”。
	if (m_pCurBdgeGeom) {
		m_pViewPort->SetDrawColor(*pDisplayParam->GetActiveObjColor());
		geomBaseDrawer.Draw2DLineArcs(m_pCurBdgeGeom.get());
	}

	// 恢复设置。
	m_pViewPort->SetDrawColor(dwOldColor);

	// draw cursor.
	CursorDrawer::DrawAimSelCursor(m_pViewPort);

	return TRUE;
}

void BridgeSequenceBreakAction::UpdateForUndoRedo()
{
	// 更新“切割节点”缓存。
	RefreshCutNodes();
}

void BridgeSequenceBreakAction::CalcCurBdgeGeomInfo(const Point2D& pt)
{
	// 得到所有“桥接节点”及它的“入口桥”的基本图形。
	vector<pair<BridgeNodePtr, LineArc2DListPtr> > bdgeGeomInfos = BridgeSequenceManager::GetAllBdgeGeomInfo(m_pSequenceData->GetCutSequences());

	// 得到离这个点最近的“桥基本图形”。
	double dDis = INFINITE_VAL;
	int iIndex = INVALID_IDX;
	for (unsigned int i = 0; i < bdgeGeomInfos.size(); i++)
	{
		pair<BridgeNodePtr, LineArc2DListPtr> bdgeGeomInfo = bdgeGeomInfos.at(i);
		double dTmpDis = bdgeGeomInfo.second->GetClosestDis(pt);
		if (dTmpDis < dDis)
		{
			dDis = dTmpDis;
			iIndex = i;
		}
	}

	if (iIndex != INVALID_IDX)
	{
		m_pCurBdgeGeom = bdgeGeomInfos[iIndex].second;
		m_pCurBdgeNode = bdgeGeomInfos[iIndex].first;
	}
}

END_CUTLEADER_NAMESPACE()

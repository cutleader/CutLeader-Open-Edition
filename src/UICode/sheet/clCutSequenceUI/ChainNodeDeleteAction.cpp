#include "StdAfx.h"
#include "ChainNodeDeleteAction.h"

#include "baseConst.h"
#include "GlViewPort.h"
#include "CommandManager.h"
#include "CursorDrawer.h"
#include "clBaseDataResource.h"
#include "ValueInputManager.h"
#include "Point2DList.h"

#include "SequenceConfigItem.h"
#include "IMaterialSize.h"
#include "CLOptionInfo.h"
#include "DisplayParam.h"
#include "SheetEditParam.h"

#include "IArrayPattern.h"
#include "IPatternLoop.h"
#include "PatternLoopNode.h"
#include "PatternLoopNodeList.h"
#include "LoopDrawer.h"
#include "LoopCutFeatureDrawer.h"
#include "PartPlacementList.h"
#include "PartInstance.h"
#include "PartInstanceList.h"
#include "LoopInstanceList.h"
#include "LoopInstanceCache.h"
#include "LoopInstance.h"
#include "PartPlacementManager.h"
#include "LoopInstanceManager.h"
#include "clCutSequenceResource.h"
#include "CutNodeList.h"
#include "PtChainNode.h"
#include "LoopChainNode.h"
#include "ChainNodeList.h"
#include "ChainCutSequence.h"
#include "CutSequenceList.h"
#include "ChainNodeDeleteCommand.h"
#include "CutSequenceDrawer.h"
#include "PartLayoutData.h"
#include "CutNodeManager.h"
#include "ChainCutSequenceManager.h"
#include "CutSequenceSelectAction.h"
#include "SequenceData.h"

BEGIN_CUTLEADER_NAMESPACE()

ChainNodeDeleteAction::ChainNodeDeleteAction(PartLayoutDataPtr pPartLayoutData, SequenceDataPtr pSequenceData, 
	ConfigDataPtr pConfigData, GlViewPortPtr pViewPort)
	: SequenceEditActionBase(pPartLayoutData, pSequenceData, pConfigData, pViewPort)
{
	m_iMsgID = IDS_SEQ_CHAIN_DEL_NODE;
	m_pActiveChainNode.reset();
}

ChainNodeDeleteAction::~ChainNodeDeleteAction(void)
{
}

BOOL ChainNodeDeleteAction::LButtonDown(double& dPtX, double& dPtY)
{
	CString strProductName, strMsg;
	strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);

	if (m_pActiveChainNode)
	{
		// 检查能不能删除这个节点。
		ChainCutSequencePtr pChainSeq = ChainCutSequenceManager::SearchChainSeq(m_pSequenceData->GetCutSequences(), m_pActiveChainNode);
		if (m_pActiveChainNode->GetChainNodeType() == CHAIN_NODE_LOOP)
		{
			LoopChainNodePtr pChainLoopNode = boost::dynamic_pointer_cast<LoopChainNode>(m_pActiveChainNode);
			if (pChainSeq->GetLoopInstanceList()->size() == 1)
			{
				strMsg = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_CHAIN_DEL_LAST_LOOP);
				MessageBox(m_pViewPort->GetWnd(), strMsg, strProductName, MB_OK|MB_ICONWARNING);
				return FALSE;
			}
		}

		// 执行命令。
		ICommandPtr pCommand(new ChainNodeDeleteCommand(GetEditorData(), m_pSequenceData->GetCutSequences(), m_pConfigData,
			pChainSeq, m_pActiveChainNode));
		pCommand->Do();
		CommandManager::GetInstance()->AddCommand(pCommand);

		// 更新“切割节点”缓存。
		RefreshCutNodes();

		// 更新当前节点。
		m_pActiveChainNode.reset();
	}
	else
	{
		strMsg = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_CHAIN_DEL_NO_NODE_SEL);
		MessageBox(m_pViewPort->GetWnd(), strMsg, strProductName, MB_OK|MB_ICONWARNING);
	}

	return TRUE;
}

BOOL ChainNodeDeleteAction::MovePoint(double& dPtX, double& dPtY)
{
	Point2D pt(dPtX, dPtY);

	// 得到最近的连割节点。
	ChainNodeListPtr pChainNodes = ChainCutSequenceManager::GetAllChainNode(m_pSequenceData->GetCutSequences());
	m_pActiveChainNode = pChainNodes->GetClosestNode(pt);

	return TRUE;
}

BOOL ChainNodeDeleteAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	SequenceEditActionBase::OnKeyDown(nChar, nRepCnt, nFlags);

	if (ValueInputManager::GetInstance()->IsInputCancel(nChar))
	{
		// figure out the next action.
		m_pNextAction.reset(new CutSequenceSelectAction(m_pPartLayoutData, m_pSequenceData, m_pConfigData, m_pViewPort));
	}

	return TRUE;
}

BOOL ChainNodeDeleteAction::DrawBeforeFinish()
{
	CutSequenceDrawer cutSeqDrawer(m_pViewPort);

	// 老的设置。
	COLORREF dwOldColor;
	m_pViewPort->GetDrawColor(dwOldColor);
	int iOldLineWid = m_pViewPort->GetLineWidth();

	// 绘制当前的“连割节点”。
	if (m_pActiveChainNode)
	{
		cutSeqDrawer.DrawActiveChainNode(m_pActiveChainNode.get());
	}

	// 恢复设置。
	m_pViewPort->SetDrawColor(dwOldColor);
	m_pViewPort->SetLineWidth(iOldLineWid);

	// draw cursor.
	CursorDrawer::DrawAimSelCursor(m_pViewPort);

	return TRUE;
}

void ChainNodeDeleteAction::UpdateForUndoRedo()
{
	// 更新“切割节点”缓存。
	RefreshCutNodes();
}

END_CUTLEADER_NAMESPACE()

#include "StdAfx.h"
#include "NCSimulateAction.h"

#include "GlViewPort.h"
#include "Line2D.h"
#include "Rect2D.h"
#include "DummyAction.h"
#include "CursorDrawer.h"
#include "ValueInputManager.h"

#include "CLOptionInfo.h"
#include "DisplayParam.h"
#include "IMaterialSize.h"

#include "CutNodeList.h"
#include "CutNodeManager.h"
#include "CutSequenceDrawer.h"
#include "clPostProcessorResource.h"
#include "PartPlacementEditAction.h"
#include "CanvasGeometryEditAction.h"

BEGIN_CUTLEADER_NAMESPACE()

NCSimulateAction::NCSimulateAction(IMaterialSizePtr pMaterialSize, GlViewPortPtr pViewPort)
{
	m_pViewPort = pViewPort;
	m_iMsgID = IDS_MSG_SIM_NC_ROW;
    m_pWrongToolSizeLoopInstances.reset(new WrongToolSizeLoopInstances);
	m_pMaterialSize = pMaterialSize;
}

NCSimulateAction::~NCSimulateAction(void)
{
}

BOOL NCSimulateAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (ValueInputManager::GetInstance()->IsInputCancel(nChar))
	{
		if (m_pMaterialSize->GetType() == MAT_SIZE_Canvas)
			CanvasGeometryEditAction::PostMessage2EnterCanvasLoopSelectAction();
		else
			PartPlacementEditAction::PostMessage2EnterPartPlacementSelectAction();
	}

	return ActionBase::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL NCSimulateAction::DrawBeforeFinish()
{
    const DisplayParam* pDisplayParam = CLOptionInfo::GetInstance()->GetDisplayParam().get();
    CutSequenceDrawer cutSequenceDrawer(m_pViewPort);

	// 老的设置。
	DWORD dwOldColor;
	m_pViewPort->GetDrawColor(dwOldColor);
	int iOldLineWid = m_pViewPort->GetLineWidth();

	// 把选中的NC代码行对应的“切割节点”画出来。
	m_pViewPort->SetDrawColor(*pDisplayParam->GetSeqSimColor());
	m_pViewPort->SetLineWidth(2);
	for (unsigned int i = 0; i < m_pCutNodeList->size(); i++)
	{
		const ICutNode* pCutNode = m_pCutNodeList->at(i).get();
		SequenceSimulateShapeListPtr pSimShapeList = CutNodeManager::CutNode2SimShape(pCutNode);
		cutSequenceDrawer.DrawSequenceSimulateShapes(pSimShapeList.get(), false);
	}

	// 恢复绘图设置。
	m_pViewPort->SetDrawColor(dwOldColor);
	m_pViewPort->SetLineWidth(iOldLineWid);

    // 绘制偏移有问题的轮廓实例。
    if (pDisplayParam->GetShowIncorrectToolSize())
    {
        m_pWrongToolSizeLoopInstances->Draw(m_pViewPort);
    }

	// draw cursor.
	CursorDrawer::DrawDefaultCursor(m_pViewPort);

	return TRUE;
}

void NCSimulateAction::ViewCutNode(SequenceSimulateShapeListPtr pSeqSimShapeList, CutNodeListPtr pCutNodeList)
{
	m_pSequenceSimulateShapeList = pSeqSimShapeList;
	m_pCutNodeList = pCutNodeList;
}

END_CUTLEADER_NAMESPACE()

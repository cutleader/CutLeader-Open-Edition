#include "StdAfx.h"
#include "FlyCutSequenceAddAction.h"

#include "baseConst.h"
#include "MathEx.h"
#include "IntegerList.h"
#include "GlViewPort.h"
#include "CommandManager.h"
#include "CursorDrawer.h"
#include "clBaseDataResource.h"
#include "ValueInputManager.h"
#include "Point2DList.h"
#include "Line2D.h"
#include "Polygon2D.h"
#include "LogMgr.h"

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
#include "PartItem.h"
#include "PartPlacementList.h"
#include "PartInstance.h"
#include "PartInstanceList.h"
#include "LoopInstanceList.h"
#include "LoopInstanceCache.h"
#include "LoopInstance.h"
#include "PartPlacementManager.h"
#include "PartPlacementDrawer.h"
#include "LoopInstanceManager.h"
#include "clCutSequenceResource.h"
#include "FlyCutSequence.h"
#include "FlyCutSequenceAddCommand.h"
#include "CutNodeList.h"
#include "CutSequenceList.h"
#include "CutSequenceDrawer.h"
#include "PartLayoutData.h"
#include "CutNodeManager.h"
#include "CutSequenceSelectAction.h"
#include "ConfigData.h"
#include "FlyCutSequenceManager.h"
#include "CanvasGeometryEditAction.h"
#include "PartPlacementEditAction.h"
#include "LoopToolManager.h"
#include "SequenceHelper.h"
#include "SequenceData.h"

BEGIN_CUTLEADER_NAMESPACE()

FlyCutSequenceAddAction::FlyCutSequenceAddAction(PartLayoutDataPtr pPartLayoutData, SequenceDataPtr pSequenceData, ConfigDataPtr pConfigData, GlViewPortPtr pViewPort)
	: SequenceEditActionBase(pPartLayoutData, pSequenceData, pConfigData, pViewPort)
{
	m_emState = NONE;
	m_iMsgID = IDS_SEQ_AddFlyCut_TIP;
	m_pRectSelectLoopInstances.reset(new LoopInstanceList);

	// 得到所有“回路实例”。
	m_pAllLoopInstances = LoopInstanceManager::GetLoopInstanceList(m_pPartLayoutData->GetPartInstances());
}

FlyCutSequenceAddAction::~FlyCutSequenceAddAction(void)
{
}

BOOL FlyCutSequenceAddAction::LButtonDown(double& dPtX, double& dPtY)
{
	if (m_emState == NONE || m_emState == FLYCUT_SEQ_SECOND_POINT)
	{
		m_emState = FLYCUT_SEQ_FIRST_POINT;

		m_data[0] = dPtX;
		m_data[1] = dPtY;
		m_data[2] = dPtX;
		m_data[3] = dPtY;
	}

	return TRUE;
}

BOOL FlyCutSequenceAddAction::LButtonUp(double& dPtX, double& dPtY)
{
	CString strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);

	if (m_emState == FLYCUT_SEQ_FIRST_POINT)
	{
		m_emState = FLYCUT_SEQ_SECOND_POINT;

		// 得到选中的轮廓实例。
		LoopInstanceListPtr pSelectedLoopInsts(new LoopInstanceList);
		if (m_pRectSelectLoopInstances->size() > 0) {
			pSelectedLoopInsts->insert(pSelectedLoopInsts->end(), m_pRectSelectLoopInstances->begin(), m_pRectSelectLoopInstances->end());
		}
		else {
			if (m_pActiveLoopInstance) {
				pSelectedLoopInsts->push_back(m_pActiveLoopInstance);
			}
			else {
				MessageBox(m_pViewPort->GetWnd(), MultiLanguageMgr::GetInstance()->TranslateString(IDS_FLYCUT_SEQ_NO_HOLE_SEL_TIP), strProductName, MB_OK|MB_ICONWARNING);
				return TRUE;
			}
		}
		// 能运行到这里，肯定有轮廓被选中了。

		// 基于选中的轮廓创建飞切工序，此处没有判断返回值。
		const EditorData& editorData = GetEditorData();
		const CWnd* pView = editorData.GetView();
		const CString& strEditorName = editorData.GetEditorName();
		PartInstanceListPtr pPartInstances = m_pPartLayoutData->GetPartInstances();
		SequenceHelper::CreateFlyCutSequence(pView, strEditorName, pSelectedLoopInsts, pPartInstances, m_pSequenceData->GetCutSequences(), m_pConfigData);

		// 进入轮廓选择action。
		if (m_pConfigData->GetMatSize()->GetType() == MAT_SIZE_Canvas) {
			CanvasGeometryEditAction::PostMessage2EnterCanvasLoopSelectAction_PreSelectLoopAndText(pSelectedLoopInsts.get());
		}
		else {
			PartPlacementEditAction::PostMessage2EnterLoopInstanceSelectAction(pSelectedLoopInsts.get());
		}
	}

	return TRUE;
}

BOOL FlyCutSequenceAddAction::MovePoint(double& dPtX, double& dPtY)
{
	Point2D pt(dPtX, dPtY);

	if (m_emState == FLYCUT_SEQ_FIRST_POINT)
	{
		m_data[2] = dPtX;
		m_data[3] = dPtY;
		Rect2D rect(m_data[0] < m_data[2] ? m_data[0] : m_data[2], m_data[0] < m_data[2] ? m_data[2] : m_data[0],
			m_data[1] < m_data[3] ? m_data[1] : m_data[3], m_data[1] < m_data[3] ? m_data[3] : m_data[1]);
		m_pRectSelectLoopInstances = LoopInstanceManager::GetLoopInstsInRect(m_pAllLoopInstances, rect);
		m_pActiveLoopInstance.reset();
	}
	else {
		m_pActiveLoopInstance = LoopInstanceManager::GetClosestLoopInstEx(m_pAllLoopInstances, pt);
		m_pRectSelectLoopInstances->clear();
	}

	return TRUE;
}

BOOL FlyCutSequenceAddAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	SequenceEditActionBase::OnKeyDown(nChar, nRepCnt, nFlags);

	return TRUE;
}

BOOL FlyCutSequenceAddAction::DrawBeforeFinish()
{
	const DisplayParam* pDisplayParam = CLOptionInfo::GetInstance()->GetDisplayParam().get();
	LoopDrawer loopDrawer(m_pViewPort);

	// 绘制高亮的轮廓实例。
	if (m_emState == FLYCUT_SEQ_FIRST_POINT) // 框选。
	{
		// 绘制框框。
		DrawSelectionBox(m_data[0], m_data[1], m_data[2], m_data[3]);

		// 绘制框选到的轮廓实例。
		for (unsigned int i = 0; i < m_pRectSelectLoopInstances->size(); i++) {
			LoopInstancePtr pLoopInstance = m_pRectSelectLoopInstances->at(i);
			COLORREF iColor = LoopToolManager::GetLoopDisplayColor(pLoopInstance->GetCacheData()->GetLoopTool().get());
			loopDrawer.DrawActiveLoop(pLoopInstance->GetCacheData()->GetPatternLoop().get(), iColor, pLoopInstance->GetTransformMatrix());
		}
	}
	else // 点选
	{
		// 绘制当前的轮廓实例。
		if (m_pActiveLoopInstance) {
			COLORREF iColor = LoopToolManager::GetLoopDisplayColor(m_pActiveLoopInstance->GetCacheData()->GetLoopTool().get());
			loopDrawer.DrawActiveLoop(m_pActiveLoopInstance->GetCacheData()->GetPatternLoop().get(), iColor, m_pActiveLoopInstance->GetTransformMatrix());
		}
	}

	// draw cursor.
	CursorDrawer::DrawAimSelCursor(m_pViewPort);

	return TRUE;
}

void FlyCutSequenceAddAction::UpdateForUndoRedo()
{
	// 更新“切割节点”缓存。
	RefreshCutNodes();
}

END_CUTLEADER_NAMESPACE()

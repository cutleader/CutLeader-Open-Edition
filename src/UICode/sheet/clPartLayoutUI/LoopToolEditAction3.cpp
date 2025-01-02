#include "StdAfx.h"
#include "LoopToolEditAction3.h"

#include "GlViewPort.h"
#include "CommandManager.h"
#include "DummyAction.h"
#include "ValueInputManager.h"
#include "CursorDrawer.h"
#include "clBaseDataResource.h"
#include "LogMgr.h"

#include "CLOptionInfo.h"
#include "DisplayParam.h"
#include "IMaterialSize.h"

#include "PatternList.h"
#include "IPatternLoop.h"
#include "PatternLoopList.h"
#include "LoopDrawer.h"
#include "clGeometryFeatureResource.h"
#include "LoopCutFeature.h"
#include "LoopCutFeatureList.h"
#include "LoopToolData.h"
#include "LoopToolDataList.h"
#include "LoopStartCutDataList.h"
#include "LoopToolEditPanel.h"
#include "clCutFeatureResource.h"
#include "PartInstance.h"
#include "LoopToolManager.h"
#include "LoopInstanceManager.h"
#include "LoopInstanceCache.h"
#include "LoopInstance.h"
#include "LoopInstanceList.h"
#include "ToolInstance.h"
#include "ToolInstanceList.h"
#include "ToolAssignConfigItem.h"
#include "LoopToolChangeCommand_2.h"
#include "DisableToolOnLoopCommand.h"
#include "PartInstanceList.h"
#include "PartPlacementList.h"
#include "clPartLayoutUIResource.h"
#include "PartLayoutHelper.h"
#include "CanvasGeometryEditAction.h"

BEGIN_CUTLEADER_NAMESPACE()

LoopToolEditAction3::LoopToolEditAction3(IMaterialSizePtr pMaterialSize, PartPlacementListPtr pPartPlacements, PartInstanceListPtr pPartInstances, PartGroupListPtr pPartGroups, int iToolIndex, GlViewPortPtr pViewPort,
    ToolAssignConfigItemPtr pToolAssignConfigItem, LeadConfigItemPtr pLeadConfigItem, ConditionTypeListPtr pCondTypeList, ChangeLoopToolActionBar* pChangeLoopToolActionBar)
    : PartPlacementEditAction(pMaterialSize, pPartPlacements, pPartInstances, pPartGroups, pViewPort)
{
	m_iMsgID = IDS_MSG_EDIT_LOOP_TOOL_2;

    m_pChangeLoopToolActionBar = pChangeLoopToolActionBar;
    m_iToolIndex = iToolIndex;

    m_iState = NONE;
    m_pToolAssignConfigItem = pToolAssignConfigItem;
	m_pLeadConfigItem = pLeadConfigItem;
    m_pCondTypeList = pCondTypeList;
    m_pActiveLoopInsts.reset(new LoopInstanceList);

	// 得到所有“回路实例”。
	m_pAllLoopInstances = LoopInstanceManager::GetLoopInstanceList(m_pPartInstanceList);

	if (m_pMatSize->GetType() != MAT_SIZE_Canvas) {
		m_pChangeLoopToolActionBar->ShowChangeLoopToolActionBar();
	}
}

LoopToolEditAction3::~LoopToolEditAction3(void)
{
}

BOOL LoopToolEditAction3::LButtonDown(double& dPtX, double& dPtY)
{
    if (m_iState == NONE || m_iState == LOOPINST_SELECT_SECOND_POINT)
    {
        m_iState = LOOPINST_SELECT_FIRST_POINT;

        m_boxData[0] = dPtX;
        m_boxData[1] = dPtY;
        m_boxData[2] = dPtX;
        m_boxData[3] = dPtY;
    }

	return TRUE;
}

BOOL LoopToolEditAction3::LButtonUp(double& dPtX, double& dPtY)
{
    CString strProductName, strMsg;
    strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);

    if (m_iState == LOOPINST_SELECT_FIRST_POINT)
    {
        m_iState = LOOPINST_SELECT_SECOND_POINT;

        // 得到选中的轮廓
        LoopInstanceListPtr pSelectedLoopInsts(new LoopInstanceList);
        if (m_pActiveLoopInsts->size() > 0) {
            pSelectedLoopInsts->insert(pSelectedLoopInsts->end(), m_pActiveLoopInsts->begin(), m_pActiveLoopInsts->end());
        }

        if (pSelectedLoopInsts->size() == 0) {
            CString strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);
            MessageBox(m_pViewPort->GetWnd(), _T("Please select loop instances, and then change their tools."), strProductName, MB_OK | MB_ICONWARNING);
            return TRUE;
        }
		// 能运行到这里，肯定有轮廓被选中了。

    	// 看看要不要涉及到相同零件的轮廓。
		LoopInstanceListPtr pLoopInstances_ChangeTool(new LoopInstanceList);
		pLoopInstances_ChangeTool->insert(pLoopInstances_ChangeTool->end(), pSelectedLoopInsts->begin(), pSelectedLoopInsts->end());
		if (m_pMatSize->GetType() != MAT_SIZE_Canvas && m_pChangeLoopToolActionBar->IsSpread())
		{
			for (unsigned int i = 0; i < pSelectedLoopInsts->size(); i++) {
				const LoopInstance* pSelectedLoopInstance = pSelectedLoopInsts->operator[](i).get();
				for (unsigned int j = 0; j < m_pAllLoopInstances->size(); j++) {
					LoopInstancePtr pLoopInstance = m_pAllLoopInstances->operator[](j);
					if (pLoopInstance->GetPatternLoopID() == pSelectedLoopInstance->GetPatternLoopID()) {
						if (!pLoopInstances_ChangeTool->GetLoopInstance_ByPartInstanceAndPatternLoop(pLoopInstance->GetPartInstance()->GetID(), pLoopInstance->GetPatternLoopID())) {
							pLoopInstances_ChangeTool->push_back(pLoopInstance);
						}
					}
				}
			}
		}

		// 设置这些轮廓实例的刀具。
		const EditorData& editorData = GetEditorData();
		PartLayoutHelper::SetToolToLoopInstances(editorData.GetView(), editorData.GetEditorName(), pLoopInstances_ChangeTool, m_iToolIndex,
			m_loopTopologyItemData, m_pToolAssignConfigItem.get(), m_pLeadConfigItem);

		// 选中这些轮廓实例.
		if (m_pMatSize->GetType() == MAT_SIZE_Canvas) {
			CanvasGeometryEditAction::PostMessage2EnterCanvasLoopSelectAction_PreSelectLoopAndText(pLoopInstances_ChangeTool.get());
		}
		else {
			PartPlacementEditAction::PostMessage2EnterLoopInstanceSelectAction(pLoopInstances_ChangeTool.get());
		}
    }

    return TRUE;
}

BOOL LoopToolEditAction3::MovePoint(double& dPtX, double& dPtY)
{
	Point2D pt(dPtX, dPtY);

    m_pActiveLoopInsts->clear();
    if (m_iState == LOOPINST_SELECT_FIRST_POINT)
    {
        m_boxData[2] = dPtX;
        m_boxData[3] = dPtY;

        Rect2D rect;
        rect.SetXMin(m_boxData[0] < m_boxData[2] ? m_boxData[0] : m_boxData[2]);
        rect.SetXMax(m_boxData[0] < m_boxData[2] ? m_boxData[2] : m_boxData[0]);
        rect.SetYMin(m_boxData[1] < m_boxData[3] ? m_boxData[1] : m_boxData[3]);
        rect.SetYMax(m_boxData[1] < m_boxData[3] ? m_boxData[3] : m_boxData[1]);

        if (m_pChangeLoopToolActionBar->GetIsSelectPart()) // 框选零件
        {
            PartPlacementListPtr pPartPlacements = m_pPartPlacementList->GetPartPlacementInRect(rect);
            for (unsigned int i = 0; i < pPartPlacements->size(); i++)
            {
                PartPlacementPtr pPartPlacement = pPartPlacements->operator [](i);
                PartInstanceListPtr pPartInstances = m_pPartInstanceList->GetPartInstanceByPartPlacement(pPartPlacement.get());
                LoopInstanceListPtr pLoopInstances = LoopInstanceManager::GetLoopInstanceList(pPartInstances);
                m_pActiveLoopInsts->insert(m_pActiveLoopInsts->end(), pLoopInstances->begin(), pLoopInstances->end());
            }
        }
        else
        {
		    m_pActiveLoopInsts = LoopInstanceManager::GetLoopInstsInRect(m_pAllLoopInstances, rect);
        }
    }
    else
    {
        if (m_pChangeLoopToolActionBar->GetIsSelectPart()) // 选择零件
        {
            PartPlacementPtr pPartPlacement = m_pPartPlacementList->GetPartPlacementClosestTo(pt);
            PartInstanceListPtr pPartInstances = m_pPartInstanceList->GetPartInstanceByPartPlacement(pPartPlacement.get());
            LoopInstanceListPtr pLoopInstances = LoopInstanceManager::GetLoopInstanceList(pPartInstances);
            m_pActiveLoopInsts->insert(m_pActiveLoopInsts->end(), pLoopInstances->begin(), pLoopInstances->end());
        }
        else
        {
            LoopInstancePtr pActiveLoopInstance = LoopInstanceManager::GetClosestLoopInst(m_pAllLoopInstances, pt);
            if (pActiveLoopInstance)
            {
                m_pActiveLoopInsts->push_back(pActiveLoopInstance);
            }
        }
    }

	return TRUE;
}

BOOL LoopToolEditAction3::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	return __super::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL LoopToolEditAction3::DrawBeforeFinish()
{
    LoopDrawer loopDrawer(m_pViewPort);

    if (m_iState == LOOPINST_SELECT_FIRST_POINT) // 框选。
    {
        // 绘制框框。
        {
            // backup the old draw config.
            bool bOldStipple = false;
            int iOldRepeat = 1;
            unsigned short iOldPattern = 0xffff;
            m_pViewPort->GetLineStipple(bOldStipple, iOldRepeat, iOldPattern);

            // draw the selection rectangle.
            m_pViewPort->SetLineStipple(true, 2, 0xcccc);
            m_pViewPort->DrawRect(m_boxData[0], m_boxData[1], m_boxData[2], m_boxData[3]);

            // restore the drawer
            m_pViewPort->SetLineStipple(bOldStipple, iOldRepeat, iOldPattern);
        }
    }

    // 绘制候选轮廓。
    for (unsigned int i = 0; i < m_pActiveLoopInsts->size(); i++)
    {
        LoopInstancePtr pActiveLoopInst = m_pActiveLoopInsts->at(i);
        COLORREF iColor = LoopToolManager::GetLoopDisplayColor(pActiveLoopInst->GetCacheData()->GetLoopTool().get());
        loopDrawer.DrawActiveLoop(pActiveLoopInst->GetCacheData()->GetPatternLoop().get(), iColor, pActiveLoopInst->GetTransformMatrix());
    }

	// draw cursor.
	CursorDrawer::DrawAimSelCursor(m_pViewPort);

	return TRUE;
}

void LoopToolEditAction3::UpdateForUndoRedo()
{
	m_pActiveLoopInsts->clear();
}

void LoopToolEditAction3::SetTool_toSelectedLoopInstances(LoopInstanceListPtr pLoopInstances)
{
    if (pLoopInstances->size() == 0) {
        LogMgr::DebugWarn(_T("301865"), CWnd::FromHandle(m_pViewPort->GetWnd()));
        return;
    }

    // get the loop tool data list which will change tool.
    LoopToolDataListPtr pLoopToolList_4_edit(new LoopToolDataList);
    LoopStartCutDataListPtr pLoopStaEndCutList_4_edit(new LoopStartCutDataList);
	for (unsigned int i = 0; i < pLoopInstances->size(); i++) {
		LoopToolDataPtr pLoopTool = pLoopInstances->at(i)->GetCacheData()->GetLoopTool();
		pLoopToolList_4_edit->push_back(pLoopTool);
		LoopStartCutDataPtr pLoopStartCutData = pLoopInstances->at(i)->GetCacheData()->GetLoopStartCutData();
		pLoopStaEndCutList_4_edit->push_back(pLoopStartCutData);
	}

    // run the command.
    ICommandPtr pCommand;
    if (m_iToolIndex == -1) // -1 means no-cutting selected loops
    {
        // 发消息给板材视图，问问是不是还要把其他的轮廓设置为“不加工”。
        LoopToolDataListPtr pLoopToolDataList_other(new LoopToolDataList);
        GetEditorData().GetView()->SendMessage(WM_QueryOtherLoopTools, (WPARAM)pLoopToolList_4_edit.get(), (LPARAM)pLoopToolDataList_other.get());
        LoopToolDataListPtr pLoopToolDataList(new LoopToolDataList);
        pLoopToolDataList->insert(pLoopToolDataList->end(), pLoopToolList_4_edit->begin(), pLoopToolList_4_edit->end());
        pLoopToolDataList->insert(pLoopToolDataList->end(), pLoopToolDataList_other->begin(), pLoopToolDataList_other->end());
        pCommand.reset(new DisableToolOnLoopCommand(GetEditorData(), pLoopToolDataList, true));
    }
    else if (m_iToolIndex == -2) // -2 means cutting selected loops
    {
        // 发消息给板材视图，问问是不是还要把其他的轮廓设置为“不加工”。
        LoopToolDataListPtr pLoopToolDataList_other(new LoopToolDataList);
        GetEditorData().GetView()->SendMessage(WM_QueryOtherLoopTools, (WPARAM)pLoopToolList_4_edit.get(), (LPARAM)pLoopToolDataList_other.get());
        LoopToolDataListPtr pLoopToolDataList(new LoopToolDataList);
        pLoopToolDataList->insert(pLoopToolDataList->end(), pLoopToolList_4_edit->begin(), pLoopToolList_4_edit->end());
        pLoopToolDataList->insert(pLoopToolDataList->end(), pLoopToolDataList_other->begin(), pLoopToolDataList_other->end());
        pCommand.reset(new DisableToolOnLoopCommand(GetEditorData(), pLoopToolDataList, false));
    }
    else
    {
        ToolInstancePtr pToolInstance = m_pToolAssignConfigItem->GetToolInstanceList()->operator [](m_iToolIndex);
        if (!pToolInstance) {
            LogMgr::DebugWarn(_T("974352"), CWnd::FromHandle(m_pViewPort->GetWnd()));
            return;
        }
        pCommand.reset(new LoopToolChangeCommand_2(GetEditorData(), m_loopTopologyItemData, pLoopToolList_4_edit, pToolInstance, pLoopStaEndCutList_4_edit, m_pLeadConfigItem));
    }
    pCommand->Do();
    CommandManager::GetInstance()->AddCommand(pCommand);

    // 更新视图显示
	CWnd::FromHandle(GetEditorData().GetView()->m_hWnd)->Invalidate();
}

END_CUTLEADER_NAMESPACE()

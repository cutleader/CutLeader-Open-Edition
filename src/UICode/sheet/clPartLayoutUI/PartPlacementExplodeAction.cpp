#include "StdAfx.h"
#include "PartPlacementExplodeAction.h"

#include "GlViewPort.h"
#include "CommandManager.h"
#include "ICommand.h"
#include "MathEx.h"
#include "ValueInputManager.h"
#include "DummyAction.h"
#include "CursorDrawer.h"
#include "clBaseDataResource.h"
#include "LogMgr.h"

#include "CLOptionInfo.h"
#include "DisplayParam.h"

#include "PatternList.h"
#include "LoopStartCutDataList.h"
#include "Part.h"
#include "PartPlacementDrawer.h"
#include "PartPlacement.h"
#include "PartPlacementList.h"
#include "PartPlacementExplodeCommand.h"
#include "clPartLayoutResource.h"
#include "PartPlacementSelectAction.h"
#include "PartPlacementManager.h"

BEGIN_CUTLEADER_NAMESPACE()

PartPlacementExplodeAction::PartPlacementExplodeAction(IMaterialSizePtr pMaterialSize, PartPlacementListPtr pPartPlacements, PartInstanceListPtr pPartInstances, PartGroupListPtr pPartGroups, GlViewPortPtr pViewPort)
    : PartPlacementEditAction(pMaterialSize, pPartPlacements, pPartInstances, pPartGroups, pViewPort)
{
	m_iMsgID = IDS_MSG_EXPLODE_PARTGRID;
}

PartPlacementExplodeAction::~PartPlacementExplodeAction(void)
{
}

BOOL PartPlacementExplodeAction::LButtonDown(double& dPtX, double& dPtY)
{
	CString str, strProductName;
	strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);

	if (m_pSelectedPartPlacement)
	{
		// 必须选择阵列零件。
		if (m_pSelectedPartPlacement->IsGrid())
		{
			ExplodeGridPartPlacement(m_pSelectedPartPlacement);
		}
		else
		{
			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_OUTPUT_SELECT_GRID_PART);
			MessageBox(m_pViewPort->GetWnd(), str, strProductName, MB_OK | MB_ICONWARNING);
		}
	}

	return TRUE;
}

void PartPlacementExplodeAction::ExplodeGridPartPlacement(PartPlacementPtr pPartPlacement)
{
    if (!pPartPlacement->IsGrid())
    {
        LogMgr::DebugWarn(_T("876521"), CWnd::FromHandle(m_pViewPort->GetWnd()));
        return;
    }

    // 炸散阵列零件得到的零件放置和零件实例。
    PartPlacementListPtr pNewPartPlacements = PartPlacementManager::ExplodeGridPart(pPartPlacement.get());

    // 执行命令。
    ICommandPtr pCommand(new PartPlacementExplodeCommand(GetEditorData(), m_pPartPlacementList, m_pPartInstanceList, pPartPlacement, pNewPartPlacements));
    pCommand->Do();
    CommandManager::GetInstance()->AddCommand(pCommand);

    // 选中炸开后的零件放置。
    PostMessage2EnterPartPlacementSelectAction(pNewPartPlacements.get());
}

BOOL PartPlacementExplodeAction::MovePoint(double& dPtX, double& dPtY)
{
	Point2D pt(dPtX, dPtY);
	m_pSelectedPartPlacement = m_pPartPlacementList->GetPartPlacementClosestTo(pt);

	return TRUE;
}

BOOL PartPlacementExplodeAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	PartPlacementEditAction::OnKeyDown(nChar, nRepCnt, nFlags);

	return TRUE;
}

BOOL PartPlacementExplodeAction::DrawBeforeFinish()
{
	PartPlacementDrawer partPlacementDrawer(m_pViewPort);

	if (m_pSelectedPartPlacement)
    {
		partPlacementDrawer.DrawActivePartPlacement(m_pSelectedPartPlacement.get());
    }

	CursorDrawer::DrawAimSelCursor(m_pViewPort);

	return TRUE;
}

END_CUTLEADER_NAMESPACE()

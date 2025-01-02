#include "StdAfx.h"
#include "PartPlacementGridAction.h"

#include "GlViewPort.h"
#include "CommandManager.h"
#include "ICommand.h"
#include "MathEx.h"
#include "ValueInputManager.h"
#include "DummyAction.h"
#include "CursorDrawer.h"
#include "clBaseDataResource.h"
#include "LogMgr.h"
#include "ClWindowFinder.h"
#include "LineArc2DList.h"

#include "CLOptionInfo.h"
#include "DisplayParam.h"

#include "PatternList.h"
#include "LoopStartCutDataList.h"
#include "Part.h"
#include "PartPlacementDrawer.h"
#include "PartPlacement.h"
#include "PartPlacementList.h"
#include "PartPlacementGridCommand.h"
#include "clPartLayoutResource.h"
#include "PartPlacementSelectAction.h"
#include "CreatePartGridActionBar.h"
#include "PartLayoutHelper.h"

BEGIN_CUTLEADER_NAMESPACE()

PartPlacementGridAction::PartPlacementGridAction(IMaterialSizePtr pMaterialSize, PartPlacementListPtr pPartPlacements, PartInstanceListPtr pPartInstances,
	PartGroupListPtr pPartGroups, PartPlacementListPtr pSelectedPartPlacements, GlViewPortPtr pViewPort, CreatePartGridActionBar* pCreatePartGridActionBar)
    : PartPlacementPreSelectAction(pMaterialSize, pPartPlacements, pPartInstances, pPartGroups, pViewPort)
{
	m_pSelectedPartPlacements = pSelectedPartPlacements;

	m_pCreatePartGridActionBar = pCreatePartGridActionBar;
	m_pCreatePartGridActionBar->AddObserver(this);

	if (m_pSelectedPartPlacements->size() == 0) {
		m_iMsgID = IDS_OUTPUT_GRID_SEL_PART;
	}
	else {
		m_iMsgID = IDS_OUTPUT_GRID_PARTPMT;
		m_pCreatePartGridActionBar->Show_CreatePartGridActionBar();
		GeneratePartPlacement4Preview();
	}
}

PartPlacementGridAction::~PartPlacementGridAction(void)
{
	m_pCreatePartGridActionBar->RemoveObserver(GetID());
}

BOOL PartPlacementGridAction::LButtonDown(double& dPtX, double& dPtY)
{
	if (m_pSelectedPartPlacements->size() == 0) {
		__super::LButtonDown(dPtX, dPtY);
	}

	return TRUE;
}

BOOL PartPlacementGridAction::LButtonUp(double& dPtX, double& dPtY)
{
	if (m_pSelectedPartPlacements->size() == 0)
	{
		__super::LButtonUp(dPtX, dPtY);

		if (m_pSelectedPartPlacements->size() == 1 && !m_pSelectedPartPlacements->front()->IsGrid()) { // 选中了一个非阵列零件。
			m_pCreatePartGridActionBar->Show_CreatePartGridActionBar();
			GeneratePartPlacement4Preview();
		}
		else {
			CString str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_OUTPUT_GRID_SEL_PART);
			MessageBox(m_pViewPort->GetWnd(), str, MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME), MB_OK | MB_ICONWARNING);
		}
	}

	return TRUE;
}

BOOL PartPlacementGridAction::MovePoint(double& dPtX, double& dPtY)
{
	if (m_pSelectedPartPlacements->size() == 0) { // 还没选中零件放置。
		__super::MovePoint(dPtX, dPtY);
	}

	return TRUE;
}

BOOL PartPlacementGridAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	return __super::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL PartPlacementGridAction::DrawBeforeFinish()
{
	PartPlacementDrawer partPlacementDrawer(m_pViewPort);
	if (m_pSelectedPartPlacements->size() == 0) {
		__super::DrawBeforeFinish();
	}
	else {
		if (m_pPartPlacement4Preview) { // 绘制用于预览的零件放置。
			partPlacementDrawer.DrawSelectedPartPlacement(m_pPartPlacement4Preview.get());
		}
		else {
			partPlacementDrawer.DrawSelectedPartPlacement(m_pSelectedPartPlacements->front().get());
		}

		CursorDrawer::DrawAimSelCursor(m_pViewPort);
	}

	return TRUE;
}

void PartPlacementGridAction::UpdateForUndoRedo()
{
}

void PartPlacementGridAction::Update(int iEventID, const CComVariant& varEventContent)
{
	if (iEventID == ID_EVENT_PartGridParamChanged)
	{
		GeneratePartPlacement4Preview();
	}
	else if (iEventID == ID_EVENT_GridPart_OK)
	{
		if (!m_pPartPlacement4Preview) {
			LogMgr::DebugWarn(_T("953278"));
			return;
		}

		// 执行命令。
		int	iRowCount = m_pPartPlacement4Preview->GetRowCount();
		int	iColCount = m_pPartPlacement4Preview->GetColCount();
		double dSpacingX = m_pPartPlacement4Preview->GetSpacingX();
		double dSpacingY = m_pPartPlacement4Preview->GetSpacingY();
		ICommandPtr pCommand(new PartPlacementGridCommand(GetEditorData(), m_pPartPlacementList, m_pPartInstanceList, m_pSelectedPartPlacements->front(),
			iRowCount, iColCount, dSpacingX, dSpacingY));
		pCommand->Do();
		CommandManager::GetInstance()->AddCommand(pCommand);

		// 更新任务
		ClWindowFinder::GetCurrentView_1()->SendMessage(WM_updateSheetPreviewData, 0, 0);
		ClWindowFinder::GetCurrentView_2()->SendMessage(WM_redrawSheetThumbnailOnSheetManagementPanel, 0, 0);
		ClWindowFinder::GetCurrentView_2()->SendMessage(WM_ReDisplayNestedCountOfEachPart, 0, 0);

		// 选中阵列零件放置。
		PostMessage2EnterPartPlacementSelectAction(m_pSelectedPartPlacements->front().get());
	}
	else if (iEventID == ID_EVENT_GridPart_Cancel) {
		PostMessage2EnterPartPlacementSelectAction();
	}
}

void PartPlacementGridAction::GeneratePartPlacement4Preview()
{
	// 根据PartGridParam参数来创建零件放置阵列。
	const PartGridParam* pPartGridParam = m_pCreatePartGridActionBar->GetPartGridParam();
	m_pPartPlacement4Preview = PartLayoutHelper::CreatePartGrid(m_pSelectedPartPlacements->front().get(), pPartGridParam);

	CWnd::FromHandle(m_pViewPort->GetWnd())->Invalidate();
}

END_CUTLEADER_NAMESPACE()

#include "StdAfx.h"
#include "AddPartPlacementFromDatacenterAction.h"

#include "CommandManager.h"
#include "ICommand.h"
#include "ValueInputManager.h"
#include "GlViewPort.h"
#include "CursorDrawer.h"
#include "ClFolderHelper.h"
#include "DataBaseManager.h"
#include "ClWindowFinder.h"

#include "DisplayParam.h"
#include "SheetEditParam.h"
#include "CLOptionInfo.h"
#include "ImpExpConfig.h"

#include "PartCadData.h"
#include "PartItem.h"
#include "Part.h"
#include "PartLoader.h"
#include "PartManager.h"
#include "PartBuilder.h"
#include "clPartLayoutResource.h"
#include "PartLayoutEditor.h"
#include "PartPlacement.h"
#include "PartPlacementList.h"
#include "PartPlacementAddCommand.h"
#include "PartPlacementManager.h"
#include "PartPlacementDrawer.h"

BEGIN_CUTLEADER_NAMESPACE()

AddPartPlacementFromDatacenterAction::AddPartPlacementFromDatacenterAction(IMaterialSizePtr pMaterialSize, PartPlacementListPtr pPartPlacements, PartInstanceListPtr pPartInstances,
	PartGroupListPtr pPartGroups, GlViewPortPtr pViewPort, LONGLONG iPartID, LONGLONG iShtParamConfigID, PartDistanceDetectActionBar* pPartDistanceDetectActionBar)
    : PartPlacementEditAction(pMaterialSize, pPartPlacements, pPartInstances, pPartGroups, pViewPort), PartPlacementMoveBase(m_pViewPort, pPartDistanceDetectActionBar)
{
	m_iMsgID = IDS_OUTPUT_PARTPMT_BASEPT;

	// 生成零件放置对象，并把它的中心点调整到当前光标位置。
	PartPlacementListPtr pMovePartPlacements(new PartPlacementList);
	PartPlacementPtr pNewPartPlacement;
	LONGLONG iCamDataID, iExpLibItemID;
	DBConnectPtr pPartDBCon = DataBaseManager::ConnectPartDB(iPartID);
	if (PartManager::HasCamDatasetA(iPartID, iShtParamConfigID, iCamDataID, iExpLibItemID))
	{
		PartItemPtr pPartItem = PartLoader::LoadPartItem(pPartDBCon, iPartID, iCamDataID);
		pNewPartPlacement = PartPlacementManager::BuildPartPmt(pPartItem.get(), Matrix2D(), true);
	}
	else
	{
		PartCadDataPtr pPartCadData = PartLoader::LoadPartCadData(pPartDBCon);
		UINT iAssignLimit = CLOptionInfo::GetInstance()->GetImpExpConfig()->GetAssignLimit();
		PartCamDataPtr pNewPartCamData = PartBuilder::BuildPartCamData(pPartCadData, iShtParamConfigID, iAssignLimit, FALSE);
		PartPtr pPart = PartLoader::LoadPart(pPartDBCon, iPartID);
		pNewPartPlacement.reset(new PartPlacement(pPart, pNewPartCamData));
	}
	pMovePartPlacements->push_back(pNewPartPlacement);

	// 为移动零件准备数据。
	GetPartDistanceDetectActionBar()->Show_PartDistanceDetectActionBar();
	UpdateBeforeMov(pMovePartPlacements, pMovePartPlacements->GetRectBox().GetCenterPt(), pMaterialSize, m_pPartInstanceList);
}

AddPartPlacementFromDatacenterAction::~AddPartPlacementFromDatacenterAction(void)
{
}

BOOL AddPartPlacementFromDatacenterAction::LButtonDown(double& dPtX, double& dPtY)
{
	// 把“零件放置”放到板材上。
	ICommandPtr pCommand(new PartPlacementAddCommand(GetEditorData(), m_pPartPlacementList, GetMovePartPlacements()->at(0), m_pPartInstanceList));
	pCommand->Do();
	CommandManager::GetInstance()->AddCommand(pCommand);

	// 准备下一次操作的数据。
	{
		PartPlacementListPtr pMovePartPlacements(new PartPlacementList);
		PartPlacementPtr pPartPlacement = PartPlacementManager::ClonePartPmt(GetMovePartPlacements()->front().get());
		pMovePartPlacements->push_back(pPartPlacement);
		UpdateBeforeMov(pMovePartPlacements, pMovePartPlacements->GetRectBox().GetCenterPt(), m_pMatSize, m_pPartInstanceList);
	}

	// 更新任务
	{
		// 板材相关
		ClWindowFinder::GetCurrentView_1()->SendMessage(WM_updateSheetPreviewData, 0, 0);
		ClWindowFinder::GetCurrentView_2()->SendMessage(WM_redrawSheetThumbnailOnSheetManagementPanel, 0, 0);

		// 零件相关
		ClWindowFinder::GetCurrentView_2()->SendMessage(WM_PartAddedOnSheet, 0, 0);
	}

	return TRUE;
}

BOOL AddPartPlacementFromDatacenterAction::MovePoint(double& dPtX, double& dPtY)
{
	Point2D currentPt(dPtX, dPtY);

	// 移动的过程中更新一些数据。
	UpdateWhenMov(currentPt);

	return TRUE;
}

BOOL AddPartPlacementFromDatacenterAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	PartPlacementEditAction::OnKeyDown(nChar, nRepCnt, nFlags);
	UpdateDataWhenRotate(nChar);

	return TRUE;
}

BOOL AddPartPlacementFromDatacenterAction::DrawBeforeFinish()
{
	// 绘制移动中的“零件放置”。
	PartPlacementDrawer partPlacementDrawer(m_pViewPort);
	const PartPlacementList* pMovePartPlacements = GetMovePartPlacements().get();
	partPlacementDrawer.DrawPartPlacements(pMovePartPlacements);

	// draw cursor.
	CursorDrawer::DrawCrossLineCursor(m_pViewPort);

	return TRUE;
}

void AddPartPlacementFromDatacenterAction::UpdateForUndoRedo()
{
	// 为零件吸附准备数据。
	{
		PartPlacementListPtr pMovePartPlacements(new PartPlacementList);
		PartPlacementPtr pPartPlacement = PartPlacementManager::ClonePartPmt(GetMovePartPlacements()->front().get());
		pMovePartPlacements->push_back(pPartPlacement);
		UpdateBeforeMov(pMovePartPlacements, pMovePartPlacements->GetRectBox().GetCenterPt(), m_pMatSize, m_pPartInstanceList);
	}
}

END_CUTLEADER_NAMESPACE()

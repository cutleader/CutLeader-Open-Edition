#include "StdAfx.h"
#include "PartInplaceEditManager.h"

#include <PatternList.h>

#include "baseConst.h"
#include "DataBaseManager.h"
#include "DataItem.h"
#include "LogMgr.h"
#include "ClWindowFinder.h"
#include "FolderNode.h"
#include "DataCenterItemLoader.h"
#include "clBaseDataResource.h"
#include "DateHelper.h"

#include "ParamConfigLoader.h"
#include "CLOptionInfo.h"
#include "ImpExpConfig.h"

#include "UIManager.h"
#include "PartLoader.h"
#include "DocManager.h"
#include "PartDocument.h"
#include "PartView.h"
#include "PartInplaceEditData.h"
#include "clFrameResource.h"
#include "MainFrame.h"
#include "PartBuilder.h"
#include "PartItem.h"
#include "PartManager.h"
#include "Part.h"
#include "TaskItem.h"
#include "TaskPart.h"
#include "TaskPartList.h"
#include "Sheet.h"
#include "SheetList.h"
#include "TaskView.h"
#include "clPartLayoutResource.h"
#include "PartCamData.h"
#include "PartCadData.h"
#include "CutSequenceList.h"
#include "PartCamDataList.h"
#include "PartPlacement.h"
#include "PartPlacementList.h"
#include "LoopCutFeatureManager.h"
#include "clPartLayoutUIResource.h"
#include "LoopStartCutProcessor.h"
#include "SheetList.h"
#include "TaskSheetManagementBar.h"
#include "SheetManager.h"

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

void PartInplaceEditManager::OpenPartDocByInPlaceEditMode(CMultiDocTemplate* pPartDocTemplate)
{
	if (!PartInplaceEditData::GetInstance()->GetIsUnderInPlaceEditMode()) {
		LogMgr::DebugWarn(_T("994473"));
		return;
	}

	// 在位编辑的输入。
	const Part* pPart = PartInplaceEditData::GetInstance()->GetPart();
	const PartCamData* pPartCamData = PartInplaceEditData::GetInstance()->GetPartCamData();

	// 创建一个临时的零件对象。
	PartPtr pNewPart;
	{
		pNewPart.reset(new Part);
		pNewPart->SetName(pPart->GetName());
		pNewPart->SetCreateTime(DateHelper::GetCurrentTime());
		pNewPart->SetModifyTime(DateHelper::GetCurrentTime());

		// 克隆CAD数据。
		PartCadDataPtr pNewCadData = PartBuilder::CloneCadData(pPart->GetCadData().get());
		pNewPart->SetCadData(pNewCadData);

		// 克隆CAM数据。
		PartCamDataListPtr pNewCamDataList(new PartCamDataList);
		PartCamDataPtr pNewPartCamData;
		if (pPartCamData) {
			pNewPartCamData = PartBuilder::CloneCamData(pPartCamData, pPart->GetCadData().get(), pNewCadData.get());
		}
		else {
			pNewPartCamData = PartBuilder::CloneCamData(pPart->GetCamDataList()->front().get(), pPart->GetCadData().get(), pNewCadData.get());
		}
		pNewCamDataList->push_back(pNewPartCamData);
		pNewPart->SetCamDataList(pNewCamDataList);
	}

	// 创建零件项并打开进行在位编辑。
	PartItemPtr pPartItem(new PartItem(pNewPart, pNewPart->GetCamDataList()->front()));
	PartInplaceEditData::GetInstance()->SetLatestModifyTimeStamp(pPartItem->GetLatestModifyTimeStamp());
	pPartDocTemplate->OpenDocumentFile(NULL);
	((PartDocument*)DocManager::GetActiveDocument())->SetPartItem(pPartItem);
	DocManager::GetActivePartView()->Init();
	DocManager::GetActivePartView()->PositionAndShow_LeavePartInplaceEditBar(); // 在零件视图中显示一个小面板。

	UpdateGui();
}

void PartInplaceEditManager::CancelInPlaceEditMode()
{
	// 这里只重置在位编辑标记。
	PartInplaceEditData::GetInstance()->ResetInplaceEditFlag();

	// 关闭当前任务，不保存所做的修改。
	DocManager::GetActiveDocument()->OnCloseDocument();

	UpdateGui();

	// 最后重置在位编辑数据。
	PartInplaceEditData::GetInstance()->ResetInplaceEditData();
}

void PartInplaceEditManager::FinishInPlaceEditMode()
{
	// 得到当前零件。
	PartItemPtr pPartItem;
	if (PartView* pPartView = dynamic_cast<PartView*>(ClWindowFinder::GetCurrentView_1())) {
		pPartItem = pPartView->GetCurrentPart();
		if (pPartItem->GetPart()->GetCadData()->GetPatternList()->size() == 0) { // 零件不能是空的。
			CString str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_InplaceEditPart_EmptyPart);
			MessageBox(pPartView->m_hWnd, str, MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME), MB_OK | MB_ICONINFORMATION);
			return;
		}
	} else {
		LogMgr::DebugWarn(_T("652900"));
		return;
	}

	// 这里只重置在位编辑标记。
	PartInplaceEditData::GetInstance()->ResetInplaceEditFlag();

	// 如果没有修改什么，那就什么也不做，退出。
	if (pPartItem->GetLatestModifyTimeStamp() == PartInplaceEditData::GetInstance()->GetLatestModifyTimeStamp()) {
		DocManager::GetActiveDocument()->OnCloseDocument(); // 关闭当前任务，不保存所做的修改。
		UpdateGui();
		PartInplaceEditData::GetInstance()->ResetInplaceEditData(); // 最后重置在位编辑数据。
		return;
	}

	// 检查当前零件的cam数据。
	PartCadDataPtr pPartCadData = pPartItem->GetPart()->GetCadData();
	PartCamDataPtr pPartCamData = pPartItem->GetCamData();
	const LoopTopologyItemList* pLoopTopologyItems = pPartCadData->GetLoopTopologyItems();
	DBConnectPtr pExpLibDBCon = DataBaseManager::GetInstance()->GetExpLibDBConnect();
	ParamConfigPtr pParamConfig = ParamConfigLoader::LoadParamConfig(pExpLibDBCon, pPartCamData->GetParamConfigID());
	LoopCutFeatureManager::CheckIntegrity(pPartCadData->GetPatternLoopList().get(), pLoopTopologyItems, pPartCamData->GetLoopFeatureList(),
		pParamConfig.get(), TRUE, FALSE, FALSE, Matrix2D());

	// 克隆一份正在在位编辑的零件并另存到一个目录，然后关掉正在在位编辑的零件，这些操作后当前的文档应该就是在位编辑前的那个任务文档。
	PartPtr pNewPart;
	{
		// 克隆一份零件。
		PartCadDataPtr pNewCadData = PartBuilder::CloneCadData(pPartCadData.get());
		PartCamDataListPtr pNewCamDataList(new PartCamDataList);
		PartCamDataPtr pNewPartCamData = PartBuilder::CloneCamData(pPartCamData.get(), pPartCadData.get(), pNewCadData.get());
		pNewCamDataList->push_back(pNewPartCamData);
		CString strPartName = PartInplaceEditData::GetInstance()->GetPart()->GetName(); // 注意，要使用之前板材中零件的名称。
		pNewPart.reset(new Part(strPartName, _T(""), pNewCadData, pNewCamDataList));

		// 保存它到一个目录下。
		DBConnectPtr pMainDBCon = DataBaseManager::GetInstance()->GetMainDBConnect();
		if (IDataCenterItemPtr pDataCenterItem = DataCenterItemLoader::LoadProductTree(pMainDBCon)->SearchDataCenterItem(PartsLibraryFolderID)) {
			FolderNodePtr pSaveFolder = boost::dynamic_pointer_cast<FolderNode>(pDataCenterItem);
			PartManager::SaveNewPart(pNewPart, pSaveFolder);
		} else {
			LogMgr::DebugWarn(_T("553908"));
			return;
		}

		// 关闭当前零件并更新Gui。
		DocManager::GetActiveDocument()->OnCloseDocument();
		UpdateGui();
	}

	// 替换掉任务中指定的零件，并更新界面。
	{
		// 验证一下当前文档是不是在位编辑零件之前的当前文档。
		const CObList& tabGroups = ((CMDIFrameWndEx*)AfxGetApp()->GetMainWnd())->GetMDITabGroups();
		CMFCTabCtrl* pTabCtrl = (CMFCTabCtrl*)tabGroups.GetHead();
		if (pTabCtrl->GetActiveTab() != PartInplaceEditData::GetInstance()->GetTaskTabIndex()) {
			LogMgr::DebugWarn(_T("290076"));
			return;
		}

		// 替换掉任务对象中对应的零件。
		SheetListPtr pSheetList = ReplacePartInTask(pNewPart);

		// 这里重新显示被修改了板材的缩略图。零件管理器中的零件缩略图会自动刷新。
		for (unsigned int i = 0; i < pSheetList->size(); i++) {
			const Sheet* pSheet = pSheetList->operator[](i).get();
			TaskSheetManagementBar::GetInstance()->GetTaskSheetManagementDlg()->RedrawSheetThumbnail(pSheet->GetID());
		}
		ClWindowFinder::GetCurrentView_1()->Invalidate(); // 刷新一下当前板材视图。
	}

	// 最后重置在位编辑数据。
	PartInplaceEditData::GetInstance()->ResetInplaceEditData();
}

SheetListPtr PartInplaceEditManager::ReplacePartInTask(PartPtr pNewPart)
{
	SheetListPtr pSheetList(new SheetList);

	// 得到当前任务对象。
	TaskItemPtr pTaskItem;
	if (TaskView* pTaskView = dynamic_cast<TaskView*>(ClWindowFinder::GetCurrentView_2())) {
		pTaskItem = pTaskView->GetTaskItem();
	}
	else {
		LogMgr::DebugWarn(_T("653981"));
		return pSheetList;
	}

	// 替换掉对应的TaskPart中的Part对象，保持其他属性不变。
	LONGLONG iPartIDInTask = PartInplaceEditData::GetInstance()->GetPart()->GetID();
	TaskPartPtr pTaskPart = pTaskItem->GetTaskPartByPartID(iPartIDInTask);
	if (pTaskPart) {
		pTaskPart->SetPart(pNewPart);
		pTaskPart->SetModified(TRUE);
	}
	else {
		LogMgr::DebugWarn(_T("775309"));
		return pSheetList;
	}

	// 更新任务中的板材。
	for (unsigned int i = 0; i < pTaskItem->GetSheetList()->size(); i++)
	{
		SheetPtr pSheet = pTaskItem->GetSheetList()->operator[](i);

		// 如果该板材上没有这个零件，则不需处理该板材。
		if (pSheet->GetPartInstanceCount(iPartIDInTask) == 0) {
			continue;
		}

		LONGLONG iSheetParamConfigID = pSheet->GetParamConfigID();
		for (unsigned int j = 0; j < pSheet->GetPartPlacements()->size(); j++)
		{
			PartPlacementPtr pPartPlacement = pSheet->GetPartPlacements()->operator[](j);
			if (pPartPlacement->GetPart()->GetID() != iPartIDInTask) {
				continue;
			}

			Rect2D oldRectBox = pPartPlacement->GetRectBox();

			// 替换零件放置中的零件。
			pPartPlacement->SetPart(pNewPart);

			// 替换零件放置中的cam数据，这个cam数据对应的参数配置和板材的要相同。
			PartCamDataPtr pPartCamData4PartPlacement;
			const PartCamData* pPartCamData = pNewPart->GetCamDataList()->GetPartCamDataByParamConfigID(iSheetParamConfigID).get();
			if (pPartCamData) {
				pPartCamData4PartPlacement = boost::dynamic_pointer_cast<PartCamData>(pPartCamData->Clone());
				LoopStartCutProcessor::UpdateCache(pPartCamData4PartPlacement->GetLoopStaEndCutList()); // 更新缓存。
			}
			else { // 此时需要根据板材的参数配置创建一套cam数据。
				unsigned int iAssignLimit = CLOptionInfo::GetInstance()->GetImpExpConfig()->GetAssignLimit();
				pPartCamData4PartPlacement = PartBuilder::BuildPartCamData(pNewPart->GetCadData(), iSheetParamConfigID, iAssignLimit, FALSE);
			}
			pPartPlacement->SetCamData(pPartCamData4PartPlacement);

			// 把修改过的零件放置的中心点对齐到修改前的中心点。
			Vector2D vect(pPartPlacement->GetRectBox().GetCenterPt(), oldRectBox.GetCenterPt());
			Matrix2D mat = pPartPlacement->GetMatrix();
			mat.Transfer(vect);
			pPartPlacement->SetMatrix(mat);
		}
		pSheet->GetCutSequences()->clear();
		pSheet->SetPreviewData(SheetManager::CalcPreviewData(pSheet.get()));
		pSheet->SetModified(TRUE);
		pSheetList->push_back(pSheet);
	}

	return pSheetList;
}

void PartInplaceEditManager::UpdateGui()
{
	// 更新ribbon。
	{
		RibbonBar* pRibbonBar = UIManager::GetInstance()->GetRibbonBar();

		// 更新各个工具条的可见性。
		pRibbonBar->SetCategoryVisibility();

		// 更新“开始”工具条。
		CMFCRibbonCategory* pStartCategory = pRibbonBar->GetCategory(0);
		while (pStartCategory->GetPanelCount() > 0)
			pStartCategory->RemovePanel(0);
		pRibbonBar->AddGroups2StartCategory(pStartCategory);

		pRibbonBar->SetActiveCategory(pStartCategory);
		pRibbonBar->RecalcLayout();
	}

	// 更新文档tab标签。
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetApp()->GetMainWnd();
	{
		// 更新文档tab标签的风格。
		pMainFrame->SetTabControlStyle();

		// 更新文档tab标签的显示。
		const CObList& TabGroups = pMainFrame->GetMDITabGroups();
		CMFCTabCtrl* pTabCtrl = (CMFCTabCtrl*)TabGroups.GetHead();
		if (PartInplaceEditData::GetInstance()->GetIsUnderInPlaceEditMode()) { // 此时在位编辑标记已经设为true，只需显示要编辑的零件。
			for (unsigned int i = 0; i < pTabCtrl->GetTabsNum() - 1; i++) // 要进行在位编辑的零件在最后一个标签，所以循环这么写。
				pTabCtrl->ShowTab(i, FALSE);
		}
		else { // 此时在位编辑标记已经设为false。
			for (unsigned int i = 0; i < pTabCtrl->GetTabsNum(); i++) // 此时进行在位编辑的零件已经被关闭了，所以循环这么写。
				pTabCtrl->ShowTab(i, TRUE);
			pTabCtrl->SetActiveTab(PartInplaceEditData::GetInstance()->GetTaskTabIndex()); // 恢复显示在位编辑前的任务文档。
		}
	}

	// 更新主窗口的关闭按钮。
	pMainFrame->UpdateMainWindowCloseButton();
}

END_CUTLEADER_NAMESPACE()

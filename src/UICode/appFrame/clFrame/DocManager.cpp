#include "StdAfx.h"
#include "DocManager.h"

#include "IntegerList.h"
#include "DateHelper.h"
#include "LogMessage.h"
#include "LogMgr.h"
#include "clBaseDataResource.h"
#include "DataItem.h"
#include "FolderNode.h"
#include "DataSaveInfoDlg.h"
#include "DataBaseManager.h"
#include "ProgressData.h"
#include "ProgressDlg.h"
#include "DataCenterItemLoader.h"
#include "DataCenterItemWriter.h"
#include "ClWindowFinder.h"
#include "PathHelper.h"
#include "DataCenterItemList.h"

#include "IMaterialSize.h"
#include "MaterialLibLoader.h"
#include "MaterialLibWriter.h"
#include "ParamConfigLoader.h"
#include "DefaultParamConfig.h"

#include "Part.h"
#include "PartCadData.h"
#include "PartCamData.h"
#include "PartCamDataList.h"
#include "CamDataSelDlg.h"
#include "PartItem.h"
#include "PartLoader.h"
#include "PartWriter.h"
#include "PartInfoDlg.h"
#include "PartBuilder.h"
#include "PartManager.h"
#include "LoopCutFeatureManager.h"
#include "Sheet.h"
#include "SheetLoader.h"
#include "SheetWriter.h"
#include "SheetProcessor.h"
#include "SheetInfoDlg.h"
#include "SheetManager.h"
#include "clSheetResource.h"
#include "NestScheduleDlg.h"
#include "NestJob.h"
#include "NestJobMgr.h"
#include "clDataManagerResource.h"
#include "DataItemTracker.h"
#include "RecentData.h"
#include "RecentDataList.h"
#include "clFrameResource.h"
#include "PartDocument.h"
#include "PartView.h"
#include "UIManager.h"
#include "TaskPartManagementBar.h"
#include "TaskSheetManagementBar.h"
#include "TaskItemManager.h"
#include "SheetList.h"
#include "TaskView.h"
#include "TaskItem.h"
#include "TaskDocument.h"
#include "MaterialStockWarningDlg.h"
#include "NewPartFromDxfDwgDlg.h"
#include "SheetSubView.h"
#include "ImportManager.h"
#include "TaskInfoDlg.h"
#include "ClTaskResource.h"
#include "DefaultParamDlg.h"
#include "PartHelper.h"
#include "clCanvasGeometryUIResource.h"
#include "NestTask.h"
#include "NestPartList.h"
#include "TaskPart.h"
#include "NestPart.h"
#include "TaskPartList.h"
#include "DxfDwgConvertor.h"
#include "SheetHelper.h"
#include "TaskHelper.h"
#include "clFrameUIResource.h"
#include "PartInplaceEditData.h"

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

void DocManager::NewPartDoc(CMultiDocTemplate* pPartDocTemplate)
{
	PartInfoDlg partInfoDlg(AfxGetApp()->GetMainWnd());
	if (partInfoDlg.DoModal() == IDOK)
	{
		// open the part.
		pPartDocTemplate->OpenDocumentFile(NULL);

		// create the part item from the part, now there are only one cam data in part.
		PartPtr pPart = partInfoDlg.GetPart();
		PartItemPtr pPartItem(new PartItem(pPart, pPart->GetCamDataList()->at(0)));

		// set PartItem into PartDocument.
		PartDocument* pPartDoc = dynamic_cast<PartDocument*>(GetActiveDocument());
		pPartDoc->SetPartItem(pPartItem);

		// init the part view.
		PartView* pPartView = dynamic_cast<PartView*>(GetActivePartView());
		pPartView->Init();
	}
}

void DocManager::NewPartDocFromDxfDwg(CMultiDocTemplate* pPartDocTemplate)
{
	NewPartFromDxfDwgDlg impFileDlg;
	if (impFileDlg.DoModal() == IDOK)
	{
		CString strFilePath = impFileDlg.GetPathName();

		// 把dxf/dwg文件导入成零件。
		PartPtr pPart = PartHelper::ImportDxfDwg2Part(strFilePath);

		// build the part item.
		PartItemPtr pPartItem(new PartItem(pPart, pPart->GetCamDataList()->at(0)));

		// open the part item.
		pPartDocTemplate->OpenDocumentFile(NULL);

		// set PartItem into PartDocument.
		PartDocument* pPartDoc = dynamic_cast<PartDocument*>(DocManager::GetActiveDocument());
		pPartDoc->SetPartItem(pPartItem);

		// init the part view.
		PartView* pPartView = GetActivePartView();
		pPartView->Init();
	}
}

void DocManager::NewPartDocUnderPartInplaceEditMode(CMultiDocTemplate* pPartDocTemplate)
{
	if (!PartInplaceEditData::GetInstance()->GetIsUnderInPlaceEditMode()) {
		LogMgr::DebugWarn(_T("455683"));
		return;
	}

	PartInfoDlg partInfoDlg(AfxGetApp()->GetMainWnd());
	if (partInfoDlg.DoModal() == IDOK)
	{
		// create the part item from the part, now there are only one cam data in part.
		PartPtr pPart = partInfoDlg.GetPart();
		PartItemPtr pPartItem(new PartItem(pPart, pPart->GetCamDataList()->at(0)));

		// set PartItem into PartDocument.
		PartDocument* pPartDoc = dynamic_cast<PartDocument*>(GetActiveDocument());
		pPartDoc->SetPartItem(pPartItem);

		// init the part view.
		PartView* pPartView = dynamic_cast<PartView*>(GetActivePartView());
		pPartView->Init();
	}
}

void DocManager::NewPartDocFromDxfDwgUnderPartInplaceEditMode(CMultiDocTemplate* pPartDocTemplate)
{
	if (!PartInplaceEditData::GetInstance()->GetIsUnderInPlaceEditMode()) {
		LogMgr::DebugWarn(_T("455689"));
		return;
	}

	NewPartFromDxfDwgDlg impFileDlg;
	if (impFileDlg.DoModal() == IDOK)
	{
		// 把dxf/dwg文件导入成零件。
		CString strFilePath = impFileDlg.GetPathName();
		PartPtr pPart = PartHelper::ImportDxfDwg2Part(strFilePath);
		PartItemPtr pPartItem(new PartItem(pPart, pPart->GetCamDataList()->at(0)));

		// set PartItem into PartDocument.
		PartDocument* pPartDoc = dynamic_cast<PartDocument*>(DocManager::GetActiveDocument());
		pPartDoc->SetPartItem(pPartItem);

		// init the part view.
		PartView* pPartView = GetActivePartView();
		pPartView->Init();
	}
}

void DocManager::NewTaskDoc(CMultiDocTemplate* pTaskDocTemplate)
{
	TaskInfoDlg taskInfoDlg;
	if (taskInfoDlg.DoModal() == IDOK)
	{
		TaskItemPtr pTaskItem = taskInfoDlg.GetTaskItem();

		// 初始化任务文档和任务视图。
		TaskDocument* pTaskDoc = (TaskDocument*)pTaskDocTemplate->OpenDocumentFile(NULL);
		pTaskDoc->SetTaskItem(pTaskItem);
		POSITION viewPosition = pTaskDoc->GetFirstViewPosition();
		TaskView* pTaskView = (TaskView*)pTaskDoc->GetNextView(viewPosition);
		pTaskView->InitTaskView();
		pTaskDoc->SetTitle(pTaskItem->GetName());

		// 更新任务管理面板的数据。
		TaskPartManagementBar::GetInstance()->GetTaskPartManagementDlg()->Redisplay_TaskPartManagementDlg(pTaskItem);
		TaskSheetManagementBar::GetInstance()->GetTaskSheetManagementDlg()->Redisplay_TaskSheetManagementDlg(pTaskItem);

		ClWindowFinder::GetCurrentView_2()->SendMessage(WM_UpdateLoopToolPanel, 0, 0);
	}
}

void DocManager::NewTaskDocBySingleDocMode(CMultiDocTemplate* pTaskDocTemplate)
{
	vector<CMDIChildWndEx*> allChildFrames = ClWindowFinder::FindAllChildFrames();
	if (allChildFrames.size() > 1)
	{
		LogMgr::DebugWarn(_T("470943"));
		return;
	}

	TaskInfoDlg taskInfoDlg;
	if (taskInfoDlg.DoModal() == IDOK)
	{
		// 如果之前打开了一个任务，这里保存并关闭之。
		if (allChildFrames.size() == 1)
			CloseCurrentTaskDoc();

		TaskItemPtr pTaskItem = taskInfoDlg.GetTaskItem();

		// 初始化任务文档和任务视图。
		{
			TaskDocument* pTaskDoc = (TaskDocument*)pTaskDocTemplate->OpenDocumentFile(NULL);
			pTaskDoc->SetTaskItem(pTaskItem);
			POSITION viewPosition = pTaskDoc->GetFirstViewPosition();
			TaskView* pTaskView = (TaskView*)pTaskDoc->GetNextView(viewPosition);
			pTaskView->InitTaskView();
			pTaskDoc->SetTitle(pTaskItem->GetName());
		}

		// 更新任务管理面板的数据。
		TaskPartManagementBar::GetInstance()->GetTaskPartManagementDlg()->Redisplay_TaskPartManagementDlg(pTaskItem);
		TaskSheetManagementBar::GetInstance()->GetTaskSheetManagementDlg()->Redisplay_TaskSheetManagementDlg(pTaskItem);

		ClWindowFinder::GetCurrentView_2()->SendMessage(WM_UpdateLoopToolPanel, 0, 0);
	}
}

void DocManager::CloseCurrentTaskDoc()
{
	vector<CMDIChildWndEx*> allChildFrames = ClWindowFinder::FindAllChildFrames();
	if (allChildFrames.size() != 1)
	{
		LogMgr::DebugWarn(_T("398723"));
		return;
	}

	TaskDocument* pTaskDoc = (TaskDocument*)GetActiveDocument();
	if (!pTaskDoc)
	{
		LogMgr::DebugWarn(_T("220965"));
		return;
	}

	// 保存的任务对象。
	TaskItemPtr pTaskItem = pTaskDoc->GetTaskItem();
	if (pTaskItem->IsModified())
	{
		CString strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);
		CString str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SaveTaskDoc);
		str.Replace(_T("WILLBEREPLACED1"), pTaskItem->GetName());
		if (::MessageBox(AfxGetApp()->GetMainWnd()->m_hWnd, str, strProductName, MB_YESNO | MB_ICONQUESTION) == IDYES)
		{
			DataProperty taskProp;
			DBConnectPtr pMainDBCon = DataBaseManager::GetInstance()->GetMainDBConnect();
			if (!DataCenterItemLoader::GetDataProp(pMainDBCon, pTaskItem->GetID(), taskProp)) // 数据库中没有该任务。
			{
				pTaskItem->SetCreateTime(DateHelper::GetCurrentTime());
				pTaskItem->SetModifyTime(DateHelper::GetCurrentTime());
				FolderNodePtr pParentFolder = DataCenterItemLoader::LoadProductTree(pMainDBCon);
				TaskItemManager::SaveNewTaskItem(pTaskItem, pParentFolder);
			}
			else // 数据库中已经有了该任务。
				TaskItemManager::SaveExistTaskItem(pTaskItem);
		}
	}
	
	// 关闭任务文档。
	pTaskDoc->OnCloseDocument();
}

void DocManager::NewDefaultTaskDoc(CMultiDocTemplate* pTaskDocTemplate)
{
	// 新建任务对象
	TaskItemPtr pTaskItem;
	{
		pTaskItem.reset(new TaskItem);
		CString strTaskName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_NEW_TASK);
		pTaskItem->SetName(strTaskName);

		DBConnectPtr pMatLibDBCon = DataBaseManager::GetInstance()->GetMatLibDBConnect();
		LONGLONG iMatSizeID = MaterialLibLoader::GetDflMatSize(pMatLibDBCon);
		DBConnectPtr pExpLibDBCon = DataBaseManager::GetInstance()->GetExpLibDBConnect();
		DefaultParamConfigPtr pDefaultParamConfig = ParamConfigLoader::LoadDflParamConfig(pExpLibDBCon);

		// 创建底图板材。
		{
			LONGLONG iMaterialThicknessID = MaterialLibLoader::GetMatThickID(pMatLibDBCon, iMatSizeID);
			LONGLONG iCanvasMaterialSizeID = MaterialLibLoader::GetCanvasMaterialSizeID(pMatLibDBCon, iMaterialThicknessID);
			CString strCanvasSheetName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_Canvas);
			SheetPtr pSheet = SheetManager::CreateCanvasSheet(strCanvasSheetName, iCanvasMaterialSizeID, pDefaultParamConfig->GetParamConfigID());
			SheetProcessor::UpdateSheetCache(pSheet);
			pTaskItem->GetSheetList()->push_back(pSheet);
		}

		// 创建初始板材。
		{
			CString strInitialSheetName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SHEET);
			strInitialSheetName += _T("-1");
			SheetPtr pSheet = SheetManager::CreateNormalSheet(strInitialSheetName, iMatSizeID, pDefaultParamConfig->GetParamConfigID());
			SheetProcessor::UpdateSheetCache(pSheet);
			pTaskItem->GetSheetList()->push_back(pSheet);
		}
	}

	// 打开任务文档
	CDocument* pDoc = pTaskDocTemplate->OpenDocumentFile(NULL);

	// 初始化TaskDoc对象。
	TaskDocument* pTaskDoc = (TaskDocument*)pDoc;
	pTaskDoc->SetTaskItem(pTaskItem);

	// 得到TaskView对象。
	POSITION viewPosition = pTaskDoc->GetFirstViewPosition();
	TaskView* pTaskView = (TaskView*)pTaskDoc->GetNextView(viewPosition);
	pTaskView->InitTaskView();

	// 设置标题
	pTaskDoc->SetTitle(pTaskItem->GetName());

	// 更新任务管理面板的数据。
	TaskPartManagementBar::GetInstance()->GetTaskPartManagementDlg()->Redisplay_TaskPartManagementDlg(pTaskItem);
	TaskSheetManagementBar::GetInstance()->GetTaskSheetManagementDlg()->Redisplay_TaskSheetManagementDlg(pTaskItem);

	ClWindowFinder::GetCurrentView_2()->SendMessage(WM_UpdateLoopToolPanel, 0, 0);
}

void DocManager::NewTaskAndAddDxf2Canvas(CMultiDocTemplate* pTaskDocTemplate, const CString& strDxfFilePath)
{
	// 新建任务对象
	TaskItemPtr pTaskItem;
	{
		pTaskItem.reset(new TaskItem);
		CString strTaskName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_NEW_TASK);
		pTaskItem->SetName(strTaskName);

		DBConnectPtr pMatLibDBCon = DataBaseManager::GetInstance()->GetMatLibDBConnect();
		LONGLONG iMatSizeID = MaterialLibLoader::GetDflMatSize(pMatLibDBCon);
		DBConnectPtr pExpLibDBCon = DataBaseManager::GetInstance()->GetExpLibDBConnect();
		DefaultParamConfigPtr pDefaultParamConfig = ParamConfigLoader::LoadDflParamConfig(pExpLibDBCon);

		// 创建底图板材。
		{
			LONGLONG iMaterialThicknessID = MaterialLibLoader::GetMatThickID(pMatLibDBCon, iMatSizeID);
			LONGLONG iCanvasMaterialSizeID = MaterialLibLoader::GetCanvasMaterialSizeID(pMatLibDBCon, iMaterialThicknessID);
			CString strCanvasSheetName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_Canvas);
			SheetPtr pSheet = SheetHelper::CreateCanvasSheetFromDxf(strCanvasSheetName, iCanvasMaterialSizeID, pDefaultParamConfig->GetParamConfigID(), strDxfFilePath);
			SheetProcessor::UpdateSheetCache(pSheet);
			pTaskItem->GetSheetList()->push_back(pSheet);
		}

		// 创建初始板材。
		{
			CString strInitialSheetName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SHEET);
			strInitialSheetName += _T("-1");
			SheetPtr pSheet = SheetManager::CreateNormalSheet(strInitialSheetName, iMatSizeID, pDefaultParamConfig->GetParamConfigID());
			SheetProcessor::UpdateSheetCache(pSheet);
			pTaskItem->GetSheetList()->push_back(pSheet);
		}
	}

	// 打开任务文档
	CDocument* pDoc = pTaskDocTemplate->OpenDocumentFile(NULL);

	// 初始化TaskDoc对象。
	TaskDocument* pTaskDoc = (TaskDocument*)pDoc;
	pTaskDoc->SetTaskItem(pTaskItem);

	// 得到TaskView对象。
	POSITION viewPosition = pTaskDoc->GetFirstViewPosition();
	TaskView* pTaskView = (TaskView*)pTaskDoc->GetNextView(viewPosition);
	pTaskView->InitTaskView();

	// 设置标题
	pTaskDoc->SetTitle(pTaskItem->GetName());

	// 更新任务管理面板的数据。
	TaskPartManagementBar::GetInstance()->GetTaskPartManagementDlg()->Redisplay_TaskPartManagementDlg(pTaskItem);
	TaskSheetManagementBar::GetInstance()->GetTaskSheetManagementDlg()->Redisplay_TaskSheetManagementDlg(pTaskItem);

	ClWindowFinder::GetCurrentView_2()->SendMessage(WM_UpdateLoopToolPanel, 0, 0);
}

void DocManager::SetDefaultParam()
{
	DefaultParamDlg dlg;
	dlg.DoModal();
}

void DocManager::NewTaskDocFromNesting(CMultiDocTemplate* pTaskDocTemplate, bool bSingleDocMode)
{
	if (bSingleDocMode)
	{
		if (ClWindowFinder::FindAllChildFrames().size() > 1)
		{
			LogMgr::DebugWarn(_T("409512"));
			return;
		}
	}

	NestScheduleDlg nestScheduleDlg(AfxGetApp()->GetMainWnd());
	if (nestScheduleDlg.DoModal() == IDC_BTN_EXECUTE_NEST)
	{
		// 如果之前打开了一个任务，这里保存并关闭之。
		if (ClWindowFinder::FindAllChildFrames().size() == 1)
			CloseCurrentTaskDoc();

		// 把排版作业转化为任务对象。
		TaskItemPtr pTaskItem(new TaskItem);
		{
			pTaskItem->SetName(MultiLanguageMgr::GetInstance()->TranslateString(IDS_NEW_TASK));
			NestJobPtr pNestJob = nestScheduleDlg.GetNestJob();

			// 添加任务零件。
			const NestPartList* pNestParts = pNestJob->GetNestTask()->GetNestPartList().get();
			for (unsigned int i = 0; i < pNestParts->size(); i++)
			{
				const NestPart* pNestPart = pNestParts->operator [](i).get();
				LONGLONG iPartID = pNestPart->GetPartItem()->GetPart()->GetID();
				DBConnectPtr pPartDBCon = DataBaseManager::ConnectPartDB(iPartID);
				PartPtr pPart = PartLoader::LoadPart(pPartDBCon, iPartID, PARTLOAD_ALL);
				TaskPartPtr pTaskPart(new TaskPart(pPart, pNestPart->GetPriority(), pNestPart->GetRotStyle(), pNestPart->GetNestCount()));
				pTaskItem->GetTaskPartList()->push_back(pTaskPart);
			}

			// 创建底图板材。
			{
				DBConnectPtr pExpLibDBCon = DataBaseManager::GetInstance()->GetExpLibDBConnect();
				DefaultParamConfigPtr pDefaultParamConfig = ParamConfigLoader::LoadDflParamConfig(pExpLibDBCon);

				DBConnectPtr pMatLibDBCon = DataBaseManager::GetInstance()->GetMatLibDBConnect();
				LONGLONG iMaterialSizeID = MaterialLibLoader::GetDflMatSize(pMatLibDBCon);
				LONGLONG iMaterialThicknessID = MaterialLibLoader::GetMatThickID(pMatLibDBCon, iMaterialSizeID);
				LONGLONG iCanvasMaterialSizeID = MaterialLibLoader::GetCanvasMaterialSizeID(pMatLibDBCon, iMaterialThicknessID);

				CString strCanvasSheetName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_Canvas);
				SheetPtr pSheet = SheetManager::CreateCanvasSheet(strCanvasSheetName, iCanvasMaterialSizeID, pDefaultParamConfig->GetParamConfigID());
				SheetProcessor::UpdateSheetCache(pSheet);
				pTaskItem->GetSheetList()->push_back(pSheet);
			}

			// 初始化任务对象中的板材。
			for (unsigned int i = 0; i < pNestJob->GetSheetList()->size(); i++)
			{
				SheetPtr pSheet = pNestJob->GetSheetList()->operator [](i);
				pTaskItem->GetSheetList()->push_back(pSheet);
			}
		}

		// 打开任务文档
		CDocument* pDoc = pTaskDocTemplate->OpenDocumentFile(NULL);

		// 初始化TaskDoc对象。
		TaskDocument* pTaskDoc = (TaskDocument*)pDoc;
		pTaskDoc->SetTaskItem(pTaskItem);

		// 得到TaskView对象。
		POSITION viewPosition = pTaskDoc->GetFirstViewPosition();
		TaskView* pTaskView = (TaskView*)pTaskDoc->GetNextView(viewPosition);
		pTaskView->InitTaskView();

		// 设置标题
		pTaskDoc->SetTitle(pTaskItem->GetName());

		// 更新任务管理面板的数据。
		TaskPartManagementBar::GetInstance()->GetTaskPartManagementDlg()->Redisplay_TaskPartManagementDlg(pTaskItem);
		TaskSheetManagementBar::GetInstance()->GetTaskSheetManagementDlg()->Redisplay_TaskSheetManagementDlg(pTaskItem);

		ClWindowFinder::GetCurrentView_2()->SendMessage(WM_UpdateLoopToolPanel, 0, 0);
	}
}

void DocManager::OpenPartDoc(DataItemPtr pDataItem, CMultiDocTemplate* pPartDocTemplate)
{
	// check whether "pDataItem" is opened.
	if (ActivePartDoc(pDataItem, pPartDocTemplate))
		return;

	// init connection to the part db file.
	DBConnectPtr pPartDBCon = DataBaseManager::ConnectPartDB(pDataItem->GetDataID());

	// if the part has more than one cam data, select one.
	LONGLONG iSelCAMDatdID = INVALID_ID;
	vector<DataProperty> camDataList = PartLoader::GetPartCamDataProp(pPartDBCon);
	if (camDataList.size() > 1)
	{
		CamDataSelDlg dlg;
		dlg.SetPartID(pDataItem->GetDataID());
		if (dlg.DoModal() == IDOK)
			iSelCAMDatdID = dlg.GetSelCamDataID();
		else
			return;
	}
	else if (camDataList.size() == 1)
	{
		DataProperty prop = camDataList[0];
		const CComVariant& v = prop.GetItem(0).GetValue();
		iSelCAMDatdID = v.llVal;
	}

	// load the PartItem object, and open it.
	PartItemPtr pPartItem = PartLoader::LoadPartItem(pPartDBCon, pDataItem->GetDataID(), iSelCAMDatdID);
	pPartDocTemplate->OpenDocumentFile(NULL);

	// set PartItem into PartDocument.
	PartDocument* pPartDoc = dynamic_cast<PartDocument*>(GetActiveDocument());
	pPartDoc->SetPartItem(pPartItem);

	// 在打开了文档后，更新文档的访问历史。
	UpdateDocHistoryAfterOpen(pDataItem);

	// init the part view.
	PartView* pPartView = dynamic_cast<PartView*>(GetActivePartView());
	pPartView->Init();
}

void DocManager::UpdateDocHistoryAfterOpen(DataItemPtr pDataItem)
{
	DataItemTracker::GetInstance()->AddDataItem(pDataItem);

	// update data which opened recently.
	RecentDataListPtr pRecentDataList = DataItemTracker::GetInstance()->GetRecentData();
	RecentDataPtr pRecentData = pRecentDataList->GetItemByDataID(pDataItem->GetDataID());
	if (pRecentData)
		pRecentDataList->MoveToHead(pRecentData->GetID());
	else {
		RecentDataPtr pRecentData(new RecentData(pDataItem->GetFullPath(), pDataItem->GetDataID()));
		pRecentDataList->insert(pRecentDataList->begin(), pRecentData);
	}
	UIManager::GetInstance()->UpdateRecentDataMenu();
}

void DocManager::OpenPartDocUnderPartInplaceEditMode(DataItemPtr pDataItem, CMultiDocTemplate* pPartDocTemplate)
{
	if (!PartInplaceEditData::GetInstance()->GetIsUnderInPlaceEditMode()) {
		LogMgr::DebugWarn(_T("572341"));
		return;
	}

	// check whether "pDataItem" is opened.
	if (ActivePartDoc(pDataItem, pPartDocTemplate))
		return;

	// 选一个cam数据打开。
	LONGLONG iCamDataID = INVALID_ID;
	DBConnectPtr pPartDBCon = DataBaseManager::ConnectPartDB(pDataItem->GetDataID());
	vector<DataProperty> camDataList = PartLoader::GetPartCamDataProp(pPartDBCon);
	if (camDataList.size() > 1) {
		CamDataSelDlg dlg;
		dlg.SetPartID(pDataItem->GetDataID());
		if (dlg.DoModal() == IDOK)
			iCamDataID = dlg.GetSelCamDataID();
		else {
			return;
		}
	} else if (camDataList.size() == 1) {
		iCamDataID = camDataList[0].GetItem(0).GetValue().llVal;
	}

	// set PartItem into PartDocument.
	PartItemPtr pPartItem = PartLoader::LoadPartItem(pPartDBCon, pDataItem->GetDataID(), iCamDataID);
	PartDocument* pPartDoc = dynamic_cast<PartDocument*>(GetActiveDocument());
	pPartDoc->SetPartItem(pPartItem);

	// init the part view.
	PartView* pPartView = dynamic_cast<PartView*>(GetActivePartView());
	pPartView->Init();
}

void DocManager::OpenTaskDoc(DataItemPtr pDataItem, CMultiDocTemplate* pTaskDocTemplate)
{
	// check whether "pDataItem" is opened.
	if (ActiveTaskDoc(pDataItem, pTaskDocTemplate))
		return;

	// load task and open it.
	TaskItemPtr pTaskItem;
	CDocument* pDoc = NULL;
	{
		// load task.
		pTaskItem = TaskItemManager::LoadTaskItemFromDb(pDataItem->GetDataID());

		// 检查板材中的轮廓加工工艺。
		SheetListPtr pSheetList = pTaskItem->GetSheetList();
		for (unsigned int i = 0; i < pSheetList->size(); i++)
		{
			SheetPtr pSheet = pSheetList->operator [](i);
			SheetProcessor::CheckLoopCutFeature(pSheet);
			if (pSheet->IsModified())
				SheetManager::SaveExistSheet2SheetDbFile(pSheet);
		}

		// open the sheet.
		pDoc = pTaskDocTemplate->OpenDocumentFile(NULL);
	}

	// 初始化TaskDoc对象。
	TaskDocument* pTaskDoc = (TaskDocument*)pDoc;
	pTaskDoc->SetTaskItem(pTaskItem);

	// 在打开了文档后，更新文档的访问历史。
	UpdateDocHistoryAfterOpen(pDataItem);

	// 得到TaskView对象。
	POSITION viewPosition = pTaskDoc->GetFirstViewPosition();
	TaskView* pTaskView = (TaskView*)pTaskDoc->GetNextView(viewPosition);
	pTaskView->InitTaskView();

	// 设置标题
	pTaskDoc->SetTitle(pTaskItem->GetName());

	// 更新任务管理面板的数据。
	TaskPartManagementBar::GetInstance()->GetTaskPartManagementDlg()->Redisplay_TaskPartManagementDlg(pTaskItem);
	TaskSheetManagementBar::GetInstance()->GetTaskSheetManagementDlg()->Redisplay_TaskSheetManagementDlg(pTaskItem);

	ClWindowFinder::GetCurrentView_2()->SendMessage(WM_UpdateLoopToolPanel, 0, 0);
}

void  DocManager::OpenTaskDocBySingleDocMode(DataItemPtr pDataItem, CMultiDocTemplate* pTaskDocTemplate)
{
	vector<CMDIChildWndEx*> allChildFrames = ClWindowFinder::FindAllChildFrames();

	if (allChildFrames.size() == 0) {
		OpenTaskDoc(pDataItem, pTaskDocTemplate);
	}
	else if (allChildFrames.size() == 1) { // 当前打开了一个文档。
		CloseCurrentTaskDoc(); // 保存当前的任务对象，然后关闭之。
		OpenTaskDoc(pDataItem, pTaskDocTemplate); // 打开新的任务对象。
	}
	else {
		LogMgr::DebugWarn(_T("643643"));
	}
}

void DocManager::OpenSpecifiedSheet(CMultiDocTemplate* pTaskDocTemplate, long long iTaskID, long long iSheetID)
{
	// load task and open it.
	TaskItemPtr pTaskItem = TaskHelper::LoadTaskItem(iTaskID);
	CDocument* pDoc = pTaskDocTemplate->OpenDocumentFile(NULL);

	// 初始化任务视图。
	TaskDocument* pTaskDoc = (TaskDocument*)pDoc;
	pTaskDoc->SetTaskItem(pTaskItem);
	POSITION viewPosition = pTaskDoc->GetFirstViewPosition();
	TaskView* pTaskView = (TaskView*)pTaskDoc->GetNextView(viewPosition);
	pTaskView->InitTaskView();
	pTaskDoc->SetTitle(pTaskItem->GetName());

	// 激活指定板材。
	LONGLONG* pSheetID = new LONGLONG(iSheetID);
	pTaskView->SendMessage(WM_activeSheetSubView, (WPARAM)pSheetID, 0);
	delete pSheetID;

	// 在打开了文档后，更新文档的访问历史。
	{
		FolderNodePtr pFolderNode = DataCenterItemLoader::LoadProductTree(DataBaseManager::GetInstance()->GetMainDBConnect());
		DataCenterItemListPtr pAllDataItems(new DataCenterItemList);
		pFolderNode->GetAllDataItem(pAllDataItems);
		DataItemPtr pDataItem = boost::dynamic_pointer_cast<DataItem>(pAllDataItems->GetItemByDataID(iTaskID));
		if (!pDataItem) {
			LogMgr::DebugWarn(_T("742068"));
		}
		UpdateDocHistoryAfterOpen(pDataItem);
	}

	// 更新任务管理面板的数据。
	TaskPartManagementBar::GetInstance()->GetTaskPartManagementDlg()->Redisplay_TaskPartManagementDlg(pTaskItem);
	TaskSheetManagementBar::GetInstance()->GetTaskSheetManagementDlg()->Redisplay_TaskSheetManagementDlg(pTaskItem);

	ClWindowFinder::GetCurrentView_2()->SendMessage(WM_UpdateLoopToolPanel, 0, 0);
}

BOOL DocManager::ActivePartDoc(DataItemPtr pDataItem, CMultiDocTemplate* pPartDocTemplate)
{
	// check whether this DataItem is opened.
	if (DataItemTracker::GetInstance()->IsDataItemOpened(pDataItem->GetDataID()))
	{
		// find the opened document and active it.
		POSITION docpos = pPartDocTemplate->GetFirstDocPosition();
		while (docpos)
		{
			CDocument* pDoc = pPartDocTemplate->GetNextDoc(docpos);
			PartDocument* pPartDoc = dynamic_cast<PartDocument*>(pDoc);

			PartItemPtr pPartItem = pPartDoc->GetPartItem();
			if (pPartItem->GetPart()->GetID() == pDataItem->GetDataID())
			{
				// get the relative view of the document.
				POSITION pos = pDoc->GetFirstViewPosition();
				CView* pFirstView = pDoc->GetNextView(pos);

				// activate the view.
				CMDIFrameWnd* pFrame = (CMDIFrameWnd*)AfxGetMainWnd();
				pFrame->MDIActivate(pFirstView->GetParentFrame());

				return TRUE;
			}
		}
	}

	return FALSE;
}

BOOL DocManager::ActivePartDoc(LONGLONG iPartID, CMultiDocTemplate* pPartDocTemplate)
{
	// check whether this DataItem is opened.
	if (DataItemTracker::GetInstance()->IsDataItemOpened(iPartID))
	{
		// find the opened document and active it.
		POSITION docpos = pPartDocTemplate->GetFirstDocPosition();
		while (docpos)
		{
			CDocument* pDoc = pPartDocTemplate->GetNextDoc(docpos);
			PartDocument* pPartDoc = dynamic_cast<PartDocument*>(pDoc);

			PartItemPtr pPartItem = pPartDoc->GetPartItem();
			if (pPartItem->GetPart()->GetID() == iPartID)
			{
				// get the relative view of the document.
				POSITION pos = pDoc->GetFirstViewPosition();
				CView* pFirstView = pDoc->GetNextView(pos);

				// activate the view.
				CMDIFrameWnd* pFrame = (CMDIFrameWnd*)AfxGetMainWnd();
				pFrame->MDIActivate(pFirstView->GetParentFrame());

				return TRUE;
			}
		}
	}

	return FALSE;
}

BOOL DocManager::ActiveTaskDoc(DataItemPtr pDataItem, CMultiDocTemplate* pTaskDocTemplate)
{
	// check whether this DataItem is opened.
	if (DataItemTracker::GetInstance()->IsDataItemOpened(pDataItem->GetDataID()))
	{
		// find the opened document and active it.
		POSITION docpos = pTaskDocTemplate->GetFirstDocPosition();
		while (docpos)
		{
			CDocument* pDoc = pTaskDocTemplate->GetNextDoc(docpos);
			TaskDocument* pTaskDoc = dynamic_cast<TaskDocument*>(pDoc);

			TaskItemPtr pTaskItem = pTaskDoc->GetTaskItem();
			if (pTaskItem->GetID() == pDataItem->GetDataID())
			{
				// get the relative view of the document.
				POSITION pos = pDoc->GetFirstViewPosition();
				CView* pFirstView = pDoc->GetNextView(pos);

				// activate the view.
				CMDIFrameWnd* pFrame = (CMDIFrameWnd*)AfxGetMainWnd();
				pFrame->MDIActivate(pFirstView->GetParentFrame());

				return TRUE;
			}
		}
	}

	return FALSE;
}

void DocManager::UpdateDocHistoryAfterSaveAs(LONGLONG iOldDataID, DataItemPtr pNewDataItem)
{
	// update the current opened doc.
	DataItemTracker::GetInstance()->RemoveDataItem(iOldDataID);
	DataItemTracker::GetInstance()->AddDataItem(pNewDataItem);

	// update data which opened recently.
	RecentDataListPtr pRecentDataList = DataItemTracker::GetInstance()->GetRecentData();
	RecentDataPtr pRecentData(new RecentData(pNewDataItem->GetFullPath(), pNewDataItem->GetDataID()));
	pRecentDataList->insert(pRecentDataList->begin(), pRecentData);
	UIManager::GetInstance()->UpdateRecentDataMenu();
}

void DocManager::SaveAsPart(PartDocument* pPartDoc)
{
	/************************************************************************/
	// prepare some data.

	CString strProductName, strMsgContent, strTaskName;
	strTaskName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_TASK_SAVE_PART);
	strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);

	PartItemPtr pOldPartItem = pPartDoc->GetPartItem();
	/************************************************************************/

	// choose where to save.
	DataSaveInfoDlg dlg;
	DBConnectPtr pMainDBCon = DataBaseManager::GetInstance()->GetMainDBConnect();
	dlg.Init(strTaskName, DataCenterItemLoader::LoadProductTree(pMainDBCon), pOldPartItem->GetPart()->GetName());
	if (dlg.DoModal() == IDOK)
	{
		FolderNodePtr pParentFolder = dlg.GetSelectFolder();
		CString strName = dlg.GetName();

		// check the integrity of the part.
		DBConnectPtr pExpLibDBCon = DataBaseManager::GetInstance()->GetExpLibDBConnect();
		PartCadDataPtr pOldCadData = pOldPartItem->GetPart()->GetCadData();
		PartCamDataPtr pOldCamData = pOldPartItem->GetCamData();
		ParamConfigPtr pParamConfig = ParamConfigLoader::LoadParamConfig(pExpLibDBCon, pOldCamData->GetParamConfigID());
		LoopCutFeatureManager::CheckIntegrity(pOldCadData->GetPatternLoopList().get(), pOldCadData->GetLoopTopologyItems(), pOldCamData->GetLoopFeatureList(),
			pParamConfig.get(), TRUE, FALSE, FALSE, Matrix2D());

		// 克隆零件并保存。
		PartPtr pNewPart = PartBuilder::ClonePart(pOldPartItem->GetPart().get());
		pNewPart->SetName(strName);
		DataItemPtr pDataItem = PartManager::SaveNewPart(pNewPart, pParentFolder);

		// 新建一个零件项
		int iPartCamDataIndex = pOldPartItem->GetPart()->GetCamDataList()->GetPartCamDataIndex(pOldPartItem->GetCamData().get());
		PartCamDataPtr pNewPartCamData = pNewPart->GetCamDataList()->operator [](iPartCamDataIndex);
		PartItemPtr pNewPartItem(new PartItem(pNewPart, pNewPartCamData));
		pNewPartItem->SetModified(FALSE);

		// set the new part item to doc.
		pPartDoc->SetPartItem(pNewPartItem);

		// 在另存了文档后，更新文档的访问历史。
		UpdateDocHistoryAfterSaveAs(pOldPartItem->GetPart()->GetID(), pDataItem);

		// update the part view.
		PartView* pPartView = GetActivePartView();
		pPartView->Init();

		// create a message.
		strMsgContent = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PART_SAVED);
		strMsgContent.Replace(_T("WILLBEREPLACED1"), pOldPartItem->GetPart()->GetName());
		LogMessagePtr pLogMsg(new LogMessage(DateHelper::GetCurrentTime(), LOG_MSG_INFO, strTaskName, strMsgContent));
		LogMgr::WriteLog(pLogMsg);
		POSITION pos = pPartDoc->GetFirstViewPosition();
		MessageBox(pPartDoc->GetNextView(pos)->m_hWnd, strMsgContent, strProductName, MB_OK | MB_ICONINFORMATION);
	}
}

void DocManager::SaveAsTask(TaskDocument* pTaskDoc)
{
	CString strProductName, strMsgContent, strTaskName;
	strTaskName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_TASK_SAVE_TASK);
	strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);
	TaskItemPtr pOldTaskItem = pTaskDoc->GetTaskItem();

	// 克隆任务
	TaskItemPtr pNewTaskItem = TaskItemManager::CloneTaskItem_notClonePart(pOldTaskItem.get());

	// 找出没有足够库存的材料。
	vector<pair<DataProperty, int>> materialData_noEnoughStock = TaskItemManager::GetMaterialData_noEnoughStock(pNewTaskItem.get());
	if (materialData_noEnoughStock.size() > 0) // 显示对话框让用户选择。
	{
		MaterialStockWarningDlg dlg;
		dlg.Set_MaterialData_NoEnoughStock(materialData_noEnoughStock);
		if (dlg.DoModal() == IDC_BTN_CancelSave) {
			return;
		}
	}

	// choose where to save.
	DataSaveInfoDlg dlg;
	DBConnectPtr pMainDBCon = DataBaseManager::GetInstance()->GetMainDBConnect();
	dlg.Init(strTaskName, DataCenterItemLoader::LoadProductTree(pMainDBCon), pNewTaskItem->GetName());
	if (dlg.DoModal() == IDOK)
	{
		FolderNodePtr pFolder = dlg.GetSelectFolder();
		CString strName = dlg.GetName();
		pNewTaskItem->SetName(strName);

		// 初始化任务文档。
		pTaskDoc->SetTaskItem(pNewTaskItem);
		pTaskDoc->SetTitle(pNewTaskItem->GetName());

		// 保存新的任务。
		DataItemPtr pDataItem = TaskItemManager::SaveNewTaskItem(pNewTaskItem, pFolder);

		// 在另存了文档后，更新文档的访问历史。
		UpdateDocHistoryAfterSaveAs(pOldTaskItem->GetID(), pDataItem);

		// update the task view.
		TaskView* pTaskView = dynamic_cast<TaskView*>(ClWindowFinder::GetCurrentView_2());
		if (!pTaskView) {
			LogMgr::DebugWarn(_T("46934"));
		}
		pTaskView->InitTaskView_4SaveAsTask();

		// 更新任务管理面板的数据。
		TaskPartManagementBar::GetInstance()->GetTaskPartManagementDlg()->Redisplay_TaskPartManagementDlg(pNewTaskItem);
		TaskSheetManagementBar::GetInstance()->GetTaskSheetManagementDlg()->Redisplay_TaskSheetManagementDlg(pNewTaskItem);

		// create a message.
		strMsgContent = MultiLanguageMgr::GetInstance()->TranslateString(IDS_TASK_SAVED);
		strMsgContent.Replace(_T("WILLBEREPLACED1"), pNewTaskItem->GetName());
		LogMessagePtr pLogMsg(new LogMessage(DateHelper::GetCurrentTime(), LOG_MSG_INFO, strTaskName, strMsgContent));
		LogMgr::WriteLog(pLogMsg);
		POSITION pos = pTaskDoc->GetFirstViewPosition();
		MessageBox(pTaskDoc->GetNextView(pos)->m_hWnd, strMsgContent, strProductName, MB_OK | MB_ICONINFORMATION);
	}
}

CLeaderDoc* DocManager::GetActiveDocument()
{
	CMDIFrameWnd* pFrame = (CMDIFrameWnd*)AfxGetApp()->GetMainWnd();

	// get the active MDI child window.
	CMDIChildWnd* pChild = (CMDIChildWnd*)pFrame->GetActiveFrame();
	if (!pChild)
		return NULL;

	// get the active view attached to the active MDI child window.
	CLeaderView* pView = (CLeaderView*)pChild->GetActiveView();
	if (!pView)
		return NULL;

	// 得到文档对象。
	CLeaderDoc* pDoc = pView->GetDocument();
	if (!pDoc)
		return NULL;

	return pDoc;
}

PartView* DocManager::GetActivePartView()
{
	CMDIFrameWnd* pFrame = (CMDIFrameWnd*)AfxGetApp()->GetMainWnd();

	// get the active MDI child window.
	CMDIChildWnd* pChild = (CMDIChildWnd*)pFrame->GetActiveFrame();
	if (!pChild)
	{
		LogMgr::DebugWarn(_T("63987"));
	}

	// get the active view attached to the active MDI child window.
	PartView* pPartView = (PartView*)pChild->GetActiveView();
	if (!pPartView)
	{
		LogMgr::DebugWarn(_T("33412"));
	}

	return pPartView;
}

CLeaderView* DocManager::GetCurrentView(CString& strViewName)
{
	CMDIFrameWnd* pFrame = (CMDIFrameWnd*)AfxGetApp()->GetMainWnd();
	CMDIChildWnd* pChild = (CMDIChildWnd*)pFrame->GetActiveFrame();
	if (!pChild)
	{
		LogMgr::DebugWarn(_T("65354"));
		return 0;
	}

	CView* pView = pChild->GetActiveView();
	if (PartView* pPartView = dynamic_cast<PartView*>(pView))
	{
		pPartView->GetWindowText(strViewName);
		return pPartView;
	}
	else if (SheetSubView* pSheetSubView = dynamic_cast<SheetSubView*>(pView))
	{
		strViewName = pSheetSubView->GetSheet()->GetName();
		return pSheetSubView;
	}

	return 0;
}

END_CUTLEADER_NAMESPACE()

#include "stdafx.h"
#include "DataCenterDlg.h"

#include "DateHelper.h"
#include "IntegerList.h"
#include "clUtilityResource.h"
#include "XMenu.h"
#include "versionInfo.h"
#include "LogMessage.h"
#include "LogMgr.h"
#include "baseConst.h"
#include "clBaseDataResource.h"
#include "DataBaseManager.h"
#include "DataCenterItemWriter.h"
#include "DataCenterItemLoader.h"
#include "DataCenterItemList.h"
#include "DataCenterItemBase.h"
#include "FolderNode.h"
#include "DataItem.h"
#include "DataCenterNode.h"
#include "FolderInfoDlg.h"

#include "IMaterialSize.h"
#include "MaterialLibLoader.h"
#include "clOptionResource.h"
#include "CLOptionInfo.h"

#include "PatternProcessor.h"
#include "Part.h"
#include "PatternList.h"
#include "PartCadData.h"
#include "PartItem.h"
#include "PartLoader.h"
#include "PartWriter.h"
#include "PartBuilder.h"
#include "clDataImportExportResource.h"
#include "Sheet.h"
#include "SheetLoader.h"
#include "SheetWriter.h"
#include "SheetManager.h"
#include "SheetInfoDlg.h"
#include "NestJob.h"
#include "NestScheduleDlg.h"
#include "NestJobMgr.h"
#include "clDataManagerResource.h"
#include "DependNodeMgr.h"
#include "DependMsgDlg.h"
#include "DependViewDlg.h"
#include "DependNode.h"
#include "DependNodeList.h"
#include "PartRepairDlg.h"
#include "DataItemTracker.h"
#include "TaskItemManager.h"
#include "TaskItem.h"
#include "PartInplaceEditData.h"

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(DataCenterDlg, CResizableDialog)

DataCenterDlg::DataCenterDlg(CWnd* pParent /*=NULL*/)
	: CLResizableDialog(DataCenterDlg::IDD, pParent)
{
	m_iNaviNodeID = INVALID_ID;
	m_productNodeType = DataCenterNodeType_NotInit;
}

DataCenterDlg::~DataCenterDlg()
{
}

BEGIN_MESSAGE_MAP(DataCenterDlg, CResizableDialog)
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()

	// the message for tree.
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_PRODUCT, OnSelchangedTree)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE_PRODUCT, OnDblclkTree)
	ON_NOTIFY(NM_RCLICK, IDC_TREE_PRODUCT, OnRClickTree)
	ON_MESSAGE(WM_DATA_ITEM_DB_CLICK, OnDataItemDbClick)
	ON_MESSAGE(WM_TREE_NODE_MOVED, OnTreeNodeMoved)

	// for the popup menu.
	ON_COMMAND(IDC_CREATE_FOLDER, OnCreateFolder)
	ON_COMMAND(IDC_ImportClTask, OnImportClTask)
	ON_COMMAND(IDC_ImportClPart, OnImportClPart)
	ON_COMMAND(IDC_ImportDxfDwg, OnImportDxfDwg)
	ON_COMMAND(IDC_ExportOneBusinessData, OnExportOneBusinessData)
	ON_COMMAND(IDC_DELETE_PRODUCT_ITEM, OnDeleteItem)

	ON_BN_CLICKED(IDC_BTN_EXPLORER_OPEN, OnExplorerOpen)
	ON_BN_CLICKED(IDC_BTN_EXPLORER_SAVE, OnExplorerSave)
	ON_BN_CLICKED(IDC_BTN_EXPLORER_CANCEL, OnExplorerCancel)
END_MESSAGE_MAP()

void DataCenterDlg::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_TREE_PRODUCT, m_dataCenterTree);
	DDX_Control(pDX, IDC_BTN_EXPLORER_OPEN, m_btnExplorerOpen);
	DDX_Control(pDX, IDC_BTN_EXPLORER_SAVE, m_btnExplorerSave);
	DDX_Control(pDX, IDC_BTN_EXPLORER_CANCEL, m_btnExplorerCancel);
}

void DataCenterDlg::InitSubDialog()
{
	//
	m_folderContentPage.Create(IDD_FOLDER_CONTENT_PAGE, this);
	m_folderContentPage.ShowWindow(SW_HIDE);

	//
	m_partInfoPage.Create(IDD_PARTINFO_PAGE, this);
	m_partInfoPage.ShowWindow(SW_HIDE);

	m_taskInfoPage.Create(IDD_ClDataManagerUI_TaskInfo, this);
	m_taskInfoPage.ShowWindow(SW_HIDE);

	//
	m_itemListPage.Create(IDD_clDataManager_ItemList, this);
	m_itemListPage.ShowWindow(SW_HIDE);
}

void DataCenterDlg::RepositionSubDlg()
{
	// no node selected, of course no page to display.
	if (m_productNodeType == DataCenterNodeType_NotInit)
		return;

	// calculate the position of the child page.
	CWnd* pWndPos = GetDlgItem(IDC_STATIC_CHILD_AREA);
	CRect pagePos;
	if (pWndPos)
	{
		pWndPos->GetWindowRect(&pagePos);
		ScreenToClient(&pagePos);
	}

	// because the layout library will re-layout the controls, so we should re-position the sub dialog.
	{
		HideAllSubDialog();
		if (m_productNodeType == DataCenterNodeType_Folder)
		{
			m_folderContentPage.SetWindowPos(pWndPos, pagePos.left, pagePos.top, pagePos.Width(), pagePos.Height(), 0);
			m_folderContentPage.ShowWindow(SW_SHOW);
		}
		else if (m_productNodeType == DataCenterNodeType_Part)
		{
			m_partInfoPage.SetWindowPos(pWndPos, pagePos.left, pagePos.top, pagePos.Width(), pagePos.Height(), 0);
			m_partInfoPage.ShowWindow(SW_SHOW);
		}
		else if (m_productNodeType == DataCenterNodeType_Task)
		{
			m_taskInfoPage.SetWindowPos(pWndPos, pagePos.left, pagePos.top, pagePos.Width(), pagePos.Height(), 0);
			m_taskInfoPage.ShowWindow(SW_SHOW);
		}
		else if (m_productNodeType == DataCenterNodeType_Multiple)
		{
			m_itemListPage.SetWindowPos(pWndPos, pagePos.left, pagePos.top, pagePos.Width(), pagePos.Height(), 0);
			m_itemListPage.ShowWindow(SW_SHOW);
		}
	}
}

BOOL DataCenterDlg::OnInitDialog()
{
	CLResizableDialog::OnInitDialog();

	// layout the controls.
	AddAnchor(IDC_GROUP_TREE, TOP_LEFT, ANCHOR(30, 100));
	AddAnchor(IDC_TREE_PRODUCT, TOP_LEFT, ANCHOR(30, 100));
	AddAnchor(IDC_STATIC_CHILD_AREA, ANCHOR(30, 0), BOTTOM_RIGHT);
	AddAnchor(IDC_BTN_EXPLORER_OPEN, BOTTOM_RIGHT);
	AddAnchor(IDC_BTN_EXPLORER_APPLY, BOTTOM_RIGHT);
	AddAnchor(IDC_BTN_EXPLORER_CANCEL, BOTTOM_RIGHT);

	// init the product tree.
	m_dataCenterHelper.Init(&m_dataCenterTree);

	// init sub dialogs.
	InitSubDialog();


	/************************************************************************/
	// init the tree.

	m_dataCenterTree.Init();

	// load the root node.
	DBConnectPtr pMainDBCon = DataBaseManager::GetInstance()->GetMainDBConnect();
	m_pWorkspace = DataCenterItemLoader::LoadProductTree(pMainDBCon);

	// display the tree.
	bool bShowHiddenDataItem = false;
	if (CLOptionInfo::GetInstance()->GetProductParam()->IsUseInside())
		bShowHiddenDataItem = true;
	m_dataCenterTree.InitTree(m_pWorkspace, bShowHiddenDataItem);

	// navigate to a specified node.
	m_dataCenterTree.NavigateToNode(m_iNaviNodeID);
	/************************************************************************/


	SetBackgroundColor(SkinManager::GetBackgroundColor());

	SetWindowTheme(GetDlgItem(IDC_GROUP_TREE)->GetSafeHwnd(), L"", L"");
	SetWindowTheme(GetDlgItem(IDC_TREE_PRODUCT)->GetSafeHwnd(), L"", L"");

	UpdateData(FALSE);

	return TRUE;
}

BOOL DataCenterDlg::DocModified()
{
	/************************************************************************/
	// prepare some data.

	HTREEITEM hItem = m_dataCenterTree.GetSelectedItem();
	if (!hItem)
		return FALSE;

	// get the DataCenterNode which is saved in the tree node.
	UINT dataVal = m_dataCenterTree.GetItemData(hItem);
	DataCenterNode* pData = (DataCenterNode*)dataVal;
	/************************************************************************/


	if (pData->GetNodeType() == DataCenterNodeType_Folder)
	{
		// force all edit boxes lose focus.
		m_folderContentPage.SetFocus();

		// check whether we should save the folder item.
		FolderNodePtr pFolderNode = m_folderContentPage.GetFolderItem();
		if (pFolderNode && pFolderNode->IsModified())
			return TRUE;
	}
	else if (pData->GetNodeType() == DataCenterNodeType_Part)
	{
		// force all edit boxes lose focus.
		m_partInfoPage.SetFocus();

		// check whether we should save the part.
		PartPtr pPart = m_partInfoPage.GetPart();
		if (pPart && pPart->IsModified())
			return TRUE;
	}
	else if (pData->GetNodeType() == DataCenterNodeType_Task)
	{
		TaskItemPtr pTaskItem = m_taskInfoPage.GetTaskItem();
		if (pTaskItem && pTaskItem->IsModified())
		{
			return TRUE;
		}
	}

	return FALSE;
}

void DataCenterDlg::SaveCurrentPage()
{
	/************************************************************************/
	// prepare some data here.

	HTREEITEM hItem = m_dataCenterTree.GetSelectedItem();
	if (!hItem)
		return;

	// get the DataCenterNode which is saved in the tree node.
	UINT dataVal = m_dataCenterTree.GetItemData(hItem);
	DataCenterNode* pData = (DataCenterNode*)dataVal;
	DataCenterNodeType nodeType = pData->GetNodeType();
	IDataCenterItemPtr pDataCenterItem = m_pWorkspace->SearchDataCenterItem(pData->GetDataCenterItemID());

	// for message.
	LogMessagePtr pLogMsg;
	CString strTaskName, strMsgContent;
	/************************************************************************/

	// check whether can save the page we will leave.
	if (SaveModification(hItem, FALSE))
	{
		// prepare the message.
		if (nodeType == DataCenterNodeType_Part)
		{
			strTaskName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_TASK_SAVE_PART_PAGE);
			strMsgContent = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PART_PAGE_SAVED);
		}
		else if (nodeType == DataCenterNodeType_Folder)
		{
			strTaskName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_TASK_SAVE_FOLDER_PAGE);
			strMsgContent = MultiLanguageMgr::GetInstance()->TranslateString(IDS_FOLDER_PAGE_SAVED);
		}
		strMsgContent.Replace(_T("WILLBEREPLACED1"), pDataCenterItem->GetName());
		pLogMsg.reset(new LogMessage(DateHelper::GetCurrentTime(), LOG_MSG_INFO, strTaskName, strMsgContent));
	}
	else
	{
		// prepare the message.
		if (nodeType == DataCenterNodeType_Part)
		{
			strTaskName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_TASK_SAVE_PART_PAGE);
			strMsgContent = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PART_PAGE_NOT_CHANGED);
		}
		else if (nodeType == DataCenterNodeType_Folder)
		{
			strTaskName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_TASK_SAVE_FOLDER_PAGE);
			strMsgContent = MultiLanguageMgr::GetInstance()->TranslateString(IDS_FOLDER_PAGE_NOT_CHANGED);
		}
		strMsgContent.Replace(_T("WILLBEREPLACED1"), pDataCenterItem->GetName());
		pLogMsg.reset(new LogMessage(DateHelper::GetCurrentTime(), LOG_MSG_WARNING, strTaskName, strMsgContent));
	}

	// write the log message.
	LogMgr::WriteLog(pLogMsg);

	// prompt user.
	CString strProductName;
	strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);
	MessageBox(strMsgContent, strProductName, MB_OK | MB_ICONINFORMATION);
}

void DataCenterDlg::HideAllSubDialog()
{
	m_folderContentPage.ShowWindow(SW_HIDE);
	m_partInfoPage.ShowWindow(SW_HIDE);
	m_taskInfoPage.ShowWindow(SW_HIDE);
	m_itemListPage.ShowWindow(SW_HIDE);
}

void DataCenterDlg::OnSize(UINT nType, int cx, int cy)
{
	CResizableDialog::OnSize(nType, cx, cy);

	// re-position the sub dialog.
	RepositionSubDlg();
}

HBRUSH DataCenterDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	return SkinManager::GetDialogCtrlColor(pDC, pWnd, nCtlColor);
}

void DataCenterDlg::OnSelchangedTree(NMHDR* pNMHDR, LRESULT* pResult)
{
	if (m_dataCenterTree.CanIgnore_selectChange_event())
	{
		return;
	}

	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	TVITEM newItem = pNMTreeView->itemNew;
	if (newItem.hItem == 0)
		return;
	TVITEM oldItem = pNMTreeView->itemOld;

	// disable the "open" button.
	((CWnd*)GetDlgItem(IDC_BTN_EXPLORER_OPEN))->EnableWindow(FALSE);

	// check the modifications in the previous page. if "oldItem.hItem" is NULL, we know this is the first time to select a node.
	if (oldItem.hItem)
	{
		// we should check whether we should save the data on the previous page.
		if (SaveModification(oldItem.hItem))
		{
			// get the DataCenterNode which is saved in the tree node.
			UINT iOldDataVal = m_dataCenterTree.GetItemData(oldItem.hItem);
			DataCenterNode* pOldData = (DataCenterNode*)iOldDataVal;
			DataCenterNodeType iOldNodeType = pOldData->GetNodeType();
			IDataCenterItemPtr pDataCenterItem = m_pWorkspace->SearchDataCenterItem(pOldData->GetDataCenterItemID());

			// write the log message.
			CString strTaskName, strMsgContent;
			if (iOldNodeType == DataCenterNodeType_Part)
			{
				strTaskName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_TASK_SAVE_PART_PAGE);
				strMsgContent = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PART_PAGE_SAVED);
			}
			else if (iOldNodeType == DataCenterNodeType_Task)
			{
				strTaskName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_TASK_SAVE_Task_PAGE);
				strMsgContent = MultiLanguageMgr::GetInstance()->TranslateString(IDS_Task_PAGE_SAVED);
			}
			else if (iOldNodeType == DataCenterNodeType_Folder)
			{
				strTaskName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_TASK_SAVE_FOLDER_PAGE);
				strMsgContent = MultiLanguageMgr::GetInstance()->TranslateString(IDS_FOLDER_PAGE_SAVED);
			}
			strMsgContent.Replace(_T("WILLBEREPLACED1"), pDataCenterItem->GetName());
			LogMessagePtr pLogMsg(new LogMessage(DateHelper::GetCurrentTime(), LOG_MSG_INFO, strTaskName, strMsgContent));
			LogMgr::WriteLog(pLogMsg);
		}
	}

	// the new node.
	BOOL bNodeTypeChanged = FALSE;
	DataCenterNode* pNewData = (DataCenterNode*)m_dataCenterTree.GetItemData(newItem.hItem);
	if (m_productNodeType != pNewData->GetNodeType())
	{
		m_productNodeType = pNewData->GetNodeType();
		bNodeTypeChanged = TRUE;
	}

	// process the case that more than one items are selected or no items are selected.
	int iSelNodeCount = m_dataCenterTree.GetSelectedCount();
	if (iSelNodeCount > 1)
	{
		m_productNodeType = DataCenterNodeType_Multiple;

		// get the selected node.
		vector<HTREEITEM> selNodeList = m_dataCenterTree.GetSelectedList();

		// collect all selected product nodes.
		DataCenterItemListPtr pDataCenterItemList(new DataCenterItemList);
		for (unsigned int i = 0; i < selNodeList.size(); i++)
		{
			// get the product item.
			HTREEITEM hItem = selNodeList.at(i);
			DataCenterNode* pTmpData = (DataCenterNode*)m_dataCenterTree.GetItemData(hItem);
			IDataCenterItemPtr pDataCenterItem = m_pWorkspace->SearchDataCenterItem(pTmpData->GetDataCenterItemID());

			pDataCenterItemList->push_back(pDataCenterItem);
		}

		// display ItemListPage.
		RepositionSubDlg();
		m_itemListPage.DisplayItemList(pDataCenterItemList);

		return;
	}
	else if (iSelNodeCount == 0)
	{
		return;
	}

	// if reach here, one node is selected.

	// update the last selected node.
	DataItemTracker::GetInstance()->SetLastSelNodeID(pNewData->GetDataCenterItemID());

	// re-position the sub dialog.
	if (bNodeTypeChanged)
		RepositionSubDlg();

	// display the page according to the node selected.
	if (pNewData->GetNodeType() == DataCenterNodeType_Folder)
	{
		// get the selected folder item.
		IDataCenterItemPtr pDataCenterItem = m_pWorkspace->SearchDataCenterItem(pNewData->GetDataCenterItemID());
		FolderNodePtr pTmpFolderNode = boost::dynamic_pointer_cast<FolderNode>(pDataCenterItem);

		// get all data nodes under this folder item.
		DataCenterItemListPtr pDataItemList(new DataCenterItemList);
		pTmpFolderNode->GetAllDataItem(pDataItemList);

		// display FolderContentPage.
		m_folderContentPage.DisplayDataItemList(pTmpFolderNode, pDataItemList);
	}
	else if (pNewData->GetNodeType() == DataCenterNodeType_Part)
	{
		// get the selected part node.
		IDataCenterItemPtr pDataCenterItem = m_pWorkspace->SearchDataCenterItem(pNewData->GetDataCenterItemID());
		DataItemPtr pPartNode = boost::dynamic_pointer_cast<DataItem>(pDataCenterItem);

		// load the part.
		DBConnectPtr pPartDBCon = DataBaseManager::ConnectPartDB(pPartNode->GetDataID());
		PartPtr pPart = PartLoader::LoadPart(pPartDBCon, pPartNode->GetDataID(), PARTLOAD_PREVIEW);

		// display PartInfoPage.
		m_partInfoPage.DisplayPartInfo(pPart);

		// 不作为后台cam运行或者处于在位编辑零件状态时，可以从数据中心打开零件。
		if (!CLOptionInfo::GetInstance()->GetProductParam()->GetIsRunAsBackgroundApp() || PartInplaceEditData::GetInstance()->GetIsUnderInPlaceEditMode()) {
			((CWnd*)GetDlgItem(IDC_BTN_EXPLORER_OPEN))->EnableWindow(TRUE);
		}
	}
	else if (pNewData->GetNodeType() == DataCenterNodeType_Task)
	{
		// get the selected task node.
		IDataCenterItemPtr pDataCenterItem = m_pWorkspace->SearchDataCenterItem(pNewData->GetDataCenterItemID());

		// load the task.
		DataItemPtr pTaskNode = boost::dynamic_pointer_cast<DataItem>(pDataCenterItem);
		TaskItemPtr pTaskItem = TaskItemManager::LoadTaskItemFromDb_4TaskInfoPageOfDataCenter(pTaskNode->GetDataID());

		// display
		m_taskInfoPage.DisplayTaskItem(pTaskItem);

		// 当处于在位编辑零件时，不可以从数据中心打开任务。
		if (!PartInplaceEditData::GetInstance()->GetIsUnderInPlaceEditMode()) {
			((CWnd*)GetDlgItem(IDC_BTN_EXPLORER_OPEN))->EnableWindow(TRUE);
		}
	}
}

void DataCenterDlg::OnDblclkTree(NMHDR* pNMHDR, LRESULT* pResult)
{
	// get the selected tree item.
	HTREEITEM hItem = m_dataCenterTree.GetSelectedItem();
	if (!hItem)
		return;

	// get the selected ProductItem object.
	UINT dataVal = m_dataCenterTree.GetItemData(hItem);
	DataCenterNode* pData = (DataCenterNode*)dataVal;
	IDataCenterItemPtr pDataCenterItem = m_pWorkspace->SearchDataCenterItem(pData->GetDataCenterItemID());

	// if we db-click data item.
	if (!pDataCenterItem->IsFolder())
	{
		DataItemPtr pSelDataItem = boost::dynamic_pointer_cast<DataItem>(pDataCenterItem);

		DATANODE_TYPE emDataType = pSelDataItem->GetDataType();
		if (CLOptionInfo::GetInstance()->GetProductParam()->GetIsRunAsBackgroundApp() && emDataType == DATANODE_PART) { // 作为后台cam运行时，不可以从数据中心打开零件。
			return;
		} else if (PartInplaceEditData::GetInstance()->GetIsUnderInPlaceEditMode() && emDataType == DATANODE_Task) { // 处于在位编辑零件时，不可以从数据中心打开任务。
			return;
		}

		OpenDataItem(pSelDataItem);
	}

	*pResult = 0;
}

void DataCenterDlg::OnRClickTree(NMHDR* pNMHDR, LRESULT* pResult)
{
	// check whether click on the item.
	POINT pos;
	{
		// convert the coordinate from screen to client.
		if (!GetCursorPos(&pos))
		{
			return;
		}
		m_dataCenterTree.ScreenToClient(&pos);

		// check the hit item.
		UINT uFlags;
		HTREEITEM hItem = m_dataCenterTree.HitTest(pos, &uFlags);
		if ((hItem == NULL) || !(uFlags & TVHT_ONITEM))
		{
			return;
		}
		else
		{
			if (!m_dataCenterTree.IsSelected(hItem))
			{
				m_dataCenterTree.SelectAll(FALSE);
				m_dataCenterTree.SetItemState(hItem, TVIS_SELECTED, TVIS_SELECTED);
			}
		}
	}

	// figure out the position where the menu will pop up.
	m_dataCenterTree.ClientToScreen(&pos);

	// 显示弹出菜单
	CString str;
	XMenu xMenu;
	xMenu.CreatePopupMenu();
	int iSelNodeCount = m_dataCenterTree.GetSelectedCount();
	if (iSelNodeCount > 1) // 选中了不止一个树节点
	{
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_DELETE);
		xMenu.AppendMenu(MF_STRING, IDC_DELETE_PRODUCT_ITEM, str.AllocSysString());
	}
	else if (iSelNodeCount == 1) // 只选中了一个树节点
	{
		// get the select node.
		vector<HTREEITEM> selItemList = m_dataCenterTree.GetSelectedList();
		HTREEITEM hItem = selItemList.at(0);
		DataCenterNode* pData = (DataCenterNode*)m_dataCenterTree.GetItemData(hItem);
		if (pData->GetNodeType() == DataCenterNodeType_Folder)
		{
			// folder
			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_CREATE_FOLDER);
			xMenu.AppendMenu(MF_STRING, IDC_CREATE_FOLDER, str.AllocSysString());

			// import.
			xMenu.AppendMenu(MF_SEPARATOR);
			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_ImportClTask);
			xMenu.AppendMenu(MF_STRING, IDC_ImportClTask, str.AllocSysString());
			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_ImportClPart);
			xMenu.AppendMenu(MF_STRING, IDC_ImportClPart, str.AllocSysString());
			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_ImportDxfDwg);
			xMenu.AppendMenu(MF_STRING, IDC_ImportDxfDwg, str.AllocSysString());

			//
			xMenu.AppendMenu(MF_SEPARATOR);
			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_DELETE);
			xMenu.AppendMenu(MF_STRING, IDC_DELETE_PRODUCT_ITEM, str.AllocSysString());
		}
		else if (pData->GetNodeType() == DataCenterNodeType_Part || pData->GetNodeType() == DataCenterNodeType_Task)
		{
			// export.
			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_EXPORT);
			xMenu.AppendMenu(MF_STRING, IDC_ExportOneBusinessData, str.AllocSysString());
			xMenu.AppendMenu(MF_SEPARATOR);

			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_DELETE);
			xMenu.AppendMenu(MF_STRING, IDC_DELETE_PRODUCT_ITEM, str.AllocSysString());
		}
	}
	xMenu.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, pos.x, pos.y, this);

	*pResult = 0;
}

LRESULT DataCenterDlg::OnDataItemDbClick(WPARAM wParam, LPARAM lParam)
{
	DataCenterNode* pDataCenterNode = reinterpret_cast<DataCenterNode*>(wParam);

	// navigate to the node.
	m_iNaviNodeID = pDataCenterNode->GetDataCenterItemID();
	m_dataCenterTree.NavigateToNode(m_iNaviNodeID);

	return true;
}

void DataCenterDlg::OnCreateFolder()
{
	m_dataCenterHelper.CreateFolder(m_pWorkspace);
}

void DataCenterDlg::OnImportClTask()
{
	m_dataCenterHelper.ImportClTask(m_pWorkspace);
}

void DataCenterDlg::OnImportClPart()
{
	m_dataCenterHelper.ImportClPart(m_pWorkspace);
}

void DataCenterDlg::OnImportDxfDwg()
{
	m_dataCenterHelper.ImportDxfDwg(m_pWorkspace);
}

void DataCenterDlg::OnExportOneBusinessData()
{
	m_dataCenterHelper.ExportOneBusinessData(m_pWorkspace);
}

void DataCenterDlg::OnDeleteItem()
{
	m_dataCenterHelper.DeleteSelItem(m_pWorkspace, m_hWnd);
}

void DataCenterDlg::OnExplorerOpen()
{
	// get the selected data object.
	HTREEITEM hItem = m_dataCenterTree.GetFirstSelectedItem();
	DataCenterNode* pData = (DataCenterNode*)m_dataCenterTree.GetItemData(hItem);
	IDataCenterItemPtr pDataCenterItem = m_pWorkspace->SearchDataCenterItem(pData->GetDataCenterItemID());

	// open it.
	DataItemPtr pSelDataItem = boost::dynamic_pointer_cast<DataItem>(pDataCenterItem);
	OpenDataItem(pSelDataItem);
}

void DataCenterDlg::OnExplorerSave()
{
	CString strProductName, str;
	strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);

	HTREEITEM hItem = m_dataCenterTree.GetSelectedItem();
	if (SaveModification(hItem, FALSE))
	{
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_CHANGE_SAVED);
		MessageBox(str, strProductName, MB_ICONINFORMATION);
	}
	else
	{
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_NO_CHANGE);
		MessageBox(str, strProductName, MB_ICONWARNING);
	}
}

void DataCenterDlg::OnExplorerCancel()
{
	EndDialog(IDC_BTN_EXPLORER_CANCEL);
}

BOOL DataCenterDlg::SaveModification(HTREEITEM hItem, BOOL bNeedConfirm)
{
	BOOL bModified = FALSE;

	CString strProductName, str;
	strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);

	DBConnectPtr pMainDBCon = DataBaseManager::GetInstance()->GetMainDBConnect();

	// get the DataCenterNode which is saved in the tree node.
	UINT dataVal = m_dataCenterTree.GetItemData(hItem);
	DataCenterNode* pData = (DataCenterNode*)dataVal;

	if (pData->GetNodeType() == DataCenterNodeType_Folder)
	{
		// force all edit boxes lose focus.
		m_folderContentPage.SetFocus();

		// check whether we should save the folder item.
		FolderNodePtr pFolderNode = m_folderContentPage.GetFolderItem();
		if (pFolderNode && pFolderNode->IsModified())
		{
			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_SAVE_FOLDER);
			if ((bNeedConfirm && MessageBox(str, strProductName, MB_YESNO | MB_ICONQUESTION) == IDYES) || !bNeedConfirm)
			{
				bModified = TRUE;

				/************************************************************************/
				// save the basic data of folder.

				// since we modified the folder, we should update the modify date.
				pFolderNode->SetModifyTime(DateHelper::GetCurrentTime());

				// save the "basic info" of the part.
				DataCenterItemWriter::UpdateFolderBasicData(pMainDBCon, pFolderNode);
				/************************************************************************/

				/************************************************************************/
				// update the UI.

				// update the name of the tree node.
				m_dataCenterTree.SetItemText(hItem, pFolderNode->GetName());

				// update the page.
				m_folderContentPage.UpdateModifyDate();
				/************************************************************************/
			}
		}
	}
	else if (pData->GetNodeType() == DataCenterNodeType_Part)
	{
		// force all edit boxes lose focus.
		m_partInfoPage.SetFocus();

		// check whether we should save the part.
		PartPtr pPart = m_partInfoPage.GetPart();
		if (pPart && pPart->IsModified())
		{
			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_SAVE_PART);
			if ((bNeedConfirm && MessageBox(str, strProductName, MB_YESNO | MB_ICONQUESTION) == IDYES) || !bNeedConfirm)
			{
				bModified = TRUE;

				/************************************************************************/
				// save the part.

				// since we modified the part, we should update the modify date.
				pPart->SetModifyTime(DateHelper::GetCurrentTime());

				// save the "basic info" of the part.
				DataCenterItemWriter::UpdateBasicData(pMainDBCon, pPart->GetID(), pPart->GetName(), pPart->GetComment());
				DataCenterItemWriter::UpdateModifyDate(pMainDBCon, pPart->GetID(), pPart->GetModifyTime());

				pPart->SetModified(FALSE);
				/************************************************************************/

				/************************************************************************/
				// update UI.

				// update the name of the tree node.
				m_dataCenterTree.SetItemText(hItem, pPart->GetName());

				// update the name of this DataItem.
				IDataCenterItemPtr pDataCenterItem = m_pWorkspace->SearchDataCenterItem(pData->GetDataCenterItemID());
				DataItemPtr pDataItem = boost::dynamic_pointer_cast<DataItem>(pDataCenterItem);
				pDataItem->SetName(pPart->GetName());

				// update the page.
				m_partInfoPage.UpdateModifyDate();
				/************************************************************************/
			}
		}
	}
	else if (pData->GetNodeType() == DataCenterNodeType_Task)
	{
		TaskItemPtr pTaskItem = m_taskInfoPage.GetTaskItem();
		if (pTaskItem && pTaskItem->IsModified())
		{
			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_SAVE_Task);
			if ((bNeedConfirm && MessageBox(str, strProductName, MB_YESNO | MB_ICONQUESTION) == IDYES) || !bNeedConfirm)
			{
				bModified = TRUE;

				// save the sheet.
				{
					// since we modified the task, we should update the modify date.
					pTaskItem->SetModifyTime(DateHelper::GetCurrentTime());

					// save the "basic info" of the task.
					DataCenterItemWriter::UpdateBasicData(pMainDBCon, pTaskItem->GetID(), pTaskItem->GetName(), pTaskItem->GetComment());
					DataCenterItemWriter::UpdateModifyDate(pMainDBCon, pTaskItem->GetID(), pTaskItem->GetModifyTime());

					pTaskItem->SetModified(FALSE);
				}

				// update UI.
				{
					// update the name of the tree node.
					m_dataCenterTree.SetItemText(hItem, pTaskItem->GetName());

					// update the name of this DataItem.
					IDataCenterItemPtr pDataCenterItem = m_pWorkspace->SearchDataCenterItem(pData->GetDataCenterItemID());
					DataItemPtr pDataItem = boost::dynamic_pointer_cast<DataItem>(pDataCenterItem);
					pDataItem->SetName(pTaskItem->GetName());

					// update the page.
					m_taskInfoPage.UpdateModifyDate();
				}
			}
		}
	}

	return bModified;
}

LRESULT DataCenterDlg::OnTreeNodeMoved(WPARAM wParam, LPARAM lParam)
{
	// get the items which will be moved.
	vector<HTREEITEM> dragItemList = m_dataCenterTree.GetDragItems();
	HTREEITEM dropTargetItem = m_dataCenterTree.GetDropTargetItem();

	// DataCenterHelper will help me to finish this.
	m_dataCenterHelper.MoveItem(m_pWorkspace, dragItemList, dropTargetItem);

	return true;
}

void DataCenterDlg::OpenDataItem(DataItemPtr pDataItem)
{
	// 打开前检查要不要修复零件。
	if (pDataItem->GetDataType() == DATANODE_PART)
	{
		DBConnectPtr pPartDBCon = DataBaseManager::ConnectPartDB(pDataItem->GetDataID());
		vector<DataProperty> camDataList = PartLoader::GetPartCamDataProp(pPartDBCon);
		if (camDataList.size() == 0)
		{
			PartRepairDlg dlg;
			dlg.SetPart(pDataItem->GetDataID());
			if (dlg.DoModal() == IDOK)
				m_partInfoPage.UpdateCamDataGrid();
			else
				return;
		}
	}

	// 打开前检查要不要修复任务。
	if (pDataItem->GetDataType() == DATANODE_Task)
	{
		if (TaskItemManager::WhetherSomeSheetLostMaterial(pDataItem->GetDataID()) || TaskItemManager::WhetherSomeSheetLostCamParam(pDataItem->GetDataID()))
		{
			CString str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_LostCamParamOrMaterial);
			MessageBox(str, MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME), MB_OK | MB_ICONWARNING);
			return;
		}
	}

	if (pDataItem->GetDataType() == DATANODE_PART || pDataItem->GetDataType() == DATANODE_Task)
	{
		m_pOpeningDataItem = pDataItem;
		EndDialog(IDC_BTN_EXPLORER_OPEN);
	}
}

END_CUTLEADER_NAMESPACE()

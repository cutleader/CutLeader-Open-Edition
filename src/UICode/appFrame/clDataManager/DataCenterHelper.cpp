#include "StdAfx.h"
#include "DataCenterHelper.h"

#include "baseConst.h"
#include "DBConnect.h"
#include "taoresource.h"
#include "IntegerList.h"
#include "DateHelper.h"
#include "ProgressData.h"
#include "ProgressDlg.h"
#include "clSecureResource.h"
#include "LogMessage.h"
#include "LogMessageList.h"
#include "LogMgr.h"
#include "clBaseDataResource.h"
#include "ClFolderHelper.h"
#include "ClFileHelper.h"
#include "DataBaseManager.h"
#include "DataCenterItemLoader.h"
#include "DataCenterItemList.h"
#include "DataCenterItemBase.h"
#include "FolderNode.h"
#include "DataItem.h"
#include "DataCenterNode.h"
#include "PathHelper.h"

#include "MaterialLibLoader.h"
#include "MaterialLibWriter.h"
#include "IMaterialSize.h"
#include "MaterialThickness.h"
#include "Material.h"

#include "PartCadData.h"
#include "PartCamData.h"
#include "PartCamDataList.h"
#include "Part.h"
#include "PartItem.h"
#include "PartLoader.h"
#include "PartWriter.h"
#include "PartInfoDlg.h"
#include "PartBuilder.h"
#include "PartManager.h"
#include "Sheet.h"
#include "SheetLoader.h"
#include "SheetWriter.h"
#include "SheetManager.h"
#include "SheetProcessor.h"
#include "SheetInfoDlg.h"
#include "NestJob.h"
#include "NestScheduleDlg.h"
#include "NestJobMgr.h"
#include "clDataImportExportResource.h"
#include "ImportManager.h"
#include "ExportManager.h"
#include "clDataManagerResource.h"
#include "FolderInfoDlg.h"
#include "DataCenterItemWriter.h"
#include "DependNodeMgr.h"
#include "DependMsgDlg.h"
#include "DependViewDlg.h"
#include "DependNode.h"
#include "DependNodeList.h"
#include "DataItemTracker.h"
#include "TaskItem.h"
#include "TaskItemManager.h"
#include "ImportDxfDwgTask.h"
#include "ImportClPart2DataCenterDlg.h"
#include "ImportClTaskTask.h"
#include "ImportClPartTask.h"
#include "ImportDxfDwg2DataCenterDlg.h"
#include "PartHelper.h"

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

DataCenterHelper::DataCenterHelper(void)
{
}

DataCenterHelper::~DataCenterHelper(void)
{
}


void DataCenterHelper::Init(DataCenterTree* pProductTree)
{
	m_pDataCenterTree = pProductTree;
}

void DataCenterHelper::CreateFolder(FolderNodePtr pWorkspace)
{
	CString strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);
	CString str;

	/************************************************************************/
	// get the selected item, you know, it should be a folder node.

	HTREEITEM hParentNode = m_pDataCenterTree->GetSelectedList().at(0);
	UINT dataVal = m_pDataCenterTree->GetItemData(hParentNode);
	DataCenterNode* pData = (DataCenterNode*)dataVal;

	IDataCenterItemPtr pDataCenterItem = pWorkspace->SearchDataCenterItem(pData->GetDataCenterItemID());
	FolderNodePtr pParentFolder = boost::dynamic_pointer_cast<FolderNode>(pDataCenterItem);
	/************************************************************************/

	if (pData->GetNodeType() != DataCenterNodeType_Folder)
	{
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_SEL_FOLDER);
		MessageBox(NULL, str, strProductName, MB_OK | MB_ICONWARNING);
	}
	else
	{
		// create a folder item.
		FolderInfoDlg dlg;
		if (dlg.DoModal() == IDOK)
		{
			CString strName, strComment;
			dlg.GetMatNameInfo(strName, strComment);

			/************************************************************************/
			// build a folder item.

			FolderNodePtr pFolderNode(new FolderNode);
			pFolderNode->SetParentID(pParentFolder->GetID());
			pFolderNode->SetName(strName);
			pFolderNode->SetComment(strComment);
			pFolderNode->SetCreateTime(DateHelper::GetCurrentTime());
			pFolderNode->SetModifyTime(DateHelper::GetCurrentTime());
			DataCenterItemListPtr pDataCenterItemList(new DataCenterItemList);
			pFolderNode->SetSubItemList(pDataCenterItemList);

			// append it to parent node.
			pParentFolder->GetSubItemList()->push_back(pFolderNode);
			/************************************************************************/

			// save it to database.
			DBConnectPtr pMainDBCon = DataBaseManager::GetInstance()->GetMainDBConnect();
			DataCenterItemWriter::CreateDataCenterItem(pMainDBCon, pFolderNode);

			/************************************************************************/
			// update the tree node.

			// the data which will be saved to the tree node.
			DataCenterNode* pDataCenterNode = new DataCenterNode();
			pDataCenterNode->SetDataCenterItemID(pFolderNode->GetID());
			pDataCenterNode->SetNodeType(DataCenterNodeType_Folder);

			// create tree node.
			TVINSERTSTRUCT hItem;
			hItem.hParent = hParentNode;
			hItem.item.pszText = pFolderNode->GetName().AllocSysString();
			hItem.item.cchTextMax = MAXLENGTH_STR;
			hItem.hInsertAfter = TVI_LAST;
			hItem.item.iImage = 3;
			hItem.item.iSelectedImage = 3;
			hItem.item.mask = TVIF_IMAGE | TVIF_PARAM | TVIF_SELECTEDIMAGE | TVIF_TEXT;

			// save data in tree node.
			HTREEITEM hSubNode = m_pDataCenterTree->InsertItem(&hItem);
			m_pDataCenterTree->SetItemData(hSubNode, (int)pDataCenterNode);
			m_pDataCenterTree->Expand(hParentNode, TVE_EXPAND);
			/************************************************************************/
		}
	}
}

void DataCenterHelper::ImportClTask(FolderNodePtr pWorkspace)
{
	// 得到当前选中的目录节点。
	HTREEITEM hSelectedTreeNode;
	FolderNodePtr pSelectedFolderNode;
	{
		hSelectedTreeNode = m_pDataCenterTree->GetSelectedList().at(0);
		DataCenterNode* pData = (DataCenterNode*)m_pDataCenterTree->GetItemData(hSelectedTreeNode);
		pSelectedFolderNode = boost::dynamic_pointer_cast<FolderNode>(pWorkspace->SearchDataCenterItem(pData->GetDataCenterItemID()));
	}

	// 得到要导入的任务文件。
	vector<CString> taskFilePathList;
	{
		CString strFileName;
		CString strFilter = MultiLanguageMgr::GetInstance()->TranslateString(IDS_IMPEXP_TaskItem_FileExt);
		CString strDflExt = MultiLanguageMgr::GetInstance()->TranslateString(IDS_IMPEXP_TaskItem_Ext);
		CFileDialog importFileDlg(TRUE, strDflExt, NULL, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT, strFilter);
		int iBufferSize = 100 * 1000 * MAX_PATH;
		importFileDlg.m_ofn.lpstrFile = strFileName.GetBuffer(iBufferSize);
		importFileDlg.m_ofn.nMaxFile = iBufferSize;
		if (importFileDlg.DoModal() == IDOK)
		{
			POSITION pos = importFileDlg.GetStartPosition();
			while (pos != NULL)
			{
				taskFilePathList.push_back(importFileDlg.GetNextPathName(pos));
			}
		}
		strFileName.ReleaseBuffer();
	}

	// 没选文件，那就退出。
	if (taskFilePathList.size() == 0)
	{
		return;
	}

	// 执行导入操作。
	DataCenterItemListPtr pNewTaskFolders;
	{
		ImportClTaskTaskPtr pTask(new ImportClTaskTask(pSelectedFolderNode, taskFilePathList));
		ProgressDlg progressDlg;
		progressDlg.Init(MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_IMPORT_DATA), pTask);
		progressDlg.DoModal();
		pNewTaskFolders = pTask->GetNewDataCenterItems();
	}

	// 没有能成功导入任务，退出。这里不要提示，之前代码中应该已经提示了。
	if (pNewTaskFolders->size() == 0)
	{
		return;
	}

	// 把新生成的目录节点挂到工作区树节点上。
	{
		// 挂上新的目录节点
		vector<HTREEITEM> newTreeItemList;
		m_pDataCenterTree->AppendSubNode(pNewTaskFolders, hSelectedTreeNode, newTreeItemList);

		// 选中新挂上的目录节点
		m_pDataCenterTree->Expand(hSelectedTreeNode, TVE_EXPAND);
		m_pDataCenterTree->SelectAll(FALSE);
		m_pDataCenterTree->CanIgnore_selectChange_event(true);
		for (unsigned int i = 0; i < newTreeItemList.size(); i++)
		{
			HTREEITEM newTreeItem = newTreeItemList[i];
			DataCenterNode* pDataCenterNode = (DataCenterNode*)m_pDataCenterTree->GetItemData(newTreeItem);
			if (pNewTaskFolders->GetItemByID(pDataCenterNode->GetDataCenterItemID()))
			{
				m_pDataCenterTree->CanIgnore_selectChange_event(false);
				m_pDataCenterTree->SetItemState(newTreeItem, LVIS_SELECTED, LVIS_SELECTED);
			}
		}
	}
}

void DataCenterHelper::ImportClPart(FolderNodePtr pWorkspace)
{
	// 得到当前选中的目录节点。
	HTREEITEM hSelectedTreeNode;
	FolderNodePtr pSelectedFolderNode;
	{
		hSelectedTreeNode = m_pDataCenterTree->GetSelectedList().at(0);
		DataCenterNode* pData = (DataCenterNode*)m_pDataCenterTree->GetItemData(hSelectedTreeNode);
		pSelectedFolderNode = boost::dynamic_pointer_cast<FolderNode>(pWorkspace->SearchDataCenterItem(pData->GetDataCenterItemID()));
	}

	// 得到要导入的任务文件。
	vector<CString> clPartPathList;
	{
		ImportClPart2DataCenterDlg importFileDlg;
		if (importFileDlg.DoModal() == IDOK)
		{
			POSITION pos = importFileDlg.GetStartPosition();
			while (pos != NULL)
			{
				clPartPathList.push_back(importFileDlg.GetNextPathName(pos));
			}
		}
	}

	// 没选文件，那就退出。
	if (clPartPathList.size() == 0)
	{
		return;
	}

	// 执行导入操作。
	DataCenterItemListPtr pNewTaskFolders;
	{
		ImportClPartTaskPtr pTask(new ImportClPartTask(pSelectedFolderNode, clPartPathList));
		ProgressDlg progressDlg;
		progressDlg.Init(MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_IMPORT_DATA), pTask);
		progressDlg.DoModal();
		pNewTaskFolders = pTask->GetNewDataCenterItems();
	}

	// 没有能成功导入任务，退出。这里不要提示，之前代码中应该已经提示了。
	if (pNewTaskFolders->size() == 0)
	{
		return;
	}

	// 把新生成的目录节点挂到工作区树节点上。
	{
		// 挂上新的目录节点
		vector<HTREEITEM> newTreeItemList;
		m_pDataCenterTree->AppendSubNode(pNewTaskFolders, hSelectedTreeNode, newTreeItemList);

		// 选中新挂上的目录节点
		m_pDataCenterTree->Expand(hSelectedTreeNode, TVE_EXPAND);
		m_pDataCenterTree->SelectAll(FALSE);
		m_pDataCenterTree->CanIgnore_selectChange_event(true);
		for (unsigned int i = 0; i < newTreeItemList.size(); i++)
		{
			if (i == newTreeItemList.size() - 1)
			{
				m_pDataCenterTree->CanIgnore_selectChange_event(false);
			}
			m_pDataCenterTree->SetItemState(newTreeItemList[i], LVIS_SELECTED, LVIS_SELECTED);
		}
	}
}

void DataCenterHelper::ImportDxfDwg(FolderNodePtr pWorkspace)
{
	// 得到当前选中的目录节点。
	HTREEITEM hSelectedTreeNode;
	FolderNodePtr pSelectedFolderNode;
	{
		hSelectedTreeNode = m_pDataCenterTree->GetSelectedList().at(0);
		DataCenterNode* pData = (DataCenterNode*)m_pDataCenterTree->GetItemData(hSelectedTreeNode);
		pSelectedFolderNode = boost::dynamic_pointer_cast<FolderNode>(pWorkspace->SearchDataCenterItem(pData->GetDataCenterItemID()));
	}

	// 得到要导入的任务文件。
	vector<CString> dxfdwgPathList;
	LONGLONG iParamConfigID;
	{
		ImportDxfDwg2DataCenterDlg importFileDlg;
		if (importFileDlg.DoModal() == IDOK)
		{
			iParamConfigID = importFileDlg.GetParamConfigID();
			POSITION pos = importFileDlg.GetStartPosition();
			while (pos != NULL)
			{
				dxfdwgPathList.push_back(importFileDlg.GetNextPathName(pos));
			}
		}
	}

	// 没选文件，那就退出。
	if (dxfdwgPathList.size() == 0)
	{
		return;
	}

	// 执行导入操作。
	DataCenterItemListPtr pNewTaskFolders;
	{
		ImportDxfDwgTaskPtr pTask(new ImportDxfDwgTask(pSelectedFolderNode, dxfdwgPathList, iParamConfigID));
		ProgressDlg progressDlg;
		progressDlg.Init(MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_IMPORT_DATA), pTask);
		progressDlg.DoModal();
		pNewTaskFolders = pTask->GetNewDataCenterItems();
	}

	// 没有能成功导入任务，退出。这里不要提示，之前代码中应该已经提示了。
	if (pNewTaskFolders->size() == 0)
	{
		return;
	}

	// 把新生成的目录节点挂到工作区树节点上。
	{
		// 挂上新的目录节点
		vector<HTREEITEM> newTreeItemList;
		m_pDataCenterTree->AppendSubNode(pNewTaskFolders, hSelectedTreeNode, newTreeItemList);

		// 选中新挂上的目录节点
		m_pDataCenterTree->Expand(hSelectedTreeNode, TVE_EXPAND);
		m_pDataCenterTree->SelectAll(FALSE);
		m_pDataCenterTree->CanIgnore_selectChange_event(true);
		for (unsigned int i = 0; i < newTreeItemList.size(); i++)
		{
			if (i == newTreeItemList.size() - 1)
			{
				m_pDataCenterTree->CanIgnore_selectChange_event(false);
			}
			m_pDataCenterTree->SetItemState(newTreeItemList[i], LVIS_SELECTED, LVIS_SELECTED);
		}
	}
}

void DataCenterHelper::ExportOneBusinessData(FolderNodePtr pWorkspace)
{
	CString strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);

	// 加载要导出的业务对象。
	IBusinessDataPtr pBusinessData;
	{
		HTREEITEM hItem = m_pDataCenterTree->GetSelectedList().at(0);
		DataCenterNode* pData = (DataCenterNode*)m_pDataCenterTree->GetItemData(hItem);
		IDataCenterItemPtr pDataCenterItem = pWorkspace->SearchDataCenterItem(pData->GetDataCenterItemID());
		const DataItem* pDataNode = (const DataItem*)pDataCenterItem.get();
		if (pDataNode->GetDataType() == DATANODE_PART)
		{
			DBConnectPtr pPartDBCon = DataBaseManager::ConnectPartDB(pDataNode->GetDataID());
			pBusinessData = PartLoader::LoadPart(pPartDBCon, pDataNode->GetDataID());
		}
		else if (pDataNode->GetDataType() == DATANODE_Task)
		{
			pBusinessData = TaskItemManager::LoadTaskItemFromDb(pDataNode->GetDataID());
		}
	}

	//
	BusinessDataType emBusinessDataType = pBusinessData->GetBusinessDataType();
	if (emBusinessDataType == BusinessData_TaskItem)
	{
		CString strFilter = MultiLanguageMgr::GetInstance()->TranslateString(IDS_IMPEXP_TaskItem_FileExt);
		CString strDflExt = MultiLanguageMgr::GetInstance()->TranslateString(IDS_IMPEXP_TaskItem_Ext);
		CFileDialog exportDataDlg(FALSE, strDflExt, NULL, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, strFilter);
		if (exportDataDlg.DoModal() == IDOK)
		{
			CString strFullFilePath = exportDataDlg.GetPathName();
			TaskItemPtr pTaskItem = boost::dynamic_pointer_cast<TaskItem>(pBusinessData);
			if (ExportManager::ExportTask(pTaskItem.get(), strFullFilePath))
			{
				MessageBox(NULL, MultiLanguageMgr::GetInstance()->TranslateString(IDS_EXP_TASK_SUCCESS), strProductName, MB_OK | MB_ICONINFORMATION);
			}
		}
	}
	else if (emBusinessDataType == BusinessData_Part)
	{
		CString strFilter = MultiLanguageMgr::GetInstance()->TranslateString(IDS_EXP_PRT_DXFDWG_FILEEXT);
		CString strDflExt = MultiLanguageMgr::GetInstance()->TranslateString(IDS_IMPEXP_DXF_EXT);
		CFileDialog exportDataDlg(FALSE, strDflExt, NULL, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, strFilter);
		if (exportDataDlg.DoModal() == IDOK)
		{
			CString strFullFilePath = exportDataDlg.GetPathName();
			PartPtr pPart = boost::dynamic_pointer_cast<Part>(pBusinessData);

			// 导出零件。
			BOOL bRet = TRUE;
			CString strExt = PathHelper::GetFileExt(strFullFilePath);
			if (strExt.CompareNoCase(MultiLanguageMgr::GetInstance()->TranslateString(IDS_IMPEXP_DXF_EXT)) == 0 ||
				strExt.CompareNoCase(MultiLanguageMgr::GetInstance()->TranslateString(IDS_IMPEXP_DWG_EXT)) == 0)
			{
				bRet = PartHelper::ExportPartAsDxfDwg(pPart, strFullFilePath);
			}
			else if (strExt.CompareNoCase(MultiLanguageMgr::GetInstance()->TranslateString(IDS_IMPEXP_TAOPRT_EXT)) == 0)
			{
				bRet = ExportManager::ExpPartAsPart(pPart, strFullFilePath);
			}
			if (bRet)
			{
				MessageBox(NULL, MultiLanguageMgr::GetInstance()->TranslateString(IDS_EXP_PRT_SUCCESS), strProductName, MB_OK | MB_ICONINFORMATION);
			}
		}
	}
	else
	{
		LogMgr::DebugWarn(_T("34697"));
	}
}

void DataCenterHelper::DeleteSelItem(FolderNodePtr pWorkspace, HWND hWnd)
{
	CString str, strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);

	vector<HTREEITEM> selTreeItemList = m_pDataCenterTree->GetSelectedTreeItems(); // 得到选中的树节点，在“眼睛看到的选中的树节点”中取最上层的那些节点。
	if (selTreeItemList.size() > 1) // 选中了多个树节点
	{
		// 得到选中的产品节点，在“眼睛看到的选中的树节点”中取最上层的那些节点。
		DataCenterItemListPtr pSelectedDataCenterItems(new DataCenterItemList);
		{
			IntegerListPtr pProductDataList = m_pDataCenterTree->GetSelectedTreeNodes();
			for (unsigned int i = 0; i < pProductDataList->size(); i++)
			{
				const DataCenterNode* pData = (const DataCenterNode*)pProductDataList->at(i);
				IDataCenterItemPtr pDataCenterItem = pWorkspace->SearchDataCenterItem(pData->GetDataCenterItemID());
				pSelectedDataCenterItems->push_back(pDataCenterItem);
			}
		}
		if (pSelectedDataCenterItems->size() != selTreeItemList.size())
		{
			LogMgr::DebugWarn(_T("54840"));
		}

		// 递归得到所有选中的数据节点。
		DataCenterItemListPtr pSelectedDataItems(new DataCenterItemList);
		pSelectedDataCenterItems->GetAllDataItem(pSelectedDataItems);

		// 如果这些选中的数据节点有些被打开了，那就警告用户。
		for (unsigned int i = 0; i < pSelectedDataItems->size(); i++)
		{
			DataItemPtr pDataItem = boost::dynamic_pointer_cast<DataItem>(pSelectedDataItems->operator [](i));
			if (DataItemTracker::GetInstance()->IsDataItemOpened(pDataItem->GetDataID()))
			{
				MessageBox(hWnd, MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_DEL_OPENED_ITEMS), strProductName, MB_OK | MB_ICONWARNING);
				return;
			}
		}

		// 检查有没有其他文档依赖选择集中的文档。
		{
			// 得到选中数据文件的依赖树。
			DataCenterItemListPtr pAllDataItems(new DataCenterItemList);
			pWorkspace->GetAllDataItem(pAllDataItems);
			DependNodeListPtr pDependNodeList = DependNodeMgr::GetDependTree(pAllDataItems.get(), pSelectedDataItems.get());

			if (!pDependNodeList->HasDependence()) // 没有其他数据项依赖选中的数据项。
			{
				str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_DEL_SEL_ITEMS);
				if (MessageBox(hWnd, str, strProductName, MB_YESNO | MB_ICONQUESTION) != IDYES)
				{
					return;
				}
			}
			else // 有依赖项
			{
				// 依赖项是不是也在选择集中。
				bool bAllDataInSelection = true;
				IntegerListPtr pDataItemIDList = pDependNodeList->GetAllDataItemID();
				for (unsigned int i = 0; i < pDataItemIDList->size(); i++)
				{
					if (!pSelectedDataItems->GetItemByID(pDataItemIDList->at(i)))
					{
						bAllDataInSelection = false;
						break;
					}
				}

				if (bAllDataInSelection)
				{
					str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_DEL_SEL_ITEMS);
					if (MessageBox(hWnd, str, strProductName, MB_YESNO | MB_ICONQUESTION) != IDYES)
					{
						return;
					}
				}
				else // 选择集之外有数据项依赖选择集中的数据项，不允许删除。
				{
					DependMsgDlg dlg(pDependNodeList);
					dlg.DoModal();
					return;
				}
			}
		}

		// 删除树控件上的节点。
		for (unsigned int i = 0; i < selTreeItemList.size(); i++)
		{
			m_pDataCenterTree->DeleteItem(selTreeItemList[i]);
		}

		// 删除产品结构树上的节点。
		for (unsigned int i = 0; i < pSelectedDataCenterItems->size(); i++)
		{
			pWorkspace->DeleteSubNodeByDataCenterItemID(pSelectedDataCenterItems->operator [](i)->GetID());
		}

		// 更新数据库
		{
			DBConnectPtr pMainDBCon = DataBaseManager::GetInstance()->GetMainDBConnect();
			DBConnectPtr pMatLibDBCon = DataBaseManager::GetInstance()->GetMatLibDBConnect();
			pMainDBCon->BeginTransaction();
			pMatLibDBCon->BeginTransaction();

			// 删除业务数据，注意，这里需要先删除任务再删除零件，并且这里不删除业务数据在主数据库中的产品项。
			for (unsigned int i = 0; i < pSelectedDataItems->size(); i++)
			{
				DataItemPtr pDataItem = boost::dynamic_pointer_cast<DataItem>(pSelectedDataItems->operator [](i));
				if (pDataItem->GetDataType() == DATANODE_Task)
				{
					TaskItemPtr pTaskItem = TaskItemManager::LoadTaskItemFromDb_4TaskInfoPageOfDataCenter(pDataItem->GetDataID());
					TaskItemManager::DeleteTaskItemFromDB_notCareDataCenterItemInMainDB(pTaskItem.get(), false);
				}
			}
			for (unsigned int i = 0; i < pSelectedDataItems->size(); i++)
			{
				DataItemPtr pDataItem = boost::dynamic_pointer_cast<DataItem>(pSelectedDataItems->operator [](i));
				if (pDataItem->GetDataType() == DATANODE_PART)
				{
					ClFileHelper::DeletePrtDB(pDataItem->GetDataID());
				}
			}

			// 把产品项从主数据库删除
			DataCenterItemListPtr pFolderNodeList = pSelectedDataCenterItems->GetFolderNodeList();
			for (unsigned int i = 0; i < pSelectedDataCenterItems->size(); i++)
			{
				const IDataCenterItem* pDataCenterItem = pSelectedDataCenterItems->operator [](i).get();
				if (const DataItem* pDataItem = dynamic_cast<const DataItem*>(pDataCenterItem))
				{
					DataCenterItemWriter::DeleteDataItemByDataID(pMainDBCon, pDataItem->GetDataID(), FALSE);
				}
				else if (const FolderNode* pFolderNode = dynamic_cast<const FolderNode*>(pDataCenterItem))
				{
					DataCenterItemWriter::DeleteFolderItemFromDB(pMainDBCon, pFolderNode);
				}
			}

			pMainDBCon->CommitTransaction();
			pMatLibDBCon->CommitTransaction();
		}
	}
	else if (selTreeItemList.size() == 1) // 只选中了一个树节点
	{
		HTREEITEM hItem = selTreeItemList.at(0);
		DataCenterNode* pData = (DataCenterNode*)m_pDataCenterTree->GetItemData(hItem);
		if (pData->GetNodeType() == DataCenterNodeType_Folder) // 选中了一个文件夹
		{
			// 这里用智能指针，目的是为了在下面的代码范围内，该对象都活着。
			FolderNodePtr pFolderNode = boost::dynamic_pointer_cast<FolderNode>(pWorkspace->SearchDataCenterItem(pData->GetDataCenterItemID()));

			// 不能删除根目录
			if (pFolderNode->GetID() == WorkSpaceFolderID)
			{
				MessageBox(hWnd, MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_DEL_ROOT), strProductName, MB_OK | MB_ICONWARNING);
				return;
			}

			// 不能删除零件仓库目录
			if (pFolderNode->GetID() == PartsLibraryFolderID)
			{
				MessageBox(hWnd, MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_DeletePartsLibraryFolder), strProductName, MB_OK | MB_ICONWARNING);
				return;
			}

			// 递归得到文件夹下所有的数据项。
			DataCenterItemListPtr pSelectedDataItems(new DataCenterItemList);
			pFolderNode->GetAllDataItem(pSelectedDataItems);

			// 如果这些选中的数据节点有些被打开了，那就警告用户。
			for (unsigned int i = 0; i < pSelectedDataItems->size(); i++)
			{
				DataItemPtr pDataItem = boost::dynamic_pointer_cast<DataItem>(pSelectedDataItems->operator [](i));
				if (DataItemTracker::GetInstance()->IsDataItemOpened(pDataItem->GetDataID()))
				{
					MessageBox(hWnd, MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_DEL_OPENED_ITEMS), strProductName, MB_OK | MB_ICONWARNING);
					return;
				}
			}

			// 检查有没有其他文档依赖选择集中的文档。
			{
				// 得到选中数据文件的依赖树。
				DataCenterItemListPtr pAllDataItems(new DataCenterItemList);
				pWorkspace->GetAllDataItem(pAllDataItems);
				DependNodeListPtr pDependNodeList = DependNodeMgr::GetDependTree(pAllDataItems.get(), pSelectedDataItems.get());

				if (!pDependNodeList->HasDependence()) // 没有其他数据项依赖选中的数据项。
				{
					str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_DEL_SEL_ITEMS);
					if (MessageBox(hWnd, str, strProductName, MB_YESNO | MB_ICONQUESTION) != IDYES)
					{
						return;
					}
				}
				else // 有依赖项
				{
					// 依赖项是不是也在选择集中。
					bool bAllDataInSelection = true;
					IntegerListPtr pDataItemIDList = pDependNodeList->GetAllDataItemID();
					for (unsigned int i = 0; i < pDataItemIDList->size(); i++)
					{
						if (!pSelectedDataItems->GetItemByID(pDataItemIDList->at(i)))
						{
							bAllDataInSelection = false;
							break;
						}
					}

					if (bAllDataInSelection)
					{
						str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_DEL_SEL_ITEMS);
						if (MessageBox(hWnd, str, strProductName, MB_YESNO | MB_ICONQUESTION) != IDYES)
						{
							return;
						}
					}
					else // 选择集之外有数据项依赖选择集中的数据项，不允许删除。
					{
						DependMsgDlg dlg(pDependNodeList);
						dlg.DoModal();
						return;
					}
				}
			}

			// 删除树控件上的节点。
			m_pDataCenterTree->DeleteItem(hItem);

			// 删除产品结构树上的节点。
			pWorkspace->DeleteSubNodeByDataCenterItemID(pFolderNode->GetID());

			// 更新数据库
			{
				DBConnectPtr pMainDBCon = DataBaseManager::GetInstance()->GetMainDBConnect();
				DBConnectPtr pMatLibDBCon = DataBaseManager::GetInstance()->GetMatLibDBConnect();
				pMainDBCon->BeginTransaction();
				pMatLibDBCon->BeginTransaction();

				// 删除业务数据，注意，这里需要先删除任务再删除零件，并且这里不删除业务数据在主数据库中的产品项。
				for (unsigned int i = 0; i < pSelectedDataItems->size(); i++)
				{
					DataItemPtr pDataItem = boost::dynamic_pointer_cast<DataItem>(pSelectedDataItems->operator [](i));
					if (pDataItem->GetDataType() == DATANODE_Task)
					{
						TaskItemPtr pTaskItem = TaskItemManager::LoadTaskItemFromDb_4TaskInfoPageOfDataCenter(pDataItem->GetDataID());
						TaskItemManager::DeleteTaskItemFromDB_notCareDataCenterItemInMainDB(pTaskItem.get(), false);
					}
				}
				for (unsigned int i = 0; i < pSelectedDataItems->size(); i++)
				{
					DataItemPtr pDataItem = boost::dynamic_pointer_cast<DataItem>(pSelectedDataItems->operator [](i));
					if (pDataItem->GetDataType() == DATANODE_PART)
					{
						ClFileHelper::DeletePrtDB(pDataItem->GetDataID());
					}
				}

				// 把目录从主数据库删除
				DataCenterItemWriter::DeleteFolderItemFromDB(pMainDBCon, pFolderNode.get());

				pMainDBCon->CommitTransaction();
				pMatLibDBCon->CommitTransaction();
			}
		}
		else if (pData->GetNodeType() == DataCenterNodeType_Part)
		{
			DataItemPtr pPartDataItem = boost::dynamic_pointer_cast<DataItem>(pWorkspace->SearchDataCenterItem(pData->GetDataCenterItemID()));

			// 如果零件被打开了，那就警告用户。
			if (DataItemTracker::GetInstance()->IsDataItemOpened(pPartDataItem->GetDataID()))
			{
				MessageBox(hWnd, MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_DEL_OPENED_ITEMS), strProductName, MB_OK | MB_ICONWARNING);
				return;
			}

			// 要是有任务依赖这个零件，那就不能删除这个零件。
			DataCenterItemListPtr pAllDataItems(new DataCenterItemList);
			pWorkspace->GetAllDataItem(pAllDataItems);
			DependNodePtr pDependNode = DependNodeMgr::GetDependTree(pAllDataItems.get(), pPartDataItem);
			if (pDependNode->GetDependNodeList()->size() > 0)
			{
				DependNodeListPtr pDependNodeList(new DependNodeList);
				pDependNodeList->push_back(pDependNode);
				DependMsgDlg dlg(pDependNodeList);
				dlg.DoModal();
				return;
			}

			// 确认是否删除
			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_DEL_SEL_ITEMS);
			if (MessageBox(hWnd, str, strProductName, MB_YESNO | MB_ICONQUESTION) != IDYES)
			{
				return;
			}

			// 删除树控件上的节点。
			m_pDataCenterTree->DeleteItem(hItem);

			// 删除产品结构树上的节点。
			pWorkspace->DeleteSubNodeByDataCenterItemID(pPartDataItem->GetID());

			// 更新数据库
			{
				// 删除业务数据，注意，这里不删除业务数据在主数据库中的产品项。
				ClFileHelper::DeletePrtDB(pPartDataItem->GetDataID());

				// 把目录从主数据库删除
				DBConnectPtr pMainDBCon = DataBaseManager::GetInstance()->GetMainDBConnect();
				DataCenterItemWriter::DeleteDataItemByDataID(pMainDBCon, pPartDataItem->GetDataID());
			}
		}
		else if (pData->GetNodeType() == DataCenterNodeType_Task)
		{
			DataItemPtr pTaskDataItem = boost::dynamic_pointer_cast<DataItem>(pWorkspace->SearchDataCenterItem(pData->GetDataCenterItemID()));

			// 如果任务被打开了，那就警告用户。
			if (DataItemTracker::GetInstance()->IsDataItemOpened(pTaskDataItem->GetDataID()))
			{
				MessageBox(hWnd, MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_DEL_OPENED_ITEMS), strProductName, MB_OK | MB_ICONWARNING);
				return;
			}

			// 确认是否删除
			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_DEL_SEL_ITEMS);
			if (MessageBox(hWnd, str, strProductName, MB_YESNO | MB_ICONQUESTION) != IDYES)
			{
				return;
			}

			// 删除树控件上的节点。
			m_pDataCenterTree->DeleteItem(hItem);

			// 删除产品结构树上的节点。
			pWorkspace->DeleteSubNodeByDataCenterItemID(pTaskDataItem->GetID());

			// 更新数据库
			{
				// 删除业务数据，注意，这里不删除业务数据在主数据库中的产品项。
				TaskItemPtr pTaskItem = TaskItemManager::LoadTaskItemFromDb_4TaskInfoPageOfDataCenter(pTaskDataItem->GetDataID());
				TaskItemManager::DeleteTaskItemFromDB_notCareDataCenterItemInMainDB(pTaskItem.get(), false);

				// 把目录从主数据库删除
				DBConnectPtr pMainDBCon = DataBaseManager::GetInstance()->GetMainDBConnect();
				DataCenterItemWriter::DeleteDataItemByDataID(pMainDBCon, pTaskDataItem->GetDataID());
			}
		}
	}
}

void DataCenterHelper::MoveItem(FolderNodePtr pWorkspace, vector<HTREEITEM> itemList, HTREEITEM targetItem)
{
	DBConnectPtr pMainDBCon = DataBaseManager::GetInstance()->GetMainDBConnect();

	/************************************************************************/
	// get some data of the parent node.

	// get the new parent ID.
	DataCenterNode* pData = (DataCenterNode*)m_pDataCenterTree->GetItemData(targetItem);
	LONGLONG iNewParentID = pData->GetDataCenterItemID();

	// get the new parent item.
	IDataCenterItemPtr pTmpDataCenterItem = pWorkspace->SearchDataCenterItem(iNewParentID);
	FolderNodePtr pParentFolder = boost::dynamic_pointer_cast<FolderNode>(pTmpDataCenterItem);
	/************************************************************************/


	/************************************************************************/
	// change the parent ID of each item in "itemList"

	// begin the db transaction.
	pMainDBCon->BeginTransaction();

	for (unsigned int i = 0; i < itemList.size(); i++)
	{
		// get the product item.
		HTREEITEM hItem = itemList.at(i);
		pData = (DataCenterNode*)m_pDataCenterTree->GetItemData(hItem);
		IDataCenterItemPtr pDataCenterItem = pWorkspace->SearchDataCenterItem(pData->GetDataCenterItemID());

		// change the data structure.
		pDataCenterItem->SetParentID(iNewParentID);

		// update "pWorkspace".
		pWorkspace->DeleteSubNodeByDataCenterItemID(pDataCenterItem->GetID());
		pParentFolder->GetSubItemList()->push_back(pDataCenterItem);

		// save the change to database.
		DataCenterItemWriter::ChangeParentNode(pMainDBCon, pDataCenterItem, FALSE);
	}

	// end the db transaction.
	pMainDBCon->CommitTransaction();
	/************************************************************************/
}

END_CUTLEADER_NAMESPACE()

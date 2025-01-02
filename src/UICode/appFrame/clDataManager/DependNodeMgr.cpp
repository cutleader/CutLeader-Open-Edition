#include "StdAfx.h"
#include "DependNodeMgr.h"

#include "baseConst.h"
#include "DBConnect.h"
#include "DataBaseManager.h"

#include "IMaterialSize.h"

#include "PartLoader.h"
#include "PartManager.h"
#include "Sheet.h"
#include "SheetLoader.h"
#include "NestJob.h"
#include "DataCenterItemLoader.h"
#include "DataCenterItemList.h"
#include "DataCenterItemBase.h"
#include "FolderNode.h"
#include "DataItem.h"
#include "DataCenterNode.h"
#include "clDataManagerResource.h"
#include "DependNode.h"
#include "DependNodeList.h"
#include "TaskItemManager.h"


using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

DependNodePtr DependNodeMgr::GetDependTree(const DataCenterItemList* pAllDataItems, DataItemPtr pDataItem)
{
	DependNodePtr pDependNode(new DependNode);
	pDependNode->SetDataItem(pDataItem);

	DBConnectPtr pMainDBCon = DataBaseManager::GetInstance()->GetMainDBConnect();

	vector<DataProperty> taskInfoList;
	if (pDataItem->GetDataType() == DATANODE_PART)
	{
		// check each sheet which depend on this part.
		TaskItemManager::GetTaskInfoList(pMainDBCon, taskInfoList, pDataItem->GetDataID());
		for (unsigned int i = 0; i < taskInfoList.size(); i++)
		{
			DataProperty prop = taskInfoList[i];

			// 依赖零件的任务
			LONGLONG iTaskID = prop.GetItem(0).GetValue().llVal;
			DataItemPtr pTaskDataItem = boost::dynamic_pointer_cast<DataItem>(pAllDataItems->GetItemByDataID(iTaskID));

			// 建立依赖项
			DependNodePtr pTmpDependNode(new DependNode);
			pTmpDependNode->SetDataItem(pTaskDataItem);
			pDependNode->GetDependNodeList()->push_back(pTmpDependNode);
		}
	}
	else if (pDataItem->GetDataType() == DATANODE_Task) // 没有数据项依赖任务
	{
		;
	}

	return pDependNode;
}

DependNodeListPtr DependNodeMgr::GetDependTree(const DataCenterItemList* pAllDataItemList, const FolderNode* pFolderNode)
{
	DependNodeListPtr pDependNodeList(new DependNodeList);

	// get all data items under "pDataItemList".
	DataCenterItemListPtr pDataItemList(new DataCenterItemList);
	pFolderNode->GetAllDataItem(pDataItemList);

	// get the dependence tree of each data item.
	for (unsigned int i = 0; i < pDataItemList->size(); i++)
	{
		IDataCenterItemPtr pDataCenterItem = pDataItemList->at(i);
		DataItemPtr pDataItem = boost::dynamic_pointer_cast<DataItem>(pDataCenterItem);

		//
		DependNodePtr pDependNode = GetDependTree(pAllDataItemList, pDataItem);
		pDependNodeList->push_back(pDependNode);
	}

	return pDependNodeList;
}

DependNodeListPtr DependNodeMgr::GetDependTree(const DataCenterItemList* pAllDataItemList, const DataCenterItemList* pDataItemList)
{
	DependNodeListPtr pDependNodeList(new DependNodeList);

	// get the dependence tree of each data item.
	for (unsigned int i = 0; i < pDataItemList->size(); i++)
	{
		IDataCenterItemPtr pDataCenterItem = pDataItemList->at(i);
		DataItemPtr pDataItem = boost::dynamic_pointer_cast<DataItem>(pDataCenterItem);

		//
		DependNodePtr pDependNode = GetDependTree(pAllDataItemList, pDataItem);
		pDependNodeList->push_back(pDependNode);
	}

	return pDependNodeList;
}

END_CUTLEADER_NAMESPACE()

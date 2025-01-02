#pragma once

#include "ClBaseDataCommon.h"


DECLARE_CUTLEADER_CLASS(DBConnect)
DECLARE_CUTLEADER_CLASS(IntegerList)
DECLARE_CUTLEADER_CLASS(FolderNode)
DECLARE_CUTLEADER_CLASS(IDataCenterItem)


BEGIN_CUTLEADER_NAMESPACE()


// this class will modify "productStruct_folder" and "productStruct_data" table.
// notes:
//   1) this class do not affect other tables.
class ClBaseData_Export DataCenterItemWriter
{
public:
	/************************************************************************/
	// about creating items.

	// append "pDataCenterItem" to "pFolderNode".
	// notes:
	//   1) we can know the parent from "pDataCenterItem::parentID".
	//   2) this function think "pDataCenterItem" is a new item and it has no child.
	static void CreateDataCenterItem(DBConnectPtr pDBConnect, IDataCenterItemPtr pDataCenterItem, BOOL bTransaction = TRUE);
	/************************************************************************/


	/************************************************************************/
	// 关于删除产品项

	// 删除文件夹，当然也删除其孩子们。这个函数没有启动事务。
	static void DeleteFolderItemFromDB(DBConnectPtr pDBConnect, const FolderNode* pFolderNode);

	// delete data item by item ID.
	static void DeleteDataItem(DBConnectPtr pDBConnect, IntegerListPtr pDataItemIDList, BOOL bTransaction = TRUE);

	// delete data item by data ID.
	static void DeleteDataItemByDataID(DBConnectPtr pDBConnect, LONGLONG iDataID, BOOL bTransaction = TRUE);
	static void DeleteDataItemByDataID(DBConnectPtr pDBConnect, IntegerListPtr pDataIDList, BOOL bTransaction = TRUE);
	/************************************************************************/


	/************************************************************************/
	// about modifying some data.

	// update the basic data of the data.
	// notes:
	//   1) after call this, must call function "UpdateModifyDate".
	static void UpdateBasicData(DBConnectPtr pDBConnect, LONGLONG iDataID, CString strName, CString strComment, BOOL bTransaction = TRUE);

	// update the modified date of the data.
	static void UpdateModifyDate(DBConnectPtr pDBConnect, LONGLONG iDataID, LONGLONG iModifyTimeStamp, BOOL bTransaction = TRUE);

	// save the basic data of folder into database.
	static void UpdateFolderBasicData(DBConnectPtr pDBConnect, FolderNodePtr pFolderNode);

	// change the parent node of "pDataCenterItem".
	static void ChangeParentNode(DBConnectPtr pDBConnect, IDataCenterItemPtr pDataCenterItem, BOOL bTransaction = TRUE);
	/************************************************************************/

	// 检查工作区或者零件仓库目录是否存在，不在就创建。
	static void CheckWorkspaceFolder(DBConnectPtr pDBConnect);
	static void CheckPartLibraryFolder(DBConnectPtr pDBConnect);
};

END_CUTLEADER_NAMESPACE()

#pragma once

#include "DataProperty.h"
#include "DataItem.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(DBConnect)
DECLARE_CUTLEADER_CLASS(FolderNode)
DECLARE_CUTLEADER_CLASS(DataItem)

BEGIN_CUTLEADER_NAMESPACE()

class ClBaseData_Export DataCenterItemLoader
{
public:
	/************************************************************************/
	// about get data prop.

	// get the data prop.
	// notes:
	//   1) data in a DataProperty:
	//      1) data name
	//      2) data comment
	//      3) create date
	//      4) modify date
	//      5) 产品项ID。
	static BOOL GetDataProp(DBConnectPtr pDBConnect, LONGLONG iDataID, DataProperty& dataProp);

	// get data prop list.
	// notes:
	//   1) data in a DataProperty:
	//      1) data id
	//      2) data name
	//      3) data comment
	//      4) create date
	//      5) modify date
	static BOOL GetDataPropList(DBConnectPtr pDBConnect, std::vector<DataProperty>& dataPropList, DATANODE_TYPE iDataType);
	/************************************************************************/


	// load all nodes of the product tree.
	static FolderNodePtr LoadProductTree(DBConnectPtr pDBConnect);

	// get the sub-node of the folder node.
	// notes:
	//   1) if no sub-nodes under "pFolderNode", return false.
	//   2) this function is a recursive function.
	/* 参数:
	*  strPath: the full path of "pFolderNode", e.g., for "folder1", "strPath" will be "My Workspace/Folder1".
	*/
	static BOOL LoadSubNode(DBConnectPtr pDBConnect, FolderNodePtr pFolderNode, CString strPath);

	// load the data item by the data ID.
	// notes:
	//   1) 调用者需要把该数据项的路径发过来。
	static DataItemPtr LoadDataItem(DBConnectPtr pDBConnect, LONGLONG iDataID, CString strPath);
};

END_CUTLEADER_NAMESPACE()

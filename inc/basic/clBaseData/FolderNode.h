#pragma once

#include "DataCenterItemBase.h"
#include "boost/enable_shared_from_this.hpp"


DECLARE_CUTLEADER_CLASS(IntegerList)
DECLARE_CUTLEADER_CLASS(DataCenterItemList)
DECLARE_CUTLEADER_CLASS(FolderNode)
DECLARE_CUTLEADER_CLASS(IDataCenterItem)


BEGIN_CUTLEADER_NAMESPACE()


// 工作区目录和零件仓库目录的ID。
#define WorkSpaceFolderID           123
#define PartsLibraryFolderID        234

// “目录项”对象。
// 注：
//  1) 根目录名称在数据库是英文，所以加载时要根据语言处理一下。
class ClBaseData_Export FolderNode : public DataCenterItemBase, public boost::enable_shared_from_this<FolderNode>
{
public:
	FolderNode(void);
	FolderNode(LONGLONG iParentID, CString strName, DataCenterItemListPtr pSubItemList, LONGLONG iCreateTime, LONGLONG iModifyTime);
	virtual ~FolderNode(void);

public: // implement interface IDataCenterItem.
	virtual BOOL IsFolder() const override { return TRUE; }

public: // get/set functions.
	DataCenterItemListPtr GetSubItemList() const { return m_pDataCenterItemList; }
	void SetSubItemList(DataCenterItemListPtr pDataCenterItemList) { m_pDataCenterItemList = pDataCenterItemList; }

public:
	/************************************************************************/
	// about get data items.
	
	// get all DataItem under this folder.
	// notes:
	//   1) this function should be a recursive one.
	//   2) caller must allocate "pDataItemList".
	void GetAllDataItem(DataCenterItemListPtr pDataItemList) const;

	// get DataItems under this folder.
	// notes:
	//   1) this function is not a recursive one.
	DataCenterItemListPtr GetDataItemList() const;

	// use the data ID list to get data item IDs.
	IntegerListPtr GetDataItem(IntegerListPtr pDataIDList) const;
	/************************************************************************/


	// 根据ID搜索指定的数据中心元素。
	// notes:
	//   1) this function should be a recursive one.
	IDataCenterItemPtr SearchDataCenterItem(LONGLONG iDataCenterItemID);

	// 根据全路径搜索指定的数据中心元素。
	// notes:
	//   1) this function should be a recursive one.
	IDataCenterItemPtr SearchDataCenterItemByFullPath(CString strFullPath);

	// 删除文件夹下面的子节点，这是递归函数。
	void DeleteSubNodeByDataCenterItemID(LONGLONG iDataCenterItemID);

private:
	// the sub-nodes under this folder.
	// notes:
	//   1) this class do not manage the memory.
	DataCenterItemListPtr m_pDataCenterItemList;
};

END_CUTLEADER_NAMESPACE()

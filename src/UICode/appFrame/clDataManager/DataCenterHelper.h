#pragma once

#include "clDataManagerCommon.h"
#include "DataCenterTree.h"
#include <vector>
#include "IBusinessData.h"


DECLARE_CUTLEADER_CLASS(IntegerList)
DECLARE_CUTLEADER_CLASS(DataItem)
DECLARE_CUTLEADER_CLASS(FolderNode)
DECLARE_CUTLEADER_CLASS(DependNode)
DECLARE_CUTLEADER_CLASS(ExportDataInfo)


BEGIN_CUTLEADER_NAMESPACE()


// use this class to help ExplorerView, otherwise ExplorerView will become a huge one.
class ClDataManager_Export DataCenterHelper
{
public:
	DataCenterHelper(void);
	~DataCenterHelper(void);

public:
	void Init(DataCenterTree* pProductTree);

	void CreateFolder(FolderNodePtr pWorkspace);

	// 导入
	void ImportClTask(FolderNodePtr pWorkspace);
	void ImportClPart(FolderNodePtr pWorkspace);
	void ImportDxfDwg(FolderNodePtr pWorkspace);

	// 导出被选中的一个业务对象，此时pWorkspace中就一个数据节点被选中。
	void ExportOneBusinessData(FolderNodePtr pWorkspace);

	// move "itemList" under "targetItem".
	// notes:
	//   1) this function do not update the tree. it will update "pWorkspace", and the database.
	//   2) caller should make sure "targetItem" is not the data item.
	void MoveItem(FolderNodePtr pWorkspace, std::vector<HTREEITEM> itemList, HTREEITEM targetItem);

	// delete selected node(s).
	// notes:
	//   1) this function will care everything about deleting the selected nodes.
	//   2) if the selected items or the dependence of the selected items have been opened, cannot delete.
	void DeleteSelItem(FolderNodePtr pWorkspace, HWND hWnd);

private:
	// the tree in ProductExplSheet.
	DataCenterTree* m_pDataCenterTree;
};

END_CUTLEADER_NAMESPACE()

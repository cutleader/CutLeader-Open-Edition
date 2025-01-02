#pragma once

#include "ClDataImportExportUICommon.h"
#include "TaskEx.h"
#include <vector>


DECLARE_CUTLEADER_CLASS(FolderNode)
DECLARE_CUTLEADER_CLASS(DataCenterItemList)
DECLARE_CUTLEADER_CLASS(ImportDxfDwgTask)


BEGIN_CUTLEADER_NAMESPACE()

// 这个任务负责导入dxf/dwg到数据中心。
class ClDataImportExportUI_Export ImportDxfDwgTask : public TaskEx
{
public:
    ImportDxfDwgTask(FolderNodePtr pParentFolder, const std::vector<CString>& dxfDwgPathList, LONGLONG iParamConfigID);
	~ImportDxfDwgTask(void);

public: // inherit from class TaskEx.
	virtual void Run();

public:
	DataCenterItemListPtr GetNewDataCenterItems() const { return m_pNewDataCenterItems; }

private:
	FolderNodePtr m_pParentFolder;

	// 要导入的dxf/dwg文件
	std::vector<CString> m_dxfDwgPathList;

    // 把dxf/dwg导入为零件时用到的加工参数。
    LONGLONG m_iParamConfigID;

	// 导入后新生成的产品项。
	DataCenterItemListPtr m_pNewDataCenterItems;
};

END_CUTLEADER_NAMESPACE()

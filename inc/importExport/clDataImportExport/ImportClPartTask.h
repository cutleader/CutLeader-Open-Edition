#pragma once

#include "ClDataImportExportCommon.h"
#include "TaskEx.h"
#include <vector>


DECLARE_CUTLEADER_CLASS(FolderNode)
DECLARE_CUTLEADER_CLASS(DataCenterItemList)
DECLARE_CUTLEADER_CLASS(ImportClPartTask)


BEGIN_CUTLEADER_NAMESPACE()

// 这个任务负责导入ClPart到数据库。
class ClDataImportExport_Export ImportClPartTask : public TaskEx
{
public:
    ImportClPartTask(FolderNodePtr pParentFolder, const std::vector<CString>& clPartPathList);
	~ImportClPartTask(void);

public: // inherit from class TaskEx.
	virtual void Run();

public:
	DataCenterItemListPtr GetNewDataCenterItems() const { return m_pNewDataCenterItems; }

private:
	FolderNodePtr m_pParentFolder;

	// 要导入的ClPart文件
	std::vector<CString> m_clPartPathList;

	// 导入后新生成的产品项。
	DataCenterItemListPtr m_pNewDataCenterItems;
};

END_CUTLEADER_NAMESPACE()

#pragma once

#include "ClDataImportExportCommon.h"
#include "TaskEx.h"
#include <vector>


DECLARE_CUTLEADER_CLASS(FolderNode)
DECLARE_CUTLEADER_CLASS(DataCenterItemList)
DECLARE_CUTLEADER_CLASS(ImportClTaskTask)


BEGIN_CUTLEADER_NAMESPACE()

// ������������������ݵ����ݿ⡣
class ClDataImportExport_Export ImportClTaskTask : public TaskEx
{
public:
    ImportClTaskTask(FolderNodePtr pParentFolder, const std::vector<CString>& taskFilePathList);
	~ImportClTaskTask(void);

public: // inherit from class TaskEx.
	virtual void Run();

public:
	DataCenterItemListPtr GetNewDataCenterItems() const { return m_pNewDataCenterItems; }

private:
	FolderNodePtr m_pParentFolder;

	// Ҫ����������ļ�
	std::vector<CString> m_taskFilePathList;

	// ����������ɵĲ�Ʒ�
	DataCenterItemListPtr m_pNewDataCenterItems;
};

END_CUTLEADER_NAMESPACE()

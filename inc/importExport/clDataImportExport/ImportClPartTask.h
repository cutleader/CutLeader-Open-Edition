#pragma once

#include "ClDataImportExportCommon.h"
#include "TaskEx.h"
#include <vector>


DECLARE_CUTLEADER_CLASS(FolderNode)
DECLARE_CUTLEADER_CLASS(DataCenterItemList)
DECLARE_CUTLEADER_CLASS(ImportClPartTask)


BEGIN_CUTLEADER_NAMESPACE()

// �����������ClPart�����ݿ⡣
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

	// Ҫ�����ClPart�ļ�
	std::vector<CString> m_clPartPathList;

	// ����������ɵĲ�Ʒ�
	DataCenterItemListPtr m_pNewDataCenterItems;
};

END_CUTLEADER_NAMESPACE()

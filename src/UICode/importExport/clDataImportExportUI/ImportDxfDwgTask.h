#pragma once

#include "ClDataImportExportUICommon.h"
#include "TaskEx.h"
#include <vector>


DECLARE_CUTLEADER_CLASS(FolderNode)
DECLARE_CUTLEADER_CLASS(DataCenterItemList)
DECLARE_CUTLEADER_CLASS(ImportDxfDwgTask)


BEGIN_CUTLEADER_NAMESPACE()

// �����������dxf/dwg���������ġ�
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

	// Ҫ�����dxf/dwg�ļ�
	std::vector<CString> m_dxfDwgPathList;

    // ��dxf/dwg����Ϊ���ʱ�õ��ļӹ�������
    LONGLONG m_iParamConfigID;

	// ����������ɵĲ�Ʒ�
	DataCenterItemListPtr m_pNewDataCenterItems;
};

END_CUTLEADER_NAMESPACE()

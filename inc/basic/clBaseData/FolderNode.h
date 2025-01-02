#pragma once

#include "DataCenterItemBase.h"
#include "boost/enable_shared_from_this.hpp"


DECLARE_CUTLEADER_CLASS(IntegerList)
DECLARE_CUTLEADER_CLASS(DataCenterItemList)
DECLARE_CUTLEADER_CLASS(FolderNode)
DECLARE_CUTLEADER_CLASS(IDataCenterItem)


BEGIN_CUTLEADER_NAMESPACE()


// ������Ŀ¼������ֿ�Ŀ¼��ID��
#define WorkSpaceFolderID           123
#define PartsLibraryFolderID        234

// ��Ŀ¼�����
// ע��
//  1) ��Ŀ¼���������ݿ���Ӣ�ģ����Լ���ʱҪ�������Դ���һ�¡�
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


	// ����ID����ָ������������Ԫ�ء�
	// notes:
	//   1) this function should be a recursive one.
	IDataCenterItemPtr SearchDataCenterItem(LONGLONG iDataCenterItemID);

	// ����ȫ·������ָ������������Ԫ�ء�
	// notes:
	//   1) this function should be a recursive one.
	IDataCenterItemPtr SearchDataCenterItemByFullPath(CString strFullPath);

	// ɾ���ļ���������ӽڵ㣬���ǵݹ麯����
	void DeleteSubNodeByDataCenterItemID(LONGLONG iDataCenterItemID);

private:
	// the sub-nodes under this folder.
	// notes:
	//   1) this class do not manage the memory.
	DataCenterItemListPtr m_pDataCenterItemList;
};

END_CUTLEADER_NAMESPACE()

#pragma once

#include "DataCenterItemBase.h"

DECLARE_CUTLEADER_CLASS(DataItem)

BEGIN_CUTLEADER_NAMESPACE()

// ��һ�������������������������Ľ������ǲ��ɼ��ģ������ĵ�ʹ�������������ơ�
#define HiddenDataItemName _T("HiddenDataItemName-653427")

// the type of data node.
enum DATANODE_TYPE
{
	DATANODE_PART = 0,
	DATANODE_Task = 1,
};

// �����������
class ClBaseData_Export DataItem : public DataCenterItemBase
{
public:
	DataItem(void);
	DataItem(LONGLONG iDataID, LONGLONG iParentID, DATANODE_TYPE dataType, CString strName, LONGLONG iCreateTime, LONGLONG iModifyTime);
	virtual ~DataItem();

public: // implement interface IDataCenterItem.
	virtual BOOL IsFolder() const override { return FALSE; }

public: // get/set functions.
	void SetDataID(LONGLONG iDataID) { m_iDataID = iDataID; }
	LONGLONG GetDataID() const { return m_iDataID; }

	void SetDataType(DATANODE_TYPE dataType) { m_dataType = dataType; }
	DATANODE_TYPE GetDataType() const { return m_dataType; }

private:
	// the data ID. e.g. the ID of part/sheet/...
	LONGLONG m_iDataID;

	// the type of the data.
	DATANODE_TYPE m_dataType;
};

END_CUTLEADER_NAMESPACE()

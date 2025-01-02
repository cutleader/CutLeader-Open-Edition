#pragma once

#include "clBaseDataCommon.h"

BEGIN_CUTLEADER_NAMESPACE()

// 数据中心树节点类型。
enum DataCenterNodeType
{
	DataCenterNodeType_NotInit		= 0,
	DataCenterNodeType_Part			= 1,
	DataCenterNodeType_Task			= 2,
	DataCenterNodeType_Folder		= 3,
	DataCenterNodeType_Multiple		= 4, // 选中了多个节点
};

// 数据中心树节点。
class ClBaseData_Export DataCenterNode
{
public:
	DataCenterNode();
	DataCenterNode(DataCenterNodeType emNodeType, LONGLONG iDataCenterItemID);
	~DataCenterNode();

public: // get/set functions.
	void SetNodeType(DataCenterNodeType emNodeType) { m_emNodeType = emNodeType; }
	DataCenterNodeType GetNodeType() const { return m_emNodeType; }

	void SetDataCenterItemID(LONGLONG iDataCenterItemID) { m_iDataCenterItemID = iDataCenterItemID; }
	LONGLONG GetDataCenterItemID() const { return m_iDataCenterItemID; }

private:
	// 节点类型
	DataCenterNodeType m_emNodeType;

	// 节点ID
	LONGLONG m_iDataCenterItemID;
};

END_CUTLEADER_NAMESPACE()

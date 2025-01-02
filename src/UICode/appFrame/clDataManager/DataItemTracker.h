#pragma once

#include "clDataManagerCommon.h"

DECLARE_CUTLEADER_CLASS(DataItem)
DECLARE_CUTLEADER_CLASS(DataCenterItemList)
DECLARE_CUTLEADER_CLASS(RecentDataList)

BEGIN_CUTLEADER_NAMESPACE()

// 这个类记录了数据中心的访问历史。
// 注：
// 1) 该类记录了当前被打开的数据项、最近打开过的数据项，以及最后访问的数据中心节点。
class ClDataManager_Export DataItemTracker
{
public:
	~DataItemTracker(void);

private:
	DataItemTracker(void);

public: // get/set functions.
	void SetRecentData(RecentDataListPtr pRecentDataList) { m_pRecentDataList = pRecentDataList; }
	RecentDataListPtr GetRecentData() const { return m_pRecentDataList; }

	LONGLONG GetLastSelNodeID() const { return m_iLastSelNodeID; }
	void SetLastSelNodeID(LONGLONG iLastSelNodeID) { m_iLastSelNodeID = iLastSelNodeID; }

public:
	static DataItemTracker* GetInstance();

public:
	// 该数据项是不是已经被打开了。
	BOOL IsDataItemOpened(LONGLONG iDataID) const;

	// 把一个数据项添加到“已打开”列表。
	void AddDataItem(DataItemPtr pDataItem);

	// 把一个数据项从“已打开”列表删除。
	void RemoveDataItem(LONGLONG iDataID);

private:
	static DataItemTracker* m_pDataItemTracker;

public:
	// 当前被打开的数据项。
	DataCenterItemListPtr m_pDataItemList;

	// 最近打开过的数据项，这个集合会被保存到数据库。
	RecentDataListPtr m_pRecentDataList;

	// 最后访问的数据中心节点，为了下次打开时还能定位到这个节点。
	LONGLONG m_iLastSelNodeID;
};

END_CUTLEADER_NAMESPACE()

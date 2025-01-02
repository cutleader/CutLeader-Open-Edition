#pragma once

#include "clDataManagerCommon.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(RecentData)

BEGIN_CUTLEADER_NAMESPACE()

// the RecentData list.
// notes:
//   1) the latest data will be placed at first.
class ClDataManager_Export RecentDataList : public std::vector<RecentDataPtr>
{
public:
	RecentDataList(void);
	~RecentDataList(void);

public:
	RecentDataPtr GetItemByDataID(LONGLONG iDataID);

	void RemoveItemByID(LONGLONG iID);

	// move the item to the head.
	void MoveToHead(LONGLONG iID);
};

END_CUTLEADER_NAMESPACE()

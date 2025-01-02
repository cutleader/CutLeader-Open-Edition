#include "StdAfx.h"
#include "RecentDataList.h"

#include "RecentData.h"

BEGIN_CUTLEADER_NAMESPACE()

RecentDataList::RecentDataList(void)
{
}

RecentDataList::~RecentDataList(void)
{
}

RecentDataPtr RecentDataList::GetItemByDataID(LONGLONG iDataID)
{
	RecentDataPtr pRecentData;

	for (vector<RecentDataPtr>::iterator iter = begin(); iter != end(); iter++)
	{
		RecentDataPtr pTmpRecentData = (RecentDataPtr)(*iter);
		if (pTmpRecentData->GetDataID() == iDataID)
		{
			pRecentData = pTmpRecentData;
			break;
		}
	}

	return pRecentData;
}

void RecentDataList::RemoveItemByID(LONGLONG iID)
{
	for (vector<RecentDataPtr>::iterator iter = begin(); iter != end(); iter++)
	{
		RecentDataPtr pRecentData = (RecentDataPtr)(*iter);
		if (pRecentData->GetID() == iID)
		{
			erase(iter);
			return;
		}
	}
}

void RecentDataList::MoveToHead(LONGLONG iID)
{
	RecentDataPtr pRecentData;
	for (vector<RecentDataPtr>::iterator iter = begin(); iter != end(); iter++)
	{
		RecentDataPtr pTmpRecentData = (RecentDataPtr)(*iter);
		if (pTmpRecentData->GetID() == iID)
		{
			pRecentData = pTmpRecentData;
			erase(iter);
			break;
		}
	}

	insert(begin(), pRecentData);
}

END_CUTLEADER_NAMESPACE()

#include "StdAfx.h"
#include "DependNodeList.h"

#include "IntegerList.h"

#include "DependNode.h"
#include "DataCenterItemList.h"

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

DependNodeList::DependNodeList(void)
{
}

DependNodeList::~DependNodeList(void)
{
}

DataCenterItemListPtr DependNodeList::GetAllDataItem()
{
	// get all data item, repeat data item maybe exist.
	DataCenterItemListPtr pProductItemList(new DataCenterItemList);
	for (unsigned int i = 0; i < size(); i++)
	{
		DependNodePtr pDependNode = at(i);
		DataCenterItemListPtr pTmpProductItemList = pDependNode->GetAllDataItem();
		pProductItemList->insert(pProductItemList->begin(), pTmpProductItemList->begin(), pTmpProductItemList->end());
	}

	// filter out the same DataItem.
	DataCenterItemListPtr pRetDataItemList(new DataCenterItemList);
	for (unsigned int i = 0; i < pProductItemList->size(); i++)
	{
		IDataCenterItemPtr pTmpProductItem = pProductItemList->at(i);
		if (pRetDataItemList->GetItemByID(pTmpProductItem->GetID()))
			;
		else
			pRetDataItemList->push_back(pTmpProductItem);
	}

	return pRetDataItemList;
}

IntegerListPtr DependNodeList::GetAllDataItemID()
{
	// get all data item, repeat data item maybe exist.
	IntegerListPtr pIntegerList(new IntegerList);
	for (unsigned int i = 0; i < size(); i++)
	{
		DependNodePtr pDependNode = at(i);
		IntegerListPtr pTmpIntegerList = pDependNode->GetAllDataItemID();
		pIntegerList->InsertIntList(pTmpIntegerList);
	}

	// filter out the same DataItem.
	IntegerListPtr pRetIDList(new IntegerList);
	for (unsigned int i = 0; i < pIntegerList->size(); i++)
	{
		LONGLONG iDataItemID = pIntegerList->at(i);
		if (!pRetIDList->Include(iDataItemID))
			pRetIDList->AddInt(iDataItemID);
	}

	return pRetIDList;
}

BOOL DependNodeList::HasDependence()
{
	for (unsigned int i = 0; i < size(); i++)
	{
		DependNodePtr pDependNode = at(i);
		if (pDependNode->HasDependence())
		{
			return TRUE;
		}
	}

	return FALSE;
}

END_CUTLEADER_NAMESPACE()

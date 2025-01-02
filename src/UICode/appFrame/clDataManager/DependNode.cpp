#include "StdAfx.h"
#include "DependNode.h"

#include "IntegerList.h"

#include "DataItem.h"
#include "DataCenterItemBase.h"
#include "DataCenterItemList.h"
#include "DependNodeList.h"

BEGIN_CUTLEADER_NAMESPACE()

DependNode::DependNode(void)
{
	m_pDependNodeList.reset(new DependNodeList);
}

DependNode::~DependNode(void)
{

}

IntegerListPtr DependNode::GetAllDataItemID() const
{
	IntegerListPtr pIntegerList(new IntegerList);
	pIntegerList->AddInt(m_pDataItem->GetID());

	for (unsigned int i = 0; i < m_pDependNodeList->size(); i++)
	{
		DependNodePtr pDependNode = m_pDependNodeList->at(i);
		IntegerListPtr pTmpIntegerList = pDependNode->GetAllDataItemID();
		pIntegerList->InsertIntList(pTmpIntegerList);
	}

	return pIntegerList;
}

DataCenterItemListPtr DependNode::GetAllDataItem() const
{
	DataCenterItemListPtr pDataCenterItemList(new DataCenterItemList);
	pDataCenterItemList->push_back(m_pDataItem);

	for (unsigned int i = 0; i < m_pDependNodeList->size(); i++)
	{
		DependNodePtr pDependNode = m_pDependNodeList->at(i);
		DataCenterItemListPtr pTmpDataCenterItemList = pDependNode->GetAllDataItem();

		//
		pDataCenterItemList->insert(pDataCenterItemList->end(), pTmpDataCenterItemList->begin(), pTmpDataCenterItemList->end());
	}

	return pDataCenterItemList;
}

BOOL DependNode::HasDependence() const
{
	if (m_pDependNodeList->size() > 0)
		return TRUE;

	return FALSE;
}

END_CUTLEADER_NAMESPACE()

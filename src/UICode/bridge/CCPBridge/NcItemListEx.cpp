#include "stdafx.h"
#include "NcItemListEx.h"

#include "LineArc2DList.h"
#include "LineItemEx.h"
#include "ArcItemEx.h"

BEGIN_CUTLEADER_NAMESPACE()

NcItemListEx::NcItemListEx(void)
{
	m_ppNcItemList = new NcItemListPtr();
	m_ppNcItemList->reset(new NcItemList());
}

NcItemListEx::NcItemListEx(NcItemListPtr* ppNcItemList)
{
	m_ppNcItemList = new NcItemListPtr;
	*m_ppNcItemList = *ppNcItemList;
}

NcItemListEx::~NcItemListEx()
{
	this->!NcItemListEx();
}

NcItemListEx::!NcItemListEx()
{
	if (m_ppNcItemList)
		delete m_ppNcItemList;
}

int NcItemListEx::Count()
{
	return (*m_ppNcItemList)->size();
}

NcItemEx^ NcItemListEx::GetItemByIndex(int iIndex)
{
	NcItemPtr pNcItem = (*m_ppNcItemList)->at(iIndex);
	return gcnew NcItemEx(&pNcItem);
}

void NcItemListEx::AddItem(NcItemEx^ ncItem)
{
	(*m_ppNcItemList)->push_back(ncItem->GetNcItem());
}

void NcItemListEx::RemoveItem(int iIndex)
{
	(*m_ppNcItemList)->erase((*m_ppNcItemList)->begin()+iIndex);
}

void NcItemListEx::Clear()
{
	(*m_ppNcItemList)->clear();
}

END_CUTLEADER_NAMESPACE()

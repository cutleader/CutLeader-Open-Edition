#pragma once

#include "ccpBridgeCommon.h"
#include "NcItemList.h"
#include "NcItemEx.h"

BEGIN_CUTLEADER_NAMESPACE()

[ClassInterface(ClassInterfaceType::AutoDual)]
[ComVisible(true)]
public ref class NcItemListEx
{
public:
	NcItemListEx(void);
	NcItemListEx(NcItemListPtr* ppNcItemList);
	~NcItemListEx();
	!NcItemListEx();

public:
	int Count();
	NcItemEx^ GetItemByIndex(int iIndex);
	void AddItem(NcItemEx^ ncItem);
	void RemoveItem(int iIndex);
	void Clear();

public:
	NcItemListPtr GetNcItemList() { return *m_ppNcItemList; }

private:
	NcItemListPtr* m_ppNcItemList;
};

END_CUTLEADER_NAMESPACE()
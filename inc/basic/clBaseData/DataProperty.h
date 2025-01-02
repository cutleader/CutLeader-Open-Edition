#pragma once

#include "clBaseDataCommon.h"
#include "DataPropItem.h"
#include <vector>

BEGIN_CUTLEADER_NAMESPACE()

class ClBaseData_Export DataProperty
{
public:
	DataProperty(void);
	~DataProperty(void);

public:
	/************************************************************************/
	// ���ڵõ����������

	const DataPropItem& GetItem(int index) const { return m_propItems[index]; }

	// get the property items by the category type of the property item.
	void GetItemsByType(int iCategoryType, std::vector<DataPropItem>& propItems) const;

	// get the property item by item name.
	DataPropItem GetItemByName(int iItemName) const;
	/************************************************************************/


	int Count() const { return m_propItems.size(); }
	void AddProperty(const DataPropItem& propItem) { m_propItems.push_back(propItem); }

	// ���ǵ�ָ���ġ����������
	void SetItem(int iIndex, const DataPropItem& propItem);

private:
	// the property items.
	std::vector<DataPropItem> m_propItems;
};

END_CUTLEADER_NAMESPACE()

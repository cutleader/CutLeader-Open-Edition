#pragma once

#include "clReportTemplateCommon.h"
#include "BindItem.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(BindItemList)
DECLARE_CUTLEADER_CLASS(BindItem)

BEGIN_CUTLEADER_NAMESPACE()

class ClReportTemplate_Export BindItemList : public std::vector<BindItemPtr>
{
public:
	BindItemList(void);
	~BindItemList(void);

public:
	// get the bind items by the control type.
	BindItemListPtr GetItemsByCtrlType(BIND_ITEM_CTRL_TYPE bindItemCtrlType);

	// get the bind items by the type.
	BindItemPtr GetItemByType(BIND_ITEM_TYPE bindItemType);
};

END_CUTLEADER_NAMESPACE()

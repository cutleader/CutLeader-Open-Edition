#pragma once

#include "ParamConfigItem.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(ParamConfigItem)

BEGIN_CUTLEADER_NAMESPACE()

class ClExpertLib_Export ParamConfigItemList : public std::vector<ParamConfigItemPtr>
{
public:
	ParamConfigItemList(void);
	virtual ~ParamConfigItemList(void);

public:
	ParamConfigItemPtr GetItemByType(PARAMCONFIG_ITEM_TYPE type) const;
};

END_CUTLEADER_NAMESPACE()

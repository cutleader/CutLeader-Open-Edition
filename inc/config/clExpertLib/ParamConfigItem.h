#pragma once

#include "clExpertLibCommon.h"
#include "IData.h"

BEGIN_CUTLEADER_NAMESPACE()

enum PARAMCONFIG_ITEM_TYPE
{
	// the micro joint assignment rules.
	PARAMCONFIG_ITEM_CUT_MJ_ASSIGN		= 10,

	// the rules to assign the corner.
	PARAMCONFIG_ITEM_CORNER_ASSIGN		= 12,

	// the rules to assign lead in/out.
	PARAMCONFIG_ITEM_LEAD_ASSIGN		= 13,

	// the rules to assign pierce.
	PARAMCONFIG_ITEM_PIERCE_ASSIGN		= 14,

    // 刀具分配规则
    PARAMCONFIG_ITEM_TOOL_ASSIGN		= 15,

	// the rules to sequence.
	PARAMCONFIG_ITEM_GENERATE_SEQ		= 21,

	// the rules to generate NC code.
	PARAMCONFIG_ITEM_GENERATE_NC		= 22,

	// the nest config.
	PARAMCONFIG_ITEM_NEST				= 31,
};

// “参数配置项”接口。
// 注：
//   1) set the ParamConfig ID as parent ID.
class ClExpertLib_Export ParamConfigItem : virtual public IData
{
public:
	virtual PARAMCONFIG_ITEM_TYPE GetParamConfigItemType() const = 0;
};

END_CUTLEADER_NAMESPACE()

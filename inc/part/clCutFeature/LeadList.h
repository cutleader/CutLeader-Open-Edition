#pragma once

#include "clExpertLibCommon.h"
#include "clCutFeatureCommon.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(ILead)

BEGIN_CUTLEADER_NAMESPACE()

class ClCutFeature_Export LeadList : public std::vector<ILeadPtr>
{
public:
	LeadList(void);
	~LeadList(void);

public:
	ILeadPtr GetItemByID(LONGLONG iID);

	// 检查所有的引入/引出类型是否都是iLeadType.
	BOOL CheckLeadType(LeadType iLeadType);
};

END_CUTLEADER_NAMESPACE()

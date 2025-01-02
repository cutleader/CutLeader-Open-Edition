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

	// ������е�����/���������Ƿ���iLeadType.
	BOOL CheckLeadType(LeadType iLeadType);
};

END_CUTLEADER_NAMESPACE()

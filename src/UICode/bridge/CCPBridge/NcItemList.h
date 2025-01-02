#pragma once

#include "clGeometryBaseCommon.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(NcItem)
DECLARE_CUTLEADER_CLASS(NcItemList)

BEGIN_CUTLEADER_NAMESPACE()

class NcItemList : public std::vector<NcItemPtr>
{
public:
	NcItemList(void);
	~NcItemList(void);
};

END_CUTLEADER_NAMESPACE()

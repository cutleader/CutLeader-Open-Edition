#pragma once

#include "clMaterialLibCommon.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(IMaterialLibNode)

BEGIN_CUTLEADER_NAMESPACE()

class ClMaterialLib_Export MaterialLibNodeList : public std::vector<IMaterialLibNodePtr>
{
public:
	MaterialLibNodeList(void);
	virtual ~MaterialLibNodeList(void);
};

END_CUTLEADER_NAMESPACE()

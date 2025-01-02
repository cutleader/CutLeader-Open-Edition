#pragma once

#include "clUtilityCommon.h"
#include "libxl.h"
using namespace libxl;

BEGIN_CUTLEADER_NAMESPACE()


class ClUtility_Export LibXlHelper
{
public:
    static void Init(Book* pBook);
};

END_CUTLEADER_NAMESPACE()

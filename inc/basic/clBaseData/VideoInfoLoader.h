#pragma once

#include "clBaseDataCommon.h"
#include "clBaseDataCommon.h"
#include <vector>
#include "MultiLanguageMgr.h"

DECLARE_CUTLEADER_CLASS(HelpVideoCategory)

BEGIN_CUTLEADER_NAMESPACE()

class ClBaseData_Export VideoInfoLoader
{
public:
	static std::vector<HelpVideoCategoryPtr> LoadHelpVideo(LANG_TYPE langType);
};

END_CUTLEADER_NAMESPACE()

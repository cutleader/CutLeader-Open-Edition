#pragma once

#include "clGeometryFeatureCommon.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(DBConnect)
DECLARE_CUTLEADER_CLASS(PatternList)
DECLARE_CUTLEADER_CLASS(PatternLoopList)
DECLARE_CUTLEADER_CLASS(TextStructList)

BEGIN_CUTLEADER_NAMESPACE()

class ClGeometryFeature_Export TextStructLoader
{
public:
	// 加载文字对象。
	static TextStructListPtr LoadTextStructs(DBConnectPtr pDBConnect, const PatternLoopList* pPatternLoops);
};

END_CUTLEADER_NAMESPACE()

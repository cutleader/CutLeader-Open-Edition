#pragma once

#include "clPartLayoutCommon.h"
#include <vector>

// declare class.
DECLARE_CUTLEADER_CLASS(NestPolygonInfo)
//

BEGIN_CUTLEADER_NAMESPACE()

// 已排多边形的信息。
class ClPartLayout_Export NestPolygonInfoList : public std::vector<NestPolygonInfoPtr>
{
public:
	NestPolygonInfoList();
	~NestPolygonInfoList(void);
};

END_CUTLEADER_NAMESPACE()
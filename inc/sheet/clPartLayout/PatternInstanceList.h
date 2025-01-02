#pragma once

#include "clPartLayoutCommon.h"
#include "Point2D.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(PatternInstance)

BEGIN_CUTLEADER_NAMESPACE()

class ClPartLayout_Export PatternInstanceList : public std::vector<PatternInstancePtr>
{
public:
	PatternInstanceList(void);
	~PatternInstanceList(void);

public:
	// 得到距离“pt”最近的图形实例。
	PatternInstancePtr GetClosestPatInst(const Point2D& pt);
};

END_CUTLEADER_NAMESPACE()

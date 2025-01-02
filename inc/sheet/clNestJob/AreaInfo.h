#pragma once

#include "clNestJobCommon.h"
#include <map>

DECLARE_CUTLEADER_CLASS(Polygon2D)

BEGIN_CUTLEADER_NAMESPACE()

// 记录零件的面积数据。
class AreaInfo
{
public:
	AreaInfo() { dAllPartArea = .0; }

	// 缓存零件的面积<part id, part area>。这里的面积是用同样的精度计算的。
	std::map<__int64, double> partAreaData;

	// 缓存零件的外边框<part id, part outer poly>。这里的外边框是用同样的精度计算的。
	std::map<__int64, Polygon2DPtr> partOutPolyData;

	// 总面积。
	double dAllPartArea;
};

END_CUTLEADER_NAMESPACE()
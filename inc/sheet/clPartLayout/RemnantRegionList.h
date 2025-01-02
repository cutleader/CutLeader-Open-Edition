#pragma once

#include "clPartLayoutCommon.h"
#include "Rect2D.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(RemnantRegion)
DECLARE_CUTLEADER_CLASS(Polygon2DList)
DECLARE_CUTLEADER_CLASS(RemnantRegionList)

BEGIN_CUTLEADER_NAMESPACE()

// 余料区域集合。
class ClPartLayout_Export RemnantRegionList : public std::vector<RemnantRegionPtr>
{
public:
	RemnantRegionList(void);
	~RemnantRegionList(void);

public:
	// 得到包含“多边形集合”的最小“余料区域”。
	/* 参数：
	*  polysRect: 多边形集合的外接矩形。
	*/
	// 注：
	//  1) 如果没有就返回空。
	//  2) 找到的“余料区域”并不是严格包含“pPolys”，因为判断严格包含需要花费不少时间，所以这里用外接矩形来判断包含。
	//     对找到的“余料区域”还有一个限制，就是从“多边形集合”挑一个点，这个点不能在“余料区域”外面。所以这样得到的
	//     “余料区域”即使不能严格包含这些多边形，也不至于这些多边形都在“余料区域”外面。
	RemnantRegionPtr GetContainer(const Polygon2DList* pPolys, const Rect2D& polysRect) const;
};

END_CUTLEADER_NAMESPACE()

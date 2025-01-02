#pragma once

#include "clGeometryFeatureCommon.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(LoopTopologyItem)
DECLARE_CUTLEADER_CLASS(IPatternLoop)
DECLARE_CUTLEADER_CLASS(PatternLoopList)

BEGIN_CUTLEADER_NAMESPACE()

// “图形回路”拓扑项集合。
class ClGeometryFeature_Export LoopTopologyItemList : public std::vector<LoopTopologyItemPtr>
{
public:
	LoopTopologyItemList(void);
	~LoopTopologyItemList(void);

public:
	// 删除拓扑项。
	// 注：
	//   1) 不是递归。
	void RemoveTopItem(LoopTopologyItemPtr pLoopTopItem);

	// 某个“拓扑项”是否存在。
	BOOL TopItemExist(LoopTopologyItemPtr pLoopTopItem);

	// 这个拓扑关系中是不是有这个几何轮廓。
	bool IncludePatternLoop(const IPatternLoop* pPatternLoop) const;

	// 根据几何轮廓ID得到拓扑项。
	const LoopTopologyItem* FindTopologyItemByPatternLoop(long long iPatternLoopID) const;

	// 得到轮廓拓扑层数，这个函数不是递归函数，不过每个节点的CalcLayerNum函数是递归函数。
	int CalcLayerNum() const;

	// 得到几何轮廓在哪一层，外轮廓是第0层，这里的层数是从0开始的，如果找不到就要报错并返回-1。
	int GetLayerIndexOfPatternLoop(long long iPatternLoopID) const;


	/************************************************************************/
	// 关于外轮廓。
	// 注：
	// 1）调用这些接口时要注意LoopTopologyItemList必须是PartCadData下面的那个数据。

	// 判断这个几何轮廓是不是零件的外轮廓，零件的外轮廓必须是闭合的，且包含其他所有轮廓。
	bool IsBoundaryLoop(const IPatternLoop* pPatternLoop) const;

	// 得到零件的外轮廓。
	int GetBoundaryLoop(const PatternLoopList* pAllPatternLoops) const;
	IPatternLoopPtr GetBoundaryLoopEx() const;
	/************************************************************************/
};

END_CUTLEADER_NAMESPACE()

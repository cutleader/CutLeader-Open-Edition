#pragma once

#include "clGeometryFeatureCommon.h"
#include "ClData.h"

DECLARE_CUTLEADER_CLASS(LoopTopologyItemList)
DECLARE_CUTLEADER_CLASS(IPatternLoop)
DECLARE_CUTLEADER_CLASS(PatternLoopList)
DECLARE_CUTLEADER_CLASS(LoopTopologyItem)

BEGIN_CUTLEADER_NAMESPACE()

// “图形回路”拓扑项。
class ClGeometryFeature_Export LoopTopologyItem : public ClData
{
public:
	LoopTopologyItem(IPatternLoopPtr pPatternLoop);
	~LoopTopologyItem(void);

public: // get/set functions.
	void SetPatternLoop(IPatternLoopPtr pPatternLoop) { m_pPatternLoop = pPatternLoop;}
	IPatternLoopPtr GetPatternLoop() const { return m_pPatternLoop; }
	const IPatternLoop* GetPatternLoop_FR() const { return m_pPatternLoop.get(); }

	void SetSubItemList(LoopTopologyItemListPtr pSubItemList) { m_pSubItemList = pSubItemList; }
	LoopTopologyItemListPtr GetSubItemList() const { return m_pSubItemList; }

public:
	// 递归得到轮廓拓扑中的所有几何轮廓。
	void GetAllPatternLoops(PatternLoopListPtr pAllPatternLoops) const;

	// 递归得到这个拓扑节点的最大层数，如果这个轮廓没有内部轮廓，那么层数为1。
	int CalcLayerNum() const;

	// 递归得到几何轮廓在哪一层，如果iPatternLoopID就是当前轮廓，那么就是在第0层，如果找不到这个几何轮廓就返回-1。
	int GetLayerIndexOfPatternLoop(long long iPatternLoopID) const;

	// 递归克隆这个拓扑结构，集合轮廓是外面克隆好传入的。
	// 注：
	// 1) m_pPartPlacements中的零件在pPartPlacements_old中不一定都能找到，所以克隆出的零件组中含的零件数量可能比原零件组中少。
	LoopTopologyItemPtr CloneRecursively(const PatternLoopList* pPatternLoops_old, const PatternLoopList* pPatternLoops_new) const;

private:
	// 当前图形回路。
	IPatternLoopPtr m_pPatternLoop;

	// 该回路包含的“拓扑项”。
	LoopTopologyItemListPtr m_pSubItemList;
};

END_CUTLEADER_NAMESPACE()

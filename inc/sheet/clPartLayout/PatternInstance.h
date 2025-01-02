#pragma once

#include "clPartLayoutCommon.h"
#include "Point2D.h"
#include "ClData.h"

DECLARE_CUTLEADER_CLASS(PatternLoopNode)
DECLARE_CUTLEADER_CLASS(PatternInstanceCache)
DECLARE_CUTLEADER_CLASS(LoopInstance)

BEGIN_CUTLEADER_NAMESPACE()

// 图形实例。
// 注：
//   1) 这个对象没有被存入数据库。
class ClPartLayout_Export PatternInstance : public ClData
{
public:
	PatternInstance(void);
	PatternInstance(LoopInstancePtr pLoopInstance, PatternLoopNodePtr pPatternLoopNode);
	~PatternInstance(void);

public: // get/set functions
	void SetLoopInst(LoopInstancePtr pLoopInstance) { m_pLoopInstance = pLoopInstance; }
	LoopInstancePtr GetLoopInstance() const { return m_pLoopInstance; }

	void SetPatternLoopNode(PatternLoopNodePtr pPatternLoopNode) { m_pPatternLoopNode = pPatternLoopNode; }
	PatternLoopNodePtr GetPatternLoopNode() const { return m_pPatternLoopNode; }

public:
	// 关于缓存数据。
	PatternInstanceCachePtr GetCacheData() const { return m_pPatternInstanceCache; }
	void UpdateCache();

	// 这个图形实例到点的距离。
	double DistanceTo(const Point2D& pt) const;

private:
	// 图形实例所在的回路实例。
	LoopInstancePtr m_pLoopInstance;

	// “回路节点”对象。
    // 注：这个节点不一定是几何轮廓中的那个节点，轮廓方向和切割方向不同时，这个节点是克隆出来的。不过这个节点中的数据和几何轮廓中的是一样的。
	PatternLoopNodePtr m_pPatternLoopNode;

	// “图形实例”的缓存数据。
	PatternInstanceCachePtr m_pPatternInstanceCache;
};

END_CUTLEADER_NAMESPACE()

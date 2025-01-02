#pragma once

#include "clGeometryFeatureCommon.h"
#include "Point2D.h"

BEGIN_CUTLEADER_NAMESPACE()

// 记录轮廓上面的点。
// 注：
//  1) 目前有的地方没有用这个结构。
class ClGeometryFeature_Export LoopPt
{
public:
	LoopPt(void);
	LoopPt(const Point2D& loopPt, int iNodeIndex);
	~LoopPt(void);

public: // get/set functions
	void SetLoopPt(const Point2D& pt) { m_loopPt = pt; }
	Point2D GetLoopPt() const { return m_loopPt; }

	void SetNodeIndex(int iIndex) { m_iNodeIndex = iIndex; }
	int GetNodeIndex() const { return m_iNodeIndex; }

private:
	// 轮廓上面的点。
	Point2D m_loopPt;

	// 点所在的回路节点。
	int m_iNodeIndex;
};

END_CUTLEADER_NAMESPACE()
